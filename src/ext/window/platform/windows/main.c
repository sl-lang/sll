#undef NOUSER
#include <windows.h>



HINSTANCE _winapi_hinstance;



void _deinit_platform(void){
	UnregisterClassA("_sll_window_extension_default_class",_winapi_hinstance);
}



void _init_platform(void){
	_winapi_hinstance=GetModuleHandle(NULL);
	WNDCLASSA wnd_class={
		0,
		DefWindowProcA,
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
