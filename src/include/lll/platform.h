#ifndef __LLL_PLATFORM_H__
#define __LLL_PLATFORM_H__ 1
#include <lll/common.h>
#include <lll/types.h>



extern const char* lll_platform_string;



__LLL_FUNC void* lll_platform_allocate_page(lll_page_size_t sz);



__LLL_FUNC void lll_platform_free_page(void* pg,lll_page_size_t sz);



__LLL_FUNC lll_time_t lll_platform_get_current_time(void);



__LLL_FUNC lll_page_size_t lll_platform_get_page_size(void);



__LLL_FUNC lll_buffer_size_t lll_platform_path_absolute(const char* fp,lll_buffer_t bf,lll_buffer_size_t bfl);



__LLL_FUNC lll_bool_t lll_platform_path_exists(const char* fp);



__LLL_FUNC lll_bool_t lll_platform_path_is_directory(const char* fp);



__LLL_FUNC void lll_platform_setup_console(void);



#endif
