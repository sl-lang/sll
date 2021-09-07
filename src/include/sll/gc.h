#ifndef __SLL_GC_H__
#define __SLL_GC_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_CREATE() __SLL_ADD_DEBUG_DATA(sll_create_object(),__SLL_DEBUG_TYPE_CREATE)
#define SLL_ACQUIRE(x) ((__SLL_ADD_DEBUG_DATA((x),__SLL_DEBUG_TYPE_ACQUIRE))->rc++)
#define SLL_RELEASE(x) sll_release_object(__SLL_ADD_DEBUG_DATA((x),__SLL_DEBUG_TYPE_RELEASE))
#define SLL_NO_DEBUG_DATA ._dbg0=0xffff,._dbg1=0xff



__SLL_FUNC sll_runtime_object_t* sll__add_debug_data(sll_runtime_object_t* o,const char* fp,unsigned int ln,const char* fn,uint8_t t);



__SLL_FUNC sll_runtime_object_t* sll_create_object(void);



__SLL_FUNC void sll_get_runtime_object_stack_data(sll_runtime_object_stack_data_t* o);



__SLL_FUNC void sll_release_object(sll_runtime_object_t* o);



__SLL_FUNC __SLL_RETURN sll_verify_runtime_object_stack_cleanup(const sll_runtime_object_stack_data_t* rst);



#endif
