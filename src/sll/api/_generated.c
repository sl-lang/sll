// WARNING: This is an auto-generated file. Any changes made to this file might be lost at any moment. Do Not Edit!
#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/_generated.h>
#include <sll/common.h>
#include <sll/constants.h>
#include <sll/static_object.h>
#include <sll/types.h>



extern __SLL_API_TYPE_sll_api_file_close sll_api_file_close(__SLL_API_ARGS_sll_api_file_close);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_file_close_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!((SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_HANDLE&&a->dt.h.t!=SLL_UNKNOWN_HANDLE_TYPE))){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_file_close(&(a->dt.h));
	return SLL_FROM_INT(out);
}
INTERNAL_FUNCTION("file_close",sll_api_file_close_raw,SLL_INTERNAL_FUNCTION_TYPE_REQUIRED);



extern __SLL_API_TYPE_sll_api_file_open sll_api_file_open(__SLL_API_ARGS_sll_api_file_open);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_file_open_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(handle_zero);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(handle_zero);
	}
	const sll_runtime_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (!(SLL_RUNTIME_OBJECT_GET_TYPE(b)==SLL_RUNTIME_OBJECT_TYPE_STRING)){
			return SLL_ACQUIRE_STATIC(handle_zero);
		}
	}
	sll_handle_data_t out;
	sll_api_file_open(&(a->dt.s),(b?&(b->dt.s):NULL),&out);
	sll_runtime_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_RUNTIME_OBJECT_TYPE_HANDLE;
	out_o->dt.h=out;
	return out_o;
}
INTERNAL_FUNCTION("file_open",sll_api_file_open_raw,SLL_INTERNAL_FUNCTION_TYPE_REQUIRED);



extern __SLL_API_TYPE_sll_api_file_write sll_api_file_write(__SLL_API_ARGS_sll_api_file_write);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_file_write_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<2){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!((SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_HANDLE&&a->dt.h.t!=SLL_UNKNOWN_HANDLE_TYPE)||SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_INT)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t*const* b=al+1;
	sll_arg_count_t bc=all-1;
	sll_integer_t out=sll_api_file_write(a,b,bc);
	return SLL_FROM_INT(out);
}
INTERNAL_FUNCTION("file_write",sll_api_file_write_raw,SLL_INTERNAL_FUNCTION_TYPE_REQUIRED);



extern __SLL_API_TYPE_sll_api_json_parse sll_api_json_parse(__SLL_API_ARGS_sll_api_json_parse);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_json_parse_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(handle_zero);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(handle_zero);
	}
	return sll_api_json_parse(&(a->dt.s));
}
INTERNAL_FUNCTION("json_parse",sll_api_json_parse_raw,SLL_INTERNAL_FUNCTION_TYPE_DEFAULT);



extern __SLL_API_TYPE_sll_api_json_stringify sll_api_json_stringify(__SLL_API_ARGS_sll_api_json_stringify);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_json_stringify_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_runtime_object_t* a=*(al+0);
	sll_string_t out;
	sll_api_json_stringify(a,&out);
	sll_runtime_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}
INTERNAL_FUNCTION("json_stringify",sll_api_json_stringify_raw,SLL_INTERNAL_FUNCTION_TYPE_DEFAULT);



extern __SLL_API_TYPE_sll_api_json_type sll_api_json_type(__SLL_API_ARGS_sll_api_json_type);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_json_type_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(handle_zero);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_INT)){
		return SLL_ACQUIRE_STATIC(handle_zero);
	}
	sll_handle_data_t out;
	sll_api_json_type(a->dt.i,&out);
	sll_runtime_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_RUNTIME_OBJECT_TYPE_HANDLE;
	out_o->dt.h=out;
	return out_o;
}
INTERNAL_FUNCTION("json_type",sll_api_json_type_raw,SLL_INTERNAL_FUNCTION_TYPE_DEFAULT);



extern __SLL_API_TYPE_sll_api_path_absolute sll_api_path_absolute(__SLL_API_ARGS_sll_api_path_absolute);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_path_absolute_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	sll_string_t out;
	sll_api_path_absolute(&(a->dt.s),&out);
	sll_runtime_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}
INTERNAL_FUNCTION("path_absolute",sll_api_path_absolute_raw,SLL_INTERNAL_FUNCTION_TYPE_DEFAULT);



extern __SLL_API_TYPE_sll_api_path_exists sll_api_path_exists(__SLL_API_ARGS_sll_api_path_exists);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_path_exists_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_path_exists(&(a->dt.s));
	return SLL_FROM_INT(out);
}
INTERNAL_FUNCTION("path_exists",sll_api_path_exists_raw,SLL_INTERNAL_FUNCTION_TYPE_DEFAULT);



extern __SLL_API_TYPE_sll_api_path_is_dir sll_api_path_is_dir(__SLL_API_ARGS_sll_api_path_is_dir);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_path_is_dir_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_path_is_dir(&(a->dt.s));
	return SLL_FROM_INT(out);
}
INTERNAL_FUNCTION("path_is_dir",sll_api_path_is_dir_raw,SLL_INTERNAL_FUNCTION_TYPE_DEFAULT);



extern __SLL_API_TYPE_sll_api_path_relative sll_api_path_relative(__SLL_API_ARGS_sll_api_path_relative);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_path_relative_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	sll_string_t out;
	sll_api_path_relative(&(a->dt.s),&out);
	sll_runtime_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}
INTERNAL_FUNCTION("path_relative",sll_api_path_relative_raw,SLL_INTERNAL_FUNCTION_TYPE_DEFAULT);



extern __SLL_API_TYPE_sll_api_path_size sll_api_path_size(__SLL_API_ARGS_sll_api_path_size);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_path_size_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_path_size(&(a->dt.s));
	return SLL_FROM_INT(out);
}
INTERNAL_FUNCTION("path_size",sll_api_path_size_raw,SLL_INTERNAL_FUNCTION_TYPE_DEFAULT);



extern __SLL_API_TYPE_sll_api_string_convert sll_api_string_convert(__SLL_API_ARGS_sll_api_string_convert);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_string_convert_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_runtime_object_t*const* a=al+0;
	sll_arg_count_t ac=all-0;
	sll_string_t out;
	sll_api_string_convert(a,ac,&out);
	sll_runtime_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}
INTERNAL_FUNCTION("string_convert",sll_api_string_convert_raw,SLL_INTERNAL_FUNCTION_TYPE_DEFAULT);



extern __SLL_API_TYPE_sll_api_string_length sll_api_string_length(__SLL_API_ARGS_sll_api_string_length);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_string_length_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t*const* a=al+0;
	sll_arg_count_t ac=all-0;
	sll_integer_t out=sll_api_string_length(a,ac);
	return SLL_FROM_INT(out);
}
INTERNAL_FUNCTION("string_length",sll_api_string_length_raw,SLL_INTERNAL_FUNCTION_TYPE_DEFAULT);



extern __SLL_API_TYPE_sll_api_string_to_lower_case sll_api_string_to_lower_case(__SLL_API_ARGS_sll_api_string_to_lower_case);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_string_to_lower_case_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	sll_string_t out;
	sll_api_string_to_lower_case(&(a->dt.s),&out);
	sll_runtime_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}
INTERNAL_FUNCTION("string_to_lower_case",sll_api_string_to_lower_case_raw,SLL_INTERNAL_FUNCTION_TYPE_DEFAULT);



extern __SLL_API_TYPE_sll_api_string_to_upper_case sll_api_string_to_upper_case(__SLL_API_ARGS_sll_api_string_to_upper_case);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_string_to_upper_case_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	sll_string_t out;
	sll_api_string_to_upper_case(&(a->dt.s),&out);
	sll_runtime_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}
INTERNAL_FUNCTION("string_to_upper_case",sll_api_string_to_upper_case_raw,SLL_INTERNAL_FUNCTION_TYPE_DEFAULT);



extern __SLL_API_TYPE_sll_api_sys_arg_get sll_api_sys_arg_get(__SLL_API_ARGS_sll_api_sys_arg_get);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_sys_arg_get_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_INT)){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	return sll_api_sys_arg_get(a->dt.i);
}
INTERNAL_FUNCTION("sys_arg_get",sll_api_sys_arg_get_raw,SLL_INTERNAL_FUNCTION_TYPE_DEFAULT);



extern __SLL_API_TYPE_sll_api_sys_arg_get_count sll_api_sys_arg_get_count(__SLL_API_ARGS_sll_api_sys_arg_get_count);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_sys_arg_get_count_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	sll_integer_t out=sll_api_sys_arg_get_count();
	return SLL_FROM_INT(out);
}
INTERNAL_FUNCTION("sys_arg_get_count",sll_api_sys_arg_get_count_raw,SLL_INTERNAL_FUNCTION_TYPE_DEFAULT);



extern __SLL_API_TYPE_sll_api_sys_get_platform sll_api_sys_get_platform(__SLL_API_ARGS_sll_api_sys_get_platform);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_sys_get_platform_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	return sll_api_sys_get_platform();
}
INTERNAL_FUNCTION("sys_get_platform",sll_api_sys_get_platform_raw,SLL_INTERNAL_FUNCTION_TYPE_DEFAULT);



extern __SLL_API_TYPE_sll_api_time_current sll_api_time_current(__SLL_API_ARGS_sll_api_time_current);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_time_current_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	sll_float_t out=sll_api_time_current();
	return SLL_FROM_FLOAT(out);
}
INTERNAL_FUNCTION("time_current",sll_api_time_current_raw,SLL_INTERNAL_FUNCTION_TYPE_DEFAULT);



extern __SLL_API_TYPE_sll_api_time_current_nanos sll_api_time_current_nanos(__SLL_API_ARGS_sll_api_time_current_nanos);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_time_current_nanos_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	sll_integer_t out=sll_api_time_current_nanos();
	return SLL_FROM_INT(out);
}
INTERNAL_FUNCTION("time_current_nanos",sll_api_time_current_nanos_raw,SLL_INTERNAL_FUNCTION_TYPE_DEFAULT);



extern __SLL_API_TYPE_sll_api_time_sleep sll_api_time_sleep(__SLL_API_ARGS_sll_api_time_sleep);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_time_sleep_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(float_zero);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_INT||SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_FLOAT)){
		return SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_time_sleep(a);
	return SLL_FROM_FLOAT(out);
}
INTERNAL_FUNCTION("time_sleep",sll_api_time_sleep_raw,SLL_INTERNAL_FUNCTION_TYPE_DEFAULT);



extern __SLL_API_TYPE_sll_api_time_sleep_nanos sll_api_time_sleep_nanos(__SLL_API_ARGS_sll_api_time_sleep_nanos);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_time_sleep_nanos_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_INT||SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_FLOAT)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_time_sleep_nanos(a);
	return SLL_FROM_INT(out);
}
INTERNAL_FUNCTION("time_sleep_nanos",sll_api_time_sleep_nanos_raw,SLL_INTERNAL_FUNCTION_TYPE_DEFAULT);



extern __SLL_API_TYPE_sll_api_url_execute_request sll_api_url_execute_request(__SLL_API_ARGS_sll_api_url_execute_request);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_url_execute_request_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<5){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* b=*(al+1);
	if (!(SLL_RUNTIME_OBJECT_GET_TYPE(b)==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* c=*(al+2);
	if (!(SLL_RUNTIME_OBJECT_GET_TYPE(c)==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* d=*(al+3);
	if (!(SLL_RUNTIME_OBJECT_GET_TYPE(d)==SLL_RUNTIME_OBJECT_TYPE_MAP)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* e=*(al+4);
	if (!(SLL_RUNTIME_OBJECT_GET_TYPE(e)==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	return sll_api_url_execute_request(&(a->dt.s),&(b->dt.s),&(c->dt.s),&(d->dt.m),&(e->dt.s));
}
INTERNAL_FUNCTION("url_execute_request",sll_api_url_execute_request_raw,SLL_INTERNAL_FUNCTION_TYPE_REQUIRED);



extern __SLL_API_TYPE_sll_api_util_instruction_count sll_api_util_instruction_count(__SLL_API_ARGS_sll_api_util_instruction_count);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_util_instruction_count_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	sll_integer_t out=sll_api_util_instruction_count();
	return SLL_FROM_INT(out);
}
INTERNAL_FUNCTION("util_instruction_count",sll_api_util_instruction_count_raw,SLL_INTERNAL_FUNCTION_TYPE_DEFAULT);



extern __SLL_API_TYPE_sll_api_util_ref_count sll_api_util_ref_count(__SLL_API_ARGS_sll_api_util_ref_count);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_util_ref_count_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* a=*(al+0);
	sll_integer_t out=sll_api_util_ref_count(a);
	return SLL_FROM_INT(out);
}
INTERNAL_FUNCTION("util_ref_count",sll_api_util_ref_count_raw,SLL_INTERNAL_FUNCTION_TYPE_DEFAULT);
