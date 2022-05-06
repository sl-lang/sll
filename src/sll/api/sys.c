#include <sll/_internal/common.h>
#include <sll/_internal/library.h>
#include <sll/_internal/print.h>
#include <sll/_internal/static_object.h>
#include <sll/_internal/static_string.h>
#include <sll/_internal/string.h>
#include <sll/abi.h>
#include <sll/api/hash.h>
#include <sll/api/path.h>
#include <sll/api/sys.h>
#include <sll/array.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/environment.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/init.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/new_object.h>
#include <sll/platform/file.h>
#include <sll/platform/library.h>
#include <sll/platform/path.h>
#include <sll/platform/thread.h>
#include <sll/platform/util.h>
#include <sll/sandbox.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/version.h>
#include <sll/vm.h>



#if SLL_VERSION_HAS_SHA
static __STATIC_STRING(_sys_full_commit,SLL_VERSION_FULL_SHA);
#endif
static sll_array_length_t _sys_argc=0;
static sll_string_t* _sys_argv=NULL;
static library_t** _sys_lh=NULL;
static sll_array_length_t _sys_lhl=0;
static sll_bool_t _sys_init=0;
static sll_bool_t _sys_vm_init=0;



static void _cleanup_data(void){
	if (_sys_argv){
		for (sll_array_length_t i=0;i<_sys_argc;i++){
			sll_free_string(_sys_argv+i);
		}
		sll_deallocate(_sys_argv);
		_sys_argv=NULL;
	}
	_sys_init=0;
}



static void _cleanup_vm_data(void){
	if (_sys_lhl){
		while (_sys_lhl){
			_sys_lhl--;
			library_t* l=*(_sys_lh+_sys_lhl);
			sll_free_string((sll_string_t*)&(l->nm));
			void (*fn)(void)=sll_platform_lookup_symbol(l->h,SLL_ABI_NAME(SLL_ABI_DEINIT));
			if (fn){
				fn();
			}
			SLL_CRITICAL_ERROR(sll_platform_unload_library(l->h));
			sll_deallocate(l);
		}
		sll_deallocate(_sys_lh);
		_sys_lh=NULL;
	}
	_sys_vm_init=0;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_get_args(sll_array_t* out){
	sll_object_t* obj=sll_new_object(SLL_CHAR("s+"),_sys_argv,_sys_argc);
	*out=obj->dt.a;
	SLL_CRITICAL(sll_destroy_object(obj));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_cpu_t sll_api_sys_get_cpu_count(void){
	return *sll_platform_cpu_count;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_get_env(sll_map_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_ENVIRONMENT)){
		SLL_INIT_MAP(out);
		return;
	}
	sll_map_create(sll_environment->l,out);
	for (sll_environment_length_t i=0;i<sll_environment->l;i++){
		const sll_environment_variable_t* kv=*(sll_environment->dt+i);
		out->v[i<<1]=STRING_TO_OBJECT(&(kv->k));
		out->v[(i<<1)+1]=STRING_TO_OBJECT(&(kv->v));
	}
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_get_executable(sll_string_t* out){
	sll_string_clone(sll_executable_file_path,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_get_library(sll_string_t* out){
	sll_string_clone(sll_library_file_path,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_get_platform(sll_string_t* out){
	sll_string_clone(sll_platform_string,out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_sandbox_flags_t sll_api_sys_get_sandbox_flags(void){
	return sll_get_sandbox_flags();
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_get_version(sll_array_t* out){
#if SLL_VERSION_HAS_SHA
	sll_new_object_array(SLL_CHAR("hhhsi"),out,SLL_VERSION_MAJOR,SLL_VERSION_MINOR,SLL_VERSION_PATCH,&_sys_full_commit,SLL_VERSION_BUILD_TIME);
#else
	sll_new_object_array(SLL_CHAR("hhhZi"),out,SLL_VERSION_MAJOR,SLL_VERSION_MINOR,SLL_VERSION_PATCH,SLL_VERSION_BUILD_TIME);
#endif
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_sys_load_library(const sll_string_t* name,sll_size_t sz,__SLL_U64 h0,__SLL_U64 h1,__SLL_U64 h2,__SLL_U64 h3){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_LOAD_LIBRARY)||name->l>=SLL_API_MAX_FILE_PATH_LENGTH){
		return 0;
	}
	sll_audit(SLL_CHAR("sll.sys.library.load"),SLL_CHAR("s"),name);
	sll_string_length_t lib_fp=sll_path_split(sll_library_file_path);
	sll_string_length_t src_nm_off=sll_path_split(name);
	sll_string_length_t fpl=lib_fp+STATIC_STRING_LEN(LIBRARY_DIRECTORY)+name->l-src_nm_off+STATIC_STRING_LEN(LIBRARY_EXTENSION);
	if (fpl>=SLL_API_MAX_FILE_PATH_LENGTH){
		return 0;
	}
	sll_string_t fp;
	sll_string_create(fpl,&fp);
	sll_string_length_t fp_off=lib_fp;
	sll_copy_data(sll_library_file_path->v,fp_off,fp.v);
	sll_copy_data(LIBRARY_DIRECTORY,STATIC_STRING_LEN(LIBRARY_DIRECTORY),fp.v+fp_off);
	fp_off+=STATIC_STRING_LEN(LIBRARY_DIRECTORY);
	sll_copy_data(name->v+src_nm_off,name->l-src_nm_off,fp.v+fp_off);
	fp_off+=name->l-src_nm_off;
	sll_copy_data(LIBRARY_EXTENSION,STATIC_STRING_LEN(LIBRARY_EXTENSION),fp.v+fp_off);
	sll_string_calculate_checksum(&fp);
	if (!sll_platform_path_exists(fp.v)){
		sll_free_string(&fp);
		return 0;
	}
	for (sll_array_length_t i=0;i<_sys_lhl;i++){
		if (STRING_EQUAL(&((*(_sys_lh+i))->nm),&fp)){
			sll_free_string(&fp);
			return 1;
		}
	}
	if (sz){
		sll_file_descriptor_t fd=sll_platform_file_open(fp.v,SLL_FILE_FLAG_READ,NULL);
		if (fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
			return 0;
		}
		sll_size_t f_sz=sll_platform_file_size(fd,NULL);
		if (f_sz==SLL_NO_FILE_SIZE||f_sz!=sz){
			SLL_CRITICAL_ERROR(sll_platform_file_close(fd));
			return 0;
		}
		sll_sha256_data_t sha=SLL_INIT_SHA256_STRUCT;
		sll_char_t bf[LIBRARY_HASH_BUFFER_SIZE];
		sll_size_t off;
		do{
			off=sll_platform_file_read(fd,bf,LIBRARY_HASH_BUFFER_SIZE,NULL);
			if (off==SLL_NO_FILE_SIZE){
				SLL_CRITICAL_ERROR(sll_platform_file_close(fd));
				return 0;
			}
			sll_hash_sha256(&sha,bf,off&0xffffffffffffffc0ll);
		} while (off==LIBRARY_HASH_BUFFER_SIZE);
		SLL_CRITICAL_ERROR(sll_platform_file_close(fd));
		sll_char_t tmp[128];
		sll_set_memory(tmp,128,0);
		sll_copy_data(bf+(off&0xffffffffffffffc0ull),off&0x3f,tmp);
		off&=0x3f;
		sll_char_t tmp_off=(off<56?56:120);
		tmp[off]=128;
		tmp[tmp_off]=(sz>>53)&0xff;
		tmp[tmp_off+1]=(sz>>45)&0xff;
		tmp[tmp_off+2]=(sz>>37)&0xff;
		tmp[tmp_off+3]=(sz>>29)&0xff;
		tmp[tmp_off+4]=(sz>>21)&0xff;
		tmp[tmp_off+5]=(sz>>13)&0xff;
		tmp[tmp_off+6]=(sz>>5)&0xff;
		tmp[tmp_off+7]=(sz<<3)&0xff;
		sll_hash_sha256(&sha,tmp,(off<56?64:128));
		if (((((__SLL_U64)sha.a)<<32)|sha.b)!=h0||((((__SLL_U64)sha.c)<<32)|sha.d)!=h1||((((__SLL_U64)sha.e)<<32)|sha.f)!=h2||((((__SLL_U64)sha.g)<<32)|sha.h)!=h3){
			return 0;
		}
	}
	sll_library_handle_t h=sll_platform_load_library(fp.v,NULL);
	if (!h){
		sll_free_string(&fp);
		return 0;
	}
	sll_bool_t (*fn)(sll_version_t)=sll_platform_lookup_symbol(h,SLL_ABI_NAME(SLL_ABI_INIT));
	if (!fn||!fn(SLL_VERSION)){
		SLL_CRITICAL_ERROR(sll_platform_unload_library(h));
		sll_free_string(&fp);
		return 0;
	}
	_sys_lhl++;
	_sys_lh=sll_reallocate(_sys_lh,_sys_lhl*sizeof(library_t*));
	library_t* n=sll_allocate(sizeof(library_t));
	sll_copy_data(&fp,sizeof(sll_string_t),(sll_string_t*)(&(n->nm)));
	n->h=h;
	*(_sys_lh+_sys_lhl-1)=n;
	if (!_sys_vm_init){
		sll_register_cleanup(_cleanup_vm_data,SLL_CLEANUP_TYPE_VM);
		_sys_vm_init=1;
	}
	return 1;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_remove_env(const sll_string_t* key){
	if (!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_ENVIRONMENT)){
		sll_audit(SLL_CHAR("sll.sys.env.delete"),SLL_CHAR("s"),key);
		sll_remove_environment_variable(key);
	}
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_set_sandbox_flag(sll_sandbox_flag_t flag){
	sll_audit(SLL_CHAR("sll.sys.sandbox.set"),SLL_CHAR("u"),flag);
	sll_set_sandbox_flag(flag);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_set_env(const sll_string_t* key,const sll_string_t* value){
	if (!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_ENVIRONMENT)){
		sll_audit(SLL_CHAR("sll.sys.env.set"),SLL_CHAR("ss"),key,value);
		sll_set_environment_variable(key,value);
	}
}



__SLL_EXTERNAL void sll_set_argument(sll_array_length_t i,const sll_char_t* a){
	if (i>=_sys_argc){
		return;
	}
	sll_free_string(_sys_argv+i);
	sll_string_from_pointer(a,_sys_argv+i);
}



__SLL_EXTERNAL void sll_set_argument_count(sll_array_length_t ac){
	SLL_ASSERT(ac>0);
	if (_sys_argv){
		for (sll_array_length_t i=0;i<_sys_argc;i++){
			sll_free_string(_sys_argv+i);
		}
		sll_deallocate(_sys_argv);
	}
	else if (!_sys_init){
		sll_register_cleanup(_cleanup_data,SLL_CLEANUP_TYPE_GLOBAL);
		_sys_init=1;
	}
	_sys_argc=ac;
	_sys_argv=sll_allocate(ac*sizeof(sll_string_t));
	for (sll_array_length_t i=0;i<ac;i++){
		SLL_INIT_STRING(_sys_argv+i);
	}
}
