#ifndef __SLL__INTERNAL_WEAKREF_H__
#define __SLL__INTERNAL_WEAKREF_H__ 1
#include <sll/types.h>



typedef struct _WEAKREF_KEY_PAIR{
	addr_t obj;
	sll_weak_reference_t wr;
	sll_weak_ref_destructor_t cb;
	void* arg;
} weakref_key_pair_t;



void _weakref_cleanup_data(void);



void _weakref_delete(sll_object_t* o);



#endif
