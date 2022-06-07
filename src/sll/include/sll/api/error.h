#ifndef __SLL_API_ERROR_H__
#define __SLL_API_ERROR_H__ 1
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/types.h>
#include <sll/vm.h>
/**
 * \flags group
 * \name Error API
 * \group error-api
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_api_error_get_call_stack
 * \group error-api
 * \desc Docs!
 * \api DD|a
 * \arg sll_call_stack_size_t pop
 * \arg sll_thread_index_t thread_index
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_error_get_call_stack(sll_call_stack_size_t pop,sll_thread_index_t thread_index,sll_array_t* out);



/**
 * \flags func
 * \name sll_api_error_get_error_string
 * \group error-api
 * \desc Docs!
 * \api Q|s
 * \arg sll_error_t err
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_error_get_error_string(sll_error_t err,sll_string_t* out);



#endif
