#ifndef __SLL_LOG_H__
#define __SLL_LOG_H__ 1
#include <sll/common.h>
#include <sll/file.h>
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
 * \type sll_logger_flags_t
 */
#define SLL_LOG_FLAG_SHOW 1



/**
 * \flags macro var
 * \name SLL_LOG_FLAG_NO_HEADER
 * \group log
 * \desc Docs!
 * \type sll_logger_flags_t
 */
#define SLL_LOG_FLAG_NO_HEADER 2



/**
 * \flags type var
 * \name sll_logger_flags_t
 * \group log
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_logger_flags_t;



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
 * \ret sll_bool_t
 */
__SLL_EXTERNAL sll_bool_t sll_log(const sll_char_t* fp,const sll_char_t* fn,sll_file_offset_t ln,sll_bool_t w,const sll_char_t* t,...);



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
 * \ret sll_bool_t
 */
__SLL_EXTERNAL sll_bool_t sll_log_raw(const sll_char_t* fp,const sll_char_t* fn,sll_file_offset_t ln,sll_bool_t w,const sll_string_t* s);



/**
 * \flags check_output func
 * \name sll_set_log_flags
 * \group log
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \arg const sll_char_t* fn
 * \arg sll_logger_flags_t fl
 * \arg sll_bool_t st
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_set_log_flags(const sll_char_t* fp,const sll_char_t* fn,sll_logger_flags_t fl,sll_bool_t st);



#endif
