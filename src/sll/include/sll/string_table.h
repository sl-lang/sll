#ifndef __SLL_STRING_TABLE_H__
#define __SLL_STRING_TABLE_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name String Table
 * \group string-table
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_MAX_STRING_INDEX
 * \group string-table
 * \desc Docs!
 * \type sll_string_index_t
 */
#define SLL_MAX_STRING_INDEX __SLL_U32_MAX



/**
 * \flags type var
 * \name sll_string_index_t
 * \group string-table
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_string_index_t;



/**
 * \flags type
 * \name sll_string_table_t
 * \group string-table
 * \desc Docs!
 * \arg sll_string_t* dt
 * \arg sll_string_index_t l
 */
typedef struct _SLL_STRING_TABLE{
    sll_string_t* dt;
    sll_string_index_t l;
} sll_string_table_t;



/**
 * \flags check_output func
 * \name sll_add_string
 * \group string-table
 * \desc Docs!
 * \arg sll_string_table_t* st
 * \arg sll_string_t* s
 * \arg sll_bool_t d
 * \ret sll_string_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_add_string(sll_string_table_t* st,sll_string_t* s,sll_bool_t d);



/**
 * \flags check_output func
 * \name sll_create_object_string
 * \group string-table
 * \desc Docs!
 * \arg sll_string_table_t* st
 * \arg const sll_char_t* dt
 * \arg sll_string_length_t l
 * \ret sll_string_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_create_object_string(sll_string_table_t* st,const sll_char_t* dt,sll_string_length_t l);



#endif
