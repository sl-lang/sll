#include <sll/common.h>
#include <sll/object.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_object_get_type(const sll_object_t* o){
	return SLL_OBJECT_GET_TYPE(o);
}
