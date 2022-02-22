#ifndef __SLL__SLL_INTERNAL_H__
#define __SLL__SLL_INTERNAL_H__ 1
#ifdef __SLL_BUILD_WINDOWS
#include <intrin.h>
#endif
#include <sll/_size_types.h>
#include <sll/api.h>
#include <sll/generated/api.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/version.h>



#ifdef __SLL_BUILD_WINDOWS
#pragma intrinsic(_BitScanForward64)
#pragma intrinsic(__popcnt64)
#pragma intrinsic(_rotl)
#pragma intrinsic(_rotl64)
#pragma section(".sobject$a",read)
#pragma section(".sobject$b",read)
#pragma section(".sobject$z",read)
#pragma section(".initstr$a",read)
#pragma section(".initstr$b",read)
#pragma section(".initstr$z",read)
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
#define _ASSUME_ALIGNED(p,n,x) \
	do{ \
		if ((ADDR(p)-(x))&((1ull<<(n))-1)){ \
			__assume(0); \
		} \
	} while (0)
#define _CUSTOM_SECTION_(nm) __declspec(allocate(#nm))
#define _CUSTOM_SECTION(nm) _CUSTOM_SECTION_(.##nm##$b)
#define STATIC_OBJECT_SETUP static const __declspec(allocate(".sobject$a")) static_object_t* __static_object_start=0;static const __declspec(allocate(".sobject$z")) static_object_t* __static_object_end=0
#define INIT_STRING_SETUP static const __declspec(allocate(".initstr$a")) init_string_t* __init_string_start=0;static const __declspec(allocate(".initstr$z")) init_string_t* __init_string_end=0
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
#define _ASSUME_ALIGNED(p,n,x) \
	do{ \
		p=__builtin_assume_aligned((p),1<<(n),(x)); \
	} while (0)
#define _CUSTOM_SECTION(nm) _CUSTOM_SECTION_(nm)
#ifdef __SLL_BUILD_DARWIN
#define _CUSTOM_SECTION_(nm) __attribute__((used,__section__("__DATA,"#nm)))
#define STATIC_OBJECT_SETUP extern unsigned long long int __start_sobject __asm("section$start$__DATA$sobject");extern unsigned long long int __stop_sobject __asm("section$end$__DATA$sobject")
#define INIT_STRING_SETUP extern unsigned long long int __start_initstr __asm("section$start$__DATA$initstr");extern unsigned long long int __stop_initstr __asm("section$end$__DATA$initstr")
#else
#define _CUSTOM_SECTION_(nm) __attribute__((used,section(#nm)))
#define STATIC_OBJECT_SETUP extern const static_object_t* __start_sobject;extern const static_object_t* __stop_sobject
#define INIT_STRING_SETUP extern const init_string_t* __start_initstr;extern const init_string_t* __stop_initstr
#endif
#define __static_object_start __start_sobject
#define __static_object_end __stop_sobject
#define __init_string_start __start_initstr
#define __init_string_end __stop_initstr
#endif
#ifdef DEBUG_BUILD
#define SLL_UNREACHABLE() _force_exit(SLL_CHAR("File \""__FILE__"\", Line "_STRINGIFY(__LINE__)" ("),SLL_CHAR(__func__),SLL_CHAR("): Unreachable Code\n"));
#endif



#define __API_FUNC(nm) __SLL_EXTERNAL __SLL_API_TYPE_sll_api_##nm sll_api_##nm(__SLL_API_ARGS_sll_api_##nm)
#define __STATIC_STRING(nm,dt) sll_string_t nm=SLL_INIT_STRING_STRUCT;static const init_string_t _UNIQUE_NAME(__init_string)={&(nm),{.s=SLL_CHAR(dt)},sizeof(dt)/sizeof(char)-1};static const _CUSTOM_SECTION(initstr) init_string_t* _UNIQUE_NAME(__init_string_ptr)=&_UNIQUE_NAME(__init_string)
#define __STATIC_STRING_CODE(nm,c) sll_string_t nm=SLL_INIT_STRING_STRUCT;static void _UNIQUE_NAME(__init_string_fn)(sll_string_t* out){c};static const init_string_t _UNIQUE_NAME(__init_string)={&(nm),{.fn=_UNIQUE_NAME(__init_string_fn)},SLL_MAX_STRING_LENGTH};static const _CUSTOM_SECTION(initstr) init_string_t* _UNIQUE_NAME(__init_string_ptr)=&_UNIQUE_NAME(__init_string)
#define __SLL_STATIC_INT_OBJECT(v) static sll_object_t _int_##v##_static_data={1,SLL_OBJECT_TYPE_INT,NULL,.dt={.i=(v)}};_DECL_GC_OBJECT(&_int_##v##_static_data)
#define __SLL_STATIC_NEG_INT_OBJECT(v) static sll_object_t _int_neg_##v##_static_data={1,SLL_OBJECT_TYPE_INT,NULL,.dt={.i=-(v)}};_DECL_GC_OBJECT(&_int_neg_##v##_static_data)
#define __SLL_STATIC_CHAR_OBJECT(v) static sll_object_t _char_##v##_static_data={1,SLL_OBJECT_TYPE_CHAR,NULL,.dt={.c=(sll_char_t)(v)}};_DECL_GC_OBJECT(&_char_##v##_static_data)
#define __SLL_STATIC_OBJECT(nm,t,f,v) static sll_object_t _##nm##_static_data={1,t,NULL,.dt={.f=v}};__SLL_EXTERNAL sll_object_t* sll_static_##nm=&_##nm##_static_data



#define _DECL_GC_OBJECT(rt) static const static_object_t _UNIQUE_NAME(__static_object)={(rt),__FILE__,__LINE__};static const _CUSTOM_SECTION(sobject) static_object_t* _UNIQUE_NAME(__static_object_ptr)=&_UNIQUE_NAME(__static_object)
#define _UNIQUE_NAME_JOIN2(a,b) a##_##b
#define _UNIQUE_NAME_JOIN(a,b) _UNIQUE_NAME_JOIN2(a,b)
#define _UNIQUE_NAME(a) _UNIQUE_NAME_JOIN(a,__LINE__)
#define _STRINGIFY_(x) #x
#define _STRINGIFY(x) _STRINGIFY_(x)



#define SLL_UNIMPLEMENTED() _force_exit(SLL_CHAR("File \""__FILE__"\", Line "_STRINGIFY(__LINE__)" ("),SLL_CHAR(__func__),SLL_CHAR("): Unimplemented\n"));
#ifdef DEBUG_BUILD
#define ASSUME_ALIGNED(p,n,x) SLL_ASSERT(!((ADDR(p)-(x))&((1ull<<(n))-1)))
#define SLL_ASSERT(x) \
	do{ \
		if (!(x)){ \
			_force_exit(SLL_CHAR("File \""__FILE__"\", Line "_STRINGIFY(__LINE__)" ("),SLL_CHAR(__func__),SLL_CHAR("): "_STRINGIFY(x)": Assertion Failed\n")); \
		} \
	} while (0)
#define CHECK_INTERNAL_FUNCTION_NAME(s) SLL_ASSERT(sll_string_count_char((s),':')==1)
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

#define CONSTRUCT_DWORD(a,b,c,d) ((((magic_number_t)(d))<<24)|(((magic_number_t)(c))<<16)|(((magic_number_t)(b))<<8)|(a))

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
#define ASSEMBLY_INSTRUCTION_TYPE_NOP SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED4
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
#define OBJECT_CHANGE_IN_LOOP SLL_OBJECT_FLAG_RESERVED0
#define OBJECT_EXTERNAL_STRING SLL_OBJECT_FLAG_RESERVED1

#define GC_INIT_PAGE_COUNT 4
#define GC_GET_NEXT_OBJECT(o) ((sll_object_t*)((o)->dt.s.v))
#define GC_SET_NEXT_OBJECT(o,n) ((o)->dt.s.v=SLL_CHAR((n)))

#define MAX_CLEANUP_TABLE_SIZE 512

#define MAX_ASSEMBLY_INSTRUCTION_LABEL SLL_MAX_INSTRUCTION_INDEX

#define NODE_TYPE_UNKNOWN SLL_NODE_TYPE_RESERVED0

#define ALLOCATOR_ALLOC_SIZE 65536
#define ALLOCATOR_STACK_ALLOC_SIZE 262144
#define ASSEMBLY_INSTRUCTION_STACK_ALLOC_SIZE 65536
#define FILE_DYNAMIC_BUFFER_ALLOC_SIZE 65536
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
#define FILE_FLAG_DYNAMIC_BUFFERS SLL_FILE_FLAG_RESERVED2
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

#define CLI_VM_STACK_SIZE 4096
#define CLI_VM_CALL_STACK_SIZE 256

#define CLI_FLAG_ASSEMBLY_GENERATED 1
#define CLI_FLAG_EXPAND_PATH 2
#define CLI_FLAG_GENERATE_ASSEMBLY 4
#define CLI_FLAG_GENERATE_COMPILED_OBJECT 8
#define CLI_FLAG_GENERATE_SLL 16
#define CLI_FLAG_HELP 32
#define CLI_FLAG_NO_RUN 64
#define CLI_FLAG_PRINT_ASSEMBLY 128
#define CLI_FLAG_PRINT_NODES 256
#define CLI_FLAG_SINGLE_OUTPUT 512
#define CLI_FLAG_STRIP_DEBUG 1024
#define CLI_FLAG_VERBOSE 2048
#define CLI_FLAG_VERSION 4096

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

#define SERIAL_OBJECT_TYPE (SLL_MAX_OBJECT_TYPE+1)

#define PRINT_STATIC_STRING(s,wf) sll_file_write((wf),(s),sizeof(s)/sizeof(char)-1)

#define JSON_NUMBER_INT 0
#define JSON_NUMBER_FLOAT 1

#define DISABLE_FILE_HASH 255

#define THREAD_SCHEDULER_INSTRUCTION_COUNT 10
#define THREAD_IS_UNUSED(t) (ADDR(t)>>63)
#define THREAD_NEXT_UNUSED(id) (PTR((id)|0x8000000000000000ull))
#define THREAD_GET_NEXT_UNUSED(t) ((sll_thread_index_t)(ADDR((t))&0x7fffffffffffffffull))
#define THREAD_SIZE SLL_ROUND_PAGE(sizeof(thread_data_t)+sll_current_vm_config->c_st_sz*sizeof(sll_call_stack_frame_t)+sll_current_vm_config->s_sz*sizeof(sll_object_t*))

#define THREAD_ALLOCATOR_CACHE_POOL_SIZE 16

#define THREAD_LOCK_UNUSED 0xfffffffe
#define THREAD_LOCK_GET_NEXT_ID(l) ((sll_lock_index_t)ADDR((l)->last))
#define THREAD_LOCK_SET_NEXT_ID(l,v) ((l)->last=PTR((sll_lock_index_t)(v)))

#define THREAD_SEMAPHORE_UNUSED 0xffffffff
#define THREAD_SEMAPHORE_GET_NEXT_ID(s) ((sll_semaphore_index_t)ADDR((s)->last))
#define THREAD_SEMAPHORE_SET_NEXT_ID(s,v) ((s)->last=PTR((sll_semaphore_index_t)(v)))

#define THREAD_STATE_INITIALIZED 0
#define THREAD_STATE_RUNNING 1
#define THREAD_STATE_QUEUED 2
#define THREAD_STATE_WAIT_LOCK 3
#define THREAD_STATE_WAIT_SEMAPHORE 4
#define THREAD_STATE_WAIT_THREAD 5
#define THREAD_STATE_TERMINATED 6
#define THREAD_STATE_UNDEFINED 255

#define ADDR(x) ((addr_t)(x))
#define PTR(x) ((void*)(addr_t)(x))



typedef __SLL_U8 bucket_index_t;



typedef __SLL_U8 thread_state_t;



typedef sll_instruction_index_t assembly_instruction_label_t;



typedef __SLL_U32 lock_list_length_t;



typedef __SLL_U32 magic_number_t;



typedef __SLL_U32 queue_length_t;



typedef __SLL_U32 return_table_size_t;



typedef __SLL_U32 semaphore_list_length_t;



typedef __SLL_U32 small_bitmap_t;



typedef __SLL_U32 thread_list_length_t;



typedef __SLL_U32 wait_list_length_t;



typedef __SLL_U64 addr_t;



typedef __SLL_U64 bitmap_t;



typedef __SLL_U64 wide_data_t;



typedef struct __SCOPE_DATA{
	bitmap_t* m;
	sll_scope_t l_sc;
	sll_scope_t ml;
} scope_data_t;



typedef struct __NEW_VARIABLE_DATA{
	sll_node_t** dt;
	sll_arg_count_t sz;
} new_variable_data_t;



typedef struct __EXTRA_COMPILATION_DATA{
	scope_data_t sc;
	sll_compilation_data_t* c_dt;
	sll_internal_function_table_t* i_ft;
	sll_import_resolver_t ir;
	new_variable_data_t* nv_dt;
	const scope_data_t* not_fn_sc;
	sll_string_index_t a_nm;
	sll_bool_t fn;
} extra_compilation_data_t;



typedef struct __IDENTIFIER_DATA{
	sll_variable_index_t v;
	sll_bool_t rm;
} identifier_data_t;



typedef struct __IDENTIFIER_MAP_DATA{
	identifier_data_t* s_im[SLL_MAX_SHORT_IDENTIFIER_LENGTH];
	identifier_data_t* l_im;
	sll_scope_t l_sc;
	sll_variable_index_t* sc_vi;
	sll_variable_index_t vc;
} identifier_map_data_t;



typedef struct __IDENTIFIER_REMOVE_DATA{
	void** s[SLL_MAX_SHORT_IDENTIFIER_LENGTH];
	void** l;
} identifier_remove_data_t;



typedef struct __LOOP{
	assembly_instruction_label_t cnt;
	assembly_instruction_label_t brk;
} loop_t;



typedef struct __ASSEMBLY_GENERATOR_DATA{
	sll_assembly_data_t* a_dt;
	const sll_source_file_t* sf;
	identifier_map_data_t it;
	assembly_instruction_label_t n_lbl;
	identifier_remove_data_t rm;
	loop_t l_dt;
	assembly_instruction_label_t rt;
	sll_stack_offset_t s_off;
} assembly_generator_data_t;



typedef struct __STRING_MAP_DATA{
	sll_string_index_t ml;
	bitmap_t* m;
	sll_string_index_t* im;
} strint_map_data_t;



typedef struct __INTERNAL_FUNCTION{
	const sll_char_t* nm;
	const sll_internal_function_pointer_t f;
} internal_function_t;



typedef struct __OBJECT_DEBUG_DATA_TRACE_DATA{
	sll_char_t fp[256];
	sll_char_t fn[256];
	unsigned int ln;
} object_debug_data_trace_data_t;



typedef struct __OBJECT_DEBUG_DATA{
	object_debug_data_trace_data_t c;
	object_debug_data_trace_data_t** al;
	sll_array_length_t all;
	object_debug_data_trace_data_t** rl;
	sll_array_length_t rll;
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
	__SLL_U64 dt;
} user_mem_block_t;



typedef struct __MEM_BLOCK{
	struct __MEM_BLOCK* n;
} mem_block_t;



typedef struct __MEM_STACK_BLOCK{
	__SLL_U64 dt;
} mem_stack_block_t;



typedef union __EXTENDED_FILE_DATA{
	sll_file_t f;
	sll_file_t* p;
} extended_file_data_t;



typedef struct __EXTENDED_FILE{
	extended_file_data_t dt;
	sll_ref_count_t rc;
	sll_bool_t p;
} extended_file_t;



typedef struct __CODE_GENERATION_DATA{
	const sll_source_file_t* sf;
	const sll_internal_function_table_t* i_ft;
	sll_bool_t p;
} code_generation_data_t;



typedef struct __FUNCTION_LOG_DATA{
	const sll_string_t nm;
	sll_flags_t fl;
} function_log_data_t;



typedef struct __FILE_LOG_DATA{
	const sll_string_t nm;
	function_log_data_t** dt;
	sll_array_length_t dtl;
	sll_flags_t fl;
} file_log_data_t;



typedef struct __LIBRARY{
	const sll_string_t nm;
	sll_library_handle_t h;
} library_t;



typedef struct __FILE_LINE{
	sll_file_offset_t ln;
	sll_string_index_t fp;
} file_line_t;



typedef union __FLOAT_DATA{
	float v;
	__SLL_U32 dt;
} float_data_t;



typedef union __DOUBLE_DATA{
	double v;
	__SLL_U64 dt;
} double_data_t;



typedef struct __ASSEMBLY_LOOP_GENERATOR_DATA{
	loop_t p_l_dt;
	bitmap_t* v_st;
} assembly_loop_generator_data_t;



typedef union __STATIC_STRING_SOURCE{
	const sll_char_t* s;
	void (*fn)(sll_string_t*);
} static_string_source_t;



typedef struct __INIT_STRING{
	sll_string_t* p;
	static_string_source_t dt;
	sll_string_length_t dtl;
} init_string_t;



typedef struct __DYNAMIC_BUFFER_CHUNK{
	sll_size_t sz;
	struct __DYNAMIC_BUFFER_CHUNK* n;
	sll_char_t dt[];
} dynamic_buffer_chunk_t;



typedef union __JSON_NUMBER{
	sll_integer_t i;
	sll_float_t f;
} json_number_t;



typedef struct __SOURCE_FILE_MAPPING_DATA{
	sll_string_index_t* sm;
	sll_function_index_t f_id_off;
	sll_scope_t sc_off;
	sll_identifier_index_t* idx_off[SLL_MAX_SHORT_IDENTIFIER_LENGTH+1];
} source_file_mapping_data_t;



typedef struct __THREAD_DATA{
	sll_object_t** stack;
	sll_instruction_index_t ii;
	sll_stack_offset_t si;
	sll_thread_index_t nxt;
	sll_thread_index_t wait;
	sll_object_t* ret;
	sll_call_stack_t c_st;
	sll_char_t tm;
	thread_state_t st;
} thread_data_t;



typedef struct __LOCK{
	sll_thread_index_t lock;
	sll_thread_index_t first;
	thread_data_t* last;
} lock_t;



typedef struct __SEMAPHORE{
	sll_semaphore_counter_t count;
	sll_thread_index_t first;
	thread_data_t* last;
} semaphore_t;



#ifdef __SLL_BUILD_WINDOWS
extern void* _win_dll_handle;
#endif
extern thread_data_t* _scheduler_current_thread;
extern sll_thread_index_t _scheduler_thread_idx;



sll_assembly_instruction_t* _acquire_next_instruction(sll_assembly_data_t* a_dt);



sll_node_t* _acquire_next_node(sll_source_file_t* sf);



void _call_function(thread_data_t* thr,sll_function_index_t fn,sll_arg_count_t ac);



void _file_end_hash(sll_file_t* f);



void _file_init_std_streams(void);



void _file_release_std_streams(void);



void _file_start_hash(sll_file_t* f);



__SLL_NO_RETURN void _force_exit(const sll_char_t* a,const sll_char_t* b,const sll_char_t* c);



__SLL_NO_RETURN void _force_exit_platform(void);



void _gc_release_data(void);



sll_assembly_instruction_t* _get_instruction_at_offset(const sll_assembly_data_t* a_dt,sll_instruction_index_t off);



sll_node_t* _get_node_at_offset(const sll_source_file_t* sf,sll_node_offset_t off);



void _init_assembly_stack(sll_assembly_data_t* a_dt);



void _init_node_stack(sll_source_file_t* sf);



void _init_platform(void);



void _log_release_data(void);



void _memory_release_data(void);



void _print_char(sll_char_t c,sll_file_t* wf);



void _print_float(sll_float_t v,sll_file_t* wf);



void _print_int(sll_integer_t v,sll_file_t* wf);



void _reset_sandbox(void);



void _scheduler_deinit(void);



thread_data_t* _scheduler_get_thread(sll_thread_index_t t);



void _scheduler_init(void);



sll_thread_index_t _scheduler_new_thread(void);



void _scheduler_queue_next(void);



sll_thread_index_t _scheduler_queue_pop(void);



void _scheduler_queue_thread(sll_thread_index_t t);



void _scheduler_set_thread(sll_thread_index_t t);



void _scheduler_terminate_thread(sll_object_t* ret);



sll_bool_t _scheduler_wait_lock(sll_integer_t w);



sll_bool_t _scheduler_wait_semaphore(sll_integer_t w);



sll_bool_t _scheduler_wait_thread(sll_integer_t w);



#endif
