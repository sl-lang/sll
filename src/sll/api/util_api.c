#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/static_object.h>
#include <sll/types.h>



__API_FUNC(util_ref_count){
	return SLL_FROM_INT(a->rc);
}
