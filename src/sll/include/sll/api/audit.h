#ifndef __SLL_API_AUDIT_H__
#define __SLL_API_AUDIT_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Audit API
 * \group audit-api
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_api_audit__init
 * \group array-api
 * \desc Docs!
 * \api i|v
 * \arg sll_integer_t a
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_audit__init(sll_integer_t a);



/**
 * \flags func
 * \name sll_api_audit_audit
 * \group array-api
 * \desc Docs!
 * \api sso!|v
 * \arg sll_string_t* a
 * \arg sll_string_t* b
 * \arg sll_object_t*const* c
 * \arg sll_arg_count_t cc
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_audit_audit(sll_string_t* a,sll_string_t* b,sll_object_t*const* c,sll_arg_count_t cc);



#endif
