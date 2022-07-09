#undef NOMSG
#undef NOSHOWWINDOW
#undef NOUSER
#undef NOWINSTYLES
#include <windows.h>
#include <sll.h>
#include <stdint.h>
#include <window/common.h>
#include <window/platform/windows.h>
#include <window/window.h>



unsigned __int64 _window_wnd_proc(void* id,unsigned int msg,unsigned __int64 w_param,__int64 l_param){
	return DefWindowProcA(id,msg,w_param,l_param);
}



__WINDOW_API_CALL window_handle_t window_api_window_create(int32_t x,int32_t y,uint32_t w,uint32_t h,window_handle_t parent){
	return CreateWindowExA(0,"_sll_window_extension_default_class","",WS_OVERLAPPEDWINDOW,x,y,w,h,(parent==(window_handle_t)(0xffffffffffffffffull)?NULL:parent),NULL,_winapi_hinstance,NULL);
}



__WINDOW_API_CALL void window_api_window_destroy(window_handle_t id){
	DestroyWindow(id);
}



__WINDOW_API_CALL void window_api_window_poll_events(sll_bool_t blocking,sll_array_t* out){
	SLL_INIT_ARRAY(out);
	MSG msg;
	while (PeekMessageA(&msg,NULL,0,0,PM_REMOVE)){
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
}



__WINDOW_API_CALL void window_api_window_set_geometry(window_handle_t id,int32_t x,int32_t y,uint32_t w,uint32_t h){
	MoveWindow(id,x,y,w,h,TRUE);
}



__WINDOW_API_CALL void window_api_window_set_size_constraints(window_handle_t id,uint32_t min_w,uint32_t min_h,uint32_t max_w,uint32_t max_h){
}



__WINDOW_API_CALL void window_api_window_set_state(window_handle_t id,sll_char_t state){
}



__WINDOW_API_CALL void window_api_window_set_title(window_handle_t id,const sll_string_t* name){
	SetWindowTextA(id,name->data);
}



__WINDOW_API_CALL void window_api_window_set_visibility(window_handle_t id,sll_bool_t show){
	ShowWindow(id,(show?SW_SHOWNORMAL:SW_HIDE));
}
