#ifndef __SLL_SANDBOX_H__
#define __SLL_SANDBOX_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Sandbox
 * \group sandbox
 * \desc Docs!
 */



#define SLL_SANDBOX_FLAG_DISABLE_FILE_IO 1
#define SLL_SANDBOX_FLAG_ENABLE_STDIN_IO 2
#define SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO 4
#define SLL_SANDBOX_FLAG_DISABLE_PATH_API 8
#define SLL_SANDBOX_FLAG_DISABLE_PROCESS_API 16



/**
 * \flags check_output func
 * \name sll_get_sandbox_flag
 * \group sandbox
 * \desc Docs!
 * \arg sll_sandbox_flags_t f
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_get_sandbox_flag(sll_sandbox_flags_t f);



/**
 * \flags check_output func
 * \name sll_get_sandbox_flags
 * \group sandbox
 * \desc Docs!
 * \ret sll_sandbox_flags_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_sandbox_flags_t sll_get_sandbox_flags(void);



/**
 * \flags func
 * \name sll_set_sandbox_flags
 * \group sandbox
 * \desc Docs!
 * \arg sll_sandbox_flags_t f
 * \ret sll_sandbox_flags_t
 */
__SLL_EXTERNAL sll_sandbox_flags_t sll_set_sandbox_flags(sll_sandbox_flags_t f);



#endif
