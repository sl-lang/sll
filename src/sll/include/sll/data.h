#ifndef __SLL_DATA_H__
#define __SLL_DATA_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Data Operations
 * \group data
 * \desc Docs!
 */



#define SLL_COPY_STRING_NULL(s,d) (*((sll_char_t*)sll_copy_string((s),(d)))=0)



/**
 * \flags check_output func
 * \name sll_compare_data
 * \group data
 * \desc Docs!
 * \arg const void* a
 * \arg const void* b
 * \arg sll_size_t l
 * \ret sll_compare_result_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_compare_data(const void* a,const void* b,sll_size_t l);



/**
 * \flags func
 * \name sll_copy_data
 * \group data
 * \desc Docs!
 * \arg const void* s
 * \arg sll_size_t l
 * \arg void* d
 */
__SLL_EXTERNAL void sll_copy_data(const void* s,sll_size_t l,void* d);



/**
 * \flags func
 * \name sll_copy_string
 * \group data
 * \desc Docs!
 * \arg const sll_char_t* s
 * \arg void* d
 * \ret void*
 */
__SLL_EXTERNAL void* sll_copy_string(const sll_char_t* s,void* d);



/**
 * \flags func
 * \name sll_copy_string_null
 * \group data
 * \desc Docs!
 * \arg const sll_char_t* s
 * \arg void* d
 */
__SLL_EXTERNAL void sll_copy_string_null(const sll_char_t* s,void* d);



/**
 * \flags func
 * \name sll_set_memory
 * \group data
 * \desc Docs!
 * \arg void* p
 * \arg sll_size_t l
 * \arg sll_char_t v
 */
__SLL_EXTERNAL void sll_set_memory(void* p,sll_size_t l,sll_char_t v);



/**
 * \flags func
 * \name sll_zero_memory
 * \group data
 * \desc Docs!
 * \arg void* p
 * \arg sll_size_t l
 */
__SLL_EXTERNAL void sll_zero_memory(void* p,sll_size_t l);



#endif
