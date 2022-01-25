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



/**
 * \flags func macro var_arg
 * \name SLL_LOG
 * \group log
 * \desc Docs!
 * \arg const sll_char_t* t
 */
#define SLL_LOG(t,...) sll_log(SLL_CHAR(__FILE__),SLL_CHAR(__func__),__LINE__,0,SLL_CHAR(t),##__VA_ARGS__)



/**
 * \flags func macro var_arg
 * \name SLL_WARN
 * \group log
 * \desc Docs!
 * \arg const sll_char_t* t
 */
#define SLL_WARN(t,...) sll_log(SLL_CHAR(__FILE__),SLL_CHAR(__func__),__LINE__,1,SLL_CHAR(t),##__VA_ARGS__)



/**
 * \flags macro var
 * \name SLL_LOG_FLAG_SHOW
 * \group log
 * \desc Docs!
 * \type sll_flags_t
 */
#define SLL_LOG_FLAG_SHOW 1



/**
 * \flags macro var
 * \name SLL_LOG_FLAG_NO_HEADER
 * \group log
 * \desc Docs!
 * \type sll_flags_t
 */
#define SLL_LOG_FLAG_NO_HEADER 2



/**
 * \flags func var_arg
 * \name sll_log
 * \group log
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \arg const sll_char_t* fn
 * \arg sll_file_offset_t ln
 * \arg sll_bool_t w
 * \arg const sll_char_t* t
 */
__SLL_EXTERNAL void sll_log(const sll_char_t* fp,const sll_char_t* fn,sll_file_offset_t ln,sll_bool_t w,const sll_char_t* t,...);



/**
 * \flags func
 * \name sll_log_raw
 * \group log
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \arg const sll_char_t* fn
 * \arg sll_file_offset_t ln
 * \arg sll_bool_t w
 * \arg const sll_string_t* s
 */
__SLL_EXTERNAL void sll_log_raw(const sll_char_t* fp,const sll_char_t* fn,sll_file_offset_t ln,sll_bool_t w,const sll_string_t* s);



/**
 * \flags func
 * \name sll_set_log_default
 * \group log
 * \desc Docs!
 * \arg sll_flags_t fl
 * \arg sll_bool_t st
 */
__SLL_EXTERNAL void sll_set_log_default(sll_flags_t fl,sll_bool_t st);



/**
 * \flags func
 * \name sll_set_log_file
 * \group log
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \arg sll_flags_t fl
 * \arg sll_bool_t st
 */
__SLL_EXTERNAL void sll_set_log_file(const sll_char_t* fp,sll_flags_t fl,sll_bool_t st);



/**
 * \flags func
 * \name sll_set_log_function
 * \group log
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \arg const sll_char_t* fn
 * \arg sll_flags_t fl
 * \arg sll_bool_t st
 */
__SLL_EXTERNAL void sll_set_log_function(const sll_char_t* fp,const sll_char_t* fn,sll_flags_t fl,sll_bool_t st);



#endif
