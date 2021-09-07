#ifndef __SLL_HANDLE_H__
#define __SLL_HANDLE_H__ 1
#include <sll/common.h>
#include <sll/constants.h>
#include <sll/types.h>



#define SLL_HANDLE_DESCRIPTOR_HEADER(nm) nm,0,0
#define SLL_LOOKUP_HANDLE_DESCRIPTOR_FAST(hl,t) (*((hl)->dt+(t)-1))
#define SLL_FROM_HANDLE(t,v) __SLL_ADD_DEBUG_DATA(sll_handle_to_object((t),(v)),__SLL_DEBUG_TYPE_CREATE)
#define SLL_ZERO_HANDLE_STRUCT {SLL_UNKNOWN_HANDLE_TYPE,0}



__SLL_FUNC void sll_cleanup_handles(sll_handle_list_t* hl,sll_handle_type_t hll);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_handle_type_t sll_create_handle(sll_handle_list_t* hl,sll_handle_descriptor_t* hd);



__SLL_FUNC void sll_free_handle_list(sll_handle_list_t* hl);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_handle_to_object(sll_handle_type_t t,sll_handle_t h);



__SLL_FUNC void sll_init_handle_list(sll_handle_list_t* o);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_handle_descriptor_t* sll_lookup_handle_descriptor(sll_handle_list_t* hl,sll_handle_type_t t);



#endif
