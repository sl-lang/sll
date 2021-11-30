#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/operator.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <sll/util.h>



__SLL_EXTERNAL void sll_quicksort(const sll_object_t** a,sll_array_length_t l,sll_compare_result_t cmp){
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
		sll_quicksort(a,i-1,cmp);
	}
	i++;
	if (i<l){
		sll_quicksort(a+i,l-i,cmp);
	}
}



__API_FUNC(sort_sort){
	if (!a->l){
		SLL_INIT_ARRAY(out);
		return;
	}
	SLL_CHECK_NO_MEMORY(sll_array_create(a->l,out));
	if (a->l==1){
		out->v[0]=a->v[0];
		SLL_ACQUIRE(a->v[0]);
		return;
	}
	sll_compare_result_t cmp=(b?SLL_COMPARE_RESULT_ABOVE:SLL_COMPARE_RESULT_BELOW);
	sll_string_length_t i=0;
	for (sll_array_length_t j=0;j<a->l;j++){
		out->v[j]=a->v[j];
		SLL_ACQUIRE(a->v[j]);
		if (j<a->l-1&&sll_operator_compare(a->v[j],a->v[a->l-1])==cmp){
			out->v[j]=out->v[i];
			out->v[i]=a->v[j];
			i++;
		}
	}
	out->v[a->l-1]=out->v[i];
	out->v[i]=a->v[a->l-1];
	if (i>1){
		sll_quicksort((const sll_object_t**)(out->v),i-1,cmp);
	}
	i++;
	if (i<a->l-1){
		sll_quicksort((const sll_object_t**)(out->v+i),a->l-i-1,cmp);
	}
}
