#ifndef __SLL_UTIL_H__
#define __SLL_UTIL_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Utilities
 * \group util
 * \desc Docs!
 */



#define SLL_COMPARE_RESULT_BELOW 0
#define SLL_COMPARE_RESULT_EQUAL 1
#define SLL_COMPARE_RESULT_ABOVE 2



/**
 * \flags check_output func
 * \name sll_add_string
 * \group util
 * \desc Docs!
 * \arg sll_string_table_t* st
 * \arg sll_string_t* s
 * \arg sll_bool_t d
 * \ret sll_string_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_add_string(sll_string_table_t* st,sll_string_t* s,sll_bool_t d);



/**
 * \flags check_output func
 * \name sll_add_string_object
 * \group util
 * \desc Docs!
 * \arg sll_string_table_t* st
 * \arg sll_object_t* v
 * \ret sll_string_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_add_string_object(sll_string_table_t* st,sll_object_t* v);



/**
 * \flags check_output func
 * \name sll_create_string
 * \group util
 * \desc Docs!
 * \arg sll_string_table_t* st
 * \arg const sll_char_t* dt
 * \arg sll_string_length_t l
 * \ret sll_string_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_create_string(sll_string_table_t* st,const sll_char_t* dt,sll_string_length_t l);



/**
 * \flags check_output func
 * \name sll_get_location
 * \group util
 * \desc Docs!
 * \arg const sll_assembly_data_t* a_dt
 * \arg sll_instruction_index_t ii
 * \arg sll_string_index_t* fp
 * \arg sll_string_index_t* fn
 * \ret sll_file_offset_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_offset_t sll_get_location(const sll_assembly_data_t* a_dt,sll_instruction_index_t ii,sll_string_index_t* fp,sll_string_index_t* fn);



#endif
