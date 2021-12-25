#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/process.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/platform.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <sll/util.h>
#include <sll/vm.h>



static sll_object_type_t _process_ret_type=0;



__API_FUNC(process__init){
	_process_ret_type=(sll_object_type_t)a;
}



__API_FUNC(process_execute_shell){
	return sll_platform_execute_shell(a->v);
}



__API_FUNC(process_get_pid){
	return sll_platform_get_pid();
}



__API_FUNC(process_start){
	SLL_ASSERT(_process_ret_type);
	if (!a->l){
		sll_object_t* oa=SLL_CREATE();
		oa->t=SLL_OBJECT_TYPE_ARRAY;
		if (!sll_array_create(0,&(oa->dt.a))){
			SLL_UNIMPLEMENTED();
		}
		sll_object_t* std=SLL_CREATE();
		std->t=SLL_OBJECT_TYPE_ARRAY;
		if (!sll_array_create(1,&(std->dt.a))){
			SLL_UNIMPLEMENTED();
		}
		sll_object_t* stdin=SLL_CREATE();
		stdin->t=SLL_OBJECT_TYPE_STRING;
		sll_string_clone(c,&(stdin->dt.s));
		std->dt.a.v[0]=stdin;
		sll_object_t* dt[]={
			oa,
			b,
			sll_static_int[0],
			std
		};
		sll_object_t* o=sll_create_object_type(sll_current_runtime_data->tt,_process_ret_type,dt,4);
		SLL_RELEASE(oa);
		SLL_RELEASE(std);
		return o;
	}
	sll_char_t** args=sll_allocate((a->l+1)*sizeof(sll_char_t*));
	for (sll_array_length_t i=0;i<a->l;i++){
		sll_object_t* n=sll_operator_cast(a->v[i],sll_static_int[SLL_OBJECT_TYPE_STRING]);
		*(args+i)=sll_allocate((n->dt.s.l+1)*sizeof(sll_char_t));
		sll_copy_data(n->dt.s.v,n->dt.s.l+1,*(args+i));
		SLL_RELEASE(n);
	}
	*(args+a->l)=NULL;
	sll_string_t nm;
	sll_string_from_pointer(SLL_CHAR("flags"),&nm);
	sll_object_t* fl=sll_object_get_field(sll_current_runtime_data->tt,b,&nm);
	sll_free_string(&nm);
	SLL_ASSERT(SLL_OBJECT_GET_TYPE(fl)==SLL_OBJECT_TYPE_INT);
	if (fl->dt.i&(SLL_PROCESS_FLAG_PASS_STDIN|SLL_PROCESS_FLAG_CAPTURE_STDOUT|SLL_PROCESS_FLAG_CAPTURE_STDERR)){
		SLL_UNIMPLEMENTED();
	}
	sll_process_handle_t ph=sll_platform_start_process((const sll_char_t*const*)args,NULL);
	if (!ph){
		SLL_UNIMPLEMENTED();
	}
	if (!(fl->dt.i&SLL_PROCESS_FLAG_WAIT)){
		SLL_UNIMPLEMENTED();
	}
	SLL_RELEASE(fl);
	sll_return_code_t rc=sll_platform_wait_process(ph);
	sll_platform_close_process_handle(ph);
	for (sll_array_length_t i=0;i<a->l;i++){
		sll_deallocate(*(args+i));
	}
	sll_deallocate(args);
	sll_object_t* oa=SLL_CREATE();
	oa->t=SLL_OBJECT_TYPE_ARRAY;
	if (!sll_array_clone(a,&(oa->dt.a))){
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* std=SLL_CREATE();
	std->t=SLL_OBJECT_TYPE_ARRAY;
	if (!sll_array_create(1,&(std->dt.a))){
		SLL_UNIMPLEMENTED();
	}
	sll_object_t* stdin=SLL_CREATE();
	stdin->t=SLL_OBJECT_TYPE_STRING;
	sll_string_clone(c,&(stdin->dt.s));
	std->dt.a.v[0]=stdin;
	sll_object_t* dt[]={
		oa,
		b,
		SLL_FROM_INT(rc),
		std
	};
	sll_object_t* o=sll_create_object_type(sll_current_runtime_data->tt,_process_ret_type,dt,4);
	SLL_RELEASE(oa);
	SLL_RELEASE(dt[2]);
	SLL_RELEASE(std);
	return o;
}
