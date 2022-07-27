#include <sll/_internal/common.h>
#include <sll/common.h>
#include <sll/container.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_handle_container_alloc(sll_handle_container_t* container){
	sll_size_t out;
	SLL_HANDLE_CONTAINER_ALLOC(container,&out);
	return out;
}



__SLL_EXTERNAL void sll_handle_container_clear(sll_handle_container_t* container){
	SLL_HANDLE_CONTAINER_CLEAR(container);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_handle_container_check(sll_handle_container_t* container,sll_size_t index){
	return SLL_HANDLE_CONTAINER_CHECK(container,index);
}



__SLL_EXTERNAL void sll_handle_container_dealloc(sll_handle_container_t* container,sll_size_t index){
	SLL_ASSERT(SLL_HANDLE_CONTAINER_CHECK(container,index));
	SLL_HANDLE_CONTAINER_DEALLOC(container,index);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_handle_container_get(sll_handle_container_t* container,sll_size_t index){
	return SLL_HANDLE_CONTAINER_GET(container,index);
}



__SLL_EXTERNAL void sll_handle_container_init(sll_handle_container_t* container){
	SLL_HANDLE_CONTAINER_INIT(container);
}



__SLL_EXTERNAL void sll_handle_container_iter(sll_handle_container_t* container,sll_container_callback_t callback){
	SLL_HANDLE_CONTAINER_ITER(container,void,elem,{
		callback(elem);
	});
}



__SLL_EXTERNAL void sll_handle_container_iter_clear(sll_handle_container_t* container,sll_container_callback_t callback){
	SLL_HANDLE_CONTAINER_ITER_CLEAR(container,void,elem,{
		callback(elem);
	});
}
