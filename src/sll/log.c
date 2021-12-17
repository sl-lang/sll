#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/var_arg.h>



typedef struct __FILE_LOG_DATA{
	sll_string_t s;
	sll_bool_t st;
} file_log_data_t;



static sll_bool_t _log_default=0;
static file_log_data_t** _log_f_dt=NULL;
static sll_array_length_t _log_f_dtl=0;



__SLL_EXTERNAL void sll_log(const char* fp,const char* fn,const sll_char_t* t,...){
	if (!_log_default&&!_log_f_dtl){
		return;
	}
	sll_string_t fp_s;
	sll_string_from_pointer(SLL_CHAR(fp),&fp_s);
	for (sll_array_length_t i=0;i<_log_f_dtl;i++){
		file_log_data_t* k=*(_log_f_dt+i);
		if (sll_string_equal(&fp_s,&(k->s))){
			if (!k->st){
				return;
			}
			goto _found_state;
		}
	}
	_log_f_dtl++;
	_log_f_dt=sll_reallocate(_log_f_dt,_log_f_dtl*sizeof(file_log_data_t*));
	file_log_data_t* n=sll_allocate(sizeof(file_log_data_t));
	n->s=fp_s;
	n->st=_log_default;
	*(_log_f_dt+_log_f_dtl-1)=n;
	if (!_log_default){
		return;
	}
_found_state:;
	va_list va;
	va_start(va,t);
	sll_var_arg_list_t dt={
		SLL_VAR_ARG_LIST_TYPE_C,
		{
			.c=&va
		}
	};
	sll_string_t str;
	sll_string_format_list(t,sll_string_length_unaligned(t),&dt,&str);
	va_end(va);
	sll_file_write(sll_stdout,str.v,str.l);
	sll_free_string(&str);
}



__SLL_EXTERNAL void sll_log_set_default(sll_bool_t d){
	_log_default=d;
}
