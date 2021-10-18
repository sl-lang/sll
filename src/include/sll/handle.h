#ifndef __SLL_HANDLE_H__
#define __SLL_HANDLE_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_FROM_HANDLE(t,v) __SLL_ADD_DEBUG_DATA(sll_handle_to_object((t),(v)),__SLL_DEBUG_TYPE_CREATE)
#define SLL_HANDLE_DESCRIPTOR_HEADER(nm) nm,0,0
#define SLL_HANDLE_FREE 0xffffffffffffffff
#define SLL_HANDLE_LOOKUP_DESCRIPTOR(hl,t) (*((hl)->dt+(t)-1))
#define SLL_HANDLE_UNKNOWN_TYPE 0
#define SLL_MAX_HANDLE 0xfffffffffffffffe
#define SLL_ZERO_HANDLE(o) \
	do{ \
		(o)->t=SLL_HANDLE_UNKNOWN_TYPE; \
		(o)->h=0; \
	} while (0)
#define SLL_ZERO_HANDLE_STRUCT {SLL_HANDLE_UNKNOWN_TYPE,0}



__SLL_FUNC void sll_cleanup_handles(sll_handle_list_t* hl,sll_handle_type_t hll);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_handle_type_t sll_create_handle(sll_handle_list_t* hl,sll_handle_descriptor_t* hd);



__SLL_FUNC void sll_free_handle_list(sll_handle_list_t* hl);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_handle_to_object(sll_handle_type_t t,sll_handle_t h);



__SLL_FUNC void sll_init_handle_list(sll_handle_list_t* o);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_handle_descriptor_t* sll_handle_lookup_descriptor(sll_handle_list_t* hl,sll_handle_type_t t);



#endif
