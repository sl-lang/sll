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
/**
 * \flags subgroup
 * \name String Table
 * \group util
 * \subgroup util-str
 * \desc Docs!
 */



#define SLL_COMPARE_RESULT_BELOW 0
#define SLL_COMPARE_RESULT_EQUAL 1
#define SLL_COMPARE_RESULT_ABOVE 2

#define SLL_SANDBOX_FLAG_DISABLE_FILE_IO 1
#define SLL_SANDBOX_FLAG_ENABLE_STDIN_IO 2
#define SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO 4
#define SLL_SANDBOX_FLAG_DISABLE_PATH_API 8
#define SLL_SANDBOX_FLAG_DISABLE_PROCESS_API 16



/**
 * \flags check_output func
 * \name sll_add_string
 * \group util
 * \subgroup util-str
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
 * \subgroup util-str
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
 * \subgroup util-str
 * \desc Docs!
 * \arg sll_string_table_t* st
 * \arg const sll_char_t* dt
 * \arg sll_string_length_t l
 * \ret sll_string_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_create_string(sll_string_table_t* st,const sll_char_t* dt,sll_string_length_t l);



/**
 * \flags func
 * \name sll_deinit
 * \group util
 * \desc Docs!
 */
__SLL_EXTERNAL void sll_deinit(void);



/**
 * \flags check_output func
 * \name sll_get_environment_variable
 * \group util
 * \desc Docs!
 * \arg const sll_string_t* k
 * \arg sll_string_t* o
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_get_environment_variable(const sll_string_t* k,sll_string_t* o);



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



/**
 * \flags check_output func
 * \name sll_get_sandbox_flag
 * \group util
 * \desc Docs!
 * \arg sll_sandbox_flags_t f
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_get_sandbox_flag(sll_sandbox_flags_t f);



/**
 * \flags check_output func
 * \name sll_get_sandbox_flags
 * \group util
 * \desc Docs!
 * \ret sll_sandbox_flags_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_sandbox_flags_t sll_get_sandbox_flags(void);



/**
 * \flags func
 * \name sll_init
 * \group util
 * \desc Docs!
 */
__SLL_EXTERNAL void sll_init(void);



/**
 * \flags func
 * \name sll_remove_environment_variable
 * \group util
 * \desc Docs!
 * \arg const sll_string_t* k
 */
__SLL_EXTERNAL void sll_remove_environment_variable(const sll_string_t* k);



/**
 * \flags func
 * \name sll_set_environment_variable
 * \group util
 * \desc Docs!
 * \arg const sll_string_t* k
 * \arg const sll_string_t* v
 */
__SLL_EXTERNAL void sll_set_environment_variable(const sll_string_t* k,const sll_string_t* v);



/**
 * \flags func
 * \name sll_set_sandbox_flags
 * \group util
 * \desc Docs!
 * \arg sll_sandbox_flags_t f
 * \ret sll_sandbox_flags_t
 */
__SLL_EXTERNAL sll_sandbox_flags_t sll_set_sandbox_flags(sll_sandbox_flags_t f);



/**
 * \flags func
 * \name sll_register_cleanup
 * \group util
 * \desc Docs!
 * \arg sll_cleanup_function_t f
 */
__SLL_EXTERNAL void sll_register_cleanup(sll_cleanup_function_t f);



#endif
