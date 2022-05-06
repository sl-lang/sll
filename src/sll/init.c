#include <sll/_internal/audit.h>
#include <sll/_internal/common.h>
#include <sll/_internal/environment.h>
#include <sll/_internal/file.h>
#include <sll/_internal/gc.h>
#include <sll/_internal/init.h>
#include <sll/_internal/log.h>
#include <sll/_internal/memory.h>
#include <sll/_internal/platform.h>
#include <sll/_internal/sandbox.h>
#include <sll/_internal/scheduler.h>
#include <sll/_internal/static_string.h>
#include <sll/_internal/weakref.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/generated/cpuid_error.h>
#include <sll/init.h>
#include <sll/location.h>
#include <sll/platform/file.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/thread.h>
#include <sll/types.h>
#include <sll/vm.h>



INIT_STRING_SETUP;



static sll_cleanup_function_t _init_exit_table[MAX_CLEANUP_TABLE_SIZE];
static unsigned int _init_exit_table_size=0;
static sll_cleanup_function_t _init_vm_exit_table[MAX_CLEANUP_TABLE_SIZE];
static unsigned int _init_vm_exit_table_size=0;
static sll_bool_t _init_init=0;



static void _write_number(sll_file_descriptor_t fd,sll_file_offset_t n){
	if (!n){
		sll_platform_file_write(fd,"0",1,NULL);
		return;
	}
	sll_char_t bf[20];
	sll_string_length_t i=20;
	while (n){
		i--;
		bf[i]=(n%10)+48;
		n/=10;
	}
	sll_platform_file_write(fd,bf+i,20-i,NULL);
}



static void _write_stack_frame(sll_file_descriptor_t fd,sll_instruction_index_t ii){
	sll_platform_file_write(fd,"at ",3,NULL);
	sll_string_index_t fp_i;
	sll_string_index_t fn_i;
	sll_file_offset_t ln=sll_get_location(sll_current_runtime_data->a_dt,ii,&fp_i,&fn_i);
	const sll_string_t* str=sll_current_runtime_data->a_dt->st.dt+fp_i;
	sll_platform_file_write(fd,str->v,str->l,NULL);
	sll_platform_file_write(fd,":",1,NULL);
	_write_number(fd,ln);
	sll_platform_file_write(fd," (",2,NULL);
	if (fn_i==SLL_MAX_STRING_INDEX){
		sll_platform_file_write(fd,"@code)\n",7,NULL);
		return;
	}
	str=sll_current_runtime_data->a_dt->st.dt+fn_i;
	sll_platform_file_write(fd,str->v,str->l,NULL);
	sll_platform_file_write(fd,")\n",2,NULL);
}



void _cleanup_vm_exit_tables(void){
	while (_init_vm_exit_table_size){
		_init_vm_exit_table_size--;
		_init_vm_exit_table[_init_vm_exit_table_size]();
	}
}



__SLL_NO_RETURN void _force_exit(const sll_char_t* a,const sll_char_t* b,const sll_char_t* c){
	sll_file_descriptor_t fd=sll_platform_get_default_stream_descriptor(SLL_PLATFORM_STREAM_ERROR);
	sll_platform_file_write(fd,a,sll_string_length(a),NULL);
	sll_platform_file_write(fd,b,sll_string_length(b),NULL);
	sll_platform_file_write(fd,c,sll_string_length(c),NULL);
	if (sll_current_runtime_data&&sll_current_runtime_data->a_dt){
		const sll_call_stack_t* c_st=sll_thread_get_call_stack(_scheduler_current_thread_index);
		if (c_st){
			_write_stack_frame(fd,sll_thread_get_instruction_index(SLL_UNKNOWN_THREAD_INDEX));
			sll_call_stack_size_t i=c_st->l;
			while (i){
				i--;
				_write_stack_frame(fd,(c_st->dt+i)->_ii);
			}
			sll_platform_file_write(fd,"at <thread-",11,NULL);
			_write_number(fd,_scheduler_current_thread_index);
			sll_platform_file_write(fd,">\n",2,NULL);
		}
	}
	_force_exit_platform();
}



__SLL_EXTERNAL void sll_deinit(void){
	if (!_init_init){
		return;
	}
	while (_init_exit_table_size){
		_init_exit_table_size--;
		_init_exit_table[_init_exit_table_size]();
	}
	const init_string_t*const* l=(const init_string_t*const*)(&__init_string_start);
	while (l<(const init_string_t*const*)(&__init_string_end)){
		const init_string_t* k=*l;
		if (k){
			sll_free_string(k->p);
		}
		l++;
	}
	_weakref_cleanup_data();
	_gc_release_data();
	_deinit_environment();
	_log_release_data();
	_file_release_std_streams();
	_audit_cleanup();
	_deinit_platform_time();
	_deinit_platform();
	_memory_deinit();
	_sandbox_flags=0;
	_init_init=0;
}



__SLL_EXTERNAL void sll_init(void){
	if (_init_init){
		return;
	}
	_init_init=1;
	if (_check_cpuid_flags()){
		sll_platform_file_write(sll_platform_get_default_stream_descriptor(SLL_PLATFORM_STREAM_ERROR),CPUID_ERROR,CPUID_ERROR_SIZE,NULL);
		_force_exit_platform();
	}
	_memory_init();
	_file_init_std_streams();
	_init_platform();
	_init_platform_time();
	_init_environment();
	const init_string_t*const* l=(const init_string_t*const*)(&__init_string_start);
	while (l<(const init_string_t*const*)(&__init_string_end)){
		const init_string_t* k=*l;
		if (k){
			if (k->dtl==SLL_MAX_STRING_LENGTH){
				k->dt.fn(k->p);
			}
			else{
				sll_string_from_pointer_length(k->dt.s,k->dtl,k->p);
			}
		}
		l++;
	}
}



__SLL_EXTERNAL void sll_register_cleanup(sll_cleanup_function_t fn,sll_bool_t type){
	if (type==SLL_CLEANUP_TYPE_VM){
		if (_init_vm_exit_table_size>=MAX_CLEANUP_TABLE_SIZE){
			SLL_UNREACHABLE();
		}
		_init_vm_exit_table[_init_vm_exit_table_size]=fn;
		_init_vm_exit_table_size++;
	}
	else{
		if (_init_exit_table_size>=MAX_CLEANUP_TABLE_SIZE){
			SLL_UNREACHABLE();
		}
		_init_exit_table[_init_exit_table_size]=fn;
		_init_exit_table_size++;
	}
}
