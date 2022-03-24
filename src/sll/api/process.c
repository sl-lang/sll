#include <sll/_internal/api.h>
#include <sll/_internal/common.h>
#include <sll/_internal/static_string.h>
#include <sll/allocator.h>
#include <sll/api.h>
#include <sll/api/process.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/memory.h>
#include <sll/new_object.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/platform/process.h>
#include <sll/sandbox.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/vm.h>



static const bitmap_t _process_quote_chars[4]={
	0xd80087dfffffffffull,// %+,-.0123456789:=
	0xf800000178000000ull,// @ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz
	0xffffffffffffffffull,
	0xffffffffffffffffull
};



__SLL_EXTERNAL void sll_process_join_args(const sll_char_t*const* a,sll_string_t* o){
	sll_string_create(0,o);
	sll_allocator_move((void**)(&(o->v)),SLL_MEMORY_MOVE_DIRECTION_TO_STACK);
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
			o->v[o->l]='"';
			o->v[o->l+1]='"';
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
		o->v[o->l]='"';
		o->l++;
		for (sll_string_length_t i=0;i<tmp.l;i++){
			if (tmp.v[i]=='"'){
				sll_string_increase(o,3);
				o->v[o->l]='"';
				o->v[o->l+1]='"';
				o->v[o->l+2]='"';
				o->l+=3;
			}
			else{
				sll_string_increase(o,1);
				o->v[o->l]=tmp.v[i];
				o->l++;
			}
		}
		sll_string_increase(o,1);
		o->v[o->l]='"';
		o->l++;
_continue:
		sll_free_string(&tmp);
	} while (*a);
	sll_allocator_move((void**)(&(o->v)),SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
}



__API_FUNC(process_execute_shell){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PROCESS_API)){
		return 0;
	}
	return sll_platform_execute_shell(a->v);
}



__API_FUNC(process_get_pid){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PROCESS_API)){
		return -1;
	}
	return sll_platform_get_pid();
}



__API_FUNC(process_join){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PROCESS_API)){
		SLL_INIT_STRING(out);
		return;
	}
	sll_char_t** dt=sll_allocate_stack(a->l*sizeof(sll_char_t*));
	for (sll_array_length_t i=0;i<a->l;i++){
		sll_object_t* n=sll_operator_cast(a->v[i],sll_static_int[SLL_OBJECT_TYPE_STRING]);
		sll_char_t* p=sll_allocate_stack(n->dt.s.l);
		sll_copy_data(n->dt.s.v,n->dt.s.l,p);
		sll_release_object(n);
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
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PROCESS_API)||!a->l){
		sll_new_object_array(SLL_CHAR("a(si)0(sZZ)"),out,a,b,c,d);
		return;
	}
	sll_char_t** args=sll_allocate((a->l+1)*sizeof(sll_char_t*));
	for (sll_array_length_t i=0;i<a->l;i++){
		sll_object_t* n=sll_operator_cast(a->v[i],sll_static_int[SLL_OBJECT_TYPE_STRING]);
		*(args+i)=sll_allocate((n->dt.s.l+1)*sizeof(sll_char_t));
		sll_copy_data(n->dt.s.v,n->dt.s.l+1,*(args+i));
		sll_release_object(n);
	}
	*(args+a->l)=NULL;
	sll_process_handle_t ph=sll_platform_start_process((const sll_char_t*const*)args,NULL);
	if (!ph){
		SLL_UNIMPLEMENTED();
	}
	if (!(c&SLL_PROCESS_FLAG_WAIT)){
		SLL_UNIMPLEMENTED();
	}
	sll_return_code_t rc=sll_platform_wait_process(ph);
	sll_platform_close_process_handle(ph);
	for (sll_array_length_t i=0;i<a->l;i++){
		sll_deallocate(*(args+i));
	}
	sll_deallocate(args);
	sll_new_object_array(SLL_CHAR("a(si)h(sZZ)"),out,a,b,c,rc,d);
}
