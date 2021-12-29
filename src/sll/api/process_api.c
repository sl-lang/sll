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
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <sll/vm.h>
#include <stdint.h>



static const uint64_t _process_quote_chars[4]={
	0xd80087dfffffffff,// %+,-.0123456789:=
	0xf800000178000000,// @ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz
	0xffffffffffffffff,
	0xffffffffffffffff
};
static sll_object_type_t _process_ret_type=0;



__SLL_EXTERNAL void sll_process_join_args(const sll_char_t*const* a,sll_string_t* o){
	sll_string_create(0,o);
	o->v=sll_memory_move(o->v,SLL_MEMORY_MOVE_DIRECTION_TO_STACK);
	SLL_ASSERT(*a);
	do{
		if (o->l){
			sll_string_increase(o,1);
			o->v[o->l]=' ';
			o->l++;
		}
		sll_string_t tmp;
		sll_string_from_pointer(*a,&tmp);
		a++;
		if (!tmp.l){
			sll_string_increase(o,2);
			o->v[o->l]='\'';
			o->v[o->l+1]='\'';
			o->l+=2;
			goto _continue;
		}
		for (sll_string_length_t i=0;i<tmp.l;i++){
			if (_process_quote_chars[tmp.v[i]>>6]&(1ull<<(tmp.v[i]&63))){
				goto _quote;
			}
		}
		sll_string_increase(o,tmp.l);
		sll_copy_data(tmp.v,tmp.l,o->v+o->l);
		o->l+=tmp.l;
		goto _continue;
_quote:
		sll_string_increase(o,1);
		o->v[o->l]='\'';
		o->l++;
		for (sll_string_length_t i=0;i<tmp.l;i++){
			if (tmp.v[i]=='\''){
				sll_string_increase(o,5);
				o->v[o->l]='\'';
				o->v[o->l+1]='"';
				o->v[o->l+2]='\'';
				o->v[o->l+3]='"';
				o->v[o->l+4]='\'';
				o->l+=5;
			}
			else{
				sll_string_increase(o,1);
				o->v[o->l]=tmp.v[i];
				o->l++;
			}
		}
		sll_string_increase(o,1);
		o->v[o->l]='\'';
		o->l++;
_continue:
		sll_free_string(&tmp);
	} while (*a);
	o->v=sll_memory_move(o->v,SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
}



__API_FUNC(process__init){
	_process_ret_type=(sll_object_type_t)a;
}



__API_FUNC(process_execute_shell){
	return sll_platform_execute_shell(a->v);
}



__API_FUNC(process_get_pid){
	return sll_platform_get_pid();
}



__API_FUNC(process_join){
	sll_char_t** dt=sll_allocate_stack(a->l*sizeof(sll_char_t*));
	for (sll_array_length_t i=0;i<a->l;i++){
		sll_object_t* n=sll_operator_cast(a->v[i],sll_static_int[SLL_OBJECT_TYPE_STRING]);
		sll_char_t* p=sll_allocate_stack(n->dt.s.l);
		sll_copy_data(n->dt.s.v,n->dt.s.l,p);
		SLL_RELEASE(n);
		*(dt+i)=p;
	}
	sll_process_join_args((const sll_char_t*const*)dt,out);
	for (sll_array_length_t i=0;i<a->l;i++){
		sll_deallocate(*(dt+i));
	}
	sll_deallocate(dt);
}



__API_FUNC(process_split){
	SLL_UNIMPLEMENTED();
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
