#ifndef __SLL_AUDIT_H__
#define __SLL_AUDIT_H__ 1
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/var_arg.h>
/**
 * \flags group
 * \name Audit
 * \group audit
 * \desc Docs!
 */



/**
 * \flags func type
 * \name sll_audit_callback_t
 * \group audit
 * \desc Docs!
 * \arg const sll_string_t* name
 * \arg const sll_array_t* args
 */
typedef void (*sll_audit_callback_t)(const sll_string_t* name,const sll_array_t* args);



/**
 * \flags func var_arg
 * \name sll_audit
 * \group audit
 * \desc Docs!
 * \arg const sll_char_t* name
 * \arg const sll_char_t* format
 */
__SLL_EXTERNAL void sll_audit(const sll_char_t* name,const sll_char_t* format,...);



/**
 * \flags func
 * \name sll_audit_enable
 * \group audit
 * \desc Docs!
 * \arg sll_bool_t enable
 * \ret sll_bool_t
 */
__SLL_EXTERNAL sll_bool_t sll_audit_enable(sll_bool_t enable);



/**
 * \flags func
 * \name sll_audit_list
 * \group audit
 * \desc Docs!
 * \arg const sll_char_t* name
 * \arg const sll_char_t* format
 * \arg sll_var_arg_list_t* va
 */
__SLL_EXTERNAL void sll_audit_list(const sll_char_t* name,const sll_char_t* format,sll_var_arg_list_t* va);



/**
 * \flags func
 * \name sll_audit_register_callback
 * \group audit
 * \desc Docs!
 * \arg sll_audit_callback_t callback
 */
__SLL_EXTERNAL void sll_audit_register_callback(sll_audit_callback_t callback);



/**
 * \flags func
 * \name sll_audit_unregister_callback
 * \group audit
 * \desc Docs!
 * \arg sll_audit_callback_t callback
 * \ret sll_bool_t
 */
__SLL_EXTERNAL sll_bool_t sll_audit_unregister_callback(sll_audit_callback_t callback);



#endif
