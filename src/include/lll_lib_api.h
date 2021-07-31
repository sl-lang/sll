#ifndef __LLL_liB_API_H__
#define __LLL_liB_API_H__ 1
#include <lll_lib.h>
#include <stdint.h>



#define __LLL_RETURN_COMPARE __LLL_CHECK_OUTPUT lll_compare_result_t



#define LLL_COMPARE_RESULT_BELOW 0
#define LLL_COMPARE_RESULT_EQUAL 1
#define LLL_COMPARE_RESULT_ABOVE 2
#define LLL_COMPARE_RESULT_ZERO 3
#define LLL_COMPARE_RESULT_NONZERO 4
#define LLL_COMPARE_RESULT_ERROR 255



typedef uint8_t lll_compare_result_t;



void lll_api_get_time(lll_runtime_object_t* o,lll_arg_count_t ac,lll_runtime_object_t* a);



__LLL_IMPORT_EXPORT __LLL_RETURN_COMPARE lll_compare_runtime_object(const lll_runtime_object_t* a,const lll_runtime_object_t* b);



__LLL_IMPORT_EXPORT __LLL_RETURN_COMPARE lll_runtime_object_nonzero(const lll_runtime_object_t* o);



#endif
