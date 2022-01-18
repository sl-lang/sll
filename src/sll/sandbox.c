#include <sll/common.h>
#include <sll/types.h>



static sll_sandbox_flags_t _util_sandbox_flags=0;



void _reset_sandbox(void){
	_util_sandbox_flags=0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_get_sandbox_flag(sll_sandbox_flags_t f){
	return !!(_util_sandbox_flags&f);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_sandbox_flags_t sll_get_sandbox_flags(void){
	return _util_sandbox_flags;
}



__SLL_EXTERNAL sll_sandbox_flags_t sll_set_sandbox_flags(sll_sandbox_flags_t f){
	_util_sandbox_flags|=f;
	return _util_sandbox_flags;
}
