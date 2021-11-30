#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/path.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/platform.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>



static sll_integer_t _sys_argc=0;
static sll_string_t* _sys_argv=NULL;
static sll_string_t _sys_e=SLL_INIT_STRING_STRUCT;
static sll_string_t _sys_p=SLL_INIT_STRING_STRUCT;
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
}



__SLL_EXTERNAL void sll_set_argument(sll_integer_t i,const sll_char_t* a){
	if (i<0||i>=_sys_argc){
		return;
	}
	sll_free_string(_sys_argv+i);
	SLL_CHECK_NO_MEMORY(sll_string_from_pointer(a,_sys_argv+i));
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
	SLL_CHECK_NO_MEMORY(_sys_argv);
	for (sll_integer_t i=0;i<ac;i++){
		SLL_INIT_STRING(_sys_argv+i);
	}
}



__API_FUNC(sys_arg_get){
	if (a<0||a>=_sys_argc){
		SLL_INIT_STRING(out);
	}
	else{
		SLL_CHECK_NO_MEMORY(sll_string_clone(_sys_argv+a,out));
	}
}



__API_FUNC(sys_arg_get_count){
	return _sys_argc;
}



__API_FUNC(sys_get_executable){
	if (!_sys_e.l){
		sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
		sll_string_length_t bfl=sll_platform_get_executable_file_path(bf,SLL_API_MAX_FILE_PATH_LENGTH);
		SLL_CHECK_NO_MEMORY(sll_string_from_pointer_length(bf,bfl,&_sys_e));
		if (!_sys_end){
			sll_register_cleanup(_sys_free_data);
			_sys_end=1;
		}
	}
	SLL_CHECK_NO_MEMORY(sll_string_clone(&_sys_e,out));
}



__API_FUNC(sys_get_platform){
	if (!_sys_p.l){
		SLL_CHECK_NO_MEMORY(sll_string_from_pointer(sll_platform_string,&_sys_p));
		if (!_sys_end){
			sll_register_cleanup(_sys_free_data);
			_sys_end=1;
		}
	}
	SLL_CHECK_NO_MEMORY(sll_string_clone(&_sys_p,out));
}
