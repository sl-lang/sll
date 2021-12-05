// WARNING: This is an auto-generated file. Any changes made to this file might be lost at any moment. Do Not Edit!
#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/_generated.h>
#include <sll/common.h>
#include <sll/handle.h>
#include <sll/ift.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/types.h>



__SLL_API_TYPE_sll_api_file_close sll_api_file_close(__SLL_API_ARGS_sll_api_file_close);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_close_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_object_t* a=*(al+0);
	if (!((SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_HANDLE&&a->dt.h.t!=SLL_HANDLE_UNKNOWN_TYPE))){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_file_close(&(a->dt.h));
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_file_open sll_api_file_open(__SLL_API_ARGS_sll_api_file_open);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_open_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(handle_zero);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(handle_zero);
	}
	const sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (!(SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING)){
			return SLL_ACQUIRE_STATIC(handle_zero);
		}
	}
	sll_handle_data_t out;
	sll_api_file_open(&(a->dt.s),(b?&(b->dt.s):NULL),&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_HANDLE;
	out_o->dt.h=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_file_read sll_api_file_read(__SLL_API_ARGS_sll_api_file_read);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_read_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_object_t* a=*(al+0);
	if (!((SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_HANDLE&&a->dt.h.t!=SLL_HANDLE_UNKNOWN_TYPE)||SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT)){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (!(SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT)){
			return SLL_ACQUIRE_STATIC(str_zero);
		}
	}
	sll_string_t out;
	sll_api_file_read(a,(b?b->dt.i:0),&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_file_write sll_api_file_write(__SLL_API_ARGS_sll_api_file_write);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_write_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<2){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_object_t* a=*(al+0);
	if (!((SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_HANDLE&&a->dt.h.t!=SLL_HANDLE_UNKNOWN_TYPE)||SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_object_t*const* b=al+1;
	sll_arg_count_t bc=all-1;
	sll_integer_t out=sll_api_file_write(a,b,bc);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_hash_create sll_api_hash_create(__SLL_API_ARGS_sll_api_hash_create);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_hash_create_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_object_t* a=*(al+0);
	sll_integer_t out=sll_api_hash_create(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_json_parse sll_api_json_parse(__SLL_API_ARGS_sll_api_json_parse);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_json_parse_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(handle_zero);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(handle_zero);
	}
	sll_object_t* out=sll_api_json_parse(&(a->dt.s));
	return out;
}



__SLL_API_TYPE_sll_api_json_stringify sll_api_json_stringify(__SLL_API_ARGS_sll_api_json_stringify);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_json_stringify_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_object_t* a=*(al+0);
	sll_string_t out;
	sll_api_json_stringify(a,&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_json_type sll_api_json_type(__SLL_API_ARGS_sll_api_json_type);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_json_type_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(handle_zero);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT)){
		return SLL_ACQUIRE_STATIC(handle_zero);
	}
	sll_handle_data_t out;
	sll_api_json_type(a->dt.i,&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_HANDLE;
	out_o->dt.h=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_path_absolute sll_api_path_absolute(__SLL_API_ARGS_sll_api_path_absolute);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_absolute_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	sll_string_t out;
	sll_api_path_absolute(&(a->dt.s),&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_path_exists sll_api_path_exists(__SLL_API_ARGS_sll_api_path_exists);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_exists_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_path_exists(&(a->dt.s));
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_path_get_cwd sll_api_path_get_cwd(__SLL_API_ARGS_sll_api_path_get_cwd);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_get_cwd_raw(const sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t out;
	sll_api_path_get_cwd(&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_path_is_dir sll_api_path_is_dir(__SLL_API_ARGS_sll_api_path_is_dir);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_is_dir_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_path_is_dir(&(a->dt.s));
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_path_join sll_api_path_join(__SLL_API_ARGS_sll_api_path_join);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_join_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	sll_arg_count_t ac=all-0;
	const sll_string_t** a=sll_allocate(ac*sizeof(sll_string_t*));
	for (sll_arg_count_t idx=0;idx<ac;idx++){
		const sll_object_t* tmp=*(al+idx+0);
		if (!(SLL_OBJECT_GET_TYPE(tmp)==SLL_OBJECT_TYPE_STRING)){
			sll_deallocate((void*)a);
			return SLL_ACQUIRE_STATIC(str_zero);
		}
		*(a+idx)=&(tmp->dt.s);
	}
	sll_string_t out;
	sll_api_path_join(a,ac,&out);
	sll_deallocate((void*)a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_path_list_dir sll_api_path_list_dir(__SLL_API_ARGS_sll_api_path_list_dir);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_list_dir_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(array_zero);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(array_zero);
	}
	sll_array_t out;
	sll_api_path_list_dir(&(a->dt.s),&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_path_recursive_list_dir sll_api_path_recursive_list_dir(__SLL_API_ARGS_sll_api_path_recursive_list_dir);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_recursive_list_dir_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(array_zero);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(array_zero);
	}
	sll_array_t out;
	sll_api_path_recursive_list_dir(&(a->dt.s),&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_path_relative sll_api_path_relative(__SLL_API_ARGS_sll_api_path_relative);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_relative_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	sll_string_t out;
	sll_api_path_relative(&(a->dt.s),&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_path_set_cwd sll_api_path_set_cwd(__SLL_API_ARGS_sll_api_path_set_cwd);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_set_cwd_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_path_set_cwd(&(a->dt.s));
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_path_size sll_api_path_size(__SLL_API_ARGS_sll_api_path_size);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_size_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_path_size(&(a->dt.s));
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_sort_sort sll_api_sort_sort(__SLL_API_ARGS_sll_api_sort_sort);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sort_sort_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(array_zero);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_ARRAY)){
		return SLL_ACQUIRE_STATIC(array_zero);
	}
	const sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (!(SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT)){
			return SLL_ACQUIRE_STATIC(array_zero);
		}
	}
	sll_array_t out;
	sll_api_sort_sort(&(a->dt.a),(b?b->dt.i:0),&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_convert sll_api_string_convert(__SLL_API_ARGS_sll_api_string_convert);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_convert_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_object_t*const* a=al+0;
	sll_arg_count_t ac=all-0;
	sll_string_t out;
	sll_api_string_convert(a,ac,&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_format sll_api_string_format(__SLL_API_ARGS_sll_api_string_format);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_format_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_object_t*const* b=al+1;
	sll_arg_count_t bc=all-1;
	sll_string_t out;
	sll_api_string_format(&(a->dt.s),b,bc,&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_flip_case sll_api_string_flip_case(__SLL_API_ARGS_sll_api_string_flip_case);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_flip_case_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	sll_string_t out;
	sll_api_string_flip_case(&(a->dt.s),&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_replace sll_api_string_replace(__SLL_API_ARGS_sll_api_string_replace);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_replace_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (!(SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR||SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING)){
			return SLL_ACQUIRE_STATIC(str_zero);
		}
	}
	const sll_object_t* c=NULL;
	if (all>2){
		c=*(al+2);
		if (!(SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_CHAR||SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_STRING)){
			return SLL_ACQUIRE_STATIC(str_zero);
		}
	}
	sll_string_t out;
	sll_api_string_replace(&(a->dt.s),b,c,&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_split sll_api_string_split(__SLL_API_ARGS_sll_api_string_split);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_split_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<2){
		return SLL_ACQUIRE_STATIC(array_zero);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(array_zero);
	}
	const sll_object_t* b=*(al+1);
	if (!(SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR||SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(array_zero);
	}
	sll_array_t out;
	sll_api_string_split(&(a->dt.s),b,&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_to_lower_case sll_api_string_to_lower_case(__SLL_API_ARGS_sll_api_string_to_lower_case);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_to_lower_case_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	sll_string_t out;
	sll_api_string_to_lower_case(&(a->dt.s),&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_to_title_case sll_api_string_to_title_case(__SLL_API_ARGS_sll_api_string_to_title_case);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_to_title_case_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	sll_string_t out;
	sll_api_string_to_title_case(&(a->dt.s),&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_to_upper_case sll_api_string_to_upper_case(__SLL_API_ARGS_sll_api_string_to_upper_case);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_to_upper_case_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING)){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	sll_string_t out;
	sll_api_string_to_upper_case(&(a->dt.s),&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_sys_arg_get sll_api_sys_arg_get(__SLL_API_ARGS_sll_api_sys_arg_get);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_arg_get_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT)){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	sll_string_t out;
	sll_api_sys_arg_get(a->dt.i,&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_sys_arg_get_count sll_api_sys_arg_get_count(__SLL_API_ARGS_sll_api_sys_arg_get_count);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_arg_get_count_raw(const sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t out=sll_api_sys_arg_get_count();
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_sys_get_executable sll_api_sys_get_executable(__SLL_API_ARGS_sll_api_sys_get_executable);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_get_executable_raw(const sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t out;
	sll_api_sys_get_executable(&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_sys_get_platform sll_api_sys_get_platform(__SLL_API_ARGS_sll_api_sys_get_platform);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_get_platform_raw(const sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t out;
	sll_api_sys_get_platform(&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_time_current sll_api_time_current(__SLL_API_ARGS_sll_api_time_current);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_time_current_raw(const sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t out=sll_api_time_current();
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_time_current_nanos sll_api_time_current_nanos(__SLL_API_ARGS_sll_api_time_current_nanos);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_time_current_nanos_raw(const sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t out=sll_api_time_current_nanos();
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_time_sleep sll_api_time_sleep(__SLL_API_ARGS_sll_api_time_sleep);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_time_sleep_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC(float_zero);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT||SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT)){
		return SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_time_sleep(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_time_sleep_nanos sll_api_time_sleep_nanos(__SLL_API_ARGS_sll_api_time_sleep_nanos);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_time_sleep_nanos_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_object_t* a=*(al+0);
	if (!(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT||SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_time_sleep_nanos(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_util_instruction_count sll_api_util_instruction_count(__SLL_API_ARGS_sll_api_util_instruction_count);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_util_instruction_count_raw(const sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t out=sll_api_util_instruction_count();
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_util_ref_count sll_api_util_ref_count(__SLL_API_ARGS_sll_api_util_ref_count);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_util_ref_count_raw(const sll_object_t*const* al,sll_arg_count_t all){
	if (all<1){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_object_t* a=*(al+0);
	sll_integer_t out=sll_api_util_ref_count(a);
	return SLL_FROM_INT(out);
}



static const internal_function_t _ifunc_data_ptr[]={
	{"sll:file_close",sll_api_file_close_raw,SLL_INTERNAL_FUNCTION_FLAG_REQUIRED},
	{"sll:file_open",sll_api_file_open_raw,SLL_INTERNAL_FUNCTION_FLAG_REQUIRED},
	{"sll:file_read",sll_api_file_read_raw,SLL_INTERNAL_FUNCTION_FLAG_REQUIRED},
	{"sll:file_write",sll_api_file_write_raw,SLL_INTERNAL_FUNCTION_FLAG_REQUIRED},
	{"sll:hash_create",sll_api_hash_create_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:json_parse",sll_api_json_parse_raw,0},
	{"sll:json_stringify",sll_api_json_stringify_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:json_type",sll_api_json_type_raw,0},
	{"sll:path_absolute",sll_api_path_absolute_raw,0},
	{"sll:path_exists",sll_api_path_exists_raw,0},
	{"sll:path_get_cwd",sll_api_path_get_cwd_raw,0},
	{"sll:path_is_dir",sll_api_path_is_dir_raw,0},
	{"sll:path_join",sll_api_path_join_raw,0},
	{"sll:path_list_dir",sll_api_path_list_dir_raw,0},
	{"sll:path_recursive_list_dir",sll_api_path_recursive_list_dir_raw,0},
	{"sll:path_relative",sll_api_path_relative_raw,0},
	{"sll:path_set_cwd",sll_api_path_set_cwd_raw,0},
	{"sll:path_size",sll_api_path_size_raw,0},
	{"sll:sort_sort",sll_api_sort_sort_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_convert",sll_api_string_convert_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_format",sll_api_string_format_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_flip_case",sll_api_string_flip_case_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_replace",sll_api_string_replace_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_split",sll_api_string_split_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_to_lower_case",sll_api_string_to_lower_case_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_to_title_case",sll_api_string_to_title_case_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_to_upper_case",sll_api_string_to_upper_case_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:sys_arg_get",sll_api_sys_arg_get_raw,0},
	{"sll:sys_arg_get_count",sll_api_sys_arg_get_count_raw,0},
	{"sll:sys_get_executable",sll_api_sys_get_executable_raw,0},
	{"sll:sys_get_platform",sll_api_sys_get_platform_raw,0},
	{"sll:time_current",sll_api_time_current_raw,0},
	{"sll:time_current_nanos",sll_api_time_current_nanos_raw,0},
	{"sll:time_sleep",sll_api_time_sleep_raw,0},
	{"sll:time_sleep_nanos",sll_api_time_sleep_nanos_raw,0},
	{"sll:util_instruction_count",sll_api_util_instruction_count_raw,0},
	{"sll:util_ref_count",sll_api_util_ref_count_raw,0}
};



const sll_function_index_t _ifunc_size=37;
const internal_function_t* _ifunc_data=(const internal_function_t*)(&_ifunc_data_ptr);
