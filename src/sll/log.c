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
		if (STRING_EQUAL(&s,&(k->nm))){
			sll_free_string(&s);
			return k;
		}
	}
	_log_file_data_len++;
	_log_file_data=sll_reallocate(_log_file_data,_log_file_data_len*sizeof(file_log_data_t*));
	file_log_data_t* n=sll_allocate(sizeof(file_log_data_t));
	sll_copy_data(&s,sizeof(sll_string_t),(sll_string_t*)(&(n->nm)));
	n->dt=NULL;
	n->dtl=0;
	n->fl=_log_default;
	*(_log_file_data+j)=n;
	return n;
}



static function_log_data_t* _get_func_index(file_log_data_t* f_dt,const sll_char_t* fn){
	sll_string_t s;
	sll_string_from_pointer(fn,&s);
	sll_array_length_t i=0;
	for (;i<f_dt->dtl;i++){
		function_log_data_t* k=*(f_dt->dt+i);
		if (STRING_EQUAL(&s,&(k->nm))){
			sll_free_string(&s);
			return k;
		}
	}
	f_dt->dtl++;
	f_dt->dt=sll_reallocate(f_dt->dt,f_dt->dtl*sizeof(function_log_data_t*));
	function_log_data_t* n=sll_allocate(sizeof(function_log_data_t));
	sll_copy_data(&s,sizeof(sll_string_t),(sll_string_t*)(&(n->nm)));
	n->fl=f_dt->fl;
	*(f_dt->dt+i)=n;
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
		sll_free_string((sll_string_t*)(&(k->nm)));
		for (sll_arg_count_t i=0;i<k->dtl;i++){
			function_log_data_t* e=*(k->dt+i);
			sll_free_string((sll_string_t*)(&(e->nm)));
			sll_deallocate(e);
		}
		sll_deallocate(k->dt);
		sll_deallocate(k);
	}
	sll_deallocate(_log_file_data);
	_log_file_data=NULL;
}



__SLL_EXTERNAL sll_bool_t sll_log(const sll_char_t* fp,const sll_char_t* fn,sll_file_offset_t ln,sll_bool_t w,const sll_char_t* t,...){
	file_log_data_t* f_dt=_get_file_index(fp);
	function_log_data_t* fn_dt=_get_func_index(f_dt,fn);
	if (!w&&!(fn_dt->fl&SLL_LOG_FLAG_SHOW)){
		return 0;
	}
	va_list va;
	va_start(va,t);
	sll_var_arg_list_t dt;
	SLL_VAR_ARG_INIT_C(&dt,&va);
	sll_string_t s;
	sll_string_format_list(t,sll_string_length(t),&dt,&s);
	va_end(va);
	if (!(fn_dt->fl&SLL_LOG_FLAG_NO_HEADER)){
		_log_location(&(f_dt->nm),&(fn_dt->nm),ln,sll_stdout);
	}
	sll_file_write(sll_stdout,s.data,s.length,NULL);
	sll_file_write_char(sll_stdout,'\n',NULL);
	sll_free_string(&s);
	return 1;
}



__SLL_EXTERNAL sll_bool_t sll_log_raw(const sll_char_t* fp,const sll_char_t* fn,sll_file_offset_t ln,sll_bool_t w,const sll_string_t* s){
	if (!_log_default&&!_log_file_data_len){
		return 0;
	}
	file_log_data_t* f_dt=_get_file_index(fp);
	function_log_data_t* fn_dt=_get_func_index(f_dt,fn);
	if (!w&&!(fn_dt->fl&SLL_LOG_FLAG_SHOW)){
		return 0;
	}
	if (!(fn_dt->fl&SLL_LOG_FLAG_NO_HEADER)){
		_log_location(&(f_dt->nm),&(fn_dt->nm),ln,sll_stdout);
	}
	sll_file_write(sll_stdout,s->data,s->length,NULL);
	sll_file_write_char(sll_stdout,'\n',NULL);
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_set_log_flags(const sll_char_t* fp,const sll_char_t* fn,sll_logger_flags_t fl,sll_bool_t st){
	sll_logger_flags_t tmp=fl;
	fl&=SLL_LOG_FLAG_SHOW|SLL_LOG_FLAG_NO_HEADER;
	sll_bool_t o=(tmp==fl);
	if (fp){
		file_log_data_t* dt=_get_file_index(fp);
		if (fn){
			function_log_data_t* fn_dt=_get_func_index(dt,fn);
			SET_OR_CLEAR(fn_dt->fl,fl,st);
		}
		else{
			SET_OR_CLEAR(dt->fl,fl,st);
		}
	}
	else{
		SET_OR_CLEAR(_log_default,fl,st);
	}
	return o;
}
