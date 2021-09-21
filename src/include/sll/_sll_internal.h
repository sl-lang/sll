#ifndef __SLL__SLL_INTERNAL_H__
#define __SLL__SLL_INTERNAL_H__ 1
#ifdef _MSC_VER
#include <intrin.h>
#endif
#include <sll/_api_generated.h>
#include <sll/api.h>
#include <sll/types.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>



#ifdef _MSC_VER
#pragma intrinsic(_BitScanForward64)
#pragma section("ifunc$a",read)
#pragma section("ifunc$b",read)
#pragma section("ifunc$z",read)
#pragma section("strto$a",read)
#pragma section("strto$b",read)
#pragma section("strto$z",read)
#ifdef DEBUG_BUILD
#define SLL_UNREACHABLE() SLL_ASSERT(!"UNREACHABLE")
#else
#define SLL_UNREACHABLE() __assume(0)
#endif
static __inline __forceinline unsigned int FIND_FIRST_SET_BIT(unsigned __int64 m){
	unsigned long o;
	_BitScanForward64(&o,m);
	return o;
}
#define IGNORE_RESULT(x) ((void)(x))
#define INTERNAL_FUNCTION(nm,f,fl) const static internal_function_t _UNIQUE_NAME(__ifunc)={(nm),(f),(fl)};const static __declspec(allocate("ifunc$b")) internal_function_t* _UNIQUE_NAME(__ifunc_ptr)=&_UNIQUE_NAME(__ifunc)
#define INTERNAL_FUNCTION_SETUP const static __declspec(allocate("ifunc$a")) internal_function_t* __ifunc_start=0;const static __declspec(allocate("ifunc$z")) internal_function_t* __ifunc_end=0
#define STATIC_RUNTIME_OBJECT(rt) const static static_runtime_object_t _UNIQUE_NAME(__strto)={(rt),__FILE__,__LINE__};const static __declspec(allocate("strto$b")) static_runtime_object_t* _UNIQUE_NAME(__strto_ptr)=&_UNIQUE_NAME(__strto)
#define STATIC_RUNTIME_OBJECT_SETUP const static __declspec(allocate("strto$a")) static_runtime_object_t* __strto_start=0;const static __declspec(allocate("strto$z")) static_runtime_object_t* __strto_end=0
#else
#ifdef DEBUG_BUILD
#define SLL_UNREACHABLE() SLL_ASSERT(!"UNREACHABLE")
#else
#define SLL_UNREACHABLE() __builtin_unreachable()
#endif
#define FIND_FIRST_SET_BIT(m) (__builtin_ffsll((m))-1)
#define IGNORE_RESULT(x) \
	do{ \
		unsigned long long int __tmp __attribute__((unused))=(unsigned long long int)(x); \
	} while (0)
#define INTERNAL_FUNCTION(nm,f,fl) const static internal_function_t _UNIQUE_NAME(__ifunc)={(nm),(f),(fl)};const static __attribute__((used,section("ifunc"))) internal_function_t* _UNIQUE_NAME(__ifunc_ptr)=&_UNIQUE_NAME(__ifunc)
#define INTERNAL_FUNCTION_SETUP extern const internal_function_t* __start_ifunc;extern const internal_function_t* __stop_ifunc
#define STATIC_RUNTIME_OBJECT(rt) const static static_runtime_object_t _UNIQUE_NAME(__strto)={(rt),__FILE__,__LINE__};const static __attribute__((used,section("strto"))) static_runtime_object_t* _UNIQUE_NAME(__strto_ptr)=&_UNIQUE_NAME(__strto)
#define STATIC_RUNTIME_OBJECT_SETUP extern const static_runtime_object_t* __start_strto;extern const static_runtime_object_t* __stop_strto
#define __ifunc_start __start_ifunc
#define __ifunc_end __stop_ifunc
#define __strto_start __start_strto
#define __strto_end __stop_strto
#endif



#define __API_FUNC(nm) __SLL_FUNC __SLL_API_TYPE_sll_api_##nm sll_api_##nm(__SLL_API_ARGS_sll_api_##nm)
#define __SLL_STATIC_INT_OBJECT(v) static sll_runtime_object_t _int_##v##_static_data={1,SLL_RUNTIME_OBJECT_TYPE_INT,SLL_NO_DEBUG_DATA,.dt={.i=(v)}};STATIC_RUNTIME_OBJECT(&_int_##v##_static_data)
#define __SLL_STATIC_NEG_INT_OBJECT(v) static sll_runtime_object_t _int_neg_##v##_static_data={1,SLL_RUNTIME_OBJECT_TYPE_INT,SLL_NO_DEBUG_DATA,.dt={.i=-(v)}};STATIC_RUNTIME_OBJECT(&_int_neg_##v##_static_data)
#define __SLL_STATIC_CHAR_OBJECT(v) static sll_runtime_object_t _char_##v##_static_data={1,SLL_RUNTIME_OBJECT_TYPE_CHAR,SLL_NO_DEBUG_DATA,.dt={.c=(sll_char_t)(v)}};STATIC_RUNTIME_OBJECT(&_char_##v##_static_data)
#define __SLL_STATIC_OBJECT(nm,t,f,v) static sll_runtime_object_t _##nm##_static_data={1,t,SLL_NO_DEBUG_DATA,.dt={.f=v}};sll_runtime_object_t* __SLL_STATIC_NAME(nm)=&_##nm##_static_data



#define _UNIQUE_NAME_JOIN2(a,b) a##_##b
#define _UNIQUE_NAME_JOIN(a,b) _UNIQUE_NAME_JOIN2(a,b)
#define _UNIQUE_NAME(a) _UNIQUE_NAME_JOIN(a,__LINE__)
#define _SLL_ASSERT_STRINGIFY_(x) #x
#define _SLL_ASSERT_STRINGIFY(x) _SLL_ASSERT_STRINGIFY_(x)
#define _SLL_ASSERT_JOIN_(x) _SLL_ASSERT_##x
#define _SLL_ASSERT_JOIN(x) _SLL_ASSERT_JOIN_(x)
#define _SLL_ASSERT_COUNT_ARGS(_1,_2,_3,n,...) n
#define SLL_UNIMPLEMENTED() \
	do{ \
		printf("File \""__FILE__"\", Line "_SLL_ASSERT_STRINGIFY(__LINE__)" (%s): Unimplemented\n",__func__); \
		fflush(stdout); \
		raise(SIGABRT); \
	} while (0)
#ifdef DEBUG_BUILD
#define SLL_ASSERT(...) _SLL_ASSERT_JOIN(_SLL_ASSERT_COUNT_ARGS(__VA_ARGS__,0,1,1))(__VA_ARGS__)
#define _SLL_ASSERT_0(x,e,r) \
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
#define _SLL_ASSERT_1(x,...) \
	do{ \
		if (!(x)){ \
			printf("File \""__FILE__"\", Line "_SLL_ASSERT_STRINGIFY(__LINE__)" (%s): "_SLL_ASSERT_STRINGIFY(x)": Assertion Failed\n",__func__); \
			fflush(stdout); \
			raise(SIGABRT); \
		} \
	} while (0)
#else
#define SLL_ASSERT(...)
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

#define ASSEMBLY_INSTRUCTION_TYPE_FUNC_START SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED0
#define ASSEMBLY_INSTRUCTION_TYPE_LABEL_TARGET SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED1
#define ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED2
#define ASSEMBLY_INSTRUCTION_LABEL 128
#define ASSEMBLY_INSTRUCTION_MISC_FIELD(ai) ((ai)->dt.j)

#define OPTIMIZER_FLAG_ARGUMENT 1
#define OPTIMIZER_FLAG_IGNORE_LOOP_FLAG 2

#define COND_TYPE_UNKNOWN 0
#define COND_TYPE_ALWAYS_TRUE 1
#define COND_TYPE_ALWAYS_FALSE 2

#define CALL_STACK_SIZE 256

#define RUNTIME_OBJECT_TYPE_FUNCTION_ID SLL_RUNTIME_OBJECT_TYPE_RESERVED0
#define RUNTIME_OBJECT_TYPE_UNKNOWN SLL_RUNTIME_OBJECT_TYPE_RESERVED1
#define RUNTIME_OBJECT_CHANGE_IN_LOOP SLL_RUNTIME_OBJECT_FLAG_RESERVED0
#define RUNTIME_OBJECT_EXTERNAL_STRING SLL_RUNTIME_OBJECT_FLAG_RESERVED1

#define GC_INIT_PAGE_COUNT 4
#define GC_GET_NEXT_OBJECT(o) ((sll_runtime_object_t*)((o)->dt.s.v))
#define GC_SET_NEXT_OBJECT(o,n) ((o)->dt.s.v=(sll_char_t*)(n))

#define GC_MAX_DBG_ID 16777215

#define MAX_CLEANUP_TABLE_SIZE 512
#define MAX_LAST_CLEANUP_TABLE_SIZE 16
#define CLEANUP_ORDER_LAST SLL_CLEANUP_ORDER_RESERVED0

#define MAX_ASSEMBLY_INSTRUCTION_LABEL SLL_MAX_INSTRUCTION_INDEX

#define OBJECT_TYPE_CHANGE_STACK SLL_OBJECT_TYPE_RESERVED0

#define ASSEMBLY_INSTRUCTION_STACK_PAGE_ALLOC_COUNT 8
#define OBJECT_STACK_PAGE_ALLOC_COUNT 8



typedef uint16_t call_stack_size_t;



typedef uint16_t scope_data_mask_length_t;



typedef sll_instruction_index_t assembly_instruction_label_t;



typedef struct __SCOPE_DATA{
	uint64_t* m;
	sll_scope_t l_sc;
	scope_data_mask_length_t ml;
} scope_data_t;



typedef struct __NEW_VARIABLE_DATA{
	sll_object_t** dt;
	uint32_t sz;
} new_variable_data_t;



typedef struct __EXTRA_COMPILATION_DATA{
	uint8_t fl;
	scope_data_t sc;
	sll_internal_function_table_t* i_ft;
	sll_import_loader_t il;
	new_variable_data_t* nv_dt;
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
	sll_internal_function_table_t* i_ft;
	identifier_map_data_t it;
	identifier_remap_data_t im;
	sll_runtime_object_t** v;
	sll_variable_index_t vi;
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



typedef struct __CALL_STACK_FRAME{
	sll_instruction_index_t ii;
	sll_stack_offset_t s;
} call_stack_frame_t;



typedef struct __CALL_STACK{
	call_stack_frame_t* dt;
	call_stack_size_t l;
} call_stack_t;



typedef struct __INTERNAL_FUNCTION{
	const sll_char_t nm[256];
	sll_internal_function_pointer_t f;
	sll_internal_function_type_t t;
} internal_function_t;



typedef struct __RUNTIME_OBJECT_DEBUG_DATA_TRACE_DATA{
	const char* fp;
	unsigned int ln;
	char fn[256];
} runtime_object_debug_data_trace_data_t;



typedef struct __RUNTIME_OBJECT_DEBUG_DATA{
	runtime_object_debug_data_trace_data_t c;
	runtime_object_debug_data_trace_data_t** al;
	uint32_t all;
	runtime_object_debug_data_trace_data_t** rl;
	uint32_t rll;
} runtime_object_debug_data_t;



typedef struct __FILE_LIST_DATA{
	sll_string_t* dt;
	sll_array_length_t l;
} file_list_data_t;



typedef struct __STATIC_RUNTIME_OBJECT{
	sll_runtime_object_t* dt;
	const char* fp;
	unsigned int ln;
} static_runtime_object_t;



sll_assembly_instruction_t* _acquire_next_instruction(sll_assembly_data_t* a_dt);



sll_object_t* _acquire_next_object(sll_compilation_data_t* c_dt);



sll_object_t* _get_object_at_offset(const sll_compilation_data_t* c_dt,sll_object_offset_t off);



void _init_assembly_stack(sll_assembly_data_t* a_dt);



void _init_object_stack(sll_compilation_data_t* c_dt);



void _shift_objects(sll_object_t* o,sll_compilation_data_t* c_dt,sll_object_offset_t off);



#endif
