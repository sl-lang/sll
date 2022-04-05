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
 * \type <identifier>
 */
#define SLL_ABI_AUDIT_CALL __sll_audit



/**
 * \flags macro var
 * \name SLL_ABI_AUDIT_DEINIT
 * \group abi
 * \desc Docs!
 * \type <identifier>
 */
#define SLL_ABI_AUDIT_DEINIT __sll_audit_deinit



/**
 * \flags macro var
 * \name SLL_ABI_AUDIT_INIT
 * \group abi
 * \desc Docs!
 * \type <identifier>
 */
#define SLL_ABI_AUDIT_INIT __sll_audit_init



/**
 * \flags macro var
 * \name SLL_ABI_DEINIT
 * \group abi
 * \desc Docs!
 * \type <identifier>
 */
#define SLL_ABI_DEINIT __sll_deinit



/**
 * \flags macro var
 * \name SLL_ABI_INIT
 * \group abi
 * \desc Docs!
 * \type <identifier>
 */
#define SLL_ABI_INIT __sll_init



/**
 * \flags func macro
 * \name SLL_ABI_NAME
 * \group abi
 * \desc Docs!
 * \arg <identifier> x
 * \ret const sll_char_t*
 */
#define SLL_ABI_NAME(x) SLL_CHAR(#x)



#endif
