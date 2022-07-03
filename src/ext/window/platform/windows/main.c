#undef NOUSER
#include <windows.h>
#include <window/window.h>



void* _winapi_hinstance;



void _deinit_platform(void){
	UnregisterClassA("_sll_window_extension_default_class",_winapi_hinstance);
}



void _init_platform(void){
	_winapi_hinstance=GetModuleHandle(NULL);
	WNDCLASSA wnd_class={
		0,
		_window_wnd_proc,
		0,
		0,
		_winapi_hinstance,
		NULL,
		LoadCursorA(_winapi_hinstance,MAKEINTRESOURCE(IDC_ARROW)),
		NULL,
		NULL,
		"_sll_window_extension_default_class"
	};
	RegisterClassA(&wnd_class);
}
