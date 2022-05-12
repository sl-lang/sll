#ifndef __SLL_PLATFORM_H__
#define __SLL_PLATFORM_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/error.h>
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
 * \arg sll_size_t size
 * \ret sll_size_t
 */
#define SLL_ROUND_PAGE(size) (((size)+SLL_PAGE_SIZE-1)&(-SLL_PAGE_SIZE))



/**
 * \flags func macro
 * \name SLL_ROUND_PAGE_LARGE
 * \group platform
 * \subgroup platform-memory
 * \desc Docs!
 * \arg sll_size_t size
 * \ret sll_size_t
 */
#define SLL_ROUND_LARGE_PAGE(size) (((size)+SLL_LARGE_PAGE_SIZE-1)&(-SLL_LARGE_PAGE_SIZE))



/**
 * \flags check_output func
 * \name sll_platform_allocate_page
 * \group platform
 * \subgroup platform-memory
 * \desc Docs!
 * \arg sll_size_t size
 * \arg sll_bool_t large
 * \arg sll_error_t* err
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_platform_allocate_page(sll_size_t size,sll_bool_t large,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_platform_allocate_page_aligned
 * \group platform
 * \subgroup platform-memory
 * \desc Docs!
 * \arg sll_size_t size
 * \arg sll_size_t align
 * \arg sll_error_t* err
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_platform_allocate_page_aligned(sll_size_t size,sll_size_t align,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_platform_free_page
 * \group platform
 * \subgroup platform-memory
 * \desc Docs!
 * \arg void* ptr
 * \arg sll_size_t size
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_free_page(void* ptr,sll_size_t size);



#endif
