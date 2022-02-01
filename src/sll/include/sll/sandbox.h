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
/**
 * \flags subgroup
 * \name Flags
 * \group sandbox
 * \subgroup sandbox-flag
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_SANDBOX_FLAG_DISABLE_FILE_IO
 * \group sandbox
 * \subgroup sandbox-flag
 * \desc Docs!
 * \type sll_sandbox_flags_t
 */
#define SLL_SANDBOX_FLAG_DISABLE_FILE_IO 0



/**
 * \flags macro var
 * \name SLL_SANDBOX_FLAG_ENABLE_STDIN_IO
 * \group sandbox
 * \subgroup sandbox-flag
 * \desc Docs!
 * \type sll_sandbox_flags_t
 */
#define SLL_SANDBOX_FLAG_ENABLE_STDIN_IO 1



/**
 * \flags macro var
 * \name SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO
 * \group sandbox
 * \subgroup sandbox-flag
 * \desc Docs!
 * \type sll_sandbox_flags_t
 */
#define SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO 2



/**
 * \flags macro var
 * \name SLL_SANDBOX_FLAG_DISABLE_PATH_API
 * \group sandbox
 * \subgroup sandbox-flag
 * \desc Docs!
 * \type sll_sandbox_flags_t
 */
#define SLL_SANDBOX_FLAG_DISABLE_PATH_API 3



/**
 * \flags macro var
 * \name SLL_SANDBOX_FLAG_DISABLE_PROCESS_API
 * \group sandbox
 * \subgroup sandbox-flag
 * \desc Docs!
 * \type sll_sandbox_flags_t
 */
#define SLL_SANDBOX_FLAG_DISABLE_PROCESS_API 4



/**
 * \flags macro var
 * \name SLL_SANDBOX_FLAG_DISABLE_ENVIRONMENT
 * \group sandbox
 * \subgroup sandbox-flag
 * \desc Docs!
 * \type sll_sandbox_flags_t
 */
#define SLL_SANDBOX_FLAG_DISABLE_ENVIRONMENT 5



/**
 * \flags macro var
 * \name SLL_SANDBOX_FLAG_DISABLE_LOAD_LIBRARY
 * \group sandbox
 * \subgroup sandbox-flag
 * \desc Docs!
 * \type sll_sandbox_flags_t
 */
#define SLL_SANDBOX_FLAG_DISABLE_LOAD_LIBRARY 6



/**
 * \flags macro var
 * \name SLL_SANDBOX_FLAG_ENABLE_BUFFER_FILES
 * \group sandbox
 * \subgroup sandbox-flag
 * \desc Docs!
 * \type sll_sandbox_flags_t
 */
#define SLL_SANDBOX_FLAG_ENABLE_BUFFER_FILES 7



/**
 * \flags macro var
 * \name SLL_SANDBOX_FLAG_ENABLE_FILE_RENAME
 * \group sandbox
 * \subgroup sandbox-flag
 * \desc Docs!
 * \type sll_sandbox_flags_t
 */
#define SLL_SANDBOX_FLAG_ENABLE_FILE_RENAME 8



/**
 * \flags macro var
 * \name SLL_SANDBOX_FLAG_ENABLE_FILE_COPY
 * \group sandbox
 * \subgroup sandbox-flag
 * \desc Docs!
 * \type sll_sandbox_flags_t
 */
#define SLL_SANDBOX_FLAG_ENABLE_FILE_COPY 9



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
 * \arg sll_array_t* o
 */
__SLL_EXTERNAL void sll_get_sandbox_flags(sll_array_t* o);



/**
 * \flags func
 * \name sll_set_sandbox_flag
 * \group sandbox
 * \desc Docs!
 * \arg sll_sandbox_flags_t f
 * \ret void
 */
__SLL_EXTERNAL void sll_set_sandbox_flag(sll_sandbox_flags_t f);



#endif
