#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/platform.h>
#include <sll/runtime_object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <stdlib.h>
#include <string.h>



static sll_integer_t _sys_argc=0;
static sll_runtime_object_t** _sys_argv=NULL;
static sll_runtime_object_t _sys_p={1,SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_NO_DEBUG_DATA,.dt={.s=SLL_ZERO_STRING_STRUCT}};



static void _sys_free_argv(void){
	if (_sys_argv){
		for (sll_integer_t i=0;i<_sys_argc;i++){
			SLL_RELEASE(*(_sys_argv+i));
		}
		free(_sys_argv);
		_sys_argv=NULL;
	}
}



__SLL_FUNC void sll_set_argument(sll_integer_t i,const sll_char_t* a){
	if (i<0||i>=_sys_argc){
		return;
	}
	SLL_RELEASE(*(_sys_argv+i));
	sll_runtime_object_t* n=SLL_CREATE();
	n->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
	sll_string_from_pointer(a,&(n->dt.s));
	*(_sys_argv+i)=n;
}



__SLL_FUNC void sll_set_argument_count(sll_integer_t ac){
	SLL_ASSERT(ac>0);
	if (_sys_argv){
		for (sll_integer_t i=0;i<_sys_argc;i++){
			SLL_RELEASE(*(_sys_argv+i));
		}
		free(_sys_argv);
	}
	else{
		sll_register_cleanup(_sys_free_argv,SLL_CLEANUP_ORDER_NORMAL);
	}
	_sys_argc=ac;
	_sys_argv=malloc(ac*sizeof(sll_runtime_object_t*));
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



__API_FUNC(sys_get_platform){
	if (!_sys_p.dt.s.l){
		sll_string_from_pointer(sll_platform_string,&(_sys_p.dt.s));
	}
	SLL_ACQUIRE(&_sys_p);
	return &_sys_p;
}
