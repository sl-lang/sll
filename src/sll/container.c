#include <sll/_internal/common.h>
#include <sll/allocator.h>
#include <sll/common.h>
#include <sll/container.h>
#include <sll/data.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_container_clear(sll_container_t* container){
	SLL_CONTAINER_CLEAR(container);
}



__SLL_EXTERNAL void sll_container_filter(sll_container_t* container,sll_size_t elem_size,sll_container_check_callback_t check,sll_container_callback_t delete){
	SLL_ASSERT(elem_size);
	sll_size_t i=0;
	for (sll_size_t j=0;j<container->size;j++){
		void* var=PTR(ADDR(container->data)+j*elem_size);
		if (!check(var)){
			delete(var);
		}
		else{
			sll_copy_data(PTR(ADDR(container->data)+i*elem_size),elem_size,PTR(ADDR(container->data)+j*elem_size));
			i++;
		}
	}
	if (i!=container->size){
		container->size=i;
		sll_allocator_resize(&(container->data),i*elem_size);
	}
}



__SLL_EXTERNAL void sll_container_init(sll_container_t* container){
	SLL_CONTAINER_INIT(container);
}



__SLL_EXTERNAL void sll_container_iter(sll_container_t* container,sll_size_t elem_size,sll_container_callback_t callback){
	for (sll_size_t i=0;i<container->size;i++){
		callback(PTR(ADDR(container->data)+i*elem_size));
	}
}



__SLL_EXTERNAL void sll_container_iter_clear(sll_container_t* container,sll_size_t elem_size,sll_container_callback_t callback){
	for (sll_size_t i=0;i<container->size;i++){
		callback(PTR(ADDR(container->data)+i*elem_size));
	}
	SLL_CONTAINER_CLEAR(container);
}



__SLL_EXTERNAL void sll_container_push(sll_container_t* container,const void* elem,sll_size_t elem_size){
	container->size++;
	sll_allocator_resize(&(container->data),container->size*elem_size);
	sll_copy_data(elem,elem_size,PTR(ADDR(container->data)+(container->size-1)*elem_size));
}
