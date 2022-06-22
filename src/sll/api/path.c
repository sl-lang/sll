#include <sll/_internal/common.h>
#include <sll/_internal/static_object.h>
#include <sll/api/path.h>
#include <sll/array.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/new_object.h>
#include <sll/platform/file.h>
#include <sll/platform/path.h>
#include <sll/sandbox.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_API_CALL void sll_api_path_absolute(const sll_string_t* path,sll_string_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		sll_string_clone(path,out);
		return;
	}
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_length_t bfl=sll_platform_absolute_path(path->data,bf,SLL_API_MAX_FILE_PATH_LENGTH);
	sll_string_from_pointer_length(bf,bfl,out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_path_exists(const sll_string_t* path){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		return 0;
	}
	return sll_platform_path_exists(path->data);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_path_get_cwd(sll_string_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_PATH_API);
	}
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_error_t err;
	sll_string_length_t len=sll_platform_get_current_working_directory(bf,SLL_API_MAX_FILE_PATH_LENGTH,&err);
	if (err==SLL_NO_ERROR){
		sll_string_from_pointer_length(bf,len,out);
	}
	return err;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_path_is_dir(const sll_string_t* path){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		return 0;
	}
	return sll_platform_path_is_directory(path->data);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_path_join(const sll_string_t*const* parts,sll_arg_count_t len,sll_string_t* out){
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_length_t i=0;
	if (len){
		do{
			if ((*parts)->length>=SLL_API_MAX_FILE_PATH_LENGTH){
				SLL_INIT_STRING(out);
				return;
			}
			i=(*parts)->length;
			sll_copy_data((*parts)->data,i,bf);
			len--;
			parts++;
		} while (!i);
		while (len){
			if (!(*parts)->length){
				continue;
			}
			if (i&&bf[i-1]!='/'&&bf[i-1]!='\\'){
				bf[i]=SLL_API_FILE_PATH_SEPARATOR;
				i++;
			}
			if ((*parts)->length+i>=SLL_API_MAX_FILE_PATH_LENGTH){
				SLL_INIT_STRING(out);
				return;
			}
			sll_copy_data((*parts)->data,(*parts)->length,bf+i);
			i+=(*parts)->length;
			len--;
			parts++;
		}
	}
	sll_string_from_pointer_length(bf,i,out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_path_list_dir(const sll_string_t* path,sll_bool_t recursive,sll_array_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_PATH_API);
	}
	sll_audit(SLL_CHAR("sll.path.dir.list"),SLL_CHAR("su"),path,recursive);
	sll_string_t* dt=NULL;
	sll_error_t err;
	sll_array_length_t len=(recursive?sll_platform_list_directory_recursive:sll_platform_list_directory)((path->length?path->data:SLL_CHAR(".")),&dt,&err);
	if (err!=SLL_NO_ERROR){
		return err;
	}
	sll_array_create(len,out);
	SLL_ASSERT(!len||dt);
	for (sll_array_length_t i=0;i<len;i++){
		out->data[i]=STRING_TO_OBJECT_NOCOPY(dt+i);
	}
	sll_deallocate(dt);
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_path_mkdir(const sll_string_t* path,sll_bool_t all){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_PATH_API);
	}
	sll_audit(SLL_CHAR("sll.path.dir.create"),SLL_CHAR("sh"),path,all);
	return sll_platform_create_directory(path->data,all);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_path_set_cwd(const sll_string_t* path){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_PATH_API);
	}
	sll_audit(SLL_CHAR("sll.path.cwd.set"),SLL_CHAR("s"),path);
	return sll_platform_set_current_working_directory(path->data);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_path_size(const sll_string_t* path,sll_size_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_PATH_API);
	}
	sll_error_t err;
	sll_file_descriptor_t fd=sll_platform_file_open(path->data,SLL_FILE_FLAG_READ,&err);
	if (fd!=SLL_UNKNOWN_FILE_DESCRIPTOR){
		*out=sll_platform_file_size(fd,&err);
		SLL_CRITICAL_ERROR(sll_platform_file_close(fd));
	}
	return err;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_path_split(const sll_string_t* path,sll_array_t* out){
	sll_string_length_t i=sll_path_split(path);
	sll_new_object_array(SLL_CHAR("ll"),out,path->data,i,path->data+i,path->length-i);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_path_split_drive(const sll_string_t* path,sll_array_t* out){
	sll_string_length_t i=sll_path_split_drive(path);
	sll_new_object_array(SLL_CHAR("ll"),out,path->data,i,path->data+i,path->length-i);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_path_split(const sll_string_t* s){
	sll_char_t dt[2]={
		'/',
		'\\'
	};
	sll_string_length_t o=sll_string_index_reverse_multiple(s,dt,2,0);
	return (o==SLL_MAX_STRING_LENGTH?0:o+1);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_path_split_drive(const sll_string_t* s){
#ifdef __SLL_BUILD_WINDOWS
	if (s->length>1&&s->data[1]==':'){
		return 2;
	}
	if (s->length>2&&s->data[0]=='\\'&&s->data[1]=='\\'&&s->data[2]!='\\'){
		SLL_UNIMPLEMENTED();
	}
#endif
	return 0;
}
