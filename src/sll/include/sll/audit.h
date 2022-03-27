#ifndef __SLL_AUDIT_H__
#define __SLL_AUDIT_H__ 1
#include <sll/common.h>
#include <sll/types.h>
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
 * \name sll_audit_list
 * \group audit
 * \desc Docs!
 * \arg const sll_char_t* nm
 * \arg const sll_char_t* t
 * \arg sll_var_arg_list_t* va
 */
__SLL_EXTERNAL void sll_audit_list(const sll_char_t* nm,const sll_char_t* t,sll_var_arg_list_t* va);



#endif
