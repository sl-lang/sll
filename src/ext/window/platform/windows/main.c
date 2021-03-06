#undef NOUSER
#include <windows.h>
#include <stddef.h>
#include <window/common.h>
#include <window/platform/windows.h>
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
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
}



__WINDOW_API_CALL void* window_api_get_context(void){
	return NULL;
}
