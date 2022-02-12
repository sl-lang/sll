#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>



INIT_STRING_SETUP;



static sll_cleanup_function_t _util_exit_table[MAX_CLEANUP_TABLE_SIZE];
static unsigned int _util_exit_table_size=0;
static sll_bool_t _util_init=0;



__SLL_NO_RETURN void _force_exit(const sll_char_t* a,const sll_char_t* b,const sll_char_t* c){
	sll_file_flush(sll_stdout);
	sll_file_flush(sll_stderr);
	sll_file_descriptor_t fd=sll_platform_get_default_stream_descriptor(SLL_PLATFORM_STREAM_ERROR);
	sll_platform_file_write(fd,a,sll_string_length_unaligned(a));
	sll_platform_file_write(fd,b,sll_string_length_unaligned(b));
	sll_platform_file_write(fd,c,sll_string_length_unaligned(c));
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
	const init_string_t*const* l=&__init_string_start;
	while (l<&__init_string_end){
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
	const init_string_t*const* l=&__init_string_start;
	while (l<&__init_string_end){
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
