#ifndef __SLL_GC_H__
#define __SLL_GC_H__ 1
#include <sll/common.h>
#include <sll/types.h>
#include <stdlib.h>



#define SLL_CREATE() malloc(sizeof(sll_runtime_object_t))
#define SLL_ACQUIRE(x) (++(x)->rc)
#define SLL_RELEASE(x) (--(x)->rc||(free((x)),0))
#define SLL_CREATE_STRUCT {1,SLL_RUNTIME_OBJECT_TYPE_INT,{.i=0}}



#endif
