#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/memory.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/handle.h>
#include <sll/object.h>
#include <sll/types.h>
#include <sll/vm.h>
#include <stdint.h>



#define SETUP_HANDLE \
	do{ \
		if (_memory_ht==SLL_HANDLE_UNKNOWN_TYPE){ \
			SLL_ASSERT(sll_current_runtime_data); \
			_memory_ht=sll_create_handle(sll_current_runtime_data->hl,&_memory_type); \
			_memory_null_ref=SLL_FROM_HANDLE(_memory_ht,0); \
		} \
	} while (0)



static sll_handle_type_t _memory_ht=SLL_HANDLE_UNKNOWN_TYPE;
static sll_object_t* _memory_null_ref=NULL;
static sll_handle_descriptor_t _memory_type;



static void _memory_cleanup(sll_handle_t h){
	if (h==SLL_HANDLE_FREE){
		SLL_RELEASE(_memory_null_ref);
		_memory_null_ref=NULL;
		_memory_ht=SLL_HANDLE_UNKNOWN_TYPE;
		return;
	}
}



static void _memory_stringify(sll_handle_t h,sll_string_t* o){
	sll_string_increase(o,17);
	uint8_t s=64;
	do{
		s-=4;
		uint8_t v=(h>>s)&15;
		o->v[o->l]=v+(v>9?87:48);
		o->l++;
		if (s==32){
			o->v[o->l]='\'';
			o->l++;
		}
	} while (s);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_memory_from_object(sll_object_t* v){
	SETUP_HANDLE;
	void* p=v;
	if (SLL_OBJECT_GET_TYPE(v)==SLL_OBJECT_TYPE_STRING){
		p=v->dt.s.v;
	}
	else if (SLL_OBJECT_GET_TYPE(v)==SLL_OBJECT_TYPE_ARRAY){
		p=v->dt.a.v;
	}
	else if (SLL_OBJECT_GET_TYPE(v)==SLL_OBJECT_TYPE_HANDLE&&v->dt.h.t==_memory_ht){
		SLL_ACQUIRE(v);
		return v;
	}
	else if (SLL_OBJECT_GET_TYPE(v)==SLL_OBJECT_TYPE_MAP){
		p=v->dt.m.v;
	}
	return sll_memory_from_pointer(p);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_memory_from_pointer(void* p){
	SETUP_HANDLE;
	if (!p){
		SLL_ACQUIRE(_memory_null_ref);
		return _memory_null_ref;
	}
	sll_object_t* o=SLL_CREATE();
	o->t=SLL_OBJECT_TYPE_HANDLE;
	o->dt.h.t=_memory_ht;
	o->dt.h.h=(sll_handle_t)p;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_memory_get_null_pointer(void){
	SETUP_HANDLE;
	SLL_ACQUIRE(_memory_null_ref);
	return _memory_null_ref;
}



static sll_handle_descriptor_t _memory_type={
	SLL_HANDLE_DESCRIPTOR_HEADER("sll_memory_handle"),
	_memory_stringify,
	_memory_cleanup,
	NULL
};
