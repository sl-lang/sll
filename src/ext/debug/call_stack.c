#include <debug/util.h>
#include <sll.h>



sll_object_t* debug_get_call_stack(sll_object_t*const* al,sll_arg_count_t all){
	sll_call_stack_size_t off=(all&&SLL_OBJECT_GET_TYPE(*al)==SLL_OBJECT_TYPE_INT&&(*al)->dt.i>0?(sll_call_stack_size_t)((*al)->dt.i):0);
	sll_object_t* o=SLL_CREATE();
	o->t=SLL_OBJECT_TYPE_ARRAY;
	const sll_call_stack_t* c_st=sll_get_call_stack();
	if (off>=c_st->l){
		SLL_INIT_ARRAY(&(o->dt.a));
		return o;
	}
	sll_array_create(c_st->l-off,&(o->dt.a));
	for (sll_call_stack_size_t i=0;i<c_st->l-off;i++){
		const sll_call_stack_frame_t* k=c_st->dt+i;
		sll_object_t* nm=SLL_CREATE();
		nm->t=SLL_OBJECT_TYPE_STRING;
		sll_string_from_pointer(k->nm,&(nm->dt.s));
		sll_object_t* r_dt=SLL_CREATE();
		r_dt->t=SLL_OBJECT_TYPE_ARRAY;
		sll_array_create(2,&(r_dt->dt.a));
		r_dt->dt.a.v[0]=SLL_FROM_INT(k->_ii);
		r_dt->dt.a.v[1]=SLL_FROM_INT(k->_s);
		sll_object_t* c_st_f=SLL_CREATE();
		c_st_f->t=SLL_OBJECT_TYPE_ARRAY;
		sll_array_create(3,&(c_st_f->dt.a));
		c_st_f->dt.a.v[0]=nm;
		c_st_f->dt.a.v[1]=ii_to_loc(k->_ii);
		c_st_f->dt.a.v[2]=r_dt;
		o->dt.a.v[i]=c_st_f;
	}
	return o;
}
