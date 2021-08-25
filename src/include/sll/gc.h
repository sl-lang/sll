#ifndef __SLL_GC_H__
#define __SLL_GC_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_CREATE() sll_create_object()
#define SLL_ACQUIRE(x) (++(x)->rc)
#define SLL_RELEASE(x) sll_release_object((x))



__SLL_FUNC sll_runtime_object_t* sll_create_object(void);



__SLL_FUNC void sll_release_object(sll_runtime_object_t* o);



#endif
