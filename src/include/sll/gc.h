#ifndef __SLL_GC_H__
#define __SLL_GC_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#ifdef DEBUG_BUILD
#define SLL_CREATE() sll__add_debug_data(sll_create_object(),__FILE__,__LINE__,__func__);
#else
#define SLL_CREATE() sll_create_object()
#endif
#define SLL_ACQUIRE(x) (++(x)->rc)
#define SLL_RELEASE(x) sll_release_object((x))



__SLL_FUNC sll_runtime_object_t* sll__add_debug_data(sll_runtime_object_t* o,const char* fp,unsigned int ln,const char* fn);



__SLL_FUNC sll_runtime_object_t* sll_create_object(void);



__SLL_FUNC void sll_get_runtime_object_stack_data(sll_runtime_object_stack_data_t* o);



__SLL_FUNC void sll_release_object(sll_runtime_object_t* o);



__SLL_FUNC __SLL_RETURN sll_verify_runtime_object_stack_cleanup(const sll_runtime_object_stack_data_t* rst);



#endif
