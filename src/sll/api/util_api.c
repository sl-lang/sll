#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/static_object.h>
#include <sll/types.h>



__API_FUNC(util_ref_count){
	if (!ac){
		SLL_RETURN_ZERO;
	}
	return sll_int_to_object((*a)->rc);
}
