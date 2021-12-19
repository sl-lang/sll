#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/path.h>
#include <sll/api/sys.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/platform.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <sll/version.h>
#include <sll/vm.h>



static sll_integer_t _sys_argc=0;
static sll_string_t* _sys_argv=NULL;
static sll_string_t _sys_e=SLL_INIT_STRING_STRUCT;
static sll_string_t _sys_p=SLL_INIT_STRING_STRUCT;
static library_t** _sys_lh=NULL;
static sll_array_length_t _sys_lhl=0;
static char _sys_end=0;



static void _sys_free_data(void){
	if (_sys_argv){
		for (sll_integer_t i=0;i<_sys_argc;i++){
			sll_free_string(_sys_argv+i);
		}
		sll_deallocate(_sys_argv);
		_sys_argv=NULL;
	}
	if (_sys_e.l){
		sll_free_string(&_sys_e);
		_sys_e.l=0;
	}
	if (_sys_p.l){
		sll_free_string(&_sys_p);
		_sys_p.l=0;
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



__SLL_EXTERNAL void sll_set_argument(sll_integer_t i,const sll_char_t* a){
	if (i<0||i>=_sys_argc){
		return;
	}
	sll_free_string(_sys_argv+i);
	sll_string_from_pointer(a,_sys_argv+i);
}



__SLL_EXTERNAL void sll_set_argument_count(sll_integer_t ac){
	SLL_ASSERT(ac>0);
	if (_sys_argv){
		for (sll_integer_t i=0;i<_sys_argc;i++){
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
	for (sll_integer_t i=0;i<ac;i++){
		SLL_INIT_STRING(_sys_argv+i);
	}
}



__API_FUNC(sys_arg_get){
	if (a<0||a>=_sys_argc){
		SLL_INIT_STRING(out);
	}
	else{
		sll_string_clone(_sys_argv+a,out);
	}
}



__API_FUNC(sys_arg_get_count){
	return _sys_argc;
}



__API_FUNC(sys_get_executable){
	if (!_sys_e.l){
		sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
		sll_string_length_t bfl=sll_platform_get_executable_file_path(bf,SLL_API_MAX_FILE_PATH_LENGTH);
		sll_string_from_pointer_length(bf,bfl,&_sys_e);
		if (!_sys_end){
			sll_register_cleanup(_sys_free_data);
			_sys_end=1;
		}
	}
	sll_string_clone(&_sys_e,out);
}



__API_FUNC(sys_get_platform){
	if (!_sys_p.l){
		sll_string_from_pointer(sll_platform_string,&_sys_p);
		if (!_sys_end){
			sll_register_cleanup(_sys_free_data);
			_sys_end=1;
		}
	}
	sll_string_clone(&_sys_p,out);
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
	sll_string_length_t fpl=sll_path_absolute(a->v,fp,SLL_API_MAX_FILE_PATH_LENGTH);
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
