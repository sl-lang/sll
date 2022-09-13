#include <ds4/common.h>
#include <libudev.h>
#include <sll.h>



static struct udev* _udev_ctx;



__DS4_API_CALL void ds4_api_device_list(sll_array_t* out){
	SLL_INIT_ARRAY(out);
	struct udev_enumerate* dev_list=udev_enumerate_new(_udev_ctx);
	udev_enumerate_add_match_subsystem(dev_list,"hidraw");
	udev_enumerate_scan_devices(dev_list);
	struct udev_list_entry* entry=udev_enumerate_get_list_entry(dev_list);
	while (entry){
		struct udev_device* dev=udev_device_new_from_syspath(_udev_ctx,udev_list_entry_get_name(entry));
		struct udev_device* parent=udev_device_get_parent(dev);
		const sll_char_t* name=SLL_CHAR(udev_device_get_property_value(parent,"HID_NAME"));
		const char* path=udev_device_get_devnode(dev);
		if (parent&&name&&path&&sll_string_compare_pointer(SLL_CHAR(udev_device_get_subsystem(parent)),SLL_CHAR("hid"))==SLL_COMPARE_RESULT_EQUAL&&(sll_string_compare_pointer(name,SLL_CHAR("Sony Interactive Entertainment Wireless Controller"))==SLL_COMPARE_RESULT_EQUAL||sll_string_compare_pointer(name,SLL_CHAR("Sony Computer Entertainment Wireless Controller"))==SLL_COMPARE_RESULT_EQUAL)){
			sll_array_push(NULL,sll_string_pointer_to_object(SLL_CHAR(path)),out);
		}
		udev_device_unref(dev);
		entry=udev_list_entry_get_next(entry);
	}
	udev_enumerate_unref(dev_list);
}



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
