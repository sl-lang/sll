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
 * \flags func
 * \name sll_create_search_path
 * \group env
 * \desc Docs!
 * \arg const sll_string_t* src
 * \arg sll_search_path_t* o
 */
__SLL_EXTERNAL void sll_create_search_path(const sll_string_t* src,sll_search_path_t* o);



/**
 * \flags func
 * \name sll_free_search_path
 * \group env
 * \desc Docs!
 * \arg sll_search_path_t* sp
 */
__SLL_EXTERNAL void sll_free_search_path(sll_search_path_t* sp);



#endif
