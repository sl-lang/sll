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
 * \name Memory
 * \group util
 * \subgroup util-mem
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

#define SLL_COPY_STRING_NULL(s,d) (*((sll_char_t*)sll_copy_string((s),(d)))=0)



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
 * \name sll_compare_data
 * \group util
 * \subgroup util-mem
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
 * \group util
 * \subgroup util-mem
 * \desc Docs!
 * \arg const void* s
 * \arg sll_size_t l
 * \arg void* d
 */
__SLL_EXTERNAL void sll_copy_data(const void* s,sll_size_t l,void* d);



/**
 * \flags func
 * \name sll_copy_string
 * \group util
 * \subgroup util-mem
 * \desc Docs!
 * \arg const sll_char_t* s
 * \arg void* d
 * \ret void*
 */
__SLL_EXTERNAL void* sll_copy_string(const sll_char_t* s,void* d);



/**
 * \flags func
 * \name sll_copy_string_null
 * \group util
 * \subgroup util-mem
 * \desc Docs!
 * \arg const sll_char_t* s
 * \arg void* d
 */
__SLL_EXTERNAL void sll_copy_string_null(const sll_char_t* s,void* d);



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
 * \flags func
 * \name sll_free_string_table
 * \group util
 * \subgroup util-str
 * \desc Docs!
 * \arg sll_string_table_t* st
 */
__SLL_EXTERNAL void sll_free_string_table(sll_string_table_t* st);



/**
 * \flags func
 * \name sll_get_sandbox_flag
 * \group util
 * \desc Docs!
 * \arg sll_sandbox_flags_t f
 * \ret sll_bool_t
 */
__SLL_EXTERNAL sll_bool_t sll_get_sandbox_flag(sll_sandbox_flags_t f);



/**
 * \flags func
 * \name sll_init
 * \group util
 * \desc Docs!
 */
__SLL_EXTERNAL void sll_init(void);



/**
 * \flags func
 * \name sll_set_memory
 * \group util
 * \subgroup util-mem
 * \desc Docs!
 * \arg void* p
 * \arg sll_size_t l
 * \arg sll_char_t v
 */
__SLL_EXTERNAL void sll_set_memory(void* p,sll_size_t l,sll_char_t v);



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



/**
 * \flags check_output func
 * \name sll_version
 * \group util
 * \desc Docs!
 * \ret sll_version_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_version_t sll_version(void);



/**
 * \flags func
 * \name sll_zero_memory
 * \group util
 * \subgroup util-mem
 * \desc Docs!
 * \arg void* p
 * \arg sll_size_t l
 */
__SLL_EXTERNAL void sll_zero_memory(void* p,sll_size_t l);



#endif
