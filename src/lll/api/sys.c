#include <lll/_lll_internal.h>
#include <lll/api.h>
#include <lll/constants.h>
#include <lll/core.h>
#include <lll/platform.h>
#include <lll/string.h>
#include <lll/types.h>
#include <stdlib.h>



lll_sys_arg_count_t _sys_argc=0;
lll_string_t** _sys_argv=NULL;



__LLL_FUNC void lll_set_argument_count(lll_sys_arg_count_t ac){
	if (_sys_argv){
		for (lll_sys_arg_count_t i=0;i<_sys_argc;i++){
			if (*(_sys_argv+i)!=&_zero_string){
				lll_string_release(*(_sys_argv+i));
			}
		}
		free(_sys_argv);
	}
	_sys_argc=ac;
	_sys_argv=malloc(ac*sizeof(lll_string_t*));
	for (lll_sys_arg_count_t i=0;i<ac;i++){
		*(_sys_argv+i)=&_zero_string;
	}
}



__LLL_FUNC void lll_set_argument(lll_sys_arg_count_t i,const char* a){
	if (i>=_sys_argc){
		return;
	}
	lll_string_length_t l=0;
	lll_string_checksum_t c=0;
	while (*(a+l)){
		c^=*(a+l);
		l++;
	}
	lll_string_t* s=lll_string_create(l);
	s->rc=1;
	s->c=c;
	for (lll_string_length_t j=0;j<l;j++){
		s->v[j]=*(a+j);
	}
	s->v[l]=0;
	*(_sys_argv+i)=s;
}



__API_FUNC(sys_arg_get){
	o->t=LLL_RUNTIME_OBJECT_TYPE_STRING;
	if (!ac||a->t!=LLL_RUNTIME_OBJECT_TYPE_INT||a->dt.i<0||a->dt.i>=_sys_argc){
		o->dt.s=&_zero_string;
	}
	else{
		o->dt.s=*(_sys_argv+a->dt.i);
	}
	o->dt.s->rc++;
}



__API_FUNC(sys_arg_get_count){
	o->t=LLL_RUNTIME_OBJECT_TYPE_INT;
	o->dt.i=_sys_argc;
}



__API_FUNC(sys_get_platform){
	lll_string_length_t l=0;
	lll_string_checksum_t c=0;
	while (*(lll_platform_string+l)){
		c^=*(lll_platform_string+l);
		l++;
	}
	lll_string_t* s=lll_string_create(l);
	s->rc=1;
	s->c=c;
	for (lll_string_length_t j=0;j<l;j++){
		s->v[j]=*(lll_platform_string+j);
	}
	s->v[l]=0;
	o->t=LLL_RUNTIME_OBJECT_TYPE_STRING;
	o->dt.s=s;
}
