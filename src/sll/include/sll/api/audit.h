#ifndef __SLL_API_AUDIT_H__
#define __SLL_API_AUDIT_H__ 1
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/string.h>
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
 * \api i|
 * \arg sll_integer_t callback
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_audit__init(sll_integer_t callback);



/**
 * \flags func
 * \name sll_api_audit_audit
 * \group array-api
 * \desc Docs!
 * \api #s#so!|
 * \arg const sll_string_t* name
 * \arg const sll_string_t* format
 * \arg const sll_object_t* args
 * \arg sll_arg_count_t arg_count
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_audit_audit(const sll_string_t* name,const sll_string_t* format,const sll_object_t* args,sll_arg_count_t arg_count);



#endif
