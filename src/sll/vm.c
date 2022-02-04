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
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/platform.h>
#include <sll/sandbox.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <sll/vm.h>



#define JUMP_INSTRUCTION \
	_vm_ii=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?_vm_ii+ai->dt.rj:ai->dt.j); \
	ai=_get_instruction_at_offset(sll_current_runtime_data->a_dt,_vm_ii); \
	continue;
#define OPERATOR_INSTRUCTION_UNARY(nm) \
	{ \
		sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?_vm_var_data+ai->dt.v:_vm_stack+_vm_si-1); \
		sll_object_t* n=nm(*tos); \
		SLL_RELEASE(*tos); \
		*tos=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION_BINARY(nm) \
	{ \
		_vm_si--; \
		sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?_vm_var_data+ai->dt.v:_vm_stack+_vm_si-1); \
		sll_object_t* n=nm(*tos,*(_vm_stack+_vm_si)); \
		SLL_RELEASE(*tos); \
		SLL_RELEASE(*(_vm_stack+_vm_si)); \
		*tos=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION_TERNARY(nm) \
	{ \
		_vm_si-=2; \
		sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?_vm_var_data+ai->dt.v:_vm_stack+_vm_si-1); \
		sll_object_t* n=nm(*tos,*(_vm_stack+_vm_si),*(_vm_stack+_vm_si+1)); \
		SLL_RELEASE(*tos); \
		SLL_RELEASE(*(_vm_stack+_vm_si)); \
		SLL_RELEASE(*(_vm_stack+_vm_si+1)); \
		*tos=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION_QUATERNARY(nm) \
	{ \
		_vm_si-=3; \
		sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?_vm_var_data+ai->dt.v:_vm_stack+_vm_si-1); \
		sll_object_t* n=nm(*tos,*(_vm_stack+_vm_si),*(_vm_stack+_vm_si+1),*(_vm_stack+_vm_si+2)); \
		SLL_RELEASE(*tos); \
		SLL_RELEASE(*(_vm_stack+_vm_si)); \
		SLL_RELEASE(*(_vm_stack+_vm_si+1)); \
		SLL_RELEASE(*(_vm_stack+_vm_si+2)); \
		*tos=n; \
		break; \
	}



static sll_object_t** _vm_var_data=NULL;
static sll_identifier_index_t _vm_ii=0;
static sll_stack_offset_t _vm_si=0;
static sll_object_t** _vm_stack=NULL;



__SLL_EXTERNAL sll_instruction_index_t sll_current_instruction_count=0;
__SLL_EXTERNAL sll_instruction_index_t sll_current_instruction_index=0;
__SLL_EXTERNAL const sll_runtime_data_t* sll_current_runtime_data=NULL;
__SLL_EXTERNAL const sll_vm_config_t* sll_current_vm_config=NULL;



static sll_object_t* _call_internal_function(sll_function_index_t i,sll_object_t*const* a,sll_arg_count_t ac){
	_push_call_stack((*(sll_current_runtime_data->ift->dt+i))->nm.v,_vm_si);
	sll_object_t* o=(*(sll_current_runtime_data->ift->dt+i))->p(a,ac);
	_pop_call_stack();
	return o;
}



static void _call_function(sll_function_index_t fn,sll_arg_count_t ac){
	sll_assembly_function_t* af=sll_current_runtime_data->a_dt->ft.dt+fn;
	if (SLL_ASSEMBLY_FUNCTION_IS_VAR_ARG(af)){
		sll_object_t* tos=SLL_CREATE();
		tos->t=SLL_OBJECT_TYPE_ARRAY;
		if (SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)>ac){
			sll_static_int[0]->rc+=SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)-ac-1;
			for (sll_arg_count_t i=0;i<SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)-ac-1;i++){
				*(_vm_stack+_vm_si)=sll_static_int[0];
				_vm_si++;
			}
			SLL_INIT_ARRAY(&(tos->dt.a));
		}
		else{
			if (!sll_array_create(ac-SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)+1,&(tos->dt.a))){
				SLL_UNIMPLEMENTED();
			}
			_vm_si-=tos->dt.a.l;
			for (sll_array_length_t i=0;i<tos->dt.a.l;i++){
				tos->dt.a.v[i]=*(_vm_stack+_vm_si+i);
			}
		}
		*(_vm_stack+_vm_si)=tos;
		_vm_si++;
	}
	else{
		if (ac!=SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)){
			if (SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)>ac){
				sll_static_int[0]->rc+=SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)-ac;
				for (sll_arg_count_t i=0;i<SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)-ac;i++){
					*(_vm_stack+_vm_si+i)=sll_static_int[0];
				}
				_vm_si+=SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)-ac;
			}
			else{
				for (sll_arg_count_t i=ac-SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af);i;i--){
					_vm_si--;
					SLL_RELEASE(*(_vm_stack+_vm_si));
				}
			}
		}
	}
	_push_call_stack((sll_current_runtime_data->a_dt->st.dt+af->nm)->v,_vm_si-SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af));
	_vm_ii=af->i;
}



void _push_call_stack(const sll_char_t* nm,sll_stack_offset_t si){
	SLL_ASSERT(sll_current_runtime_data->c_st.l<=sll_current_vm_config->c_st_sz);
	(sll_current_runtime_data->c_st.dt+sll_current_runtime_data->c_st.l)->nm=nm;
	(sll_current_runtime_data->c_st.dt+sll_current_runtime_data->c_st.l)->_ii=_vm_ii;
	(sll_current_runtime_data->c_st.dt+sll_current_runtime_data->c_st.l)->_s=(si==SLL_MAX_STACK_OFFSET?_vm_si:si);
	(*((sll_call_stack_size_t*)(&(sll_current_runtime_data->c_st.l))))++;
}



void _pop_call_stack(void){
	(*((sll_call_stack_size_t*)(&(sll_current_runtime_data->c_st.l))))--;
	_vm_ii=(sll_current_runtime_data->c_st.dt+sll_current_runtime_data->c_st.l)->_ii;
	_vm_si=(sll_current_runtime_data->c_st.dt+sll_current_runtime_data->c_st.l)->_s;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_code_t sll_execute_assembly(const sll_assembly_data_t* a_dt,const sll_vm_config_t* cfg){
	sll_object_t** old_vm_var_data=_vm_var_data;
	sll_identifier_index_t old_vm_ii=_vm_ii;
	sll_stack_offset_t old_vm_si=_vm_si;
	sll_object_t** old_vm_stack=_vm_stack;
	sll_instruction_index_t old_instruction_count=sll_current_instruction_count;
	sll_instruction_index_t old_instruction_index=sll_current_instruction_index;
	const sll_runtime_data_t* old_runtime_data=sll_current_runtime_data;
	const sll_vm_config_t* old_vm_config=sll_current_vm_config;
	_vm_ii=0;
	_vm_si=0;
	sll_current_instruction_count=0;
	sll_current_vm_config=cfg;// lgtm [cpp/stack-address-escape]
	sll_size_t ptr_sz=SLL_ROUND_LARGE_PAGE(cfg->s_sz+a_dt->vc*sizeof(sll_object_t*)+cfg->c_st_sz*sizeof(sll_call_stack_frame_t));
	addr_t ptr=ADDR(sll_platform_allocate_page(ptr_sz,1));
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
	_vm_stack=(sll_object_t**)ptr;
	sll_object_t* o=sll_execute_function(0,NULL,0);
	sll_object_t* rc_o=sll_operator_cast(o,sll_static_int[SLL_OBJECT_TYPE_INT]);
	SLL_RELEASE(o);
	sll_return_code_t rc=(sll_return_code_t)(rc_o->dt.i);
	SLL_RELEASE(rc_o);
	sll_current_runtime_data=NULL;
	for (sll_variable_index_t i=0;i<a_dt->vc;i++){
		SLL_RELEASE(*(_vm_var_data+i));
	}
	sll_platform_free_page(PTR(ptr-a_dt->vc*sizeof(sll_object_t*)-a_dt->st.l*sizeof(sll_object_t)-cfg->c_st_sz*sizeof(sll_call_stack_frame_t)),ptr_sz);
	sll_free_internal_function_table(&ift);
	sll_free_object_type_list(&tt);
	_vm_var_data=old_vm_var_data;
	_vm_ii=old_vm_ii;
	_vm_si=old_vm_si;
	_vm_stack=old_vm_stack;
	sll_current_instruction_count=old_instruction_count;
	sll_current_instruction_index=old_instruction_index;
	sll_current_runtime_data=old_runtime_data;
	sll_current_vm_config=old_vm_config;
	return rc;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_execute_function(sll_integer_t fn_idx,sll_object_t*const* al,sll_arg_count_t all){
	if (!sll_current_runtime_data||!sll_current_vm_config){
		return NULL;
	}
	const sll_assembly_instruction_t* ai=NULL;
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
				*(_vm_stack+_vm_si+i)=*(al+i);
				SLL_ACQUIRE(*(al+i));
			}
			_vm_si+=all;
			_call_function((sll_function_index_t)(fn_idx-1),all);
			ai=_get_instruction_at_offset(sll_current_runtime_data->a_dt,_vm_ii);
		}
		else{
			return SLL_ACQUIRE_STATIC_INT(0);
		}
	}
	else{
		_vm_ii=0;
		_vm_si=0;
		ai=sll_current_runtime_data->a_dt->h;
	}
	sll_call_stack_size_t o_c_st=sll_current_runtime_data->c_st.l;
	sll_bool_t io=!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)||sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO);
	while (1){
		if (_vm_ii>=sll_current_runtime_data->a_dt->ic){
			SLL_UNIMPLEMENTED();
		}
		sll_current_instruction_count++;
		sll_current_instruction_index=_vm_ii;
		switch (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)){
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_POP:
				_vm_si--;
				SLL_RELEASE(*(_vm_stack+_vm_si));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO:
				_vm_si-=2;
				SLL_RELEASE(*(_vm_stack+_vm_si));
				SLL_RELEASE(*(_vm_stack+_vm_si+1));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT:
				{
					sll_object_t* tos=*(_vm_stack+_vm_si-1);
					*(_vm_stack+_vm_si-1)=*(_vm_stack+_vm_si-2);
					*(_vm_stack+_vm_si-2)=tos;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP:
				_vm_si--;
				SLL_RELEASE(*(_vm_stack+_vm_si));
				*(_vm_stack+_vm_si-1)=*(_vm_stack+_vm_si);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DUP:
				*(_vm_stack+_vm_si)=*(_vm_stack+_vm_si-1);
				SLL_ACQUIRE(*(_vm_stack+_vm_si));
				_vm_si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
				{
					sll_object_t* tos=SLL_CREATE();
					*(_vm_stack+_vm_si)=tos;
					tos->t=SLL_OBJECT_TYPE_INT;
					tos->dt.i=ai->dt.i;
					_vm_si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE:
				*(_vm_stack+_vm_si)=SLL_ACQUIRE_STATIC_INT(-1);
				_vm_si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO:
				*(_vm_stack+_vm_si)=SLL_ACQUIRE_STATIC_INT(0);
				_vm_si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE:
				*(_vm_stack+_vm_si)=SLL_ACQUIRE_STATIC_INT(1);
				_vm_si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO:
				*(_vm_stack+_vm_si)=SLL_ACQUIRE_STATIC_INT(2);
				_vm_si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE:
				*(_vm_stack+_vm_si)=SLL_ACQUIRE_STATIC_INT(3);
				_vm_si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR:
				*(_vm_stack+_vm_si)=SLL_ACQUIRE_STATIC_INT(4);
				_vm_si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
				{
					sll_object_t* tos=SLL_CREATE();
					*(_vm_stack+_vm_si)=tos;
					tos->t=SLL_OBJECT_TYPE_FLOAT;
					tos->dt.f=ai->dt.f;
					_vm_si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
				*(_vm_stack+_vm_si)=SLL_ACQUIRE_STATIC_CHAR(ai->dt.c);
				_vm_si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL:
				*(_vm_stack+_vm_si)=SLL_FROM_INT((SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?_vm_ii+ai->dt.rj:ai->dt.j));
				_vm_si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD:
				*(_vm_stack+_vm_si)=*(_vm_var_data+ai->dt.v);
				SLL_ACQUIRE(*(_vm_stack+_vm_si));
				_vm_si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
				*(_vm_stack+_vm_si)=SLL_CREATE();
				(*(_vm_stack+_vm_si))->t=SLL_OBJECT_TYPE_STRING;
				sll_string_clone(sll_current_runtime_data->a_dt->st.dt+ai->dt.s,&((*(_vm_stack+_vm_si))->dt.s));
				_vm_si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK:
				{
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_ARRAY;
					sll_array_t* a=&(tos->dt.a);
					if (!sll_array_create(ai->dt.al,a)){
						SLL_UNIMPLEMENTED();
					}
					_vm_si-=ai->dt.al;
					for (sll_array_length_t i=0;i<ai->dt.al;i++){
						a->v[i]=*(_vm_stack+_vm_si+i);
					}
					*(_vm_stack+_vm_si)=tos;
					_vm_si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO:
				{
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_ARRAY;
					if (!sll_array_create(0,&(tos->dt.a))){
						SLL_UNIMPLEMENTED();
					}
					*(_vm_stack+_vm_si)=tos;
					_vm_si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ONE:
				{
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_ARRAY;
					if (!sll_array_create(1,&(tos->dt.a))){
						SLL_UNIMPLEMENTED();
					}
					tos->dt.a.v[0]=*(_vm_stack+_vm_si-1);
					*(_vm_stack+_vm_si-1)=tos;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP:
				{
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_MAP;
					sll_map_t* m=&(tos->dt.m);
					sll_map_create((ai->dt.ml+1)>>1,m);
					_vm_si-=ai->dt.ml;
					for (sll_map_length_t i=0;i<ai->dt.ml;i++){
						m->v[i]=*(_vm_stack+_vm_si+i);
					}
					if (ai->dt.ml&1){
						m->v[ai->dt.ml]=SLL_ACQUIRE_STATIC_INT(0);
					}
					*(_vm_stack+_vm_si)=tos;
					_vm_si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP_ZERO:
				{
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_MAP;
					sll_map_create(0,&(tos->dt.m));
					*(_vm_stack+_vm_si)=tos;
					_vm_si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE:
				SLL_RELEASE(*(_vm_var_data+ai->dt.v));
				*(_vm_var_data+ai->dt.v)=*(_vm_stack+_vm_si-1);
				SLL_ACQUIRE(*(_vm_stack+_vm_si-1));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP:
				SLL_RELEASE(*(_vm_var_data+ai->dt.v));
				_vm_si--;
				*(_vm_var_data+ai->dt.v)=*(_vm_stack+_vm_si);
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
					sll_object_t* n=sll_operator_cast(*(_vm_stack+_vm_si-1),sll_static_int[SLL_OBJECT_TYPE_STRING]);
					SLL_RELEASE(*(_vm_stack+_vm_si-1));
					sll_function_index_t i=sll_lookup_internal_function(sll_current_runtime_data->ift,n->dt.s.v);
					SLL_RELEASE(n);
					*(_vm_stack+_vm_si-1)=(i==SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX?SLL_ACQUIRE_STATIC_INT(0):SLL_FROM_INT(~((sll_integer_t)i)));
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR:
			{
				_vm_si++;
				sll_function_index_t i=sll_lookup_internal_function(sll_current_runtime_data->ift,(sll_current_runtime_data->a_dt->st.dt+ai->dt.s)->v);
				*(_vm_stack+_vm_si-1)=(i==SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX?SLL_ACQUIRE_STATIC_INT(0):SLL_FROM_INT(~((sll_integer_t)i)));
				break;
			}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP:
				JUMP_INSTRUCTION;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JB:
				if (sll_operator_compare(*(_vm_stack+_vm_si-2),*(_vm_stack+_vm_si-1))==SLL_COMPARE_RESULT_BELOW){
					_vm_si-=2;
					SLL_RELEASE(*(_vm_stack+_vm_si));
					SLL_RELEASE(*(_vm_stack+_vm_si+1));
					JUMP_INSTRUCTION;
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:
				if (sll_operator_compare(*(_vm_stack+_vm_si-2),*(_vm_stack+_vm_si-1))!=SLL_COMPARE_RESULT_ABOVE){
					_vm_si-=2;
					SLL_RELEASE(*(_vm_stack+_vm_si));
					SLL_RELEASE(*(_vm_stack+_vm_si+1));
					JUMP_INSTRUCTION;
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JA:
				if (sll_operator_compare(*(_vm_stack+_vm_si-2),*(_vm_stack+_vm_si-1))==SLL_COMPARE_RESULT_ABOVE){
					_vm_si-=2;
					SLL_RELEASE(*(_vm_stack+_vm_si));
					SLL_RELEASE(*(_vm_stack+_vm_si+1));
					JUMP_INSTRUCTION;
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:
				if (sll_operator_compare(*(_vm_stack+_vm_si-2),*(_vm_stack+_vm_si-1))!=SLL_COMPARE_RESULT_BELOW){
					_vm_si-=2;
					SLL_RELEASE(*(_vm_stack+_vm_si));
					SLL_RELEASE(*(_vm_stack+_vm_si+1));
					JUMP_INSTRUCTION;
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JE:
				if (sll_operator_equal(*(_vm_stack+_vm_si-2),*(_vm_stack+_vm_si-1))){
					_vm_si-=2;
					SLL_RELEASE(*(_vm_stack+_vm_si));
					SLL_RELEASE(*(_vm_stack+_vm_si+1));
					JUMP_INSTRUCTION;
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:
				if (!sll_operator_equal(*(_vm_stack+_vm_si-2),*(_vm_stack+_vm_si-1))){
					_vm_si-=2;
					SLL_RELEASE(*(_vm_stack+_vm_si));
					SLL_RELEASE(*(_vm_stack+_vm_si+1));
					JUMP_INSTRUCTION;
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:
				if (!sll_operator_bool(*(_vm_stack+_vm_si-1))){
					_vm_si--;
					SLL_RELEASE(*(_vm_stack+_vm_si));
					JUMP_INSTRUCTION;
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ:
				if (sll_operator_bool(*(_vm_stack+_vm_si-1))){
					_vm_si--;
					SLL_RELEASE(*(_vm_stack+_vm_si));
					JUMP_INSTRUCTION;
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JSE:
				if (sll_operator_strict_equal(*(_vm_stack+_vm_si-2),*(_vm_stack+_vm_si-1))){
					_vm_si-=2;
					SLL_RELEASE(*(_vm_stack+_vm_si));
					SLL_RELEASE(*(_vm_stack+_vm_si+1));
					JUMP_INSTRUCTION;
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JSNE:
				if (!sll_operator_strict_equal(*(_vm_stack+_vm_si-2),*(_vm_stack+_vm_si-1))){
					_vm_si-=2;
					SLL_RELEASE(*(_vm_stack+_vm_si));
					SLL_RELEASE(*(_vm_stack+_vm_si+1));
					JUMP_INSTRUCTION;
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JI:
				if (sll_operator_includes(*(_vm_stack+_vm_si-2),*(_vm_stack+_vm_si-1))){
					_vm_si-=2;
					SLL_RELEASE(*(_vm_stack+_vm_si));
					SLL_RELEASE(*(_vm_stack+_vm_si+1));
					JUMP_INSTRUCTION;
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNI:
				if (!sll_operator_includes(*(_vm_stack+_vm_si-2),*(_vm_stack+_vm_si-1))){
					_vm_si-=2;
					SLL_RELEASE(*(_vm_stack+_vm_si));
					SLL_RELEASE(*(_vm_stack+_vm_si+1));
					JUMP_INSTRUCTION;
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JT:
				{
					_vm_si--;
					sll_stack_offset_t e_si=_vm_si;
					_vm_si-=ai->dt.al<<1;
					sll_stack_offset_t i=_vm_si-1;
					sll_object_t* cnd=*(_vm_stack+_vm_si-1);
					sll_integer_t cnd_v;
					if (SLL_OBJECT_GET_TYPE(cnd)==SLL_OBJECT_TYPE_INT){
						cnd_v=cnd->dt.i;
					}
					else if (SLL_OBJECT_GET_TYPE(cnd)==SLL_OBJECT_TYPE_CHAR){
						cnd_v=cnd->dt.c;
					}
					else{
						SLL_RELEASE(cnd);
						while (_vm_si<e_si){
							SLL_RELEASE(*(_vm_stack+_vm_si));
							_vm_si++;
						}
						goto _jump_to_default;
					}
					SLL_RELEASE(cnd);
					while (_vm_si<e_si){
						sll_object_t* k=*(_vm_stack+_vm_si);
						if ((SLL_OBJECT_GET_TYPE(k)==SLL_OBJECT_TYPE_INT&&k->dt.i==cnd_v)||(SLL_OBJECT_GET_TYPE(k)==SLL_OBJECT_TYPE_CHAR&&k->dt.c==cnd_v)){
							goto _cleanup_jump_table;
						}
						SLL_RELEASE(k);
						SLL_RELEASE(*(_vm_stack+_vm_si+1));
						_vm_si+=2;
					}
_jump_to_default:
					SLL_ASSERT(_vm_si==e_si);
					_vm_ii=(sll_instruction_index_t)((*(_vm_stack+_vm_si))->dt.i);
					ai=_get_instruction_at_offset(sll_current_runtime_data->a_dt,_vm_ii);
					SLL_RELEASE(*(_vm_stack+_vm_si));
					_vm_si=i;
					continue;
_cleanup_jump_table:;
					_vm_ii=(sll_instruction_index_t)((*(_vm_stack+_vm_si+1))->dt.i);
					ai=_get_instruction_at_offset(sll_current_runtime_data->a_dt,_vm_ii);
					while (_vm_si<e_si){
						SLL_RELEASE(*(_vm_stack+_vm_si));
						_vm_si++;
					}
					SLL_RELEASE(*(_vm_stack+_vm_si));
					_vm_si=i;
					continue;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?_vm_var_data+ai->dt.v:_vm_stack+_vm_si-1);
					sll_object_t* n=sll_static_int[!sll_operator_bool(*tos)];
					SLL_ACQUIRE(n);
					SLL_RELEASE(*tos);
					*tos=n;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_BOOL:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?_vm_var_data+ai->dt.v:_vm_stack+_vm_si-1);
					sll_object_t* n=sll_static_int[sll_operator_bool(*tos)];
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
					_vm_si-=ai->dt.va.l-1;
					sll_object_t* d=*(_vm_var_data+ai->dt.va.v);
					SLL_ACQUIRE(d);
					sll_arg_count_t l=ai->dt.va.l;
					sll_object_t*const* p=_vm_stack+_vm_si-1;
					do{
						SLL_ASSERT((*p)->t==SLL_OBJECT_TYPE_STRING);
						sll_object_t* nd=sll_operator_access(d,*p);
						SLL_RELEASE(d);
						SLL_RELEASE(*p);
						d=nd;
						p++;
						l--;
					} while (l);
					*(_vm_stack+_vm_si-1)=d;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN:
				{
					_vm_si-=2;
					sll_operator_assign((SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)?*(_vm_var_data+ai->dt.v):*(_vm_stack+_vm_si-1)),*(_vm_stack+_vm_si),*(_vm_stack+_vm_si+1));
					SLL_RELEASE(*(_vm_stack+_vm_si));
					SLL_RELEASE(*(_vm_stack+_vm_si+1));
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_TWO:
				SLL_UNIMPLEMENTED();
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_THREE:
				SLL_UNIMPLEMENTED();
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR_ACCESS:
				{
					sll_object_t* nv=*(_vm_stack+_vm_si-1);
					_vm_si-=ai->dt.va.l+1;
					sll_object_t* d=*(_vm_var_data+ai->dt.va.v);
					SLL_ACQUIRE(d);
					sll_arg_count_t l=ai->dt.va.l-1;
					sll_object_t*const* p=_vm_stack+_vm_si;
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
					SLL_RELEASE(d);
					SLL_RELEASE(*p);
					SLL_RELEASE(nv);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_cast);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?_vm_var_data+ai->dt.v:_vm_stack+_vm_si-1);
					sll_object_t* t=SLL_CREATE();
					t->t=SLL_OBJECT_TYPE_INT;
					t->dt.i=ai->dt.t;
					sll_object_t* n=sll_operator_cast(*tos,t);
					SLL_RELEASE(*tos);
					SLL_RELEASE(t);
					*tos=n;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_TYPEOF:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?_vm_var_data+ai->dt.v:_vm_stack+_vm_si-1);
					sll_object_t* n=sll_static_int[SLL_OBJECT_GET_TYPE(*tos)];
					SLL_ACQUIRE(n);
					SLL_RELEASE(*tos);
					*tos=n;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL:
				{
					_vm_si-=ai->dt.ac<<1;
					sll_object_type_t nt;
					if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_ANONYMOUS(ai)){
						nt=sll_add_type(sll_current_runtime_data->tt,(const sll_object_t*const*)(_vm_stack+_vm_si),ai->dt.ac,NULL);
						for (sll_arg_count_t i=0;i<(ai->dt.ac<<1);i++){
							SLL_RELEASE(*(_vm_stack+_vm_si+i));
						}
						_vm_si++;
					}
					else{
						SLL_ASSERT(SLL_OBJECT_GET_TYPE(*(_vm_stack+_vm_si-1))==SLL_OBJECT_TYPE_STRING);
						nt=sll_add_type(sll_current_runtime_data->tt,(const sll_object_t*const*)(_vm_stack+_vm_si),ai->dt.ac,&((*(_vm_stack+_vm_si-1))->dt.s));
						SLL_RELEASE(*(_vm_stack+_vm_si-1));
						for (sll_arg_count_t i=0;i<(ai->dt.ac<<1);i++){
							SLL_RELEASE(*(_vm_stack+_vm_si+i));
						}
					}
					*(_vm_stack+_vm_si-1)=SLL_FROM_INT(nt);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL_ZERO:
				{
					if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_ANONYMOUS(ai)){
						*(_vm_stack+_vm_si)=SLL_FROM_INT(sll_add_type(sll_current_runtime_data->tt,NULL,0,NULL));
						_vm_si++;
					}
					else{
						SLL_ASSERT(SLL_OBJECT_GET_TYPE(*(_vm_stack+_vm_si-1))==SLL_OBJECT_TYPE_STRING);
						sll_object_type_t nt=sll_add_type(sll_current_runtime_data->tt,NULL,0,&((*(_vm_stack+_vm_si-1))->dt.s));
						SLL_RELEASE(*(_vm_stack+_vm_si-1));
						*(_vm_stack+_vm_si-1)=SLL_FROM_INT(nt);
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW:
				{
					_vm_si-=ai->dt.ac;
					sll_object_t* t=sll_operator_cast(*(_vm_stack+_vm_si-1),sll_static_int[SLL_OBJECT_TYPE_INT]);
					SLL_RELEASE(*(_vm_stack+_vm_si-1));
					sll_object_type_t ot=(t->dt.i<0||t->dt.i>sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE?SLL_OBJECT_TYPE_INT:(sll_object_type_t)(t->dt.i));
					SLL_RELEASE(t);
					*(_vm_stack+_vm_si-1)=sll_create_object_type(sll_current_runtime_data->tt,ot,_vm_stack+_vm_si,ai->dt.ac);
					for (sll_arg_count_t i=0;i<ai->dt.ac;i++){
						SLL_RELEASE(*(_vm_stack+_vm_si+i));
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW_DECL:
				*(_vm_stack+_vm_si)=sll_create_new_object_type(sll_current_runtime_data->tt);
				_vm_si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR:
				*(_vm_stack+_vm_si)=*(_vm_var_data+ai->dt.v);
				SLL_ACQUIRE(*(_vm_stack+_vm_si));
				_vm_si++;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT:
				{
					_vm_si--;
					sll_object_t* tos=*(_vm_stack+_vm_si);
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
					_vm_si--;
					sll_object_t* tos=*(_vm_stack+_vm_si);
					if (SLL_OBJECT_GET_TYPE(tos)==SLL_OBJECT_TYPE_INT){
						sll_integer_t i=tos->dt.i;
						SLL_RELEASE(tos);
						if (i<0){
							sll_function_index_t j=(sll_function_index_t)(~i);
							if (j<sll_current_runtime_data->ift->l){
								_vm_si-=ai->dt.ac;
								sll_object_t* n=_call_internal_function(j,_vm_stack+_vm_si,ai->dt.ac);
								for (sll_arg_count_t k=0;k<ai->dt.ac;k++){
									SLL_RELEASE(*(_vm_stack+_vm_si+k));
								}
								if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)!=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP){
									*(_vm_stack+_vm_si)=n;
									_vm_si++;
								}
								else{
									SLL_RELEASE(n);
								}
								break;
							}
						}
						else if (i&&i<=sll_current_runtime_data->a_dt->ft.l){
							_call_function((sll_function_index_t)(i-1),ai->dt.ac);
							ai=_get_instruction_at_offset(sll_current_runtime_data->a_dt,_vm_ii);
							continue;
						}
					}
					else{
						SLL_RELEASE(tos);
					}
					_vm_si-=ai->dt.ac;
					for (sll_arg_count_t i=0;i<ai->dt.ac;i++){
						SLL_RELEASE(*(_vm_stack+_vm_si+i));
					}
					if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)!=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP){
						*(_vm_stack+_vm_si)=SLL_ACQUIRE_STATIC_INT(0);
						_vm_si++;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO:
				if (ai->dt.i<0){
					sll_function_index_t i=(sll_function_index_t)(~ai->dt.i);
					if (i<sll_current_runtime_data->ift->l){
						*(_vm_stack+_vm_si)=_call_internal_function(i,NULL,0);
						_vm_si++;
						break;
					}
				}
				else if (ai->dt.i&&ai->dt.i<=sll_current_runtime_data->a_dt->ft.l){
					_call_function((sll_function_index_t)(ai->dt.i-1),0);
					ai=_get_instruction_at_offset(sll_current_runtime_data->a_dt,_vm_ii);
					continue;
				}
				*(_vm_stack+_vm_si)=SLL_ACQUIRE_STATIC_INT(0);
				_vm_si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:
				if (ai->dt.i<0){
					sll_function_index_t i=(sll_function_index_t)(~ai->dt.i);
					if (i<sll_current_runtime_data->ift->l){
						_vm_si--;
						sll_object_t* n=_call_internal_function(i,_vm_stack+_vm_si,1);
						_vm_si++;
						SLL_RELEASE(*(_vm_stack+_vm_si-1));
						*(_vm_stack+_vm_si-1)=n;
						break;
					}
				}
				else if (ai->dt.i&&ai->dt.i<=sll_current_runtime_data->a_dt->ft.l){
					_call_function((sll_function_index_t)(ai->dt.i-1),1);
					ai=_get_instruction_at_offset(sll_current_runtime_data->a_dt,_vm_ii);
					continue;
				}
				SLL_RELEASE(*(_vm_stack+_vm_si-1));
				*(_vm_stack+_vm_si-1)=SLL_ACQUIRE_STATIC_INT(0);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_REF:
				{
					sll_integer_t a=(sll_integer_t)ADDR(*(_vm_stack+_vm_si-1));
					SLL_RELEASE(*(_vm_stack+_vm_si-1));
					*(_vm_stack+_vm_si-1)=SLL_FROM_INT(a);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET:
_return:;
				{
					sll_object_t* tmp=*(_vm_stack+_vm_si-1);
					if (sll_current_runtime_data->c_st.l==o_c_st){
						if (o_c_st){
							_pop_call_stack();
						}
						SLL_ACQUIRE(tmp);
						return tmp;
					}
					_pop_call_stack();
					ai=_get_instruction_at_offset(sll_current_runtime_data->a_dt,_vm_ii);
					if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)!=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP){
						*(_vm_stack+_vm_si)=tmp;
						_vm_si++;
					}
					else{
						SLL_RELEASE(tmp);
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO:
				*(_vm_stack+_vm_si)=SLL_ACQUIRE_STATIC_INT(0);
				_vm_si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				{
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_INT;
					tos->dt.i=ai->dt.i;
					*(_vm_stack+_vm_si)=tos;
					_vm_si++;
					goto _return;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				{
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_FLOAT;
					tos->dt.f=ai->dt.f;
					*(_vm_stack+_vm_si)=tos;
					_vm_si++;
					goto _return;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				*(_vm_stack+_vm_si)=SLL_ACQUIRE_STATIC_CHAR(ai->dt.c);
				_vm_si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				*(_vm_stack+_vm_si)=SLL_CREATE();
				(*(_vm_stack+_vm_si))->t=SLL_OBJECT_TYPE_STRING;
				sll_string_clone(sll_current_runtime_data->a_dt->st.dt+ai->dt.s,&((*(_vm_stack+_vm_si))->dt.s));
				_vm_si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR:
				*(_vm_stack+_vm_si)=*(_vm_var_data+ai->dt.v);
				SLL_ACQUIRE(*(_vm_stack+_vm_si));
				_vm_si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL:
				SLL_RELEASE(*(_vm_var_data+ai->dt.v));
				*(_vm_var_data+ai->dt.v)=SLL_ACQUIRE_STATIC_INT(0);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL:
				*(_vm_stack+_vm_si)=*(_vm_var_data+ai->dt.v);
				*(_vm_var_data+ai->dt.v)=SLL_ACQUIRE_STATIC_INT(0);
				_vm_si++;
				break;
		}
		ai++;
		if (ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			ai=ai->dt._p;
		}
		_vm_ii++;
		if (_vm_si>=sll_current_vm_config->s_sz){
			SLL_UNIMPLEMENTED();
		}
	}
}
