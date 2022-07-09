#undef NOMSG
#undef NOUSER
#include <windows.h>
#include <sll.h>
#include <window/common.h>



unsigned __int64 _window_wnd_proc(void* id,unsigned int msg,unsigned __int64 w_param,__int64 l_param){
	return DefWindowProcA(id,msg,w_param,l_param);
}



__WINDOW_API_CALL void window_api_event_poll(sll_bool_t blocking,sll_array_t* out){
	SLL_INIT_ARRAY(out);
	MSG msg;
	while (PeekMessageA(&msg,NULL,0,0,PM_REMOVE)){
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
}
