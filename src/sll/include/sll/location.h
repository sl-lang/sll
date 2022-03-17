#ifndef __SLL_LOCATION_H__
#define __SLL_LOCATION_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Location
 * \group location
 * \desc Docs!
 */



/**
 * \flags func macro
 * \name SLL_DEBUG_LINE_DATA_GET_DATA
 * \group location
 * \desc Docs!
 * \arg const sll_debug_line_data_t* dbg
 * \ret sll_file_offset_t
 */
#define SLL_DEBUG_LINE_DATA_GET_DATA(dbg) ((dbg)->ln&0x3fffffffffffffffull)



/**
 * \flags macro var
 * \name SLL_DEBUG_LINE_DATA_FLAG_FILE
 * \group location
 * \desc Docs!
 * \type sll_file_offset_t
 */
#define SLL_DEBUG_LINE_DATA_FLAG_FILE 0x4000000000000000ull



/**
 * \flags macro var
 * \name SLL_DEBUG_LINE_DATA_FLAG_FUNC
 * \group location
 * \desc Docs!
 * \type sll_file_offset_t
 */
#define SLL_DEBUG_LINE_DATA_FLAG_FUNC 0x8000000000000000ull



/**
 * \flags check_output func
 * \name sll_get_location
 * \group location
 * \desc Docs!
 * \arg const sll_assembly_data_t* a_dt
 * \arg sll_instruction_index_t ii
 * \arg sll_string_index_t* fp
 * \arg sll_string_index_t* fn
 * \ret sll_file_offset_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_offset_t sll_get_location(const sll_assembly_data_t* a_dt,sll_instruction_index_t ii,sll_string_index_t* fp,sll_string_index_t* fn);



/**
 * \flags func
 * \name sll_get_name
 * \group location
 * \desc Docs!
 * \arg sll_object_t* v
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_get_name(sll_object_t* v,sll_string_t* o);



#endif
