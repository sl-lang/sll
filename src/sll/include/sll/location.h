#ifndef __SLL_LOCATION_H__
#define __SLL_LOCATION_H__ 1
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Location
 * \group location
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_DEBUG_LINE_DATA_FLAG_FILE
 * \group location
 * \desc Docs!
 * \type sll_file_offset_t
 */
#define SLL_DEBUG_LINE_DATA_FLAG_FILE 1ull



/**
 * \flags macro var
 * \name SLL_DEBUG_LINE_DATA_FLAG_FUNC
 * \group location
 * \desc Docs!
 * \type sll_file_offset_t
 */
#define SLL_DEBUG_LINE_DATA_FLAG_FUNC 2ull



/**
 * \flags func macro
 * \name SLL_DEBUG_LINE_DATA_GET_DATA
 * \group location
 * \desc Docs!
 * \arg const sll_debug_line_data_t* debug_data
 * \ret sll_file_offset_t
 */
#define SLL_DEBUG_LINE_DATA_GET_DATA(debug_data) ((debug_data)->line>>2)



/**
 * \flags func macro
 * \name SLL_DEBUG_LINE_DATA_SET_DATA
 * \group location
 * \desc Docs!
 * \arg const sll_debug_line_data_t* debug_data
 * \arg sll_file_offset_t index
 * \arg sll_file_offset_t flags
 */
#define SLL_DEBUG_LINE_DATA_SET_DATA(debug_data,index,flags) (debug_data)->line=((index)<<2)|(flags)



/**
 * \flags check_output func
 * \name sll_get_location
 * \group location
 * \desc Docs!
 * \arg const sll_assembly_data_t* assembly_data
 * \arg sll_instruction_index_t instruction_index
 * \arg sll_string_index_t* file_path_string_index
 * \arg sll_string_index_t* function_string_index
 * \ret sll_file_offset_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_offset_t sll_get_location(const sll_assembly_data_t* assembly_data,sll_instruction_index_t instruction_index,sll_string_index_t* file_path_string_index,sll_string_index_t* function_string_index);



/**
 * \flags func
 * \name sll_get_name
 * \group location
 * \desc Docs!
 * \arg sll_object_t* object
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_get_name(sll_object_t* object,sll_string_t* out);



#endif
