#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/memory.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <fcntl.h>
#include <io.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"mswsock.lib")
#pragma comment(lib,"advapi32.lib")



__SLL_EXTERNAL const sll_char_t* sll_platform_string=SLL_CHAR("windows");



static HANDLE _win_wh=INVALID_HANDLE_VALUE;
static int _win_stdout_m=0;
static int _win_stderr_m=0;
static DWORD _win_stdout_cm=0xffffffff;
static DWORD _win_stderr_cm=0xffffffff;



static void _reset_console_color(void){
	if (_win_stdout_cm!=0xffffffff){
		SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE),_win_stdout_cm);
		_win_stdout_cm=0xffffffff;
	}
	if (_win_stderr_cm!=0xffffffff){
		SetConsoleMode(GetStdHandle(STD_ERROR_HANDLE),_win_stderr_cm);
		_win_stderr_cm=0xffffffff;
	}
}



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
				sll_string_length_t j=sll_string_length_unaligned(SLL_CHAR(dt.cFileName));
				sll_copy_data(dt.cFileName,j,bf+i);
				bf[i+j]='\\';
				_list_dir_files(bf,i+j+1,o);
			}
			else{
				sll_string_length_t j=sll_string_length_unaligned(SLL_CHAR(dt.cFileName));
				if (!(o->l&7)){
					o->dt=sll_reallocate(o->dt,(o->l+8)*sizeof(sll_string_t));
					SLL_CHECK_NO_MEMORY(o->dt);
				}
				sll_string_t* s=o->dt+o->l;
				o->l++;
				sll_string_create(i+j,s);
				sll_string_insert_pointer_length(bf,i,0,s);
				sll_string_insert_pointer_length(dt.cFileName,j,i,s);
			}
		} while (FindNextFileA(fh,&dt));
		FindClose(fh);
	}
}



__SLL_NO_RETURN void _force_exit_platform(void){
	TerminateProcess(GetCurrentProcess(),SIGABRT);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_platform_allocate_page(sll_page_size_t sz){
	return VirtualAlloc(NULL,sz,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
}



__SLL_EXTERNAL void sll_platform_enable_console_color(void){
	sll_bool_t st=0;
	if (_win_stdout_cm==0xffffffff&&_isatty(1)){
		GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE),&_win_stdout_cm);
		SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE),_win_stdout_cm|ENABLE_PROCESSED_OUTPUT|ENABLE_WRAP_AT_EOL_OUTPUT|ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		st=1;
	}
	if (_win_stderr_cm==0xffffffff&&_isatty(2)){
		GetConsoleMode(GetStdHandle(STD_ERROR_HANDLE),&_win_stderr_cm);
		SetConsoleMode(GetStdHandle(STD_ERROR_HANDLE),_win_stderr_cm|ENABLE_PROCESSED_OUTPUT|ENABLE_WRAP_AT_EOL_OUTPUT|ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		st=1;
	}
	if (st){
		sll_register_cleanup(_reset_console_color);
	}
}



__SLL_EXTERNAL void sll_platform_file_close(sll_file_descriptor_t fd){
	CloseHandle((HANDLE)fd);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_descriptor_t sll_platform_file_open(const sll_char_t* fp,sll_file_flags_t ff){
	DWORD m=0;
	DWORD cm=OPEN_EXISTING;
	if (ff&SLL_FILE_FLAG_READ){
		m|=GENERIC_READ;
	}
	if (ff&SLL_FILE_FLAG_WRITE){
		m|=GENERIC_WRITE;
		cm=CREATE_ALWAYS;
	}
	if (ff&SLL_FILE_FLAG_APPEND){
		m|=FILE_APPEND_DATA;
		cm=OPEN_ALWAYS;
	}
	HANDLE o=CreateFileA((char*)fp,m,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,cm,FILE_ATTRIBUTE_NORMAL,NULL);
	if (o==INVALID_HANDLE_VALUE){
		return SLL_UNKNOWN_FILE_DESCRIPTOR;
	}
	return (sll_file_descriptor_t)o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_file_read(sll_file_descriptor_t fd,void* p,sll_size_t sz){
	DWORD o;
	if (!ReadFile((HANDLE)fd,p,(DWORD)sz,&o,NULL)){
		return 0;
	}
	return o;
}



__SLL_EXTERNAL void sll_platform_file_seek(sll_file_descriptor_t fd,sll_file_offset_t off){
	DWORD h=off>>32;
	SetFilePointer((HANDLE)fd,(DWORD)(off&0xffffffff),&h,FILE_BEGIN);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_file_size(sll_file_descriptor_t fd){
	LARGE_INTEGER sz;
	if (!GetFileSizeEx((HANDLE)fd,&sz)){
		return 0;
	}
	return sz.QuadPart;
}



__SLL_EXTERNAL sll_size_t sll_platform_file_write(sll_file_descriptor_t fd,const void* p,sll_size_t sz){
	DWORD o;
	if (!WriteFile((HANDLE)fd,p,(DWORD)sz,&o,NULL)){
		return 0;
	}
	return o;
}



__SLL_EXTERNAL void sll_platform_free_page(void* pg,sll_page_size_t sz){
	VirtualFree(pg,0,MEM_RELEASE);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_time_t sll_platform_get_current_time(void){
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return ((((sll_time_t)ft.dwHighDateTime)<<32)|ft.dwLowDateTime)*100-11644473600000000000;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_platform_get_current_working_directory(sll_char_t* o,sll_string_length_t ol){
	if (!GetCurrentDirectoryA(ol,(char*)o)){
		*o=0;
		return 0;
	}
	return sll_string_length_unaligned(o);
}



sll_file_descriptor_t sll_platform_get_default_stream_descriptor(sll_char_t t){
	if (t==SLL_PLATFORM_STREAM_INPUT){
		return (sll_file_descriptor_t)GetStdHandle(STD_INPUT_HANDLE);
	}
	if (t==SLL_PLATFORM_STREAM_OUTPUT){
		return (sll_file_descriptor_t)GetStdHandle(STD_OUTPUT_HANDLE);
	}
	SLL_ASSERT(t==SLL_PLATFORM_STREAM_ERROR);
	return (sll_file_descriptor_t)GetStdHandle(STD_ERROR_HANDLE);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_platform_get_executable_file_path(sll_char_t* o,sll_string_length_t ol){
	return GetModuleFileNameA(NULL,(char*)o,ol);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_page_size_t sll_platform_get_page_size(void){
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwPageSize;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_platform_list_directory(const sll_char_t* fp,sll_string_t** o){
	char bf[MAX_PATH+1];
	sll_string_length_t fpl=sll_string_length_unaligned(fp);
	sll_copy_data(fp,fpl,bf);
	if (bf[fpl-1]!='/'&&bf[fpl-1]!='\\'){
		bf[fpl]='\\';
		fpl++;
	}
	bf[fpl]='*';
	bf[fpl+1]=0;
	sll_string_t* op=sll_allocate_stack(1);
	SLL_CHECK_NO_MEMORY(op);
	sll_array_length_t ol=0;
	WIN32_FIND_DATAA dt;
	HANDLE fh=FindFirstFileA(bf,&dt);
	if (fh!=INVALID_HANDLE_VALUE){
		do{
			if ((dt.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)&&*(dt.cFileName)=='.'&&(*(dt.cFileName+1)==0||(*(dt.cFileName+1)=='.'&&*(dt.cFileName+2)==0))){
				continue;
			}
			if (!(ol&7)){
				op=sll_reallocate(op,(ol+8)*sizeof(sll_string_t));
				SLL_CHECK_NO_MEMORY(op);
			}
			sll_string_length_t l=sll_string_length_unaligned(SLL_CHAR(dt.cFileName));
			sll_string_from_pointer_length(dt.cFileName,l,op+ol);
			ol++;
		} while (FindNextFileA(fh,&dt));
		FindClose(fh);
	}
	*o=sll_reallocate(op,ol*sizeof(sll_string_t));
	SLL_CHECK_NO_MEMORY(*o);
	return ol;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_platform_list_directory_recursive(const sll_char_t* fp,sll_string_t** o){
	sll_char_t bf[MAX_PATH+1];
	sll_string_length_t i=sll_string_length_unaligned(fp);
	sll_copy_data(fp,i,bf);
	if (bf[i-1]!='/'&&bf[i-1]!='\\'){
		bf[i]='\\';
		i++;
	}
	file_list_data_t dt={
		sll_allocate_stack(1),
		0
	};
	SLL_CHECK_NO_MEMORY(dt.dt);
	_list_dir_files(bf,i,&dt);
	*o=sll_reallocate(dt.dt,dt.l*sizeof(sll_string_t));
	SLL_CHECK_NO_MEMORY(*o);
	return dt.l;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_exists(const sll_char_t* fp){
	return (GetFileAttributesA(fp)!=INVALID_FILE_ATTRIBUTES);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_is_directory(const sll_char_t* fp){
	DWORD a=GetFileAttributesA(fp);
	return (a!=INVALID_FILE_ATTRIBUTES&&(a&FILE_ATTRIBUTE_DIRECTORY));
}



__SLL_EXTERNAL void sll_platform_reset_console(void){
	_setmode(_fileno(stdout),_win_stdout_m);
	_setmode(_fileno(stderr),_win_stderr_m);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_set_current_working_directory(const sll_char_t* p){
	return !!SetCurrentDirectoryA((char*)p);
}



__SLL_EXTERNAL void sll_platform_setup_console(void){
	_win_stdout_m=_setmode(_fileno(stdout),_O_BINARY);
	_win_stderr_m=_setmode(_fileno(stderr),_O_BINARY);
}



__SLL_EXTERNAL void sll_platform_sleep(sll_time_t tm){
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
