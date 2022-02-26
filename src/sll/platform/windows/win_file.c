#include <windows.h>
#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_platform_file_close(sll_file_descriptor_t fd){
	CloseHandle((HANDLE)fd);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_descriptor_t sll_platform_file_open(const sll_char_t* fp,sll_file_flags_t ff,sll_error_t* err){
	RESET_ERROR_PTR;
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
		WINAPI_ERROR_PTR;
		return SLL_UNKNOWN_FILE_DESCRIPTOR;
	}
	return (sll_file_descriptor_t)o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_file_read(sll_file_descriptor_t fd,void* p,sll_size_t sz,sll_error_t* err){
	RESET_ERROR_PTR;
	DWORD o;
	if (!ReadFile((HANDLE)fd,p,(DWORD)sz,&o,NULL)){
		WINAPI_ERROR_PTR;
		return 0;
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_file_seek(sll_file_descriptor_t fd,sll_file_offset_t off){
	DWORD h=off>>32;
	sll_error_t o=(SetFilePointer((HANDLE)fd,(DWORD)(off&0xffffffff),&h,FILE_BEGIN)==INVALID_SET_FILE_POINTER?WINAPI_ERROR:SLL_NO_ERROR);
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_file_size(sll_file_descriptor_t fd,sll_error_t* err){
	RESET_ERROR_PTR;
	LARGE_INTEGER sz;
	if (!GetFileSizeEx((HANDLE)fd,&sz)){
		WINAPI_ERROR_PTR;
		return 0;
	}
	return sz.QuadPart;
}



__SLL_EXTERNAL sll_size_t sll_platform_file_write(sll_file_descriptor_t fd,const void* p,sll_size_t sz,sll_error_t* err){
	RESET_ERROR_PTR;
	DWORD o;
	if (!WriteFile((HANDLE)fd,p,(DWORD)sz,&o,NULL)){
		WINAPI_ERROR_PTR;
		return 0;
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_descriptor_t sll_platform_get_default_stream_descriptor(sll_char_t t){
	if (t==SLL_PLATFORM_STREAM_INPUT){
		return (sll_file_descriptor_t)GetStdHandle(STD_INPUT_HANDLE);
	}
	if (t==SLL_PLATFORM_STREAM_OUTPUT){
		return (sll_file_descriptor_t)GetStdHandle(STD_OUTPUT_HANDLE);
	}
	SLL_ASSERT(t==SLL_PLATFORM_STREAM_ERROR);
	return (sll_file_descriptor_t)GetStdHandle(STD_ERROR_HANDLE);
}
