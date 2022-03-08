#ifndef __SLL_PLATFORM_H__
#define __SLL_PLATFORM_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags subgroup
 * \name Virtual Memory Management
 * \group platform
 * \subgroup platform-memory
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_PAGE_SIZE
 * \group platform
 * \subgroup platform-memory
 * \desc Docs!
 * \type sll_size_t
 */
#define SLL_PAGE_SIZE 4096



/**
 * \flags macro var
 * \name SLL_LARGE_PAGE_SIZE
 * \group platform
 * \subgroup platform-memory
 * \desc Docs!
 * \type sll_size_t
 */
#define SLL_LARGE_PAGE_SIZE 2097152



/**
 * \flags func macro
 * \name SLL_ROUND_PAGE
 * \group platform
 * \subgroup platform-memory
 * \desc Docs!
 * \arg sll_size_t x
 * \ret sll_size_t
 */
#define SLL_ROUND_PAGE(x) (((x)+SLL_PAGE_SIZE-1)&(-SLL_PAGE_SIZE))



/**
 * \flags func macro
 * \name SLL_ROUND_PAGE_LARGE
 * \group platform
 * \subgroup platform-memory
 * \desc Docs!
 * \arg sll_size_t x
 * \ret sll_size_t
 */
#define SLL_ROUND_LARGE_PAGE(x) (((x)+SLL_LARGE_PAGE_SIZE-1)&(-SLL_LARGE_PAGE_SIZE))



/**
 * \flags check_output func
 * \name sll_platform_allocate_page
 * \group platform
 * \subgroup platform-memory
 * \desc Docs!
 * \arg sll_size_t sz
 * \arg sll_bool_t l
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_platform_allocate_page(sll_size_t sz,sll_bool_t l);



/**
 * \flags func
 * \name sll_platform_free_page
 * \group platform
 * \subgroup platform-memory
 * \desc Docs!
 * \arg void* pg
 * \arg sll_size_t sz
 */
__SLL_EXTERNAL void sll_platform_free_page(void* pg,sll_size_t sz);



#endif
