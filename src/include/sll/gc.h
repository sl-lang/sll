#ifndef __SLL_GC_H__
#define __SLL_GC_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_ACQUIRE(x) (++((sll_gc_object_t*)(x))->rc)
#define SLL_RELEASE(x) (--((sll_gc_object_t*)(x))->rc||(free((x)),0))



__SLL_FUNC void sll_acquire_object(sll_runtime_object_t* o);



__SLL_FUNC void sll_release_object(sll_runtime_object_t* o);



#endif
