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
 * \arg sll_integer_t cb
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_audit__init(sll_integer_t cb);



/**
 * \flags func
 * \name sll_api_audit_audit
 * \group array-api
 * \desc Docs!
 * \api sso!|v
 * \arg const sll_string_t* name
 * \arg const sll_string_t* fmt
 * \arg sll_object_t*const* args
 * \arg sll_arg_count_t len
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_audit_audit(const sll_string_t* name,const sll_string_t* fmt,sll_object_t*const* args,sll_arg_count_t len);



#endif
