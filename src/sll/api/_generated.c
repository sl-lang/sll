// WARNING: This is an auto-generated file. Any changes made to this file might be lost at any moment. Do Not Edit!
#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/_generated.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/handle.h>
#include <sll/ift.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/types.h>



__SLL_API_TYPE_sll_api_array_count sll_api_array_count(__SLL_API_ARGS_sll_api_array_count);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_count_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_ARRAY){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_ARRAY]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_integer_t out=sll_api_array_count(&(a->dt.a),b);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_array_create sll_api_array_create(__SLL_API_ARGS_sll_api_array_create);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_create_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_array_t out;
	sll_api_array_create(a->dt.i,&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_array_replace sll_api_array_replace(__SLL_API_ARGS_sll_api_array_replace);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_replace_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_ARRAY){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_ARRAY]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* c=NULL;
	if (all>2){
		c=*(al+2);
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_array_t out;
	sll_api_array_replace(&(a->dt.a),b,c,&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_array_reverse sll_api_array_reverse(__SLL_API_ARGS_sll_api_array_reverse);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_reverse_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_ARRAY){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_ARRAY]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_array_t out;
	sll_api_array_reverse(&(a->dt.a),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_file_close sll_api_file_close(__SLL_API_ARGS_sll_api_file_close);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_close_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if ((SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_HANDLE&&a->dt.h.t!=SLL_HANDLE_UNKNOWN_TYPE)){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_HANDLE]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_bool_t out=sll_api_file_close(&(a->dt.h));
	SLL_RELEASE(a);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_file_open sll_api_file_open(__SLL_API_ARGS_sll_api_file_open);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_open_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	sll_handle_data_t out;
	sll_api_file_open(&(a->dt.s),(b?&(b->dt.s):NULL),&out);
	SLL_RELEASE(a);
	SLL_RELEASE(b);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_HANDLE;
	out_o->dt.h=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_file_read sll_api_file_read(__SLL_API_ARGS_sll_api_file_read);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_read_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if ((SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_HANDLE&&a->dt.h.t!=SLL_HANDLE_UNKNOWN_TYPE)){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_HANDLE]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	sll_string_t out;
	sll_api_file_read(&(a->dt.h),(b?b->dt.i:0),&out);
	SLL_RELEASE(a);
	SLL_RELEASE(b);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_file_std_handle sll_api_file_std_handle(__SLL_API_ARGS_sll_api_file_std_handle);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_std_handle_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_handle_data_t out;
	sll_api_file_std_handle(a->dt.i,&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_HANDLE;
	out_o->dt.h=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_file_write sll_api_file_write(__SLL_API_ARGS_sll_api_file_write);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_write_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if ((SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_HANDLE&&a->dt.h.t!=SLL_HANDLE_UNKNOWN_TYPE)){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_HANDLE]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t*const* b=al+1;
	sll_arg_count_t bc=(all>1?all-1:0);
	sll_integer_t out=sll_api_file_write(&(a->dt.h),b,bc);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_hash_create sll_api_hash_create(__SLL_API_ARGS_sll_api_hash_create);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_hash_create_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_integer_t out=sll_api_hash_create(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_json_parse sll_api_json_parse(__SLL_API_ARGS_sll_api_json_parse);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_json_parse_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* out=sll_api_json_parse(&(a->dt.s));
	SLL_RELEASE(a);
	return out;
}



__SLL_API_TYPE_sll_api_json_stringify sll_api_json_stringify(__SLL_API_ARGS_sll_api_json_stringify);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_json_stringify_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_string_t out;
	sll_api_json_stringify(a,&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_json_type sll_api_json_type(__SLL_API_ARGS_sll_api_json_type);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_json_type_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_handle_data_t out;
	sll_api_json_type(a->dt.i,&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_HANDLE;
	out_o->dt.h=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_log_log sll_api_log_log(__SLL_API_ARGS_sll_api_log_log);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_log_log_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t*const* a=al+0;
	sll_arg_count_t ac=all;
	sll_api_log_log(a,ac);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_log_set_default sll_api_log_set_default(__SLL_API_ARGS_sll_api_log_set_default);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_log_set_default_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_api_log_set_default(!!(a->dt.i));
	SLL_RELEASE(a);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_log_set_file sll_api_log_set_file(__SLL_API_ARGS_sll_api_log_set_file);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_log_set_file_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_api_log_set_file(&(a->dt.s),!!(b->dt.i));
	SLL_RELEASE(a);
	SLL_RELEASE(b);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_log_set_function sll_api_log_set_function(__SLL_API_ARGS_sll_api_log_set_function);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_log_set_function_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* c=NULL;
	if (all>2){
		c=*(al+2);
		if (SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(c);
		}
		else{
			c=sll_operator_cast(c,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_api_log_set_function(&(a->dt.s),&(b->dt.s),!!(c->dt.i));
	SLL_RELEASE(a);
	SLL_RELEASE(b);
	SLL_RELEASE(c);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_path_absolute sll_api_path_absolute(__SLL_API_ARGS_sll_api_path_absolute);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_absolute_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_string_t out;
	sll_api_path_absolute(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_path_exists sll_api_path_exists(__SLL_API_ARGS_sll_api_path_exists);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_exists_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_integer_t out=sll_api_path_exists(&(a->dt.s));
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_path_get_cwd sll_api_path_get_cwd(__SLL_API_ARGS_sll_api_path_get_cwd);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_get_cwd_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t out;
	sll_api_path_get_cwd(&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_path_is_dir sll_api_path_is_dir(__SLL_API_ARGS_sll_api_path_is_dir);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_is_dir_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_integer_t out=sll_api_path_is_dir(&(a->dt.s));
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_path_join sll_api_path_join(__SLL_API_ARGS_sll_api_path_join);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_join_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_arg_count_t ac=all;
	sll_string_t** a=sll_allocate(ac*sizeof(sll_string_t*));
	for (sll_arg_count_t idx=0;idx<ac;idx++){
		sll_object_t* tmp=*(al+idx+0);
		if (!(SLL_OBJECT_GET_TYPE(tmp)==SLL_OBJECT_TYPE_STRING)){
			tmp=sll_operator_cast(tmp,sll_static_int[SLL_OBJECT_TYPE_STRING]);
			sll_string_clone(&(tmp->dt.s),*(a+idx));
			SLL_RELEASE(tmp);
		}
		else{
			*(a+idx)=&(tmp->dt.s);
		}
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
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_list_dir_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_array_t out;
	sll_api_path_list_dir(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_path_recursive_list_dir sll_api_path_recursive_list_dir(__SLL_API_ARGS_sll_api_path_recursive_list_dir);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_recursive_list_dir_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_array_t out;
	sll_api_path_recursive_list_dir(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_path_relative sll_api_path_relative(__SLL_API_ARGS_sll_api_path_relative);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_relative_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_string_t out;
	sll_api_path_relative(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_path_set_cwd sll_api_path_set_cwd(__SLL_API_ARGS_sll_api_path_set_cwd);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_set_cwd_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_integer_t out=sll_api_path_set_cwd(&(a->dt.s));
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_path_size sll_api_path_size(__SLL_API_ARGS_sll_api_path_size);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_size_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_integer_t out=sll_api_path_size(&(a->dt.s));
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_path_split sll_api_path_split(__SLL_API_ARGS_sll_api_path_split);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_split_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_array_t out;
	sll_api_path_split(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_sort_sort sll_api_sort_sort(__SLL_API_ARGS_sll_api_sort_sort);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sort_sort_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_ARRAY){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_ARRAY]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	sll_array_t out;
	sll_api_sort_sort(&(a->dt.a),(b?b->dt.i:0),&out);
	SLL_RELEASE(a);
	SLL_RELEASE(b);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_convert sll_api_string_convert(__SLL_API_ARGS_sll_api_string_convert);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_convert_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t*const* a=al+0;
	sll_arg_count_t ac=all;
	sll_string_t out;
	sll_api_string_convert(a,ac,&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_ends sll_api_string_ends(__SLL_API_ARGS_sll_api_string_ends);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_ends_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_bool_t out=sll_api_string_ends(&(a->dt.s),b);
	SLL_RELEASE(a);
	SLL_RELEASE(b);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_string_format sll_api_string_format(__SLL_API_ARGS_sll_api_string_format);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_format_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t*const* b=al+1;
	sll_arg_count_t bc=(all>1?all-1:0);
	sll_string_t out;
	sll_api_string_format(&(a->dt.s),b,bc,&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_flip_case sll_api_string_flip_case(__SLL_API_ARGS_sll_api_string_flip_case);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_flip_case_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_string_t out;
	sll_api_string_flip_case(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_index sll_api_string_index(__SLL_API_ARGS_sll_api_string_index);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_index_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_integer_t out=sll_api_string_index(&(a->dt.s),b);
	SLL_RELEASE(a);
	SLL_RELEASE(b);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_string_index_list sll_api_string_index_list(__SLL_API_ARGS_sll_api_string_index_list);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_index_list_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_integer_t out=sll_api_string_index_list(&(a->dt.s),b);
	SLL_RELEASE(a);
	SLL_RELEASE(b);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_string_index_reverse sll_api_string_index_reverse(__SLL_API_ARGS_sll_api_string_index_reverse);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_index_reverse_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_integer_t out=sll_api_string_index_reverse(&(a->dt.s),b);
	SLL_RELEASE(a);
	SLL_RELEASE(b);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_string_index_reverse_list sll_api_string_index_reverse_list(__SLL_API_ARGS_sll_api_string_index_reverse_list);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_index_reverse_list_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_integer_t out=sll_api_string_index_reverse_list(&(a->dt.s),b);
	SLL_RELEASE(a);
	SLL_RELEASE(b);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_string_pad_left sll_api_string_pad_left(__SLL_API_ARGS_sll_api_string_pad_left);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_pad_left_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* c=NULL;
	if (all>2){
		c=*(al+2);
		if (SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(c);
		}
		else{
			c=sll_operator_cast(c,sll_static_int[SLL_OBJECT_TYPE_CHAR]);
		}
	}
	sll_string_t out;
	sll_api_string_pad_left(&(a->dt.s),b->dt.i,(c?c->dt.c:SLL_NO_CHAR),&out);
	SLL_RELEASE(a);
	SLL_RELEASE(b);
	SLL_RELEASE(c);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_pad_right sll_api_string_pad_right(__SLL_API_ARGS_sll_api_string_pad_right);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_pad_right_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* c=NULL;
	if (all>2){
		c=*(al+2);
		if (SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(c);
		}
		else{
			c=sll_operator_cast(c,sll_static_int[SLL_OBJECT_TYPE_CHAR]);
		}
	}
	sll_string_t out;
	sll_api_string_pad_right(&(a->dt.s),b->dt.i,(c?c->dt.c:SLL_NO_CHAR),&out);
	SLL_RELEASE(a);
	SLL_RELEASE(b);
	SLL_RELEASE(c);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_replace sll_api_string_replace(__SLL_API_ARGS_sll_api_string_replace);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_replace_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	sll_object_t* c=NULL;
	if (all>2){
		c=*(al+2);
		if (SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(c);
		}
		else{
			c=sll_operator_cast(c,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	sll_string_t out;
	sll_api_string_replace(&(a->dt.s),b,c,&out);
	SLL_RELEASE(a);
	SLL_RELEASE(b);
	SLL_RELEASE(c);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_reverse sll_api_string_reverse(__SLL_API_ARGS_sll_api_string_reverse);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_reverse_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_string_t out;
	sll_api_string_reverse(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_split sll_api_string_split(__SLL_API_ARGS_sll_api_string_split);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_split_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_array_t out;
	sll_api_string_split(&(a->dt.s),b,&out);
	SLL_RELEASE(a);
	SLL_RELEASE(b);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_starts sll_api_string_starts(__SLL_API_ARGS_sll_api_string_starts);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_starts_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_bool_t out=sll_api_string_starts(&(a->dt.s),b);
	SLL_RELEASE(a);
	SLL_RELEASE(b);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_string_to_lower_case sll_api_string_to_lower_case(__SLL_API_ARGS_sll_api_string_to_lower_case);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_to_lower_case_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_string_t out;
	sll_api_string_to_lower_case(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_to_title_case sll_api_string_to_title_case(__SLL_API_ARGS_sll_api_string_to_title_case);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_to_title_case_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_string_t out;
	sll_api_string_to_title_case(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_to_upper_case sll_api_string_to_upper_case(__SLL_API_ARGS_sll_api_string_to_upper_case);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_to_upper_case_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_string_t out;
	sll_api_string_to_upper_case(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_sys_arg_get sll_api_sys_arg_get(__SLL_API_ARGS_sll_api_sys_arg_get);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_arg_get_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_string_t out;
	sll_api_sys_arg_get(a->dt.i,&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_sys_arg_get_count sll_api_sys_arg_get_count(__SLL_API_ARGS_sll_api_sys_arg_get_count);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_arg_get_count_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t out=sll_api_sys_arg_get_count();
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_sys_get_executable sll_api_sys_get_executable(__SLL_API_ARGS_sll_api_sys_get_executable);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_get_executable_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t out;
	sll_api_sys_get_executable(&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_sys_get_platform sll_api_sys_get_platform(__SLL_API_ARGS_sll_api_sys_get_platform);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_get_platform_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t out;
	sll_api_sys_get_platform(&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_sys_get_version sll_api_sys_get_version(__SLL_API_ARGS_sll_api_sys_get_version);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_get_version_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* out=sll_api_sys_get_version(a->dt.i);
	SLL_RELEASE(a);
	return out;
}



__SLL_API_TYPE_sll_api_sys_load_library sll_api_sys_load_library(__SLL_API_ARGS_sll_api_sys_load_library);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_load_library_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_bool_t out=sll_api_sys_load_library(&(a->dt.s));
	SLL_RELEASE(a);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_time_current sll_api_time_current(__SLL_API_ARGS_sll_api_time_current);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_time_current_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t out=sll_api_time_current();
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_time_current_nanos sll_api_time_current_nanos(__SLL_API_ARGS_sll_api_time_current_nanos);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_time_current_nanos_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t out=sll_api_time_current_nanos();
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_time_sleep sll_api_time_sleep(__SLL_API_ARGS_sll_api_time_sleep);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_time_sleep_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT||SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_float_t out=sll_api_time_sleep(a);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_time_sleep_nanos sll_api_time_sleep_nanos(__SLL_API_ARGS_sll_api_time_sleep_nanos);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_time_sleep_nanos_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT||SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT||SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_integer_t out=sll_api_time_sleep_nanos(a);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



static const internal_function_t _ifunc_data_ptr[]={
	{"sll:array_count",sll_api_array_count_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:array_create",sll_api_array_create_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:array_replace",sll_api_array_replace_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:array_reverse",sll_api_array_reverse_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:file_close",sll_api_file_close_raw,SLL_INTERNAL_FUNCTION_FLAG_REQUIRED},
	{"sll:file_open",sll_api_file_open_raw,0},
	{"sll:file_read",sll_api_file_read_raw,SLL_INTERNAL_FUNCTION_FLAG_REQUIRED},
	{"sll:file_std_handle",sll_api_file_std_handle_raw,0},
	{"sll:file_write",sll_api_file_write_raw,SLL_INTERNAL_FUNCTION_FLAG_REQUIRED},
	{"sll:hash_create",sll_api_hash_create_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:json_parse",sll_api_json_parse_raw,0},
	{"sll:json_stringify",sll_api_json_stringify_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:json_type",sll_api_json_type_raw,0},
	{"sll:log_log",sll_api_log_log_raw,0},
	{"sll:log_set_default",sll_api_log_set_default_raw,0},
	{"sll:log_set_file",sll_api_log_set_file_raw,0},
	{"sll:log_set_function",sll_api_log_set_function_raw,0},
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
	{"sll:path_split",sll_api_path_split_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:sort_sort",sll_api_sort_sort_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_convert",sll_api_string_convert_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_ends",sll_api_string_ends_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_format",sll_api_string_format_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_flip_case",sll_api_string_flip_case_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_index",sll_api_string_index_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_index_list",sll_api_string_index_list_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_index_reverse",sll_api_string_index_reverse_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_index_reverse_list",sll_api_string_index_reverse_list_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_pad_left",sll_api_string_pad_left_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_pad_right",sll_api_string_pad_right_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_replace",sll_api_string_replace_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_reverse",sll_api_string_reverse_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_split",sll_api_string_split_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_starts",sll_api_string_starts_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_to_lower_case",sll_api_string_to_lower_case_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_to_title_case",sll_api_string_to_title_case_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:string_to_upper_case",sll_api_string_to_upper_case_raw,SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL},
	{"sll:sys_arg_get",sll_api_sys_arg_get_raw,0},
	{"sll:sys_arg_get_count",sll_api_sys_arg_get_count_raw,0},
	{"sll:sys_get_executable",sll_api_sys_get_executable_raw,0},
	{"sll:sys_get_platform",sll_api_sys_get_platform_raw,0},
	{"sll:sys_get_version",sll_api_sys_get_version_raw,0},
	{"sll:sys_load_library",sll_api_sys_load_library_raw,0},
	{"sll:time_current",sll_api_time_current_raw,0},
	{"sll:time_current_nanos",sll_api_time_current_nanos_raw,0},
	{"sll:time_sleep",sll_api_time_sleep_raw,0},
	{"sll:time_sleep_nanos",sll_api_time_sleep_nanos_raw,0}
};



const sll_function_index_t _ifunc_size=56;
const internal_function_t* _ifunc_data=(const internal_function_t*)(&_ifunc_data_ptr);
