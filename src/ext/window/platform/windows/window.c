#include <windows.h>
#include <sll.h>
#include <stdint.h>
#include <window/common.h>



__WINDOW_API_CALL void* window_api_window_create(int32_t x,int32_t y,uint32_t w,uint32_t h,void* parent){
	return CreateWindowExA(0,"_sll_window_extension_default_class","",WS_OVERLAPPEDWINDOW,x,y,w,h,(parent==(void*)(0xffffffffffffffffull)?NULL:parent),NULL,_winapi_hinstance,NULL);
}



__WINDOW_API_CALL void window_api_window_set_visibility(void* id,sll_bool_t show){
	ShowWindow(id,(show?SW_SHOWNORMAL:SW_HIDE));
}
