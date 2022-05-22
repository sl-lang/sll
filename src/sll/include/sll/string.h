#ifndef __SLL_STRING_H__
#define __SLL_STRING_H__ 1
#include <sll/_string_types.h>
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/gc.h>
#include <sll/operator.h>
#include <sll/types.h>
#include <sll/var_arg.h>
/**
 * \flags group
 * \name String
 * \group string
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Content
 * \group string
 * \subgroup string-data
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Operations
 * \group string
 * \subgroup string-op
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_MAX_CHAR
 * \group string
 * \desc Docs!
 * \type sll_char_t
 */
#define SLL_MAX_CHAR __SLL_U8_MAX



/**
 * \flags macro var
 * \name SLL_MAX_STRING_LENGTH
 * \group string
 * \desc Docs!
 * \type sll_string_length_t
 */
#define SLL_MAX_STRING_LENGTH __SLL_U32_MAX



/**
 * \flags func macro
 * \name SLL_CHAR
 * \group string
 * \desc Docs!
 * \arg char* pointer
 * \ret sll_char_t*
 */
#define SLL_CHAR(pointer) ((sll_char_t*)(pointer))



/**
 * \flags func macro
 * \name SLL_INIT_STRING
 * \group string
 * \desc Docs!
 * \arg sll_string_t* outut
 */
#define SLL_INIT_STRING(out) \
	do{ \
		(out)->length=0; \
		(out)->checksum=0; \
		(out)->data=NULL; \
	} while (0)



/**
 * \flags macro var
 * \name SLL_INIT_STRING_STRUCT
 * \group string
 * \desc Docs!
 * \type sll_string_t
 */
#define SLL_INIT_STRING_STRUCT {0,0,NULL}



/**
 * \flags func macro
 * \name SLL_STRING_ALIGN_LENGTH
 * \group string
 * \desc Docs!
 * \arg sll_string_length_t length
 * \ret sll_string_length_t
 */
#define SLL_STRING_ALIGN_LENGTH(length) (((length)+8)&0xfffffffffffffff8ull)



/**
 * \flags func macro
 * \name SLL_STRING_COMBINE_CHECKSUMS
 * \group string
 * \desc Docs!
 * \arg sll_string_checksum_t a
 * \arg sll_string_length_t l
 * \arg sll_string_checksum_t b
 * \ret sll_string_checksum_t
 */
#define SLL_STRING_COMBINE_CHECKSUMS(a,length,b) (((sll_string_checksum_t)(a))^((((sll_string_checksum_t)(b))<<(((length)&3)<<3))|(((sll_string_checksum_t)(b))>>(32-(((length)&3)<<3)))))



/**
 * \flags func macro
 * \name SLL_STRING_ESCAPE
 * \group string
 * \desc Docs!
 * \arg sll_char_t char_
 * \ret sll_bool_t
 */
#define SLL_STRING_ESCAPE(char_) ((char_)=='\t'||(char_)=='\n'||(char_)=='\v'||(char_)=='\f'||(char_)=='\r'||(char_)=='\"'||(char_)=='\''||(char_)=='\\')



/**
 * \flags func macro
 * \name SLL_STRING_FORMAT_PADDING
 * \group string
 * \desc Docs!
 * \arg sll_char_t* pointer
 * \arg sll_string_length_t length
 */
#define SLL_STRING_FORMAT_PADDING(pointer,length) \
	do{ \
		(*((__SLL_U64*)((pointer)+((length)&0xfffffffffffffff8ull))))&=(1ull<<(((length)&7)<<3))-1; \
	} while (0)



/**
 * \flags func macro
 * \name SLL_STRING_HEX_ESCAPE
 * \group string
 * \desc Docs!
 * \arg sll_char_t char_
 * \ret sll_bool_t
 */
#define SLL_STRING_HEX_ESCAPE(char_) ((char_)<32||(char_)>126)



/**
 * \flags func macro
 * \name SLL_STRING_INSERT_POINTER_STATIC
 * \group string
 * \desc Docs!
 * \arg const sll_char_t* static_string
 * \arg sll_string_length_t index
 * \arg sll_string_t* string
 * \ret sll_string_length_t
 */
#define SLL_STRING_INSERT_POINTER_STATIC(static_string,index,string) sll_string_insert_pointer_length(SLL_CHAR(static_string),sizeof(static_string)/sizeof(char)-1,(index),(string))



/**
 * \flags macro var
 * \name SLL_INVALID_CHARACTER
 * \group string
 * \desc Docs!
 * \type sll_wide_char_t
 */
#define SLL_INVALID_CHARACTER 0xffffffff



/**
 * \flags func
 * \name sll_free_string
 * \group string
 * \desc Docs!
 * \arg sll_string_t* sstring
 */
__SLL_EXTERNAL void sll_free_string(sll_string_t* string);



/**
 * \flags func
 * \name sll_string_and
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_and(const sll_string_t* a,const sll_string_t* b,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_and_char
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_char_t char_
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_and_char(const sll_string_t* string,sll_char_t char_,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_calculate_checksum
 * \group string
 * \desc Docs!
 * \arg sll_string_t* string
 */
__SLL_EXTERNAL void sll_string_calculate_checksum(sll_string_t* string);



/**
 * \flags func
 * \name sll_string_clone
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_clone(const sll_string_t* string,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_combinations
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_string_combinations(const sll_string_t* a,const sll_string_t* b,sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_string_combine_checksums
 * \group string
 * \desc Docs!
 * \arg sll_string_checksum_t a
 * \arg sll_string_length_t length
 * \arg sll_string_checksum_t b
 * \ret sll_string_checksum_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_checksum_t sll_string_combine_checksums(sll_string_checksum_t a,sll_string_length_t length,sll_string_checksum_t b);



/**
 * \flags check_output func
 * \name sll_string_compare
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \ret sll_compare_result_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_string_compare(const sll_string_t* a,const sll_string_t* b);



/**
 * \flags check_output func
 * \name sll_string_compare_array
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg const sll_array_t* array
 * \arg sll_bool_t inv
 * \ret sll_compare_result_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_string_compare_array(const sll_string_t* string,const sll_array_t* array,sll_bool_t inv);



/**
 * \flags check_output func
 * \name sll_string_compare_map
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg const sll_map_t* map
 * \arg sll_bool_t inv
 * \ret sll_compare_result_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_string_compare_map(const sll_string_t* string,const sll_map_t* map,sll_bool_t inv);



/**
 * \flags check_output func
 * \name sll_string_compare_pointer
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_char_t* a
 * \arg const sll_char_t* b
 * \ret sll_compare_result_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_string_compare_pointer(const sll_char_t* a,const sll_char_t* b);



/**
 * \flags func
 * \name sll_string_concat
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_concat(const sll_string_t* a,const sll_string_t* b,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_concat_char
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_char_t char_
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_concat_char(const sll_string_t* string,sll_char_t char_,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_concat_chars
 * \group string
 * \desc Docs!
 * \arg sll_char_t a
 * \arg sll_char_t b
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_concat_chars(sll_char_t a,sll_char_t b,sll_string_t* out);



/**
 * \flags check_output func
 * \name sll_string_count
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count(const sll_string_t* a,const sll_string_t* b);



/**
 * \flags check_output func
 * \name sll_string_count_char
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_char_t char_
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count_char(const sll_string_t* string,sll_char_t char_);



/**
 * \flags check_output func
 * \name sll_string_count_left
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_char_t char_
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count_left(const sll_string_t* string,sll_char_t char_);



/**
 * \flags check_output func
 * \name sll_string_count_right
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_char_t char_
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count_right(const sll_string_t* string,sll_char_t char_);



/**
 * \flags func
 * \name sll_string_create
 * \group string
 * \desc Docs!
 * \arg sll_string_length_t length
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_create(sll_string_length_t length,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_decrease
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg sll_string_t* string
 * \arg sll_string_length_t length
 */
__SLL_EXTERNAL void sll_string_decrease(sll_string_t* string,sll_string_length_t length);



/**
 * \flags func
 * \name sll_string_duplicate
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_integer_t count
 * \arg sll_string_length_t extra
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_duplicate(const sll_string_t* string,sll_integer_t count,sll_string_length_t extra,sll_string_t* out);



/**
 * \flags check_output func
 * \name sll_string_ends
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg const sll_string_t* suffix
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_ends(const sll_string_t* string,const sll_string_t* suffix);



/**
 * \flags check_output func
 * \name sll_string_equal
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_equal(const sll_string_t* a,const sll_string_t* b);



/**
 * \flags check_output func
 * \name sll_string_equal_array
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg const sll_array_t* array
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_equal_array(const sll_string_t* string,const sll_array_t* array);



/**
 * \flags check_output func
 * \name sll_string_equal_map
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg const sll_map_t* map
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_equal_map(const sll_string_t* string,const sll_map_t* map);



/**
 * \flags func
 * \name sll_string_flip_case
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_flip_case(const sll_string_t* string,sll_string_t* out);



/**
 * \flags func var_arg
 * \name sll_string_format
 * \group string
 * \desc Docs!
 * \arg const sll_char_t* format
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_format(const sll_char_t* format,sll_string_t* out,...);



/**
 * \flags func
 * \name sll_string_format_list
 * \group string
 * \desc Docs!
 * \arg const sll_char_t* format
 * \arg sll_string_length_t format_length
 * \arg sll_var_arg_list_t* va
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_format_list(const sll_char_t* format,sll_string_length_t format_length,sll_var_arg_list_t* va,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_from_char
 * \group string
 * \desc Docs!
 * \arg sll_char_t char_
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_from_char(sll_char_t char_,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_from_data
 * \group string
 * \desc Docs!
 * \arg sll_object_t** object_data
 * \arg sll_string_length_t object_count
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_from_data(sll_object_t** object_data,sll_string_length_t object_count,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_from_int
 * \group string
 * \desc Docs!
 * \arg sll_integer_t int_
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_from_int(sll_integer_t int_,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_from_pointer
 * \group string
 * \desc Docs!
 * \arg const sll_char_t* pointer
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_from_pointer(const sll_char_t* pointer,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_from_pointer_length
 * \group string
 * \desc Docs!
 * \arg const sll_char_t* pointer
 * \arg sll_string_length_t length
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_from_pointer_length(const sll_char_t* pointer,sll_string_length_t length,sll_string_t* out);



/**
 * \flags check_output func
 * \name sll_string_get
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_string_length_t index
 * \ret sll_char_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_char_t sll_string_get(const sll_string_t* string,sll_string_length_t index);



/**
 * \flags check_output func
 * \name sll_string_includes
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg const sll_string_t* substring
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_includes(const sll_string_t* string,const sll_string_t* substring);



/**
 * \flags check_output func
 * \name sll_string_includes_char
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_char_t char_
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_includes_char(const sll_string_t* string,sll_char_t char_);



/**
 * \flags func
 * \name sll_string_increase
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg sll_string_t* string
 * \arg sll_string_length_t length
 */
__SLL_EXTERNAL void sll_string_increase(sll_string_t* string,sll_string_length_t length);



/**
 * \flags check_output func
 * \name sll_string_index
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg const sll_string_t* substring
 * \arg sll_string_length_t start_index
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index(const sll_string_t* string,const sll_string_t* substring,sll_string_length_t start_index);



/**
 * \flags check_output func
 * \name sll_string_index_char
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_char_t char_
 * \arg sll_bool_t inv
 * \arg sll_string_length_t start_index
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_char(const sll_string_t* string,sll_char_t char_,sll_bool_t inv,sll_string_length_t start_index);



/**
 * \flags check_output func
 * \name sll_string_index_multiple
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg const sll_char_t* char_data
 * \arg sll_string_length_t char_count
 * \arg sll_bool_t inv
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_multiple(const sll_string_t* string,const sll_char_t* char_data,sll_string_length_t char_count,sll_bool_t inv);



/**
 * \flags check_output func
 * \name sll_string_index_reverse
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg const sll_string_t* substring
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_reverse(const sll_string_t* string,const sll_string_t* substring);



/**
 * \flags check_output func
 * \name sll_string_index_reverse_char
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_char_t char_
 * \arg sll_bool_t inv
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_reverse_char(const sll_string_t* string,sll_char_t char_,sll_bool_t inv);



/**
 * \flags check_output func
 * \name sll_string_index_reverse_multiple
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg const sll_char_t* char_data
 * \arg sll_string_length_t char_count
 * \arg sll_bool_t inv
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_reverse_multiple(const sll_string_t* string,const sll_char_t* char_data,sll_string_length_t char_count,sll_bool_t inv);



/**
 * \flags func
 * \name sll_string_insert_pointer
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_char_t* pointer
 * \arg sll_string_length_t index
 * \arg sll_string_t* out
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL sll_string_length_t sll_string_insert_pointer(const sll_char_t* pointer,sll_string_length_t index,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_insert_pointer_length
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_char_t* pointer
 * \arg sll_string_length_t length
 * \arg sll_string_length_t index
 * \arg sll_string_t* out
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL sll_string_length_t sll_string_insert_pointer_length(const sll_char_t* pointer,sll_string_length_t length,sll_string_length_t index,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_inv
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_inv(const sll_string_t* string,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_join
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_object_t*const* object_data
 * \arg sll_array_length_t object_count
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_join(const sll_string_t* string,sll_object_t*const* object_data,sll_array_length_t object_count,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_join_char
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg sll_char_t char_
 * \arg sll_object_t*const* object_data
 * \arg sll_array_length_t object_count
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_join_char(sll_char_t char_,sll_object_t*const* object_data,sll_array_length_t object_count,sll_string_t* out);



/**
 * \flags check_output func
 * \name sll_string_length
 * \group string
 * \desc Docs!
 * \arg const sll_char_t* pointer
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_length(const sll_char_t* pointer);



/**
 * \flags func
 * \name sll_string_lower_case
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_lower_case(const sll_string_t* string,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_op
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \arg sll_binary_operator_t operator
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_op(const sll_string_t* a,const sll_string_t* b,sll_binary_operator_t operator,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_op_array
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg const sll_array_t* array
 * \arg sll_binary_operator_t operator
 * \arg sll_bool_t inv
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_string_op_array(const sll_string_t* string,const sll_array_t* array,sll_binary_operator_t operator,sll_bool_t inv,sll_array_t* out);



/**
 * \flags func
 * \name sll_string_op_map
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg const sll_map_t* map
 * \arg sll_binary_operator_t operator
 * \arg sll_bool_t inv
 * \arg sll_map_t* out
 */
__SLL_EXTERNAL void sll_string_op_map(const sll_string_t* string,const sll_map_t* map,sll_binary_operator_t operator,sll_bool_t inv,sll_map_t* out);



/**
 * \flags func
 * \name sll_string_or
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_or(const sll_string_t* a,const sll_string_t* b,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_or_char
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_char_t char_
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_or_char(const sll_string_t* string,sll_char_t char_,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_pad
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_string_length_t length
 * \arg sll_char_t char_
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_pad(const sll_string_t* string,sll_string_length_t length,sll_char_t char_,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_pad_left
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_string_length_t length
 * \arg sll_char_t char_
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_pad_left(const sll_string_t* string,sll_string_length_t length,sll_char_t char_,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_pad_right
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_string_length_t length
 * \arg sll_char_t char_
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_pad_right(const sll_string_t* string,sll_string_length_t length,sll_char_t char_,sll_string_t* out);



/**
 * \flags check_output func
 * \name sll_string_parse_char
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* string
 * \ret sll_char_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_char_t sll_string_parse_char(const sll_string_t* string);



/**
 * \flags check_output func
 * \name sll_string_parse_float
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* string
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_string_parse_float(const sll_string_t* string);



/**
 * \flags check_output func
 * \name sll_string_parse_int
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* string
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_string_parse_int(const sll_string_t* string);



/**
 * \flags func
 * \name sll_string_prepend_char
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_char_t char_
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_prepend_char(const sll_string_t* string,sll_char_t char_,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_remove
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg const sll_string_t* substring
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_remove(const sll_string_t* string,const sll_string_t* substring,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_replace
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg const sll_string_t* old
 * \arg const sll_string_t* new
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_replace(const sll_string_t* string,const sll_string_t* old,const sll_string_t* new,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_replace_char
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_char_t old
 * \arg sll_char_t new
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_replace_char(const sll_string_t* string,sll_char_t old,sll_char_t new,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_resize
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_integer_t delta
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_resize(const sll_string_t* string,sll_integer_t delta,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_reverse
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_reverse(const sll_string_t* string,sll_string_t* out);



/**
 * \flags check_output func
 * \name sll_string_secure_equal
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_secure_equal(const sll_string_t* a,const sll_string_t* b);



/**
 * \flags func
 * \name sll_string_select
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_integer_t start
 * \arg sll_integer_t end
 * \arg sll_integer_t step
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_select(const sll_string_t* string,sll_integer_t start,sll_integer_t end,sll_integer_t step,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_set_char
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg sll_string_t* string
 * \arg sll_char_t char_
 * \arg sll_string_length_t index
 */
__SLL_EXTERNAL void sll_string_set_char(sll_string_t* string,sll_char_t char_,sll_string_length_t index);



/**
 * \flags func
 * \name sll_string_shift
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_integer_t delta
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_shift(const sll_string_t* string,sll_integer_t delta,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_split
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg const sll_string_t* substring
 * \arg sll_array_t* o
 */
__SLL_EXTERNAL void sll_string_split(const sll_string_t* string,const sll_string_t* substring,sll_array_t* o);



/**
 * \flags func
 * \name sll_string_split_char
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_char_t char_
 * \arg sll_array_t* o
 */
__SLL_EXTERNAL void sll_string_split_char(const sll_string_t* string,sll_char_t char_,sll_array_t* o);



/**
 * \flags check_output func
 * \name sll_string_starts
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg const sll_string_t* prefix
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_starts(const sll_string_t* string,const sll_string_t* prefix);



/**
 * \flags func
 * \name sll_string_title_case
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_title_case(const sll_string_t* string,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_to_array
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_string_to_array(const sll_string_t* string,sll_array_t* out);



/**
 * \flags func
 * \name sll_string_to_map
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_map_t* out
 */
__SLL_EXTERNAL void sll_string_to_map(const sll_string_t* string,sll_map_t* out);



/**
 * \flags func
 * \name sll_string_trim
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_trim(const sll_string_t* string,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_trim_left
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_trim_left(const sll_string_t* string,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_trim_right
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_trim_right(const sll_string_t* string,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_upper_case
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_upper_case(const sll_string_t* string,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_xor
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_xor(const sll_string_t* a,const sll_string_t* b,sll_string_t* out);



/**
 * \flags func
 * \name sll_string_xor_char
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_char_t char_
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_string_xor_char(const sll_string_t* string,sll_char_t char_,sll_string_t* out);



#endif
