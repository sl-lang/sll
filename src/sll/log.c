#include <sll/_internal/common.h>
#include <sll/_internal/log.h>
#include <sll/_internal/print.h>
#include <sll/_internal/string.h>
#include <sll/api/path.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/file.h>
#include <sll/log.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/var_arg.h>
#include <stdarg.h>



#define SET_OR_CLEAR(data,flags,set) ((data)^=((-((sll_logger_flags_t)set))^(data))&(flags))



static sll_logger_flags_t _log_default=0;
static file_log_data_t** _log_file_data=NULL;
static sll_array_length_t _log_file_data_len=0;



static file_log_data_t* _get_file_index(const sll_char_t* fp){
	sll_string_t fp_s;
	sll_string_from_pointer(fp,&fp_s);
	sll_string_length_t i=sll_path_split(&fp_s);
	sll_string_t s;
	sll_string_from_pointer_length(fp+i,fp_s.length-i,&s);
	sll_free_string(&fp_s);
	sll_array_length_t j=0;
	for (;j<_log_file_data_len;j++){
		file_log_data_t* k=*(_log_file_data+j);
		if (STRING_EQUAL(&s,&(k->name))){
			sll_free_string(&s);
			return k;
		}
	}
	_log_file_data_len++;
	_log_file_data=sll_reallocate(_log_file_data,_log_file_data_len*sizeof(file_log_data_t*));
	file_log_data_t* n=sll_allocate(sizeof(file_log_data_t));
	sll_copy_data(&s,sizeof(sll_string_t),(sll_string_t*)(&(n->name)));
	n->data=NULL;
	n->length=0;
	n->flags=_log_default;
	*(_log_file_data+j)=n;
	return n;
}



static function_log_data_t* _get_func_index(file_log_data_t* f_dt,const sll_char_t* fn){
	sll_string_t s;
	sll_string_from_pointer(fn,&s);
	sll_array_length_t i=0;
	for (;i<f_dt->length;i++){
		function_log_data_t* k=*(f_dt->data+i);
		if (STRING_EQUAL(&s,&(k->name))){
			sll_free_string(&s);
			return k;
		}
	}
	f_dt->length++;
	f_dt->data=sll_reallocate(f_dt->data,f_dt->length*sizeof(function_log_data_t*));
	function_log_data_t* n=sll_allocate(sizeof(function_log_data_t));
	sll_copy_data(&s,sizeof(sll_string_t),(sll_string_t*)(&(n->name)));
	n->flags=f_dt->flags;
	*(f_dt->data+i)=n;
	return n;
}



static void _log_location(const sll_string_t* fp,const sll_string_t* fn,sll_file_offset_t ln,sll_file_t* wf){
	sll_file_write_char(sll_stdout,'[',NULL);
	sll_file_write(sll_stdout,fp->data,fp->length,NULL);
	sll_file_write_char(sll_stdout,':',NULL);
	SLL_ASSERT(ln);
	sll_char_t bf[20];
	sll_string_length_t i=0;
	while (ln){
		bf[i]=ln%10;
		ln/=10;
		i++;
	}
	while (i){
		i--;
		sll_file_write_char(wf,bf[i]+48,NULL);
	}
	sll_file_write_char(sll_stdout,'(',NULL);
	sll_file_write(sll_stdout,fn->data,fn->length,NULL);
	PRINT_STATIC_STRING(")] ",sll_stdout);
}



void _log_release_data(void){
	while (_log_file_data_len){
		_log_file_data_len--;
		file_log_data_t* k=*(_log_file_data+_log_file_data_len);
		sll_free_string((sll_string_t*)(&(k->name)));
		for (sll_arg_count_t i=0;i<k->length;i++){
			function_log_data_t* e=*(k->data+i);
			sll_free_string((sll_string_t*)(&(e->name)));
			sll_deallocate(e);
		}
		sll_deallocate(k->data);
		sll_deallocate(k);
	}
	sll_deallocate(_log_file_data);
	_log_file_data=NULL;
}



__SLL_EXTERNAL sll_bool_t sll_log(const sll_char_t* file_path,const sll_char_t* function,sll_file_offset_t line,sll_bool_t is_warning,const sll_char_t* format,...){
	file_log_data_t* f_dt=_get_file_index(file_path);
	function_log_data_t* fn_dt=_get_func_index(f_dt,function);
	if (!is_warning&&!(fn_dt->flags&SLL_LOG_FLAG_SHOW)){
		return 0;
	}
	va_list va;
	va_start(va,format);
	sll_var_arg_list_t dt;
	SLL_VAR_ARG_INIT_C(&dt,&va);
	sll_string_t s;
	sll_string_format_list(format,sll_string_length(format),&dt,&s);
	va_end(va);
	if (!(fn_dt->flags&SLL_LOG_FLAG_NO_HEADER)){
		_log_location(&(f_dt->name),&(fn_dt->name),line,sll_stdout);
	}
	sll_file_write(sll_stdout,s.data,s.length,NULL);
	sll_file_write_char(sll_stdout,'\n',NULL);
	sll_free_string(&s);
	return 1;
}



__SLL_EXTERNAL sll_bool_t sll_log_raw(const sll_char_t* file_path,const sll_char_t* function,sll_file_offset_t line,sll_bool_t is_warning,const sll_string_t* string){
	if (!_log_default&&!_log_file_data_len){
		return 0;
	}
	file_log_data_t* f_dt=_get_file_index(file_path);
	function_log_data_t* fn_dt=_get_func_index(f_dt,function);
	if (!is_warning&&!(fn_dt->flags&SLL_LOG_FLAG_SHOW)){
		return 0;
	}
	if (!(fn_dt->flags&SLL_LOG_FLAG_NO_HEADER)){
		_log_location(&(f_dt->name),&(fn_dt->name),line,sll_stdout);
	}
	sll_file_write(sll_stdout,string->data,string->length,NULL);
	sll_file_write_char(sll_stdout,'\n',NULL);
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_set_log_flags(const sll_char_t* file_path,const sll_char_t* function,sll_logger_flags_t flags,sll_bool_t state){
	sll_logger_flags_t tmp=flags;
	flags&=SLL_LOG_FLAG_SHOW|SLL_LOG_FLAG_NO_HEADER;
	sll_bool_t o=(tmp==flags);
	if (file_path){
		file_log_data_t* dt=_get_file_index(file_path);
		if (function){
			function_log_data_t* fn_dt=_get_func_index(dt,function);
			SET_OR_CLEAR(fn_dt->flags,flags,state);
		}
		else{
			SET_OR_CLEAR(dt->flags,flags,state);
		}
	}
	else{
		SET_OR_CLEAR(_log_default,flags,state);
	}
	return o;
}
