#ifndef __SLL_PLATFORM_H__
#define __SLL_PLATFORM_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_PLATFORM_STREAM_INPUT 0
#define SLL_PLATFORM_STREAM_OUTPUT 1
#define SLL_PLATFORM_STREAM_ERROR 2



__SLL_EXTERNAL extern const sll_char_t* sll_platform_string;



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_platform_allocate_page(sll_page_size_t sz);



__SLL_EXTERNAL void sll_platform_enable_console_color(void);



__SLL_EXTERNAL void sll_platform_file_close(sll_file_descriptor_t fd);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_descriptor_t sll_platform_file_open(const sll_char_t* fp,sll_file_flags_t ff);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_file_read(sll_file_descriptor_t fd,void* p,sll_size_t sz);



__SLL_EXTERNAL void sll_platform_file_seek(sll_file_descriptor_t fd,sll_file_offset_t off);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_platform_file_size(const sll_char_t* fp);



__SLL_EXTERNAL sll_size_t sll_platform_file_write(sll_file_descriptor_t fd,const void* p,sll_size_t sz);



__SLL_EXTERNAL __SLL_NO_RETURN void sll_platform_force_exit(const sll_char_t* a,const sll_char_t* b,const sll_char_t* c);



__SLL_EXTERNAL void sll_platform_free_page(void* pg,sll_page_size_t sz);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_time_t sll_platform_get_current_time(void);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_platform_get_current_working_directory(sll_char_t* o,sll_string_length_t ol);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_descriptor_t sll_platform_get_default_stream_descriptor(sll_char_t t);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_platform_get_executable_file_path(sll_char_t* o,sll_string_length_t ol);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_page_size_t sll_platform_get_page_size(void);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_platform_list_directory(const sll_char_t* fp,sll_string_t** o);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_platform_list_directory_recursive(const sll_char_t* fp,sll_string_t** o);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_exists(const sll_char_t* fp);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_is_directory(const sll_char_t* fp);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_set_current_working_directory(const sll_char_t* p);



__SLL_EXTERNAL void sll_platform_setup_console(void);



__SLL_EXTERNAL void sll_platform_sleep(sll_time_t tm);



__SLL_EXTERNAL void sll_platform_socket_init(void);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_socket_execute(const sll_string_t* h,unsigned int p,const sll_string_t* in,sll_string_t* o);



__SLL_EXTERNAL void sll_platform_reset_console(void);



#endif
