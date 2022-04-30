#ifndef __SLL_AUDIT_H__
#define __SLL_AUDIT_H__ 1
#include <sll/common.h>
#include <sll/types.h>
#include <sll/var_arg.h>
/**
 * \flags group
 * \name Audit
 * \group audit
 * \desc Docs!
 */



/**
 * \flags func var_arg
 * \name sll_audit
 * \group audit
 * \desc Docs!
 * \arg const sll_char_t* nm
 * \arg const sll_char_t* t
 */
__SLL_EXTERNAL void sll_audit(const sll_char_t* nm,const sll_char_t* t,...);



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
 * \arg const sll_char_t* nm
 * \arg const sll_char_t* t
 * \arg sll_var_arg_list_t* va
 */
__SLL_EXTERNAL void sll_audit_list(const sll_char_t* nm,const sll_char_t* t,sll_var_arg_list_t* va);



/**
 * \flags func
 * \name sll_audit_register_callback
 * \group audit
 * \desc Docs!
 * \arg sll_audit_callback_t fn
 */
__SLL_EXTERNAL void sll_audit_register_callback(sll_audit_callback_t fn);



/**
 * \flags func
 * \name sll_audit_unregister_callback
 * \group audit
 * \desc Docs!
 * \arg sll_audit_callback_t fn
 * \ret sll_bool_t
 */
__SLL_EXTERNAL sll_bool_t sll_audit_unregister_callback(sll_audit_callback_t fn);



#endif
