#ifndef __SLL_CLI_H__
#define __SLL_CLI_H__ 1
#include <sll/_size_types.h>
#include <sll/api/path.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/node.h>
#include <sll/platform/process.h>
#include <sll/types.h>
/**
 * \flags group
 * \name CLI
 * \group cli
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Flags
 * \group cli
 * \subgroup cli-flag
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_CLI_FLAG_EXPAND_PATH
 * \group cli
 * \subgroup cli-flag
 * \desc Docs!
 * \type __SLL_U32
 */
#define SLL_CLI_FLAG_EXPAND_PATH 1



/**
 * \flags macro var
 * \name SLL_CLI_FLAG_GENERATE_ASSEMBLY
 * \group cli
 * \subgroup cli-flag
 * \desc Docs!
 * \type __SLL_U32
 */
#define SLL_CLI_FLAG_GENERATE_ASSEMBLY 2



/**
 * \flags macro var
 * \name SLL_CLI_FLAG_GENERATE_BUNDLE
 * \group cli
 * \subgroup cli-flag
 * \desc Docs!
 * \type __SLL_U32
 */
#define SLL_CLI_FLAG_GENERATE_BUNDLE 4



/**
 * \flags macro var
 * \name SLL_CLI_FLAG_GENERATE_COMPILED_OBJECT
 * \group cli
 * \subgroup cli-flag
 * \desc Docs!
 * \type __SLL_U32
 */
#define SLL_CLI_FLAG_GENERATE_COMPILED_OBJECT 8



/**
 * \flags macro var
 * \name SLL_CLI_FLAG_HELP
 * \group cli
 * \subgroup cli-flag
 * \desc Docs!
 * \type __SLL_U32
 */
#define SLL_CLI_FLAG_HELP 16



/**
 * \flags macro var
 * \name SLL_CLI_FLAG_NO_PATHS
 * \group cli
 * \subgroup cli-flag
 * \desc Docs!
 * \type __SLL_U32
 */
#define SLL_CLI_FLAG_NO_PATHS 32



/**
 * \flags macro var
 * \name SLL_CLI_FLAG_NO_RUN
 * \group cli
 * \subgroup cli-flag
 * \desc Docs!
 * \type __SLL_U32
 */
#define SLL_CLI_FLAG_NO_RUN 64



/**
 * \flags macro var
 * \name SLL_CLI_FLAG_PRINT_ASSEMBLY
 * \group cli
 * \subgroup cli-flag
 * \desc Docs!
 * \type __SLL_U32
 */
#define SLL_CLI_FLAG_PRINT_ASSEMBLY 128



/**
 * \flags macro var
 * \name SLL_CLI_FLAG_PRINT_NODES
 * \group cli
 * \subgroup cli-flag
 * \desc Docs!
 * \type __SLL_U32
 */
#define SLL_CLI_FLAG_PRINT_NODES 256



/**
 * \flags macro var
 * \name SLL_CLI_FLAG_RELEASE_MODE
 * \group cli
 * \subgroup cli-flag
 * \desc Docs!
 * \type __SLL_U32
 */
#define SLL_CLI_FLAG_RELEASE_MODE 512



/**
 * \flags macro var
 * \name SLL_CLI_FLAG_STRIP_DEBUG
 * \group cli
 * \subgroup cli-flag
 * \desc Docs!
 * \type __SLL_U32
 */
#define SLL_CLI_FLAG_STRIP_DEBUG 1024



/**
 * \flags macro var
 * \name SLL_CLI_FLAG_STRIP_NAMES
 * \group cli
 * \subgroup cli-flag
 * \desc Docs!
 * \type __SLL_U32
 */
#define SLL_CLI_FLAG_STRIP_NAMES 2048



/**
 * \flags macro var
 * \name SLL_CLI_FLAG_VERBOSE
 * \group cli
 * \subgroup cli-flag
 * \desc Docs!
 * \type __SLL_U32
 */
#define SLL_CLI_FLAG_VERBOSE 4096



/**
 * \flags macro var
 * \name SLL_CLI_FLAG_VERSION
 * \group cli
 * \subgroup cli-flag
 * \desc Docs!
 * \type __SLL_U32
 */
#define SLL_CLI_FLAG_VERSION 8192



/**
 * \flags macro var
 * \name SLL_CLI_FLAG_RESERVED0
 * \group cli
 * \subgroup cli-flag
 * \desc Internally used as `CLI_FLAG_SINGLE_OUTPUT`
 * \type __SLL_U32
 */
#define SLL_CLI_FLAG_RESERVED0 16384



/**
 * \flags macro var
 * \name SLL_LOOKUP_RESULT_COMPILED_OBJECT
 * \group cli
 * \desc Docs!
 * \type sll_cli_lookup_result_t
 */
#define SLL_LOOKUP_RESULT_COMPILED_OBJECT 0



/**
 * \flags macro var
 * \name SLL_LOOKUP_RESULT_ASSEMBLY
 * \group cli
 * \desc Docs!
 * \type sll_cli_lookup_result_t
 */
#define SLL_LOOKUP_RESULT_ASSEMBLY 1



/**
 * \flags macro var
 * \name SLL_LOOKUP_RESULT_EMPTY
 * \group cli
 * \desc Docs!
 * \type sll_cli_lookup_result_t
 */
#define SLL_LOOKUP_RESULT_EMPTY 2



/**
 * \flags type var
 * \name sll_cli_lookup_result_t
 * \group cli
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_cli_lookup_result_t;



/**
 * \flags type union
 * \name sll_cli_lookup_data_return_t
 * \group cli
 * \desc Docs!
 * \arg sll_assembly_data_t assembly_data
 * \arg sll_compilation_data_t compiled_object_data
 */
typedef union _SLL_CLI_LOOKUP_DATA_RETURN{
	sll_assembly_data_t assembly_data;
	sll_compilation_data_t compiled_object_data;
} sll_cli_lookup_data_return_t;



/**
 * \flags type
 * \name sll_cli_lookup_data_t
 * \group cli
 * \desc Docs!
 * \arg sll_char_t[SLL_API_MAX_FILE_PATH_LENGTH] path
 * \arg sll_cli_lookup_data_return_t data
 */
typedef struct _SLL_CLI_LOOKUP_DATA{
	sll_char_t path[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_cli_lookup_data_return_t data;
} sll_cli_lookup_data_t;



/**
 * \flags func type
 * \name sll_cli_path_resolver_t
 * \group cli
 * \desc Docs!
 * \arg const sll_string_t* path
 * \arg sll_cli_lookup_data_t* out
 * \ret sll_cli_lookup_result_t
 */
typedef sll_cli_lookup_result_t (*sll_cli_path_resolver_t)(const sll_string_t* path,sll_cli_lookup_data_t* out);



/**
 * \flags func
 * \name sll_cli_expand_path
 * \group cli
 * \desc Docs!
 * \arg const sll_char_t* path
 * \arg sll_char_t* out
 */
__SLL_EXTERNAL void sll_cli_expand_path(const sll_char_t* path,sll_char_t* out);



/**
 * \flags check_output func
 * \name sll_cli_lookup_file
 * \group cli
 * \desc Docs!
 * \arg const sll_string_t* path
 * \arg sll_bool_t use_custom_resolvers
 * \arg sll_cli_lookup_data_t* out
 * \ret sll_cli_lookup_result_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_cli_lookup_result_t sll_cli_lookup_file(const sll_string_t* path,sll_bool_t use_custom_resolvers,sll_cli_lookup_data_t* out);



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



/**
 * \flags func
 * \name sll_cli_register_path_resolver
 * \group cli
 * \desc Docs!
 * \arg sll_cli_path_resolver_t fn
 */
__SLL_EXTERNAL void sll_cli_register_path_resolver(sll_cli_path_resolver_t fn);



/**
 * \flags func
 * \name sll_cli_unregister_path_resolver
 * \group cli
 * \desc Docs!
 * \arg sll_cli_path_resolver_t fn
 * \ret sll_bool_t
 */
__SLL_EXTERNAL sll_bool_t sll_cli_unregister_path_resolver(sll_cli_path_resolver_t fn);



#endif
