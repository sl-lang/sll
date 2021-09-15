#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/common.h>
#include <sll/constants.h>
#include <sll/static_object.h>
#include <sll/types.h>



INTERNAL_FUNCTION("file_close",sll_api_file_close_raw);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_file_close_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!((a->t==SLL_RUNTIME_OBJECT_TYPE_HANDLE&&a->dt.h.t!=SLL_UNKNOWN_HANDLE_TYPE))){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_file_close(&(a->dt.h));
	return SLL_FROM_INT(out);
}



INTERNAL_FUNCTION("file_open",sll_api_file_open_raw);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_file_open_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(handle_zero);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(a->t==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(handle_zero);
	}
	const sll_runtime_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (!(b->t==SLL_RUNTIME_OBJECT_TYPE_STRING)){
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



INTERNAL_FUNCTION("file_write",sll_api_file_write_raw);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_file_write_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<2){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!((a->t==SLL_RUNTIME_OBJECT_TYPE_HANDLE&&a->dt.h.t!=SLL_UNKNOWN_HANDLE_TYPE)||a->t==SLL_RUNTIME_OBJECT_TYPE_INT)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t*const* b=al+1;
	sll_arg_count_t bc=all-1;
	sll_integer_t out=sll_api_file_write(a,b,bc);
	return SLL_FROM_INT(out);
}



INTERNAL_FUNCTION("json_parse",sll_api_json_parse_raw);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_json_parse_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(handle_zero);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(a->t==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(handle_zero);
	}
	return sll_api_json_parse(&(a->dt.s));
}



INTERNAL_FUNCTION("json_stringify",sll_api_json_stringify_raw);
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



INTERNAL_FUNCTION("json_type",sll_api_json_type_raw);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_json_type_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(handle_zero);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(a->t==SLL_RUNTIME_OBJECT_TYPE_INT)){
		return SLL_ACQUIRE_STATIC(handle_zero);
	}
	sll_handle_data_t out;
	sll_api_json_type(a->dt.i,&out);
	sll_runtime_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_RUNTIME_OBJECT_TYPE_HANDLE;
	out_o->dt.h=out;
	return out_o;
}



INTERNAL_FUNCTION("path_absolute",sll_api_path_absolute_raw);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_path_absolute_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(a->t==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	sll_string_t out;
	sll_api_path_absolute(&(a->dt.s),&out);
	sll_runtime_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



INTERNAL_FUNCTION("path_exists",sll_api_path_exists_raw);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_path_exists_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(a->t==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_path_exists(&(a->dt.s));
	return SLL_FROM_INT(out);
}



INTERNAL_FUNCTION("path_is_dir",sll_api_path_is_dir_raw);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_path_is_dir_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(a->t==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_path_is_dir(&(a->dt.s));
	return SLL_FROM_INT(out);
}



INTERNAL_FUNCTION("path_relative",sll_api_path_relative_raw);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_path_relative_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(a->t==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	sll_string_t out;
	sll_api_path_relative(&(a->dt.s),&out);
	sll_runtime_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



INTERNAL_FUNCTION("path_size",sll_api_path_size_raw);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_path_size_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(a->t==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_path_size(&(a->dt.s));
	return SLL_FROM_INT(out);
}



INTERNAL_FUNCTION("string_convert",sll_api_string_convert_raw);
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



INTERNAL_FUNCTION("string_length",sll_api_string_length_raw);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_string_length_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t*const* a=al+0;
	sll_arg_count_t ac=all-0;
	sll_integer_t out=sll_api_string_length(a,ac);
	return SLL_FROM_INT(out);
}



INTERNAL_FUNCTION("sys_arg_get",sll_api_sys_arg_get_raw);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_sys_arg_get_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(a->t==SLL_RUNTIME_OBJECT_TYPE_INT)){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	return sll_api_sys_arg_get(a->dt.i);
}



INTERNAL_FUNCTION("sys_arg_get_count",sll_api_sys_arg_get_count_raw);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_sys_arg_get_count_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	sll_integer_t out=sll_api_sys_arg_get_count();
	return SLL_FROM_INT(out);
}



INTERNAL_FUNCTION("sys_get_platform",sll_api_sys_get_platform_raw);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_sys_get_platform_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	return sll_api_sys_get_platform();
}



INTERNAL_FUNCTION("time_current",sll_api_time_current_raw);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_time_current_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	sll_float_t out=sll_api_time_current();
	return SLL_FROM_FLOAT(out);
}



INTERNAL_FUNCTION("time_current_nanos",sll_api_time_current_nanos_raw);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_time_current_nanos_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	sll_integer_t out=sll_api_time_current_nanos();
	return SLL_FROM_INT(out);
}



INTERNAL_FUNCTION("time_sleep",sll_api_time_sleep_raw);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_time_sleep_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(float_zero);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(a->t==SLL_RUNTIME_OBJECT_TYPE_INT||a->t==SLL_RUNTIME_OBJECT_TYPE_FLOAT)){
		return SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_time_sleep(a);
	return SLL_FROM_FLOAT(out);
}



INTERNAL_FUNCTION("time_sleep_nanos",sll_api_time_sleep_nanos_raw);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_time_sleep_nanos_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(a->t==SLL_RUNTIME_OBJECT_TYPE_INT||a->t==SLL_RUNTIME_OBJECT_TYPE_FLOAT)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_time_sleep_nanos(a);
	return SLL_FROM_INT(out);
}



INTERNAL_FUNCTION("url_execute_request",sll_api_url_execute_request_raw);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_url_execute_request_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<5){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* a=*(al+0);
	if (!(a->t==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* b=*(al+1);
	if (!(b->t==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* c=*(al+2);
	if (!(c->t==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* d=*(al+3);
	if (!(d->t==SLL_RUNTIME_OBJECT_TYPE_MAP)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* e=*(al+4);
	if (!(e->t==SLL_RUNTIME_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	return sll_api_url_execute_request(&(a->dt.s),&(b->dt.s),&(c->dt.s),&(d->dt.m),&(e->dt.s));
}



INTERNAL_FUNCTION("util_ref_count",sll_api_util_ref_count_raw);
__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_util_ref_count_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_runtime_object_t* a=*(al+0);
	sll_integer_t out=sll_api_util_ref_count(a);
	return SLL_FROM_INT(out);
}
