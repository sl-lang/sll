#undef NOMSG
#undef NOCOLOR
#undef NOUSER
#undef NOWINMESSAGES
#include <windows.h>
#include <sll.h>
#include <window/common.h>
#include <window/event.h>
#include <window/keyboard.h>



static sll_array_t _events=SLL_INIT_ARRAY_STRUCT;



unsigned __int64 _window_wnd_proc(void* id,unsigned int msg,unsigned __int64 w_param,__int64 l_param){
	sll_object_t arg=NULL;
	switch (msg){
		case WM_CLOSE:
			arg=sll_new_object(SLL_CHAR("uu"),WINDOW_EVENT_CLOSE,id);
			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			arg=sll_new_object(SLL_CHAR("uuuu"),WINDOW_EVENT_KEY,id,_scancode_to_keycode[(HIWORD(l_param)&(KF_EXTENDED|0xff))],!(HIWORD(l_param)&KF_UP));
			break;
		case WM_KILLFOCUS:
		case WM_SETFOCUS:
			arg=sll_new_object(SLL_CHAR("uuu"),WINDOW_EVENT_FOCUS,id,(msg==WM_SETFOCUS));
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
			{
				unsigned int index=0;
				unsigned int state=0;
				switch (msg){
					case WM_LBUTTONDOWN:
						state=1;
						break;
					case WM_MBUTTONDOWN:
						state=1;
					case WM_MBUTTONUP:
						index=1;
						break;
					case WM_RBUTTONDOWN:
						state=1;
					case WM_RBUTTONUP:
						index=2;
						break;
					case WM_XBUTTONDOWN:
						state=1;
					case WM_XBUTTONUP:
						index=3+(HIWORD(w_param)==XBUTTON2);
						break;
				}
				arg=sll_new_object(SLL_CHAR("uuuu"),WINDOW_EVENT_BUTTON,id,index,state);
				break;
			}
		case WM_MOUSEHWHEEL:
		case WM_MOUSEWHEEL:
			arg=sll_new_object(SLL_CHAR("uuuh"),WINDOW_EVENT_SCROLL,id,(msg==WM_MOUSEWHEEL),GET_WHEEL_DELTA_WPARAM(w_param));
			break;
		case WM_MOUSELEAVE:
			arg=sll_new_object(SLL_CHAR("uu"),WINDOW_EVENT_MOUSE_LEAVE,id);
			break;
		case WM_MOUSEMOVE:
		case WM_MOVE:
			arg=sll_new_object(SLL_CHAR("uuhh"),(msg==WM_MOUSEMOVE?WINDOW_EVENT_MOUSE:WINDOW_EVENT_POSITION),id,(signed short)LOWORD(l_param),(signed short)HIWORD(l_param));
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				FillRect(BeginPaint(id,&ps),&(ps.rcPaint),(HBRUSH)COLOR_WINDOW);
				EndPaint(id,&ps);
				break;
			}
		case WM_SIZE:
			arg=sll_new_object(SLL_CHAR("uuuu"),WINDOW_EVENT_SIZE,id,LOWORD(l_param),HIWORD(l_param));
			break;
	}
	if (arg){
		sll_array_push(NULL,arg,&_events);
		SLL_RELEASE(arg);
		return 0;
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
