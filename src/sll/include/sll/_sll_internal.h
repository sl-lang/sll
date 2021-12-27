#ifndef __SLL__SLL_INTERNAL_H__
#define __SLL__SLL_INTERNAL_H__ 1
#ifdef _MSC_VER
#include <intrin.h>
#endif
#include <sll/api.h>
#include <sll/api/_generated.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdint.h>



#ifdef _MSC_VER
#pragma intrinsic(_BitScanForward64)
#pragma intrinsic(__popcnt64)
#pragma intrinsic(_rotl)
#pragma intrinsic(_rotl64)
#pragma section("ifunc$a",read)
#pragma section("ifunc$b",read)
#pragma section("ifunc$z",read)
#pragma section("strto$a",read)
#pragma section("strto$b",read)
#pragma section("strto$z",read)
#ifndef DEBUG_BUILD
#define SLL_UNREACHABLE() __assume(0)
#endif
static __inline __forceinline unsigned int FIND_FIRST_SET_BIT(unsigned __int64 m){
	unsigned long o;
	_BitScanForward64(&o,m);
	return o;
}
static __inline __forceinline unsigned int FIND_LAST_SET_BIT(unsigned __int64 m){
	unsigned long o;
	_BitScanReverse64(&o,m);
	return o;
}
#define POPULATION_COUNT(m) __popcnt64((m))
#define SWAP_BYTES(m) _byteswap_ulong((m))
#define ROTATE_BITS(a,b) _rotl((a),(b))
#define ROTATE_BITS64(a,b) _rotl64((a),(b))
#define IGNORE_RESULT(x) ((void)(x))
#define _ASSUME_ALIGNED(p,n,x) \
	do{ \
		if (((((uint64_t)(p))-(x))&((1<<(n))-1))!=0){ \
			__assume(0); \
		} \
	} while (0)
#define STATIC_OBJECT(rt) const static static_object_t _UNIQUE_NAME(__strto)={(rt),__FILE__,__LINE__};const static __declspec(allocate("strto$b")) static_object_t* _UNIQUE_NAME(__strto_ptr)=&_UNIQUE_NAME(__strto)
#define STATIC_OBJECT_SETUP const static __declspec(allocate("strto$a")) static_object_t* __strto_start=0;const static __declspec(allocate("strto$z")) static_object_t* __strto_end=0
#else
#ifndef DEBUG_BUILD
#define SLL_UNREACHABLE() __builtin_unreachable()
#endif
#define FIND_FIRST_SET_BIT(m) (__builtin_ffsll((m))-1)
#define FIND_LAST_SET_BIT(m) (63-__builtin_clzll((m)))
#define POPULATION_COUNT(m) __builtin_popcountll((m))
#define SWAP_BYTES(m) __builtin_bswap32((m))
static inline __attribute__((always_inline)) unsigned int ROTATE_BITS(unsigned int a,unsigned char b){
	__asm__("rol %1,%0":"+r"(a):"c"(b));
	return a;
}
static inline __attribute__((always_inline)) unsigned long long int ROTATE_BITS64(unsigned long long int a,unsigned char b){
	__asm__("rolq %1,%0":"+r"(a):"c"(b));
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
#define STATIC_OBJECT(rt) const static static_object_t _UNIQUE_NAME(__strto)={(rt),__FILE__,__LINE__};const static __attribute__((used,section("strto"))) static_object_t* _UNIQUE_NAME(__strto_ptr)=&_UNIQUE_NAME(__strto)
#define STATIC_OBJECT_SETUP extern const static_object_t* __start_strto;extern const static_object_t* __stop_strto
#define __strto_start __start_strto
#define __strto_end __stop_strto
#endif
#ifdef DEBUG_BUILD
#define SLL_UNREACHABLE() _force_exit(SLL_CHAR("File \""__FILE__"\", Line "_SLL_ASSERT_STRINGIFY(__LINE__)" ("),SLL_CHAR(__func__),SLL_CHAR("): Unreachable Code\n"),NULL,NULL);
#endif



#define __API_FUNC(nm) __SLL_EXTERNAL __SLL_API_TYPE_sll_api_##nm sll_api_##nm(__SLL_API_ARGS_sll_api_##nm)
#define __SLL_STATIC_INT_OBJECT(v) static sll_object_t _int_##v##_static_data={1,SLL_OBJECT_TYPE_INT,NULL,.dt={.i=(v)}};STATIC_OBJECT(&_int_##v##_static_data)
#define __SLL_STATIC_NEG_INT_OBJECT(v) static sll_object_t _int_neg_##v##_static_data={1,SLL_OBJECT_TYPE_INT,NULL,.dt={.i=-(v)}};STATIC_OBJECT(&_int_neg_##v##_static_data)
#define __SLL_STATIC_CHAR_OBJECT(v) static sll_object_t _char_##v##_static_data={1,SLL_OBJECT_TYPE_CHAR,NULL,.dt={.c=(sll_char_t)(v)}};STATIC_OBJECT(&_char_##v##_static_data)
#define __SLL_STATIC_OBJECT(nm,t,f,v) static sll_object_t _##nm##_static_data={1,t,NULL,.dt={.f=v}};__SLL_EXTERNAL sll_object_t* sll_static_##nm=&_##nm##_static_data



#define _UNIQUE_NAME_JOIN2(a,b) a##_##b
#define _UNIQUE_NAME_JOIN(a,b) _UNIQUE_NAME_JOIN2(a,b)
#define _UNIQUE_NAME(a) _UNIQUE_NAME_JOIN(a,__LINE__)
#define _SLL_ASSERT_STRINGIFY_(x) #x
#define _SLL_ASSERT_STRINGIFY(x) _SLL_ASSERT_STRINGIFY_(x)
#define SLL_UNIMPLEMENTED() _force_exit(SLL_CHAR("File \""__FILE__"\", Line "_SLL_ASSERT_STRINGIFY(__LINE__)" ("),SLL_CHAR(__func__),SLL_CHAR("): Unimplemented\n"),NULL,NULL);
#ifdef DEBUG_BUILD
#define ASSUME_ALIGNED(p,n,x) SLL_ASSERT(!((((uint64_t)(p))-(x))&((1<<(n))-1)))
#define SLL_ASSERT(x) \
	do{ \
		if (!(x)){ \
			_force_exit(SLL_CHAR("File \""__FILE__"\", Line "_SLL_ASSERT_STRINGIFY(__LINE__)" ("),SLL_CHAR(__func__),SLL_CHAR("): "_SLL_ASSERT_STRINGIFY(x)": Assertion Failed\n"),NULL,NULL); \
		} \
	} while (0)
#define CHECK_INTERNAL_FUNCTION_NAME(s) \
	do{ \
		if ((s)->l>255){ \
			_force_exit(SLL_CHAR("File \""__FILE__"\", Line "_SLL_ASSERT_STRINGIFY(__LINE__)" ("),SLL_CHAR(__func__),SLL_CHAR("): Internal Function Name longer than 255 Characters: "),(s)->v,SLL_CHAR("\n")); \
		} \
		if (sll_string_count_char((s),':')!=1){ \
			_force_exit(SLL_CHAR("File \""__FILE__"\", Line "_SLL_ASSERT_STRINGIFY(__LINE__)" ("),SLL_CHAR(__func__),SLL_CHAR("): Internal Function Name does not contain one Colon (':'): "),(s)->v,SLL_CHAR("\n")); \
		} \
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

#define EXTRA_COMPILATION_DATA_INSIDE_FUNCTION 1
#define EXTRA_COMPILATION_DATA_IMPORT 2
#define EXTRA_COMPILATION_DATA_EXPORT 4
#define EXTRA_COMPILATION_DATA_VARIABLE_DEFINITION 8

#define VARIABLE_OFFSET_NEVER_DELETE ((void*)0xffffffffffffffff)

#define ERROR_DISPLAY_TAB_WIDTH 4

#define ASSEMBLY_INSTRUCTION_TYPE_DBG SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED0
#define ASSEMBLY_INSTRUCTION_TYPE_DBG_FUNC SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED1
#define ASSEMBLY_INSTRUCTION_TYPE_FUNC_START SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED2
#define ASSEMBLY_INSTRUCTION_TYPE_LABEL_TARGET SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED3
#define ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED4
#define ASSEMBLY_INSTRUCTION_TYPE_NOP SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED5
#define ASSEMBLY_INSTRUCTION_LABEL 128
#define ASSEMBLY_INSTRUCTION_MISC_FIELD(ai) ((ai)->dt.j)

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



typedef uint16_t scope_data_mask_length_t;



typedef sll_instruction_index_t assembly_instruction_label_t;



typedef struct __SCOPE_DATA{
	uint64_t* m;
	sll_scope_t l_sc;
	scope_data_mask_length_t ml;
} scope_data_t;



typedef struct __NEW_VARIABLE_DATA{
	sll_node_t** dt;
	uint32_t sz;
} new_variable_data_t;



typedef struct __EXTRA_COMPILATION_DATA{
	uint8_t fl;
	scope_data_t sc;
	sll_internal_function_table_t* i_ft;
	sll_import_loader_t il;
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
	sll_object_type_t oi_off;
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



typedef struct __LOOP_TABLE{
	loop_t* dt;
	uint32_t sz;
} loop_table_t;



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
	loop_table_t* lt;
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
	const char* fp;
	unsigned int ln;
	char fn[256];
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
	sll_bool_t st;
} function_log_data_t;



typedef struct __FILE_LOG_DATA{
	const sll_string_t nm;
	function_log_data_t** dt;
	sll_array_length_t dtl;
	sll_bool_t st;
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



#ifdef _MSC_VER
extern void* _win_dll_handle;
#endif



sll_assembly_instruction_t* _acquire_next_instruction(sll_assembly_data_t* a_dt);



sll_node_t* _acquire_next_node(sll_compilation_data_t* c_dt);



void _file_init_std_streams(void);



void _file_release_std_streams(void);



__SLL_NO_RETURN void _force_exit(const sll_char_t* a,const sll_char_t* b,const sll_char_t* c,const sll_char_t* d,const sll_char_t* e);



__SLL_NO_RETURN void _force_exit_platform(void);



void _gc_release_data(void);



sll_assembly_instruction_t* _get_instruction_at_offset(const sll_assembly_data_t* a_dt,sll_instruction_index_t off);



sll_node_t* _get_node_at_offset(const sll_compilation_data_t* c_dt,sll_node_offset_t off);



void _init_assembly_stack(sll_assembly_data_t* a_dt);



void _init_node_stack(sll_compilation_data_t* c_dt);



void _log_release_data(void);



void _memory_release_data(void);



void _shift_nodes(sll_node_t* o,sll_compilation_data_t* c_dt,sll_node_offset_t off);



#endif
