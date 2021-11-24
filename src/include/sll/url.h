#ifndef __SLL_URL_H__
#define __SLL_URL_H__ 1
#include <sll/common.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_free_header_list(sll_header_list_t* hl);



__SLL_EXTERNAL void sll_free_http_response(sll_http_response_t* r);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_code_t sll_url_http_request(const sll_string_t* m,const sll_string_t* h,const sll_string_t* p,const sll_header_list_t* hl,const sll_string_t* dt,sll_http_response_t* o);



#endif
