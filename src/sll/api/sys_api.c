#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/init.h>
#include <sll/memory.h>
#include <sll/platform.h>
#include <sll/runtime_object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>



static sll_integer_t _sys_argc=0;
static sll_runtime_object_t** _sys_argv=NULL;
static sll_runtime_object_t _sys_e={0,SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_GC_ZERO_DEBUG_DATA_STRUCT,.dt={.s=SLL_INIT_STRING_STRUCT}};
static sll_runtime_object_t _sys_p={0,SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_GC_ZERO_DEBUG_DATA_STRUCT,.dt={.s=SLL_INIT_STRING_STRUCT}};
static char _sys_end=0;



static void _sys_free_data(void){
	if (_sys_argv){
		for (sll_integer_t i=0;i<_sys_argc;i++){
			SLL_RELEASE(*(_sys_argv+i));
		}
		sll_deallocate(_sys_argv);
		_sys_argv=NULL;
	}
	if (_sys_e.dt.s.l){
		sll_deinit_string(&(_sys_e.dt.s));
		_sys_e.dt.s.l=0;
	}
	if (_sys_p.dt.s.l){
		sll_deinit_string(&(_sys_p.dt.s));
		_sys_p.dt.s.l=0;
	}
}



__SLL_EXTERNAL void sll_set_argument(sll_integer_t i,const sll_char_t* a){
	if (i<0||i>=_sys_argc){
		return;
	}
	SLL_RELEASE(*(_sys_argv+i));
	sll_runtime_object_t* n=SLL_CREATE();
	n->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
	sll_string_from_pointer(a,&(n->dt.s));
	*(_sys_argv+i)=n;
}



__SLL_EXTERNAL void sll_set_argument_count(sll_integer_t ac){
	SLL_ASSERT(ac>0);
	if (_sys_argv){
		for (sll_integer_t i=0;i<_sys_argc;i++){
			SLL_RELEASE(*(_sys_argv+i));
		}
		sll_deallocate(_sys_argv);
	}
	else if (!_sys_end){
		sll_register_cleanup(_sys_free_data);
		_sys_end=1;
	}
	_sys_argc=ac;
	_sys_argv=sll_allocate(ac*sizeof(sll_runtime_object_t*));
	for (sll_integer_t i=0;i<ac;i++){
		*(_sys_argv+i)=SLL_ACQUIRE_STATIC(str_zero);
	}
}



__API_FUNC(sys_arg_get){
	if (a<0||a>=_sys_argc){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	SLL_ACQUIRE(*(_sys_argv+a));
	return *(_sys_argv+a);
}



__API_FUNC(sys_arg_get_count){
	return _sys_argc;
}



__API_FUNC(sys_get_executable){
	if (!_sys_e.dt.s.l){
		sll_string_from_pointer(sll_platform_string,&(_sys_e.dt.s));
		if (!_sys_end){
			sll_register_cleanup(_sys_free_data);
			_sys_end=1;
		}
	}
	SLL_ACQUIRE(&_sys_e);
	return &_sys_e;
}



__API_FUNC(sys_get_platform){
	if (!_sys_p.dt.s.l){
		sll_string_from_pointer(sll_platform_string,&(_sys_p.dt.s));
		if (!_sys_end){
			sll_register_cleanup(_sys_free_data);
			_sys_end=1;
		}
	}
	SLL_ACQUIRE(&_sys_p);
	return &_sys_p;
}
