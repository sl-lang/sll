#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/location.h>
#include <sll/platform.h>
#include <sll/scheduler.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/vm.h>



INIT_STRING_SETUP;



static sll_cleanup_function_t _util_exit_table[MAX_CLEANUP_TABLE_SIZE];
static unsigned int _util_exit_table_size=0;
static sll_bool_t _util_init=0;



static void _write_number(sll_file_descriptor_t fd,sll_file_offset_t n){
	if (!n){
		sll_platform_file_write(fd,"0",1);
		return;
	}
	sll_char_t bf[20];
	sll_string_length_t i=20;
	while (n){
		i--;
		bf[i]=(n%10)+48;
		n/=10;
	}
	sll_platform_file_write(fd,bf+i,20-i);
}



static void _write_stack_frame(sll_file_descriptor_t fd,sll_instruction_index_t ii){
	sll_platform_file_write(fd,"at ",3);
	sll_string_index_t fp_i;
	sll_string_index_t fn_i;
	sll_file_offset_t ln=sll_get_location(sll_current_runtime_data->a_dt,ii,&fp_i,&fn_i);
	const sll_string_t* str=sll_current_runtime_data->a_dt->st.dt+fp_i;
	sll_platform_file_write(fd,str->v,str->l);
	sll_platform_file_write(fd,":",1);
	_write_number(fd,ln);
	sll_platform_file_write(fd," (",2);
	if (fn_i==SLL_MAX_STRING_INDEX){
		sll_platform_file_write(fd,"@code)\n",7);
		return;
	}
	str=sll_current_runtime_data->a_dt->st.dt+fn_i;
	sll_platform_file_write(fd,str->v,str->l);
	sll_platform_file_write(fd,")\n",2);
}



__SLL_NO_RETURN void _force_exit(const sll_char_t* a,const sll_char_t* b,const sll_char_t* c){
	sll_file_flush(sll_stdout);
	sll_file_flush(sll_stderr);
	sll_file_descriptor_t fd=sll_platform_get_default_stream_descriptor(SLL_PLATFORM_STREAM_ERROR);
	sll_platform_file_write(fd,a,sll_string_length_unaligned(a));
	sll_platform_file_write(fd,b,sll_string_length_unaligned(b));
	sll_platform_file_write(fd,c,sll_string_length_unaligned(c));
	if (sll_current_runtime_data&&sll_current_runtime_data->a_dt){
		const sll_call_stack_t* c_st=sll_get_call_stack();
		if (c_st){
			_write_stack_frame(fd,sll_current_instruction_index);
			for (sll_call_stack_size_t i=0;i<c_st->l;i++){
				_write_stack_frame(fd,(c_st->dt+i)->_ii);
			}
			sll_platform_file_write(fd,"at <thread-",11);
			_write_number(fd,sll_current_thread_index);
			sll_platform_file_write(fd,">\n",2);
		}
	}
	_force_exit_platform();
}



__SLL_EXTERNAL void sll_deinit(void){
	if (!_util_init){
		return;
	}
	while (_util_exit_table_size){
		_util_exit_table_size--;
		_util_exit_table[_util_exit_table_size]();
	}
	const init_string_t*const* l=(const init_string_t*const*)(&__init_string_start);
	while (l<(const init_string_t*const*)(&__init_string_end)){
		const init_string_t* k=*l;
		if (k){
			sll_free_string(k->p);
		}
		l++;
	}
	_gc_release_data();
	_log_release_data();
	_file_release_std_streams();
	_memory_release_data();
	_reset_sandbox();
	_util_init=0;
}



__SLL_EXTERNAL void sll_init(void){
	if (_util_init){
		return;
	}
	_util_init=1;
	_file_init_std_streams();
	_init_platform();
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



__SLL_EXTERNAL void sll_register_cleanup(sll_cleanup_function_t f){
	if (_util_exit_table_size>=MAX_CLEANUP_TABLE_SIZE){
		SLL_UNREACHABLE();
	}
	_util_exit_table[_util_exit_table_size]=f;
	_util_exit_table_size++;
}
