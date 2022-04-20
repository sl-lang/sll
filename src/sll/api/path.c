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
#include <sll/types.h>



#ifdef __SLL_BUILD_WINDOWS
static const sll_char_t _path_norm_drive[256]={
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
	80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
	96,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
	80,81,82,83,84,85,86,87,88,89,122,123,124,125,126,127,
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
	176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
	208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255
};
#endif



__SLL_EXTERNAL void sll_path_relative(const sll_char_t* s,const sll_char_t* b,sll_string_t* o){
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_t ns;
	sll_string_t nb;
	sll_string_from_pointer_length(bf,sll_platform_absolute_path(s,bf,SLL_API_MAX_FILE_PATH_LENGTH),&ns);
	sll_string_from_pointer_length(bf,sll_platform_absolute_path(b,bf,SLL_API_MAX_FILE_PATH_LENGTH),&nb);
#ifdef __SLL_BUILD_WINDOWS
	sll_string_length_t s_drv=sll_path_split_drive(&ns);
	sll_string_length_t b_drv=sll_path_split_drive(&nb);
	if (s_drv!=b_drv||(s_drv==2&&_path_norm_drive[ns.v[0]]!=_path_norm_drive[nb.v[0]])||(s_drv>2&&sll_compare_data(ns.v,nb.v,s_drv)!=SLL_COMPARE_RESULT_EQUAL)){
		*o=ns;
		sll_free_string(&nb);
		return;
	}
#endif
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_path_split(const sll_string_t* s){
	sll_char_t dt[2]={
		'/',
		'\\'
	};
	sll_string_length_t o=sll_string_index_reverse_multiple(s,dt,2,0);
	return (o==SLL_MAX_STRING_INDEX?0:o+1);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_path_split_drive(const sll_string_t* s){
#ifdef __SLL_BUILD_WINDOWS
	if (s->l>1&&s->v[1]==':'){
		return 2;
	}
	if (s->l>2&&s->v[0]=='\\'&&s->v[1]=='\\'&&s->v[2]!='\\'){
		SLL_UNIMPLEMENTED();
	}
#endif
	return 0;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_path_absolute(const sll_string_t* path,sll_string_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		sll_string_clone(path,out);
		return;
	}
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_from_pointer_length(bf,sll_platform_absolute_path(path->v,bf,SLL_API_MAX_FILE_PATH_LENGTH),out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_path_exists(const sll_string_t* path){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		return 0;
	}
	return sll_platform_path_exists(path->v);
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
	return sll_platform_path_is_directory(path->v);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_path_join(const sll_string_t*const* parts,sll_arg_count_t len,sll_string_t* out){
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_length_t i=0;
	if (len){
		do{
			if ((*parts)->l>=SLL_API_MAX_FILE_PATH_LENGTH){
				SLL_INIT_STRING(out);
				return;
			}
			i=(*parts)->l;
			sll_copy_data((*parts)->v,i,bf);
			len--;
			parts++;
		} while (!i);
		while (len){
			if (!(*parts)->l){
				continue;
			}
			if (i&&bf[i-1]!='/'&&bf[i-1]!='\\'){
				bf[i]=SLL_API_FILE_PATH_SEPARATOR;
				i++;
			}
			if ((*parts)->l+i>=SLL_API_MAX_FILE_PATH_LENGTH){
				SLL_INIT_STRING(out);
				return;
			}
			sll_copy_data((*parts)->v,(*parts)->l,bf+i);
			i+=(*parts)->l;
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
	sll_audit(SLL_CHAR("sll.path.dir.list"),SLL_CHAR("sb"),path,recursive);
	sll_string_t* dt=NULL;
	sll_error_t err;
	sll_array_length_t len=(recursive?sll_platform_list_directory_recursive:sll_platform_list_directory)(path->v,&dt,&err);
	if (err!=SLL_NO_ERROR){
		return err;
	}
	sll_array_create(len,out);
	for (sll_array_length_t i=0;i<len;i++){
		out->v[i]=STRING_TO_OBJECT_NOCOPY(dt+i);
	}
	sll_deallocate(dt);
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_path_mkdir(const sll_string_t* path,sll_bool_t all){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_PATH_API);
	}
	sll_audit(SLL_CHAR("sll.path.dir.create"),SLL_CHAR("sh"),path,all);
	return sll_platform_create_directory(path->v,all);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_path_relative(const sll_string_t* path,const sll_string_t* base,sll_string_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		sll_string_clone(path,out);
		return;
	}
	sll_path_relative(path->v,base->v,out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_path_set_cwd(const sll_string_t* path){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_PATH_API);
	}
	sll_audit(SLL_CHAR("sll.path.cwd.set"),SLL_CHAR("s"),path);
	return sll_platform_set_current_working_directory(path->v);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_path_size(const sll_string_t* path){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		return 0;
	}
	sll_error_t err;
	sll_file_descriptor_t fd=sll_platform_file_open(path->v,SLL_FILE_FLAG_READ,&err);
	if (fd!=SLL_UNKNOWN_FILE_DESCRIPTOR){
		sll_size_t o=sll_platform_file_size(fd,&err);
		sll_platform_file_close(fd);
		if (o||err==SLL_NO_ERROR){
			return o;
		}
	}
	return ~err;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_path_split(const sll_string_t* path,sll_array_t* out){
	sll_string_length_t i=sll_path_split(path);
	sll_new_object_array(SLL_CHAR("ll"),out,path->v,i,path->v+i,path->l-i);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_path_split_drive(const sll_string_t* path,sll_array_t* out){
	sll_string_length_t i=sll_path_split_drive(path);
	sll_new_object_array(SLL_CHAR("ll"),out,path->v,i,path->v+i,path->l-i);
}
