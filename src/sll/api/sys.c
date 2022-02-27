#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/hash.h>
#include <sll/api/path.h>
#include <sll/api/sys.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/env.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/init.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/platform.h>
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
static char _sys_end=0;



static void _sys_free_data(void){
	if (_sys_argv){
		for (sll_array_length_t i=0;i<_sys_argc;i++){
			sll_free_string(_sys_argv+i);
		}
		sll_deallocate(_sys_argv);
		_sys_argv=NULL;
	}
	if (_sys_lhl){
		while (_sys_lhl){
			_sys_lhl--;
			library_t* l=*(_sys_lh+_sys_lhl);
			sll_free_string((sll_string_t*)&(l->nm));
			void (*fn)(void)=sll_platform_lookup_symbol(l->h,SLL_CHAR("__sll_unload"));
			if (fn){
				fn();
			}
			sll_platform_unload_library(l->h);
			sll_deallocate(l);
		}
		sll_deallocate(_sys_lh);
		_sys_lh=NULL;
	}
	_sys_end=0;
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
	else if (!_sys_end){
		sll_register_cleanup(_sys_free_data);
		_sys_end=1;
	}
	_sys_argc=ac;
	_sys_argv=sll_allocate(ac*sizeof(sll_string_t));
	for (sll_array_length_t i=0;i<ac;i++){
		SLL_INIT_STRING(_sys_argv+i);
	}
}



__API_FUNC(sys_get_args){
	sll_array_create(_sys_argc,out);
	for (sll_array_length_t i=0;i<_sys_argc;i++){
		sll_object_t* n=SLL_CREATE();
		n->t=SLL_OBJECT_TYPE_STRING;
		sll_string_clone(_sys_argv+i,&(n->dt.s));
		out->v[i]=n;
	}
}



__API_FUNC(sys_get_cpu_count){
	return sll_platform_get_cpu_count();
}



__API_FUNC(sys_get_env){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_ENVIRONMENT)){
		SLL_INIT_MAP(out);
		return;
	}
	sll_map_create(sll_environment->l,out);
	for (sll_array_length_t i=0;i<sll_environment->l;i++){
		const sll_environment_variable_t* kv=*(sll_environment->dt+i);
		sll_object_t* k=SLL_CREATE();
		k->t=SLL_OBJECT_TYPE_STRING;
		sll_string_clone(&(kv->k),&(k->dt.s));
		out->v[i<<1]=k;
		sll_object_t* v=SLL_CREATE();
		v->t=SLL_OBJECT_TYPE_STRING;
		sll_string_clone(&(kv->v),&(v->dt.s));
		out->v[(i<<1)+1]=v;
	}
}



__API_FUNC(sys_get_executable){
	sll_string_clone(sll_executable_file_path,out);
}



__API_FUNC(sys_get_library){
	sll_string_clone(sll_library_file_path,out);
}



__API_FUNC(sys_get_platform){
	sll_string_clone(sll_platform_string,out);
}



__API_FUNC(sys_get_sandbox_flags){
	sll_get_sandbox_flags(out);
}



__API_FUNC(sys_get_version){
	sll_array_create(5,out);
	out->v[0]=SLL_FROM_INT(SLL_VERSION_MAJOR);
	out->v[1]=SLL_FROM_INT(SLL_VERSION_MINOR);
	out->v[2]=SLL_FROM_INT(SLL_VERSION_PATCH);
	sll_object_t* cmt=SLL_CREATE();
	cmt->t=SLL_OBJECT_TYPE_STRING;
#if SLL_VERSION_HAS_SHA
	sll_string_clone(&_sys_full_commit,&(cmt->dt.s));
#else
	SLL_INIT_STRING(&(cmt->dt.s));
#endif
	out->v[3]=cmt;
	out->v[4]=SLL_FROM_INT(SLL_VERSION_BUILD_TIME);
}



__API_FUNC(sys_load_library){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_LOAD_LIBRARY)||a->l>=SLL_API_MAX_FILE_PATH_LENGTH){
		return 0;
	}
	sll_string_length_t lib_fp=sll_path_split(sll_library_file_path);
	sll_string_length_t src_nm_off=sll_path_split(a);
	sll_string_length_t fpl=lib_fp+STATIC_STRING_LEN(LIBRARY_DIRECTORY)+a->l-src_nm_off+STATIC_STRING_LEN(LIBRARY_EXTENSION);
	if (fpl>=SLL_API_MAX_FILE_PATH_LENGTH){
		return 0;
	}
	sll_string_t fp;
	sll_string_create(fpl,&fp);
	sll_string_length_t fp_off=lib_fp;
	sll_copy_data(sll_library_file_path->v,fp_off,fp.v);
	sll_copy_data(LIBRARY_DIRECTORY,STATIC_STRING_LEN(LIBRARY_DIRECTORY),fp.v+fp_off);
	fp_off+=STATIC_STRING_LEN(LIBRARY_DIRECTORY);
	sll_copy_data(a->v+src_nm_off,a->l-src_nm_off,fp.v+fp_off);
	fp_off+=a->l-src_nm_off;
	sll_copy_data(LIBRARY_EXTENSION,STATIC_STRING_LEN(LIBRARY_EXTENSION),fp.v+fp_off);
	sll_string_calculate_checksum(&fp);
	if (!sll_platform_path_exists(fp.v)){
		sll_free_string(&fp);
		return 0;
	}
	for (sll_array_length_t i=0;i<_sys_lhl;i++){
		if (sll_string_equal(&((*(_sys_lh+i))->nm),&fp)){
			sll_free_string(&fp);
			return 1;
		}
	}
	if (b){
		sll_file_descriptor_t fd=sll_platform_file_open(fp.v,SLL_FILE_FLAG_READ,NULL);
		if (fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
			return 0;
		}
		sll_size_t sz=sll_platform_file_size(fd,NULL);
		if (sz==SLL_NO_FILE_SIZE||sz!=(sll_size_t)b){
			sll_platform_file_close(fd);
			return 0;
		}
		sll_sha256_data_t sha=SLL_INIT_SHA256_STRUCT;
		sll_char_t bf[LIBRARY_HASH_BUFFER_SIZE];
		sll_size_t off;
		do{
			off=sll_platform_file_read(fd,bf,LIBRARY_HASH_BUFFER_SIZE,NULL);
			if (off==SLL_NO_FILE_SIZE){
				sll_platform_file_close(fd);
				return 0;
			}
			sll_hash_sha256(&sha,bf,off&0xffffffffffffffc0ll);
		} while (off==LIBRARY_HASH_BUFFER_SIZE);
		sll_platform_file_close(fd);
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
		if (((((__SLL_U64)sha.a)<<32)|sha.b)!=((__SLL_U64)c)||((((__SLL_U64)sha.c)<<32)|sha.d)!=((__SLL_U64)d)||((((__SLL_U64)sha.e)<<32)|sha.f)!=((__SLL_U64)e)||((((__SLL_U64)sha.g)<<32)|sha.h)!=((__SLL_U64)f)){
			return 0;
		}
	}
	sll_library_handle_t h=sll_platform_load_library(fp.v);
	if (h==SLL_UNKNOWN_LIBRARY_HANDLE){
		sll_free_string(&fp);
		return 0;
	}
	sll_bool_t (*fn)(sll_version_t)=sll_platform_lookup_symbol(h,SLL_CHAR("__sll_load"));
	if (!fn||!fn(SLL_VERSION)){
		sll_platform_unload_library(h);
		sll_free_string(&fp);
		return 0;
	}
	_sys_lhl++;
	_sys_lh=sll_reallocate(_sys_lh,_sys_lhl*sizeof(library_t*));
	library_t* n=sll_allocate(sizeof(library_t));
	sll_copy_data(&fp,sizeof(sll_string_t),(sll_string_t*)(&(n->nm)));
	n->h=h;
	*(_sys_lh+_sys_lhl-1)=n;
	if (!_sys_end){
		sll_register_cleanup(_sys_free_data);
		_sys_end=1;
	}
	return 1;
}



__API_FUNC(sys_remove_env){
	if (!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_ENVIRONMENT)){
		sll_remove_environment_variable(a);
	}
}



__API_FUNC(sys_set_env){
	if (!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_ENVIRONMENT)){
		sll_set_environment_variable(a,b);
	}
}
