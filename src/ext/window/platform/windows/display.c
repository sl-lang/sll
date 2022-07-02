#include <sll.h>
#include <window/common.h>
#include <windows.h>



__WINDOW_API_CALL void window_api_display_enumerate(sll_array_t* out){
	SLL_INIT_ARRAY(out);
	DWORD id=0;
	while (1){
		DISPLAY_DEVICEA device={
			sizeof(DISPLAY_DEVICEA)
		};
		if (!EnumDisplayDevicesA(NULL,id,&device,0)){
			break;
		}
		DEVMODEA settings={
			sizeof(DEVMODEA)
		};
		if (!EnumDisplaySettingsA(device.DeviceName,ENUM_CURRENT_SETTINGS,&settings)){
			goto _next_display;
		}
		sll_array_push(NULL,SLL_ACQUIRE_STATIC_INT(0),out);
_next_display:
		id++;
	}
}
