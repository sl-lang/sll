#include <sll/api/path.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/file.h>
#include <sll/internal/common.h>
#include <sll/internal/log.h>
#include <sll/internal/print.h>
#include <sll/log.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/var_arg.h>
#include <stdarg.h>



static sll_flags_t _log_default=0;
static file_log_data_t** _log_f_dt=NULL;
static sll_array_length_t _log_f_dtl=0;



static file_log_data_t* _get_file_index(const sll_char_t* fp){
	sll_string_t fp_s;
	sll_string_from_pointer(fp,&fp_s);
	sll_string_length_t i=sll_path_split(&fp_s);
	sll_string_t s;
	sll_string_from_pointer_length(fp+i,fp_s.l-i,&s);
	sll_free_string(&fp_s);
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
	n->fl=_log_default;
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
	n->fl=f_dt->fl;
	*(f_dt->dt+i)=n;
	return n;
}



static void _log_location(const sll_string_t* fp,const sll_string_t* fn,sll_file_offset_t ln,sll_file_t* wf){
	sll_file_write_char(sll_stdout,'[',NULL);
	sll_file_write(sll_stdout,fp->v,fp->l,NULL);
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
	sll_file_write(sll_stdout,fn->v,fn->l,NULL);
	PRINT_STATIC_STRING(")] ",sll_stdout);
}



void _log_release_data(void){
	while (_log_f_dtl){
		_log_f_dtl--;
		file_log_data_t* k=*(_log_f_dt+_log_f_dtl);
		sll_free_string((sll_string_t*)(&(k->nm)));
		for (sll_arg_count_t i=0;i<k->dtl;i++){
			function_log_data_t* e=*(k->dt+i);
			sll_free_string((sll_string_t*)(&(e->nm)));
			sll_deallocate(e);
		}
		sll_deallocate(k->dt);
		sll_deallocate(k);
	}
	sll_deallocate(_log_f_dt);
	_log_f_dt=NULL;
}



__SLL_EXTERNAL void sll_log(const sll_char_t* fp,const sll_char_t* fn,sll_file_offset_t ln,sll_bool_t w,const sll_char_t* t,...){
	file_log_data_t* f_dt=_get_file_index(fp);
	function_log_data_t* fn_dt=_get_func_index(f_dt,fn);
	if (!w&&!(fn_dt->fl&SLL_LOG_FLAG_SHOW)){
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
	if (!(fn_dt->fl&SLL_LOG_FLAG_NO_HEADER)){
		_log_location(&(f_dt->nm),&(fn_dt->nm),ln,sll_stdout);
	}
	sll_file_write(sll_stdout,s.v,s.l,NULL);
	sll_file_write_char(sll_stdout,'\n',NULL);
	sll_free_string(&s);
}



__SLL_EXTERNAL void sll_log_raw(const sll_char_t* fp,const sll_char_t* fn,sll_file_offset_t ln,sll_bool_t w,const sll_string_t* s){
	if (!_log_default&&!_log_f_dtl){
		return;
	}
	file_log_data_t* f_dt=_get_file_index(fp);
	function_log_data_t* fn_dt=_get_func_index(f_dt,fn);
	if (!w&&!(fn_dt->fl&SLL_LOG_FLAG_SHOW)){
		return;
	}
	if (!(fn_dt->fl&SLL_LOG_FLAG_NO_HEADER)){
		_log_location(&(f_dt->nm),&(fn_dt->nm),ln,sll_stdout);
	}
	sll_file_write(sll_stdout,s->v,s->l,NULL);
	sll_file_write_char(sll_stdout,'\n',NULL);
}



__SLL_EXTERNAL void sll_set_log_default(sll_flags_t fl,sll_bool_t st){
	if (st){
		_log_default|=fl;
	}
	else{
		_log_default&=fl;
	}
}



__SLL_EXTERNAL void sll_set_log_file(const sll_char_t* fp,sll_flags_t fl,sll_bool_t st){
	file_log_data_t* dt=_get_file_index(fp);
	if (st){
		dt->fl|=fl;
	}
	else{
		dt->fl&=~fl;
	}
}



__SLL_EXTERNAL void sll_set_log_function(const sll_char_t* fp,const sll_char_t* fn,sll_flags_t fl,sll_bool_t st){
	function_log_data_t* dt=_get_func_index(_get_file_index(fp),fn);
	if (st){
		dt->fl|=fl;
	}
	else{
		dt->fl&=~fl;
	}
}
