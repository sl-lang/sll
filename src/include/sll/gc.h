#ifndef __SLL_GC_H__
#define __SLL_GC_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_CREATE() __SLL_ADD_DEBUG_DATA(sll_create_object(),__SLL_DEBUG_TYPE_CREATE)
#define SLL_ACQUIRE_NO_DEBUG(x) ((x)->rc++)
#define SLL_ACQUIRE(x) SLL_ACQUIRE_NO_DEBUG(__SLL_ADD_DEBUG_DATA((x),__SLL_DEBUG_TYPE_ACQUIRE))
#define SLL_RELEASE(x) sll_release_object(__SLL_ADD_DEBUG_DATA((x),__SLL_DEBUG_TYPE_RELEASE))
#define SLL_NO_DEBUG_DATA ._dbg0=0xffff,._dbg1=0xff
#define SLL_RUNTIME_OBJECT_STRUCT {1,SLL_RUNTIME_OBJECT_TYPE_INT,SLL_NO_DEBUG_DATA,.dt={.i=0}}



__SLL_FUNC sll_runtime_object_t* sll__add_debug_data(sll_runtime_object_t* o,const char* fp,unsigned int ln,const char* fn,uint8_t t);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_create_object(void);



__SLL_FUNC void sll_release_object(sll_runtime_object_t* o);



__SLL_FUNC void sll_remove_debug_data(sll_runtime_object_t* o);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_return_t sll_verify_runtime_object_stack_cleanup(void);



#endif
