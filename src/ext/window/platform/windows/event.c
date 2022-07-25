#undef NOMSG
#undef NOCOLOR
#undef NOUSER
#undef NOWINMESSAGES
#include <windows.h>
#include <sll.h>
#include <window/common.h>
#include <window/event.h>



static sll_array_t _events=SLL_INIT_ARRAY_STRUCT;



unsigned __int64 _window_wnd_proc(void* id,unsigned int msg,unsigned __int64 w_param,__int64 l_param){
	sll_object_t arg=NULL;
	unsigned __int64 ret=0;
	switch (msg){
		case WM_CLOSE:
			arg=sll_new_object(SLL_CHAR("uu"),WINDOW_EVENT_CLOSE,id);
			break;
		case WM_KEYDOWN:
			SLL_LOG("WM_KEYDOWN");
			return 0;
		case WM_KEYUP:
			SLL_LOG("WM_KEYUP");
			return 0;
		case WM_KILLFOCUS:
			SLL_LOG("WM_KILLFOCUS");
			return 0;
		case WM_LBUTTONDOWN:
			SLL_LOG("WM_LBUTTONDOWN");
			return 0;
		case WM_LBUTTONUP:
			SLL_LOG("WM_LBUTTONUP");
			return 0;
		case WM_MBUTTONDOWN:
			SLL_LOG("WM_MBUTTONDOWN");
			return 0;
		case WM_MBUTTONUP:
			SLL_LOG("WM_MBUTTONUP");
			return 0;
		case WM_MOUSEHWHEEL:
			SLL_LOG("WM_MOUSEHWHEEL");
			return 0;
		case WM_MOUSELEAVE:
			SLL_LOG("WM_MOUSELEAVE");
			return 0;
		case WM_MOUSEMOVE:
			SLL_LOG("WM_MOUSEMOVE");
			return 0;
		case WM_MOUSEWHEEL:
			SLL_LOG("WM_MOUSEWHEEL");
			return 0;
		case WM_MOVE:
			SLL_LOG("WM_MOVE");
			return 0;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc=BeginPaint(id,&ps);
				FillRect(hdc,&ps.rcPaint,(HBRUSH)COLOR_WINDOW);
				EndPaint(id,&ps);
				return 0;
			}
		case WM_RBUTTONDOWN:
			SLL_LOG("WM_RBUTTONDOWN");
			return 0;
		case WM_RBUTTONUP:
			SLL_LOG("WM_RBUTTONUP");
			return 0;
		case WM_SETFOCUS:
			SLL_LOG("WM_SETFOCUS");
			return 0;
		case WM_SIZE:
			SLL_LOG("WM_SIZE");
			return 0;
		case WM_SIZING:
			SLL_LOG("WM_SIZING");
			return 0;
		case WM_SYSKEYDOWN:
			SLL_LOG("WM_SYSKEYDOWN");
			return 0;
		case WM_SYSKEYUP:
			SLL_LOG("WM_SYSKEYUP");
			return 0;
		case WM_XBUTTONDOWN:
			SLL_LOG("WM_XBUTTONDOWN");
			return 0;
		case WM_XBUTTONUP:
			SLL_LOG("WM_XBUTTONUP");
			return 0;
	}
	if (arg){
		sll_array_push(NULL,arg,&_events);
		SLL_RELEASE(arg);
		return ret;
	}
	return DefWindowProcA(id,msg,w_param,l_param);
}



__WINDOW_API_CALL void window_api_event_poll(sll_bool_t blocking,sll_array_t* out){
	MSG msg;
	if (!blocking&&!PeekMessageA(&msg,NULL,0,0,PM_NOREMOVE)){
		SLL_INIT_ARRAY(out);
		return;
	}
	GetMessageA(&msg,NULL,0,0);
	do{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	} while (PeekMessageA(&msg,NULL,0,0,PM_REMOVE));
	*out=_events;
	SLL_INIT_ARRAY(&_events);
}
