#include <sll/_sll_internal.h>
#include <sll/api/path.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <sll/var_arg.h>
#include <stdarg.h>



typedef struct __FUNCTION_LOG_DATA{
	const sll_string_t nm;
	sll_bool_t st;
} function_log_data_t;



typedef struct __FILE_LOG_DATA{
	const sll_string_t nm;
	function_log_data_t** dt;
	sll_array_length_t dtl;
	sll_bool_t st;
} file_log_data_t;



static sll_bool_t _log_default=0;
static file_log_data_t** _log_f_dt=NULL;
static sll_array_length_t _log_f_dtl=0;



static file_log_data_t* _get_file_index(const sll_char_t* fp){
	sll_string_length_t l;
	sll_string_length_t i=sll_path_split(SLL_CHAR(fp),&l);
	sll_string_t s;
	sll_string_from_pointer_length(SLL_CHAR(fp)+i,l-i,&s);
	sll_array_length_t j=0;
	for (;j<_log_f_dtl;j++){
		file_log_data_t* k=*(_log_f_dt+j);
		if (sll_string_equal(&s,&(k->nm))){
			sll_free_string(&s);
			return k;
		}
	}
	_log_f_dtl++;
	_log_f_dt=sll_reallocate(_log_f_dt,_log_f_dtl*sizeof(file_log_data_t*));
	file_log_data_t* n=sll_allocate(sizeof(file_log_data_t));
	sll_copy_data(&s,sizeof(sll_string_t),(sll_string_t*)(&(n->nm)));
	n->dt=NULL;
	n->dtl=0;
	n->st=_log_default;
	*(_log_f_dt+j)=n;
	return n;
}



static function_log_data_t* _get_func_index(file_log_data_t* f_dt,const sll_char_t* fn){
	sll_string_t s;
	sll_string_from_pointer(fn,&s);
	sll_array_length_t i=0;
	for (;i<f_dt->dtl;i++){
		function_log_data_t* k=*(f_dt->dt+i);
		if (sll_string_equal(&s,&(k->nm))){
			sll_free_string(&s);
			return k;
		}
	}
	f_dt->dtl++;
	f_dt->dt=sll_reallocate(f_dt->dt,f_dt->dtl*sizeof(function_log_data_t*));
	function_log_data_t* n=sll_allocate(sizeof(function_log_data_t));
	sll_copy_data(&s,sizeof(sll_string_t),(sll_string_t*)(&(n->nm)));
	n->st=f_dt->st;
	*(f_dt->dt+i)=n;
	return n;
}



__SLL_EXTERNAL void sll_log(const sll_char_t* fp,const sll_char_t* fn,const sll_char_t* t,...){
	if (!_log_default&&!_log_f_dtl){
		return;
	}
	file_log_data_t* f_dt=_get_file_index(fp);
	function_log_data_t* fn_dt=_get_func_index(f_dt,fn);
	if (!fn_dt->st){
		return;
	}
	va_list va;
	va_start(va,t);
	sll_var_arg_list_t dt={
		SLL_VAR_ARG_LIST_TYPE_C,
		{
			.c=&va
		}
	};
	sll_string_t s;
	sll_string_format_list(t,sll_string_length_unaligned(t),&dt,&s);
	va_end(va);
	sll_file_write_char(sll_stdout,'[');
	sll_file_write(sll_stdout,f_dt->nm.v,f_dt->nm.l);
	sll_file_write_char(sll_stdout,':');
	sll_file_write(sll_stdout,fn_dt->nm.v,fn_dt->nm.l);
	sll_file_write_char(sll_stdout,']');
	sll_file_write_char(sll_stdout,' ');
	sll_file_write(sll_stdout,s.v,s.l);
	sll_file_write_char(sll_stdout,'\n');
	sll_free_string(&s);
}



__SLL_EXTERNAL void sll_log_raw(const sll_char_t* fp,const sll_char_t* fn,const sll_string_t* s){
	if (!_log_default&&!_log_f_dtl){
		return;
	}
	file_log_data_t* f_dt=_get_file_index(fp);
	function_log_data_t* fn_dt=_get_func_index(f_dt,fn);
	if (!fn_dt->st){
		return;
	}
	sll_file_write_char(sll_stdout,'[');
	sll_file_write(sll_stdout,f_dt->nm.v,f_dt->nm.l);
	sll_file_write_char(sll_stdout,':');
	sll_file_write(sll_stdout,fn_dt->nm.v,fn_dt->nm.l);
	sll_file_write_char(sll_stdout,']');
	sll_file_write_char(sll_stdout,' ');
	sll_file_write(sll_stdout,s->v,s->l);
	sll_file_write_char(sll_stdout,'\n');
}



__SLL_EXTERNAL void sll_set_log_default(sll_bool_t st){
	_log_default=st;
}



__SLL_EXTERNAL void sll_set_log_file(const sll_char_t* fp,sll_bool_t st){
	_get_file_index(fp)->st=st;
}



__SLL_EXTERNAL void sll_set_log_function(const sll_char_t* fp,const sll_char_t* fn,sll_bool_t st){
	_get_func_index(_get_file_index(fp),fn)->st=st;
}
