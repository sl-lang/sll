#ifndef __SLL__INTERNAL_OPTIMIZER_H__
#define __SLL__INTERNAL_OPTIMIZER_H__ 1
#include <sll/_internal/common.h>
#include <sll/node.h>
#include <sll/types.h>



#define OPTIMIZER_FUNTION(name) static __SLL_FORCE_INLINE sll_node_t* __optimizer_##name(sll_source_file_t* source_file,sll_node_t* node,sll_node_t*const* children,sll_node_t* parent)



#endif
