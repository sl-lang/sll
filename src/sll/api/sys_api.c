#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/path.h>
#include <sll/api/sys.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/platform.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <sll/version.h>
#include <sll/vm.h>



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
			void* fn=sll_platform_lookup_function(l->h,SLL_CHAR("__sll_unload"));
			if (fn){
				((void(*)(void))fn)();
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
	if (!sll_array_create(_sys_argc,out)){
		SLL_UNIMPLEMENTED();
	}
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
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_from_pointer_length(bf,sll_platform_get_executable_file_path(bf,SLL_API_MAX_FILE_PATH_LENGTH),out);
}



__API_FUNC(sys_get_library){
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_from_pointer_length(bf,sll_platform_get_library_file_path(bf,SLL_API_MAX_FILE_PATH_LENGTH),out);
}



__API_FUNC(sys_get_platform){
	sll_string_from_pointer(sll_platform_string,out);
}



__API_FUNC(sys_get_version){
	sll_object_t* dt[3]={
		SLL_FROM_INT(SLL_VERSION_MAJOR),
		SLL_FROM_INT(SLL_VERSION_MINOR),
		SLL_FROM_INT(SLL_VERSION_PATCH)
	};
	sll_object_t* o=sll_create_object_type(sll_current_runtime_data->tt,(sll_object_type_t)a,dt,3);
	SLL_RELEASE(dt[0]);
	SLL_RELEASE(dt[1]);
	SLL_RELEASE(dt[2]);
	return o;
}



__API_FUNC(sys_load_library){
	if (a->l>=SLL_API_MAX_FILE_PATH_LENGTH){
		return 0;
	}
	sll_char_t fp[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_length_t fpl=sll_platform_absolute_path(a->v,fp,SLL_API_MAX_FILE_PATH_LENGTH);
	if (!sll_platform_path_exists(fp)){
		return 0;
	}
	sll_string_t s;
	sll_string_from_pointer_length(fp,fpl,&s);
	for (sll_array_length_t i=0;i<_sys_lhl;i++){
		if (sll_string_equal(&((*(_sys_lh+i))->nm),&s)){
			return 1;
		}
	}
	sll_library_handle_t h=sll_platform_load_library(fp);
	if (h==SLL_UNKNOWN_LIBRARY_HANDLE){
		sll_free_string(&s);
		return 0;
	}
	void* fn=sll_platform_lookup_function(h,SLL_CHAR("__sll_load"));
	if (!fn||!((sll_bool_t (*)(sll_version_t))fn)(SLL_VERSION)){
		sll_platform_unload_library(h);
		sll_free_string(&s);
		return 0;
	}
	_sys_lhl++;
	_sys_lh=sll_reallocate(_sys_lh,_sys_lhl*sizeof(library_t*));
	library_t* n=sll_allocate(sizeof(library_t));
	sll_copy_data(&s,sizeof(sll_string_t),(sll_string_t*)(&(n->nm)));
	n->h=h;
	*(_sys_lh+_sys_lhl-1)=n;
	if (!_sys_end){
		sll_register_cleanup(_sys_free_data);
		_sys_end=1;
	}
	return 1;
}
