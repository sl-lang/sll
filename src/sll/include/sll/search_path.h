#ifndef __SLL_SEARCH_PATH_H__
#define __SLL_SEARCH_PATH_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/string.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Search Path
 * \group search-path
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_SEARCH_PATH_SPLIT_CHAR
 * \group search-path
 * \desc Docs!
 * \type sll_char_t
 */
#ifdef __SLL_BUILD_WINDOWS
#define SLL_SEARCH_PATH_SPLIT_CHAR ';'
#else
#define SLL_SEARCH_PATH_SPLIT_CHAR ':'
#endif



/**
 * \flags macro var
 * \name SLL_SEARCH_PATH_FLAG_BEFORE
 * \group search-path
 * \desc Docs!
 * \type sll_search_path_length_t
 */
#define SLL_SEARCH_PATH_FLAG_BEFORE 1



/**
 * \flags macro var
 * \name SLL_SEARCH_PATH_FLAG_AFTER
 * \group search-path
 * \desc Docs!
 * \type sll_search_path_length_t
 */
#define SLL_SEARCH_PATH_FLAG_AFTER 2



/**
 * \flags type var
 * \name sll_search_flags_t
 * \group search-path
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_search_flags_t;



/**
 * \flags type var
 * \name sll_search_path_length_t
 * \group search-path
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_search_path_length_t;



/**
 * \flags type
 * \name sll_search_path_t
 * \group search-path
 * \desc Docs!
 * \arg sll_string_t* data
 * \arg sll_search_path_length_t length
 */
typedef struct _SLL_SEARCH_PATH{
	sll_string_t* data;
	sll_search_path_length_t length;
} sll_search_path_t;



/**
 * \flags func
 * \name sll_free_search_path
 * \group search-path
 * \desc Docs!
 * \arg sll_search_path_t* search_path
 */
__SLL_EXTERNAL void sll_free_search_path(sll_search_path_t* search_path);



/**
 * \flags func
 * \name sll_search_path_create
 * \group search-path
 * \desc Docs!
 * \arg const sll_string_t* string
 * \arg sll_search_path_t* out
 */
__SLL_EXTERNAL void sll_search_path_create(const sll_string_t* string,sll_search_path_t* out);



/**
 * \flags check_output func
 * \name sll_search_path_find
 * \group search-path
 * \desc Docs!
 * \arg const sll_search_path_t* search_path
 * \arg const sll_string_t* name
 * \arg sll_search_flags_t flag
 * \arg sll_string_t* out
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_search_path_find(const sll_search_path_t* search_path,const sll_string_t* name,sll_search_flags_t flags,sll_string_t* out);



/**
 * \flags func
 * \name sll_search_path_from_environment
 * \group search-path
 * \desc Docs!
 * \arg const sll_string_t* key
 * \arg sll_search_path_t* out
 */
__SLL_EXTERNAL void sll_search_path_from_environment(const sll_string_t* key,sll_search_path_t* out);



#endif
