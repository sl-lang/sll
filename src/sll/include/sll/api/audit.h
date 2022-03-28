#ifndef __SLL_API_AUDIT_H__
#define __SLL_API_AUDIT_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Audit API
 * \group audit-api
 * \desc Docs!
 */



/**
 * \flags api func var_arg
 * \name sll_api_audit_audit
 * \group array-api
 * \desc Docs!
 * \arg S
 * \arg S
 * \arg O
 */
__API_FUNC_DECL(audit_audit);



/**
 * \flags api func
 * \name sll_api_audit_register
 * \group array-api
 * \desc Docs!
 * \arg I
 */
__API_FUNC_DECL(audit_register);



/**
 * \flags api func
 * \name sll_api_audit_unregister
 * \group array-api
 * \desc Docs!
 * \arg I
 * \ret B
 */
__API_FUNC_DECL(audit_unregister);



#endif
