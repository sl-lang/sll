#include <sll/_internal/common.h>
#include <sll/_internal/error.h>
#include <sll/_internal/platform.h>
#include <sll/_internal/static_string.h>
#include <sll/api/path.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/environment.h>
#include <sll/init.h>
#include <sll/memory.h>
#include <sll/platform/path.h>
#include <sll/platform/util.h>
#include <sll/string.h>
#include <sll/types.h>
#include <dirent.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#ifdef __SLL_BUILD_DARWIN
#include <sys/socket.h>
#include <sys/syslimits.h>
#else
#include <sys/sendfile.h>
#endif



static __STATIC_STRING_CODE(_linux_executable_fp,{
	sll_char_t buffer[SLL_API_MAX_FILE_PATH_LENGTH];
#ifdef __SLL_BUILD_DARWIN
	sll_string_length_t l=SLL_API_MAX_FILE_PATH_LENGTH;
	if (_NSGetExecutablePath((char*)buffer,&l)==-1){
		SLL_INIT_STRING(out);
		return;
	}
#else
	ssize_t i=readlink("/proc/self/exe",(char*)buffer,SLL_API_MAX_FILE_PATH_LENGTH-1);
	if (i==-1){
		SLL_INIT_STRING(out);
		return;
	}
	*(buffer+i)=0;
#endif
	sll_char_t abs_bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_from_pointer_length(abs_bf,sll_platform_absolute_path(buffer,abs_bf,SLL_API_MAX_FILE_PATH_LENGTH),out);
});
static __STATIC_STRING_CODE(_linux_library_fp,{
	Dl_info dt;
	if (!dladdr(sll_init,&dt)){
		SLL_INIT_STRING(out);
		return;
	}
	sll_char_t buffer[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_from_pointer_length(buffer,sll_platform_absolute_path((const sll_char_t*)(dt.dli_fname),buffer,SLL_API_MAX_FILE_PATH_LENGTH),out);
});
static __STATIC_STRING_CODE(_linux_temporary_fp,{
	sll_string_t k;
	sll_string_from_pointer(SLL_CHAR("TMPDIR"),&k);
	sll_bool_t st=sll_get_environment_variable(&k,out);
	sll_free_string(&k);
	if (!st){
		sll_string_from_pointer(SLL_CHAR("/tmp"),out);
	}
});



__SLL_EXTERNAL const sll_string_t* sll_executable_file_path=&_linux_executable_fp;
__SLL_EXTERNAL const sll_string_t* sll_library_file_path=&_linux_library_fp;
__SLL_EXTERNAL const sll_string_t* sll_temporary_file_path=&_linux_temporary_fp;



static sll_error_t _list_dir_files(sll_char_t* buffer,sll_string_length_t i,file_list_data_t* o){
	buffer[i]=0;
	DIR* d=opendir((char*)buffer);
	if (!d){
		return sll_platform_get_error();
	}
	struct dirent* dt;
	while ((dt=readdir(d))){
		if (dt->d_type==DT_REG){
			sll_string_length_t j=sll_string_length(SLL_CHAR(dt->d_name));
			o->length++;
			o->data=sll_reallocate(o->data,o->length*sizeof(sll_string_t));
			sll_string_t* s=o->data+o->length-1;
			sll_string_create(i+j,s);
			sll_string_insert_pointer_length(buffer,i,0,s);
			sll_string_insert_pointer_length(SLL_CHAR(dt->d_name),j,i,s);
		}
		else if (dt->d_type==DT_DIR){
			if (*(dt->d_name)=='.'&&(*(dt->d_name+1)==0||(*(dt->d_name+1)=='.'&&*(dt->d_name+2)==0))){
				continue;
			}
			sll_string_length_t j=sll_string_length(SLL_CHAR(dt->d_name));
			sll_copy_data(dt->d_name,j,buffer+i);
			buffer[i+j]='/';
			sll_error_t err=_list_dir_files(buffer,i+j+1,o);
			if (err!=SLL_NO_ERROR){
				closedir(d);
				return err;
			}
		}
	}
	closedir(d);
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL sll_string_length_t sll_platform_absolute_path(const sll_char_t* lnk,sll_char_t* o,sll_string_length_t ol){
	if (ol>=PATH_MAX){
		if (!realpath((const char*)lnk,(char*)o)){
			return 0;
		}
		return sll_string_length(o);
	}
	sll_char_t buffer[PATH_MAX];
	if (!realpath((const char*)lnk,(char*)buffer)){
		return 0;
	}
	sll_string_length_t l=sll_string_length(buffer)+1;
	if (l>ol){
		l=ol;
	}
	sll_copy_data(buffer,l,o);
	return l;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_create_directory(const sll_char_t* fp,sll_bool_t all){
	if (all){
		SLL_UNIMPLEMENTED();
	}
	return (mkdir((char*)fp,0777)?sll_platform_get_error():SLL_NO_ERROR);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_platform_get_current_working_directory(sll_char_t* o,sll_string_length_t ol,sll_error_t* err){
	ERROR_PTR_RESET;
	if (getcwd((char*)o,ol)){
		return sll_string_length(o);
	}
	ERROR_PTR_SYSTEM;
	*o=0;
	return 0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_platform_list_directory(const sll_char_t* fp,sll_string_t** o,sll_error_t* err){
	ERROR_PTR_RESET;
	DIR* d=opendir((char*)fp);
	if (!d){
		ERROR_PTR_SYSTEM;
		*o=NULL;
		return 0;
	}
	sll_string_t* op=sll_allocate_stack(1);
	sll_array_length_t ol=0;
	struct dirent* dt;
	while ((dt=readdir(d))){
		if (dt->d_type==DT_DIR&&*(dt->d_name)=='.'&&(*(dt->d_name+1)==0||(*(dt->d_name+1)=='.'&&*(dt->d_name+2)==0))){
			continue;
		}
		if (!(ol&7)){
			void* tmp=sll_reallocate(op,(ol+8)*sizeof(sll_string_t));
			if (!tmp){
				*o=sll_reallocate(op,ol*sizeof(sll_string_t));
				closedir(d);
				return ol-1;
			}
			op=tmp;
		}
		sll_string_length_t l=sll_string_length(SLL_CHAR(dt->d_name));
		sll_string_from_pointer_length(SLL_CHAR(dt->d_name),l,op+ol);
		ol++;
	}
	closedir(d);
	*o=sll_reallocate(op,ol*sizeof(sll_string_t));
	return ol;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_platform_list_directory_recursive(const sll_char_t* fp,sll_string_t** o,sll_error_t* err){
	ERROR_PTR_RESET;
	sll_char_t buffer[PATH_MAX+1];
	sll_string_length_t l=sll_string_length(fp);
	sll_copy_data(fp,l,buffer);
	if (buffer[l-1]!='/'&&buffer[l-1]!='\\'){
		buffer[l]='/';
		l++;
	}
	file_list_data_t dt={
		sll_allocate_stack(1),
		0
	};
	sll_error_t v=_list_dir_files(buffer,l,&dt);
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
	int s_fd=open((const char*)s,O_RDONLY);
	if (s_fd==-1){
		return sll_platform_get_error();
	}
	struct stat st;
	fstat(s_fd,&st);
	int d_fd=creat((const char*)d,st.st_mode&(S_IRWXU|S_IRWXG|S_IRWXO));
	if (d_fd==-1){
		close(s_fd);
		return sll_platform_get_error();
	}
	size_t sz=st.st_size;
	sll_error_t o=SLL_NO_ERROR;
	while (sz){
#ifdef __SLL_BUILD_DARWIN
		off_t c_sz=sz;
		int v=sendfile(d_fd,s_fd,0,&c_sz,NULL,0);
#else
		int v=sendfile(d_fd,s_fd,NULL,sz);
#endif
		if (v==-1){
			o=sll_platform_get_error();
			break;
		}
		sz-=v;
	}
	close(s_fd);
	close(d_fd);
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_path_delete(const sll_char_t* fp){
	return (remove((char*)fp)?sll_platform_get_error():SLL_NO_ERROR);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_exists(const sll_char_t* fp){
	return !access((char*)fp,F_OK);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_is_directory(const sll_char_t* fp){
	struct stat st;
	return (!stat((char*)fp,&st)&&(st.st_mode&S_IFDIR));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_path_rename(const sll_char_t* s,const sll_char_t* d){
	return (rename((const char*)s,(const char*)d)?sll_platform_get_error():SLL_NO_ERROR);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_set_current_working_directory(const sll_char_t* p){
	return (chdir((char*)p)?sll_platform_get_error():SLL_NO_ERROR);
}
