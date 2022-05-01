#ifndef __SLL_PLATFORM_LIBRARY_H__
#define __SLL_PLATFORM_LIBRARY_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags subgroup
 * \name External Libraries
 * \group platform
 * \subgroup platform-lib
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_UNKNOWN_LIBRARY_HANDLE
 * \group platform
 * \subgroup platform-lib
 * \desc Docs!
 * \type sll_library_handle_t
 */
#define SLL_UNKNOWN_LIBRARY_HANDLE ((sll_library_handle_t)0xffffffffffffffffull)



/**
 * \flags type var
 * \name sll_library_handle_t
 * \group platform
 * \subgroup platform-lib
 * \desc Docs!
 * \type void*
 */
typedef void* sll_library_handle_t;



/**
 * \flags check_output func
 * \name sll_platform_load_library
 * \group platform
 * \subgroup platform-lib
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \ret sll_library_handle_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_library_handle_t sll_platform_load_library(const sll_char_t* fp);



/**
 * \flags check_output func
 * \name sll_platform_lookup_symbol
 * \group platform
 * \subgroup platform-lib
 * \desc Docs!
 * \arg sll_library_handle_t h
 * \arg const sll_char_t* nm
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_platform_lookup_symbol(sll_library_handle_t h,const sll_char_t* nm);



/**
 * \flags func
 * \name sll_platform_unload_library
 * \group platform
 * \subgroup platform-lib
 * \desc Docs!
 * \arg sll_library_handle_t h
 */
__SLL_EXTERNAL void sll_platform_unload_library(sll_library_handle_t h);



#endif
