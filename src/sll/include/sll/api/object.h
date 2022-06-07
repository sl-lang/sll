#ifndef __SLL_API_OBJECT_H__
#define __SLL_API_OBJECT_H__ 1
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/string.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Object API
 * \group object-api
 * \desc Docs!
 */



/**
 * \flags check_output func
 * \name sll_api_object_new
 * \group object-api
 * \desc Docs!
 * \api #so!|o
 * \arg const sll_string_t* format
 * \arg sll_object_t*const* args
 * \arg sll_arg_count_t lenngth
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_object_new(const sll_string_t* format,sll_object_t*const* args,sll_arg_count_t lenngth);



#endif
