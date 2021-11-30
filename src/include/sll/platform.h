#ifndef __SLL_PLATFORM_H__
#define __SLL_PLATFORM_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Platform
 * \group platform
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Files
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Paths
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Console
 * \group platform
 * \subgroup platform-console
 * \desc Docs!
 */



#define SLL_PLATFORM_STREAM_INPUT 0
#define SLL_PLATFORM_STREAM_OUTPUT 1
#define SLL_PLATFORM_STREAM_ERROR 2



/**
 * \flags var
 * \name sll_platform_string
 * \group platform
 * \desc Docs!
 * \type const sll_char_t*
 */

__SLL_EXTERNAL extern const sll_char_t* sll_platform_string;



/**
 * \flags check_output func
 * \name sll_platform_allocate_page
 * \group platform
 * \desc Docs!
 * \arg sll_page_size_t sz
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_platform_allocate_page(sll_page_size_t sz);



/**
 * \flags func
 * \name sll_platform_enable_console_color
 * \group platform
 * \subgroup platform-console
 * \desc Docs!
 */
__SLL_EXTERNAL void sll_platform_enable_console_color(void);



/**
 * \flags func
 * \name sll_platform_file_close
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \arg sll_file_descriptor_t fd
 */
__SLL_EXTERNAL void sll_platform_file_close(sll_file_descriptor_t fd);



/**
 * \flags check_output func
 * \name sll_platform_file_open
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \arg sll_file_flags_t ff
 * \ret sll_file_descriptor_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_descriptor_t sll_platform_file_open(const sll_char_t* fp,sll_file_flags_t ff);



/**
 * \flags check_output func
 * \name sll_platform_file_read
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \arg sll_file_descriptor_t fd
 * \arg void* p
 * \arg sll_size_t sz
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_file_read(sll_file_descriptor_t fd,void* p,sll_size_t sz);



/**
 * \flags func
 * \name sll_platform_file_seek
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \arg sll_file_descriptor_t fd
 * \arg sll_file_offset_t off
 */
__SLL_EXTERNAL void sll_platform_file_seek(sll_file_descriptor_t fd,sll_file_offset_t off);



/**
 * \flags check_output func
 * \name sll_platform_file_size
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \arg sll_file_descriptor_t fd
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_file_size(sll_file_descriptor_t fd);



/**
 * \flags func
 * \name sll_platform_file_write
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \arg sll_file_descriptor_t fd
 * \arg const void* p
 * \arg sll_size_t sz
 * \ret sll_size_t
 */
__SLL_EXTERNAL sll_size_t sll_platform_file_write(sll_file_descriptor_t fd,const void* p,sll_size_t sz);



/**
 * \flags func
 * \name sll_platform_force_exit
 * \group platform
 * \desc Docs!
 * \arg const sll_char_t* a
 * \arg const sll_char_t* b
 * \arg const sll_char_t* c
 * \ret void
 */
__SLL_EXTERNAL __SLL_NO_RETURN void sll_platform_force_exit(const sll_char_t* a,const sll_char_t* b,const sll_char_t* c);



/**
 * \flags func
 * \name sll_platform_free_page
 * \group platform
 * \desc Docs!
 * \arg void* pg
 * \arg sll_page_size_t sz
 */
__SLL_EXTERNAL void sll_platform_free_page(void* pg,sll_page_size_t sz);



/**
 * \flags check_output func
 * \name sll_platform_get_current_time
 * \group platform
 * \desc Docs!
 * \ret sll_time_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_time_t sll_platform_get_current_time(void);



/**
 * \flags check_output func
 * \name sll_platform_get_current_working_directory
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg sll_char_t* o
 * \arg sll_string_length_t ol
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_platform_get_current_working_directory(sll_char_t* o,sll_string_length_t ol);



/**
 * \flags check_output func
 * \name sll_platform_get_default_stream_descriptor
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \arg sll_char_t t
 * \ret sll_file_descriptor_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_descriptor_t sll_platform_get_default_stream_descriptor(sll_char_t t);



/**
 * \flags check_output func
 * \name sll_platform_get_executable_file_path
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg sll_char_t* o
 * \arg sll_string_length_t ol
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_platform_get_executable_file_path(sll_char_t* o,sll_string_length_t ol);



/**
 * \flags check_output func
 * \name sll_platform_get_page_size
 * \group platform
 * \desc Docs!
 * \ret sll_page_size_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_page_size_t sll_platform_get_page_size(void);



/**
 * \flags check_output func
 * \name sll_platform_list_directory
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \arg sll_string_t** o
 * \ret sll_array_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_platform_list_directory(const sll_char_t* fp,sll_string_t** o);



/**
 * \flags check_output func
 * \name sll_platform_list_directory_recursive
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \arg sll_string_t** o
 * \ret sll_array_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_platform_list_directory_recursive(const sll_char_t* fp,sll_string_t** o);



/**
 * \flags check_output func
 * \name sll_platform_path_exists
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_exists(const sll_char_t* fp);



/**
 * \flags check_output func
 * \name sll_platform_path_is_directory
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_is_directory(const sll_char_t* fp);



/**
 * \flags func
 * \name sll_platform_reset_console
 * \group platform
 * \subgroup platform-console
 * \desc Docs!
 */
__SLL_EXTERNAL void sll_platform_reset_console(void);



/**
 * \flags check_output func
 * \name sll_platform_set_current_working_directory
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg const sll_char_t* p
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_set_current_working_directory(const sll_char_t* p);



/**
 * \flags func
 * \name sll_platform_setup_console
 * \group platform
 * \subgroup platform-console
 * \desc Docs!
 */
__SLL_EXTERNAL void sll_platform_setup_console(void);



/**
 * \flags func
 * \name sll_platform_sleep
 * \group platform
 * \desc Docs!
 * \arg sll_time_t tm
 */
__SLL_EXTERNAL void sll_platform_sleep(sll_time_t tm);



#endif
