#ifndef __SLL_SEARCH_PATH_H__
#define __SLL_SEARCH_PATH_H__ 1
#include <sll/common.h>
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
 * \type sll_flags_t
 */
#define SLL_SEARCH_PATH_FLAG_BEFORE 1



/**
 * \flags macro var
 * \name SLL_SEARCH_PATH_FLAG_AFTER
 * \group search-path
 * \desc Docs!
 * \type sll_flags_t
 */
#define SLL_SEARCH_PATH_FLAG_AFTER 2



/**
 * \flags func
 * \name sll_free_search_path
 * \group search-path
 * \desc Docs!
 * \arg sll_search_path_t* sp
 */
__SLL_EXTERNAL void sll_free_search_path(sll_search_path_t* sp);



/**
 * \flags func
 * \name sll_search_path_create
 * \group search-path
 * \desc Docs!
 * \arg const sll_string_t* src
 * \arg sll_search_path_t* o
 */
__SLL_EXTERNAL void sll_search_path_create(const sll_string_t* src,sll_search_path_t* o);



/**
 * \flags check_output func
 * \name sll_search_path_find
 * \group search-path
 * \desc Docs!
 * \arg const sll_search_path_t* sp
 * \arg const sll_string_t* nm
 * \arg sll_flags_t fl
 * \arg sll_string_t* o
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_search_path_find(const sll_search_path_t* sp,const sll_string_t* nm,sll_flags_t fl,sll_string_t* o);



/**
 * \flags func
 * \name sll_search_path_from_environment
 * \group search-path
 * \desc Docs!
 * \arg const sll_string_t* key
 * \arg sll_search_path_t* o
 */
__SLL_EXTERNAL void sll_search_path_from_environment(const sll_string_t* key,sll_search_path_t* o);



#endif
