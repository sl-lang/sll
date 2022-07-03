#undef NOSHOWWINDOW
#undef NOUSER
#undef NOWINSTYLES
#include <windows.h>
#include <sll.h>
#include <stdint.h>
#include <window/common.h>



unsigned __int64 _window_wnd_proc(void* id,unsigned int msg,unsigned __int64 w_param,__int64 l_param){
	return DefWindowProcA(id,msg,w_param,l_param);
}



__WINDOW_API_CALL void* window_api_window_create(int32_t x,int32_t y,uint32_t w,uint32_t h,void* parent){
	return CreateWindowExA(0,"_sll_window_extension_default_class","",WS_OVERLAPPEDWINDOW,x,y,w,h,(parent==(void*)(0xffffffffffffffffull)?NULL:parent),NULL,_winapi_hinstance,NULL);
}



__WINDOW_API_CALL void window_api_window_destroy(void* id){
	DestroyWindow(id);
}



__WINDOW_API_CALL void window_api_window_poll_events(void){
	MSG msg;
	while (PeekMessageW(&msg,NULL,0,0,PM_REMOVE)){
		if (msg.message==WM_QUIT){
			//
		}
		else{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
}



__WINDOW_API_CALL void window_api_window_set_title(void* id,const sll_string_t* name){
	SetWindowTextA(id,name->data);
}



__WINDOW_API_CALL void window_api_window_set_visibility(void* id,sll_bool_t show){
	ShowWindow(id,(show?SW_SHOWNORMAL:SW_HIDE));
}
