#ifndef __SLL_STRING_H__
#define __SLL_STRING_H__ 1
#include <sll/common.h>
#include <sll/types.h>
#include <stdlib.h>
#include <string.h>



#define SLL_CHAR(x) ((sll_char_t*)(x))
#define SLL_STRING_ALIGN 8
#define SLL_STRING_ALIGN_LENGTH(l) (((l)+SLL_STRING_ALIGN)&(-SLL_STRING_ALIGN))
#define SLL_STRING_COMBINE_CHECKSUMS(a,l,b) (((sll_string_checksum_t)(a))^sll_rotate_bits((sll_string_checksum_t)(b),((l)&3)<<3))
#define SLL_STRING_ESCAPE(c) ((c)=='\t'||(c)=='\n'||(c)=='\v'||(c)=='\f'||(c)=='\r'||(c)=='\"'||(c)=='\''||(c)=='\\')
#define SLL_STRING_FORMAT_PADDING(v,l) memset((v)+(l),0,SLL_STRING_ALIGN-((l)&(SLL_STRING_ALIGN-1)))
#define SLL_STRING_HEX_ESCAPE(c) ((c)<32||(c)>126)
#define SLL_ZERO_STRING(s) \
	do{ \
		(s)->l=0; \
		(s)->c=0; \
		(s)->v=NULL; \
	} while (0)
#define SLL_ZERO_STRING_STRUCT {0,0,NULL}



__SLL_FUNC void sll_string_and(const sll_string_t* a,const sll_string_t* b,sll_string_t* o);



__SLL_FUNC void sll_string_and_char(const sll_string_t* s,sll_char_t v,sll_string_t* o);



__SLL_FUNC void sll_string_clone(const sll_string_t* s,sll_string_t* d);



__SLL_FUNC void sll_string_combinations(const sll_string_t* a,const sll_string_t* b,sll_array_t* o);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_string_checksum_t sll_string_combine_checksums(sll_string_checksum_t a,sll_string_length_t l,sll_string_checksum_t b);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_compare_result_t sll_string_compare(const sll_string_t* a,const sll_string_t* b);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_compare_result_t sll_string_compare_array(const sll_string_t* s,const sll_array_t* a,sll_bool_t inv);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_compare_result_t sll_string_compare_map(const sll_string_t* s,const sll_map_t* m,sll_bool_t inv);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count(const sll_string_t* a,const sll_string_t* b);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count_char(const sll_string_t* s,sll_char_t c);



__SLL_FUNC void sll_string_create(sll_string_length_t l,sll_string_t* o);



__SLL_FUNC void sll_string_duplicate(const sll_string_t* s,sll_integer_t v,sll_string_length_t e,sll_string_t* o);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_string_equal(const sll_string_t* a,const sll_string_t* b);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_string_equal_array(const sll_string_t* s,const sll_array_t* a);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_string_equal_map(const sll_string_t* s,const sll_map_t* m);



__SLL_FUNC void sll_string_from_data(sll_runtime_object_t** v,sll_string_length_t vl,sll_string_t* o);



__SLL_FUNC void sll_string_from_int(sll_integer_t v,sll_string_t* o);



__SLL_FUNC void sll_string_from_pointer(const sll_char_t* s,sll_string_t* o);



__SLL_FUNC sll_char_t sll_string_get(const sll_string_t* s,sll_string_length_t i);



__SLL_FUNC void sll_string_hash(sll_string_t* s);



__SLL_FUNC void sll_string_increase(sll_string_t* s,sll_string_length_t l);



__SLL_FUNC void sll_string_inv(const sll_string_t* s,sll_string_t* o);



__SLL_FUNC void sll_string_join(const sll_string_t* a,const sll_string_t* b,sll_string_t* o);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_string_length_t sll_string_length(const sll_char_t* s);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_string_length_t sll_string_length_unaligned(const sll_char_t* s);



__SLL_FUNC void sll_string_lower_case(const sll_string_t* s,sll_string_t* o);



__SLL_FUNC void sll_string_op(const sll_string_t* a,const sll_string_t* b,sll_binary_operator_t f,sll_string_t* o);



__SLL_FUNC void sll_string_op_array(const sll_string_t* s,const sll_array_t* a,sll_binary_operator_t f,sll_bool_t inv,sll_array_t* o);



__SLL_FUNC void sll_string_op_map(const sll_string_t* s,const sll_map_t* m,sll_binary_operator_t f,sll_bool_t inv,sll_map_t* o);



__SLL_FUNC void sll_string_or(const sll_string_t* a,const sll_string_t* b,sll_string_t* o);



__SLL_FUNC void sll_string_or_char(const sll_string_t* s,sll_char_t v,sll_string_t* o);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_char_t sll_string_parse_char(const sll_string_t* s);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_float_t sll_string_parse_float(const sll_string_t* s);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_integer_t sll_string_parse_int(const sll_string_t* s);



__SLL_FUNC void sll_string_remove(const sll_string_t* a,const sll_string_t* b,sll_string_t* o);



__SLL_FUNC void sll_string_replace(const sll_string_t* s,const sll_string_t* k,const sll_string_t* v,sll_string_t* o);



__SLL_FUNC void sll_string_replace_char(const sll_string_t* s,sll_char_t k,sll_char_t v,sll_string_t* o);



__SLL_FUNC void sll_string_select(const sll_string_t* s,sll_integer_t a,sll_integer_t b,sll_integer_t c,sll_string_t* o);



__SLL_FUNC void sll_string_set(const sll_string_t* s,sll_string_length_t i,sll_char_t c);



__SLL_FUNC void sll_string_shift(const sll_string_t* s,sll_integer_t v,sll_string_t* o);



__SLL_FUNC void sll_string_split(const sll_string_t* s,const sll_string_t* p,sll_array_t* o);



__SLL_FUNC void sll_string_split_char(const sll_string_t* s,sll_char_t c,sll_array_t* o);



__SLL_FUNC void sll_string_subtract_array(const sll_string_t* s,const sll_array_t* a,sll_array_t* o);



__SLL_FUNC void sll_string_subtract_map(const sll_string_t* s,const sll_map_t* m,sll_map_t* o);



__SLL_FUNC void sll_string_title_case(const sll_string_t* s,sll_string_t* o);



__SLL_FUNC void sll_string_upper_case(const sll_string_t* s,sll_string_t* o);



__SLL_FUNC void sll_string_to_array(const sll_string_t* s,sll_array_t* o);



__SLL_FUNC void sll_string_to_map(const sll_string_t* s,sll_map_t* o);



__SLL_FUNC void sll_string_xor(const sll_string_t* a,const sll_string_t* b,sll_string_t* o);



__SLL_FUNC void sll_string_xor_char(const sll_string_t* s,sll_char_t v,sll_string_t* o);



#endif
