#include <sll/_internal/api_call.h>
#include <sll/_internal/barrier.h>
#include <sll/_internal/common.h>
#include <sll/_internal/dispatcher.h>
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
#include <sll/platform/process.h>
#include <sll/sandbox.h>
#include <sll/scheduler.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/thread.h>
#include <sll/types.h>
#include <sll/vm.h>



#define JUMP_INSTRUCTION \
	thr->instruction_index=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?thr->instruction_index+ai->data.jump.target.rel:ai->data.jump.target.abs); \
	ai=ai->data.jump._instruction; \
	continue;
#define CMP_INSTRUCTION(eq,v) \
	{ \
		sll_compare_result_t cmp=sll_operator_compare(*(thr->stack+thr->stack_index-2),*(thr->stack+thr->stack_index-1)); \
		if ((cmp==v)==eq){ \
			SLL_RELEASE(*(thr->stack+thr->stack_index-2)); \
			*(thr->stack+thr->stack_index-2)=NULL; \
			SLL_RELEASE(*(thr->stack+thr->stack_index-1)); \
			*(thr->stack+thr->stack_index-1)=NULL; \
			thr->stack_index-=2; \
			JUMP_INSTRUCTION; \
		} \
		break; \
	}
#define CMP_INSTRUCTION_FUNCTION(eq) \
	{ \
		if (eq(*(thr->stack+thr->stack_index-2),*(thr->stack+thr->stack_index-1))){ \
			SLL_RELEASE(*(thr->stack+thr->stack_index-2)); \
			*(thr->stack+thr->stack_index-2)=NULL; \
			SLL_RELEASE(*(thr->stack+thr->stack_index-1)); \
			*(thr->stack+thr->stack_index-1)=NULL; \
			thr->stack_index-=2; \
			JUMP_INSTRUCTION; \
		} \
		break; \
	}
#define CMP_INSTRUCTION_BOOL(st) \
	{ \
		sll_bool_t b=sll_operator_bool(*(thr->stack+thr->stack_index-1)); \
		if (b==st){ \
			thr->stack_index--; \
			SLL_RELEASE(*(thr->stack+thr->stack_index)); \
			*(thr->stack+thr->stack_index)=NULL; \
			JUMP_INSTRUCTION; \
		} \
		break; \
	}

#define OPERATOR_INSTRUCTION_UNARY(nm) \
	{ \
		sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?VAR_REF(ai->data.variable):thr->stack+thr->stack_index-1); \
		sll_object_t* n=nm(*tos); \
		SLL_RELEASE(*tos); \
		*tos=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION_BINARY(nm) \
	{ \
		thr->stack_index--; \
		sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?VAR_REF(ai->data.variable):thr->stack+thr->stack_index-1); \
		sll_object_t* a=*(thr->stack+thr->stack_index); \
		sll_gc_add_root(a); \
		*(thr->stack+thr->stack_index)=NULL; \
		sll_object_t* n=nm(*tos,a); \
		SLL_RELEASE(*tos); \
		sll_gc_remove_root(a); \
		SLL_RELEASE(a); \
		*tos=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION_TERNARY(nm) \
	{ \
		thr->stack_index-=2; \
		sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?VAR_REF(ai->data.variable):thr->stack+thr->stack_index-1); \
		sll_object_t* a=*(thr->stack+thr->stack_index); \
		sll_gc_add_root(a); \
		*(thr->stack+thr->stack_index)=NULL; \
		sll_object_t* b=*(thr->stack+thr->stack_index+1); \
		sll_gc_add_root(b); \
		*(thr->stack+thr->stack_index+1)=NULL; \
		sll_object_t* n=nm(*tos,a,b); \
		SLL_RELEASE(*tos); \
		sll_gc_remove_root(a); \
		SLL_RELEASE(a); \
		sll_gc_remove_root(b); \
		SLL_RELEASE(b); \
		*tos=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION_QUATERNARY(nm) \
	{ \
		thr->stack_index-=3; \
		sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?VAR_REF(ai->data.variable):thr->stack+thr->stack_index-1); \
		sll_object_t* a=*(thr->stack+thr->stack_index); \
		sll_gc_add_root(a); \
		*(thr->stack+thr->stack_index)=NULL; \
		sll_object_t* b=*(thr->stack+thr->stack_index+1); \
		sll_gc_add_root(b); \
		*(thr->stack+thr->stack_index+1)=NULL; \
		sll_object_t* c=*(thr->stack+thr->stack_index+2); \
		sll_gc_add_root(c); \
		*(thr->stack+thr->stack_index+2)=NULL; \
		sll_object_t* n=nm(*tos,a,b,c); \
		SLL_RELEASE(*tos); \
		sll_gc_remove_root(a); \
		SLL_RELEASE(a); \
		sll_gc_remove_root(b); \
		SLL_RELEASE(b); \
		sll_gc_remove_root(c); \
		SLL_RELEASE(c); \
		*tos=n; \
		break; \
	}

#define NEXT_INSTRUCTION \
	do{ \
		ai++; \
		if (ai->type==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){ \
			ai=ai->data._next_instruction; \
		} \
	} while (0)
#define RELOAD_THREAD_DATA \
	do{ \
		if (_scheduler_current_thread_index==SLL_UNKNOWN_THREAD_INDEX){ \
			if (tid_dt->return_value){ \
				goto _cleanup; \
			} \
			_scheduler_set_thread(_scheduler_queue_pop()); \
		} \
		thr=_scheduler_current_thread; \
		if (thr->call_stack.length){ \
			func_var_off=ADDR((thr->call_stack.data+thr->call_stack.length-1)->_variable_memory_offset); \
		} \
		else{ \
			func_var_off=ADDR(thr->stack)-ADDR(_vm_var_data+sll_current_runtime_data->assembly_data->variable_count); \
		} \
		tls_var_off=ADDR(thr->tls)-ADDR(_vm_var_data); \
		if (thr->_last_instruction){ \
			ai=thr->_last_instruction; \
			thr->_last_instruction=NULL; \
			SLL_ASSERT(ai==_get_instruction_at_offset(sll_current_runtime_data->assembly_data,thr->instruction_index)); \
		} \
		else{ \
			ai=_get_instruction_at_offset(sll_current_runtime_data->assembly_data,thr->instruction_index); \
		} \
	} while (0)
#define VAR_REF(v) ((sll_object_t**)PTR(ADDR(_vm_var_data+SLL_ASSEMBLY_VARIABLE_GET_INDEX(v))+(SLL_ASSEMBLY_VARIABLE_GET_INDEX(v)>=sll_current_runtime_data->assembly_data->variable_count)*func_var_off+SLL_ASSEMBLY_VARIABLE_IS_TLS(v)*tls_var_off))



static sll_object_t** _vm_var_data=NULL;
static sll_size_t _vm_instruction_count=0;



__SLL_EXTERNAL const sll_runtime_data_t* sll_current_runtime_data=NULL;
__SLL_EXTERNAL const sll_vm_config_t* sll_current_vm_config=NULL;



void _call_function(thread_data_t* thr,sll_function_index_t fn,sll_arg_count_t ac,sll_bool_t fr){
	sll_assembly_function_t* af=sll_current_runtime_data->assembly_data->function_table.data+fn;
	if (SLL_ASSEMBLY_FUNCTION_IS_VAR_ARG(af)){
		sll_object_t* tos;
		if (SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)>ac){
			sll_static_int[0]->rc+=SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)-ac-1;
			for (sll_arg_count_t i=0;i<SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)-ac-1;i++){
				*(thr->stack+thr->stack_index)=sll_static_int[0];
				thr->stack_index++;
			}
			tos=sll_array_to_object(NULL);
		}
		else{
			tos=sll_array_length_to_object(ac-SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)+1);
			thr->stack_index-=tos->data.array.length;
			for (sll_array_length_t i=0;i<tos->data.array.length;i++){
				tos->data.array.data[i]=*(thr->stack+thr->stack_index+i);
				*(thr->stack+thr->stack_index+i)=NULL;
			}
		}
		*(thr->stack+thr->stack_index)=tos;
		thr->stack_index++;
	}
	else{
		if (ac!=SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)){
			if (SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)>ac){
				sll_static_int[0]->rc+=SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)-ac;
				for (sll_arg_count_t i=0;i<SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)-ac;i++){
					*(thr->stack+thr->stack_index+i)=sll_static_int[0];
				}
				thr->stack_index+=SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af)-ac;
			}
			else{
				for (sll_arg_count_t i=ac-SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af);i;i--){
					thr->stack_index--;
					SLL_RELEASE(*(thr->stack+thr->stack_index));
					*(thr->stack+thr->stack_index)=NULL;
				}
			}
		}
	}
	if (fr){
		SLL_ASSERT(thr->call_stack.length<=sll_current_vm_config->call_stack_size);
		(thr->call_stack.data+thr->call_stack.length)->_instruction_index=thr->instruction_index;
		(thr->call_stack.data+thr->call_stack.length)->_stack_offset=thr->stack_index-SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(af);
		(thr->call_stack.data+thr->call_stack.length)->_variable_memory_offset=PTR(ADDR(thr->stack+(thr->call_stack.data+thr->call_stack.length)->_stack_offset)-ADDR(_vm_var_data+sll_current_runtime_data->assembly_data->variable_count));
		(*((sll_call_stack_size_t*)(&(thr->call_stack.length))))++;
	}
	thr->instruction_index=af->instruction_index;
}



void _release_var_data(void){
	sll_object_t** obj=_vm_var_data;
	_vm_var_data=NULL;
	if (sll_current_runtime_data->assembly_data->variable_count){
		sll_variable_index_t i=sll_current_runtime_data->assembly_data->variable_count;
		do{
			i--;
			SLL_RELEASE(*(obj+i));
			*(obj+i)=NULL;
		} while (i);
		sll_gc_remove_roots(obj);
		SLL_CRITICAL_ERROR(sll_platform_free_page(obj,SLL_ROUND_PAGE(sll_current_runtime_data->assembly_data->variable_count*sizeof(sll_object_t*))));
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_code_t sll_execute_assembly(const sll_assembly_data_t* assembly_data,const sll_vm_config_t* vm_config){
	if (_vm_var_data){
		SLL_UNIMPLEMENTED();
	}
	_vm_instruction_count=0;
	sll_current_vm_config=vm_config;// lgtm [cpp/stack-address-escape]
	_vm_var_data=sll_platform_allocate_page(SLL_ROUND_PAGE(assembly_data->variable_count*sizeof(sll_object_t*)),0,NULL);
	sll_static_int[0]->rc+=assembly_data->variable_count;
	for (sll_variable_index_t i=0;i<assembly_data->variable_count;i++){
		*(_vm_var_data+i)=sll_static_int[0];
	}
	sll_gc_add_roots(_vm_var_data,assembly_data->variable_count);
	sll_internal_function_table_t ift;
	sll_clone_internal_function_table(vm_config->internal_function_table,&ift);
	sll_object_type_table_t tt=SLL_INIT_OBJECT_TYPE_TABLE_STRUCT;
	sll_runtime_data_t r_dt={
		assembly_data,
		&ift,
		&tt
	};
	sll_current_runtime_data=&r_dt;// lgtm [cpp/stack-address-escape]
	sll_audit(SLL_CHAR("sll.vm.init"),SLL_CHAR(""));
	sll_return_code_t rc=_scheduler_run();
	sll_current_runtime_data=NULL;
	sll_free_internal_function_table(&ift);
	sll_free_object_type_list(&tt);
	return rc;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_execute_function(sll_integer_t function,sll_object_t*const* args,sll_arg_count_t arg_count,sll_execution_flags_t flags){
	if (!sll_current_runtime_data||!sll_current_vm_config||!_vm_var_data){
		return NULL;
	}
	sll_thread_index_t tid=sll_thread_create(function,args,arg_count);
	if ((_scheduler_current_thread->flags&THREAD_FLAG_NO_AUDIT_TERMINATE)||(flags&EXECUTE_FUNCTION_NO_AUDIT_TERMINATE)){
		_thread_get(tid)->flags|=THREAD_FLAG_NO_AUDIT_TERMINATE;
	}
	else{
		sll_audit(SLL_CHAR("sll.thread.create"),SLL_CHAR("iO+u"),function,args,arg_count,tid);
	}
	sll_object_t* o;
	if (flags&SLL_EXECUTE_FUNCTION_ASYNC){
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_wait_thread(sll_thread_index_t thread_index){
	sll_thread_index_t s_tid=_scheduler_current_thread_index;
	if (s_tid==SLL_UNKNOWN_THREAD_INDEX){
		_scheduler_set_thread(thread_index);
	}
	SLL_ASSERT(_thread_get(thread_index)==_scheduler_current_thread);
	thread_data_t* tid_dt=_scheduler_current_thread;
	if (_scheduler_current_thread->return_value){
		goto _cleanup;
	}
	thread_data_t* thr=_scheduler_current_thread;
	const sll_assembly_instruction_t* ai;
	addr_t func_var_off;
	addr_t tls_var_off;
	RELOAD_THREAD_DATA;
	while (1){
		if (thr->instruction_index>=sll_current_runtime_data->assembly_data->instruction_count||thr->stack_index>=sll_current_vm_config->stack_size){
			SLL_UNIMPLEMENTED();
		}
		if (!thr->time_quantum){
			thr->_last_instruction=ai;
			_scheduler_queue_next();
			RELOAD_THREAD_DATA;
			if (_scheduler_current_thread_index!=thread_index&&tid_dt->return_value){
				goto _cleanup;
			}
		}
		thr->time_quantum--;
		_vm_instruction_count++;
		switch (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)){
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_POP:
				thr->stack_index--;
				SLL_RELEASE(*(thr->stack+thr->stack_index));
				*(thr->stack+thr->stack_index)=NULL;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO:
				SLL_RELEASE(*(thr->stack+thr->stack_index-1));
				*(thr->stack+thr->stack_index-1)=NULL;
				thr->stack_index-=2;
				SLL_RELEASE(*(thr->stack+thr->stack_index));
				*(thr->stack+thr->stack_index)=NULL;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT:
				{
					sll_object_t* tos=*(thr->stack+thr->stack_index-1);
					*(thr->stack+thr->stack_index-1)=*(thr->stack+thr->stack_index-2);
					*(thr->stack+thr->stack_index-2)=tos;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP:
				SLL_RELEASE(*(thr->stack+thr->stack_index-2));
				thr->stack_index--;
				*(thr->stack+thr->stack_index-1)=*(thr->stack+thr->stack_index);
				*(thr->stack+thr->stack_index)=NULL;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DUP:
				*(thr->stack+thr->stack_index)=*(thr->stack+thr->stack_index-1);
				SLL_ACQUIRE(*(thr->stack+thr->stack_index));
				thr->stack_index++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
				*(thr->stack+thr->stack_index)=sll_int_to_object(ai->data.int_);
				thr->stack_index++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT_COMPRESSED:
				{
					sll_size_t dec=sll_decompress_integer(ai->data.compressed_int);
					*(thr->stack+thr->stack_index)=sll_int_to_object(SLL_DECODE_SIGNED_INTEGER(dec));
					thr->stack_index++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE:
				*(thr->stack+thr->stack_index)=SLL_ACQUIRE_STATIC_NEG_INT(1);
				thr->stack_index++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO:
				*(thr->stack+thr->stack_index)=SLL_ACQUIRE_STATIC_INT(0);
				thr->stack_index++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE:
				*(thr->stack+thr->stack_index)=SLL_ACQUIRE_STATIC_INT(1);
				thr->stack_index++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO:
				*(thr->stack+thr->stack_index)=SLL_ACQUIRE_STATIC_INT(2);
				thr->stack_index++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE:
				*(thr->stack+thr->stack_index)=SLL_ACQUIRE_STATIC_INT(3);
				thr->stack_index++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR:
				*(thr->stack+thr->stack_index)=SLL_ACQUIRE_STATIC_INT(4);
				thr->stack_index++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
				*(thr->stack+thr->stack_index)=sll_float_to_object(ai->data.float_);
				thr->stack_index++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_COMPLEX:
				*(thr->stack+thr->stack_index)=sll_complex_to_object(&(ai->data.complex_));
				thr->stack_index++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
				*(thr->stack+thr->stack_index)=SLL_FROM_CHAR(ai->data.char_);
				thr->stack_index++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL:
				*(thr->stack+thr->stack_index)=sll_int_to_object((SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?thr->instruction_index+ai->data.jump.target.rel:ai->data.jump.target.abs));
				thr->stack_index++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_STACK:
				sll_static_int[0]->rc+=ai->data.stack_offset;
				for (sll_stack_offset_t i=0;i<ai->data.stack_offset;i++){
					*(thr->stack+thr->stack_index)=sll_static_int[0];
					thr->stack_index++;
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD:
				*(thr->stack+thr->stack_index)=*VAR_REF(ai->data.variable);
				SLL_ACQUIRE(*(thr->stack+thr->stack_index));
				thr->stack_index++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
				*(thr->stack+thr->stack_index)=STRING_TO_OBJECT(sll_current_runtime_data->assembly_data->string_table.data+ai->data.string_index);
				thr->stack_index++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK:
				{
					sll_object_t* tos=sll_array_length_to_object(ai->data.array_length);
					thr->stack_index-=ai->data.array_length;
					for (sll_array_length_t i=0;i<ai->data.array_length;i++){
						tos->data.array.data[i]=*(thr->stack+thr->stack_index+i);
						*(thr->stack+thr->stack_index+i)=NULL;
					}
					*(thr->stack+thr->stack_index)=tos;
					thr->stack_index++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO:
				*(thr->stack+thr->stack_index)=sll_array_to_object(NULL);
				thr->stack_index++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ONE:
				{
					sll_object_t* tos=sll_array_length_to_object(1);
					tos->data.array.data[0]=*(thr->stack+thr->stack_index-1);
					*(thr->stack+thr->stack_index-1)=tos;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP:
				{
					sll_object_t* tos=sll_map_length_to_object((ai->data.map_length+1)>>1);
					sll_map_t* m=&(tos->data.map);
					thr->stack_index-=ai->data.map_length;
					for (sll_map_length_t i=0;i<ai->data.map_length;i++){
						m->data[i]=*(thr->stack+thr->stack_index+i);
						*(thr->stack+thr->stack_index+i)=NULL;
					}
					if (ai->data.map_length&1){
						m->data[ai->data.map_length]=SLL_ACQUIRE_STATIC_INT(0);
					}
					*(thr->stack+thr->stack_index)=tos;
					thr->stack_index++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP_ZERO:
				*(thr->stack+thr->stack_index)=sll_map_to_object(NULL);
				thr->stack_index++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE:
				SLL_RELEASE(*VAR_REF(ai->data.variable));
				*VAR_REF(ai->data.variable)=*(thr->stack+thr->stack_index-1);
				SLL_ACQUIRE(*(thr->stack+thr->stack_index-1));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP:
				SLL_RELEASE(*VAR_REF(ai->data.variable));
				thr->stack_index--;
				*VAR_REF(ai->data.variable)=*(thr->stack+thr->stack_index);
				*(thr->stack+thr->stack_index)=NULL;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE:
				SLL_RELEASE(*VAR_REF(ai->data.variable));
				*VAR_REF(ai->data.variable)=SLL_ACQUIRE_STATIC_NEG_INT(1);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ZERO:
				SLL_RELEASE(*VAR_REF(ai->data.variable));
				*VAR_REF(ai->data.variable)=SLL_ACQUIRE_STATIC_INT(0);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ONE:
				SLL_RELEASE(*VAR_REF(ai->data.variable));
				*VAR_REF(ai->data.variable)=SLL_ACQUIRE_STATIC_INT(1);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_TWO:
				SLL_RELEASE(*VAR_REF(ai->data.variable));
				*VAR_REF(ai->data.variable)=SLL_ACQUIRE_STATIC_INT(2);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_THREE:
				SLL_RELEASE(*VAR_REF(ai->data.variable));
				*VAR_REF(ai->data.variable)=SLL_ACQUIRE_STATIC_INT(3);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_FOUR:
				SLL_RELEASE(*VAR_REF(ai->data.variable));
				*VAR_REF(ai->data.variable)=SLL_ACQUIRE_STATIC_INT(4);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP:
				{
					sll_object_t* n=sll_operator_cast(*(thr->stack+thr->stack_index-1),sll_static_int[SLL_OBJECT_TYPE_STRING]);
					sll_gc_add_root(n);
					SLL_RELEASE(*(thr->stack+thr->stack_index-1));
					sll_function_index_t i=sll_lookup_internal_function(sll_current_runtime_data->internal_function_table,n->data.string.data);
					sll_gc_remove_root(n);
					SLL_RELEASE(n);
					*(thr->stack+thr->stack_index-1)=(i==SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX?SLL_ACQUIRE_STATIC_INT(0):sll_int_to_object(~((sll_integer_t)i)));
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR:
				{
					sll_function_index_t i=sll_lookup_internal_function(sll_current_runtime_data->internal_function_table,(sll_current_runtime_data->assembly_data->string_table.data+ai->data.string_index)->data);
					*(thr->stack+thr->stack_index)=(i==SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX?SLL_ACQUIRE_STATIC_INT(0):sll_int_to_object(~((sll_integer_t)i)));
					thr->stack_index++;
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
					thr->stack_index--;
					sll_stack_offset_t e_si=thr->stack_index;
					thr->stack_index-=ai->data.array_length<<1;
					sll_stack_offset_t i=thr->stack_index-1;
					sll_object_t* cnd=*(thr->stack+thr->stack_index-1);
					sll_gc_add_root(cnd);
					*(thr->stack+thr->stack_index-1)=NULL;
					sll_integer_t cnd_v;
					if (cnd->type==SLL_OBJECT_TYPE_INT){
						cnd_v=cnd->data.int_;
					}
					else if (cnd->type==SLL_OBJECT_TYPE_CHAR){
						cnd_v=cnd->data.char_;
					}
					else{
						sll_gc_remove_root(cnd);
						SLL_RELEASE(cnd);
						while (thr->stack_index<e_si){
							SLL_RELEASE(*(thr->stack+thr->stack_index));
							*(thr->stack+thr->stack_index)=NULL;
							thr->stack_index++;
						}
						goto _jump_to_default;
					}
					sll_gc_remove_root(cnd);
					SLL_RELEASE(cnd);
					while (thr->stack_index<e_si){
						sll_object_t* k=*(thr->stack+thr->stack_index);
						if ((k->type==SLL_OBJECT_TYPE_INT&&k->data.int_==cnd_v)||(k->type==SLL_OBJECT_TYPE_CHAR&&k->data.char_==cnd_v)){
							goto _cleanup_jump_table;
						}
						SLL_RELEASE(k);
						*(thr->stack+thr->stack_index)=NULL;
						SLL_RELEASE(*(thr->stack+thr->stack_index+1));
						*(thr->stack+thr->stack_index+1)=NULL;
						thr->stack_index+=2;
					}
_jump_to_default:
					SLL_ASSERT(thr->stack_index==e_si);
					thr->instruction_index=(sll_instruction_index_t)((*(thr->stack+thr->stack_index))->data.int_);
					ai=_get_instruction_at_offset(sll_current_runtime_data->assembly_data,thr->instruction_index);
					SLL_RELEASE(*(thr->stack+thr->stack_index));
					*(thr->stack+thr->stack_index)=NULL;
					thr->stack_index=i;
					continue;
_cleanup_jump_table:;
					thr->instruction_index=(sll_instruction_index_t)((*(thr->stack+thr->stack_index+1))->data.int_);
					ai=_get_instruction_at_offset(sll_current_runtime_data->assembly_data,thr->instruction_index);
					e_si++;
					while (thr->stack_index<e_si){
						SLL_RELEASE(*(thr->stack+thr->stack_index));
						*(thr->stack+thr->stack_index)=NULL;
						thr->stack_index++;
					}
					thr->stack_index=i;
					continue;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?VAR_REF(ai->data.variable):thr->stack+thr->stack_index-1);
					sll_object_t* n=sll_static_int[!sll_operator_bool(*tos)];
					SLL_ACQUIRE(n);
					SLL_RELEASE(*tos);
					*tos=n;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_BOOL:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?VAR_REF(ai->data.variable):thr->stack+thr->stack_index-1);
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
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DEEP_COPY:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?VAR_REF(ai->data.variable):thr->stack+thr->stack_index-1);
					sll_object_t* n=sll_operator_copy(*tos,SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_DEEP_COPY);
					SLL_RELEASE(*tos);
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
					sll_stack_offset_t off=ai->data.variable_access.arg_count-1;
					sll_object_t* d=*VAR_REF(ai->data.variable_access.variable);
					SLL_ACQUIRE(d);
					sll_arg_count_t l=ai->data.variable_access.arg_count;
					sll_object_t** p=thr->stack+thr->stack_index-off-1;
					sll_gc_add_roots(p,l);
					do{
						SLL_ASSERT((*p)->type==SLL_OBJECT_TYPE_STRING);
						sll_object_t* nd=sll_operator_access(d,*p);
						SLL_RELEASE(d);
						SLL_RELEASE(*p);
						*p=NULL;
						d=nd;
						p++;
						l--;
					} while (l);
					thr->stack_index-=off;
					sll_gc_remove_roots(thr->stack+thr->stack_index-1);
					*(thr->stack+thr->stack_index-1)=d;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN:
				sll_operator_assign((SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)?*VAR_REF(ai->data.variable):*(thr->stack+thr->stack_index-3)),*(thr->stack+thr->stack_index-2),*(thr->stack+thr->stack_index-1));
				SLL_RELEASE(*(thr->stack+thr->stack_index-2));
				*(thr->stack+thr->stack_index-2)=NULL;
				SLL_RELEASE(*(thr->stack+thr->stack_index-1));
				*(thr->stack+thr->stack_index-1)=NULL;
				thr->stack_index-=2;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_TWO:
				SLL_UNIMPLEMENTED();
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_THREE:
				SLL_UNIMPLEMENTED();
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR_ACCESS:
				{
					sll_object_t* nv=*(thr->stack+thr->stack_index-1);
					*(thr->stack+thr->stack_index-1)=NULL;
					sll_stack_offset_t off=ai->data.variable_access.arg_count+1;
					sll_object_t* d=*VAR_REF(ai->data.variable_access.variable);
					SLL_ACQUIRE(d);
					sll_arg_count_t l=ai->data.variable_access.arg_count-1;
					sll_object_t** p=thr->stack+thr->stack_index-off;
					sll_gc_add_roots(p,l);
					while (l){
						SLL_ASSERT((*p)->type==SLL_OBJECT_TYPE_STRING);
						sll_object_t* nd=sll_operator_access(d,*p);
						SLL_RELEASE(d);
						SLL_RELEASE(*p);
						*p=NULL;
						d=nd;
						p++;
						l--;
					}
					SLL_ASSERT((*p)->type==SLL_OBJECT_TYPE_STRING);
					sll_operator_assign(d,*p,nv);
					SLL_RELEASE(d);
					SLL_RELEASE(*p);
					*p=NULL;
					SLL_RELEASE(nv);
					thr->stack_index-=off;
					sll_gc_remove_roots(thr->stack+thr->stack_index);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_cast);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?VAR_REF(ai->data.variable):thr->stack+thr->stack_index-1);
					sll_object_t* t=sll_int_to_object(ai->data.type);
					sll_object_t* n=sll_operator_cast(*tos,t);
					SLL_RELEASE(*tos);
					SLL_RELEASE(t);
					*tos=n;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_TYPEOF:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?VAR_REF(ai->data.variable):thr->stack+thr->stack_index-1);
					sll_object_t* n=sll_int_to_object((*tos)->type);
					SLL_RELEASE(*tos);
					*tos=n;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NAMEOF:
				{
					sll_object_t* tos=sll_create_object(SLL_OBJECT_TYPE_STRING);
					sll_get_name(*(thr->stack+thr->stack_index-1),&(tos->data.string));
					SLL_RELEASE(*(thr->stack+thr->stack_index-1));
					*(thr->stack+thr->stack_index-1)=tos;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NAMEOF_TYPE:
				{
					sll_object_t* t=sll_operator_cast(*(thr->stack+thr->stack_index-1),sll_static_int[SLL_OBJECT_TYPE_INT]);
					sll_gc_add_root(t);
					SLL_RELEASE(*(thr->stack+thr->stack_index-1));
					sll_object_t* tos=sll_create_object(SLL_OBJECT_TYPE_STRING);
					sll_get_type_name(sll_current_runtime_data->type_table,(t->data.int_<0||t->data.int_>sll_current_runtime_data->type_table->length+SLL_MAX_OBJECT_TYPE?SLL_OBJECT_TYPE_INT:(sll_object_type_t)(t->data.int_)),&(tos->data.string));
					*(thr->stack+thr->stack_index-1)=tos;
					sll_gc_remove_root(t);
					SLL_RELEASE(t);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL:
				{
					sll_stack_offset_t off=ai->data.arg_count<<1;
					sll_object_type_t nt;
					if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_ANONYMOUS(ai)){
						nt=sll_add_type(sll_current_runtime_data->type_table,thr->stack+thr->stack_index-off,ai->data.arg_count,NULL);
						for (sll_arg_count_t i=0;i<off;i++){
							thr->stack_index--;
							SLL_RELEASE(*(thr->stack+thr->stack_index));
							*(thr->stack+thr->stack_index)=NULL;
						}
						thr->stack_index++;
					}
					else{
						SLL_ASSERT((*(thr->stack+thr->stack_index-off-1))->type==SLL_OBJECT_TYPE_STRING);
						nt=sll_add_type(sll_current_runtime_data->type_table,thr->stack+thr->stack_index-off,ai->data.arg_count,&((*(thr->stack+thr->stack_index-off-1))->data.string));
						for (sll_arg_count_t i=0;i<off;i++){
							thr->stack_index--;
							SLL_RELEASE(*(thr->stack+thr->stack_index));
							*(thr->stack+thr->stack_index)=NULL;
						}
						SLL_RELEASE(*(thr->stack+thr->stack_index-1));
					}
					*(thr->stack+thr->stack_index-1)=sll_int_to_object(nt);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL_ZERO:
				{
					if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_ANONYMOUS(ai)){
						*(thr->stack+thr->stack_index)=sll_int_to_object(sll_add_type(sll_current_runtime_data->type_table,NULL,0,NULL));
						thr->stack_index++;
					}
					else{
						SLL_ASSERT((*(thr->stack+thr->stack_index-1))->type==SLL_OBJECT_TYPE_STRING);
						sll_object_type_t nt=sll_add_type(sll_current_runtime_data->type_table,NULL,0,&((*(thr->stack+thr->stack_index-1))->data.string));
						SLL_RELEASE(*(thr->stack+thr->stack_index-1));
						*(thr->stack+thr->stack_index-1)=sll_int_to_object(nt);
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW:
				{
					sll_object_t* t=sll_operator_cast(*(thr->stack+thr->stack_index-ai->data.arg_count-1),sll_static_int[SLL_OBJECT_TYPE_INT]);
					sll_object_type_t ot=(t->data.int_<0||t->data.int_>sll_current_runtime_data->type_table->length+SLL_MAX_OBJECT_TYPE?SLL_OBJECT_TYPE_INT:(sll_object_type_t)(t->data.int_));
					SLL_RELEASE(t);
					SLL_RELEASE(*(thr->stack+thr->stack_index-ai->data.arg_count-1));
					*(thr->stack+thr->stack_index-ai->data.arg_count-1)=sll_create_object_type(sll_current_runtime_data->type_table,ot,thr->stack+thr->stack_index-ai->data.arg_count,ai->data.arg_count);
					for (sll_arg_count_t i=0;i<ai->data.arg_count;i++){
						thr->stack_index--;
						SLL_RELEASE(*(thr->stack+thr->stack_index));
						*(thr->stack+thr->stack_index)=NULL;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW_DECL:
				*(thr->stack+thr->stack_index)=sll_create_new_object_type(sll_current_runtime_data->type_table);
				thr->stack_index++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR:
				*(thr->stack+thr->stack_index)=*VAR_REF(ai->data.variable);
				SLL_ACQUIRE(*(thr->stack+thr->stack_index));
				thr->stack_index++;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT:
				{
					thr->stack_index--;
					sll_object_t* tos=*(thr->stack+thr->stack_index);
					*(thr->stack+thr->stack_index)=NULL;
					sll_gc_add_root(tos);
					if (!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)||sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO)){
						if (tos->type==SLL_OBJECT_TYPE_STRING){
							sll_file_write(sll_current_vm_config->out,tos->data.string.data,tos->data.string.length*sizeof(sll_char_t),NULL);
						}
						else{
							sll_string_t str;
							sll_api_string_convert(&tos,1,&str);
							sll_file_write(sll_current_vm_config->out,str.data,str.length*sizeof(sll_char_t),NULL);
							sll_free_string(&str);
						}
					}
					sll_gc_remove_root(tos);
					SLL_RELEASE(tos);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
				if (!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)||sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO)){
					sll_file_write_char(sll_current_vm_config->out,ai->data.char_,NULL);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
				if (!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)||sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO)){
					sll_file_write(sll_current_vm_config->out,(sll_current_runtime_data->assembly_data->string_table.data+ai->data.string_index)->data,(sll_current_runtime_data->assembly_data->string_table.data+ai->data.string_index)->length*sizeof(sll_char_t),NULL);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP:
				{
					thr->stack_index--;
					sll_object_t* tos=*(thr->stack+thr->stack_index);
					sll_gc_add_root(tos);
					*(thr->stack+thr->stack_index)=NULL;
					if (tos->type==SLL_OBJECT_TYPE_INT){
						sll_integer_t i=tos->data.int_;
						sll_gc_remove_root(tos);
						SLL_RELEASE(tos);
						if (i<0){
							sll_function_index_t j=(sll_function_index_t)(~i);
							if (j<sll_current_runtime_data->internal_function_table->length){
								sll_object_t* n=_call_api_func(j,thr->stack+thr->stack_index-ai->data.arg_count,ai->data.arg_count);
								sll_gc_add_root(n);
								for (sll_arg_count_t k=0;k<ai->data.arg_count;k++){
									thr->stack_index--;
									SLL_RELEASE(*(thr->stack+thr->stack_index));
									*(thr->stack+thr->stack_index)=NULL;
								}
								sll_gc_remove_root(n);
								if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)!=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP){
									*(thr->stack+thr->stack_index)=n;
									thr->stack_index++;
								}
								else{
									SLL_RELEASE(n);
								}
								break;
							}
						}
						else if (i&&i<=sll_current_runtime_data->assembly_data->function_table.length){
							_call_function(thr,(sll_function_index_t)(i-1),ai->data.arg_count,1);
							RELOAD_THREAD_DATA;
							continue;
						}
					}
					else{
						sll_gc_remove_root(tos);
						SLL_RELEASE(tos);
					}
					for (sll_arg_count_t i=0;i<ai->data.arg_count;i++){
						thr->stack_index--;
						SLL_RELEASE(*(thr->stack+thr->stack_index));
						*(thr->stack+thr->stack_index)=NULL;
					}
					if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)!=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP){
						*(thr->stack+thr->stack_index)=SLL_ACQUIRE_STATIC_INT(0);
						thr->stack_index++;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO:
				if (ai->data.int_<0){
					sll_function_index_t i=(sll_function_index_t)(~ai->data.int_);
					if (i<sll_current_runtime_data->internal_function_table->length){
						*(thr->stack+thr->stack_index)=_call_api_func(i,NULL,0);
						thr->stack_index++;
						break;
					}
				}
				else if (ai->data.int_&&ai->data.int_<=sll_current_runtime_data->assembly_data->function_table.length){
					_call_function(thr,(sll_function_index_t)(ai->data.int_-1),0,1);
					RELOAD_THREAD_DATA;
					continue;
				}
				*(thr->stack+thr->stack_index)=SLL_ACQUIRE_STATIC_INT(0);
				thr->stack_index++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:
				if (ai->data.int_<0){
					sll_function_index_t i=(sll_function_index_t)(~ai->data.int_);
					if (i<sll_current_runtime_data->internal_function_table->length){
						sll_object_t* n=_call_api_func(i,thr->stack+thr->stack_index-1,1);
						sll_gc_add_root(n);
						SLL_RELEASE(*(thr->stack+thr->stack_index-1));
						sll_gc_remove_root(n);
						*(thr->stack+thr->stack_index-1)=n;
						break;
					}
				}
				else if (ai->data.int_&&ai->data.int_<=sll_current_runtime_data->assembly_data->function_table.length){
					_call_function(thr,(sll_function_index_t)(ai->data.int_-1),1,1);
					RELOAD_THREAD_DATA;
					continue;
				}
				SLL_RELEASE(*(thr->stack+thr->stack_index-1));
				*(thr->stack+thr->stack_index-1)=SLL_ACQUIRE_STATIC_INT(0);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ARRAY:
				{
					thr->stack_index--;
					sll_object_t* tos=*(thr->stack+thr->stack_index);
					sll_gc_add_root(tos);
					*(thr->stack+thr->stack_index)=NULL;
					if (tos->type==SLL_OBJECT_TYPE_INT){
						sll_integer_t i=tos->data.int_;
						sll_gc_remove_root(tos);
						SLL_RELEASE(tos);
						tos=sll_operator_cast(*(thr->stack+thr->stack_index-1),sll_static_int[SLL_OBJECT_TYPE_ARRAY]);
						sll_gc_add_root(tos);
						if (i<0){
							sll_function_index_t j=(sll_function_index_t)(~i);
							if (j<sll_current_runtime_data->internal_function_table->length){
								SLL_RELEASE(*(thr->stack+thr->stack_index-1));
								sll_object_t* n=_call_api_func(j,tos->data.array.data,tos->data.array.length);
								sll_gc_remove_root(tos);
								SLL_RELEASE(tos);
								*(thr->stack+thr->stack_index-1)=n;
								break;
							}
						}
						else if (i&&i<=sll_current_runtime_data->assembly_data->function_table.length){
							thr->stack_index--;
							SLL_RELEASE(*(thr->stack+thr->stack_index));
							for (sll_array_length_t j=0;j<tos->data.array.length;j++){
								*(thr->stack+thr->stack_index)=tos->data.array.data[j];
								thr->stack_index++;
								SLL_ACQUIRE(tos->data.array.data[j]);
							}
							_call_function(thr,(sll_function_index_t)(i-1),tos->data.array.length,1);
							sll_gc_remove_root(tos);
							SLL_RELEASE(tos);
							RELOAD_THREAD_DATA;
							continue;
						}
					}
					sll_gc_remove_root(tos);
					SLL_RELEASE(tos);
					SLL_RELEASE(*(thr->stack+thr->stack_index-1));
					*(thr->stack+thr->stack_index-1)=SLL_ACQUIRE_STATIC_INT(0);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_REF:
				{
					sll_integer_t a=(sll_integer_t)ADDR(*(thr->stack+thr->stack_index-1));
					SLL_RELEASE(*(thr->stack+thr->stack_index-1));
					*(thr->stack+thr->stack_index-1)=sll_int_to_object(a);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET:
_return:;
				{
					sll_object_t* tmp=*(thr->stack+thr->stack_index-1);
					sll_gc_add_root(tmp);
					SLL_ACQUIRE(tmp);
					if (!thr->call_stack.length){
						while (thr->stack_index){
							thr->stack_index--;
							SLL_RELEASE(*(thr->stack+thr->stack_index));
							*(thr->stack+thr->stack_index)=NULL;
						}
						sll_thread_index_t idx=_scheduler_current_thread_index;
						sll_gc_remove_root(tmp);
						_thread_terminate(tmp);
						SLL_RELEASE(tmp);
						if (idx==thread_index){
							goto _cleanup;
						}
						RELOAD_THREAD_DATA;
						continue;
					}
					(*((sll_call_stack_size_t*)(&(thr->call_stack.length))))--;
					thr->instruction_index=(thr->call_stack.data+thr->call_stack.length)->_instruction_index;
					sll_stack_offset_t n_si=(thr->call_stack.data+thr->call_stack.length)->_stack_offset;
					SLL_ASSERT(thr->stack_index>=n_si);
					while (thr->stack_index>n_si){
						thr->stack_index--;
						SLL_RELEASE(*(thr->stack+thr->stack_index));
						*(thr->stack+thr->stack_index)=NULL;
					}
					RELOAD_THREAD_DATA;
					sll_gc_remove_root(tmp);
					if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)!=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP){
						*(thr->stack+thr->stack_index)=tmp;
						thr->stack_index++;
					}
					else{
						SLL_RELEASE(tmp);
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO:
				*(thr->stack+thr->stack_index)=SLL_ACQUIRE_STATIC_INT(0);
				thr->stack_index++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				*(thr->stack+thr->stack_index)=sll_int_to_object(ai->data.int_);
				thr->stack_index++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				*(thr->stack+thr->stack_index)=sll_float_to_object(ai->data.float_);
				thr->stack_index++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				*(thr->stack+thr->stack_index)=SLL_FROM_CHAR(ai->data.char_);
				thr->stack_index++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				*(thr->stack+thr->stack_index)=STRING_TO_OBJECT(sll_current_runtime_data->assembly_data->string_table.data+ai->data.string_index);
				thr->stack_index++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR:
				*(thr->stack+thr->stack_index)=*VAR_REF(ai->data.variable);
				SLL_ACQUIRE(*(thr->stack+thr->stack_index));
				thr->stack_index++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL:
				SLL_RELEASE(*VAR_REF(ai->data.variable));
				*VAR_REF(ai->data.variable)=SLL_ACQUIRE_STATIC_INT(0);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL:
				*(thr->stack+thr->stack_index)=*VAR_REF(ai->data.variable);
				*VAR_REF(ai->data.variable)=SLL_ACQUIRE_STATIC_INT(0);
				thr->stack_index++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_WAIT:
				{
					sll_object_t* n_tid_o=sll_operator_cast(*(thr->stack+thr->stack_index-1),sll_static_int[SLL_OBJECT_TYPE_INT]);
					sll_integer_t w_tid=n_tid_o->data.int_;
					SLL_RELEASE(n_tid_o);
					thr->stack_index--;
					SLL_RELEASE(*(thr->stack+thr->stack_index));
					*(thr->stack+thr->stack_index)=NULL;
					thread_data_t* c_thr=thr;
					if (!_thread_wait(w_tid)){
						break;
					}
					c_thr->instruction_index++;
					NEXT_INSTRUCTION;
					c_thr->_last_instruction=ai;
_load_new_thread:;
					sll_thread_index_t n_tid=_scheduler_queue_pop();
					if (n_tid==SLL_UNKNOWN_THREAD_INDEX){
						if (tid_dt->return_value){
							goto _cleanup;
						}
						SLL_UNIMPLEMENTED();
					}
					_scheduler_set_thread(n_tid);
					RELOAD_THREAD_DATA;
					continue;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_ID:
				*(thr->stack+thr->stack_index)=sll_int_to_object(_scheduler_current_thread_index);
				thr->stack_index++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_LOCK:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_SEMAPHORE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_BARRIER_EQ:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_BARRIER_GEQ:
				{
					sll_integer_t v=0;
					if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)>SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_SEMAPHORE){
						sll_object_t* v_o=sll_operator_cast(*(thr->stack+thr->stack_index-1),sll_static_int[SLL_OBJECT_TYPE_INT]);
						v=v_o->data.int_;
						SLL_RELEASE(v_o);
						thr->stack_index--;
						SLL_RELEASE(*(thr->stack+thr->stack_index));
						*(thr->stack+thr->stack_index)=NULL;
					}
					sll_object_t* lck_o=sll_operator_cast(*(thr->stack+thr->stack_index-1),sll_static_int[SLL_OBJECT_TYPE_INT]);
					sll_integer_t lck=lck_o->data.int_;
					SLL_RELEASE(lck_o);
					thr->stack_index--;
					SLL_RELEASE(*(thr->stack+thr->stack_index));
					*(thr->stack+thr->stack_index)=NULL;
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
					c_thr->instruction_index++;
					NEXT_INSTRUCTION;
					c_thr->_last_instruction=ai;
					goto _load_new_thread;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_EXIT:
				{
					sll_object_t* ret=*(thr->stack+thr->stack_index-1);
					sll_gc_add_root(ret);
					SLL_ACQUIRE(ret);
					while (thr->stack_index){
						thr->stack_index--;
						SLL_RELEASE(*(thr->stack+thr->stack_index));
						*(thr->stack+thr->stack_index)=NULL;
					}
					sll_thread_index_t idx=_scheduler_current_thread_index;
					sll_gc_remove_root(ret);
					_thread_terminate(ret);
					SLL_RELEASE(ret);
					if (idx==thread_index){
						goto _cleanup;
					}
					RELOAD_THREAD_DATA;
					continue;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_READ_BLOCKING:
				{
					sll_object_t* sz_o=sll_operator_cast(*(thr->stack+thr->stack_index-1),sll_static_int[SLL_OBJECT_TYPE_INT]);
					sll_integer_t sz=sz_o->data.int_;
					SLL_RELEASE(sz_o);
					thr->stack_index--;
					SLL_RELEASE(*(thr->stack+thr->stack_index));
					*(thr->stack+thr->stack_index)=NULL;
					sll_object_t* fh_o=sll_operator_cast(*(thr->stack+thr->stack_index-1),sll_static_int[SLL_OBJECT_TYPE_INT]);
					sll_file_t* f=sll_file_from_handle((sll_file_handle_t)(fh_o->data.int_));
					SLL_RELEASE(fh_o);
					thr->stack_index--;
					SLL_RELEASE(*(thr->stack+thr->stack_index));
					*(thr->stack+thr->stack_index)=NULL;
					if (!f||sz<0){
						*(thr->stack+thr->stack_index)=EMPTY_STRING_TO_OBJECT();
						thr->stack_index++;
						break;
					}
					sll_string_length_t l=(sz>SLL_MAX_STRING_LENGTH?SLL_MAX_STRING_LENGTH:(sll_string_length_t)sz);
					if ((f->flags&SLL_FILE_FLAG_ASYNC)&&!sll_file_data_available(f)){
						thr->instruction_index++;
						NEXT_INSTRUCTION;
						thr->_last_instruction=ai;
						_io_dispatcher_queue(f,l);
						goto _load_new_thread;
					}
					sll_string_t bf;
					sll_error_t err;
					if (!sz){
						err=sll_file_read_all(f,&bf);
					}
					else{
						sll_string_create(l,&bf);
						sll_size_t r_sz=sll_file_read(f,bf.data,l,&err);
						if (err==SLL_NO_ERROR){
							sll_string_decrease(&bf,(sll_string_length_t)r_sz);
							sll_string_calculate_checksum(&bf);
						}
					}
					if (err!=SLL_NO_ERROR){
						sll_free_string(&bf);
						*(thr->stack+thr->stack_index)=sll_int_to_object(err);
					}
					else{
						*(thr->stack+thr->stack_index)=STRING_TO_OBJECT_NOCOPY(&bf);
					}
					thr->stack_index++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_READ_BLOCKING_CHAR:
				{
					sll_object_t* fh_o=sll_operator_cast(*(thr->stack+thr->stack_index-1),sll_static_int[SLL_OBJECT_TYPE_INT]);
					sll_file_t* f=sll_file_from_handle((sll_file_handle_t)(fh_o->data.int_));
					SLL_RELEASE(fh_o);
					thr->stack_index--;
					SLL_RELEASE(*(thr->stack+thr->stack_index));
					*(thr->stack+thr->stack_index)=NULL;
					if (!f){
						*(thr->stack+thr->stack_index)=SLL_FROM_CHAR(0);
						thr->stack_index++;
						break;
					}
					if ((f->flags&SLL_FILE_FLAG_ASYNC)&&!sll_file_data_available(f)){
						thr->instruction_index++;
						NEXT_INSTRUCTION;
						thr->_last_instruction=ai;
						_io_dispatcher_queue(f,SLL_MAX_STRING_LENGTH);
						goto _load_new_thread;
					}
					sll_error_t err;
					sll_read_char_t chr=sll_file_read_char(f,&err);
					*(thr->stack+thr->stack_index)=(chr==SLL_END_OF_DATA?(err==SLL_NO_ERROR?SLL_ACQUIRE_STATIC_INT(0):sll_int_to_object(~err)):SLL_FROM_CHAR(chr));
					thr->stack_index++;
					break;
				}
			default:
				SLL_UNREACHABLE();
		}
		NEXT_INSTRUCTION;
		thr->instruction_index++;
	}
_cleanup:
	if (s_tid!=SLL_UNKNOWN_THREAD_INDEX){
		_scheduler_set_thread(s_tid);
	}
	sll_object_t* ret=tid_dt->return_value;
	SLL_ACQUIRE(ret);
	return ret;
}
