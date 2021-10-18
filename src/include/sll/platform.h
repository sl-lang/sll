#ifndef __SLL_PLATFORM_H__
#define __SLL_PLATFORM_H__ 1
#include <sll/common.h>
#include <sll/types.h>



extern const sll_char_t* sll_platform_string;



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_platform_allocate_page(sll_page_size_t sz);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_integer_t sll_platform_file_size(const sll_char_t* fp);



__SLL_FUNC void sll_platform_free_page(void* pg,sll_page_size_t sz);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_time_t sll_platform_get_current_time(void);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_page_size_t sll_platform_get_page_size(void);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_array_length_t sll_platform_list_directory(const sll_char_t* fp,sll_string_t** o);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_array_length_t sll_platform_list_directory_recursive(const sll_char_t* fp,sll_string_t** o);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_buffer_size_t sll_platform_path_absolute(const sll_char_t* fp,sll_buffer_t bf,sll_buffer_size_t bfl);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_exists(const sll_char_t* fp);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_is_directory(const sll_char_t* fp);



__SLL_FUNC void sll_platform_setup_console(void);



__SLL_FUNC void sll_platform_sleep(sll_time_t tm);



__SLL_FUNC void sll_platform_socket_init(void);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_return_t sll_platform_socket_execute(const sll_string_t* h,unsigned int p,const sll_string_t* in,sll_string_t* o);



__SLL_FUNC void sll_platform_reset_console(void);



#endif
