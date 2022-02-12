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
 * \name Console
 * \group platform
 * \subgroup platform-console
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
 * \name External Libraries
 * \group platform
 * \subgroup platform-lib
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Virtual Memory Management
 * \group platform
 * \subgroup platform-memory
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
 * \name Processes
 * \group platform
 * \subgroup platform-process
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_PLATFORM_STREAM_INPUT
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \type sll_char_t
 */
#define SLL_PLATFORM_STREAM_INPUT 0



/**
 * \flags macro var
 * \name SLL_PLATFORM_STREAM_OUTPUT
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \type sll_char_t
 */
#define SLL_PLATFORM_STREAM_OUTPUT 1



/**
 * \flags macro var
 * \name SLL_PLATFORM_STREAM_ERROR
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \type sll_char_t
 */
#define SLL_PLATFORM_STREAM_ERROR 2



/**
 * \flags macro var
 * \name SLL_PAGE_SIZE
 * \group platform
 * \subgroup platform-memory
 * \desc Docs!
 * \type sll_size_t
 */
#define SLL_PAGE_SIZE 4096



/**
 * \flags macro var
 * \name SLL_LARGE_PAGE_SIZE
 * \group platform
 * \subgroup platform-memory
 * \desc Docs!
 * \type sll_size_t
 */
#define SLL_LARGE_PAGE_SIZE 2097152



/**
 * \flags func macro
 * \name SLL_ROUND_PAGE
 * \group platform
 * \subgroup platform-memory
 * \desc Docs!
 * \arg sll_size_t x
 * \ret sll_size_t
 */
#define SLL_ROUND_PAGE(x) (((x)+SLL_PAGE_SIZE-1)&(-SLL_PAGE_SIZE))



/**
 * \flags func macro
 * \name SLL_ROUND_PAGE_LARGE
 * \group platform
 * \subgroup platform-memory
 * \desc Docs!
 * \arg sll_size_t x
 * \ret sll_size_t
 */
#define SLL_ROUND_LARGE_PAGE(x) (((x)+SLL_LARGE_PAGE_SIZE-1)&(-SLL_LARGE_PAGE_SIZE))



/**
 * \flags var
 * \name sll_environment
 * \group platform
 * \desc Docs!
 * \type const sll_environment_t*
 */
__SLL_EXTERNAL extern const sll_environment_t* sll_environment;



/**
 * \flags var
 * \name sll_platform_string
 * \group platform
 * \desc Docs!
 * \type const sll_string_t*
 */
__SLL_EXTERNAL extern const sll_string_t* sll_platform_string;



/**
 * \flags var
 * \name sll_platform_time_zone
 * \group platform
 * \desc Docs!
 * \type const sll_time_zone_t*
 */
__SLL_EXTERNAL extern const sll_time_zone_t* sll_platform_time_zone;



/**
 * \flags func
 * \name sll_platform_absolute_path
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \arg sll_char_t* o
 * \arg sll_string_length_t ol
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL sll_string_length_t sll_platform_absolute_path(const sll_char_t* fp,sll_char_t* o,sll_string_length_t ol);



/**
 * \flags check_output func
 * \name sll_platform_allocate_page
 * \group platform
 * \subgroup platform-memory
 * \desc Docs!
 * \arg sll_size_t sz
 * \arg sll_bool_t l
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_platform_allocate_page(sll_size_t sz,sll_bool_t l);



/**
 * \flags func
 * \name sll_platform_create_console
 * \group platform
 * \subgroup platform-console
 * \desc Docs!
 */
__SLL_EXTERNAL void sll_platform_create_console(void);



/**
 * \flags func
 * \name sll_platform_enable_console_color
 * \group platform
 * \subgroup platform-console
 * \desc Docs!
 */
__SLL_EXTERNAL void sll_platform_enable_console_color(void);



/**
 * \flags check_output func
 * \name sll_platform_execute_shell
 * \group platform
 * \subgroup platform-process
 * \desc Docs!
 * \arg const sll_char_t* cmd
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_execute_shell(const sll_char_t* cmd);



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
 * \subgroup platform-memory
 * \desc Docs!
 * \arg void* pg
 * \arg sll_size_t sz
 */
__SLL_EXTERNAL void sll_platform_free_page(void* pg,sll_size_t sz);



/**
 * \flags func
 * \name sll_platform_close_process_handle
 * \group platform
 * \subgroup platform-process
 * \desc Docs!
 * \arg sll_process_handle_t ph
 */
__SLL_EXTERNAL void sll_platform_close_process_handle(sll_process_handle_t ph);



/**
 * \flags check_output func
 * \name sll_platform_get_cpu_count
 * \group platform
 * \desc Docs!
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_get_cpu_count(void);



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
 * \name sll_platform_get_library_file_path
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg sll_char_t* o
 * \arg sll_string_length_t ol
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_platform_get_library_file_path(sll_char_t* o,sll_string_length_t ol);



/**
 * \flags check_output func
 * \name sll_platform_get_pid
 * \group platform
 * \subgroup platform-process
 * \desc Docs!
 * \ret sll_pid_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_pid_t sll_platform_get_pid(void);



/**
 * \flags check_output func
 * \name sll_platform_get_temporary_file_path
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg sll_char_t* o
 * \arg sll_string_length_t ol
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_platform_get_temporary_file_path(sll_char_t* o,sll_string_length_t ol);



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
 * \name sll_platform_load_library
 * \group platform
 * \subgroup platform-lib
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \ret sll_library_handle_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_library_handle_t sll_platform_load_library(const sll_char_t* fp);



/**
 * \flags check_output func
 * \name sll_platform_lookup_function
 * \group platform
 * \subgroup platform-lib
 * \desc Docs!
 * \arg sll_library_handle_t h
 * \arg const sll_char_t* nm
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_platform_lookup_function(sll_library_handle_t h,const sll_char_t* nm);



/**
 * \flags check_output func
 * \name sll_platform_path_copy
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg const sll_char_t* s
 * \arg const sll_char_t* d
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_copy(const sll_char_t* s,const sll_char_t* d);



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
 * \flags check_output func
 * \name sll_platform_path_rename
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg const sll_char_t* s
 * \arg const sll_char_t* d
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_rename(const sll_char_t* s,const sll_char_t* d);



/**
 * \flags func
 * \name sll_platform_random
 * \group platform
 * \desc Docs!
 * \arg void* bf
 * \arg sll_size_t l
 */
__SLL_EXTERNAL void sll_platform_random(void* bf,sll_size_t l);



/**
 * \flags func
 * \name sll_platform_remove_environment_variable
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg const sll_char_t* k
 */
__SLL_EXTERNAL void sll_platform_remove_environment_variable(const sll_char_t* k);



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
 * \name sll_platform_set_environment_variable
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg const sll_char_t* k
 * \arg const sll_char_t* v
 */
__SLL_EXTERNAL void sll_platform_set_environment_variable(const sll_char_t* k,const sll_char_t* v);



/**
 * \flags func
 * \name sll_platform_sleep
 * \group platform
 * \desc Docs!
 * \arg sll_time_t tm
 */
__SLL_EXTERNAL void sll_platform_sleep(sll_time_t tm);



/**
 * \flags check_output func
 * \name sll_platform_start_process
 * \group platform
 * \subgroup platform-process
 * \desc Docs!
 * \arg const sll_char_t*const* a
 * \arg const sll_char_t*const* env
 * \ret sll_process_handle_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_process_handle_t sll_platform_start_process(const sll_char_t*const* a,const sll_char_t*const* env);



/**
 * \flags func
 * \name sll_platform_unload_library
 * \group platform
 * \subgroup platform-lib
 * \desc Docs!
 * \arg sll_library_handle_t h
 */
__SLL_EXTERNAL void sll_platform_unload_library(sll_library_handle_t h);



/**
 * \flags check_output func
 * \name sll_platform_wait_process
 * \group platform
 * \subgroup platform-process
 * \desc Docs!
 * \arg sll_process_handle_t ph
 * \ret sll_return_code_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_code_t sll_platform_wait_process(sll_process_handle_t ph);



#endif
