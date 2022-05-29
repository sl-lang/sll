#include <clib/common.h>
#include <sll.h>
#include <stddef.h>



__CLIB_API_CALL sll_library_handle_t clib_api_get_sll_library_handle(void){
	return sll_platform_load_library(NULL,NULL);
}
