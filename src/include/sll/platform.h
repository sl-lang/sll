#ifndef __SLL_PLATFORM_H__
#define __SLL_PLATFORM_H__ 1
#include <sll/common.h>
#include <sll/types.h>



extern const char* sll_platform_string;



__SLL_FUNC void* sll_platform_allocate_page(sll_page_size_t sz);



__SLL_FUNC void sll_platform_free_page(void* pg,sll_page_size_t sz);



__SLL_FUNC sll_time_t sll_platform_get_current_time(void);



__SLL_FUNC sll_page_size_t sll_platform_get_page_size(void);



__SLL_FUNC sll_buffer_size_t sll_platform_path_absolute(const char* fp,sll_buffer_t bf,sll_buffer_size_t bfl);



__SLL_FUNC sll_bool_t sll_platform_path_exists(const char* fp);



__SLL_FUNC sll_bool_t sll_platform_path_is_directory(const char* fp);



__SLL_FUNC void sll_platform_sleep(sll_time_t tm);



#endif
