#ifndef __SLL_LOG_H__
#define __SLL_LOG_H__ 1
#include <sll/common.h>
#include <sll/string.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Logging
 * \group log
 * \desc Docs!
 */



#define SLL_LOG(t,...) sll_log(SLL_CHAR(__FILE__),SLL_CHAR(__func__),SLL_CHAR(t),##__VA_ARGS__)



/**
 * \flags func var_arg
 * \name sll_log
 * \group log
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \arg const sll_char_t* fn
 * \arg const sll_char_t* t
 */
__SLL_EXTERNAL void sll_log(const sll_char_t* fp,const sll_char_t* fn,const sll_char_t* t,...);



/**
 * \flags func
 * \name sll_set_log_default
 * \group log
 * \desc Docs!
 * \arg sll_bool_t st
 */
__SLL_EXTERNAL void sll_set_log_default(sll_bool_t st);



/**
 * \flags func
 * \name sll_set_log_file
 * \group log
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \arg sll_bool_t st
 */
__SLL_EXTERNAL void sll_set_log_file(const sll_char_t* fp,sll_bool_t st);



/**
 * \flags func
 * \name sll_set_log_function
 * \group log
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \arg const sll_char_t* fn
 * \arg sll_bool_t st
 */
__SLL_EXTERNAL void sll_set_log_function(const sll_char_t* fp,const sll_char_t* fn,sll_bool_t st);



#endif
