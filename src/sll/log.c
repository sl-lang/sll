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



static sll_container_t _log_data=SLL_CONTAINER_INIT_STRUCT;
static sll_logger_flags_t _log_default=0;



static log_file_data_t* _get_file_data(const sll_char_t* file_path){
	sll_string_t file_path_str;
	sll_string_from_pointer(file_path,&file_path_str);
	SLL_CONTAINER_ITER(&_log_data,log_file_data_t*,file,{
		if (STRING_EQUAL(&file_path_str,&(file->name))){
			sll_free_string(&file_path_str);
			return file;
		}
	});
	log_file_data_t* data=sll_allocate(sizeof(log_file_data_t));
	sll_copy_data(&file_path_str,sizeof(sll_string_t),(sll_string_t*)(&(data->name)));
	SLL_CONTAINER_INIT(&(data->functions));
	data->flags=_log_default;
	SLL_CONTAINER_PUSH(&_log_data,log_file_data_t*,data);
	return data;
}



static log_function_data_t* _get_function_data(log_file_data_t* file,const sll_char_t* function_name){
	sll_string_t function_name_str;
	sll_string_from_pointer(function_name,&function_name_str);
	SLL_CONTAINER_ITER(&(file->functions),log_function_data_t*,function,{
		if (STRING_EQUAL(&function_name_str,&(function->name))){
			sll_free_string(&function_name_str);
			return function;
		}
	});
	log_function_data_t* data=sll_allocate(sizeof(log_function_data_t));
	sll_copy_data(&function_name_str,sizeof(sll_string_t),(sll_string_t*)(&(data->name)));
	data->flags=file->flags;
	SLL_CONTAINER_PUSH(&(file->functions),log_function_data_t*,data);
	return data;
}



static void _log_location(const sll_string_t* file_path,const sll_string_t* function_name,sll_file_offset_t line,sll_file_t* wf){
	sll_file_write_char(sll_stdout,'[',NULL);
	sll_file_write(sll_stdout,file_path->data,file_path->length,NULL);
	sll_file_write_char(sll_stdout,':',NULL);
	SLL_ASSERT(line);
	sll_char_t bf[20];
	sll_string_length_t i=20;
	while (line){
		i--;
		bf[i]=(line%10)+48;
		line/=10;
	}
	sll_file_write(sll_stdout,bf+i,20-i,NULL);
	sll_file_write_char(sll_stdout,'(',NULL);
	sll_file_write(sll_stdout,function_name->data,function_name->length,NULL);
	PRINT_STATIC_STRING(")] ",sll_stdout);
}



void _log_release_data(void){
	SLL_CONTAINER_ITER_CLEAR(&_log_data,log_file_data_t*,file,{
		sll_free_string((sll_string_t*)(&(file->name)));
		SLL_CONTAINER_ITER_CLEAR(&(file->functions),log_function_data_t*,function,{
			sll_free_string((sll_string_t*)(&(function->name)));
			sll_deallocate(function);
		});
		sll_deallocate(file);
	});
	_log_default=0;
}



__SLL_EXTERNAL sll_bool_t sll_log(const sll_char_t* file_path,const sll_char_t* function_name,sll_file_offset_t line,sll_bool_t is_warning,const sll_char_t* format,...){
	log_file_data_t* file=_get_file_data(file_path);
	log_function_data_t* function=_get_function_data(file,function_name);
	if (!is_warning&&!(function->flags&SLL_LOG_FLAG_SHOW)){
		return 0;
	}
	va_list va;
	va_start(va,format);
	sll_var_arg_list_t dt;
	SLL_VAR_ARG_INIT_C(&dt,&va);
	sll_string_t str;
	sll_string_format_list(format,sll_string_length(format),&dt,&str);
	va_end(va);
	if (!(function->flags&SLL_LOG_FLAG_NO_HEADER)){
		_log_location(&(file->name),&(function->name),line,sll_stdout);
	}
	sll_file_write(sll_stdout,str.data,str.length,NULL);
	sll_file_write_char(sll_stdout,'\n',NULL);
	sll_free_string(&str);
	return 1;
}



__SLL_EXTERNAL sll_bool_t sll_log_raw(const sll_char_t* file_path,const sll_char_t* function_name,sll_file_offset_t line,sll_bool_t is_warning,const sll_string_t* string){
	if (!_log_default&&!_log_data.size){
		return 0;
	}
	log_file_data_t* file=_get_file_data(file_path);
	log_function_data_t* function=_get_function_data(file,function_name);
	if (!is_warning&&!(function->flags&SLL_LOG_FLAG_SHOW)){
		return 0;
	}
	if (!(function->flags&SLL_LOG_FLAG_NO_HEADER)){
		_log_location(&(file->name),&(function->name),line,sll_stdout);
	}
	sll_file_write(sll_stdout,string->data,string->length,NULL);
	sll_file_write_char(sll_stdout,'\n',NULL);
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_set_log_flags(const sll_char_t* file_path,const sll_char_t* function_name,sll_logger_flags_t flags,sll_bool_t state){
	sll_logger_flags_t* ptr;
	if (!file_path){
		ptr=&_log_default;
	}
	else{
		log_file_data_t* file=_get_file_data(file_path);
		ptr=(function_name?&(_get_function_data(file,function_name)->flags):&(file->flags));
	}
	sll_logger_flags_t tmp=flags;
	flags&=SLL_LOG_FLAG_SHOW|SLL_LOG_FLAG_NO_HEADER;
	*ptr=(state?(*ptr)|flags:(*ptr)&(~flags));
	return (tmp==flags);
}
