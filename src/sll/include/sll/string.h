#ifndef __SLL_STRING_H__
#define __SLL_STRING_H__ 1
#include <sll/common.h>
#include <sll/types.h>
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
 * \flags func macro
 * \name SLL_CHAR
 * \group string
 * \desc Docs!
 * \arg char* x
 * \ret sll_char_t*
 */
#define SLL_CHAR(x) ((sll_char_t*)(x))



/**
 * \flags func macro
 * \name SLL_INIT_STRING
 * \group string
 * \desc Docs!
 * \arg sll_string_t* o
 */
#define SLL_INIT_STRING(o) \
	do{ \
		(o)->l=0; \
		(o)->c=0; \
		(o)->v=NULL; \
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
 * \arg sll_string_length_t l
 * \ret sll_string_length_t
 */
#define SLL_STRING_ALIGN_LENGTH(l) (((l)+16)&0xfffffffffffffff0ull)



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
#define SLL_STRING_COMBINE_CHECKSUMS(a,l,b) (((sll_string_checksum_t)(a))^((((sll_string_checksum_t)(b))<<(((l)&3)<<3))|(((sll_string_checksum_t)(b))>>(32-(((l)&3)<<3)))))



/**
 * \flags func macro
 * \name SLL_STRING_ESCAPE
 * \group string
 * \desc Docs!
 * \arg sll_char_t c
 * \ret sll_bool_t
 */
#define SLL_STRING_ESCAPE(c) ((c)=='\t'||(c)=='\n'||(c)=='\v'||(c)=='\f'||(c)=='\r'||(c)=='\"'||(c)=='\''||(c)=='\\')



/**
 * \flags func macro
 * \name SLL_STRING_FORMAT_PADDING
 * \group string
 * \desc Docs!
 * \arg sll_char_t* v
 * \arg sll_string_length_t l
 */
#define SLL_STRING_FORMAT_PADDING(v,l) \
	do{ \
		if (((l)&15)<8){ \
			(*((__SLL_U64*)((v)+((l)&0xfffffffffffffff0ull))))&=(1ull<<(((l)&15)<<3))-1; \
		} \
		(*((__SLL_U64*)((v)+((l)&0xfffffffffffffff0ull)+8)))&=(1ull<<((((l)&15)-8)<<3))-1; \
	} while (0)



/**
 * \flags func macro
 * \name SLL_STRING_HEX_ESCAPE
 * \group string
 * \desc Docs!
 * \arg sll_char_t c
 * \ret sll_bool_t
 */
#define SLL_STRING_HEX_ESCAPE(c) ((c)<32||(c)>126)



/**
 * \flags macro var
 * \name SLL_MAX_OBJECT_TYPE
 * \group object
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_STRING_INSERT_POINTER_STATIC(nm,i,s) sll_string_insert_pointer_length(SLL_CHAR(nm),sizeof(nm)/sizeof(char)-1,(i),(s))



/**
 * \flags func
 * \name sll_free_string
 * \group string
 * \desc Docs!
 * \arg sll_string_t* s
 */
__SLL_EXTERNAL void sll_free_string(sll_string_t* s);



/**
 * \flags func
 * \name sll_string_and
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_and(const sll_string_t* a,const sll_string_t* b,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_and_char
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_char_t v
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_and_char(const sll_string_t* s,sll_char_t v,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_calculate_checksum
 * \group string
 * \desc Docs!
 * \arg sll_string_t* s
 */
__SLL_EXTERNAL void sll_string_calculate_checksum(sll_string_t* s);



/**
 * \flags func
 * \name sll_string_clone
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_string_t* d
 */
__SLL_EXTERNAL void sll_string_clone(const sll_string_t* s,sll_string_t* d);



/**
 * \flags func
 * \name sll_string_combinations
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \arg sll_array_t* o
 */
__SLL_EXTERNAL void sll_string_combinations(const sll_string_t* a,const sll_string_t* b,sll_array_t* o);



/**
 * \flags check_output func
 * \name sll_string_combine_checksums
 * \group string
 * \desc Docs!
 * \arg sll_string_checksum_t a
 * \arg sll_string_length_t l
 * \arg sll_string_checksum_t b
 * \ret sll_string_checksum_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_checksum_t sll_string_combine_checksums(sll_string_checksum_t a,sll_string_length_t l,sll_string_checksum_t b);



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
 * \arg const sll_string_t* s
 * \arg const sll_array_t* a
 * \arg sll_bool_t inv
 * \ret sll_compare_result_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_string_compare_array(const sll_string_t* s,const sll_array_t* a,sll_bool_t inv);



/**
 * \flags check_output func
 * \name sll_string_compare_map
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg const sll_map_t* m
 * \arg sll_bool_t inv
 * \ret sll_compare_result_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_string_compare_map(const sll_string_t* s,const sll_map_t* m,sll_bool_t inv);



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
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_concat(const sll_string_t* a,const sll_string_t* b,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_concat_char
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_char_t c
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_concat_char(const sll_string_t* s,sll_char_t c,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_concat_chars
 * \group string
 * \desc Docs!
 * \arg sll_char_t a
 * \arg sll_char_t b
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_concat_chars(sll_char_t a,sll_char_t b,sll_string_t* o);



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
 * \arg const sll_string_t* s
 * \arg sll_char_t c
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count_char(const sll_string_t* s,sll_char_t c);



/**
 * \flags check_output func
 * \name sll_string_count_left
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_char_t c
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count_left(const sll_string_t* s,sll_char_t c);



/**
 * \flags check_output func
 * \name sll_string_count_right
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_char_t c
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count_right(const sll_string_t* s,sll_char_t c);



/**
 * \flags func
 * \name sll_string_create
 * \group string
 * \desc Docs!
 * \arg sll_string_length_t l
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_create(sll_string_length_t l,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_decrease
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg sll_string_t* s
 * \arg sll_string_length_t l
 */
__SLL_EXTERNAL void sll_string_decrease(sll_string_t* s,sll_string_length_t l);



/**
 * \flags func
 * \name sll_string_duplicate
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_integer_t v
 * \arg sll_string_length_t e
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_duplicate(const sll_string_t* s,sll_integer_t v,sll_string_length_t e,sll_string_t* o);



/**
 * \flags check_output func
 * \name sll_string_ends
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_ends(const sll_string_t* a,const sll_string_t* b);



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
 * \arg const sll_string_t* s
 * \arg const sll_array_t* a
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_equal_array(const sll_string_t* s,const sll_array_t* a);



/**
 * \flags check_output func
 * \name sll_string_equal_map
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg const sll_map_t* m
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_equal_map(const sll_string_t* s,const sll_map_t* m);



/**
 * \flags func
 * \name sll_string_flip_case
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_flip_case(const sll_string_t* s,sll_string_t* o);



/**
 * \flags func var_arg
 * \name sll_string_format
 * \group string
 * \desc Docs!
 * \arg const sll_char_t* t
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_format(const sll_char_t* t,sll_string_t* o,...);



/**
 * \flags func
 * \name sll_string_format_list
 * \group string
 * \desc Docs!
 * \arg const sll_char_t* t
 * \arg sll_string_length_t l
 * \arg sll_var_arg_list_t* va
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_format_list(const sll_char_t* t,sll_string_length_t l,sll_var_arg_list_t* va,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_from_char
 * \group string
 * \desc Docs!
 * \arg sll_char_t c
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_from_char(sll_char_t c,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_from_data
 * \group string
 * \desc Docs!
 * \arg sll_object_t** v
 * \arg sll_string_length_t vl
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_from_data(sll_object_t** v,sll_string_length_t vl,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_from_int
 * \group string
 * \desc Docs!
 * \arg sll_integer_t v
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_from_int(sll_integer_t v,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_from_pointer
 * \group string
 * \desc Docs!
 * \arg const sll_char_t* s
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_from_pointer(const sll_char_t* s,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_from_pointer_length
 * \group string
 * \desc Docs!
 * \arg const sll_char_t* s
 * \arg sll_string_length_t l
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_from_pointer_length(const sll_char_t* s,sll_string_length_t l,sll_string_t* o);



/**
 * \flags check_output func
 * \name sll_string_get
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_string_length_t i
 * \ret sll_char_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_char_t sll_string_get(const sll_string_t* s,sll_string_length_t i);



/**
 * \flags check_output func
 * \name sll_string_includes
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_includes(const sll_string_t* a,const sll_string_t* b);



/**
 * \flags check_output func
 * \name sll_string_includes_char
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_char_t c
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_includes_char(const sll_string_t* s,sll_char_t c);



/**
 * \flags func
 * \name sll_string_increase
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg sll_string_t* s
 * \arg sll_string_length_t l
 */
__SLL_EXTERNAL void sll_string_increase(sll_string_t* s,sll_string_length_t l);



/**
 * \flags check_output func
 * \name sll_string_index
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index(const sll_string_t* a,const sll_string_t* b);



/**
 * \flags check_output func
 * \name sll_string_index_char
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_char_t c
 * \arg sll_bool_t inv
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_char(const sll_string_t* s,sll_char_t c,sll_bool_t inv);



/**
 * \flags check_output func
 * \name sll_string_index_multiple
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg const sll_char_t* cl
 * \arg sll_string_length_t cll
 * \arg sll_bool_t inv
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_multiple(const sll_string_t* s,const sll_char_t* cl,sll_string_length_t cll,sll_bool_t inv);



/**
 * \flags check_output func
 * \name sll_string_index_reverse
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_reverse(const sll_string_t* a,const sll_string_t* b);



/**
 * \flags check_output func
 * \name sll_string_index_reverse_char
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_char_t c
 * \arg sll_bool_t inv
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_reverse_char(const sll_string_t* s,sll_char_t c,sll_bool_t inv);



/**
 * \flags check_output func
 * \name sll_string_index_reverse_multiple
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg const sll_char_t* cl
 * \arg sll_string_length_t cll
 * \arg sll_bool_t inv
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_reverse_multiple(const sll_string_t* s,const sll_char_t* cl,sll_string_length_t cll,sll_bool_t inv);



/**
 * \flags func
 * \name sll_string_insert_pointer
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_char_t* s
 * \arg sll_string_length_t i
 * \arg sll_string_t* o
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL sll_string_length_t sll_string_insert_pointer(const sll_char_t* s,sll_string_length_t i,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_insert_pointer_length
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_char_t* s
 * \arg sll_string_length_t l
 * \arg sll_string_length_t i
 * \arg sll_string_t* o
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL sll_string_length_t sll_string_insert_pointer_length(const sll_char_t* s,sll_string_length_t l,sll_string_length_t i,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_inv
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_inv(const sll_string_t* s,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_join
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_object_t*const* a
 * \arg sll_array_length_t al
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_join(const sll_string_t* s,sll_object_t*const* a,sll_array_length_t al,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_join_char
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg sll_char_t c
 * \arg sll_object_t*const* a
 * \arg sll_array_length_t al
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_join_char(sll_char_t c,sll_object_t*const* a,sll_array_length_t al,sll_string_t* o);



/**
 * \flags check_output func
 * \name sll_string_length
 * \group string
 * \desc Docs!
 * \arg const sll_char_t* s
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_length(const sll_char_t* s);



/**
 * \flags check_output func
 * \name sll_string_length_unaligned
 * \group string
 * \desc Docs!
 * \arg const sll_char_t* s
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_length_unaligned(const sll_char_t* s);



/**
 * \flags func
 * \name sll_string_lower_case
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_lower_case(const sll_string_t* s,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_op
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \arg sll_binary_operator_t f
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_op(const sll_string_t* a,const sll_string_t* b,sll_binary_operator_t f,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_op_array
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg const sll_array_t* a
 * \arg sll_binary_operator_t f
 * \arg sll_bool_t inv
 * \arg sll_array_t* o
 */
__SLL_EXTERNAL void sll_string_op_array(const sll_string_t* s,const sll_array_t* a,sll_binary_operator_t f,sll_bool_t inv,sll_array_t* o);



/**
 * \flags func
 * \name sll_string_op_map
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg const sll_map_t* m
 * \arg sll_binary_operator_t f
 * \arg sll_bool_t inv
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_string_op_map(const sll_string_t* s,const sll_map_t* m,sll_binary_operator_t f,sll_bool_t inv,sll_map_t* o);



/**
 * \flags func
 * \name sll_string_or
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_or(const sll_string_t* a,const sll_string_t* b,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_or_char
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_char_t v
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_or_char(const sll_string_t* s,sll_char_t v,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_pad
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_string_length_t l
 * \arg sll_char_t v
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_pad(const sll_string_t* s,sll_string_length_t l,sll_char_t c,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_pad_left
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_string_length_t l
 * \arg sll_char_t v
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_pad_left(const sll_string_t* s,sll_string_length_t l,sll_char_t c,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_pad_right
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_string_length_t l
 * \arg sll_char_t v
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_pad_right(const sll_string_t* s,sll_string_length_t l,sll_char_t c,sll_string_t* o);



/**
 * \flags check_output func
 * \name sll_string_parse_char
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* s
 * \ret sll_char_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_char_t sll_string_parse_char(const sll_string_t* s);



/**
 * \flags check_output func
 * \name sll_string_parse_float
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* s
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_string_parse_float(const sll_string_t* s);



/**
 * \flags check_output func
 * \name sll_string_parse_int
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* s
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_string_parse_int(const sll_string_t* s);



/**
 * \flags func
 * \name sll_string_prepend_char
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_char_t c
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_prepend_char(const sll_string_t* s,sll_char_t c,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_remove
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_remove(const sll_string_t* a,const sll_string_t* b,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_replace
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg const sll_string_t* k
 * \arg const sll_string_t* v
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_replace(const sll_string_t* s,const sll_string_t* k,const sll_string_t* v,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_replace_char
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_char_t k
 * \arg sll_char_t v
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_replace_char(const sll_string_t* s,sll_char_t k,sll_char_t v,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_reverse
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_reverse(const sll_string_t* s,sll_string_t* o);



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
__SLL_EXTERNAL sll_bool_t sll_string_secure_equal(const sll_string_t* a,const sll_string_t* b);



/**
 * \flags func
 * \name sll_string_select
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_integer_t a
 * \arg sll_integer_t b
 * \arg sll_integer_t c
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_select(const sll_string_t* s,sll_integer_t a,sll_integer_t b,sll_integer_t c,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_set_char
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg sll_char_t c
 * \arg sll_string_length_t i
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_set_char(sll_char_t c,sll_string_length_t i,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_shift
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_integer_t v
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_shift(const sll_string_t* s,sll_integer_t v,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_split
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg const sll_string_t* p
 * \arg sll_array_t* o
 */
__SLL_EXTERNAL void sll_string_split(const sll_string_t* s,const sll_string_t* p,sll_array_t* o);



/**
 * \flags func
 * \name sll_string_split_char
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_char_t c
 * \arg sll_array_t* o
 */
__SLL_EXTERNAL void sll_string_split_char(const sll_string_t* s,sll_char_t c,sll_array_t* o);



/**
 * \flags check_output func
 * \name sll_string_starts
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_starts(const sll_string_t* a,const sll_string_t* b);



/**
 * \flags func
 * \name sll_string_title_case
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_title_case(const sll_string_t* s,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_to_array
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_array_t* o
 */
__SLL_EXTERNAL void sll_string_to_array(const sll_string_t* s,sll_array_t* o);



/**
 * \flags func
 * \name sll_string_to_map
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_string_to_map(const sll_string_t* s,sll_map_t* o);



/**
 * \flags func
 * \name sll_string_trim
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_trim(const sll_string_t* s,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_trim_left
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_trim_left(const sll_string_t* s,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_trim_right
 * \group string
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_trim_right(const sll_string_t* s,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_upper_case
 * \group string
 * \subgroup string-data
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_upper_case(const sll_string_t* s,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_xor
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_xor(const sll_string_t* a,const sll_string_t* b,sll_string_t* o);



/**
 * \flags func
 * \name sll_string_xor_char
 * \group string
 * \subgroup string-op
 * \desc Docs!
 * \arg const sll_string_t* s
 * \arg sll_char_t v
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_string_xor_char(const sll_string_t* s,sll_char_t v,sll_string_t* o);



#endif
