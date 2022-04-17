#ifndef __SLL_API_OBJECT_H__
#define __SLL_API_OBJECT_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Object API
 * \group object-api
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_api_object_new
 * \group object-api
 * \desc Docs!
 * \api so!|o
 * \arg sll_string_t* a
 * \arg sll_object_t*const* b
 * \arg sll_arg_count_t bc
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_object_new(sll_string_t* a,sll_object_t*const* b,sll_arg_count_t bc);



#endif
