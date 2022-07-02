#undef NOGDI
#include <sll.h>
#include <window/common.h>
#include <window/display.h>
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
		sll_float_t rotation=0;
		if (settings.dmDisplayOrientation==DMDO_90){
			rotation=WINDOW_MATH_PI/2;
		}
		else if (settings.dmDisplayOrientation==DMDO_180){
			rotation=WINDOW_MATH_PI;
		}
		else if (settings.dmDisplayOrientation==DMDO_270){
			rotation=WINDOW_MATH_PI*3/2;
		}
		sll_integer_t flags=0;
		if (device.StateFlags&DISPLAY_DEVICE_ACTIVE){
			flags|=WINDOW_DISPLAY_FLAG_ACTIVE;
		}
		if (!settings.dmPosition.x&&!settings.dmPosition.y){
			flags|=WINDOW_DISPLAY_FLAG_MAIN_DISPLAY;
		}
		sll_object_t* display=sll_new_object(SLL_CHAR("uhhuufi"),id,settings.dmPosition.x,settings.dmPosition.y,settings.dmPelsWidth,settings.dmPelsHeight,rotation,flags);
		sll_array_push(NULL,display,out);
		SLL_RELEASE(display);
_next_display:
		id++;
	}
}
