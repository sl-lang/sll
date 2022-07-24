#ifndef __SLL__INTERNAL_WEAKREF_H__
#define __SLL__INTERNAL_WEAKREF_H__ 1
#include <sll/gc.h>
#include <sll/types.h>
#include <sll/weakref.h>



typedef struct _WEAKREF_OBJECT_DATA{
	addr_t object;
	sll_weak_ref_destructor_t destructor;
	void* ctx;
} weakref_object_data_t;



void _weakref_cleanup_data(void);



void _weakref_delete(sll_object_t o);



#endif
