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



static void _quicksort(sll_object_t* elements,sll_array_length_t length,sll_compare_result_t cmp){
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<length;j++){
		if (sll_operator_compare(*(elements+j),*(elements+length))==cmp){
			sll_object_t tmp=*(elements+i);
			*(elements+i)=*(elements+j);
			*(elements+j)=t;
			i++;
		}
	}
	sll_object_t tmp=*(elements+i);
	*(elements+i)=*(elements+length);
	*(elements+length)=t;
	if (i>1){
		_quicksort(elements,i-1,cmp);
	}
	i++;
	if (i<length){
		_quicksort(elements+i,length-i,cmp);
	}
}



static void _quicksort_with_keys(sll_object_t* keys,sll_object_t* elements,sll_array_length_t length,sll_compare_result_t cmp){
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<length;j++){
		if (sll_operator_compare(*(keys+j),*(keys+length))==cmp){
			sll_object_t t=*(elements+i);
			*(elements+i)=*(elements+j);
			*(elements+j)=t;
			t=*(keys+i);
			*(keys+i)=*(keys+j);
			*(keys+j)=t;
			i++;
		}
	}
	sll_object_t t=*(elements+i);
	*(elements+i)=*(elements+length);
	*(elements+length)=t;
	t=*(keys+i);
	*(keys+i)=*(keys+length);
	*(keys+length)=t;
	if (i>1){
		_quicksort_with_keys(keys,elements,i-1,cmp);
	}
	i++;
	if (i<length){
		_quicksort_with_keys(keys+i,elements+i,length-i,cmp);
	}
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t sll_api_sort_quicksort(sll_object_t arr,sll_bool_t reverse,sll_bool_t inplace,sll_integer_t key_fn){
	if (inplace){
		SLL_ACQUIRE(arr);
	}
	else{
		arr=sll_array_to_object(&(arr->data.array));
	}
	if (arr->data.array.length>1){
		sll_quicksort(arr->data.array.data,arr->data.array.length,(reverse?SLL_COMPARE_RESULT_ABOVE:SLL_COMPARE_RESULT_BELOW),key_fn);
	}
	return arr;
}



__SLL_EXTERNAL void sll_quicksort(sll_object_t* elements,sll_array_length_t length,sll_compare_result_t cmp,sll_integer_t key_fn){
	if (!key_fn){
		_quicksort(elements,length-1,cmp);
		return;
	}
	sll_object_t* keys=sll_allocate_stack(length*sizeof(sll_object_t));
	for (sll_array_length_t i=0;i<length;i++){
		*(keys+i)=sll_execute_function(key_fn,elements+i,1,0);
	}
	_quicksort_with_keys(keys,elements,length-1,cmp);
	for (sll_array_length_t i=0;i<length;i++){
		SLL_RELEASE(*(keys+i));
	}
	sll_deallocate(keys);
}
