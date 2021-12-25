#include <windows.h>
#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>



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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_platform_get_current_working_directory(sll_char_t* o,sll_string_length_t ol){
	if (!GetCurrentDirectoryA(ol,(char*)o)){
		*o=0;
		return 0;
	}
	return sll_string_length_unaligned(o);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_platform_get_executable_file_path(sll_char_t* o,sll_string_length_t ol){
	return GetModuleFileNameA(NULL,(char*)o,ol);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_platform_get_library_file_path(sll_char_t* o,sll_string_length_t ol){
	return GetModuleFileNameA((HMODULE)_win_dll_handle,(char*)o,ol);
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
	sll_array_length_t ol=0;
	WIN32_FIND_DATAA dt;
	HANDLE fh=FindFirstFileA(bf,&dt);
	if (fh!=INVALID_HANDLE_VALUE){
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
			sll_string_length_t l=sll_string_length_unaligned(SLL_CHAR(dt.cFileName));
			sll_string_from_pointer_length(dt.cFileName,l,op+ol);
			ol++;
		} while (FindNextFileA(fh,&dt));
		FindClose(fh);
	}
	*o=sll_reallocate(op,ol*sizeof(sll_string_t));
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
	_list_dir_files(bf,i,&dt);
	*o=sll_reallocate(dt.dt,dt.l*sizeof(sll_string_t));
	return dt.l;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_exists(const sll_char_t* fp){
	return (GetFileAttributesA(fp)!=INVALID_FILE_ATTRIBUTES);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_is_directory(const sll_char_t* fp){
	DWORD a=GetFileAttributesA(fp);
	return (a!=INVALID_FILE_ATTRIBUTES&&(a&FILE_ATTRIBUTE_DIRECTORY));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_set_current_working_directory(const sll_char_t* p){
	return !!SetCurrentDirectoryA((char*)p);
}
