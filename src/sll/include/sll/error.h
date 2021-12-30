#ifndef __SLL_ERROR_H__
#define __SLL_ERROR_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Error
 * \group error
 * \desc Docs!
 */



#define SLL_ERROR_UNMATCHED_OPEN_PARENTHESES 0
#define SLL_ERROR_UNMATCHED_CLOSE_PARENTHESES 1
#define SLL_ERROR_UNMATCHED_CURLY_OPEN_BRACKETS 2
#define SLL_ERROR_UNMATCHED_CURLY_CLOSE_BRACKETS 3
#define SLL_ERROR_UNMATCHED_ARRAY_OPEN_BRACKETS 4
#define SLL_ERROR_UNMATCHED_ARRAY_CLOSE_BRACKETS 5
#define SLL_ERROR_UNMATCHED_MAP_OPEN_BRACKETS 6
#define SLL_ERROR_UNMATCHED_MAP_CLOSE_BRACKETS 7
#define SLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER 8
#define SLL_ERROR_UNKNOWN_DECIMAL_CHARCTER 9
#define SLL_ERROR_UNKNOWN_OCTAL_CHARCTER 10
#define SLL_ERROR_UNKNOWN_BINARY_CHARCTER 11

#define SLL_MAX_ERROR SLL_ERROR_UNKNOWN_BINARY_CHARCTER



/**
 * \flags check_output func
 * \name sll_print_error
 * \group error
 * \desc Docs!
 * \arg sll_file_t* rf
 * \arg const sll_error_t* e
 */
__SLL_EXTERNAL void sll_print_error(sll_file_t* rf,const sll_error_t* e);



#endif
