#ifndef __SLL_INTERNAL_CLI_H__
#define __SLL_INTERNAL_CLI_H__ 1
#include <sll/_size_types.h>
#include <sll/log.h>
#include <sll/platform/path.h>
#include <sll/string.h>
#include <sll/types.h>



#define CLI_VM_STACK_SIZE 131072
#define CLI_VM_CALL_STACK_SIZE 256

#define CLI_FLAG_ASSEMBLY_GENERATED 1
#define CLI_FLAG_EXPAND_PATH 2
#define CLI_FLAG_GENERATE_ASSEMBLY 4
#define CLI_FLAG_GENERATE_BUNDLE 8
#define CLI_FLAG_GENERATE_COMPILED_OBJECT 16
#define CLI_FLAG_HELP 32
#define CLI_FLAG_NO_PATHS 64
#define CLI_FLAG_NO_RUN 128
#define CLI_FLAG_PRINT_ASSEMBLY 256
#define CLI_FLAG_PRINT_NODES 512
#define CLI_FLAG_SINGLE_OUTPUT 1024
#define CLI_FLAG_STRIP_DEBUG 2048
#define CLI_FLAG_STRIP_NAMES 4096
#define CLI_FLAG_VERBOSE 8192
#define CLI_FLAG_VERSION 16384

#define CLI_LOG_IF_VERBOSE(t,...) \
	if (fl&CLI_FLAG_VERBOSE){ \
		SLL_LOG(t,##__VA_ARGS__); \
	}
#define CLI_EXPAND_PATH(s,d) \
	if (fl&CLI_FLAG_EXPAND_PATH){ \
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



#endif
