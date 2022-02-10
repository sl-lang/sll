#include <sll/_sll_internal.h>
#include <sll/api/string.h>
#include <sll/array.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/ift.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/platform.h>
#include <sll/sandbox.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <sll/vm.h>



#define READ_THREAD_DATA \
	do{ \
		ii=(_vm_thr+_vm_thr_idx)->ii; \
		si=(_vm_thr+_vm_thr_idx)->si; \
	} while (0)
#define READ_THREAD_DATA_INIT \
	do{ \
		stack=(_vm_thr+_vm_thr_idx)->s; \
		READ_THREAD_DATA; \
	} while (0)
#define WRITE_THREAD_DATA \
	do{ \
		(_vm_thr+_vm_thr_idx)->ii=ii; \
		(_vm_thr+_vm_thr_idx)->si=si; \
	} while (0)

#define JUMP_INSTRUCTION \
	ii=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?ii+ai->dt.rj:ai->dt.j); \
	ai=_get_instruction_at_offset(sll_current_runtime_data->a_dt,ii); \
	continue;
#define CMP_INSTRUCTION(eq,v) \
	{ \
		WRITE_THREAD_DATA; \
		sll_compare_result_t cmp=sll_operator_compare(*(stack+si-2),*(stack+si-1)); \
		READ_THREAD_DATA; \
		if (cmp eq##=v){ \
			si-=2; \
			SLL_RELEASE(*(stack+si)); \
			SLL_RELEASE(*(stack+si+1)); \
			JUMP_INSTRUCTION; \
		} \
		break; \
	}
#define CMP_INSTRUCTION_EQ(eq) \
	{ \
		WRITE_THREAD_DATA; \
		sll_bool_t st=eq(*(stack+si-2),*(stack+si-1)); \
		READ_THREAD_DATA; \
		if (st){ \
			si-=2; \
			SLL_RELEASE(*(stack+si)); \
			SLL_RELEASE(*(stack+si+1)); \
			JUMP_INSTRUCTION; \
		} \
		break; \
	}
#define CMP_INSTRUCTION_BOOL(st) \
	{ \
		WRITE_THREAD_DATA; \
		sll_bool_t b=sll_operator_bool(*(stack+si-1)); \
		READ_THREAD_DATA; \
		if (b==st){ \
			si--; \
			SLL_RELEASE(*(stack+si)); \
			JUMP_INSTRUCTION; \
		} \
		break; \
	}

#define OPERATOR_INSTRUCTION_UNARY(nm) \
	{ \
		sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?_vm_var_data+ai->dt.v:stack+si-1); \
		WRITE_THREAD_DATA; \
		sll_object_t* n=nm(*tos); \
		READ_THREAD_DATA; \
		SLL_RELEASE(*tos); \
		*tos=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION_BINARY(nm) \
	{ \
		si--; \
		sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?_vm_var_data+ai->dt.v:stack+si-1); \
		WRITE_THREAD_DATA; \
		sll_object_t* n=nm(*tos,*(stack+si)); \
		READ_THREAD_DATA; \
		SLL_RELEASE(*tos); \
		SLL_RELEASE(*(stack+si)); \
		*tos=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION_TERNARY(nm) \
	{ \
		si-=2; \
		sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?_vm_var_data+ai->dt.v:stack+si-1); \
		WRITE_THREAD_DATA; \
		sll_object_t* n=nm(*tos,*(stack+si),*(stack+si+1)); \
		READ_THREAD_DATA; \
		SLL_RELEASE(*tos); \
		SLL_RELEASE(*(stack+si)); \
		SLL_RELEASE(*(stack+si+1)); \
		*tos=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION_QUATERNARY(nm) \
	{ \
		si-=3; \
		sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?_vm_var_data+ai->dt.v:stack+si-1); \
		WRITE_THREAD_DATA; \
		sll_object_t* n=nm(*tos,*(stack+si),*(stack+si+1),*(stack+si+2)); \
		READ_THREAD_DATA; \
		SLL_RELEASE(*tos); \
		SLL_RELEASE(*(stack+si)); \
		SLL_RELEASE(*(stack+si+1)); \
		SLL_RELEASE(*(stack+si+2)); \
		*tos=n; \
		break; \
	}



static sll_object_t** _vm_var_data=NULL;
static thread_data_t* _vm_thr=NULL;
static sll_thread_index_t _vm_thr_idx=0;
static sll_thread_index_t _vm_thr_count=0;



__SLL_EXTERNAL sll_instruction_index_t sll_current_instruction_count=0;
__SLL_EXTERNAL sll_instruction_index_t sll_current_instruction_index=0;
__SLL_EXTERNAL const sll_runtime_data_t* sll_current_runtime_data=NULL;
__SLL_EXTERNAL const sll_vm_config_t* sll_current_vm_config=NULL;



static sll_object_t* _call_internal_function(sll_function_index_t i,sll_object_t*const* a,sll_arg_count_t ac){
	_push_call_stack((*(sll_current_runtime_data->ift->dt+i))->nm.v,(_vm_thr+_vm_thr_idx)->si);
	sll_object_t* o=(*(sll_current_runtime_data->ift->dt+i))->p(a,ac);
	_pop_call_stack();
	return o;
}



static void _call_function(sll_function_index_t fn,sll_arg_count_t ac){
	sll_assembly_function_t* af=sll_current_runtime_data->a_dt->ft.dt+fn;
	sll_object_t** stack=(_vm_thr+_vm_thr_idx)->s;
	sll_stack_offset_t si=(_vm_thr+_vm_thr_idx)->si;
	if (SLL_ASSEMBLY_FUNCTION_IS_VAR_ARG(af)){
		sll_object_t* tos=SLL_CREATE();
		tos->t=SLL_OBJECT_TYPE_ARRAY;
		if (SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)>ac){
			sll_static_int[0]->rc+=SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)-ac-1;
			for (sll_arg_count_t i=0;i<SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)-ac-1;i++){
				*(stack+si)=sll_static_int[0];
				si++;
			}
			SLL_INIT_ARRAY(&(tos->dt.a));
		}
		else{
			sll_array_create(ac-SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)+1,&(tos->dt.a));
			si-=tos->dt.a.l;
			for (sll_array_length_t i=0;i<tos->dt.a.l;i++){
				tos->dt.a.v[i]=*(stack+si+i);
			}
		}
		*(stack+si)=tos;
		si++;
	}
	else{
		if (ac!=SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)){
			if (SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)>ac){
				sll_static_int[0]->rc+=SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)-ac;
				for (sll_arg_count_t i=0;i<SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)-ac;i++){
					*(stack+si+i)=sll_static_int[0];
				}
				si+=SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)-ac;
			}
			else{
				for (sll_arg_count_t i=ac-SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af);i;i--){
					si--;
					SLL_RELEASE(*(stack+si));
				}
			}
		}
	}
	_push_call_stack((sll_current_runtime_data->a_dt->st.dt+af->nm)->v,si-SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af));
	(_vm_thr+_vm_thr_idx)->ii=af->i;
	(_vm_thr+_vm_thr_idx)->si=si;
}



void _push_call_stack(const sll_char_t* nm,sll_stack_offset_t si){
	SLL_ASSERT(sll_current_runtime_data->c_st.l<=sll_current_vm_config->c_st_sz);
	(sll_current_runtime_data->c_st.dt+sll_current_runtime_data->c_st.l)->nm=nm;
	(sll_current_runtime_data->c_st.dt+sll_current_runtime_data->c_st.l)->_ii=(_vm_thr+_vm_thr_idx)->ii;
	(sll_current_runtime_data->c_st.dt+sll_current_runtime_data->c_st.l)->_s=(si==SLL_MAX_STACK_OFFSET?(_vm_thr+_vm_thr_idx)->si:si);
	(*((sll_call_stack_size_t*)(&(sll_current_runtime_data->c_st.l))))++;
}



void _pop_call_stack(void){
	(*((sll_call_stack_size_t*)(&(sll_current_runtime_data->c_st.l))))--;
	(_vm_thr+_vm_thr_idx)->ii=(sll_current_runtime_data->c_st.dt+sll_current_runtime_data->c_st.l)->_ii;
	(_vm_thr+_vm_thr_idx)->si=(sll_current_runtime_data->c_st.dt+sll_current_runtime_data->c_st.l)->_s;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_code_t sll_execute_assembly(const sll_assembly_data_t* a_dt,const sll_vm_config_t* cfg){
	if (_vm_var_data){
		SLL_UNIMPLEMENTED();
	}
	sll_current_instruction_count=0;
	sll_current_vm_config=cfg;// lgtm [cpp/stack-address-escape]
	sll_size_t ptr_sz=SLL_ROUND_PAGE(a_dt->vc*sizeof(sll_object_t*)+cfg->c_st_sz*sizeof(sll_call_stack_frame_t));
	addr_t ptr=ADDR(sll_platform_allocate_page(ptr_sz,0));
	_vm_var_data=(sll_object_t**)ptr;
	sll_static_int[0]->rc+=a_dt->vc;
	for (sll_variable_index_t i=0;i<a_dt->vc;i++){
		*(_vm_var_data+i)=sll_static_int[0];
	}
	ptr+=a_dt->vc*sizeof(sll_object_t*);
	sll_internal_function_table_t ift;
	sll_clone_internal_function_table(cfg->ift,&ift);
	sll_object_type_table_t tt=SLL_INIT_OBJECT_TYPE_TABLE_STRUCT;
	sll_runtime_data_t r_dt={
		a_dt,
		&ift,
		&tt,
		{
			(sll_call_stack_frame_t*)ptr,
			0
		}
	};
	sll_current_runtime_data=&r_dt;// lgtm [cpp/stack-address-escape]
	ptr+=cfg->c_st_sz*sizeof(sll_call_stack_frame_t);
	_vm_thr=sll_allocate(sizeof(thread_data_t));
	_vm_thr->s=sll_platform_allocate_page(SLL_ROUND_PAGE(cfg->s_sz),0);
	_vm_thr->idx=0;
	_vm_thr->ii=0;
	_vm_thr->si=0;
	_vm_thr->tm=0;
	_vm_thr_idx=0;
	_vm_thr_count=1;
	sll_object_t* o=sll_execute_function(0,NULL,0);
	sll_object_t* rc_o=sll_operator_cast(o,sll_static_int[SLL_OBJECT_TYPE_INT]);
	SLL_RELEASE(o);
	sll_return_code_t rc=(sll_return_code_t)(rc_o->dt.i);
	SLL_RELEASE(rc_o);
	sll_current_runtime_data=NULL;
	for (sll_variable_index_t i=0;i<a_dt->vc;i++){
		SLL_RELEASE(*(_vm_var_data+i));
	}
	while (_vm_thr_count){
		_vm_thr_count--;
		sll_platform_free_page((_vm_thr+_vm_thr_count)->s,SLL_ROUND_PAGE(cfg->s_sz));
	}
	sll_deallocate(_vm_thr);
	sll_platform_free_page(PTR(ptr-a_dt->vc*sizeof(sll_object_t*)-a_dt->st.l*sizeof(sll_object_t)-cfg->c_st_sz*sizeof(sll_call_stack_frame_t)),ptr_sz);
	sll_free_internal_function_table(&ift);
	sll_free_object_type_list(&tt);
	_vm_var_data=NULL;
	return rc;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_execute_function(sll_integer_t fn_idx,sll_object_t*const* al,sll_arg_count_t all){
	if (!sll_current_runtime_data||!sll_current_vm_config){
		return NULL;
	}
	sll_object_t** stack=NULL;
	sll_instruction_index_t ii=0;
	sll_stack_offset_t si=0;
	const sll_assembly_instruction_t* ai=NULL;
	READ_THREAD_DATA_INIT;
	if (sll_current_runtime_data->c_st.l){
		if (fn_idx<0){
			sll_function_index_t i=(sll_function_index_t)(~fn_idx);
			if (i<sll_current_runtime_data->ift->l){
				return _call_internal_function(i,al,all);
			}
			return SLL_ACQUIRE_STATIC_INT(0);
		}
		else if (fn_idx&&fn_idx<=sll_current_runtime_data->a_dt->ft.l){
			for (sll_arg_count_t i=0;i<all;i++){
				*(stack+si+i)=*(al+i);
				SLL_ACQUIRE(*(al+i));
			}
			si+=all;
			WRITE_THREAD_DATA;
			_call_function((sll_function_index_t)(fn_idx-1),all);
			READ_THREAD_DATA;
			ai=_get_instruction_at_offset(sll_current_runtime_data->a_dt,ii);
		}
		else{
			return SLL_ACQUIRE_STATIC_INT(0);
		}
	}
	else{
		ai=sll_current_runtime_data->a_dt->h;
	}
	sll_call_stack_size_t o_c_st=sll_current_runtime_data->c_st.l;
	sll_bool_t io=!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)||sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO);
	while (1){
		if (ii>=sll_current_runtime_data->a_dt->ic||si>=sll_current_vm_config->s_sz){
			return SLL_ACQUIRE_STATIC_INT(0);
		}
		sll_current_instruction_count++;
		sll_current_instruction_index=ii;
		switch (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)){
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_POP:
				si--;
				SLL_RELEASE(*(stack+si));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO:
				si-=2;
				SLL_RELEASE(*(stack+si));
				SLL_RELEASE(*(stack+si+1));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT:
				{
					sll_object_t* tos=*(stack+si-1);
					*(stack+si-1)=*(stack+si-2);
					*(stack+si-2)=tos;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP:
				si--;
				SLL_RELEASE(*(stack+si));
				*(stack+si-1)=*(stack+si);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DUP:
				*(stack+si)=*(stack+si-1);
				SLL_ACQUIRE(*(stack+si));
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
				{
					sll_object_t* tos=SLL_CREATE();
					*(stack+si)=tos;
					tos->t=SLL_OBJECT_TYPE_INT;
					tos->dt.i=ai->dt.i;
					si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE:
				*(stack+si)=SLL_ACQUIRE_STATIC_INT(-1);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO:
				*(stack+si)=SLL_ACQUIRE_STATIC_INT(0);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE:
				*(stack+si)=SLL_ACQUIRE_STATIC_INT(1);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO:
				*(stack+si)=SLL_ACQUIRE_STATIC_INT(2);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE:
				*(stack+si)=SLL_ACQUIRE_STATIC_INT(3);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR:
				*(stack+si)=SLL_ACQUIRE_STATIC_INT(4);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
				{
					sll_object_t* tos=SLL_CREATE();
					*(stack+si)=tos;
					tos->t=SLL_OBJECT_TYPE_FLOAT;
					tos->dt.f=ai->dt.f;
					si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
				*(stack+si)=SLL_ACQUIRE_STATIC_CHAR(ai->dt.c);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL:
				*(stack+si)=SLL_FROM_INT((SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?ii+ai->dt.rj:ai->dt.j));
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD:
				*(stack+si)=*(_vm_var_data+ai->dt.v);
				SLL_ACQUIRE(*(stack+si));
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
				*(stack+si)=SLL_CREATE();
				(*(stack+si))->t=SLL_OBJECT_TYPE_STRING;
				sll_string_clone(sll_current_runtime_data->a_dt->st.dt+ai->dt.s,&((*(stack+si))->dt.s));
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK:
				{
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_ARRAY;
					sll_array_t* a=&(tos->dt.a);
					sll_array_create(ai->dt.al,a);
					si-=ai->dt.al;
					for (sll_array_length_t i=0;i<ai->dt.al;i++){
						a->v[i]=*(stack+si+i);
					}
					*(stack+si)=tos;
					si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO:
				{
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_ARRAY;
					sll_array_create(0,&(tos->dt.a));
					*(stack+si)=tos;
					si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ONE:
				{
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_ARRAY;
					sll_array_create(1,&(tos->dt.a));
					tos->dt.a.v[0]=*(stack+si-1);
					*(stack+si-1)=tos;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP:
				{
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_MAP;
					sll_map_t* m=&(tos->dt.m);
					sll_map_create((ai->dt.ml+1)>>1,m);
					si-=ai->dt.ml;
					for (sll_map_length_t i=0;i<ai->dt.ml;i++){
						m->v[i]=*(stack+si+i);
					}
					if (ai->dt.ml&1){
						m->v[ai->dt.ml]=SLL_ACQUIRE_STATIC_INT(0);
					}
					*(stack+si)=tos;
					si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP_ZERO:
				{
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_MAP;
					sll_map_create(0,&(tos->dt.m));
					*(stack+si)=tos;
					si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE:
				SLL_RELEASE(*(_vm_var_data+ai->dt.v));
				*(_vm_var_data+ai->dt.v)=*(stack+si-1);
				SLL_ACQUIRE(*(stack+si-1));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP:
				SLL_RELEASE(*(_vm_var_data+ai->dt.v));
				si--;
				*(_vm_var_data+ai->dt.v)=*(stack+si);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE:
				SLL_RELEASE(*(_vm_var_data+ai->dt.v));
				*(_vm_var_data+ai->dt.v)=SLL_ACQUIRE_STATIC_INT(-1);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ZERO:
				SLL_RELEASE(*(_vm_var_data+ai->dt.v));
				*(_vm_var_data+ai->dt.v)=SLL_ACQUIRE_STATIC_INT(0);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ONE:
				SLL_RELEASE(*(_vm_var_data+ai->dt.v));
				*(_vm_var_data+ai->dt.v)=SLL_ACQUIRE_STATIC_INT(1);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_TWO:
				SLL_RELEASE(*(_vm_var_data+ai->dt.v));
				*(_vm_var_data+ai->dt.v)=SLL_ACQUIRE_STATIC_INT(2);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_THREE:
				SLL_RELEASE(*(_vm_var_data+ai->dt.v));
				*(_vm_var_data+ai->dt.v)=SLL_ACQUIRE_STATIC_INT(3);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_FOUR:
				SLL_RELEASE(*(_vm_var_data+ai->dt.v));
				*(_vm_var_data+ai->dt.v)=SLL_ACQUIRE_STATIC_INT(4);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP:
				{
					WRITE_THREAD_DATA;
					sll_object_t* n=sll_operator_cast(*(stack+si-1),sll_static_int[SLL_OBJECT_TYPE_STRING]);
					READ_THREAD_DATA;
					SLL_RELEASE(*(stack+si-1));
					sll_function_index_t i=sll_lookup_internal_function(sll_current_runtime_data->ift,n->dt.s.v);
					SLL_RELEASE(n);
					*(stack+si-1)=(i==SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX?SLL_ACQUIRE_STATIC_INT(0):SLL_FROM_INT(~((sll_integer_t)i)));
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR:
				{
					si++;
					sll_function_index_t i=sll_lookup_internal_function(sll_current_runtime_data->ift,(sll_current_runtime_data->a_dt->st.dt+ai->dt.s)->v);
					*(stack+si-1)=(i==SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX?SLL_ACQUIRE_STATIC_INT(0):SLL_FROM_INT(~((sll_integer_t)i)));
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP:
				JUMP_INSTRUCTION;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JB:
				CMP_INSTRUCTION(=,SLL_COMPARE_RESULT_BELOW);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:
				CMP_INSTRUCTION(!,SLL_COMPARE_RESULT_ABOVE);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JA:
				CMP_INSTRUCTION(=,SLL_COMPARE_RESULT_ABOVE);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:
				CMP_INSTRUCTION(!,SLL_COMPARE_RESULT_BELOW);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JE:
				CMP_INSTRUCTION_EQ(sll_operator_equal);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:
				CMP_INSTRUCTION_EQ(!sll_operator_equal);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:
				CMP_INSTRUCTION_BOOL(0);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ:
				CMP_INSTRUCTION_BOOL(1);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JSE:
				CMP_INSTRUCTION_EQ(sll_operator_strict_equal);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JSNE:
				CMP_INSTRUCTION_EQ(!sll_operator_strict_equal);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JI:
				CMP_INSTRUCTION_EQ(sll_operator_includes);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNI:
				CMP_INSTRUCTION_EQ(!sll_operator_includes);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JT:
				{
					si--;
					sll_stack_offset_t e_si=si;
					si-=ai->dt.al<<1;
					sll_stack_offset_t i=si-1;
					sll_object_t* cnd=*(stack+si-1);
					sll_integer_t cnd_v;
					if (SLL_OBJECT_GET_TYPE(cnd)==SLL_OBJECT_TYPE_INT){
						cnd_v=cnd->dt.i;
					}
					else if (SLL_OBJECT_GET_TYPE(cnd)==SLL_OBJECT_TYPE_CHAR){
						cnd_v=cnd->dt.c;
					}
					else{
						SLL_RELEASE(cnd);
						while (si<e_si){
							SLL_RELEASE(*(stack+si));
							si++;
						}
						goto _jump_to_default;
					}
					SLL_RELEASE(cnd);
					while (si<e_si){
						sll_object_t* k=*(stack+si);
						if ((SLL_OBJECT_GET_TYPE(k)==SLL_OBJECT_TYPE_INT&&k->dt.i==cnd_v)||(SLL_OBJECT_GET_TYPE(k)==SLL_OBJECT_TYPE_CHAR&&k->dt.c==cnd_v)){
							goto _cleanup_jump_table;
						}
						SLL_RELEASE(k);
						SLL_RELEASE(*(stack+si+1));
						si+=2;
					}
_jump_to_default:
					SLL_ASSERT(si==e_si);
					ii=(sll_instruction_index_t)((*(stack+si))->dt.i);
					ai=_get_instruction_at_offset(sll_current_runtime_data->a_dt,ii);
					SLL_RELEASE(*(stack+si));
					si=i;
					continue;
_cleanup_jump_table:;
					ii=(sll_instruction_index_t)((*(stack+si+1))->dt.i);
					ai=_get_instruction_at_offset(sll_current_runtime_data->a_dt,ii);
					while (si<e_si){
						SLL_RELEASE(*(stack+si));
						si++;
					}
					SLL_RELEASE(*(stack+si));
					si=i;
					continue;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?_vm_var_data+ai->dt.v:stack+si-1);
					WRITE_THREAD_DATA;
					sll_object_t* n=sll_static_int[!sll_operator_bool(*tos)];
					READ_THREAD_DATA;
					SLL_ACQUIRE(n);
					SLL_RELEASE(*tos);
					*tos=n;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_BOOL:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?_vm_var_data+ai->dt.v:stack+si-1);
					WRITE_THREAD_DATA;
					sll_object_t* n=sll_static_int[sll_operator_bool(*tos)];
					READ_THREAD_DATA;
					SLL_ACQUIRE(n);
					SLL_RELEASE(*tos);
					*tos=n;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_INC:
				OPERATOR_INSTRUCTION_UNARY(sll_operator_inc);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DEC:
				OPERATOR_INSTRUCTION_UNARY(sll_operator_dec);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_add);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_SUB:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_sub);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MULT:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_mult);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DIV:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_div);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_FDIV:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_floor_div);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MOD:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_mod);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_AND:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_and);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_OR:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_or);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_XOR:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_xor);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_INV:
				OPERATOR_INSTRUCTION_UNARY(sll_operator_inv);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_SHR:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_shr);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_SHL:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_shl);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LENGTH:
				OPERATOR_INSTRUCTION_UNARY(sll_operator_len);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_COPY:
				OPERATOR_INSTRUCTION_UNARY(sll_operator_dup);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_access);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_TWO:
				OPERATOR_INSTRUCTION_TERNARY(sll_operator_access_range);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_THREE:
				OPERATOR_INSTRUCTION_QUATERNARY(sll_operator_access_range_step);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_VAR:
				{
					si-=ai->dt.va.l-1;
					sll_object_t* d=*(_vm_var_data+ai->dt.va.v);
					SLL_ACQUIRE(d);
					sll_arg_count_t l=ai->dt.va.l;
					sll_object_t*const* p=stack+si-1;
					WRITE_THREAD_DATA;
					do{
						SLL_ASSERT((*p)->t==SLL_OBJECT_TYPE_STRING);
						sll_object_t* nd=sll_operator_access(d,*p);
						SLL_RELEASE(d);
						SLL_RELEASE(*p);
						d=nd;
						p++;
						l--;
					} while (l);
					READ_THREAD_DATA;
					*(stack+si-1)=d;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN:
				{
					si-=2;
					WRITE_THREAD_DATA;
					sll_operator_assign((SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)?*(_vm_var_data+ai->dt.v):*(stack+si-1)),*(stack+si),*(stack+si+1));
					READ_THREAD_DATA;
					SLL_RELEASE(*(stack+si));
					SLL_RELEASE(*(stack+si+1));
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_TWO:
				SLL_UNIMPLEMENTED();
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_THREE:
				SLL_UNIMPLEMENTED();
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR_ACCESS:
				{
					sll_object_t* nv=*(stack+si-1);
					si-=ai->dt.va.l+1;
					sll_object_t* d=*(_vm_var_data+ai->dt.va.v);
					SLL_ACQUIRE(d);
					sll_arg_count_t l=ai->dt.va.l-1;
					sll_object_t*const* p=stack+si;
					WRITE_THREAD_DATA;
					while (l){
						SLL_ASSERT((*p)->t==SLL_OBJECT_TYPE_STRING);
						sll_object_t* nd=sll_operator_access(d,*p);
						SLL_RELEASE(d);
						SLL_RELEASE(*p);
						d=nd;
						p++;
						l--;
					}
					SLL_ASSERT((*p)->t==SLL_OBJECT_TYPE_STRING);
					sll_operator_assign(d,*p,nv);
					READ_THREAD_DATA;
					SLL_RELEASE(d);
					SLL_RELEASE(*p);
					SLL_RELEASE(nv);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_cast);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?_vm_var_data+ai->dt.v:stack+si-1);
					sll_object_t* t=SLL_CREATE();
					t->t=SLL_OBJECT_TYPE_INT;
					t->dt.i=ai->dt.t;
					WRITE_THREAD_DATA;
					sll_object_t* n=sll_operator_cast(*tos,t);
					READ_THREAD_DATA;
					SLL_RELEASE(*tos);
					SLL_RELEASE(t);
					*tos=n;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_TYPEOF:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?_vm_var_data+ai->dt.v:stack+si-1);
					sll_object_t* n=SLL_FROM_INT(SLL_OBJECT_GET_TYPE(*tos));
					SLL_RELEASE(*tos);
					*tos=n;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL:
				{
					si-=ai->dt.ac<<1;
					sll_object_type_t nt;
					if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_ANONYMOUS(ai)){
						nt=sll_add_type(sll_current_runtime_data->tt,(const sll_object_t*const*)(stack+si),ai->dt.ac,NULL);
						for (sll_arg_count_t i=0;i<(ai->dt.ac<<1);i++){
							SLL_RELEASE(*(stack+si+i));
						}
						si++;
					}
					else{
						SLL_ASSERT(SLL_OBJECT_GET_TYPE(*(stack+si-1))==SLL_OBJECT_TYPE_STRING);
						nt=sll_add_type(sll_current_runtime_data->tt,(const sll_object_t*const*)(stack+si),ai->dt.ac,&((*(stack+si-1))->dt.s));
						SLL_RELEASE(*(stack+si-1));
						for (sll_arg_count_t i=0;i<(ai->dt.ac<<1);i++){
							SLL_RELEASE(*(stack+si+i));
						}
					}
					*(stack+si-1)=SLL_FROM_INT(nt);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL_ZERO:
				{
					if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_ANONYMOUS(ai)){
						*(stack+si)=SLL_FROM_INT(sll_add_type(sll_current_runtime_data->tt,NULL,0,NULL));
						si++;
					}
					else{
						SLL_ASSERT(SLL_OBJECT_GET_TYPE(*(stack+si-1))==SLL_OBJECT_TYPE_STRING);
						sll_object_type_t nt=sll_add_type(sll_current_runtime_data->tt,NULL,0,&((*(stack+si-1))->dt.s));
						SLL_RELEASE(*(stack+si-1));
						*(stack+si-1)=SLL_FROM_INT(nt);
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW:
				{
					si-=ai->dt.ac;
					WRITE_THREAD_DATA;
					sll_object_t* t=sll_operator_cast(*(stack+si-1),sll_static_int[SLL_OBJECT_TYPE_INT]);
					SLL_RELEASE(*(stack+si-1));
					sll_object_type_t ot=(t->dt.i<0||t->dt.i>sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE?SLL_OBJECT_TYPE_INT:(sll_object_type_t)(t->dt.i));
					SLL_RELEASE(t);
					*(stack+si-1)=sll_create_object_type(sll_current_runtime_data->tt,ot,stack+si,ai->dt.ac);
					READ_THREAD_DATA;
					for (sll_arg_count_t i=0;i<ai->dt.ac;i++){
						SLL_RELEASE(*(stack+si+i));
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW_DECL:
				*(stack+si)=sll_create_new_object_type(sll_current_runtime_data->tt);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR:
				*(stack+si)=*(_vm_var_data+ai->dt.v);
				SLL_ACQUIRE(*(stack+si));
				si++;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT:
				{
					si--;
					sll_object_t* tos=*(stack+si);
					if (io){
						if (tos->t==SLL_OBJECT_TYPE_STRING){
							sll_file_write(sll_current_vm_config->out,tos->dt.s.v,tos->dt.s.l*sizeof(sll_char_t));
						}
						else{
							sll_string_t str;
							sll_api_string_convert(&tos,1,&str);
							sll_file_write(sll_current_vm_config->out,str.v,str.l*sizeof(sll_char_t));
							sll_free_string(&str);
						}
					}
					SLL_RELEASE(tos);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
				if (io){
					sll_file_write_char(sll_current_vm_config->out,ai->dt.c);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
				if (io){
					sll_file_write(sll_current_vm_config->out,(sll_current_runtime_data->a_dt->st.dt+ai->dt.s)->v,(sll_current_runtime_data->a_dt->st.dt+ai->dt.s)->l*sizeof(sll_char_t));
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP:
				{
					si--;
					sll_object_t* tos=*(stack+si);
					if (SLL_OBJECT_GET_TYPE(tos)==SLL_OBJECT_TYPE_INT){
						sll_integer_t i=tos->dt.i;
						SLL_RELEASE(tos);
						if (i<0){
							sll_function_index_t j=(sll_function_index_t)(~i);
							if (j<sll_current_runtime_data->ift->l){
								si-=ai->dt.ac;
								WRITE_THREAD_DATA;
								sll_object_t* n=_call_internal_function(j,stack+si,ai->dt.ac);
								READ_THREAD_DATA;
								for (sll_arg_count_t k=0;k<ai->dt.ac;k++){
									SLL_RELEASE(*(stack+si+k));
								}
								if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)!=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP){
									*(stack+si)=n;
									si++;
								}
								else{
									SLL_RELEASE(n);
								}
								break;
							}
						}
						else if (i&&i<=sll_current_runtime_data->a_dt->ft.l){
							WRITE_THREAD_DATA;
							_call_function((sll_function_index_t)(i-1),ai->dt.ac);
							READ_THREAD_DATA;
							ai=_get_instruction_at_offset(sll_current_runtime_data->a_dt,ii);
							continue;
						}
					}
					else{
						SLL_RELEASE(tos);
					}
					si-=ai->dt.ac;
					for (sll_arg_count_t i=0;i<ai->dt.ac;i++){
						SLL_RELEASE(*(stack+si+i));
					}
					if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)!=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP){
						*(stack+si)=SLL_ACQUIRE_STATIC_INT(0);
						si++;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO:
				if (ai->dt.i<0){
					sll_function_index_t i=(sll_function_index_t)(~ai->dt.i);
					if (i<sll_current_runtime_data->ift->l){
						WRITE_THREAD_DATA;
						sll_object_t* n=_call_internal_function(i,NULL,0);
						READ_THREAD_DATA;
						*(stack+si)=n;
						si++;
						break;
					}
				}
				else if (ai->dt.i&&ai->dt.i<=sll_current_runtime_data->a_dt->ft.l){
					WRITE_THREAD_DATA;
					_call_function((sll_function_index_t)(ai->dt.i-1),0);
					READ_THREAD_DATA;
					ai=_get_instruction_at_offset(sll_current_runtime_data->a_dt,ii);
					continue;
				}
				*(stack+si)=SLL_ACQUIRE_STATIC_INT(0);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:
				if (ai->dt.i<0){
					sll_function_index_t i=(sll_function_index_t)(~ai->dt.i);
					if (i<sll_current_runtime_data->ift->l){
						si--;
						WRITE_THREAD_DATA;
						sll_object_t* n=_call_internal_function(i,stack+si,1);
						READ_THREAD_DATA;
						si++;
						SLL_RELEASE(*(stack+si-1));
						*(stack+si-1)=n;
						break;
					}
				}
				else if (ai->dt.i&&ai->dt.i<=sll_current_runtime_data->a_dt->ft.l){
					WRITE_THREAD_DATA;
					_call_function((sll_function_index_t)(ai->dt.i-1),1);
					READ_THREAD_DATA;
					ai=_get_instruction_at_offset(sll_current_runtime_data->a_dt,ii);
					continue;
				}
				SLL_RELEASE(*(stack+si-1));
				*(stack+si-1)=SLL_ACQUIRE_STATIC_INT(0);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_REF:
				{
					sll_integer_t a=(sll_integer_t)ADDR(*(stack+si-1));
					SLL_RELEASE(*(stack+si-1));
					*(stack+si-1)=SLL_FROM_INT(a);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET:
_return:;
				{
					sll_object_t* tmp=*(stack+si-1);
					if (sll_current_runtime_data->c_st.l==o_c_st){
						if (o_c_st){
							WRITE_THREAD_DATA;
							_pop_call_stack();
							READ_THREAD_DATA;
						}
						SLL_ACQUIRE(tmp);
						return tmp;
					}
					WRITE_THREAD_DATA;
					_pop_call_stack();
					READ_THREAD_DATA;
					ai=_get_instruction_at_offset(sll_current_runtime_data->a_dt,ii);
					if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)!=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP){
						*(stack+si)=tmp;
						si++;
					}
					else{
						SLL_RELEASE(tmp);
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO:
				*(stack+si)=SLL_ACQUIRE_STATIC_INT(0);
				si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				{
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_INT;
					tos->dt.i=ai->dt.i;
					*(stack+si)=tos;
					si++;
					goto _return;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				{
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_FLOAT;
					tos->dt.f=ai->dt.f;
					*(stack+si)=tos;
					si++;
					goto _return;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				*(stack+si)=SLL_ACQUIRE_STATIC_CHAR(ai->dt.c);
				si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				*(stack+si)=SLL_CREATE();
				(*(stack+si))->t=SLL_OBJECT_TYPE_STRING;
				sll_string_clone(sll_current_runtime_data->a_dt->st.dt+ai->dt.s,&((*(stack+si))->dt.s));
				si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR:
				*(stack+si)=*(_vm_var_data+ai->dt.v);
				SLL_ACQUIRE(*(stack+si));
				si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL:
				SLL_RELEASE(*(_vm_var_data+ai->dt.v));
				*(_vm_var_data+ai->dt.v)=SLL_ACQUIRE_STATIC_INT(0);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL:
				*(stack+si)=*(_vm_var_data+ai->dt.v);
				*(_vm_var_data+ai->dt.v)=SLL_ACQUIRE_STATIC_INT(0);
				si++;
				break;
		}
		ai++;
		if (ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			ai=ai->dt._p;
		}
		ii++;
	}
}
