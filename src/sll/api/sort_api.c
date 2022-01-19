#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <sll/vm.h>



static void _quicksort_raw(const sll_object_t** a,sll_array_length_t l,sll_compare_result_t cmp){
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<l;j++){
		if (sll_operator_compare(*(a+j),*(a+l))==cmp){
			const sll_object_t* t=*(a+i);
			*(a+i)=*(a+j);
			*(a+j)=t;
			i++;
		}
	}
	const sll_object_t* t=*(a+i);
	*(a+i)=*(a+l);
	*(a+l)=t;
	if (i>1){
		_quicksort_raw(a,i-1,cmp);
	}
	i++;
	if (i<l){
		_quicksort_raw(a+i,l-i,cmp);
	}
}



static void _quicksort_extra(const sll_object_t** a,const sll_object_t** b,sll_array_length_t l,sll_compare_result_t cmp){
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<l;j++){
		if (sll_operator_compare(*(a+j),*(a+l))==cmp){
			const sll_object_t* t=*(a+i);
			*(a+i)=*(a+j);
			*(a+j)=t;
			t=*(b+i);
			*(b+i)=*(b+j);
			*(b+j)=t;
			i++;
		}
	}
	const sll_object_t* t=*(a+i);
	*(a+i)=*(a+l);
	*(a+l)=t;
	t=*(b+i);
	*(b+i)=*(b+l);
	*(b+l)=t;
	if (i>1){
		_quicksort_extra(a,b,i-1,cmp);
	}
	i++;
	if (i<l){
		_quicksort_extra(a+i,b+i,l-i,cmp);
	}
}



__SLL_EXTERNAL void sll_quicksort(sll_object_t** a,sll_array_length_t l,sll_compare_result_t cmp,sll_integer_t fn){
	if (!fn){
		_quicksort_raw((const sll_object_t**)a,l-1,cmp);
		return;
	}
	sll_object_t** tmp=sll_allocate_stack(l*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<l;i++){
		*(tmp+i)=sll_execute_function(fn,a+i,1);
	}
	_quicksort_extra((const sll_object_t**)tmp,(const sll_object_t**)a,l-1,cmp);
	for (sll_array_length_t i=0;i<l;i++){
		SLL_RELEASE(*(tmp+i));
	}
	sll_deallocate(tmp);
}



__API_FUNC(sort_quicksort){
	sll_object_t* o;
	sll_array_t* out;
	if (c){
		o=SLL_ACQUIRE_STATIC_INT(0);
		out=a;
	}
	else{
		o=SLL_CREATE();
		o->t=SLL_OBJECT_TYPE_ARRAY;
		out=&(o->dt.a);
		if (!sll_array_clone(a,out)){
			SLL_UNIMPLEMENTED();
		}
	}
	if (a->l>1){
		sll_quicksort(out->v,out->l,(b?SLL_COMPARE_RESULT_ABOVE:SLL_COMPARE_RESULT_BELOW),d);
	}
	return o;
}
