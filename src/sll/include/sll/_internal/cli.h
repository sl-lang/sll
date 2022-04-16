#ifndef __SLL__INTERNAL_CLI_H__
#define __SLL__INTERNAL_CLI_H__ 1
#include <sll/_size_types.h>
#include <sll/log.h>
#include <sll/platform/path.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/version.h>



#define CLI_VM_STACK_SIZE 131072
#define CLI_VM_CALL_STACK_SIZE 256

#define CLI_FLAG_ASSEMBLY_GENERATED SLL_CLI_FLAG_RESERVED0
#define CLI_FLAG_SINGLE_OUTPUT SLL_CLI_FLAG_RESERVED1

#define CLI_LOG_IF_VERBOSE(t,...) \
	if (fl&SLL_CLI_FLAG_VERBOSE){ \
		SLL_LOG(t,##__VA_ARGS__); \
	}
#define CLI_EXPAND_PATH(s,d) \
	if (fl&SLL_CLI_FLAG_EXPAND_PATH){ \
		sll_platform_absolute_path(s,d,SLL_API_MAX_FILE_PATH_LENGTH); \
	} \
	else{ \
		SLL_COPY_STRING_NULL(s,d); \
	}

#if SLL_VERSION_HAS_SHA
#define CLI_BUILD_TYPE_STRING "commit/"SLL_VERSION_SHA" [https://github.com/sl-lang/sll/tree/"SLL_VERSION_TAG"]"
#else
#define CLI_BUILD_TYPE_STRING "local"
#endif



typedef struct __CLI_BUNDLE_SOURCE{
	sll_char_t* nm;
	sll_bundle_t b;
} cli_bundle_source_t;



typedef struct __CLI_AUDIT_LIBRARY{
	const sll_char_t* nm;
	sll_library_handle_t lh;
} cli_audit_library_t;



#endif
