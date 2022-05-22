#ifndef __SLL_DATA_H__
#define __SLL_DATA_H__ 1
#include <sll/_size_types.h>
#include <sll/_string_types.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Data Operations
 * \group data
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_COMPARE_RESULT_BELOW
 * \group data
 * \desc Docs!
 * \type sll_compare_result_t
 */
#define SLL_COMPARE_RESULT_BELOW 0



/**
 * \flags macro var
 * \name SLL_COMPARE_RESULT_EQUAL
 * \group data
 * \desc Docs!
 * \type sll_compare_result_t
 */
#define SLL_COMPARE_RESULT_EQUAL 1



/**
 * \flags macro var
 * \name SLL_COMPARE_RESULT_ABOVE
 * \group data
 * \desc Docs!
 * \type sll_compare_result_t
 */
#define SLL_COMPARE_RESULT_ABOVE 2



/**
 * \flags func macro
 * \name SLL_COPY_STRING_NULL
* \group data
 * \desc Docs!
 * \arg const sll_char_t* source
 * \arg void* target
 */
#define SLL_COPY_STRING_NULL(source,target) (*((sll_char_t*)sll_copy_string((source),(target)))=0)



/**
 * \flags type var
 * \name sll_compare_result_t
 * \group data
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_compare_result_t;



/**
 * \flags check_output func
 * \name sll_compare_data
 * \group data
 * \desc Docs!
 * \arg const void* a
 * \arg const void* b
 * \arg sll_size_t length
 * \ret sll_compare_result_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_compare_data(const void* a,const void* b,sll_size_t length);



/**
 * \flags check_output func
 * \name sll_contains_character
 * \group data
 * \desc Docs!
 * \arg const void* pointer
 * \arg sll_size_t length
 * \arg sll_char_t char_
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_contains_character(const void* pointer,sll_size_t length,sll_char_t char_);



/**
 * \flags func
 * \name sll_copy_data
 * \group data
 * \desc Docs!
 * \arg const void* source
 * \arg sll_size_t length
 * \arg void* target
 */
__SLL_EXTERNAL void sll_copy_data(const void* source,sll_size_t length,void* target);



/**
 * \flags func
 * \name sll_copy_string
 * \group data
 * \desc Docs!
 * \arg const sll_char_t* source
 * \arg void* target
 * \ret void*
 */
__SLL_EXTERNAL void* sll_copy_string(const sll_char_t* source,void* target);



/**
 * \flags func
 * \name sll_copy_string_null
 * \group data
 * \desc Docs!
 * \arg const sll_char_t* source
 * \arg void* target
 */
__SLL_EXTERNAL void sll_copy_string_null(const sll_char_t* source,void* target);



/**
 * \flags func
 * \name sll_set_memory
 * \group data
 * \desc Docs!
 * \arg void* pointer
 * \arg sll_size_t length
 * \arg sll_char_t value
 */
__SLL_EXTERNAL void sll_set_memory(void* pointer,sll_size_t length,sll_char_t value);



/**
 * \flags func
 * \name sll_zero_memory
 * \group data
 * \desc Docs!
 * \arg void* pointer
 * \arg sll_size_t length
 */
__SLL_EXTERNAL void sll_zero_memory(void* pointer,sll_size_t length);



#endif
