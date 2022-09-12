#include <ds4/common.h>
#include <sll.h>
#include <windows.h>



__DS4_API_CALL void ds4_api_device_list(sll_array_t* out){
	SLL_INIT_ARRAY(out);
}



__DS4_EXTERNAL sll_bool_t SLL_ABI_INIT(sll_version_t version){
	return (version==SLL_VERSION);
}
