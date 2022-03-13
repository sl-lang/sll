#include <windows.h>
#include <sll/_internal/common.h>
#include <sll/_internal/error.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/platform/file.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_file_async_read(sll_file_descriptor_t fd){
	DWORD d=GetFileType((HANDLE)fd);
	return (d!=FILE_TYPE_UNKNOWN&&d!=FILE_TYPE_DISK);
}



__SLL_EXTERNAL void sll_platform_file_close(sll_file_descriptor_t fd){
	CloseHandle((HANDLE)fd);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_file_data_available(sll_file_descriptor_t fd){
	switch (GetFileType((HANDLE)fd)){
		case FILE_TYPE_CHAR:
			{
				DWORD sz;
				while (GetNumberOfConsoleInputEvents((HANDLE)fd,&sz)&&sz){
					INPUT_RECORD ir;
					DWORD cnt;
					if (!PeekConsoleInput((HANDLE)fd,&ir,1,&cnt)||!cnt){
						return 0;
					}
					if (ir.EventType==KEY_EVENT){
						return 1;
					}
					if (!ReadConsoleInput((HANDLE)fd,&ir,1,&cnt)){
						return 0;
					}

				}
				return 0;
			}
		case FILE_TYPE_PIPE:
			SLL_UNIMPLEMENTED();
	}
	return 1;
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
		return SLL_NO_FILE_SIZE;
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
		return SLL_NO_FILE_SIZE;
	}
	return sz.QuadPart;
}



__SLL_EXTERNAL sll_size_t sll_platform_file_write(sll_file_descriptor_t fd,const void* p,sll_size_t sz,sll_error_t* err){
	RESET_ERROR_PTR;
	DWORD o;
	if (!WriteFile((HANDLE)fd,p,(DWORD)sz,&o,NULL)){
		WINAPI_ERROR_PTR;
		return SLL_NO_FILE_SIZE;
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
