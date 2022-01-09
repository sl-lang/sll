#ifndef __EXT_DEBUG_UTIL_H__
#define __EXT_DEBUG_UTIL_H__ 1
#include <sll.h>



extern sll_object_type_t debug_cs_type;
extern sll_object_type_t debug_cs_raw_type;
extern sll_object_type_t debug_loc_type;
extern sll_object_type_t debug_vm_cfg_type;



sll_object_t* ii_to_loc(sll_instruction_index_t ii);



sll_object_t* debug_init(sll_object_t*const* al,sll_arg_count_t all);



sll_object_t* debug_get_ref_count(sll_object_t*const* al,sll_arg_count_t all);



#endif
