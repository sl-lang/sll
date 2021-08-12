#ifndef ___SLL_INTERNAL_H__
#define ___SLL_INTERNAL_H__ 1
#ifdef _MSC_VER
#include <intrin.h>
#endif
#include <sll/types.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>



#ifdef _MSC_VER
#pragma intrinsic(_BitScanForward64)
#pragma section("ifunc$a",read)
#pragma section("ifunc$b",read)
#pragma section("ifunc$z",read)
#ifdef DEBUG_BUILD
#define UNREACHABLE() SLL_ASSERT(!"UNREACHABLE")
#else
#define UNREACHABLE() __assume(0)
#endif
static __inline __forceinline unsigned int FIND_FIRST_SET_BIT(unsigned __int64 m){
	unsigned long o;
	_BitScanForward64(&o,m);
	return o;
}
#define IGNORE_RESULT(x) ((void)(x))
#define INTERNAL_FUNCTION(nm,f) const static internal_function_t _INTERNAL_FUNCTION_NAME(__ifunc)={(nm),(f)};const static __declspec(allocate("ifunc$b")) internal_function_t* _INTERNAL_FUNCTION_NAME(__ifunc_ptr)=&_INTERNAL_FUNCTION_NAME(__ifunc);
#define IINTERNAL_FUNCTION_SETUP const static __declspec(allocate("ifunc$a")) internal_function_t* __ifunc_start=0;const static __declspec(allocate("ifunc$z")) internal_function_t* __ifunc_end=0;
#else
#ifdef DEBUG_BUILD
#define UNREACHABLE() SLL_ASSERT(!"UNREACHABLE")
#else
#define UNREACHABLE() __builtin_unreachable()
#endif
#define FIND_FIRST_SET_BIT(m) (__builtin_ffsll((m))-1)
#define IGNORE_RESULT(x) \
	do{ \
		unsigned long long int __tmp __attribute__((unused))=(unsigned long long int)(x); \
	} while (0)
#define INTERNAL_FUNCTION(nm,f) const static internal_function_t _INTERNAL_FUNCTION_NAME(__ifunc)={(nm),(f)};const static __attribute__((used,section("ifunc"))) internal_function_t* _INTERNAL_FUNCTION_NAME(__ifunc_ptr)=&_INTERNAL_FUNCTION_NAME(__ifunc);
#define IINTERNAL_FUNCTION_SETUP extern const internal_function_t* __start_ifunc;extern const internal_function_t* __stop_ifunc;
#define __ifunc_start __start_ifunc
#define __ifunc_end __stop_ifunc
#endif



#define _INTERNAL_FUNCTION_JOIN2(a,b,c) a##b##_##c
#define _INTERNAL_FUNCTION_JOIN(a,b,c) _INTERNAL_FUNCTION_JOIN2(a,b,c)
#define _INTERNAL_FUNCTION_NAME(a) _INTERNAL_FUNCTION_JOIN(a,__FILE_ID__,__LINE__)
#define _SLL_ASSERT_STRINGIFY_(x) #x
#define _SLL_ASSERT_STRINGIFY(x) _SLL_ASSERT_STRINGIFY_(x)
#define _SLL_ASSERT_JOIN_(x) SLL_ASSERT##x
#define _SLL_ASSERT_JOIN(x) _SLL_ASSERT_JOIN_(x)
#define _SLL_ASSERT_COUNT_ARGS(_1,_2,_3,n,...) n
#ifdef DEBUG_BUILD
#define SLL_ASSERT(...) _SLL_ASSERT_JOIN(_SLL_ASSERT_COUNT_ARGS(__VA_ARGS__,_ERROR,_EXIT,_EXIT))(__VA_ARGS__)
#define SLL_ASSERT_ERROR(x,e,r) \
	do{ \
		if (!(x)){ \
			(e)->t=SLL_ERROR_SLL_ASSERTION; \
			const char __tmp0[]="File \""__FILE__"\", Line "_SLL_ASSERT_STRINGIFY(__LINE__)" ("; \
			uint32_t __i=0; \
			for (uint32_t __j=0;__j<sizeof(__tmp0)/sizeof(char)-1;__j++){ \
				(e)->dt.str[__i]=__tmp0[__j]; \
				__i++; \
			} \
			for (uint32_t __j=0;__j<sizeof(__func__)/sizeof(char)-1;__j++){ \
				(e)->dt.str[__i]=__func__[__j]; \
				__i++; \
			} \
			const char __tmp1[]="): "_SLL_ASSERT_STRINGIFY(x)": Assertion Failed"; \
			for (uint32_t __j=0;__j<sizeof(__tmp1)/sizeof(char);__j++){ \
				(e)->dt.str[__i]=__tmp1[__j]; \
				__i++; \
			} \
			return r; \
		} \
	} while (0)
#define SLL_ASSERT_EXIT(x,...) \
	do{ \
		if (!(x)){ \
			printf("File \""__FILE__"\", Line "_SLL_ASSERT_STRINGIFY(__LINE__)" (%s): "_SLL_ASSERT_STRINGIFY(x)": Assertion Failed\n",__func__); \
			raise(SIGABRT); \
		} \
	} while (0)
#else
#define SLL_ASSERT(...)
#define SLL_ASSERT_ERROR(x,e,r)
#define SLL_ASSERT_EXIT(x,...)
#endif

#define ALIGN(a) ((((uint64_t)(a))+STRING_HEAP_ALIGNMENT-1)&(~(STRING_HEAP_ALIGNMENT-1)))
#define CORRECT_ALIGNMENT(n) SLL_ASSERT(!(((uint64_t)(n))&(STRING_HEAP_ALIGNMENT-1)))

#define CONSTRUCT_DWORD(a,b,c,d) ((((uint32_t)(d))<<24)|(((uint32_t)(c))<<16)|(((uint32_t)(b))<<8)|(a))

#define STRING_HEAP_ALIGNMENT 8
#define HEAP_SIGNATURE 'H'
#define GET_HEAP_SIGNATURE(s) ((s)->h&0xff)
#define GET_PADDING(s) ((s)->h>>8)

#define MEMORY_NODE_SIGNATURE 0x8000000000000000
#define MEMORY_NODE_GET_SIZE(n) ((n)->sz&0x7fffffffffffffff)

#define ASSEMBLY_FILE_MAGIC_NUMBER CONSTRUCT_DWORD('L','L','A',0)
#define COMPLIED_OBJECT_FILE_MAGIC_NUMBER CONSTRUCT_DWORD('L','L','C',0)

#define EXTRA_COMPILATION_DATA_INSIDE_FUNCTION 1
#define EXTRA_COMPILATION_DATA_IMPORT 2
#define EXTRA_COMPILATION_DATA_EXPORT 4
#define EXTRA_COMPILATION_DATA_VARIABLE_DEFINITION 8

#define ERROR_DISPLAY_TAB_WIDTH 4

#define ASSEMBLY_INSTRUCTION_TYPE_FUNC_START SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED0
#define ASSEMBLY_INSTRUCTION_TYPE_LABEL_TARGET SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED1
#define ASSEMBLY_INSTRUCTION_LABEL 128
#define ASSEMBLY_INSTRUCTION_MISC_FIELD(ai) ((ai)->dt.j)

#define OPTIMIZER_FLAG_ARGUMENT 1
#define OPTIMIZER_FLAG_IGNORE_LOOP_FLAG 2

#define COND_TYPE_UNKNOWN 0
#define COND_TYPE_ALWAYS_TRUE 1
#define COND_TYPE_ALWAYS_FALSE 2

#define CALL_STACK_SIZE 256

#define RUNTIME_OBJECT_CHANGE_IN_LOOP SLL_RUNTIME_OBJECT_RESERVED0
#define RUNTIME_OBJECT_TYPE_UNKNOWN 5



typedef uint16_t call_stack_size_t;



typedef uint16_t scope_data_mask_length_t;



typedef sll_instruction_index_t assembly_instruction_label_t;



typedef struct __SCOPE_DATA{
	uint64_t* m;
	sll_scope_t l_sc;
	scope_data_mask_length_t ml;
} scope_data_t;



typedef struct __EXTRA_COMPILATION_DATA{
	uint8_t fl;
	scope_data_t sc;
	sll_internal_function_table_t* i_ft;
	sll_import_loader_t il;
} extra_compilation_data_t;



typedef struct __IDENTIFIER_PAIR{
	sll_identifier_index_t a;
	sll_identifier_index_t b;
} identifier_pair_t;



typedef struct __IMPORT_MODULE_DATA{
	sll_identifier_list_length_t off[SLL_MAX_SHORT_IDENTIFIER_LENGTH+1];
	sll_string_index_t* sm;
	sll_function_index_t f_off;
	sll_object_t* s;
	sll_object_t* d;
	identifier_pair_t* eim;
	sll_export_table_length_t eiml;
} import_module_data_t;



typedef struct __IDENTIFIER_DATA{
	sll_variable_index_t v;
	uint32_t c;
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



typedef struct __OPTIMIZER_DATA{
	sll_compilation_data_t* c_dt;
	identifier_map_data_t it;
	identifier_remap_data_t im;
	sll_runtime_object_t* v;
	sll_variable_index_t vi;
	uint8_t rm;
} optimizer_data_t;



typedef struct __IDENTIFIER_REMOVE_DATA{
	void** s[SLL_MAX_SHORT_IDENTIFIER_LENGTH];
	void** l;
} identifier_remove_data_t;



typedef struct __ASSEMBLY_GENERATOR_DATA{
	sll_assembly_data_t* a_dt;
	const sll_compilation_data_t* c_dt;
	identifier_map_data_t it;
	assembly_instruction_label_t n_lbl;
	identifier_remove_data_t rm;
} assembly_generator_data_t;



typedef struct __STRING_MAP_DATA{
	sll_string_index_t ml;
	uint64_t* m;
	sll_string_index_t* im;
} strint_map_data_t;



typedef struct __CALL_STACK_FRAME{
	sll_instruction_index_t ii;
	sll_stack_offset_t s;
} call_stack_frame_t;



typedef struct __CALL_STACK{
	call_stack_frame_t* dt;
	call_stack_size_t l;
} call_stack_t;



typedef struct __INTERNAL_FUNCTION{
	const char nm[256];
	sll_internal_function_pointer_t f;
} internal_function_t;



typedef struct __PAGE_HEADER{
	void* p;
	uint64_t sz;
} page_header_t;



typedef struct __MEMORY_NODE{
	uint64_t sz;
	struct __MEMORY_NODE* p;
	struct __MEMORY_NODE* n;
} memory_node_t;



extern sll_string_t _zero_string;



#endif
