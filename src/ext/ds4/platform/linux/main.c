#include <ds4/common.h>
#include <libudev.h>
#include <sll.h>



static struct udev* _udev_ctx;



__DS4_EXTERNAL sll_bool_t SLL_ABI_INIT(sll_version_t version){
	if (version!=SLL_VERSION){
		return 0;
	}
	_udev_ctx=udev_new();
	return 1;
}



__DS4_EXTERNAL void SLL_ABI_DEINIT(void){
	udev_unref(_udev_ctx);
}
