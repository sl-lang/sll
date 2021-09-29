#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/handle.h>
#include <sll/types.h>
#include <stdint.h>



static sll_handle_type_t _memory_ht=SLL_UNKNOWN_HANDLE_TYPE;
static sll_handle_descriptor_t _memory_type;



static void _memory_cleanup(sll_handle_t h){
	if (h==SLL_HANDLE_FREE){
		_memory_ht=SLL_UNKNOWN_HANDLE_TYPE;
		return;
	}
}



static sll_string_length_t _memory_stringify(sll_handle_t h,sll_string_length_t i,sll_string_t* o){
	if (!o){
		return i+17;
	}
	uint8_t s=64;
	do{
		s-=4;
		uint8_t v=(h>>s)&15;
		o->v[i]=v+(v>9?87:48);
		i++;
		if (s==32){
			o->v[i]='\'';
			i++;
		}
	} while (s);
	return i;
}



__SLL_FUNC sll_runtime_object_t* sll_memory_from_pointer(void* p){
	if (_memory_ht==SLL_UNKNOWN_HANDLE_TYPE){
		SLL_ASSERT(sll_current_runtime_data);
		_memory_ht=sll_create_handle(sll_current_runtime_data->hl,&_memory_type);
	}
	sll_runtime_object_t* o=SLL_CREATE();
	o->t=SLL_RUNTIME_OBJECT_TYPE_HANDLE;
	o->dt.h.t=_memory_ht;
	o->dt.h.h=(sll_handle_t)p;
	return o;
}



static sll_handle_descriptor_t _memory_type={
	SLL_HANDLE_DESCRIPTOR_HEADER("sll_memory_handle"),
	_memory_stringify,
	_memory_cleanup
};
