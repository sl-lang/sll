#include <sll/_internal/common.h>
#include <sll/_internal/gc.h>
#include <sll/_internal/weakref.h>
#include <sll/common.h>
#include <sll/container.h>
#include <sll/gc.h>
#include <sll/init.h>
#include <sll/memory.h>
#include <sll/types.h>
#include <sll/weakref.h>



static sll_map_container_t _weakref_data;
static sll_bool_t _weakref_cb=0;
static sll_weak_reference_t _weakref_next=NULL;



static sll_weak_reference_t _create_new(addr_t object){
	if (!_weakref_next){
		sll_map_container_init(NULL,NULL,&_weakref_data);
	}
	_weakref_next=PTR(ADDR(_weakref_next)+1);
	weakref_object_data_t* data=sll_allocate(sizeof(weakref_object_data_t));
	data->object=object;
	data->destructor=NULL;
	data->ctx=NULL;
	sll_map_container_set(&_weakref_data,_weakref_next,data);
	return _weakref_next;
}



static sll_bool_t _execute_destructors(sll_weak_reference_t weak_reference,weakref_object_data_t* value,sll_object_t object){
	if (value->object!=ADDR(object)){
		return 0;
	}
	value->destructor(weak_reference,object,value->ctx);
	sll_deallocate(value);
	return 1;
}



void _weakref_cleanup_data(void){
	if (!_weakref_next){
		return;
	}
	sll_map_container_iter(&_weakref_data,sll_deallocate);
	sll_map_container_deinit(&_weakref_data);
	_weakref_cb=0;
	_weakref_next=NULL;
}



void _weakref_delete(sll_object_t object){
	SLL_ASSERT(object->_flags&GC_FLAG_HAS_WEAKREF);
	sll_map_container_filter(&_weakref_data,(sll_map_container_filter_callback_t)_execute_destructors,object);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_weak_reference_t sll_weakref_clone(sll_weak_reference_t weak_reference){
	if (!_weakref_next){
		return NULL;
	}
	weakref_object_data_t* data=sll_map_container_get(&_weakref_data,weak_reference,NULL);
	return (data?_create_new(data->object):NULL);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_weak_reference_t sll_weakref_create(sll_object_t object){
	object->_flags|=GC_FLAG_HAS_WEAKREF;
	return _create_new(ADDR(object));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_weakref_delete(sll_weak_reference_t weak_reference){
	if (!_weakref_next){
		return 0;
	}
	weakref_object_data_t* data=sll_map_container_delete(&_weakref_data,weak_reference,NULL);
	sll_deallocate(data);
	return !!data;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_weakref_get(sll_weak_reference_t weak_reference){
	if (!_weakref_next){
		return NULL;
	}
	weakref_object_data_t* data=sll_map_container_get(&_weakref_data,weak_reference,NULL);
	return (data?PTR(data->object):NULL);
}



__SLL_EXTERNAL void sll_weakref_set_callback(sll_weak_reference_t weak_reference,sll_weak_ref_destructor_t destructor,void* arg){
	if (!_weakref_next){
		return;
	}
	weakref_object_data_t* data=sll_map_container_get(&_weakref_data,weak_reference,NULL);
	if (data){
		data->destructor=destructor;
		data->ctx=arg;
	}
}
