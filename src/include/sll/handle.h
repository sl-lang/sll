#ifndef __SLL_HANDLE_H__
#define __SLL_HANDLE_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_LOOKUP_HANDLE_DESCRIPTOR(hl,t) (*((hl)->dt+(t)-1))
#ifdef DEBUG_BUILD
#define SLL_FROM_HANDLE(t,v) sll__add_debug_data(sll_handle_to_object((t),(v)),__FILE__,__LINE__,__func__);
#else
#define SLL_FROM_HANDLE(t,v) sll_handle_to_object((t),(v))
#endif
#define SLL_ZERO_HANDLE_STRUCT {0,0}



__SLL_FUNC __SLL_RETURN_HANDLE_TYPE sll_create_handle(sll_handle_list_t* hl,const sll_char_t* nm,sll_handle_destructor_t df);



__SLL_FUNC void sll_free_handle_list(sll_handle_list_t* hl);



__SLL_FUNC void sll_init_handle_list(sll_handle_list_t* o);



__SLL_FUNC sll_runtime_object_t* sll_handle_to_object(sll_handle_type_t t,sll_handle_t h);



#endif
