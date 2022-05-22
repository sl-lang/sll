#ifndef __SLL_STRING_TABLE_H__
#define __SLL_STRING_TABLE_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/string.h>
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
 * \arg sll_string_t* data
 * \arg sll_string_index_t length
 */
typedef struct _SLL_STRING_TABLE{
	sll_string_t* data;
	sll_string_index_t length;
} sll_string_table_t;



/**
 * \flags check_output func
 * \name sll_add_string
 * \group string-table
 * \desc Docs!
 * \arg sll_string_table_t* string_table
 * \arg sll_string_t* string
 * \ret sll_string_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_add_string(sll_string_table_t* string_table,sll_string_t* string);



#endif
