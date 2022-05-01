#ifndef __SLL_ENVIRONMENT_H__
#define __SLL_ENVIRONMENT_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/search_path.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Environment Variables
 * \group environment
 * \desc Docs!
 */



/**
 * \flags type var
 * \name sll_environment_length_t
 * \group environment
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_environment_length_t;



/**
 * \flags type
 * \name sll_environment_variable_t
 * \group environment
 * \desc Docs!
 * \arg const sll_string_t k
 * \arg const sll_string_t v
 */
typedef struct _SLL_ENVIRONMENT_VARIABLE{
    const sll_string_t k;
    const sll_string_t v;
} sll_environment_variable_t;



/**
 * \flags type
 * \name sll_environment_t
 * \group environment
 * \desc Docs!
 * \arg const sll_environment_variable_t*const* dt
 * \arg const sll_environment_length_t l
 */
typedef struct _SLL_ENVIRONMENT{
    const sll_environment_variable_t*const* dt;
    const sll_environment_length_t l;
} sll_environment_t;



/**
 * \flags var
 * \name sll_env_path
 * \group environment
 * \desc Docs!
 * \type const sll_search_path_t*
 */
__SLL_EXTERNAL extern const sll_search_path_t* sll_env_path;



/**
 * \flags func
 * \name sll_expand_environment_variable
 * \group environment
 * \desc Docs!
 * \arg const sll_string_t* k
 * \arg sll_string_t* o
 * \ret sll_bool_t
 */
__SLL_EXTERNAL sll_bool_t sll_expand_environment_variable(const sll_string_t* k,sll_string_t* o);



/**
 * \flags func
 * \name sll_get_environment_variable
 * \group environment
 * \desc Docs!
 * \arg const sll_string_t* k
 * \arg sll_string_t* o
 * \ret sll_bool_t
 */
__SLL_EXTERNAL sll_bool_t sll_get_environment_variable(const sll_string_t* k,sll_string_t* o);



/**
 * \flags func
 * \name sll_remove_environment_variable
 * \group environment
 * \desc Docs!
 * \arg const sll_string_t* k
 */
__SLL_EXTERNAL void sll_remove_environment_variable(const sll_string_t* k);



/**
 * \flags func
 * \name sll_set_environment_variable
 * \group environment
 * \desc Docs!
 * \arg const sll_string_t* k
 * \arg const sll_string_t* v
 */
__SLL_EXTERNAL void sll_set_environment_variable(const sll_string_t* k,const sll_string_t* v);



#endif
