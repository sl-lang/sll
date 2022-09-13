#include <windows.h>
#include <setupapi.h>
#include <initguid.h>
#include <hidclass.h>
#include <hidsdi.h>
#include <ds4/common.h>
#include <sll.h>



#define SONY_VENDOR_ID 0x054c
#define DS4_PRODUT_ID1 0x054c
#define DS4_PRODUT_ID2 0x09cc



__DS4_API_CALL void ds4_api_device_list(sll_array_t* out){
	SLL_INIT_ARRAY(out);
	GUID guid=GUID_DEVINTERFACE_HID;
	HDEVINFO dev_list=SetupDiGetClassDevsA(&guid,NULL,NULL,DIGCF_PRESENT|DIGCF_DEVICEINTERFACE);
	uint32_t dev_list_idx=0;
	while (1){
		SP_DEVICE_INTERFACE_DATA dev_interface={
			sizeof(SP_DEVICE_INTERFACE_DATA)
		};
		if (!SetupDiEnumDeviceInterfaces(dev_list,NULL,&guid,dev_list_idx,&dev_interface)){
			break;
		}
		dev_list_idx++;
		DWORD size;
		if (!SetupDiGetDeviceInterfaceDetailA(dev_list,&dev_interface,NULL,0,&size,NULL)&&GetLastError()!=ERROR_INSUFFICIENT_BUFFER){
			continue;
		}
		SP_DEVICE_INTERFACE_DETAIL_DATA_A dev={
			sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A)
		};
 		if (!SetupDiGetDeviceInterfaceDetailA(dev_list,&dev_interface,&dev,size,NULL,NULL)){
			continue;
		}
		DWORD j=0;
		do{
			SP_DEVINFO_DATA dev_info={
				sizeof(SP_DEVINFO_DATA)
			};
			if (!SetupDiEnumDeviceInfo(dev_list,j,&dev_info)){
				goto _check_next_device;
			}
			j++;
			char dev_name[MAX_PATH];
			if (!SetupDiGetDeviceRegistryPropertyA(dev_list,&dev_info,SPDRP_CLASS,NULL,(PBYTE)dev_name,MAX_PATH,NULL)||sll_string_compare_pointer(SLL_CHAR(dev_name),SLL_CHAR("HIDClass"))){
				continue;
			}
		} while (!SetupDiGetDeviceRegistryPropertyA(dev_list,&dev_info,SPDRP_DRIVER,NULL,(PBYTE)dev_name,MAX_PATH,NULL));
		HANDLE fh=CreateFileA(dev.DevicePath,0,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,0);
		if (fh==INVALID_HANDLE_VALUE){
			continue;
		}
		HIDD_ATTRIBUTES attributes={
			sizeof(HIDD_ATTRIBUTES)
		};
		if (HidD_GetAttributes(fh,&attributes)&&attributes.VendorID==SONY_VENDOR_ID&&(attributes.ProductID==DS4_PRODUT_ID1||attributes.ProductID==DS4_PRODUT_ID2)){
			sll_array_push(NULL,sll_string_pointer_to_object(SLL_CHAR(dev.DevicePath)),out);
		}
		CloseHandle(fh);
_check_next_device:;
	}
	SetupDiDestroyDeviceInfoList(dev_list);
}



__DS4_EXTERNAL sll_bool_t SLL_ABI_INIT(sll_version_t version){
	return (version==SLL_VERSION);
}
