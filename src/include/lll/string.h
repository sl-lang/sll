#ifndef __LLL_STRING_H__
#define __LLL_STRING_H__ 1
#include <lll/common.h>
#include <lll/types.h>



__LLL_FUNC __LLL_RETURN_COMPARE lll_string_compare(const lll_string_t* a,const lll_string_t* b);



__LLL_FUNC __LLL_RETURN_COMPARE lll_string_compare_ascii(const lll_string_t* a,const char* b);



__LLL_FUNC __LLL_RETURN_COMPARE lll_string_compare_ignore_case(const lll_string_t* a,const lll_string_t* b);



__LLL_FUNC __LLL_RETURN_COMPARE lll_string_compare_ignore_case_ascii(const lll_string_t* a,const char* b);



__LLL_FUNC lll_string_length_t lll_string_to_ascii(const lll_string_t* s,char* d);



__LLL_FUNC lll_string_length_t lll_string_length_as_bytes(const lll_string_t* s);



__LLL_FUNC lll_string_length_t lll_string_to_bytes(const lll_string_t* s,lll_buffer_t d);



__LLL_FUNC void lll_string_to_stream(const lll_string_t* s,lll_output_data_stream_t* os);



__LLL_FUNC void lll_string_encode_char_to_stream(lll_char_t c,lll_output_data_stream_t* os);



#endif
