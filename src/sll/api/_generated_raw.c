// WARNING: This is an auto-generated file. Any changes made to this file might be lost at any moment. Do Not Edit!
#include <sll/_internal/api.h>
#include <sll/_internal/common.h>
#include <sll/_internal/gc.h>
#include <sll/api.h>
#include <sll/common.h>
#include <sll/generated/api.h>
#include <sll/parse_args.h>
#include <sll/static_object.h>
#include <sll/types.h>



__SLL_API_TYPE_sll_api_array_count sll_api_array_count(__SLL_API_ARGS_sll_api_array_count);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_count_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_array_t* a;
	sll_object_t* b;
	sll_parse_args(SLL_CHAR("ao"),al,all,&a,&b);
	sll_integer_t out=sll_api_array_count(a,b);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_array_create sll_api_array_create(__SLL_API_ARGS_sll_api_array_create);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_create_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_array_t out;
	sll_api_array_create(a,&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_array_extend sll_api_array_extend(__SLL_API_ARGS_sll_api_array_extend);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_extend_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_array_t* a;
	sll_array_t* b;
	sll_parse_args(SLL_CHAR("aa"),al,all,&a,&b);
	sll_api_array_extend(a,b);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_array_join sll_api_array_join(__SLL_API_ARGS_sll_api_array_join);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_join_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_array_t* a;
	sll_object_t* b;
	sll_parse_args(SLL_CHAR("ao"),al,all,&a,&b);
	sll_array_t out;
	sll_api_array_join(a,b,&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_array_pop sll_api_array_pop(__SLL_API_ARGS_sll_api_array_pop);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_pop_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_array_t* a;
	sll_parse_args(SLL_CHAR("a"),al,all,&a);
	sll_object_t* out=sll_api_array_pop(a);
	return out;
}



__SLL_API_TYPE_sll_api_array_push sll_api_array_push(__SLL_API_ARGS_sll_api_array_push);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_push_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_array_t* a;
	sll_object_t* b;
	sll_parse_args(SLL_CHAR("ao"),al,all,&a,&b);
	sll_api_array_push(a,b);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_array_remove sll_api_array_remove(__SLL_API_ARGS_sll_api_array_remove);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_remove_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_array_t* a;
	sll_object_t* b;
	sll_parse_args(SLL_CHAR("ao"),al,all,&a,&b);
	sll_api_array_remove(a,b);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_array_replace sll_api_array_replace(__SLL_API_ARGS_sll_api_array_replace);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_replace_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_array_t* a;
	sll_object_t* b;
	sll_object_t* c;
	sll_parse_args(SLL_CHAR("aoo"),al,all,&a,&b,&c);
	sll_array_t out;
	sll_api_array_replace(a,b,c,&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_array_reverse sll_api_array_reverse(__SLL_API_ARGS_sll_api_array_reverse);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_reverse_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_array_t* a;
	sll_parse_args(SLL_CHAR("a"),al,all,&a);
	sll_array_t out;
	sll_api_array_reverse(a,&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_array_shift sll_api_array_shift(__SLL_API_ARGS_sll_api_array_shift);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_shift_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_array_t* a;
	sll_parse_args(SLL_CHAR("a"),al,all,&a);
	sll_object_t* out=sll_api_array_shift(a);
	return out;
}



__SLL_API_TYPE_sll_api_array_split sll_api_array_split(__SLL_API_ARGS_sll_api_array_split);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_split_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_array_t* a;
	sll_object_t* b;
	sll_parse_args(SLL_CHAR("ao"),al,all,&a,&b);
	sll_array_t out;
	sll_api_array_split(a,b,&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_array_unshift sll_api_array_unshift(__SLL_API_ARGS_sll_api_array_unshift);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_unshift_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_array_t* a;
	sll_object_t* b;
	sll_parse_args(SLL_CHAR("ao"),al,all,&a,&b);
	sll_api_array_unshift(a,b);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_base64_decode sll_api_base64_decode(__SLL_API_ARGS_sll_api_base64_decode);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_base64_decode_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_object_t* out=sll_api_base64_decode(a);
	return out;
}



__SLL_API_TYPE_sll_api_base64_encode sll_api_base64_encode(__SLL_API_ARGS_sll_api_base64_encode);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_base64_encode_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_string_t out;
	sll_api_base64_encode(a,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_date_get_time_zone sll_api_date_get_time_zone(__SLL_API_ARGS_sll_api_date_get_time_zone);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_date_get_time_zone_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_array_t out;
	sll_api_date_get_time_zone(&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_date_merge sll_api_date_merge(__SLL_API_ARGS_sll_api_date_merge);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_date_merge_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_integer_t b;
	sll_integer_t c;
	sll_integer_t d;
	sll_integer_t e;
	sll_float_t f;
	sll_parse_args(SLL_CHAR("iiiiif"),al,all,&a,&b,&c,&d,&e,&f);
	sll_float_t out=sll_api_date_merge(a,b,c,d,e,f);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_date_split sll_api_date_split(__SLL_API_ARGS_sll_api_date_split);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_date_split_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_array_t out;
	sll_api_date_split(a,&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_error_get_call_stack sll_api_error_get_call_stack(__SLL_API_ARGS_sll_api_error_get_call_stack);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_error_get_call_stack_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_integer_t b;
	sll_parse_args(SLL_CHAR("ii"),al,all,&a,&b);
	sll_array_t out;
	sll_api_error_get_call_stack(a,b,&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_file_close sll_api_file_close(__SLL_API_ARGS_sll_api_file_close);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_close_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_bool_t out=sll_api_file_close(a);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_file_copy sll_api_file_copy(__SLL_API_ARGS_sll_api_file_copy);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_copy_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_string_t* b;
	sll_parse_args(SLL_CHAR("ss"),al,all,&a,&b);
	sll_bool_t out=sll_api_file_copy(a,b);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_file_delete sll_api_file_delete(__SLL_API_ARGS_sll_api_file_delete);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_delete_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_bool_t out=sll_api_file_delete(a);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_file_flush sll_api_file_flush(__SLL_API_ARGS_sll_api_file_flush);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_flush_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_bool_t out=sll_api_file_flush(a);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_file_from_data sll_api_file_from_data(__SLL_API_ARGS_sll_api_file_from_data);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_from_data_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_integer_t b;
	sll_parse_args(SLL_CHAR("si"),al,all,&a,&b);
	sll_integer_t out=sll_api_file_from_data(a,b);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_file_get_buffer sll_api_file_get_buffer(__SLL_API_ARGS_sll_api_file_get_buffer);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_get_buffer_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_string_t out;
	sll_api_file_get_buffer(a,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_file_get_temp_path sll_api_file_get_temp_path(__SLL_API_ARGS_sll_api_file_get_temp_path);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_get_temp_path_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t out;
	sll_api_file_get_temp_path(&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_file_inc_handle sll_api_file_inc_handle(__SLL_API_ARGS_sll_api_file_inc_handle);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_inc_handle_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_api_file_inc_handle(a);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_file_open sll_api_file_open(__SLL_API_ARGS_sll_api_file_open);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_open_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_integer_t b;
	sll_parse_args(SLL_CHAR("si"),al,all,&a,&b);
	sll_integer_t out=sll_api_file_open(a,b);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_file_peek sll_api_file_peek(__SLL_API_ARGS_sll_api_file_peek);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_peek_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_object_t* out=sll_api_file_peek(a);
	return out;
}



__SLL_API_TYPE_sll_api_file_read sll_api_file_read(__SLL_API_ARGS_sll_api_file_read);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_read_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_integer_t b;
	sll_parse_args(SLL_CHAR("ii"),al,all,&a,&b);
	sll_object_t* out=sll_api_file_read(a,b);
	return out;
}



__SLL_API_TYPE_sll_api_file_read_char sll_api_file_read_char(__SLL_API_ARGS_sll_api_file_read_char);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_read_char_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_object_t* out=sll_api_file_read_char(a);
	return out;
}



__SLL_API_TYPE_sll_api_file_rename sll_api_file_rename(__SLL_API_ARGS_sll_api_file_rename);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_rename_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_string_t* b;
	sll_parse_args(SLL_CHAR("ss"),al,all,&a,&b);
	sll_bool_t out=sll_api_file_rename(a,b);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_file_std_handle sll_api_file_std_handle(__SLL_API_ARGS_sll_api_file_std_handle);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_std_handle_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_integer_t out=sll_api_file_std_handle(a);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_file_write sll_api_file_write(__SLL_API_ARGS_sll_api_file_write);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_write_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_string_t* b;
	sll_parse_args(SLL_CHAR("is"),al,all,&a,&b);
	sll_integer_t out=sll_api_file_write(a,b);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_float_get_compare_error sll_api_float_get_compare_error(__SLL_API_ARGS_sll_api_float_get_compare_error);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_float_get_compare_error_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t out=sll_api_float_get_compare_error();
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_float_set_compare_error sll_api_float_set_compare_error(__SLL_API_ARGS_sll_api_float_set_compare_error);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_float_set_compare_error_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_api_float_set_compare_error(a);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_hash_md5 sll_api_hash_md5(__SLL_API_ARGS_sll_api_hash_md5);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_hash_md5_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_integer_t b;
	sll_integer_t c;
	sll_integer_t d;
	sll_string_t* e;
	sll_parse_args(SLL_CHAR("iiiis"),al,all,&a,&b,&c,&d,&e);
	sll_array_t out;
	sll_api_hash_md5(a,b,c,d,e,&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_hash_sha1 sll_api_hash_sha1(__SLL_API_ARGS_sll_api_hash_sha1);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_hash_sha1_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_integer_t b;
	sll_integer_t c;
	sll_integer_t d;
	sll_integer_t e;
	sll_string_t* f;
	sll_parse_args(SLL_CHAR("iiiiis"),al,all,&a,&b,&c,&d,&e,&f);
	sll_array_t out;
	sll_api_hash_sha1(a,b,c,d,e,f,&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_hash_sha256 sll_api_hash_sha256(__SLL_API_ARGS_sll_api_hash_sha256);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_hash_sha256_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_integer_t b;
	sll_integer_t c;
	sll_integer_t d;
	sll_integer_t e;
	sll_integer_t f;
	sll_integer_t g;
	sll_integer_t h;
	sll_string_t* i;
	sll_parse_args(SLL_CHAR("iiiiiiiis"),al,all,&a,&b,&c,&d,&e,&f,&g,&h,&i);
	sll_array_t out;
	sll_api_hash_sha256(a,b,c,d,e,f,g,h,i,&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_hash_sha512 sll_api_hash_sha512(__SLL_API_ARGS_sll_api_hash_sha512);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_hash_sha512_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_integer_t b;
	sll_integer_t c;
	sll_integer_t d;
	sll_integer_t e;
	sll_integer_t f;
	sll_integer_t g;
	sll_integer_t h;
	sll_string_t* i;
	sll_parse_args(SLL_CHAR("iiiiiiiis"),al,all,&a,&b,&c,&d,&e,&f,&g,&h,&i);
	sll_array_t out;
	sll_api_hash_sha512(a,b,c,d,e,f,g,h,i,&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_int_to_bin sll_api_int_to_bin(__SLL_API_ARGS_sll_api_int_to_bin);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_int_to_bin_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_string_t out;
	sll_api_int_to_bin(a,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_int_to_dec sll_api_int_to_dec(__SLL_API_ARGS_sll_api_int_to_dec);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_int_to_dec_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_string_t out;
	sll_api_int_to_dec(a,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_int_to_hex sll_api_int_to_hex(__SLL_API_ARGS_sll_api_int_to_hex);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_int_to_hex_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_string_t out;
	sll_api_int_to_hex(a,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_int_to_oct sll_api_int_to_oct(__SLL_API_ARGS_sll_api_int_to_oct);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_int_to_oct_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_string_t out;
	sll_api_int_to_oct(a,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_json__init sll_api_json__init(__SLL_API_ARGS_sll_api_json__init);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_json__init_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a;
	sll_object_t* b;
	sll_object_t* c;
	sll_parse_args(SLL_CHAR("ooo"),al,all,&a,&b,&c);
	sll_api_json__init(a,b,c);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_json_parse sll_api_json_parse(__SLL_API_ARGS_sll_api_json_parse);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_json_parse_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_object_t* out=sll_api_json_parse(a);
	return out;
}



__SLL_API_TYPE_sll_api_json_stringify sll_api_json_stringify(__SLL_API_ARGS_sll_api_json_stringify);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_json_stringify_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a;
	sll_parse_args(SLL_CHAR("o"),al,all,&a);
	sll_string_t out;
	sll_api_json_stringify(a,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_log_log sll_api_log_log(__SLL_API_ARGS_sll_api_log_log);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_log_log_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t** a;
	sll_arg_count_t ac;
	sll_parse_args(SLL_CHAR("o+"),al,all,&a,&ac);
	sll_api_log_log(a,ac);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_log_set_default sll_api_log_set_default(__SLL_API_ARGS_sll_api_log_set_default);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_log_set_default_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_bool_t b;
	sll_parse_args(SLL_CHAR("ib"),al,all,&a,&b);
	sll_api_log_set_default(a,b);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_log_set_file sll_api_log_set_file(__SLL_API_ARGS_sll_api_log_set_file);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_log_set_file_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_integer_t b;
	sll_bool_t c;
	sll_parse_args(SLL_CHAR("sib"),al,all,&a,&b,&c);
	sll_api_log_set_file(a,b,c);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_log_set_function sll_api_log_set_function(__SLL_API_ARGS_sll_api_log_set_function);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_log_set_function_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_string_t* b;
	sll_integer_t c;
	sll_bool_t d;
	sll_parse_args(SLL_CHAR("ssib"),al,all,&a,&b,&c,&d);
	sll_api_log_set_function(a,b,c,d);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_map_extend sll_api_map_extend(__SLL_API_ARGS_sll_api_map_extend);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_map_extend_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_map_t* a;
	sll_map_t* b;
	sll_parse_args(SLL_CHAR("mm"),al,all,&a,&b);
	sll_api_map_extend(a,b);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_map_remove sll_api_map_remove(__SLL_API_ARGS_sll_api_map_remove);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_map_remove_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_map_t* a;
	sll_object_t* b;
	sll_parse_args(SLL_CHAR("mo"),al,all,&a,&b);
	sll_api_map_remove(a,b);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_math_abs sll_api_math_abs(__SLL_API_ARGS_sll_api_math_abs);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_abs_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_float_t out=sll_api_math_abs(a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_math_acos sll_api_math_acos(__SLL_API_ARGS_sll_api_math_acos);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_acos_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_float_t out=sll_api_math_acos(a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_math_acosh sll_api_math_acosh(__SLL_API_ARGS_sll_api_math_acosh);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_acosh_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_float_t out=sll_api_math_acosh(a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_math_asin sll_api_math_asin(__SLL_API_ARGS_sll_api_math_asin);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_asin_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_float_t out=sll_api_math_asin(a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_math_asinh sll_api_math_asinh(__SLL_API_ARGS_sll_api_math_asinh);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_asinh_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_float_t out=sll_api_math_asinh(a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_math_atan sll_api_math_atan(__SLL_API_ARGS_sll_api_math_atan);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_atan_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_float_t out=sll_api_math_atan(a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_math_atan2 sll_api_math_atan2(__SLL_API_ARGS_sll_api_math_atan2);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_atan2_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_float_t b;
	sll_parse_args(SLL_CHAR("ff"),al,all,&a,&b);
	sll_float_t out=sll_api_math_atan2(a,b);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_math_atanh sll_api_math_atanh(__SLL_API_ARGS_sll_api_math_atanh);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_atanh_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_float_t out=sll_api_math_atanh(a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_math_cbrt sll_api_math_cbrt(__SLL_API_ARGS_sll_api_math_cbrt);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_cbrt_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_float_t out=sll_api_math_cbrt(a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_math_ceil sll_api_math_ceil(__SLL_API_ARGS_sll_api_math_ceil);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_ceil_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_integer_t out=sll_api_math_ceil(a);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_math_combinations sll_api_math_combinations(__SLL_API_ARGS_sll_api_math_combinations);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_combinations_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_integer_t b;
	sll_parse_args(SLL_CHAR("ii"),al,all,&a,&b);
	sll_integer_t out=sll_api_math_combinations(a,b);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_math_copy_sign sll_api_math_copy_sign(__SLL_API_ARGS_sll_api_math_copy_sign);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_copy_sign_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_float_t b;
	sll_parse_args(SLL_CHAR("ff"),al,all,&a,&b);
	sll_float_t out=sll_api_math_copy_sign(a,b);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_math_cos sll_api_math_cos(__SLL_API_ARGS_sll_api_math_cos);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_cos_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_float_t out=sll_api_math_cos(a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_math_cosh sll_api_math_cosh(__SLL_API_ARGS_sll_api_math_cosh);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_cosh_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_float_t out=sll_api_math_cosh(a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_math_euler_phi sll_api_math_euler_phi(__SLL_API_ARGS_sll_api_math_euler_phi);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_euler_phi_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_integer_t out=sll_api_math_euler_phi(a);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_math_factorial sll_api_math_factorial(__SLL_API_ARGS_sll_api_math_factorial);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_factorial_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_integer_t out=sll_api_math_factorial(a);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_math_factors sll_api_math_factors(__SLL_API_ARGS_sll_api_math_factors);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_factors_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_array_t out;
	sll_api_math_factors(a,&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_math_floor sll_api_math_floor(__SLL_API_ARGS_sll_api_math_floor);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_floor_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_integer_t out=sll_api_math_floor(a);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_math_gcd sll_api_math_gcd(__SLL_API_ARGS_sll_api_math_gcd);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_gcd_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_integer_t b;
	sll_parse_args(SLL_CHAR("ii"),al,all,&a,&b);
	sll_integer_t out=sll_api_math_gcd(a,b);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_math_int_log2 sll_api_math_int_log2(__SLL_API_ARGS_sll_api_math_int_log2);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_int_log2_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_integer_t out=sll_api_math_int_log2(a);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_math_int_pow sll_api_math_int_pow(__SLL_API_ARGS_sll_api_math_int_pow);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_int_pow_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_integer_t b;
	sll_parse_args(SLL_CHAR("ii"),al,all,&a,&b);
	sll_integer_t out=sll_api_math_int_pow(a,b);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_math_int_sqrt sll_api_math_int_sqrt(__SLL_API_ARGS_sll_api_math_int_sqrt);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_int_sqrt_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_integer_t out=sll_api_math_int_sqrt(a);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_math_log sll_api_math_log(__SLL_API_ARGS_sll_api_math_log);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_log_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_float_t out=sll_api_math_log(a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_math_log10 sll_api_math_log10(__SLL_API_ARGS_sll_api_math_log10);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_log10_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_float_t out=sll_api_math_log10(a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_math_log2 sll_api_math_log2(__SLL_API_ARGS_sll_api_math_log2);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_log2_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_float_t out=sll_api_math_log2(a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_math_permutations sll_api_math_permutations(__SLL_API_ARGS_sll_api_math_permutations);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_permutations_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_integer_t b;
	sll_parse_args(SLL_CHAR("ii"),al,all,&a,&b);
	sll_integer_t out=sll_api_math_permutations(a,b);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_math_pow sll_api_math_pow(__SLL_API_ARGS_sll_api_math_pow);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_pow_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_float_t b;
	sll_parse_args(SLL_CHAR("ff"),al,all,&a,&b);
	sll_float_t out=sll_api_math_pow(a,b);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_math_round sll_api_math_round(__SLL_API_ARGS_sll_api_math_round);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_round_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_integer_t out=sll_api_math_round(a);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_math_sin sll_api_math_sin(__SLL_API_ARGS_sll_api_math_sin);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_sin_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_float_t out=sll_api_math_sin(a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_math_sinh sll_api_math_sinh(__SLL_API_ARGS_sll_api_math_sinh);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_sinh_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_float_t out=sll_api_math_sinh(a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_math_sqrt sll_api_math_sqrt(__SLL_API_ARGS_sll_api_math_sqrt);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_sqrt_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_float_t out=sll_api_math_sqrt(a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_math_tan sll_api_math_tan(__SLL_API_ARGS_sll_api_math_tan);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_tan_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_float_t out=sll_api_math_tan(a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_math_tanh sll_api_math_tanh(__SLL_API_ARGS_sll_api_math_tanh);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_tanh_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_float_t out=sll_api_math_tanh(a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_object_new sll_api_object_new(__SLL_API_ARGS_sll_api_object_new);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_object_new_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_object_t** b;
	sll_arg_count_t bc;
	sll_parse_args(SLL_CHAR("so+"),al,all,&a,&b,&bc);
	sll_object_t* out=sll_api_object_new(a,b,bc);
	return out;
}



__SLL_API_TYPE_sll_api_path_absolute sll_api_path_absolute(__SLL_API_ARGS_sll_api_path_absolute);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_absolute_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_string_t out;
	sll_api_path_absolute(a,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_path_exists sll_api_path_exists(__SLL_API_ARGS_sll_api_path_exists);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_exists_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_integer_t out=sll_api_path_exists(a);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_path_get_cwd sll_api_path_get_cwd(__SLL_API_ARGS_sll_api_path_get_cwd);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_get_cwd_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t out;
	sll_api_path_get_cwd(&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_path_is_dir sll_api_path_is_dir(__SLL_API_ARGS_sll_api_path_is_dir);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_is_dir_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_integer_t out=sll_api_path_is_dir(a);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_path_join sll_api_path_join(__SLL_API_ARGS_sll_api_path_join);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_join_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t** a;
	sll_arg_count_t ac;
	sll_parse_args(SLL_CHAR("s+"),al,all,&a,&ac);
	sll_string_t out;
	sll_api_path_join(a,ac,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_path_list_dir sll_api_path_list_dir(__SLL_API_ARGS_sll_api_path_list_dir);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_list_dir_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_array_t out;
	sll_api_path_list_dir(a,&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_path_mkdir sll_api_path_mkdir(__SLL_API_ARGS_sll_api_path_mkdir);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_mkdir_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_bool_t b;
	sll_parse_args(SLL_CHAR("sb"),al,all,&a,&b);
	sll_bool_t out=sll_api_path_mkdir(a,b);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_path_recursive_list_dir sll_api_path_recursive_list_dir(__SLL_API_ARGS_sll_api_path_recursive_list_dir);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_recursive_list_dir_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_array_t out;
	sll_api_path_recursive_list_dir(a,&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_path_relative sll_api_path_relative(__SLL_API_ARGS_sll_api_path_relative);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_relative_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_string_t* b;
	sll_parse_args(SLL_CHAR("ss"),al,all,&a,&b);
	sll_string_t out;
	sll_api_path_relative(a,b,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_path_set_cwd sll_api_path_set_cwd(__SLL_API_ARGS_sll_api_path_set_cwd);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_set_cwd_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_integer_t out=sll_api_path_set_cwd(a);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_path_size sll_api_path_size(__SLL_API_ARGS_sll_api_path_size);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_size_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_integer_t out=sll_api_path_size(a);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_path_split sll_api_path_split(__SLL_API_ARGS_sll_api_path_split);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_split_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_array_t out;
	sll_api_path_split(a,&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_path_split_drive sll_api_path_split_drive(__SLL_API_ARGS_sll_api_path_split_drive);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_split_drive_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_array_t out;
	sll_api_path_split_drive(a,&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_process_execute_shell sll_api_process_execute_shell(__SLL_API_ARGS_sll_api_process_execute_shell);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_process_execute_shell_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_bool_t out=sll_api_process_execute_shell(a);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_process_get_pid sll_api_process_get_pid(__SLL_API_ARGS_sll_api_process_get_pid);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_process_get_pid_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t out=sll_api_process_get_pid();
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_process_join sll_api_process_join(__SLL_API_ARGS_sll_api_process_join);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_process_join_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_array_t* a;
	sll_parse_args(SLL_CHAR("a"),al,all,&a);
	sll_string_t out;
	sll_api_process_join(a,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_process_split sll_api_process_split(__SLL_API_ARGS_sll_api_process_split);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_process_split_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_array_t out;
	sll_api_process_split(a,&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_process_start sll_api_process_start(__SLL_API_ARGS_sll_api_process_start);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_process_start_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_array_t* a;
	sll_string_t* b;
	sll_integer_t c;
	sll_string_t* d;
	sll_parse_args(SLL_CHAR("asis"),al,all,&a,&b,&c,&d);
	sll_array_t out;
	sll_api_process_start(a,b,c,d,&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_random_get_float sll_api_random_get_float(__SLL_API_ARGS_sll_api_random_get_float);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_random_get_float_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_float_t b;
	sll_parse_args(SLL_CHAR("ff"),al,all,&a,&b);
	sll_float_t out=sll_api_random_get_float(a,b);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_random_get_int sll_api_random_get_int(__SLL_API_ARGS_sll_api_random_get_int);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_random_get_int_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_integer_t b;
	sll_parse_args(SLL_CHAR("ii"),al,all,&a,&b);
	sll_integer_t out=sll_api_random_get_int(a,b);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_random_get_string sll_api_random_get_string(__SLL_API_ARGS_sll_api_random_get_string);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_random_get_string_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_char_t b;
	sll_char_t c;
	sll_parse_args(SLL_CHAR("icc"),al,all,&a,&b,&c);
	sll_string_t out;
	sll_api_random_get_string(a,b,c,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_serial_decode_float sll_api_serial_decode_float(__SLL_API_ARGS_sll_api_serial_decode_float);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_serial_decode_float_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_float_t out=sll_api_serial_decode_float(a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_serial_decode_integer sll_api_serial_decode_integer(__SLL_API_ARGS_sll_api_serial_decode_integer);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_serial_decode_integer_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_integer_t out=sll_api_serial_decode_integer(a);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_serial_decode_object sll_api_serial_decode_object(__SLL_API_ARGS_sll_api_serial_decode_object);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_serial_decode_object_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_object_t* out=sll_api_serial_decode_object(a);
	return out;
}



__SLL_API_TYPE_sll_api_serial_decode_signed_integer sll_api_serial_decode_signed_integer(__SLL_API_ARGS_sll_api_serial_decode_signed_integer);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_serial_decode_signed_integer_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_integer_t out=sll_api_serial_decode_signed_integer(a);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_serial_decode_string sll_api_serial_decode_string(__SLL_API_ARGS_sll_api_serial_decode_string);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_serial_decode_string_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_object_t* out=sll_api_serial_decode_string(a);
	return out;
}



__SLL_API_TYPE_sll_api_serial_encode_float sll_api_serial_encode_float(__SLL_API_ARGS_sll_api_serial_encode_float);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_serial_encode_float_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_float_t b;
	sll_parse_args(SLL_CHAR("if"),al,all,&a,&b);
	sll_api_serial_encode_float(a,b);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_serial_encode_integer sll_api_serial_encode_integer(__SLL_API_ARGS_sll_api_serial_encode_integer);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_serial_encode_integer_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_integer_t b;
	sll_parse_args(SLL_CHAR("ii"),al,all,&a,&b);
	sll_api_serial_encode_integer(a,b);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_serial_encode_object sll_api_serial_encode_object(__SLL_API_ARGS_sll_api_serial_encode_object);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_serial_encode_object_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_object_t** b;
	sll_arg_count_t bc;
	sll_parse_args(SLL_CHAR("io+"),al,all,&a,&b,&bc);
	sll_api_serial_encode_object(a,b,bc);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_serial_encode_signed_integer sll_api_serial_encode_signed_integer(__SLL_API_ARGS_sll_api_serial_encode_signed_integer);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_serial_encode_signed_integer_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_integer_t b;
	sll_parse_args(SLL_CHAR("ii"),al,all,&a,&b);
	sll_api_serial_encode_signed_integer(a,b);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_serial_encode_string sll_api_serial_encode_string(__SLL_API_ARGS_sll_api_serial_encode_string);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_serial_encode_string_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_string_t* b;
	sll_parse_args(SLL_CHAR("is"),al,all,&a,&b);
	sll_api_serial_encode_string(a,b);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_sort_quicksort sll_api_sort_quicksort(__SLL_API_ARGS_sll_api_sort_quicksort);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sort_quicksort_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_array_t* a;
	sll_bool_t b;
	sll_bool_t c;
	sll_integer_t d;
	sll_parse_args(SLL_CHAR("abbi"),al,all,&a,&b,&c,&d);
	sll_object_t* out=sll_api_sort_quicksort(a,b,c,d);
	return out;
}



__SLL_API_TYPE_sll_api_string_checksum sll_api_string_checksum(__SLL_API_ARGS_sll_api_string_checksum);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_checksum_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_integer_t out=sll_api_string_checksum(a);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_string_convert sll_api_string_convert(__SLL_API_ARGS_sll_api_string_convert);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_convert_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t** a;
	sll_arg_count_t ac;
	sll_parse_args(SLL_CHAR("o+"),al,all,&a,&ac);
	sll_string_t out;
	sll_api_string_convert(a,ac,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_string_count sll_api_string_count(__SLL_API_ARGS_sll_api_string_count);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_count_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_char_string_t b;
	sll_parse_args(SLL_CHAR("sy"),al,all,&a,&b);
	sll_integer_t out=sll_api_string_count(a,&b);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_string_count_left sll_api_string_count_left(__SLL_API_ARGS_sll_api_string_count_left);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_count_left_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_char_t b;
	sll_parse_args(SLL_CHAR("sc"),al,all,&a,&b);
	sll_integer_t out=sll_api_string_count_left(a,b);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_string_count_right sll_api_string_count_right(__SLL_API_ARGS_sll_api_string_count_right);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_count_right_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_char_t b;
	sll_parse_args(SLL_CHAR("sc"),al,all,&a,&b);
	sll_integer_t out=sll_api_string_count_right(a,b);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_string_ends sll_api_string_ends(__SLL_API_ARGS_sll_api_string_ends);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_ends_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_char_string_t b;
	sll_parse_args(SLL_CHAR("sy"),al,all,&a,&b);
	sll_bool_t out=sll_api_string_ends(a,&b);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_string_flip_case sll_api_string_flip_case(__SLL_API_ARGS_sll_api_string_flip_case);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_flip_case_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_string_t out;
	sll_api_string_flip_case(a,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_string_format sll_api_string_format(__SLL_API_ARGS_sll_api_string_format);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_format_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_object_t** b;
	sll_arg_count_t bc;
	sll_parse_args(SLL_CHAR("so+"),al,all,&a,&b,&bc);
	sll_string_t out;
	sll_api_string_format(a,b,bc,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_string_index sll_api_string_index(__SLL_API_ARGS_sll_api_string_index);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_index_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_char_string_t b;
	sll_parse_args(SLL_CHAR("sy"),al,all,&a,&b);
	sll_integer_t out=sll_api_string_index(a,&b);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_string_index_list sll_api_string_index_list(__SLL_API_ARGS_sll_api_string_index_list);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_index_list_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_char_string_t b;
	sll_bool_t c;
	sll_parse_args(SLL_CHAR("syb"),al,all,&a,&b,&c);
	sll_integer_t out=sll_api_string_index_list(a,&b,c);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_string_index_reverse sll_api_string_index_reverse(__SLL_API_ARGS_sll_api_string_index_reverse);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_index_reverse_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_char_string_t b;
	sll_parse_args(SLL_CHAR("sy"),al,all,&a,&b);
	sll_integer_t out=sll_api_string_index_reverse(a,&b);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_string_index_reverse_list sll_api_string_index_reverse_list(__SLL_API_ARGS_sll_api_string_index_reverse_list);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_index_reverse_list_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_char_string_t b;
	sll_bool_t c;
	sll_parse_args(SLL_CHAR("syb"),al,all,&a,&b,&c);
	sll_integer_t out=sll_api_string_index_reverse_list(a,&b,c);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_string_join sll_api_string_join(__SLL_API_ARGS_sll_api_string_join);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_join_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_char_string_t a;
	sll_array_t* b;
	sll_parse_args(SLL_CHAR("ya"),al,all,&a,&b);
	sll_string_t out;
	sll_api_string_join(&a,b,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_string_pad sll_api_string_pad(__SLL_API_ARGS_sll_api_string_pad);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_pad_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_integer_t b;
	sll_char_t c;
	sll_parse_args(SLL_CHAR("sic"),al,all,&a,&b,&c);
	sll_string_t out;
	sll_api_string_pad(a,b,c,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_string_pad_left sll_api_string_pad_left(__SLL_API_ARGS_sll_api_string_pad_left);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_pad_left_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_integer_t b;
	sll_char_t c;
	sll_parse_args(SLL_CHAR("sic"),al,all,&a,&b,&c);
	sll_string_t out;
	sll_api_string_pad_left(a,b,c,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_string_pad_right sll_api_string_pad_right(__SLL_API_ARGS_sll_api_string_pad_right);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_pad_right_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_integer_t b;
	sll_char_t c;
	sll_parse_args(SLL_CHAR("sic"),al,all,&a,&b,&c);
	sll_string_t out;
	sll_api_string_pad_right(a,b,c,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_string_replace sll_api_string_replace(__SLL_API_ARGS_sll_api_string_replace);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_replace_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_char_string_t b;
	sll_char_string_t c;
	sll_parse_args(SLL_CHAR("syy"),al,all,&a,&b,&c);
	sll_string_t out;
	sll_api_string_replace(a,&b,&c,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_string_reverse sll_api_string_reverse(__SLL_API_ARGS_sll_api_string_reverse);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_reverse_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_string_t out;
	sll_api_string_reverse(a,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_string_secure_equal sll_api_string_secure_equal(__SLL_API_ARGS_sll_api_string_secure_equal);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_secure_equal_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_string_t* b;
	sll_parse_args(SLL_CHAR("ss"),al,all,&a,&b);
	sll_bool_t out=sll_api_string_secure_equal(a,b);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_string_split sll_api_string_split(__SLL_API_ARGS_sll_api_string_split);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_split_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_char_string_t b;
	sll_parse_args(SLL_CHAR("sy"),al,all,&a,&b);
	sll_array_t out;
	sll_api_string_split(a,&b,&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_string_starts sll_api_string_starts(__SLL_API_ARGS_sll_api_string_starts);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_starts_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_char_string_t b;
	sll_parse_args(SLL_CHAR("sy"),al,all,&a,&b);
	sll_bool_t out=sll_api_string_starts(a,&b);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_string_to_lower_case sll_api_string_to_lower_case(__SLL_API_ARGS_sll_api_string_to_lower_case);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_to_lower_case_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_string_t out;
	sll_api_string_to_lower_case(a,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_string_to_title_case sll_api_string_to_title_case(__SLL_API_ARGS_sll_api_string_to_title_case);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_to_title_case_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_string_t out;
	sll_api_string_to_title_case(a,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_string_to_upper_case sll_api_string_to_upper_case(__SLL_API_ARGS_sll_api_string_to_upper_case);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_to_upper_case_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_string_t out;
	sll_api_string_to_upper_case(a,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_string_trim sll_api_string_trim(__SLL_API_ARGS_sll_api_string_trim);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_trim_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_string_t out;
	sll_api_string_trim(a,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_string_trim_left sll_api_string_trim_left(__SLL_API_ARGS_sll_api_string_trim_left);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_trim_left_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_string_t out;
	sll_api_string_trim_left(a,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_string_trim_right sll_api_string_trim_right(__SLL_API_ARGS_sll_api_string_trim_right);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_trim_right_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_string_t out;
	sll_api_string_trim_right(a,&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_struct_double_from_bits sll_api_struct_double_from_bits(__SLL_API_ARGS_sll_api_struct_double_from_bits);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_struct_double_from_bits_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_float_t out=sll_api_struct_double_from_bits(a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_struct_double_to_bits sll_api_struct_double_to_bits(__SLL_API_ARGS_sll_api_struct_double_to_bits);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_struct_double_to_bits_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_integer_t out=sll_api_struct_double_to_bits(a);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_struct_float_from_bits sll_api_struct_float_from_bits(__SLL_API_ARGS_sll_api_struct_float_from_bits);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_struct_float_from_bits_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_float_t out=sll_api_struct_float_from_bits(a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_struct_float_to_bits sll_api_struct_float_to_bits(__SLL_API_ARGS_sll_api_struct_float_to_bits);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_struct_float_to_bits_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t a;
	sll_parse_args(SLL_CHAR("f"),al,all,&a);
	sll_integer_t out=sll_api_struct_float_to_bits(a);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_sys_get_args sll_api_sys_get_args(__SLL_API_ARGS_sll_api_sys_get_args);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_get_args_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_array_t out;
	sll_api_sys_get_args(&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_sys_get_cpu_count sll_api_sys_get_cpu_count(__SLL_API_ARGS_sll_api_sys_get_cpu_count);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_get_cpu_count_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t out=sll_api_sys_get_cpu_count();
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_sys_get_env sll_api_sys_get_env(__SLL_API_ARGS_sll_api_sys_get_env);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_get_env_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_map_t out;
	sll_api_sys_get_env(&out);
	return sll_map_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_sys_get_executable sll_api_sys_get_executable(__SLL_API_ARGS_sll_api_sys_get_executable);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_get_executable_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t out;
	sll_api_sys_get_executable(&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_sys_get_library sll_api_sys_get_library(__SLL_API_ARGS_sll_api_sys_get_library);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_get_library_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t out;
	sll_api_sys_get_library(&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_sys_get_platform sll_api_sys_get_platform(__SLL_API_ARGS_sll_api_sys_get_platform);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_get_platform_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t out;
	sll_api_sys_get_platform(&out);
	return sll_string_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_sys_get_sandbox_flags sll_api_sys_get_sandbox_flags(__SLL_API_ARGS_sll_api_sys_get_sandbox_flags);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_get_sandbox_flags_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_array_t out;
	sll_api_sys_get_sandbox_flags(&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_sys_get_version sll_api_sys_get_version(__SLL_API_ARGS_sll_api_sys_get_version);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_get_version_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_array_t out;
	sll_api_sys_get_version(&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_sys_load_library sll_api_sys_load_library(__SLL_API_ARGS_sll_api_sys_load_library);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_load_library_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_integer_t b;
	sll_integer_t c;
	sll_integer_t d;
	sll_integer_t e;
	sll_integer_t f;
	sll_parse_args(SLL_CHAR("siiiii"),al,all,&a,&b,&c,&d,&e,&f);
	sll_bool_t out=sll_api_sys_load_library(a,b,c,d,e,f);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_sys_remove_env sll_api_sys_remove_env(__SLL_API_ARGS_sll_api_sys_remove_env);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_remove_env_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_api_sys_remove_env(a);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_sys_set_env sll_api_sys_set_env(__SLL_API_ARGS_sll_api_sys_set_env);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_set_env_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_string_t* b;
	sll_parse_args(SLL_CHAR("ss"),al,all,&a,&b);
	sll_api_sys_set_env(a,b);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_sys_set_sandbox_flag sll_api_sys_set_sandbox_flag(__SLL_API_ARGS_sll_api_sys_set_sandbox_flag);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_set_sandbox_flag_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t* a;
	sll_parse_args(SLL_CHAR("s"),al,all,&a);
	sll_bool_t out=sll_api_sys_set_sandbox_flag(a);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_thread_create sll_api_thread_create(__SLL_API_ARGS_sll_api_thread_create);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_thread_create_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_array_t* b;
	sll_parse_args(SLL_CHAR("ia"),al,all,&a,&b);
	sll_integer_t out=sll_api_thread_create(a,b);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_thread_create_barrier sll_api_thread_create_barrier(__SLL_API_ARGS_sll_api_thread_create_barrier);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_thread_create_barrier_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t out=sll_api_thread_create_barrier();
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_thread_create_lock sll_api_thread_create_lock(__SLL_API_ARGS_sll_api_thread_create_lock);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_thread_create_lock_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t out=sll_api_thread_create_lock();
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_thread_create_semaphore sll_api_thread_create_semaphore(__SLL_API_ARGS_sll_api_thread_create_semaphore);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_thread_create_semaphore_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_integer_t out=sll_api_thread_create_semaphore(a);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_thread_get_internal_data sll_api_thread_get_internal_data(__SLL_API_ARGS_sll_api_thread_get_internal_data);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_thread_get_internal_data_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_array_t out;
	sll_api_thread_get_internal_data(a,&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_thread_increase_barrier sll_api_thread_increase_barrier(__SLL_API_ARGS_sll_api_thread_increase_barrier);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_thread_increase_barrier_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_integer_t out=sll_api_thread_increase_barrier(a);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_thread_release_lock sll_api_thread_release_lock(__SLL_API_ARGS_sll_api_thread_release_lock);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_thread_release_lock_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_bool_t out=sll_api_thread_release_lock(a);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_thread_release_semaphore sll_api_thread_release_semaphore(__SLL_API_ARGS_sll_api_thread_release_semaphore);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_thread_release_semaphore_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_bool_t out=sll_api_thread_release_semaphore(a);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_thread_reset_barrier sll_api_thread_reset_barrier(__SLL_API_ARGS_sll_api_thread_reset_barrier);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_thread_reset_barrier_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_integer_t out=sll_api_thread_reset_barrier(a);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_thread_restart sll_api_thread_restart(__SLL_API_ARGS_sll_api_thread_restart);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_thread_restart_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_bool_t out=sll_api_thread_restart(a);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_thread_suspend sll_api_thread_suspend(__SLL_API_ARGS_sll_api_thread_suspend);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_thread_suspend_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_bool_t out=sll_api_thread_suspend(a);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_time_current sll_api_time_current(__SLL_API_ARGS_sll_api_time_current);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_time_current_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t out=sll_api_time_current();
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_time_current_ns sll_api_time_current_ns(__SLL_API_ARGS_sll_api_time_current_ns);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_time_current_ns_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t out=sll_api_time_current_ns();
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_time_sleep sll_api_time_sleep(__SLL_API_ARGS_sll_api_time_sleep);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_time_sleep_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_int_float_t a;
	sll_parse_args(SLL_CHAR("x"),al,all,&a);
	sll_float_t out=sll_api_time_sleep(&a);
	return sll_float_to_object(out);
}



__SLL_API_TYPE_sll_api_time_sleep_ns sll_api_time_sleep_ns(__SLL_API_ARGS_sll_api_time_sleep_ns);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_time_sleep_ns_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_int_float_t a;
	sll_parse_args(SLL_CHAR("x"),al,all,&a);
	sll_integer_t out=sll_api_time_sleep_ns(&a);
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_vm_get_config sll_api_vm_get_config(__SLL_API_ARGS_sll_api_vm_get_config);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_vm_get_config_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_array_t out;
	sll_api_vm_get_config(&out);
	return sll_array_to_object_nocopy(&out);
}



__SLL_API_TYPE_sll_api_vm_get_instruction_count sll_api_vm_get_instruction_count(__SLL_API_ARGS_sll_api_vm_get_instruction_count);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_vm_get_instruction_count_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t out=sll_api_vm_get_instruction_count();
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_vm_get_instruction_index sll_api_vm_get_instruction_index(__SLL_API_ARGS_sll_api_vm_get_instruction_index);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_vm_get_instruction_index_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t out=sll_api_vm_get_instruction_index();
	return sll_int_to_object(out);
}



__SLL_API_TYPE_sll_api_vm_get_location sll_api_vm_get_location(__SLL_API_ARGS_sll_api_vm_get_location);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_vm_get_location_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t a;
	sll_parse_args(SLL_CHAR("i"),al,all,&a);
	sll_object_t* out=sll_api_vm_get_location(a);
	return out;
}



__SLL_API_TYPE_sll_api_vm_get_ref_count sll_api_vm_get_ref_count(__SLL_API_ARGS_sll_api_vm_get_ref_count);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_vm_get_ref_count_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a;
	sll_parse_args(SLL_CHAR("o"),al,all,&a);
	sll_integer_t out=sll_api_vm_get_ref_count(a);
	return sll_int_to_object(out);
}



static const internal_function_t _ifunc_data_ptr[]={
	{
		SLL_CHAR("sll:array_count"),
		sll_api_array_count_raw
	},
	{
		SLL_CHAR("sll:array_create"),
		sll_api_array_create_raw
	},
	{
		SLL_CHAR("sll:array_extend"),
		sll_api_array_extend_raw
	},
	{
		SLL_CHAR("sll:array_join"),
		sll_api_array_join_raw
	},
	{
		SLL_CHAR("sll:array_pop"),
		sll_api_array_pop_raw
	},
	{
		SLL_CHAR("sll:array_push"),
		sll_api_array_push_raw
	},
	{
		SLL_CHAR("sll:array_remove"),
		sll_api_array_remove_raw
	},
	{
		SLL_CHAR("sll:array_replace"),
		sll_api_array_replace_raw
	},
	{
		SLL_CHAR("sll:array_reverse"),
		sll_api_array_reverse_raw
	},
	{
		SLL_CHAR("sll:array_shift"),
		sll_api_array_shift_raw
	},
	{
		SLL_CHAR("sll:array_split"),
		sll_api_array_split_raw
	},
	{
		SLL_CHAR("sll:array_unshift"),
		sll_api_array_unshift_raw
	},
	{
		SLL_CHAR("sll:base64_decode"),
		sll_api_base64_decode_raw
	},
	{
		SLL_CHAR("sll:base64_encode"),
		sll_api_base64_encode_raw
	},
	{
		SLL_CHAR("sll:date_get_time_zone"),
		sll_api_date_get_time_zone_raw
	},
	{
		SLL_CHAR("sll:date_merge"),
		sll_api_date_merge_raw
	},
	{
		SLL_CHAR("sll:date_split"),
		sll_api_date_split_raw
	},
	{
		SLL_CHAR("sll:error_get_call_stack"),
		sll_api_error_get_call_stack_raw
	},
	{
		SLL_CHAR("sll:file_close"),
		sll_api_file_close_raw
	},
	{
		SLL_CHAR("sll:file_copy"),
		sll_api_file_copy_raw
	},
	{
		SLL_CHAR("sll:file_delete"),
		sll_api_file_delete_raw
	},
	{
		SLL_CHAR("sll:file_flush"),
		sll_api_file_flush_raw
	},
	{
		SLL_CHAR("sll:file_from_data"),
		sll_api_file_from_data_raw
	},
	{
		SLL_CHAR("sll:file_get_buffer"),
		sll_api_file_get_buffer_raw
	},
	{
		SLL_CHAR("sll:file_get_temp_path"),
		sll_api_file_get_temp_path_raw
	},
	{
		SLL_CHAR("sll:file_inc_handle"),
		sll_api_file_inc_handle_raw
	},
	{
		SLL_CHAR("sll:file_open"),
		sll_api_file_open_raw
	},
	{
		SLL_CHAR("sll:file_peek"),
		sll_api_file_peek_raw
	},
	{
		SLL_CHAR("sll:file_read"),
		sll_api_file_read_raw
	},
	{
		SLL_CHAR("sll:file_read_char"),
		sll_api_file_read_char_raw
	},
	{
		SLL_CHAR("sll:file_rename"),
		sll_api_file_rename_raw
	},
	{
		SLL_CHAR("sll:file_std_handle"),
		sll_api_file_std_handle_raw
	},
	{
		SLL_CHAR("sll:file_write"),
		sll_api_file_write_raw
	},
	{
		SLL_CHAR("sll:float_get_compare_error"),
		sll_api_float_get_compare_error_raw
	},
	{
		SLL_CHAR("sll:float_set_compare_error"),
		sll_api_float_set_compare_error_raw
	},
	{
		SLL_CHAR("sll:hash_md5"),
		sll_api_hash_md5_raw
	},
	{
		SLL_CHAR("sll:hash_sha1"),
		sll_api_hash_sha1_raw
	},
	{
		SLL_CHAR("sll:hash_sha256"),
		sll_api_hash_sha256_raw
	},
	{
		SLL_CHAR("sll:hash_sha512"),
		sll_api_hash_sha512_raw
	},
	{
		SLL_CHAR("sll:int_to_bin"),
		sll_api_int_to_bin_raw
	},
	{
		SLL_CHAR("sll:int_to_dec"),
		sll_api_int_to_dec_raw
	},
	{
		SLL_CHAR("sll:int_to_hex"),
		sll_api_int_to_hex_raw
	},
	{
		SLL_CHAR("sll:int_to_oct"),
		sll_api_int_to_oct_raw
	},
	{
		SLL_CHAR("sll:json__init"),
		sll_api_json__init_raw
	},
	{
		SLL_CHAR("sll:json_parse"),
		sll_api_json_parse_raw
	},
	{
		SLL_CHAR("sll:json_stringify"),
		sll_api_json_stringify_raw
	},
	{
		SLL_CHAR("sll:log_log"),
		sll_api_log_log_raw
	},
	{
		SLL_CHAR("sll:log_set_default"),
		sll_api_log_set_default_raw
	},
	{
		SLL_CHAR("sll:log_set_file"),
		sll_api_log_set_file_raw
	},
	{
		SLL_CHAR("sll:log_set_function"),
		sll_api_log_set_function_raw
	},
	{
		SLL_CHAR("sll:map_extend"),
		sll_api_map_extend_raw
	},
	{
		SLL_CHAR("sll:map_remove"),
		sll_api_map_remove_raw
	},
	{
		SLL_CHAR("sll:math_abs"),
		sll_api_math_abs_raw
	},
	{
		SLL_CHAR("sll:math_acos"),
		sll_api_math_acos_raw
	},
	{
		SLL_CHAR("sll:math_acosh"),
		sll_api_math_acosh_raw
	},
	{
		SLL_CHAR("sll:math_asin"),
		sll_api_math_asin_raw
	},
	{
		SLL_CHAR("sll:math_asinh"),
		sll_api_math_asinh_raw
	},
	{
		SLL_CHAR("sll:math_atan"),
		sll_api_math_atan_raw
	},
	{
		SLL_CHAR("sll:math_atan2"),
		sll_api_math_atan2_raw
	},
	{
		SLL_CHAR("sll:math_atanh"),
		sll_api_math_atanh_raw
	},
	{
		SLL_CHAR("sll:math_cbrt"),
		sll_api_math_cbrt_raw
	},
	{
		SLL_CHAR("sll:math_ceil"),
		sll_api_math_ceil_raw
	},
	{
		SLL_CHAR("sll:math_combinations"),
		sll_api_math_combinations_raw
	},
	{
		SLL_CHAR("sll:math_copy_sign"),
		sll_api_math_copy_sign_raw
	},
	{
		SLL_CHAR("sll:math_cos"),
		sll_api_math_cos_raw
	},
	{
		SLL_CHAR("sll:math_cosh"),
		sll_api_math_cosh_raw
	},
	{
		SLL_CHAR("sll:math_euler_phi"),
		sll_api_math_euler_phi_raw
	},
	{
		SLL_CHAR("sll:math_factorial"),
		sll_api_math_factorial_raw
	},
	{
		SLL_CHAR("sll:math_factors"),
		sll_api_math_factors_raw
	},
	{
		SLL_CHAR("sll:math_floor"),
		sll_api_math_floor_raw
	},
	{
		SLL_CHAR("sll:math_gcd"),
		sll_api_math_gcd_raw
	},
	{
		SLL_CHAR("sll:math_int_log2"),
		sll_api_math_int_log2_raw
	},
	{
		SLL_CHAR("sll:math_int_pow"),
		sll_api_math_int_pow_raw
	},
	{
		SLL_CHAR("sll:math_int_sqrt"),
		sll_api_math_int_sqrt_raw
	},
	{
		SLL_CHAR("sll:math_log"),
		sll_api_math_log_raw
	},
	{
		SLL_CHAR("sll:math_log10"),
		sll_api_math_log10_raw
	},
	{
		SLL_CHAR("sll:math_log2"),
		sll_api_math_log2_raw
	},
	{
		SLL_CHAR("sll:math_permutations"),
		sll_api_math_permutations_raw
	},
	{
		SLL_CHAR("sll:math_pow"),
		sll_api_math_pow_raw
	},
	{
		SLL_CHAR("sll:math_round"),
		sll_api_math_round_raw
	},
	{
		SLL_CHAR("sll:math_sin"),
		sll_api_math_sin_raw
	},
	{
		SLL_CHAR("sll:math_sinh"),
		sll_api_math_sinh_raw
	},
	{
		SLL_CHAR("sll:math_sqrt"),
		sll_api_math_sqrt_raw
	},
	{
		SLL_CHAR("sll:math_tan"),
		sll_api_math_tan_raw
	},
	{
		SLL_CHAR("sll:math_tanh"),
		sll_api_math_tanh_raw
	},
	{
		SLL_CHAR("sll:object_new"),
		sll_api_object_new_raw
	},
	{
		SLL_CHAR("sll:path_absolute"),
		sll_api_path_absolute_raw
	},
	{
		SLL_CHAR("sll:path_exists"),
		sll_api_path_exists_raw
	},
	{
		SLL_CHAR("sll:path_get_cwd"),
		sll_api_path_get_cwd_raw
	},
	{
		SLL_CHAR("sll:path_is_dir"),
		sll_api_path_is_dir_raw
	},
	{
		SLL_CHAR("sll:path_join"),
		sll_api_path_join_raw
	},
	{
		SLL_CHAR("sll:path_list_dir"),
		sll_api_path_list_dir_raw
	},
	{
		SLL_CHAR("sll:path_mkdir"),
		sll_api_path_mkdir_raw
	},
	{
		SLL_CHAR("sll:path_recursive_list_dir"),
		sll_api_path_recursive_list_dir_raw
	},
	{
		SLL_CHAR("sll:path_relative"),
		sll_api_path_relative_raw
	},
	{
		SLL_CHAR("sll:path_set_cwd"),
		sll_api_path_set_cwd_raw
	},
	{
		SLL_CHAR("sll:path_size"),
		sll_api_path_size_raw
	},
	{
		SLL_CHAR("sll:path_split"),
		sll_api_path_split_raw
	},
	{
		SLL_CHAR("sll:path_split_drive"),
		sll_api_path_split_drive_raw
	},
	{
		SLL_CHAR("sll:process_execute_shell"),
		sll_api_process_execute_shell_raw
	},
	{
		SLL_CHAR("sll:process_get_pid"),
		sll_api_process_get_pid_raw
	},
	{
		SLL_CHAR("sll:process_join"),
		sll_api_process_join_raw
	},
	{
		SLL_CHAR("sll:process_split"),
		sll_api_process_split_raw
	},
	{
		SLL_CHAR("sll:process_start"),
		sll_api_process_start_raw
	},
	{
		SLL_CHAR("sll:random_get_float"),
		sll_api_random_get_float_raw
	},
	{
		SLL_CHAR("sll:random_get_int"),
		sll_api_random_get_int_raw
	},
	{
		SLL_CHAR("sll:random_get_string"),
		sll_api_random_get_string_raw
	},
	{
		SLL_CHAR("sll:serial_decode_float"),
		sll_api_serial_decode_float_raw
	},
	{
		SLL_CHAR("sll:serial_decode_integer"),
		sll_api_serial_decode_integer_raw
	},
	{
		SLL_CHAR("sll:serial_decode_object"),
		sll_api_serial_decode_object_raw
	},
	{
		SLL_CHAR("sll:serial_decode_signed_integer"),
		sll_api_serial_decode_signed_integer_raw
	},
	{
		SLL_CHAR("sll:serial_decode_string"),
		sll_api_serial_decode_string_raw
	},
	{
		SLL_CHAR("sll:serial_encode_float"),
		sll_api_serial_encode_float_raw
	},
	{
		SLL_CHAR("sll:serial_encode_integer"),
		sll_api_serial_encode_integer_raw
	},
	{
		SLL_CHAR("sll:serial_encode_object"),
		sll_api_serial_encode_object_raw
	},
	{
		SLL_CHAR("sll:serial_encode_signed_integer"),
		sll_api_serial_encode_signed_integer_raw
	},
	{
		SLL_CHAR("sll:serial_encode_string"),
		sll_api_serial_encode_string_raw
	},
	{
		SLL_CHAR("sll:sort_quicksort"),
		sll_api_sort_quicksort_raw
	},
	{
		SLL_CHAR("sll:string_checksum"),
		sll_api_string_checksum_raw
	},
	{
		SLL_CHAR("sll:string_convert"),
		sll_api_string_convert_raw
	},
	{
		SLL_CHAR("sll:string_count"),
		sll_api_string_count_raw
	},
	{
		SLL_CHAR("sll:string_count_left"),
		sll_api_string_count_left_raw
	},
	{
		SLL_CHAR("sll:string_count_right"),
		sll_api_string_count_right_raw
	},
	{
		SLL_CHAR("sll:string_ends"),
		sll_api_string_ends_raw
	},
	{
		SLL_CHAR("sll:string_flip_case"),
		sll_api_string_flip_case_raw
	},
	{
		SLL_CHAR("sll:string_format"),
		sll_api_string_format_raw
	},
	{
		SLL_CHAR("sll:string_index"),
		sll_api_string_index_raw
	},
	{
		SLL_CHAR("sll:string_index_list"),
		sll_api_string_index_list_raw
	},
	{
		SLL_CHAR("sll:string_index_reverse"),
		sll_api_string_index_reverse_raw
	},
	{
		SLL_CHAR("sll:string_index_reverse_list"),
		sll_api_string_index_reverse_list_raw
	},
	{
		SLL_CHAR("sll:string_join"),
		sll_api_string_join_raw
	},
	{
		SLL_CHAR("sll:string_pad"),
		sll_api_string_pad_raw
	},
	{
		SLL_CHAR("sll:string_pad_left"),
		sll_api_string_pad_left_raw
	},
	{
		SLL_CHAR("sll:string_pad_right"),
		sll_api_string_pad_right_raw
	},
	{
		SLL_CHAR("sll:string_replace"),
		sll_api_string_replace_raw
	},
	{
		SLL_CHAR("sll:string_reverse"),
		sll_api_string_reverse_raw
	},
	{
		SLL_CHAR("sll:string_secure_equal"),
		sll_api_string_secure_equal_raw
	},
	{
		SLL_CHAR("sll:string_split"),
		sll_api_string_split_raw
	},
	{
		SLL_CHAR("sll:string_starts"),
		sll_api_string_starts_raw
	},
	{
		SLL_CHAR("sll:string_to_lower_case"),
		sll_api_string_to_lower_case_raw
	},
	{
		SLL_CHAR("sll:string_to_title_case"),
		sll_api_string_to_title_case_raw
	},
	{
		SLL_CHAR("sll:string_to_upper_case"),
		sll_api_string_to_upper_case_raw
	},
	{
		SLL_CHAR("sll:string_trim"),
		sll_api_string_trim_raw
	},
	{
		SLL_CHAR("sll:string_trim_left"),
		sll_api_string_trim_left_raw
	},
	{
		SLL_CHAR("sll:string_trim_right"),
		sll_api_string_trim_right_raw
	},
	{
		SLL_CHAR("sll:struct_double_from_bits"),
		sll_api_struct_double_from_bits_raw
	},
	{
		SLL_CHAR("sll:struct_double_to_bits"),
		sll_api_struct_double_to_bits_raw
	},
	{
		SLL_CHAR("sll:struct_float_from_bits"),
		sll_api_struct_float_from_bits_raw
	},
	{
		SLL_CHAR("sll:struct_float_to_bits"),
		sll_api_struct_float_to_bits_raw
	},
	{
		SLL_CHAR("sll:sys_get_args"),
		sll_api_sys_get_args_raw
	},
	{
		SLL_CHAR("sll:sys_get_cpu_count"),
		sll_api_sys_get_cpu_count_raw
	},
	{
		SLL_CHAR("sll:sys_get_env"),
		sll_api_sys_get_env_raw
	},
	{
		SLL_CHAR("sll:sys_get_executable"),
		sll_api_sys_get_executable_raw
	},
	{
		SLL_CHAR("sll:sys_get_library"),
		sll_api_sys_get_library_raw
	},
	{
		SLL_CHAR("sll:sys_get_platform"),
		sll_api_sys_get_platform_raw
	},
	{
		SLL_CHAR("sll:sys_get_sandbox_flags"),
		sll_api_sys_get_sandbox_flags_raw
	},
	{
		SLL_CHAR("sll:sys_get_version"),
		sll_api_sys_get_version_raw
	},
	{
		SLL_CHAR("sll:sys_load_library"),
		sll_api_sys_load_library_raw
	},
	{
		SLL_CHAR("sll:sys_remove_env"),
		sll_api_sys_remove_env_raw
	},
	{
		SLL_CHAR("sll:sys_set_env"),
		sll_api_sys_set_env_raw
	},
	{
		SLL_CHAR("sll:sys_set_sandbox_flag"),
		sll_api_sys_set_sandbox_flag_raw
	},
	{
		SLL_CHAR("sll:thread_create"),
		sll_api_thread_create_raw
	},
	{
		SLL_CHAR("sll:thread_create_barrier"),
		sll_api_thread_create_barrier_raw
	},
	{
		SLL_CHAR("sll:thread_create_lock"),
		sll_api_thread_create_lock_raw
	},
	{
		SLL_CHAR("sll:thread_create_semaphore"),
		sll_api_thread_create_semaphore_raw
	},
	{
		SLL_CHAR("sll:thread_get_internal_data"),
		sll_api_thread_get_internal_data_raw
	},
	{
		SLL_CHAR("sll:thread_increase_barrier"),
		sll_api_thread_increase_barrier_raw
	},
	{
		SLL_CHAR("sll:thread_release_lock"),
		sll_api_thread_release_lock_raw
	},
	{
		SLL_CHAR("sll:thread_release_semaphore"),
		sll_api_thread_release_semaphore_raw
	},
	{
		SLL_CHAR("sll:thread_reset_barrier"),
		sll_api_thread_reset_barrier_raw
	},
	{
		SLL_CHAR("sll:thread_restart"),
		sll_api_thread_restart_raw
	},
	{
		SLL_CHAR("sll:thread_suspend"),
		sll_api_thread_suspend_raw
	},
	{
		SLL_CHAR("sll:time_current"),
		sll_api_time_current_raw
	},
	{
		SLL_CHAR("sll:time_current_ns"),
		sll_api_time_current_ns_raw
	},
	{
		SLL_CHAR("sll:time_sleep"),
		sll_api_time_sleep_raw
	},
	{
		SLL_CHAR("sll:time_sleep_ns"),
		sll_api_time_sleep_ns_raw
	},
	{
		SLL_CHAR("sll:vm_get_config"),
		sll_api_vm_get_config_raw
	},
	{
		SLL_CHAR("sll:vm_get_instruction_count"),
		sll_api_vm_get_instruction_count_raw
	},
	{
		SLL_CHAR("sll:vm_get_instruction_index"),
		sll_api_vm_get_instruction_index_raw
	},
	{
		SLL_CHAR("sll:vm_get_location"),
		sll_api_vm_get_location_raw
	},
	{
		SLL_CHAR("sll:vm_get_ref_count"),
		sll_api_vm_get_ref_count_raw
	}
};



const sll_function_index_t _ifunc_size=181;
const internal_function_t* _ifunc_data=(const internal_function_t*)(&_ifunc_data_ptr);
