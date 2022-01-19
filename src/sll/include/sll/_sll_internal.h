#ifndef __SLL__SLL_INTERNAL_H__
#define __SLL__SLL_INTERNAL_H__ 1
#ifdef _MSC_VER
#include <intrin.h>
#endif
#include <sll/api.h>
#include <sll/generated/api.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/version.h>
#include <stdint.h>



#ifdef _MSC_VER
#pragma intrinsic(_BitScanForward64)
#pragma intrinsic(__popcnt64)
#pragma intrinsic(_rotl)
#pragma intrinsic(_rotl64)
#pragma section("s_obj$a",read)
#pragma section("s_obj$b",read)
#pragma section("s_obj$z",read)
#pragma section("s_str$a",read)
#pragma section("s_str$b",read)
#pragma section("s_str$z",read)
#ifndef DEBUG_BUILD
#define SLL_UNREACHABLE() __assume(0)
#endif
#define __SLL_FORCE_INLINE __inline __forceinline
static __SLL_FORCE_INLINE unsigned int FIND_FIRST_SET_BIT(unsigned __int64 m){
	unsigned long o;
	_BitScanForward64(&o,m);
	return o;
}
static __SLL_FORCE_INLINE unsigned int FIND_LAST_SET_BIT(unsigned __int64 m){
	unsigned long o;
	_BitScanReverse64(&o,m);
	return o;
}
#define POPULATION_COUNT(m) __popcnt64((m))
#define SWAP_BYTES(m) _byteswap_ulong((m))
#define SWAP_BYTES64(m) _byteswap_uint64((m))
#define ROTATE_BITS(a,b) _rotl((a),(b))
#define ROTATE_BITS_RIGHT(a,b) _rotr((a),(b))
#define ROTATE_BITS64(a,b) _rotl64((a),(b))
#define ROTATE_BITS_RIGHT64(a,b) _rotr64((a),(b))
#define IGNORE_RESULT(x) ((void)(x))
#define _ASSUME_ALIGNED(p,n,x) \
	do{ \
		if ((((uint64_t)(p))-(x))&((1<<(n))-1)){ \
			__assume(0); \
		} \
	} while (0)
#define _CUSTOM_SECTION(nm) __declspec(allocate(#nm))
#define CUSTOM_SECTION(nm) _CUSTOM_SECTION(nm##$b)
#define STATIC_OBJECT_SETUP static const __declspec(allocate("s_obj$a")) static_object_t* __s_obj_start=0;static const __declspec(allocate("s_obj$z")) static_object_t* __s_obj_end=0
#define STATIC_STRING_SETUP static const __declspec(allocate("s_str$a")) static_string_t* __s_str_start=0;static const __declspec(allocate("s_str$z")) static_string_t* __s_str_end=0
#else
#ifndef DEBUG_BUILD
#define SLL_UNREACHABLE() __builtin_unreachable()
#endif
#define __SLL_FORCE_INLINE inline __attribute__((always_inline))
#define FIND_FIRST_SET_BIT(m) (__builtin_ffsll((m))-1)
#define FIND_LAST_SET_BIT(m) (63-__builtin_clzll((m)))
#define POPULATION_COUNT(m) __builtin_popcountll((m))
#define SWAP_BYTES(m) __builtin_bswap32((m))
#define SWAP_BYTES64(m) __builtin_bswap64((m))
static __SLL_FORCE_INLINE unsigned int ROTATE_BITS(unsigned int a,unsigned char b){
	__asm__("rol %1,%0":"+r"(a):"c"(b));
	return a;
}
static __SLL_FORCE_INLINE unsigned int ROTATE_BITS_RIGHT(unsigned int a,unsigned char b){
	__asm__("ror %1,%0":"+r"(a):"c"(b));
	return a;
}
static __SLL_FORCE_INLINE unsigned long long int ROTATE_BITS64(unsigned long long int a,unsigned char b){
	__asm__("rolq %1,%0":"+r"(a):"c"(b));
	return a;
}
static __SLL_FORCE_INLINE unsigned long long int ROTATE_BITS_RIGHT64(unsigned long long int a,unsigned char b){
	__asm__("rorq %1,%0":"+r"(a):"c"(b));
	return a;
}
#define IGNORE_RESULT(x) \
	do{ \
		unsigned long long int __tmp __attribute__((unused))=(unsigned long long int)(x); \
	} while (0)
#define _ASSUME_ALIGNED(p,n,x) \
	do{ \
		p=__builtin_assume_aligned((p),(n),(x)); \
	} while (0)
#define CUSTOM_SECTION(nm) __attribute__((used,section(#nm)))
#define STATIC_OBJECT_SETUP extern const static_object_t* __start_s_obj;extern const static_object_t* __stop_s_obj
#define STATIC_STRING_SETUP extern const static_string_t* __start_s_str;extern const static_string_t* __stop_s_str
#define __s_obj_start __start_s_obj
#define __s_obj_end __stop_s_obj
#define __s_str_start __start_s_str
#define __s_str_end __stop_s_str
#endif
#ifdef DEBUG_BUILD
#define SLL_UNREACHABLE() _force_exit(SLL_CHAR("File \""__FILE__"\", Line "_STRINGIFY(__LINE__)" ("),SLL_CHAR(__func__),SLL_CHAR("): Unreachable Code\n"));
#endif



#define __API_FUNC(nm) __SLL_EXTERNAL __SLL_API_TYPE_sll_api_##nm sll_api_##nm(__SLL_API_ARGS_sll_api_##nm)
#define __STATIC_STRING(nm,dt) sll_string_t nm=SLL_INIT_STRING_STRUCT;static const static_string_t _UNIQUE_NAME(__s_str)={&(nm),{.s=SLL_CHAR(dt)},sizeof(dt)/sizeof(char)-1};static const CUSTOM_SECTION(s_str) static_string_t* _UNIQUE_NAME(__s_str_ptr)=&_UNIQUE_NAME(__s_str)
#define __STATIC_STRING_CODE(nm,c) sll_string_t nm=SLL_INIT_STRING_STRUCT;static void _UNIQUE_NAME(__s_str_fn)(sll_string_t* out){c};static const static_string_t _UNIQUE_NAME(__s_str)={&(nm),{.fn=_UNIQUE_NAME(__s_str_fn)},SLL_MAX_STRING_LENGTH};static const CUSTOM_SECTION(s_str) static_string_t* _UNIQUE_NAME(__s_str_ptr)=&_UNIQUE_NAME(__s_str)
#define __SLL_STATIC_INT_OBJECT(v) static sll_object_t _int_##v##_static_data={1,SLL_OBJECT_TYPE_INT,NULL,.dt={.i=(v)}};_DECL_GC_OBJECT(&_int_##v##_static_data)
#define __SLL_STATIC_NEG_INT_OBJECT(v) static sll_object_t _int_neg_##v##_static_data={1,SLL_OBJECT_TYPE_INT,NULL,.dt={.i=-(v)}};_DECL_GC_OBJECT(&_int_neg_##v##_static_data)
#define __SLL_STATIC_CHAR_OBJECT(v) static sll_object_t _char_##v##_static_data={1,SLL_OBJECT_TYPE_CHAR,NULL,.dt={.c=(sll_char_t)(v)}};_DECL_GC_OBJECT(&_char_##v##_static_data)
#define __SLL_STATIC_OBJECT(nm,t,f,v) static sll_object_t _##nm##_static_data={1,t,NULL,.dt={.f=v}};__SLL_EXTERNAL sll_object_t* sll_static_##nm=&_##nm##_static_data



#define _DECL_GC_OBJECT(rt) static const static_object_t _UNIQUE_NAME(__s_obj)={(rt),__FILE__,__LINE__};static const CUSTOM_SECTION(s_obj) static_object_t* _UNIQUE_NAME(__s_obj_ptr)=&_UNIQUE_NAME(__s_obj)
#define _UNIQUE_NAME_JOIN2(a,b) a##_##b
#define _UNIQUE_NAME_JOIN(a,b) _UNIQUE_NAME_JOIN2(a,b)
#define _UNIQUE_NAME(a) _UNIQUE_NAME_JOIN(a,__LINE__)
#define _STRINGIFY_(x) #x
#define _STRINGIFY(x) _STRINGIFY_(x)



#define SLL_UNIMPLEMENTED() _force_exit(SLL_CHAR("File \""__FILE__"\", Line "_STRINGIFY(__LINE__)" ("),SLL_CHAR(__func__),SLL_CHAR("): Unimplemented\n"));
#ifdef DEBUG_BUILD
#define ASSUME_ALIGNED(p,n,x) SLL_ASSERT(!((((uint64_t)(p))-(x))&((1<<(n))-1)))
#define SLL_ASSERT(x) \
	do{ \
		if (!(x)){ \
			_force_exit(SLL_CHAR("File \""__FILE__"\", Line "_STRINGIFY(__LINE__)" ("),SLL_CHAR(__func__),SLL_CHAR("): "_STRINGIFY(x)": Assertion Failed\n")); \
		} \
	} while (0)
#define CHECK_INTERNAL_FUNCTION_NAME(s) \
	do{ \
		SLL_ASSERT((s)->l<256); \
		SLL_ASSERT(sll_string_count_char((s),':')==1); \
	} while (0)
#else
#define ASSUME_ALIGNED(p,n,x) _ASSUME_ALIGNED(p,(n),(x))
#define SLL_ASSERT(x) \
	do{ \
		if (!(x)){ \
			SLL_UNREACHABLE(); \
		} \
	} while (0)
#define CHECK_INTERNAL_FUNCTION_NAME(s)
#endif

#define CONSTRUCT_DWORD(a,b,c,d) ((((uint32_t)(d))<<24)|(((uint32_t)(c))<<16)|(((uint32_t)(b))<<8)|(a))

#define ASSEMBLY_FILE_MAGIC_NUMBER CONSTRUCT_DWORD('S','L','A',0)
#define COMPLIED_OBJECT_FILE_MAGIC_NUMBER CONSTRUCT_DWORD('S','L','C',0)

#define EXTRA_COMPILATION_DATA_IMPORT 1
#define EXTRA_COMPILATION_DATA_EXPORT 2
#define EXTRA_COMPILATION_DATA_VARIABLE_DEFINITION 4

#define VARIABLE_OFFSET_NEVER_DELETE ((void*)0xffffffffffffffff)

#define ERROR_DISPLAY_TAB_WIDTH 4

#define ASSEMBLY_INSTRUCTION_TYPE_DBG SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED0
#define ASSEMBLY_INSTRUCTION_TYPE_DBG_FUNC SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED1
#define ASSEMBLY_INSTRUCTION_TYPE_FUNC_START SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED2
#define ASSEMBLY_INSTRUCTION_TYPE_LABEL_TARGET SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED3
#define ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED4
#define ASSEMBLY_INSTRUCTION_TYPE_NOP SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED5
#define ASSEMBLY_INSTRUCTION_FLAG_LABEL 128
#define ASSEMBLY_INSTRUCTION_MISC_FIELD(ai) ((ai)->dt.j)
#define ASSEMBLY_INSTRUCTION_GET_LABEL_DATA(lbl) ((lbl)&0x7fffffff)
#define ASSEMBLY_INSTRUCTION_FLAG_LABEL_REF 0x80000000

#define OPTIMIZER_FLAG_ARGUMENT 1
#define OPTIMIZER_FLAG_ASSIGN 2

#define COND_TYPE_UNKNOWN 0
#define COND_TYPE_ALWAYS_TRUE 1
#define COND_TYPE_ALWAYS_FALSE 2

#define OBJECT_TYPE_FUNCTION_ID SLL_OBJECT_TYPE_RESERVED0
#define OBJECT_TYPE_UNKNOWN SLL_OBJECT_TYPE_RESERVED1
#define OBJECT_CHANGE_IN_LOOP SLL_OBJECT_FLAG_RESERVED0
#define OBJECT_EXTERNAL_STRING SLL_OBJECT_FLAG_RESERVED1

#define GC_INIT_PAGE_COUNT 4
#define GC_GET_NEXT_OBJECT(o) ((sll_object_t*)((o)->dt.s.v))
#define GC_SET_NEXT_OBJECT(o,n) ((o)->dt.s.v=SLL_CHAR((n)))

#define MAX_CLEANUP_TABLE_SIZE 512

#define MAX_ASSEMBLY_INSTRUCTION_LABEL SLL_MAX_INSTRUCTION_INDEX

#define NODE_TYPE_UNKNOWN SLL_NODE_TYPE_RESERVED0
#define NODE_TYPE_CHANGE_STACK SLL_NODE_TYPE_RESERVED1

#define ALLOCATOR_ALLOC_SIZE 65536
#define ALLOCATOR_STACK_ALLOC_SIZE 262144
#define ASSEMBLY_INSTRUCTION_STACK_ALLOC_SIZE 65536
#define GC_OBJECT_POOL_ALLOC_SIZE 65536
#define NODE_STACK_ALLOC_SIZE 65536

#define STRING_COMPRESSION_MIN_LENGTH 64
#define STRING_COMPRESSION_OFFSET_BIT_COUNT 9
#define STRING_COMPRESSION_LENGTH_BIT_COUNT 4

#define OPTIMIZER_ASSIGN_VARIABLE_REMOVE ((void*)0xffffffffffffffff)

#define OPTIMIZER_NEW_OBJECT 1
#define OPTIMIZER_NO_VARIABLES 2

#define ALLOCATOR_MAX_SMALL_SIZE 512

#define FILE_FLAG_MEMORY SLL_FILE_FLAG_RESERVED0
#define FILE_FLAG_NO_RELEASE SLL_FILE_FLAG_RESERVED1
#define FILE_BUFFER_SIZE 2097152

#define STRING_FORMAT_FLAG_PAD_ZERO 1
#define STRING_FORMAT_FLAG_SIGN 2
#define STRING_FORMAT_FLAG_SPACE_SIGN 4
#define STRING_FORMAT_FLAG_JUSTIFY_LEFT 8
#define STRING_FORMAT_FLAG_ALTERNATIVE_FORM 16
#define STRING_FORMAT_FLAG_PERCISION 32
#define STRING_FORMAT_FLAG_HH_BITS 64
#define STRING_FORMAT_FLAG_H_BITS 128
#define STRING_FORMAT_FLAG_L_BITS 256
#define STRING_FORMAT_FLAG_LL_BITS 512
#define STRING_FORMAT_FLAG_UPPERCASE 1024

#define NOT_FIELD(o) SLL_ASSERT((o)->t!=SLL_NODE_TYPE_FIELD)

#define GET_VAR_INDEX_FLAG_UNKNOWN 1
#define GET_VAR_INDEX_FLAG_ASSIGN 2
#define GET_VAR_INDEX_FLAG_FUNC 4

#define CLI_VM_STACK_SIZE 65536
#define CLI_VM_CALL_STACK_SIZE 256

#define CLI_FLAG_ASSEMBLY_GENERATED 1
#define CLI_FLAG_EXPAND_PATH 2
#define CLI_FLAG_GENERATE_ASSEMBLY 4
#define CLI_FLAG_GENERATE_COMPILED_OBJECT 8
#define CLI_FLAG_GENERATE_SLL 16
#define CLI_FLAG_HELP 32
#define CLI_FLAG_NO_CONSOLE 64
#define CLI_FLAG_NO_RUN 128
#define CLI_FLAG_OPTIMIZE 256
#define CLI_FLAG_PRINT_ASSEMBLY 512
#define CLI_FLAG_PRINT_OBJECT 1024
#define CLI_FLAG_SINGLE_OUTPUT 2048
#define CLI_FLAG_STRIP_DEBUG 4096
#define CLI_FLAG_VERBOSE 8192
#define CLI_FLAG_VERSION 16384

#ifdef SLL_VERSION_HAS_SHA
#define CLI_BUILD_TYPE_STRING "commit/"SLL_VERSION_SHA" [https://github.com/sl-lang/sll/tree/"SLL_VERSION_TAG"]"
#else
#define CLI_BUILD_TYPE_STRING "local"
#endif



typedef sll_instruction_index_t assembly_instruction_label_t;



typedef struct __SCOPE_DATA{
	uint64_t* m;
	sll_scope_t l_sc;
	sll_scope_t ml;
} scope_data_t;



typedef struct __NEW_VARIABLE_DATA{
	sll_node_t** dt;
	uint32_t sz;
} new_variable_data_t;



typedef struct __EXTRA_COMPILATION_DATA{
	scope_data_t sc;
	sll_internal_function_table_t* i_ft;
	sll_import_resolver_t ir;
	new_variable_data_t* nv_dt;
	sll_string_index_t a_nm;
} extra_compilation_data_t;



typedef struct __IDENTIFIER_PAIR{
	sll_identifier_index_t a;
	sll_identifier_index_t b;
} identifier_pair_t;



typedef struct __IMPORT_MODULE_DATA{
	sll_identifier_list_length_t off[SLL_MAX_SHORT_IDENTIFIER_LENGTH+1];
	sll_string_index_t* sm;
	sll_function_index_t f_off;
	identifier_pair_t* eim;
	sll_export_table_length_t eiml;
	sll_scope_t sc_off;
	sll_compilation_data_t* c_dt;
	sll_string_index_t fp_off;
} import_module_data_t;



typedef struct __IDENTIFIER_DATA{
	sll_variable_index_t v;
	uint8_t rm;
} identifier_data_t;



typedef struct __IDENTIFIER_MAP_DATA{
	identifier_data_t* s_im[SLL_MAX_SHORT_IDENTIFIER_LENGTH];
	identifier_data_t* l_im;
	sll_variable_index_t n_vi;
	sll_scope_t l_sc;
	sll_variable_index_t* sc_vi;
	sll_variable_index_t vc;
} identifier_map_data_t;



typedef struct __IDENTIFIER_REMAP_DATA{
	sll_identifier_index_t* s[SLL_MAX_SHORT_IDENTIFIER_LENGTH];
	sll_identifier_index_t* l;
} identifier_remap_data_t;



typedef struct __VARIABLE_ASSIGNMENT_DATA{
	uint64_t* s_sm[SLL_MAX_SHORT_IDENTIFIER_LENGTH];
	uint64_t* l_sm;
} variable_assignment_data_t;



typedef struct __OPTIMIZER_DATA{
	sll_compilation_data_t* c_dt;
	sll_internal_function_table_t* i_ft;
	identifier_map_data_t it;
	identifier_remap_data_t im;
	uint32_t s_sm_l[SLL_MAX_SHORT_IDENTIFIER_LENGTH];
	uint32_t l_sm_l;
	variable_assignment_data_t va;
	sll_object_t** v;
	sll_node_t* a_v;
	uint8_t rm;
} optimizer_data_t;



typedef struct __IDENTIFIER_REMOVE_DATA{
	void** s[SLL_MAX_SHORT_IDENTIFIER_LENGTH];
	void** l;
} identifier_remove_data_t;



typedef struct __LOOP{
	assembly_instruction_label_t s;
	assembly_instruction_label_t e;
} loop_t;



typedef struct __RETURN_TABLE{
	assembly_instruction_label_t* dt;
	uint32_t sz;
} return_table_t;



typedef struct __ASSEMBLY_GENERATOR_DATA{
	sll_assembly_data_t* a_dt;
	const sll_compilation_data_t* c_dt;
	identifier_map_data_t it;
	assembly_instruction_label_t n_lbl;
	identifier_remove_data_t rm;
	loop_t l_dt;
	return_table_t* rt;
} assembly_generator_data_t;



typedef struct __STRING_MAP_DATA{
	sll_string_index_t ml;
	uint64_t* m;
	sll_string_index_t* im;
} strint_map_data_t;



typedef struct __INTERNAL_FUNCTION{
	const sll_char_t nm[256];
	sll_internal_function_pointer_t f;
	sll_internal_function_type_t t;
} internal_function_t;



typedef struct __OBJECT_DEBUG_DATA_TRACE_DATA{
	sll_char_t fp[256];
	sll_char_t fn[256];
	unsigned int ln;
} object_debug_data_trace_data_t;



typedef struct __OBJECT_DEBUG_DATA{
	object_debug_data_trace_data_t c;
	object_debug_data_trace_data_t** al;
	uint32_t all;
	object_debug_data_trace_data_t** rl;
	uint32_t rll;
} object_debug_data_t;



typedef struct __FILE_LIST_DATA{
	sll_string_t* dt;
	sll_array_length_t l;
} file_list_data_t;



typedef struct __STATIC_OBJECT{
	sll_object_t* dt;
	const char* fp;
	unsigned int ln;
} static_object_t;



typedef struct __OBJECT_LABEL{
	const sll_node_t* o;
	assembly_instruction_label_t l;
} object_label_t;



typedef struct __PAGE_HEADER{
	struct __PAGE_HEADER* n;
} page_header_t;



typedef struct __USER_MEM_BLOCK{
	uint64_t dt;
} user_mem_block_t;



typedef struct __MEM_BLOCK{
	struct __MEM_BLOCK* n;
} mem_block_t;



typedef struct __MEM_STACK_BLOCK{
	uint64_t dt;
} mem_stack_block_t;



typedef union __EXTENDED_FILE_DATA{
	sll_file_t f;
	sll_file_t* p;
} extended_file_data_t;



typedef struct __EXTENDED_FILE{
	extended_file_data_t dt;
	uint32_t rc;
	sll_bool_t p;
} extended_file_t;



typedef struct __CODE_GENERATION_DATA{
	const sll_compilation_data_t* c_dt;
	const sll_internal_function_table_t* i_ft;
	sll_bool_t p;
} code_generation_data_t;



typedef struct __FUNCTION_LOG_DATA{
	const sll_string_t nm;
	uint8_t fl;
} function_log_data_t;



typedef struct __FILE_LOG_DATA{
	const sll_string_t nm;
	function_log_data_t** dt;
	sll_array_length_t dtl;
	uint8_t fl;
} file_log_data_t;



typedef struct __LIBRARY{
	const sll_string_t nm;
	sll_library_handle_t h;
} library_t;



typedef struct __FILE_LINE_DATA{
	sll_file_offset_t* dt;
	sll_string_index_t c;
} file_line_data_t;



typedef struct __FILE_LINE{
	sll_file_offset_t ln;
	sll_string_index_t fp;
} file_line_t;



typedef union __FLOAT_DATA{
	sll_float_t v;
	uint64_t dt;
} float_data_t;



typedef struct __ASSEMBLY_LOOP_GENERATOR_DATA{
	loop_t p_l_dt;
	uint64_t* v_st;
} assembly_loop_generator_data_t;



typedef union __STATIC_STRING_SOURCE{
	const sll_char_t* s;
	void (*fn)(sll_string_t*);
} static_string_source_t;



typedef struct ____STATIC_STRING{
	sll_string_t* p;
	static_string_source_t dt;
	sll_string_length_t dtl;
} static_string_t;



#ifdef _MSC_VER
extern void* _win_dll_handle;
#endif



sll_assembly_instruction_t* _acquire_next_instruction(sll_assembly_data_t* a_dt);



sll_node_t* _acquire_next_node(sll_compilation_data_t* c_dt);



void _file_init_std_streams(void);



void _file_release_std_streams(void);



void _init_platform(void);



__SLL_NO_RETURN void _force_exit(const sll_char_t* a,const sll_char_t* b,const sll_char_t* c);



__SLL_NO_RETURN void _force_exit_platform(void);



void _gc_release_data(void);



sll_assembly_instruction_t* _get_instruction_at_offset(const sll_assembly_data_t* a_dt,sll_instruction_index_t off);



sll_node_t* _get_node_at_offset(const sll_compilation_data_t* c_dt,sll_node_offset_t off);



void _init_assembly_stack(sll_assembly_data_t* a_dt);



void _init_node_stack(sll_compilation_data_t* c_dt);



void _log_release_data(void);



void _memory_release_data(void);



void _push_call_stack(const sll_char_t* nm,sll_stack_offset_t si);



void _pop_call_stack(void);



void _reset_sandbox(void);



void _shift_nodes(sll_node_t* o,sll_compilation_data_t* c_dt,sll_node_offset_t off);



#endif
