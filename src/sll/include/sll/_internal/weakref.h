#ifndef __SLL__INTERNAL_WEAKREF_H__
#define __SLL__INTERNAL_WEAKREF_H__ 1
#include <sll/gc.h>
#include <sll/types.h>
#include <sll/weakref.h>



typedef struct _WEAKREF_KEY_PAIR{
	addr_t object;
	sll_weak_reference_t wr;
	sll_weak_ref_destructor_t destructor;
	void* ctx;
} weakref_key_pair_t;



void _weakref_cleanup_data(void);



void _weakref_delete(sll_object_t* o);



#endif
