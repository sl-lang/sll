#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"mswsock.lib")
#pragma comment(lib,"advapi32.lib")



const sll_char_t* sll_platform_string=SLL_CHAR("windows");



static HANDLE _win_wh=INVALID_HANDLE_VALUE;
static int _win_stdout_m=0;
static int _win_stderr_m=0;



static void _list_dir_files(sll_char_t* bf,sll_string_length_t i,file_list_data_t* o){
	WIN32_FIND_DATAA dt;
	bf[i]='*';
	bf[i+1]=0;
	HANDLE fh=FindFirstFileA(bf,&dt);
	if (fh!=INVALID_HANDLE_VALUE){
		do{
			if (dt.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){
				if (*(dt.cFileName)=='.'&&(*(dt.cFileName+1)==0||(*(dt.cFileName+1)=='.'&&*(dt.cFileName+2)==0))){
					continue;
				}
				SLL_UNIMPLEMENTED();
			}
			else{
				sll_string_length_t j=sll_string_length_unaligned(SLL_CHAR(dt.cFileName));
				o->l++;
				o->dt=realloc(o->dt,o->l*sizeof(sll_string_t));
				sll_string_t* s=o->dt+o->l-1;
				sll_string_create(i+j,s);
				memcpy(s->v,bf,i);
				memcpy(s->v+i,dt.cFileName,j);
				sll_string_hash(s);
			}
		} while (FindNextFileA(fh,&dt));
		FindClose(fh);
	}
}



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_platform_allocate_page(sll_page_size_t sz){
	return VirtualAlloc(NULL,sz,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_integer_t sll_platform_file_size(const sll_char_t* fp){
	HANDLE fh=CreateFileA(fp,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (fh==INVALID_HANDLE_VALUE){
		return 0;
	}
	LARGE_INTEGER sz;
	if (!GetFileSizeEx(fh,&sz)){
		CloseHandle(fh);
		return 0;
	}
	CloseHandle(fh);
	return sz.QuadPart;
}



__SLL_FUNC void sll_platform_free_page(void* pg,sll_page_size_t sz){
	VirtualFree(pg,0,MEM_RELEASE);
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_time_t sll_platform_get_current_time(void){
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return ((((sll_time_t)ft.dwHighDateTime)<<32)|ft.dwLowDateTime)*100-11644473600000000000;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_page_size_t sll_platform_get_page_size(void){
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwPageSize;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_array_length_t sll_platform_list_directory_recursive(const sll_char_t* fp,sll_string_t** o){
	sll_char_t bf[MAX_PATH+1];
	sll_string_length_t i=sll_string_length_unaligned(fp);
	memcpy(bf,fp,i);
	file_list_data_t dt={
		NULL,
		0
	};
	_list_dir_files(bf,i,&dt);
	*o=dt.dt;
	return dt.l;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_buffer_size_t sll_platform_path_absolute(const sll_char_t* fp,sll_buffer_t bf,sll_buffer_size_t bfl){
	return (sll_buffer_size_t)GetFullPathNameA(fp,bfl,bf,NULL);
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_exists(const sll_char_t* fp){
	return (GetFileAttributesA(fp)!=INVALID_FILE_ATTRIBUTES);
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_is_directory(const sll_char_t* fp){
	DWORD a=GetFileAttributesA(fp);
	return (a!=INVALID_FILE_ATTRIBUTES&&(a&FILE_ATTRIBUTE_DIRECTORY));
}



__SLL_FUNC void sll_platform_setup_console(void){
	_win_stdout_m=_setmode(_fileno(stdout),_O_BINARY);
	_win_stderr_m=_setmode(_fileno(stderr),_O_BINARY);
}



__SLL_FUNC void sll_platform_sleep(sll_time_t tm){
	if (_win_wh==INVALID_HANDLE_VALUE){
		_win_wh=CreateEvent(NULL,TRUE,FALSE,FALSE);
	}
	sll_time_t e=GetTickCount64()*1000000+tm;
	while (1){
		ResetEvent(_win_wh);
		if (WaitForSingleObjectEx(_win_wh,(DWORD)((tm+999999)/1000000),FALSE)!=WAIT_OBJECT_0){
			return;
		}
		sll_time_t c=GetTickCount64()*1000000;
		if (c>=e){
			break;
		}
		tm=c-e;
	}
}



__SLL_FUNC void sll_platform_socket_init(void){
	WSADATA dt;
	if (WSAStartup(MAKEWORD(2,2),&dt)){
		SLL_UNIMPLEMENTED();
	}
	sll_register_cleanup(WSACleanup,SLL_CLEANUP_ORDER_NORMAL);
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_return_t sll_platform_socket_execute(const sll_string_t* h,unsigned int p,const sll_string_t* in,sll_string_t* o){
	SLL_ZERO_STRING(o);
	struct addrinfo ah;
	memset(&ah,0,sizeof(struct addrinfo));
	ah.ai_family=AF_UNSPEC;
	ah.ai_socktype=SOCK_STREAM;
	ah.ai_protocol=IPPROTO_TCP;
	struct addrinfo* r=NULL;
	char ps[6]={0,0,0,0,0,0};
	uint8_t i=4;
	do{
		ps[i]=(p%10)+48;
		i--;
		p/=10;
	} while (p);
	if (getaddrinfo(h->v,ps+i+1,&ah,&r)){
		return SLL_RETURN_ERROR;
	}
	SOCKET s=INVALID_SOCKET;
	for (struct addrinfo* k=r;k;k=k->ai_next){
		s=socket(k->ai_family,k->ai_socktype,k->ai_protocol);
		if (s==INVALID_SOCKET){
			freeaddrinfo(r);
			return SLL_RETURN_ERROR;
		}
		if (connect(s,k->ai_addr,(int)k->ai_addrlen)!=SOCKET_ERROR){
			break;
		}
		s=INVALID_SOCKET;
	}
	freeaddrinfo(r);
	if (s==INVALID_SOCKET){
		return SLL_RETURN_ERROR;
	}
	if (send(s,in->v,in->l,0)==SOCKET_ERROR){
		closesocket(s);
		return SLL_RETURN_ERROR;
	}
	o->l=0;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(0)*sizeof(sll_char_t));
	sll_char_t bf[4096];
	int l=recv(s,bf,4096,0);
	shutdown(s,SD_SEND);
	while (l){
		if (l<0){
			closesocket(s);
			return SLL_RETURN_ERROR;
		}
		o->v=realloc(o->v,SLL_STRING_ALIGN_LENGTH(o->l+l)*sizeof(sll_char_t));
		memcpy(o->v+o->l,bf,l);
		o->l+=l;
		l=recv(s,bf,4096,0);
	};
	closesocket(s);
	o->v[o->l]=0;
	sll_string_hash(o);
	return SLL_RETURN_NO_ERROR;
}



__SLL_FUNC void sll_platform_reset_console(void){
	_setmode(_fileno(stdout),_win_stdout_m);
	_setmode(_fileno(stderr),_win_stderr_m);
}
