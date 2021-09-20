#ifndef __SLL_DEBUG_H__
#define __SLL_DEBUG_H__ 1
#include <sll/common.h>
#include <sll/types.h>



__SLL_FUNC void sll_debug_print_object(const sll_object_t* o,const sll_compilation_data_t* c_dt);



__SLL_FUNC void sll_debug_print_runtime_object(const sll_runtime_object_t* v);



#endif
