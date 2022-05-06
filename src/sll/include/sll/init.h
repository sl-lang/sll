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
 * \flags macro var
 * \name SLL_CLEANUP_TYPE_GLOBAL
 * \group init
 * \desc Docs!
 * \type sll_bool_t
 */
#define SLL_CLEANUP_TYPE_GLOBAL 0



/**
 * \flags macro var
 * \name SLL_CLEANUP_TYPE_VM
 * \group init
 * \desc Docs!
 * \type sll_bool_t
 */
#define SLL_CLEANUP_TYPE_VM 1



/**
 * \flags func type
 * \name sll_cleanup_function_t
 * \group init
 * \desc Docs!
 */
typedef void (*sll_cleanup_function_t)(void);



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
 * \arg sll_cleanup_function_t fn
 * \arg sll_bool_t type
 */
__SLL_EXTERNAL void sll_register_cleanup(sll_cleanup_function_t fn,sll_bool_t type);



#endif
