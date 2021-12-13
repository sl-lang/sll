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
