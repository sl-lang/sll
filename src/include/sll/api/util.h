#ifndef __SLL_API_UTIL_H__
#define __SLL_API_UTIL_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Utility API
 * \group util-api
 * \desc Docs!
 */



/**
 * \flags api func optimizable
 * \name sll_api_util_instruction_count
 * \group util-api
 * \desc Docs!
 * \ret I -> Number of instruction executed by the VM from the start of the program
 */
__API_FUNC_DECL(util_instruction_count);



/**
 * \flags api func optimizable
 * \name sll_api_util_ref_count
 * \group util-api
 * \desc Docs!
 * \arg O
 * \ret 0 -> Failure
 * \ret I -> Number of references to the given object
 */
__API_FUNC_DECL(util_ref_count);



#endif
