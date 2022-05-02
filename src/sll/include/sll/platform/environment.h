#ifndef __SLL_PLATFORM_ENVIRONMENT_H__
#define __SLL_PLATFORM_ENVIRONMENT_H__ 1
#include <sll/common.h>
#include <sll/environment.h>
#include <sll/error.h>
#include <sll/platform/thread.h>
#include <sll/string.h>
#include <sll/types.h>
/**
 * \flags subgroup
 * \name Environment
 * \group platform
 * \subgroup platform-env
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_platform_remove_environment_variable
 * \group platform
 * \subgroup platform-env
 * \desc Docs!
 * \arg const sll_char_t* k
 */
__SLL_EXTERNAL void sll_platform_remove_environment_variable(const sll_char_t* k);



/**
 * \flags func
 * \name sll_platform_set_environment_variable
 * \group platform
 * \subgroup platform-env
 * \desc Docs!
 * \arg const sll_char_t* k
 * \arg const sll_char_t* v
 */
__SLL_EXTERNAL void sll_platform_set_environment_variable(const sll_char_t* k,const sll_char_t* v);



#endif
