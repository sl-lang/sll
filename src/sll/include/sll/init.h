#ifndef __SLL_INIT_H__
#define __SLL_INIT_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Initialization
 * \group init
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_deinit
 * \group init
 * \desc Docs!
 */
__SLL_EXTERNAL void sll_deinit(void);



/**
 * \flags func
 * \name sll_init
 * \group init
 * \desc Docs!
 */
__SLL_EXTERNAL void sll_init(void);



/**
 * \flags func
 * \name sll_register_cleanup
 * \group init
 * \desc Docs!
 * \arg sll_cleanup_function_t f
 */
__SLL_EXTERNAL void sll_register_cleanup(sll_cleanup_function_t f);



#endif
