#include <sll/_internal/common.h>
#include <sll/_internal/log.h>
#include <sll/_internal/print.h>
#include <sll/_internal/string.h>
#include <sll/common.h>
#include <sll/container.h>
#include <sll/data.h>
#include <sll/file.h>
#include <sll/log.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/var_arg.h>
#include <stdarg.h>



static sll_logger_flags_t _log_default=0;
static sll_container_t _log_data=SLL_CONTAINER_INIT_STRUCT;



static file_log_data_t* _get_file_data(const sll_char_t* file_path){
	sll_string_t file_path_str;
	sll_string_from_pointer(file_path,&file_path_str);
	SLL_CONTAINER_ITER(&_log_data,file_log_data_t*,file,{
		if (STRING_EQUAL(&file_path_str,&(file->name))){
			sll_free_string(&file_path_str);
			return file;
		}
	});
	file_log_data_t* data=sll_allocate(sizeof(file_log_data_t));
	sll_copy_data(&file_path_str,sizeof(sll_string_t),(sll_string_t*)(&(data->name)));
	SLL_CONTAINER_INIT(&(data->functions));
	data->length=0;
	data->flags=_log_default;
	SLL_CONTAINER_PUSH(&_log_data,data);
	return data;
}



static function_log_data_t* _get_function_data(file_log_data_t* file,const sll_char_t* func){
	sll_string_t func_str;
	sll_string_from_pointer(func,&func_str);
	SLL_CONTAINER_ITER(&(file->functions),function_log_data_t*,function,{
		if (STRING_EQUAL(&func_str,&(function->name))){
			sll_free_string(&func_str);
			return function;
		}
	});
	function_log_data_t* data=sll_allocate(sizeof(function_log_data_t));
	sll_copy_data(&func_str,sizeof(sll_string_t),(sll_string_t*)(&(data->name)));
	data->flags=file->flags;
	SLL_CONTAINER_PUSH(&(file->functions),data);
	return data;
}



static void _log_location(const sll_string_t* file_path,const sll_string_t* func,sll_file_offset_t line,sll_file_t* wf){
	sll_file_write_char(sll_stdout,'[',NULL);
	sll_file_write(sll_stdout,file_path->data,file_path->length,NULL);
	sll_file_write_char(sll_stdout,':',NULL);
	SLL_ASSERT(line);
	sll_char_t bf[20];
	sll_string_length_t i=0;
	while (line){
		bf[i]=line%10;
		line/=10;
		i++;
	}
	while (i){
		i--;
		sll_file_write_char(wf,bf[i]+48,NULL);
	}
	sll_file_write_char(sll_stdout,'(',NULL);
	sll_file_write(sll_stdout,func->data,func->length,NULL);
	PRINT_STATIC_STRING(")] ",sll_stdout);
}



void _log_release_data(void){
	SLL_CONTAINER_ITER(&_log_data,file_log_data_t*,file,{
		sll_free_string((sll_string_t*)(&(file->name)));
		SLL_CONTAINER_ITER(&(file->functions),function_log_data_t*,function,{
			sll_free_string((sll_string_t*)(&(function->name)));
			sll_deallocate(function);
		});
		SLL_CONTAINER_CLEAR(&(file->functions));
		sll_deallocate(file);
	});
	SLL_CONTAINER_CLEAR(&_log_data);
	_log_default=0;
}



__SLL_EXTERNAL sll_bool_t sll_log(const sll_char_t* file_path,const sll_char_t* function,sll_file_offset_t line,sll_bool_t is_warning,const sll_char_t* format,...){
	file_log_data_t* f_dt=_get_file_data(file_path);
	function_log_data_t* fn_dt=_get_function_data(f_dt,function);
	if (!is_warning&&!(fn_dt->flags&SLL_LOG_FLAG_SHOW)){
		return 0;
	}
	va_list va;
	va_start(va,format);
	sll_var_arg_list_t dt;
	SLL_VAR_ARG_INIT_C(&dt,&va);
	sll_string_t str;
	sll_string_format_list(format,sll_string_length(format),&dt,&str);
	va_end(va);
	if (!(fn_dt->flags&SLL_LOG_FLAG_NO_HEADER)){
		_log_location(&(f_dt->name),&(fn_dt->name),line,sll_stdout);
	}
	sll_file_write(sll_stdout,str.data,str.length,NULL);
	sll_file_write_char(sll_stdout,'\n',NULL);
	sll_free_string(&str);
	return 1;
}



__SLL_EXTERNAL sll_bool_t sll_log_raw(const sll_char_t* file_path,const sll_char_t* function,sll_file_offset_t line,sll_bool_t is_warning,const sll_string_t* string){
	if (!_log_default&&!_log_data.size){
		return 0;
	}
	file_log_data_t* f_dt=_get_file_data(file_path);
	function_log_data_t* fn_dt=_get_function_data(f_dt,function);
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
	sll_logger_flags_t* ptr;
	if (!file_path){
		ptr=&_log_default;
	}
	else{
		file_log_data_t* dt=_get_file_data(file_path);
		ptr=(function?&(_get_function_data(dt,function)->flags):&(dt->flags));
	}
	sll_logger_flags_t tmp=flags;
	flags&=SLL_LOG_FLAG_SHOW|SLL_LOG_FLAG_NO_HEADER;
	*ptr=(state?(*ptr)|flags:(*ptr)&(~flags));
	return (tmp==flags);
}
