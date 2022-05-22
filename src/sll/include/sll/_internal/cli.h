#ifndef __SLL__INTERNAL_CLI_H__
#define __SLL__INTERNAL_CLI_H__ 1
#include <sll/_size_types.h>
#include <sll/bundle.h>
#include <sll/log.h>
#include <sll/platform/library.h>
#include <sll/platform/path.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/version.h>



#define CLI_VM_STACK_SIZE 131072
#define CLI_VM_CALL_STACK_SIZE 256

#define CLI_FLAG_SINGLE_OUTPUT SLL_CLI_FLAG_RESERVED0

#define CLI_PATH_RESOLVER_TABLE_SIZE 512

#if SLL_VERSION_HAS_SHA
#define CLI_BUILD_TYPE_STRING "commit/"SLL_VERSION_SHA" [https://github.com/sl-lang/sll/tree/"SLL_VERSION_TAG"]"
#else
#define CLI_BUILD_TYPE_STRING "local"
#endif



typedef struct _CLI_INCLUDE_DIR{
	sll_string_t name;
	sll_string_t path;
} cli_include_dir_t;



typedef struct _CLI_BUNDLE_SOURCE{
	sll_char_t* name;
	sll_bundle_t bundle;
} cli_bundle_source_t;



typedef struct _CLI_AUDIT_LIBRARY{
	const sll_char_t* name;
	sll_library_handle_t handle;
} cli_audit_library_t;



#endif
