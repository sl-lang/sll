#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/types.h>



__API_FUNC(array_count){
	return sll_array_count(a,b);
}



__API_FUNC(array_create){
	sll_array_create_zero((a<0?0:(a>SLL_MAX_ARRAY_LENGTH?SLL_MAX_ARRAY_LENGTH:(sll_array_length_t)a)),out);
}



__API_FUNC(array_replace){
	sll_array_replace(a,b,(sll_object_t*)c,out);
}



__API_FUNC(array_reverse){
	sll_array_reverse(a,out);
}
