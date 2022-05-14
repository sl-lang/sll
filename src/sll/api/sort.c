#include <sll/api/sort.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <sll/vm.h>



static void _quicksort(sll_object_t** a,sll_array_length_t l,sll_compare_result_t cmp){
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<l;j++){
		if (sll_operator_compare(*(a+j),*(a+l))==cmp){
			sll_object_t* t=*(a+i);
			*(a+i)=*(a+j);
			*(a+j)=t;
			i++;
		}
	}
	sll_object_t* t=*(a+i);
	*(a+i)=*(a+l);
	*(a+l)=t;
	if (i>1){
		_quicksort(a,i-1,cmp);
	}
	i++;
	if (i<l){
		_quicksort(a+i,l-i,cmp);
	}
}



static void _quicksort_extra(sll_object_t** a,sll_object_t** b,sll_array_length_t l,sll_compare_result_t cmp){
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<l;j++){
		if (sll_operator_compare(*(a+j),*(a+l))==cmp){
			sll_object_t* t=*(a+i);
			*(a+i)=*(a+j);
			*(a+j)=t;
			t=*(b+i);
			*(b+i)=*(b+j);
			*(b+j)=t;
			i++;
		}
	}
	sll_object_t* t=*(a+i);
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



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sort_quicksort(sll_object_t* arr,sll_bool_t reverse,sll_bool_t inplace,sll_integer_t key_fn){
	if (inplace){
		SLL_ACQUIRE(arr);
	}
	else{
		arr=sll_array_to_object(&(arr->dt.a));
	}
	if (arr->dt.a.l>1){
		sll_quicksort(arr->dt.a.v,arr->dt.a.l,(reverse?SLL_COMPARE_RESULT_ABOVE:SLL_COMPARE_RESULT_BELOW),key_fn);
	}
	return arr;
}



__SLL_EXTERNAL void sll_quicksort(sll_object_t** a,sll_array_length_t l,sll_compare_result_t cmp,sll_integer_t fn){
	if (!fn){
		_quicksort(a,l-1,cmp);
		return;
	}
	sll_object_t** tmp=sll_allocate_stack(l*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<l;i++){
		*(tmp+i)=sll_execute_function(fn,a+i,1,0);
	}
	_quicksort_extra(tmp,a,l-1,cmp);
	for (sll_array_length_t i=0;i<l;i++){
		SLL_RELEASE(*(tmp+i));
	}
	sll_deallocate(tmp);
}
