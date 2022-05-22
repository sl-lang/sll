#include <windows.h>
#include <winioctl.h>
#include <sll/_internal/common.h>
#include <sll/_internal/error.h>
#include <sll/_internal/platform.h>
#include <sll/_internal/static_string.h>
#include <sll/api/path.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>



static __STATIC_STRING_CODE(_win_executable_fp,{
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_from_pointer_length(bf,GetModuleFileNameA(NULL,bf,SLL_API_MAX_FILE_PATH_LENGTH),out);
});
static __STATIC_STRING_CODE(_win_library_fp,{
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_from_pointer_length(bf,GetModuleFileNameA((HMODULE)_win_dll_handle,bf,SLL_API_MAX_FILE_PATH_LENGTH),out);
});
static __STATIC_STRING_CODE(_win_temporary_fp,{
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_from_pointer_length(bf,GetTempPathA(SLL_API_MAX_FILE_PATH_LENGTH,bf),out);
});



__SLL_EXTERNAL const sll_string_t* sll_executable_file_path=&_win_executable_fp;
__SLL_EXTERNAL const sll_string_t* sll_library_file_path=&_win_library_fp;
__SLL_EXTERNAL const sll_string_t* sll_temporary_file_path=&_win_temporary_fp;



static sll_error_t _list_dir_files(sll_char_t* bf,sll_string_length_t i,file_list_data_t* o){
	WIN32_FIND_DATAA dt;
	bf[i]='*';
	bf[i+1]=0;
	HANDLE fh=FindFirstFileA(bf,&dt);
	if (fh==INVALID_HANDLE_VALUE){
		return sll_platform_get_error();
	}
	do{
		if (dt.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){
			if (*(dt.cFileName)=='.'&&(*(dt.cFileName+1)==0||(*(dt.cFileName+1)=='.'&&*(dt.cFileName+2)==0))){
				continue;
			}
			sll_string_length_t j=sll_string_length(SLL_CHAR(dt.cFileName));
			sll_copy_data(dt.cFileName,j,bf+i);
			bf[i+j]='\\';
			sll_error_t err=_list_dir_files(bf,i+j+1,o);
			if (err!=SLL_NO_ERROR){
				FindClose(fh);
				return err;
			}
		}
		else{
			sll_string_length_t j=sll_string_length(SLL_CHAR(dt.cFileName));
			o->length++;
			o->data=sll_reallocate(o->data,o->length*sizeof(sll_string_t));
			sll_string_t* s=o->data+o->length-1;
			sll_string_create(i+j,s);
			sll_string_insert_pointer_length(bf,i,0,s);
			sll_string_insert_pointer_length(dt.cFileName,j,i,s);
		}
	} while (FindNextFileA(fh,&dt));
	FindClose(fh);
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL sll_string_length_t sll_platform_absolute_path(const sll_char_t* fp,sll_char_t* o,sll_string_length_t ol){
	return GetFullPathNameA(fp,ol,o,NULL);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_create_directory(const sll_char_t* fp,sll_bool_t all){
	if (all){
		SLL_UNIMPLEMENTED();
	}
	return (CreateDirectoryA(fp,NULL)?SLL_NO_ERROR:sll_platform_get_error());
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_platform_get_current_working_directory(sll_char_t* o,sll_string_length_t ol,sll_error_t* err){
	ERROR_PTR_RESET;
	if (GetCurrentDirectoryA(ol,(char*)o)){
		return sll_string_length(o);
	}
	ERROR_PTR_SYSTEM;
	*o=0;
	return 0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_platform_list_directory(const sll_char_t* fp,sll_string_t** o,sll_error_t* err){
	ERROR_PTR_RESET;
	char bf[MAX_PATH+1];
	sll_string_length_t fpl=sll_string_length(fp);
	sll_copy_data(fp,fpl,bf);
	if (bf[fpl-1]!='/'&&bf[fpl-1]!='\\'){
		bf[fpl]='\\';
		fpl++;
	}
	bf[fpl]='*';
	bf[fpl+1]=0;
	WIN32_FIND_DATAA dt;
	HANDLE fh=FindFirstFileA(bf,&dt);
	if (fh==INVALID_HANDLE_VALUE){
		ERROR_PTR_SYSTEM;
		*o=NULL;
		return 0;
	}
	sll_string_t* op=sll_allocate_stack(1);
	sll_array_length_t ol=0;
	do{
		if ((dt.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)&&*(dt.cFileName)=='.'&&(*(dt.cFileName+1)==0||(*(dt.cFileName+1)=='.'&&*(dt.cFileName+2)==0))){
			continue;
		}
		if (!(ol&7)){
			void* tmp=sll_reallocate(op,(ol+8)*sizeof(sll_string_t));
			if (!tmp){
				*o=sll_reallocate(op,ol*sizeof(sll_string_t));
				FindClose(fh);
				return ol-1;
			}
			op=tmp;
		}
		sll_string_length_t l=sll_string_length(SLL_CHAR(dt.cFileName));
		sll_string_from_pointer_length(dt.cFileName,l,op+ol);
		ol++;
	} while (FindNextFileA(fh,&dt));
	FindClose(fh);
	*o=sll_reallocate(op,ol*sizeof(sll_string_t));
	return ol;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_platform_list_directory_recursive(const sll_char_t* fp,sll_string_t** o,sll_error_t* err){
	ERROR_PTR_RESET;
	sll_char_t bf[MAX_PATH+1];
	sll_string_length_t i=sll_string_length(fp);
	sll_copy_data(fp,i,bf);
	if (bf[i-1]!='/'&&bf[i-1]!='\\'){
		bf[i]='\\';
		i++;
	}
	file_list_data_t dt={
		sll_allocate_stack(1),
		0
	};
	sll_error_t v=_list_dir_files(bf,i,&dt);
	if (v!=SLL_NO_ERROR){
		*err=v;
		sll_deallocate(dt.data);
		*o=NULL;
		return 0;
	}
	if (!dt.length){
		sll_deallocate(dt.data);
		dt.data=NULL;
	}
	*o=dt.data;
	return dt.length;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_path_copy(const sll_char_t* s,const sll_char_t* d){
	return (CopyFileA((const char*)s,(const char*)d,FALSE)?SLL_NO_ERROR:sll_platform_get_error());
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_path_delete(const sll_char_t* fp){
	DWORD a=GetFileAttributesA(fp);
	if (a==INVALID_FILE_ATTRIBUTES){
		return SLL_ERROR_NO_FILE_PATH;
	}
	return (((a&FILE_ATTRIBUTE_DIRECTORY)?!RemoveDirectoryA(fp):DeleteFileA(fp))?sll_platform_get_error():SLL_NO_ERROR);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_exists(const sll_char_t* fp){
	return (GetFileAttributesA(fp)!=INVALID_FILE_ATTRIBUTES);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_is_directory(const sll_char_t* fp){
	DWORD a=GetFileAttributesA(fp);
	return (a!=INVALID_FILE_ATTRIBUTES&&(a&FILE_ATTRIBUTE_DIRECTORY));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_path_rename(const sll_char_t* s,const sll_char_t* d){
	return (MoveFileA((const char*)s,(const char*)d)?SLL_NO_ERROR:sll_platform_get_error());
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_set_current_working_directory(const sll_char_t* p){
	return (SetCurrentDirectoryA((const char*)p)?SLL_NO_ERROR:sll_platform_get_error());
}
