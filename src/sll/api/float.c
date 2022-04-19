#include <sll/audit.h>
#include <sll/common.h>
#include <sll/operator.h>
#include <sll/sandbox.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL sll_float_t sll_float_compare_error=1e-6;



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_float_get_compare_error(void){
	return sll_float_compare_error;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_float_set_compare_error(sll_float_t err){
	sll_float_t o=sll_float_compare_error;
	if (!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FLOAT_COMPARE_ERROR_CHANGE)){
		sll_audit(SLL_CHAR("sll.float.error.set"),SLL_CHAR("f"),err);
		sll_float_compare_error=err;
	}
	return o;
}
