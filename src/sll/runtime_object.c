#include <sll/common.h>
#include <sll/runtime_object.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_type_t sll_runtime_object_get_type(const sll_runtime_object_t* o){
	return SLL_RUNTIME_OBJECT_GET_TYPE(o);
}
