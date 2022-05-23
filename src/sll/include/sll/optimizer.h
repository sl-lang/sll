#ifndef __SLL_OPTIMIZER_H__
#define __SLL_OPTIMIZER_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/node.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Optimizer
 * \group optimizer
 * \desc Docs!
 */



/**
 * \flags type var
 * \name sll_optimization_round_count_t
 * \group optimizer
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_optimization_round_count_t;



/**
 * \flags func
 * \name sll_optimize_source_file
 * \group optimizer
 * \desc Docs!
 * \arg sll_source_file_t* source_file
 * \arg sll_optimization_round_count_t round_count
 */
__SLL_EXTERNAL void sll_optimize_source_file(sll_source_file_t* source_file,sll_optimization_round_count_t round_count);



#endif
