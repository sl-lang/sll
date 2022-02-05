#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/common.h>
#include <sll/operator.h>
#include <sll/sandbox.h>
#include <sll/types.h>



__API_FUNC(float_get_compare_error){
	return sll_float_compare_error;
}



__API_FUNC(float_set_compare_error){
	if (!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FLOAT_COMPARE_ERROR_CHANGE)){
		sll_float_compare_error=a;
	}
}
