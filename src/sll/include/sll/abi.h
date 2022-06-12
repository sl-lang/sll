#ifndef __SLL_ABI_H__
#define __SLL_ABI_H__ 1
#include <sll/string.h>
/**
 * \flags group
 * \name Library ABI
 * \group abi
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_ABI_AUDIT_CALL
 * \group abi
 * \desc Docs!
 * \type __identifier__
 */
#define SLL_ABI_AUDIT_CALL __sll_audit



/**
 * \flags macro var
 * \name SLL_ABI_AUDIT_DEINIT
 * \group abi
 * \desc Docs!
 * \type __identifier__
 */
#define SLL_ABI_AUDIT_DEINIT __sll_audit_deinit



/**
 * \flags macro var
 * \name SLL_ABI_AUDIT_INIT
 * \group abi
 * \desc Docs!
 * \type __identifier__
 */
#define SLL_ABI_AUDIT_INIT __sll_audit_init



/**
 * \flags macro var
 * \name SLL_ABI_DEINIT
 * \group abi
 * \desc Docs!
 * \type __identifier__
 */
#define SLL_ABI_DEINIT __sll_deinit



/**
 * \flags macro var
 * \name SLL_ABI_INIT
 * \group abi
 * \desc Docs!
 * \type __identifier__
 */
#define SLL_ABI_INIT __sll_init



/**
 * \flags macro var
 * \name SLL_ABI_INTERNAL_FUNCTION_TABLE_DESCRIPTOR
 * \group abi
 * \desc Docs!
 * \type __identifier__
 */
#define SLL_ABI_INTERNAL_FUNCTION_TABLE_DESCRIPTOR __sll_ift



/**
 * \flags macro var
 * \name SLL_ABI_PATH_RESOLVER_DEINIT
 * \group abi
 * \desc Docs!
 * \type __identifier__
 */
#define SLL_ABI_PATH_RESOLVER_DEINIT __sll_path_resolver_deinit



/**
 * \flags macro var
 * \name SLL_ABI_PATH_RESOLVER_INIT
 * \group abi
 * \desc Docs!
 * \type __identifier__
 */
#define SLL_ABI_PATH_RESOLVER_INIT __sll_path_resolver_init



/**
 * \flags macro var
 * \name SLL_ABI_PATH_RESOLVER_RESOLVE
 * \group abi
 * \desc Docs!
 * \type __identifier__
 */
#define SLL_ABI_PATH_RESOLVER_RESOLVE __sll_path_resolver



/**
 * \flags func macro
 * \name SLL_ABI_NAME
 * \group abi
 * \desc Docs!
 * \arg __identifier__ x
 * \ret const sll_char_t*
 */
#define _SLL_ABI_NAME(x) #x
#define SLL_ABI_NAME(x) SLL_CHAR(_SLL_ABI_NAME(x))



#endif
