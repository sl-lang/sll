#ifndef __SLL_ENV_H__
#define __SLL_ENV_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Environment Variables
 * \group env
 * \desc Docs!
 */



/**
 * \flags check_output func
 * \name sll_get_environment_variable
 * \group env
 * \desc Docs!
 * \arg const sll_string_t* k
 * \arg sll_string_t* o
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_get_environment_variable(const sll_string_t* k,sll_string_t* o);



/**
 * \flags func
 * \name sll_remove_environment_variable
 * \group env
 * \desc Docs!
 * \arg const sll_string_t* k
 */
__SLL_EXTERNAL void sll_remove_environment_variable(const sll_string_t* k);



/**
 * \flags func
 * \name sll_set_environment_variable
 * \group env
 * \desc Docs!
 * \arg const sll_string_t* k
 * \arg const sll_string_t* v
 */
__SLL_EXTERNAL void sll_set_environment_variable(const sll_string_t* k,const sll_string_t* v);



#endif
