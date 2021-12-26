#ifndef __EXT_DEBUG_VM_H__
#define __EXT_DEBUG_VM_H__ 1
#include <sll.h>



sll_object_t* debug_get_instruction_count(sll_object_t*const* al,sll_arg_count_t all);



sll_object_t* debug_get_instruction_index(sll_object_t*const* al,sll_arg_count_t all);



sll_object_t* debug_get_vm_config(sll_object_t*const* al,sll_arg_count_t all);



#endif
