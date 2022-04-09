#include <sll/_internal/api.h>
#include <sll/api.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/operator.h>
#include <sll/sandbox.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL sll_float_t sll_float_compare_error=1e-6;



__API_FUNC(float_get_compare_error){
	return sll_float_compare_error;
}



__API_FUNC(float_set_compare_error){
	if (!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FLOAT_COMPARE_ERROR_CHANGE)){
		sll_audit(SLL_CHAR("sll.float.error.set"),SLL_CHAR("f"),a);
		sll_float_compare_error=a;
	}
}
