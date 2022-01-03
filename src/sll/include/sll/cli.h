#ifndef __SLL_CLI_H__
#define __SLL_CLI_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name CLI
 * \group cli
 * \desc Docs!
 */



/**
 * \flags check_output func
 * \name sll_cli_main
 * \group cli
 * \desc Docs!
 * \arg sll_array_length_t argc
 * \arg const sll_char_t*const* argv
 * \ret sll_return_code_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_code_t sll_cli_main(sll_array_length_t argc,const sll_char_t*const* argv);



#endif
