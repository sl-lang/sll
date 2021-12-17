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



#define SLL_LOG(t,...) sll_log(__FILE__,__func__,SLL_CHAR(t),##__VA_ARGS__)



/**
 * \flags func var_arg
 * \name sll_log
 * \group log
 * \desc Docs!
 * \arg const char* fp
 * \arg const char* fn
 * \arg const sll_char_t* t
 */
__SLL_EXTERNAL void sll_log(const char* fp,const char* fn,const sll_char_t* t,...);



/**
 * \flags func
 * \name sll_log_set_default
 * \group log
 * \desc Docs!
 * \arg sll_bool_t d
 */
__SLL_EXTERNAL void sll_log_set_default(sll_bool_t d);



#endif
