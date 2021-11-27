#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_create_type(sll_object_type_table_t* tt,const sll_object_t*const* p,sll_arg_count_t l){
	SLL_ASSERT(tt->l+SLL_MAX_OBJECT_TYPE<SLL_OBJECT_TYPE_RESERVED2);
	tt->l++;
	tt->dt=sll_reallocate((void*)(tt->dt),tt->l*sizeof(const sll_object_type_data_t*));
	sll_object_type_data_t* n=sll_allocate(sizeof(sll_object_type_data_t)+l*sizeof(sll_object_type_data_entry_t));
	n->l=l;
	for (sll_arg_count_t i=0;i<l;i++){
		sll_object_t* v=sll_operator_cast((sll_object_t*)(*p),sll_static_int[SLL_OBJECT_TYPE_INT]);
		if (v->dt.i<0||v->dt.i>tt->l+SLL_MAX_OBJECT_TYPE-1){
			n->dt[i].t=SLL_OBJECT_TYPE_INT;
		}
		else{
			n->dt[i].t=(sll_object_type_t)(v->dt.i);
		}
		SLL_RELEASE(v);
		p++;
		v=sll_operator_cast((sll_object_t*)(*p),sll_static_int[SLL_OBJECT_TYPE_STRING]);
		sll_string_clone(&(v->dt.s),&(n->dt[i].nm));
		SLL_RELEASE(v);
		p++;
	}
	*(tt->dt+tt->l-1)=n;
	return tt->l+SLL_MAX_OBJECT_TYPE-1;
}



__SLL_EXTERNAL void sll_free_object_type_list(sll_object_type_table_t* tt){
	if (tt->l){
		for (sll_object_type_t i=0;i<tt->l;i++){
			const sll_object_type_data_t* k=*(tt->dt+i);
			for (sll_arg_count_t j=0;j<k->l;j++){
				sll_free_string((sll_string_t*)(&(k->dt[j].nm)));
			}
			sll_deallocate((void*)k);
		}
		sll_deallocate((void*)(tt->dt));
		tt->dt=NULL;
		tt->l=0;
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_object_get_type(const sll_object_t* o){
	return SLL_OBJECT_GET_TYPE(o);
}
