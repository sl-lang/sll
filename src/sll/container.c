#include <sll/_internal/common.h>
#include <sll/common.h>
#include <sll/container.h>



__SLL_EXTERNAL void sll_container_clear(sll_container_t* c){
	SLL_CONTAINER_CLEAR(c);
}



__SLL_EXTERNAL void sll_container_filter(sll_container_t* c,sll_size_t elem_size,sll_container_check_callback_t check,sll_container_callback_t delete){
	SLL_ASSERT(elem_size);
	sll_size_t i=0;
	for (sll_size_t j=0;j<c->size;j++){
		void* var=c->data+j;
		if (!check(var)){
			delete(var);
		}
		else{
			*(c->data+i)=*(c->data+j);
			i++;
		}
	};
	if (i!=c->size){
		c->size=i;
		sll_allocator_resize((void**)(&(c->data)),i*elem_size);
	}
}



__SLL_EXTERNAL void sll_container_init(sll_container_t* c){
	SLL_CONTAINER_INIT(c);
}



__SLL_EXTERNAL void sll_container_iter(sll_container_t* c,sll_container_callback_t callback){
	for (sll_size_t i=0;i<c->size;i++){
		callback(c->data+i);
	}
}



__SLL_EXTERNAL void sll_container_iter_clear(sll_container_t* c,sll_container_callback_t callback){
	for (sll_size_t i=0;i<c->size;i++){
		callback(c->data+i);
	}
	SLL_CONTAINER_CLEAR(c);
}
