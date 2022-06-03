#include <clib/common.h>
#include <clib/function.h>
#include <sll.h>



__CLIB_API_CALL sll_object_t* clib_api_call_function(void* address,sll_array_t* args){
	return sll_int_to_object(_call_function());
}
