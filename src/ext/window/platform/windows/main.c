#include <windows.h>



HINSTANCE _winapi_hinstance;



void _deinit_platform(void){
}



void _init_platform(void){
	_winapi_hinstance=GetModuleHandle(NULL);
}
