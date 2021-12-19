#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
#include <sll/vm.h>



__API_FUNC(util_instruction_count){
	return sll_current_instruction_count;
}



__API_FUNC(util_ref_count){
	return a->rc;
}
