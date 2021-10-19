#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/operator.h>
#include <sll/runtime_object.h>
#include <sll/static_object.h>
#include <sll/types.h>



static sll_string_length_t _sort_partition(sll_runtime_object_t** a,sll_string_length_t i,sll_string_length_t j){
	sll_runtime_object_t* p=*(a+j);
	sll_string_length_t o=i;
	while (i<j){
		if (sll_operator_compare(*(a+i),p)==SLL_COMPARE_RESULT_BELOW){
			sll_runtime_object_t* tmp=*(a+o);
			*(a+o)=*(a+i);
			*(a+i)=tmp;
			o++;
		}
		i++;
	}
	sll_runtime_object_t* tmp=*(a+o);
	*(a+o)=*(a+i);
	*(a+i)=tmp;
	return o;
}



static void _sort(sll_runtime_object_t** a,sll_string_length_t i,sll_string_length_t j){
	if (i<j){
		sll_string_length_t k=_sort_partition(a,i,j);
		if (k){
			_sort(a,i,k-1);
		}
		_sort(a,k+1,j);
	}
}



__API_FUNC(sort_sort){
	sll_runtime_object_t* o=SLL_CREATE();
	o->t=SLL_RUNTIME_OBJECT_TYPE_ARRAY;
	sll_array_create(a->l,&(o->dt.a));
	for (sll_array_length_t i=0;i<a->l;i++){
		o->dt.a.v[i]=a->v[i];
		SLL_ACQUIRE(a->v[i]);
	}
	_sort(o->dt.a.v,0,a->l-1);
	return o;
}
