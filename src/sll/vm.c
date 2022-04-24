#include <sll/_internal/api_call.h>
#include <sll/_internal/barrier.h>
#include <sll/_internal/common.h>
#include <sll/_internal/dispatcher.h>
#include <sll/_internal/gc.h>
#include <sll/_internal/lock.h>
#include <sll/_internal/scheduler.h>
#include <sll/_internal/semaphore.h>
#include <sll/_internal/stack.h>
#include <sll/_internal/static_object.h>
#include <sll/_internal/thread.h>
#include <sll/_internal/vm.h>
#include <sll/api/file.h>
#include <sll/api/serial.h>
#include <sll/api/string.h>
#include <sll/array.h>
#include <sll/assembly.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/compression.h>
#include <sll/data.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/ift.h>
#include <sll/location.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/platform/memory.h>
#include <sll/sandbox.h>
#include <sll/scheduler.h>
#include <sll/static_object.h>
#include <sll/thread.h>
#include <sll/types.h>
#include <sll/vm.h>



#define JUMP_INSTRUCTION \
	thr->ii=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?thr->ii+ai->dt.j.t.rel:ai->dt.j.t.abs); \
	ai=ai->dt.j._p; \
	continue;
#define CMP_INSTRUCTION(eq,v) \
	{ \
		sll_compare_result_t cmp=sll_operator_compare(*(thr->stack+thr->si-2),*(thr->stack+thr->si-1)); \
		if ((cmp==v)==eq){ \
			GC_RELEASE(*(thr->stack+thr->si-2)); \
			GC_RELEASE(*(thr->stack+thr->si-1)); \
			thr->si-=2; \
			JUMP_INSTRUCTION; \
		} \
		break; \
	}
#define CMP_INSTRUCTION_FUNCTION(eq) \
	{ \
		if (eq(*(thr->stack+thr->si-2),*(thr->stack+thr->si-1))){ \
			GC_RELEASE(*(thr->stack+thr->si-2)); \
			GC_RELEASE(*(thr->stack+thr->si-1)); \
			thr->si-=2; \
			JUMP_INSTRUCTION; \
		} \
		break; \
	}
#define CMP_INSTRUCTION_BOOL(st) \
	{ \
		sll_bool_t b=sll_operator_bool(*(thr->stack+thr->si-1)); \
		if (b==st){ \
			thr->si--; \
			GC_RELEASE(*(thr->stack+thr->si)); \
			JUMP_INSTRUCTION; \
		} \
		break; \
	}

#define OPERATOR_INSTRUCTION_UNARY(nm) \
	{ \
		sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?VAR_REF(ai->dt.v):thr->stack+thr->si-1); \
		sll_object_t* n=nm(*tos); \
		GC_RELEASE(*tos); \
		*tos=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION_BINARY(nm) \
	{ \
		thr->si--; \
		sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?VAR_REF(ai->dt.v):thr->stack+thr->si-1); \
		sll_object_t* a=*(thr->stack+thr->si); \
		sll_object_t* n=nm(*tos,a); \
		GC_RELEASE(*tos); \
		GC_RELEASE(a); \
		*tos=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION_TERNARY(nm) \
	{ \
		thr->si-=2; \
		sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?VAR_REF(ai->dt.v):thr->stack+thr->si-1); \
		sll_object_t* a=*(thr->stack+thr->si); \
		sll_object_t* b=*(thr->stack+thr->si+1); \
		sll_object_t* n=nm(*tos,a,b); \
		GC_RELEASE(*tos); \
		GC_RELEASE(a); \
		GC_RELEASE(b); \
		*tos=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION_QUATERNARY(nm) \
	{ \
		thr->si-=3; \
		sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?VAR_REF(ai->dt.v):thr->stack+thr->si-1); \
		sll_object_t* a=*(thr->stack+thr->si); \
		sll_object_t* b=*(thr->stack+thr->si+1); \
		sll_object_t* c=*(thr->stack+thr->si+2); \
		sll_object_t* n=nm(*tos,a,b,c); \
		GC_RELEASE(*tos); \
		GC_RELEASE(a); \
		GC_RELEASE(b); \
		GC_RELEASE(c); \
		*tos=n; \
		break; \
	}

#define NEXT_INSTRUCTION \
	do{ \
		ai++; \
		if (ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){ \
			ai=ai->dt._p; \
		} \
	} while (0)
#define RELOAD_THREAD_DATA \
	do{ \
		if (_scheduler_current_thread_index==SLL_UNKNOWN_THREAD_INDEX){ \
			if (tid_dt->ret){ \
				goto _cleanup; \
			} \
			_scheduler_set_thread(_scheduler_queue_pop()); \
		} \
		thr=_scheduler_current_thread; \
		if (thr->c_st.l){ \
			func_var_off=ADDR((thr->c_st.dt+thr->c_st.l-1)->_var_mem_off); \
		} \
		else{ \
			func_var_off=ADDR(thr->stack)-ADDR(_vm_var_data+sll_current_runtime_data->a_dt->vc); \
		} \
		tls_var_off=ADDR(thr->tls)-ADDR(_vm_var_data); \
		if (thr->_last_ai){ \
			ai=thr->_last_ai; \
			thr->_last_ai=NULL; \
			SLL_ASSERT(ai==_get_instruction_at_offset(sll_current_runtime_data->a_dt,thr->ii)); \
		} \
		else{ \
			ai=_get_instruction_at_offset(sll_current_runtime_data->a_dt,thr->ii); \
		} \
	} while (0)
#define VAR_REF(v) ((sll_object_t**)PTR(ADDR(_vm_var_data+SLL_ASSEMBLY_VARIABLE_GET_INDEX(v))+(SLL_ASSEMBLY_VARIABLE_GET_INDEX(v)>=sll_current_runtime_data->a_dt->vc)*func_var_off+SLL_ASSEMBLY_VARIABLE_IS_TLS(v)*tls_var_off))



static sll_object_t** _vm_var_data=NULL;
static sll_size_t _vm_instruction_count=0;



__SLL_EXTERNAL const sll_runtime_data_t* sll_current_runtime_data=NULL;
__SLL_EXTERNAL const sll_vm_config_t* sll_current_vm_config=NULL;



void _call_function(thread_data_t* thr,sll_function_index_t fn,sll_arg_count_t ac,sll_bool_t fr){
	sll_assembly_function_t* af=sll_current_runtime_data->a_dt->ft.dt+fn;
	if (SLL_ASSEMBLY_FUNCTION_IS_VAR_ARG(af)){
		sll_object_t* tos;
		if (SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)>ac){
			sll_static_int[0]->rc+=SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)-ac-1;
			for (sll_arg_count_t i=0;i<SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)-ac-1;i++){
				*(thr->stack+thr->si)=sll_static_int[0];
				thr->si++;
			}
			tos=sll_array_to_object(NULL);
		}
		else{
			tos=sll_array_length_to_object(ac-SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)+1);
			thr->si-=tos->dt.a.l;
			for (sll_array_length_t i=0;i<tos->dt.a.l;i++){
				tos->dt.a.v[i]=*(thr->stack+thr->si+i);
			}
		}
		*(thr->stack+thr->si)=tos;
		thr->si++;
	}
	else{
		if (ac!=SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)){
			if (SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)>ac){
				sll_static_int[0]->rc+=SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)-ac;
				for (sll_arg_count_t i=0;i<SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)-ac;i++){
					*(thr->stack+thr->si+i)=sll_static_int[0];
				}
				thr->si+=SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)-ac;
			}
			else{
				for (sll_arg_count_t i=ac-SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af);i;i--){
					thr->si--;
					GC_RELEASE(*(thr->stack+thr->si));
				}
			}
		}
	}
	if (fr){
		SLL_ASSERT(thr->c_st.l<=sll_current_vm_config->c_st_sz);
		(thr->c_st.dt+thr->c_st.l)->nm=(sll_current_runtime_data->a_dt->st.dt+af->nm)->v;
		(thr->c_st.dt+thr->c_st.l)->_ii=thr->ii;
		(thr->c_st.dt+thr->c_st.l)->_s=thr->si-SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af);
		(thr->c_st.dt+thr->c_st.l)->_var_mem_off=PTR(ADDR(thr->stack+(thr->c_st.dt+thr->c_st.l)->_s)-ADDR(_vm_var_data+sll_current_runtime_data->a_dt->vc));
		(*((sll_call_stack_size_t*)(&(thr->c_st.l))))++;
	}
	thr->ii=af->i;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_code_t sll_execute_assembly(const sll_assembly_data_t* a_dt,const sll_vm_config_t* cfg){
	if (_vm_var_data){
		SLL_UNIMPLEMENTED();
	}
	_vm_instruction_count=0;
	sll_current_vm_config=cfg;// lgtm [cpp/stack-address-escape]
	_vm_var_data=sll_platform_allocate_page(SLL_ROUND_PAGE(a_dt->vc*sizeof(sll_object_t*)),0);
	sll_static_int[0]->rc+=a_dt->vc;
	for (sll_variable_index_t i=0;i<a_dt->vc;i++){
		*(_vm_var_data+i)=sll_static_int[0];
	}
	sll_internal_function_table_t ift;
	sll_clone_internal_function_table(cfg->ift,&ift);
	sll_object_type_table_t tt=SLL_INIT_OBJECT_TYPE_TABLE_STRUCT;
	sll_runtime_data_t r_dt={
		a_dt,
		&ift,
		&tt
	};
	sll_current_runtime_data=&r_dt;// lgtm [cpp/stack-address-escape]
	sll_audit(SLL_CHAR("sll.vm.init"),SLL_CHAR(""));
	sll_return_code_t rc=_scheduler_run();
	for (sll_variable_index_t i=0;i<a_dt->vc;i++){
		GC_RELEASE(*(_vm_var_data+i));
	}
	sll_platform_free_page(_vm_var_data,SLL_ROUND_PAGE(a_dt->vc*sizeof(sll_object_t*)));
	_vm_var_data=NULL;
	sll_current_runtime_data=NULL;
	sll_free_internal_function_table(&ift);
	sll_free_object_type_list(&tt);
	return rc;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_execute_function(sll_integer_t fn_idx,sll_object_t*const* al,sll_arg_count_t all,sll_flags_t fl){
	if (!sll_current_runtime_data||!sll_current_vm_config){
		return NULL;
	}
	sll_thread_index_t tid=sll_thread_create(fn_idx,al,all);
	if ((_scheduler_current_thread->flags&THREAD_FLAG_NO_AUDIT_TERMINATE)||(fl&EXECUTE_FUNCTION_NO_AUDIT_TERMINATE)){
		_thread_get(tid)->flags|=THREAD_FLAG_NO_AUDIT_TERMINATE;
	}
	else{
		sll_audit(SLL_CHAR("sll.thread.create"),SLL_CHAR("iO+u"),fn_idx,al,all,tid);
	}
	sll_object_t* o;
	if (fl&SLL_EXECUTE_FUNCTION_ASYNC){
		o=sll_wait_thread(tid);
	}
	else{
		sll_thread_index_t s_tid=_scheduler_current_thread_index;
		_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
		_scheduler_set_thread(tid);
		o=sll_wait_thread(tid);
		SLL_ASSERT(_scheduler_current_thread_index==SLL_UNKNOWN_THREAD_INDEX);
		if (s_tid!=SLL_UNKNOWN_THREAD_INDEX){
			_scheduler_set_thread(s_tid);
		}
	}
	if (!sll_thread_delete(tid)){
		SLL_UNREACHABLE();
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_vm_get_instruction_count(void){
	return _vm_instruction_count;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_wait_thread(sll_thread_index_t tid){
	sll_thread_index_t s_tid=_scheduler_current_thread_index;
	if (s_tid==SLL_UNKNOWN_THREAD_INDEX){
		_scheduler_set_thread(tid);
	}
	SLL_ASSERT(_thread_get(tid)==_scheduler_current_thread);
	thread_data_t* tid_dt=_scheduler_current_thread;
	if (_scheduler_current_thread->ret){
		goto _cleanup;
	}
	thread_data_t* thr=_scheduler_current_thread;
	const sll_assembly_instruction_t* ai;
	addr_t func_var_off;
	addr_t tls_var_off;
	RELOAD_THREAD_DATA;
	while (1){
		if (thr->ii>=sll_current_runtime_data->a_dt->ic||thr->si>=sll_current_vm_config->s_sz){
			SLL_UNIMPLEMENTED();
		}
		if (!thr->tm){
			thr->_last_ai=ai;
			_scheduler_queue_next();
			RELOAD_THREAD_DATA;
			if (_scheduler_current_thread_index!=tid&&tid_dt->ret){
				goto _cleanup;
			}
		}
		thr->tm--;
		_vm_instruction_count++;
		switch (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)){
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_POP:
				thr->si--;
				GC_RELEASE(*(thr->stack+thr->si));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO:
				GC_RELEASE(*(thr->stack+thr->si-1));
				thr->si-=2;
				GC_RELEASE(*(thr->stack+thr->si));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT:
				{
					sll_object_t* tos=*(thr->stack+thr->si-1);
					*(thr->stack+thr->si-1)=*(thr->stack+thr->si-2);
					*(thr->stack+thr->si-2)=tos;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP:
				GC_RELEASE(*(thr->stack+thr->si-2));
				thr->si--;
				*(thr->stack+thr->si-1)=*(thr->stack+thr->si);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DUP:
				*(thr->stack+thr->si)=*(thr->stack+thr->si-1);
				SLL_ACQUIRE(*(thr->stack+thr->si));
				thr->si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
				*(thr->stack+thr->si)=sll_int_to_object(ai->dt.i);
				thr->si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT_COMPRESSED:
				{
					sll_size_t dec=sll_decompress_integer(ai->dt.ci);
					*(thr->stack+thr->si)=sll_int_to_object(SLL_DECODE_SIGNED_INTEGER(dec));
					thr->si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE:
				*(thr->stack+thr->si)=SLL_ACQUIRE_STATIC_NEG_INT(1);
				thr->si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO:
				*(thr->stack+thr->si)=SLL_ACQUIRE_STATIC_INT(0);
				thr->si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE:
				*(thr->stack+thr->si)=SLL_ACQUIRE_STATIC_INT(1);
				thr->si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO:
				*(thr->stack+thr->si)=SLL_ACQUIRE_STATIC_INT(2);
				thr->si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE:
				*(thr->stack+thr->si)=SLL_ACQUIRE_STATIC_INT(3);
				thr->si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR:
				*(thr->stack+thr->si)=SLL_ACQUIRE_STATIC_INT(4);
				thr->si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
				*(thr->stack+thr->si)=sll_float_to_object(ai->dt.f);
				thr->si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_COMPLEX:
				*(thr->stack+thr->si)=sll_complex_to_object(&(ai->dt.d));
				thr->si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
				*(thr->stack+thr->si)=SLL_FROM_CHAR(ai->dt.c);
				thr->si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL:
				*(thr->stack+thr->si)=sll_int_to_object((SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?thr->ii+ai->dt.j.t.rel:ai->dt.j.t.abs));
				thr->si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_STACK:
				sll_static_int[0]->rc+=ai->dt.so;
				for (sll_stack_offset_t i=0;i<ai->dt.so;i++){
					*(thr->stack+thr->si)=sll_static_int[0];
					thr->si++;
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD:
				*(thr->stack+thr->si)=*VAR_REF(ai->dt.v);
				SLL_ACQUIRE(*(thr->stack+thr->si));
				thr->si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
				*(thr->stack+thr->si)=STRING_TO_OBJECT(sll_current_runtime_data->a_dt->st.dt+ai->dt.s);
				thr->si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK:
				{
					sll_object_t* tos=sll_array_length_to_object(ai->dt.al);
					thr->si-=ai->dt.al;
					sll_copy_data(thr->stack+thr->si,ai->dt.al*sizeof(sll_object_t*),tos->dt.a.v);
					*(thr->stack+thr->si)=tos;
					thr->si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO:
				*(thr->stack+thr->si)=sll_array_to_object(NULL);
				thr->si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ONE:
				{
					sll_object_t* tos=sll_array_length_to_object(1);
					tos->dt.a.v[0]=*(thr->stack+thr->si-1);
					*(thr->stack+thr->si-1)=tos;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP:
				{
					sll_object_t* tos=sll_map_length_to_object((ai->dt.ml+1)>>1);
					sll_map_t* m=&(tos->dt.m);
					thr->si-=ai->dt.ml;
					for (sll_map_length_t i=0;i<ai->dt.ml;i++){
						m->v[i]=*(thr->stack+thr->si+i);
					}
					if (ai->dt.ml&1){
						m->v[ai->dt.ml]=SLL_ACQUIRE_STATIC_INT(0);
					}
					*(thr->stack+thr->si)=tos;
					thr->si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP_ZERO:
				*(thr->stack+thr->si)=sll_map_to_object(NULL);
				thr->si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE:
				GC_RELEASE(*VAR_REF(ai->dt.v));
				*VAR_REF(ai->dt.v)=*(thr->stack+thr->si-1);
				SLL_ACQUIRE(*(thr->stack+thr->si-1));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP:
				GC_RELEASE(*VAR_REF(ai->dt.v));
				thr->si--;
				*VAR_REF(ai->dt.v)=*(thr->stack+thr->si);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE:
				GC_RELEASE(*VAR_REF(ai->dt.v));
				*VAR_REF(ai->dt.v)=SLL_ACQUIRE_STATIC_NEG_INT(1);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ZERO:
				GC_RELEASE(*VAR_REF(ai->dt.v));
				*VAR_REF(ai->dt.v)=SLL_ACQUIRE_STATIC_INT(0);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ONE:
				GC_RELEASE(*VAR_REF(ai->dt.v));
				*VAR_REF(ai->dt.v)=SLL_ACQUIRE_STATIC_INT(1);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_TWO:
				GC_RELEASE(*VAR_REF(ai->dt.v));
				*VAR_REF(ai->dt.v)=SLL_ACQUIRE_STATIC_INT(2);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_THREE:
				GC_RELEASE(*VAR_REF(ai->dt.v));
				*VAR_REF(ai->dt.v)=SLL_ACQUIRE_STATIC_INT(3);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_FOUR:
				GC_RELEASE(*VAR_REF(ai->dt.v));
				*VAR_REF(ai->dt.v)=SLL_ACQUIRE_STATIC_INT(4);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP:
				{
					sll_object_t* n=sll_operator_cast(*(thr->stack+thr->si-1),sll_static_int[SLL_OBJECT_TYPE_STRING]);
					GC_RELEASE(*(thr->stack+thr->si-1));
					sll_function_index_t i=sll_lookup_internal_function(sll_current_runtime_data->ift,n->dt.s.v);
					GC_RELEASE(n);
					*(thr->stack+thr->si-1)=(i==SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX?SLL_ACQUIRE_STATIC_INT(0):sll_int_to_object(~((sll_integer_t)i)));
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR:
				{
					sll_function_index_t i=sll_lookup_internal_function(sll_current_runtime_data->ift,(sll_current_runtime_data->a_dt->st.dt+ai->dt.s)->v);
					*(thr->stack+thr->si)=(i==SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX?SLL_ACQUIRE_STATIC_INT(0):sll_int_to_object(~((sll_integer_t)i)));
					thr->si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP:
				JUMP_INSTRUCTION;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JB:
				CMP_INSTRUCTION(1,SLL_COMPARE_RESULT_BELOW);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:
				CMP_INSTRUCTION(0,SLL_COMPARE_RESULT_ABOVE);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JA:
				CMP_INSTRUCTION(1,SLL_COMPARE_RESULT_ABOVE);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:
				CMP_INSTRUCTION(0,SLL_COMPARE_RESULT_BELOW);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JE:
				CMP_INSTRUCTION_FUNCTION(sll_operator_equal);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:
				CMP_INSTRUCTION_FUNCTION(!sll_operator_equal);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:
				CMP_INSTRUCTION_BOOL(0);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ:
				CMP_INSTRUCTION_BOOL(1);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JSE:
				CMP_INSTRUCTION_FUNCTION(sll_operator_strict_equal);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JSNE:
				CMP_INSTRUCTION_FUNCTION(!sll_operator_strict_equal);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JI:
				CMP_INSTRUCTION_FUNCTION(sll_operator_includes);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNI:
				CMP_INSTRUCTION_FUNCTION(!sll_operator_includes);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JT:
				{
					thr->si--;
					sll_stack_offset_t e_si=thr->si;
					thr->si-=ai->dt.al<<1;
					sll_stack_offset_t i=thr->si-1;
					sll_object_t* cnd=*(thr->stack+thr->si-1);
					sll_integer_t cnd_v;
					if (cnd->t==SLL_OBJECT_TYPE_INT){
						cnd_v=cnd->dt.i;
					}
					else if (cnd->t==SLL_OBJECT_TYPE_CHAR){
						cnd_v=cnd->dt.c;
					}
					else{
						GC_RELEASE(cnd);
						while (thr->si<e_si){
							GC_RELEASE(*(thr->stack+thr->si));
							thr->si++;
						}
						goto _jump_to_default;
					}
					GC_RELEASE(cnd);
					while (thr->si<e_si){
						sll_object_t* k=*(thr->stack+thr->si);
						if ((k->t==SLL_OBJECT_TYPE_INT&&k->dt.i==cnd_v)||(k->t==SLL_OBJECT_TYPE_CHAR&&k->dt.c==cnd_v)){
							goto _cleanup_jump_table;
						}
						GC_RELEASE(k);
						GC_RELEASE(*(thr->stack+thr->si+1));
						thr->si+=2;
					}
_jump_to_default:
					SLL_ASSERT(thr->si==e_si);
					thr->ii=(sll_instruction_index_t)((*(thr->stack+thr->si))->dt.i);
					ai=_get_instruction_at_offset(sll_current_runtime_data->a_dt,thr->ii);
					GC_RELEASE(*(thr->stack+thr->si));
					thr->si=i;
					continue;
_cleanup_jump_table:;
					thr->ii=(sll_instruction_index_t)((*(thr->stack+thr->si+1))->dt.i);
					ai=_get_instruction_at_offset(sll_current_runtime_data->a_dt,thr->ii);
					while (thr->si<e_si){
						GC_RELEASE(*(thr->stack+thr->si));
						thr->si++;
					}
					GC_RELEASE(*(thr->stack+thr->si));
					thr->si=i;
					continue;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?VAR_REF(ai->dt.v):thr->stack+thr->si-1);
					sll_object_t* n=sll_static_int[!sll_operator_bool(*tos)];
					SLL_ACQUIRE(n);
					GC_RELEASE(*tos);
					*tos=n;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_BOOL:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?VAR_REF(ai->dt.v):thr->stack+thr->si-1);
					sll_object_t* n=sll_static_int[sll_operator_bool(*tos)];
					SLL_ACQUIRE(n);
					GC_RELEASE(*tos);
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
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DEEP_COPY:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?VAR_REF(ai->dt.v):thr->stack+thr->si-1);
					sll_object_t* n=sll_operator_copy(*tos,SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_DEEP_COPY);
					GC_RELEASE(*tos);
					*tos=n;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_access);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_TWO:
				OPERATOR_INSTRUCTION_TERNARY(sll_operator_access_range);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_THREE:
				OPERATOR_INSTRUCTION_QUATERNARY(sll_operator_access_range_step);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_VAR:
				{
					sll_stack_offset_t off=ai->dt.va.l-1;
					sll_object_t* d=*VAR_REF(ai->dt.va.v);
					SLL_ACQUIRE(d);
					sll_arg_count_t l=ai->dt.va.l;
					sll_object_t*const* p=thr->stack+thr->si-off-1;
					do{
						SLL_ASSERT((*p)->t==SLL_OBJECT_TYPE_STRING);
						sll_object_t* nd=sll_operator_access(d,*p);
						GC_RELEASE(d);
						GC_RELEASE(*p);
						d=nd;
						p++;
						l--;
					} while (l);
					thr->si-=off;
					*(thr->stack+thr->si-1)=d;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN:
				sll_operator_assign((SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)?*VAR_REF(ai->dt.v):*(thr->stack+thr->si-3)),*(thr->stack+thr->si-2),*(thr->stack+thr->si-1));
				GC_RELEASE(*(thr->stack+thr->si-2));
				GC_RELEASE(*(thr->stack+thr->si-1));
				thr->si-=2;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_TWO:
				SLL_UNIMPLEMENTED();
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_THREE:
				SLL_UNIMPLEMENTED();
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR_ACCESS:
				{
					sll_object_t* nv=*(thr->stack+thr->si-1);
					sll_stack_offset_t off=ai->dt.va.l+1;
					sll_object_t* d=*VAR_REF(ai->dt.va.v);
					SLL_ACQUIRE(d);
					sll_arg_count_t l=ai->dt.va.l-1;
					sll_object_t*const* p=thr->stack+thr->si-off;
					while (l){
						SLL_ASSERT((*p)->t==SLL_OBJECT_TYPE_STRING);
						sll_object_t* nd=sll_operator_access(d,*p);
						GC_RELEASE(d);
						GC_RELEASE(*p);
						d=nd;
						p++;
						l--;
					}
					SLL_ASSERT((*p)->t==SLL_OBJECT_TYPE_STRING);
					sll_operator_assign(d,*p,nv);
					GC_RELEASE(d);
					GC_RELEASE(*p);
					GC_RELEASE(nv);
					thr->si-=off;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_cast);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?VAR_REF(ai->dt.v):thr->stack+thr->si-1);
					sll_object_t* t=sll_int_to_object(ai->dt.t);
					sll_object_t* n=sll_operator_cast(*tos,t);
					GC_RELEASE(*tos);
					GC_RELEASE(t);
					*tos=n;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_TYPEOF:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?VAR_REF(ai->dt.v):thr->stack+thr->si-1);
					sll_object_t* n=sll_int_to_object((*tos)->t);
					GC_RELEASE(*tos);
					*tos=n;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NAMEOF:
				{
					sll_object_t* tos=sll_create_object(SLL_OBJECT_TYPE_STRING);
					sll_get_name(*(thr->stack+thr->si-1),&(tos->dt.s));
					GC_RELEASE(*(thr->stack+thr->si-1));
					*(thr->stack+thr->si-1)=tos;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NAMEOF_TYPE:
				{
					sll_object_t* t=sll_operator_cast(*(thr->stack+thr->si-1),sll_static_int[SLL_OBJECT_TYPE_INT]);
					GC_RELEASE(*(thr->stack+thr->si-1));
					sll_object_t* tos=sll_create_object(SLL_OBJECT_TYPE_STRING);
					sll_get_type_name(sll_current_runtime_data->tt,(t->dt.i<0||t->dt.i>sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE?SLL_OBJECT_TYPE_INT:(sll_object_type_t)(t->dt.i)),&(tos->dt.s));
					GC_RELEASE(t);
					*(thr->stack+thr->si-1)=tos;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL:
				{
					sll_stack_offset_t off=ai->dt.ac<<1;
					sll_object_type_t nt;
					if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_ANONYMOUS(ai)){
						nt=sll_add_type(sll_current_runtime_data->tt,thr->stack+thr->si-off,ai->dt.ac,NULL);
						for (sll_arg_count_t i=0;i<off;i++){
							thr->si--;
							GC_RELEASE(*(thr->stack+thr->si));
						}
						thr->si++;
					}
					else{
						SLL_ASSERT((*(thr->stack+thr->si-off-1))->t==SLL_OBJECT_TYPE_STRING);
						nt=sll_add_type(sll_current_runtime_data->tt,thr->stack+thr->si-off,ai->dt.ac,&((*(thr->stack+thr->si-off-1))->dt.s));
						for (sll_arg_count_t i=0;i<off;i++){
							thr->si--;
							GC_RELEASE(*(thr->stack+thr->si));
						}
						GC_RELEASE(*(thr->stack+thr->si-1));
					}
					*(thr->stack+thr->si-1)=sll_int_to_object(nt);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL_ZERO:
				{
					if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_ANONYMOUS(ai)){
						*(thr->stack+thr->si)=sll_int_to_object(sll_add_type(sll_current_runtime_data->tt,NULL,0,NULL));
						thr->si++;
					}
					else{
						SLL_ASSERT((*(thr->stack+thr->si-1))->t==SLL_OBJECT_TYPE_STRING);
						sll_object_type_t nt=sll_add_type(sll_current_runtime_data->tt,NULL,0,&((*(thr->stack+thr->si-1))->dt.s));
						GC_RELEASE(*(thr->stack+thr->si-1));
						*(thr->stack+thr->si-1)=sll_int_to_object(nt);
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW:
				{
					sll_object_t* t=sll_operator_cast(*(thr->stack+thr->si-ai->dt.ac-1),sll_static_int[SLL_OBJECT_TYPE_INT]);
					GC_RELEASE(*(thr->stack+thr->si-ai->dt.ac-1));
					sll_object_type_t ot=(t->dt.i<0||t->dt.i>sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE?SLL_OBJECT_TYPE_INT:(sll_object_type_t)(t->dt.i));
					GC_RELEASE(t);
					*(thr->stack+thr->si-ai->dt.ac-1)=sll_create_object_type(sll_current_runtime_data->tt,ot,thr->stack+thr->si-ai->dt.ac,ai->dt.ac);
					for (sll_arg_count_t i=0;i<ai->dt.ac;i++){
						thr->si--;
						GC_RELEASE(*(thr->stack+thr->si));
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW_DECL:
				*(thr->stack+thr->si)=sll_create_new_object_type(sll_current_runtime_data->tt);
				thr->si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR:
				*(thr->stack+thr->si)=*VAR_REF(ai->dt.v);
				SLL_ACQUIRE(*(thr->stack+thr->si));
				thr->si++;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT:
				{
					thr->si--;
					sll_object_t* tos=*(thr->stack+thr->si);
					if (!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)||sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO)){
						if (tos->t==SLL_OBJECT_TYPE_STRING){
							sll_file_write(sll_current_vm_config->out,tos->dt.s.v,tos->dt.s.l*sizeof(sll_char_t),NULL);
						}
						else{
							sll_string_t str;
							sll_api_string_convert(&tos,1,&str);
							sll_file_write(sll_current_vm_config->out,str.v,str.l*sizeof(sll_char_t),NULL);
							sll_free_string(&str);
						}
					}
					GC_RELEASE(tos);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
				if (!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)||sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO)){
					sll_file_write_char(sll_current_vm_config->out,ai->dt.c,NULL);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
				if (!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)||sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO)){
					sll_file_write(sll_current_vm_config->out,(sll_current_runtime_data->a_dt->st.dt+ai->dt.s)->v,(sll_current_runtime_data->a_dt->st.dt+ai->dt.s)->l*sizeof(sll_char_t),NULL);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP:
				{
					thr->si--;
					sll_object_t* tos=*(thr->stack+thr->si);
					if (tos->t==SLL_OBJECT_TYPE_INT){
						sll_integer_t i=tos->dt.i;
						GC_RELEASE(tos);
						if (i<0){
							sll_function_index_t j=(sll_function_index_t)(~i);
							if (j<sll_current_runtime_data->ift->l){
								sll_object_t* n=_call_api_func(j,thr->stack+thr->si-ai->dt.ac,ai->dt.ac);
								for (sll_arg_count_t k=0;k<ai->dt.ac;k++){
									thr->si--;
									GC_RELEASE(*(thr->stack+thr->si));
								}
								if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)!=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP){
									*(thr->stack+thr->si)=n;
									thr->si++;
								}
								else{
									GC_RELEASE(n);
								}
								break;
							}
						}
						else if (i&&i<=sll_current_runtime_data->a_dt->ft.l){
							_call_function(thr,(sll_function_index_t)(i-1),ai->dt.ac,1);
							RELOAD_THREAD_DATA;
							continue;
						}
					}
					else{
						GC_RELEASE(tos);
					}
					for (sll_arg_count_t i=0;i<ai->dt.ac;i++){
						thr->si--;
						GC_RELEASE(*(thr->stack+thr->si));
					}
					if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)!=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP){
						*(thr->stack+thr->si)=SLL_ACQUIRE_STATIC_INT(0);
						thr->si++;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO:
				if (ai->dt.i<0){
					sll_function_index_t i=(sll_function_index_t)(~ai->dt.i);
					if (i<sll_current_runtime_data->ift->l){
						sll_object_t* n=_call_api_func(i,NULL,0);
						*(thr->stack+thr->si)=n;
						thr->si++;
						break;
					}
				}
				else if (ai->dt.i&&ai->dt.i<=sll_current_runtime_data->a_dt->ft.l){
					_call_function(thr,(sll_function_index_t)(ai->dt.i-1),0,1);
					RELOAD_THREAD_DATA;
					continue;
				}
				*(thr->stack+thr->si)=SLL_ACQUIRE_STATIC_INT(0);
				thr->si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:
				if (ai->dt.i<0){
					sll_function_index_t i=(sll_function_index_t)(~ai->dt.i);
					if (i<sll_current_runtime_data->ift->l){
						sll_object_t* n=_call_api_func(i,thr->stack+thr->si-1,1);
						GC_RELEASE(*(thr->stack+thr->si-1));
						*(thr->stack+thr->si-1)=n;
						break;
					}
				}
				else if (ai->dt.i&&ai->dt.i<=sll_current_runtime_data->a_dt->ft.l){
					_call_function(thr,(sll_function_index_t)(ai->dt.i-1),1,1);
					RELOAD_THREAD_DATA;
					continue;
				}
				GC_RELEASE(*(thr->stack+thr->si-1));
				*(thr->stack+thr->si-1)=SLL_ACQUIRE_STATIC_INT(0);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ARRAY:
				{
					thr->si--;
					sll_object_t* tos=*(thr->stack+thr->si);
					if (tos->t==SLL_OBJECT_TYPE_INT){
						sll_integer_t i=tos->dt.i;
						GC_RELEASE(tos);
						tos=sll_operator_cast(*(thr->stack+thr->si-1),sll_static_int[SLL_OBJECT_TYPE_ARRAY]);
						if (i<0){
							sll_function_index_t j=(sll_function_index_t)(~i);
							if (j<sll_current_runtime_data->ift->l){
								GC_RELEASE(*(thr->stack+thr->si-1));
								sll_object_t* n=_call_api_func(j,tos->dt.a.v,tos->dt.a.l);
								GC_RELEASE(tos);
								*(thr->stack+thr->si-1)=n;
								break;
							}
						}
						else if (i&&i<=sll_current_runtime_data->a_dt->ft.l){
							thr->si--;
							GC_RELEASE(*(thr->stack+thr->si));
							for (sll_array_length_t j=0;j<tos->dt.a.l;j++){
								*(thr->stack+thr->si)=tos->dt.a.v[j];
								thr->si++;
								SLL_ACQUIRE(tos->dt.a.v[j]);
							}
							GC_RELEASE(tos);
							_call_function(thr,(sll_function_index_t)(i-1),ai->dt.ac,1);
							RELOAD_THREAD_DATA;
							continue;
						}
						GC_RELEASE(tos);
					}
					else{
						GC_RELEASE(tos);
					}
					GC_RELEASE(*(thr->stack+thr->si-1));
					*(thr->stack+thr->si)=SLL_ACQUIRE_STATIC_INT(0);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_REF:
				{
					sll_integer_t a=(sll_integer_t)ADDR(*(thr->stack+thr->si-1));
					GC_RELEASE(*(thr->stack+thr->si-1));
					*(thr->stack+thr->si-1)=sll_int_to_object(a);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET:
_return:;
				{
					sll_object_t* tmp=*(thr->stack+thr->si-1);
					SLL_ACQUIRE(tmp);
					if (!thr->c_st.l){
						while (thr->si){
							thr->si--;
							GC_RELEASE(*(thr->stack+thr->si));
						}
						sll_thread_index_t idx=_scheduler_current_thread_index;
						_thread_terminate(tmp);
						GC_RELEASE(tmp);
						if (idx==tid){
							goto _cleanup;
						}
						RELOAD_THREAD_DATA;
						continue;
					}
					(*((sll_call_stack_size_t*)(&(thr->c_st.l))))--;
					thr->ii=(thr->c_st.dt+thr->c_st.l)->_ii;
					sll_stack_offset_t n_si=(thr->c_st.dt+thr->c_st.l)->_s;
					SLL_ASSERT(thr->si>=n_si);
					while (thr->si>n_si){
						thr->si--;
						GC_RELEASE(*(thr->stack+thr->si));
					}
					RELOAD_THREAD_DATA;
					if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)!=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP){
						*(thr->stack+thr->si)=tmp;
						thr->si++;
					}
					else{
						GC_RELEASE(tmp);
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO:
				*(thr->stack+thr->si)=SLL_ACQUIRE_STATIC_INT(0);
				thr->si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				*(thr->stack+thr->si)=sll_int_to_object(ai->dt.i);
				thr->si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				*(thr->stack+thr->si)=sll_float_to_object(ai->dt.f);
				thr->si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				*(thr->stack+thr->si)=SLL_FROM_CHAR(ai->dt.c);
				thr->si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				*(thr->stack+thr->si)=STRING_TO_OBJECT(sll_current_runtime_data->a_dt->st.dt+ai->dt.s);
				thr->si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR:
				*(thr->stack+thr->si)=*VAR_REF(ai->dt.v);
				SLL_ACQUIRE(*(thr->stack+thr->si));
				thr->si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL:
				GC_RELEASE(*VAR_REF(ai->dt.v));
				*VAR_REF(ai->dt.v)=SLL_ACQUIRE_STATIC_INT(0);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL:
				*(thr->stack+thr->si)=*VAR_REF(ai->dt.v);
				*VAR_REF(ai->dt.v)=SLL_ACQUIRE_STATIC_INT(0);
				thr->si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_WAIT:
				{
					sll_object_t* n_tid_o=sll_operator_cast(*(thr->stack+thr->si-1),sll_static_int[SLL_OBJECT_TYPE_INT]);
					thr->si--;
					GC_RELEASE(*(thr->stack+thr->si));
					sll_integer_t w_tid=n_tid_o->dt.i;
					GC_RELEASE(n_tid_o);
					thread_data_t* c_thr=thr;
					if (!_thread_wait(w_tid)){
						break;
					}
					c_thr->ii++;
					NEXT_INSTRUCTION;
					c_thr->_last_ai=ai;
_load_new_thread:;
					sll_thread_index_t n_tid=_scheduler_queue_pop();
					if (n_tid==SLL_UNKNOWN_THREAD_INDEX){
						if (tid_dt->ret){
							goto _cleanup;
						}
						SLL_UNIMPLEMENTED();
					}
					_scheduler_set_thread(n_tid);
					RELOAD_THREAD_DATA;
					continue;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_ID:
				*(thr->stack+thr->si)=sll_int_to_object(_scheduler_current_thread_index);
				thr->si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_LOCK:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_SEMAPHORE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_BARRIER_EQ:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_BARRIER_GEQ:
				{
					sll_integer_t v=0;
					if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)>SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_SEMAPHORE){
						sll_object_t* v_o=sll_operator_cast(*(thr->stack+thr->si-1),sll_static_int[SLL_OBJECT_TYPE_INT]);
						thr->si--;
						GC_RELEASE(*(thr->stack+thr->si));
						v=v_o->dt.i;
						GC_RELEASE(v_o);
					}
					sll_object_t* lck_o=sll_operator_cast(*(thr->stack+thr->si-1),sll_static_int[SLL_OBJECT_TYPE_INT]);
					thr->si--;
					GC_RELEASE(*(thr->stack+thr->si));
					sll_integer_t lck=lck_o->dt.i;
					GC_RELEASE(lck_o);
					thread_data_t* c_thr=thr;
					sll_bool_t wait=0;
					switch (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)){
						case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_LOCK:
							wait=_lock_wait(lck);
							break;
						case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_SEMAPHORE:
							wait=_semaphore_wait(lck);
							break;
						case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_BARRIER_EQ:
						case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_BARRIER_GEQ:
							wait=_barrier_wait(lck,v,SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_BARRIER_GEQ);
							break;
					}
					if (!wait){
						break;
					}
					c_thr->ii++;
					NEXT_INSTRUCTION;
					c_thr->_last_ai=ai;
					goto _load_new_thread;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_READ_BLOCKING:
				{
					sll_object_t* sz_o=sll_operator_cast(*(thr->stack+thr->si-1),sll_static_int[SLL_OBJECT_TYPE_INT]);
					thr->si--;
					GC_RELEASE(*(thr->stack+thr->si));
					sll_integer_t sz=sz_o->dt.i;
					GC_RELEASE(sz_o);
					sll_object_t* fh_o=sll_operator_cast(*(thr->stack+thr->si-1),sll_static_int[SLL_OBJECT_TYPE_INT]);
					thr->si--;
					GC_RELEASE(*(thr->stack+thr->si));
					sll_file_t* f=sll_file_from_handle(fh_o->dt.i);
					GC_RELEASE(fh_o);
					if (!f||sz<=0){
						*(thr->stack+thr->si)=STRING_TO_OBJECT(NULL);
						thr->si++;
						break;
					}
					sll_string_length_t l=(sz>SLL_MAX_STRING_LENGTH?SLL_MAX_STRING_LENGTH:(sll_string_length_t)sz);
					if ((f->f&SLL_FILE_FLAG_ASYNC)&&!sll_file_data_available(f)){
						thr->ii++;
						NEXT_INSTRUCTION;
						thr->_last_ai=ai;
						_io_dispatcher_queue(f,l);
						goto _load_new_thread;
					}
					sll_string_t bf;
					sll_string_create(l,&bf);
					sll_error_t err;
					sll_size_t r_sz=sll_file_read(f,bf.v,l,&err);
					if (!r_sz&&err!=SLL_NO_ERROR){
						sll_free_string(&bf);
						*(thr->stack+thr->si)=sll_int_to_object(err);
					}
					else{
						sll_string_decrease(&bf,(sll_string_length_t)r_sz);
						sll_string_calculate_checksum(&bf);
						*(thr->stack+thr->si)=STRING_TO_OBJECT_NOCOPY(&bf);
					}
					thr->si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_READ_BLOCKING_CHAR:
				{
					sll_object_t* fh_o=sll_operator_cast(*(thr->stack+thr->si-1),sll_static_int[SLL_OBJECT_TYPE_INT]);
					thr->si--;
					GC_RELEASE(*(thr->stack+thr->si));
					sll_file_t* f=sll_file_from_handle(fh_o->dt.i);
					GC_RELEASE(fh_o);
					if (!f){
						*(thr->stack+thr->si)=SLL_FROM_CHAR(0);
						thr->si++;
						break;
					}
					if ((f->f&SLL_FILE_FLAG_ASYNC)&&!sll_file_data_available(f)){
						thr->ii++;
						NEXT_INSTRUCTION;
						thr->_last_ai=ai;
						_io_dispatcher_queue(f,0);
						goto _load_new_thread;
					}
					sll_error_t err;
					sll_read_char_t chr=sll_file_read_char(f,&err);
					*(thr->stack+thr->si)=(chr==SLL_END_OF_DATA?(err==SLL_NO_ERROR?SLL_ACQUIRE_STATIC_INT(0):sll_int_to_object(~err)):SLL_FROM_CHAR(chr));
					thr->si++;
					break;
				}
			default:
				SLL_UNREACHABLE();
		}
		NEXT_INSTRUCTION;
		thr->ii++;
	}
_cleanup:
	if (s_tid!=SLL_UNKNOWN_THREAD_INDEX){
		_scheduler_set_thread(s_tid);
	}
	sll_object_t* ret=tid_dt->ret;
	SLL_ACQUIRE(ret);
	return ret;
}
