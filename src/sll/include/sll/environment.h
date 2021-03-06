#ifndef __SLL_ENVIRONMENT_H__
#define __SLL_ENVIRONMENT_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/search_path.h>
#include <sll/string.h>
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
 * \arg const sll_string_t key
 * \arg const sll_string_t value
 */
typedef struct _SLL_ENVIRONMENT_VARIABLE{
	const sll_string_t key;
	const sll_string_t value;
} sll_environment_variable_t;



/**
 * \flags type
 * \name sll_environment_t
 * \group environment
 * \desc Docs!
 * \arg const sll_environment_variable_t*const* data
 * \arg const sll_environment_length_t length
 */
typedef struct _SLL_ENVIRONMENT{
	const sll_environment_variable_t*const* data;
	const sll_environment_length_t length;
} sll_environment_t;



/**
 * \flags var
 * \name sll_environment
 * \group environment
 * \desc Docs!
 * \type const sll_environment_t*
 */
__SLL_EXTERNAL extern const sll_environment_t* sll_environment;



/**
 * \flags var
 * \name sll_environment_path
 * \group environment
 * \desc Docs!
 * \type const sll_search_path_t*
 */
__SLL_EXTERNAL extern const sll_search_path_t* sll_environment_path;



/**
 * \flags func
 * \name sll_expand_environment_variable
 * \group environment
 * \desc Docs!
 * \arg const sll_string_t* key
 * \arg sll_string_t* out
 * \ret sll_bool_t
 */
__SLL_EXTERNAL sll_bool_t sll_expand_environment_variable(const sll_string_t* key,sll_string_t* out);



/**
 * \flags func
 * \name sll_get_environment_variable
 * \group environment
 * \desc Docs!
 * \arg const sll_string_t* key
 * \arg sll_string_t* out
 * \ret sll_bool_t
 */
__SLL_EXTERNAL sll_bool_t sll_get_environment_variable(const sll_string_t* key,sll_string_t* out);



/**
 * \flags func
 * \name sll_remove_environment_variable
 * \group environment
 * \desc Docs!
 * \arg const sll_string_t* key
 */
__SLL_EXTERNAL void sll_remove_environment_variable(const sll_string_t* key);



/**
 * \flags func
 * \name sll_set_environment_variable
 * \group environment
 * \desc Docs!
 * \arg const sll_string_t* key
 * \arg const sll_string_t* value
 */
__SLL_EXTERNAL void sll_set_environment_variable(const sll_string_t* key,const sll_string_t* value);



#endif
