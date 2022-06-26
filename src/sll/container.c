#include <sll/_internal/common.h>
#include <sll/allocator.h>
#include <sll/common.h>
#include <sll/container.h>
#include <sll/data.h>



__SLL_EXTERNAL void sll_container_clear(sll_container_t* c){
	SLL_CONTAINER_CLEAR(c);
}



__SLL_EXTERNAL void sll_container_filter(sll_container_t* c,sll_size_t elem_size,sll_container_check_callback_t check,sll_container_callback_t delete){
	SLL_ASSERT(elem_size);
	sll_size_t i=0;
	for (sll_size_t j=0;j<c->size;j++){
		void* var=PTR(ADDR(c->data)+j*elem_size);
		if (!check(var)){
			delete(var);
		}
		else{
			sll_copy_data(PTR(ADDR(c->data)+i*elem_size),elem_size,PTR(ADDR(c->data)+j*elem_size));
			i++;
		}
	}
	if (i!=c->size){
		c->size=i;
		sll_allocator_resize(&(c->data),i*elem_size);
	}
}



__SLL_EXTERNAL void sll_container_init(sll_container_t* c){
	SLL_CONTAINER_INIT(c);
}



__SLL_EXTERNAL void sll_container_iter(sll_container_t* c,sll_size_t elem_size,sll_container_callback_t callback){
	for (sll_size_t i=0;i<c->size;i++){
		callback(PTR(ADDR(c->data)+i*elem_size));
	}
}



__SLL_EXTERNAL void sll_container_iter_clear(sll_container_t* c,sll_size_t elem_size,sll_container_callback_t callback){
	for (sll_size_t i=0;i<c->size;i++){
		callback(PTR(ADDR(c->data)+i*elem_size));
	}
	SLL_CONTAINER_CLEAR(c);
}



__SLL_EXTERNAL void sll_container_push(sll_container_t* c,const void* elem,sll_size_t elem_size){
	c->size++;
	sll_allocator_resize(&(c->data),c->size*elem_size);
	sll_copy_data(elem,elem_size,PTR(ADDR(c->data)+(c->size-1)*elem_size));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_handle_container_alloc(sll_handle_container_t* c){
	sll_size_t out;
	SLL_HANDLE_CONTAINER_ALLOC(c,&out);
	return out;
}



__SLL_EXTERNAL void sll_handle_container_clear(sll_handle_container_t* c){
	SLL_HANDLE_CONTAINER_CLEAR(c);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_handle_container_check(sll_handle_container_t* c,sll_size_t index){
	return SLL_HANDLE_CONTAINER_CHECK(c,index);
}



__SLL_EXTERNAL void sll_handle_container_init(sll_handle_container_t* c){
	SLL_HANDLE_CONTAINER_INIT(c);
}
