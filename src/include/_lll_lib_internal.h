#ifndef ___LLL_LIB_INTERNAL_H__
#define ___LLL_LIB_INTERNAL_H__ 1
#ifdef _MSC_VER
#include <intrin.h>
#endif
#include <lll_lib.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>



#ifdef _MSC_VER
#pragma intrinsic(_BitScanForward64)
#ifdef DEBUG_BUILD
#define UNREACHABLE() ASSERT(!"UNREACHABLE")
#else
#define UNREACHABLE() __assume(0)
#endif
static __inline __forceinline unsigned int FIND_FIRST_SET_BIT(unsigned __int64 m){
	unsigned long o;
	_BitScanForward64(&o,m);
	return o;
}
#define IGNORE(x) (void)(x)
#else
#ifdef DEBUG_BUILD
#define UNREACHABLE() ASSERT(!"UNREACHABLE")
#else
#define UNREACHABLE() __builtin_unreachable()
#endif
#define FIND_FIRST_SET_BIT(m) (__builtin_ffsll((m))-1)
#define IGNORE(x) \
	do{ \
		unsigned long long int __tmp __attribute__((unused))=(unsigned long long int)(x); \
	} while (0)
#endif



#ifdef DEBUG_BUILD
#define _ASSERT_STR_(l) #l
#define _ASSERT_STR(l) _ASSERT_STR_(l)
#define _ASSERT_JOIN_(l) ASSERT_##l
#define _ASSERT_JOIN(l) _ASSERT_JOIN_(l)
#define _ASSERT_COUNT_ARGS(_1,_2,_3,n,...) n
#define ASSERT(...) _ASSERT_JOIN(_ASSERT_COUNT_ARGS(__VA_ARGS__,ERROR,EXIT,EXIT))(__VA_ARGS__)
#define ASSERT_ERROR(x,e,r) \
	do{ \
		if (!(x)){ \
			e->t=LLL_ERROR_ASSERTION; \
			const char __tmp0[]="File \""__FILE__"\", Line "_ASSERT_STR(__LINE__)" ("; \
			uint32_t __i=0; \
			for (uint32_t __j=0;__j<sizeof(__tmp0)/sizeof(char)-1;__j++){ \
				e->dt.str[__i]=__tmp0[__j]; \
				__i++; \
			} \
			for (uint32_t __j=0;__j<sizeof(__func__)/sizeof(char)-1;__j++){ \
				e->dt.str[__i]=__func__[__j]; \
				__i++; \
			} \
			const char __tmp1[]="): "_ASSERT_STR(x)": Assertion Failed"; \
			for (uint32_t __j=0;__j<sizeof(__tmp1)/sizeof(char);__j++){ \
				e->dt.str[__i]=__tmp1[__j]; \
				__i++; \
			} \
			return r; \
		} \
	} while (0)
#define ASSERT_EXIT(x,...) \
	do{ \
		if (!(x)){ \
			printf("File \"%s\", Line %u (%s): %s: Assertion Failed\n",__FILE__,__LINE__,__func__,_ASSERT_STR(x)); \
			raise(SIGABRT); \
		} \
	} while (0)
#else
#define ASSERT(...)
#define ASSERT_ERROR(x,e,r)
#define ASSERT_EXIT(x,...)
#endif

#define CONSTRUCT_DWORD(a,b,c,d) ((((uint32_t)(d))<<24)|(((uint32_t)(c))<<16)|(((uint32_t)(b))<<8)|(a))

#define MAX(a,b) ((a)>(b)?(a):(b))

#define ASSEMBLY_FILE_MAGIC_NUMBER CONSTRUCT_DWORD('L','L','A',0)
#define COMPLIED_OBJECT_FILE_MAGIC_NUMBER CONSTRUCT_DWORD('L','L','C',0)

#define EXTRA_COMPILATION_DATA_INSIDE_FUNCTION 1

#define ERROR_DISPLAY_TAB_WIDTH 4

#define ASSEMBLY_INSTRUCTION_TYPE_FUNC_START LLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED0
#define ASSEMBLY_INSTRUCTION_TYPE_LABEL_TARGET LLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED1
#define ASSEMBLY_INSTRUCTION_LABEL 128
#define ASSEMBLY_INSTRUCTION_MISC_FIELD(ai) ((ai)->dt.j)

#define OPTIMIZER_FLAG_ARGUMENT 1

#define COND_TYPE_UNKNOWN 0
#define COND_TYPE_ALWAYS_TRUE 1
#define COND_TYPE_ALWAYS_FALSE 2

#define CALL_STACK_SIZE 256

#define OBJECT_TYPE_EXPORT LLL_OBJECT_TYPE_RESERVED0

#define RUNTIME_OBJECT_CHANGE_IN_LOOP LLL_RUNTIME_OBJECT_RESERVED0
#define RUNTIME_OBJECT_TYPE_UNKNOWN 5



typedef uint16_t call_stack_size_t;



typedef uint16_t scope_data_mask_length_t;



typedef lll_instruction_index_t assembly_instruction_label_t;



typedef struct __SCOPE_DATA{
	uint64_t* m;
	lll_scope_t l_sc;
	scope_data_mask_length_t ml;
} scope_data_t;



typedef struct __EXTRA_COMPILATION_DATA{
	uint8_t fl;
	scope_data_t sc;
	lll_internal_function_table_t* i_ft;
} extra_compilation_data_t;



typedef struct __IMPORT_DATA{
	lll_import_index_t i;
	lll_object_offset_t off;
	lll_object_t* b_off;
	uint8_t rm;
} import_data_t;



typedef struct __IMPORT_MODULE_DATA{
	lll_identifier_list_length_t off[LLL_MAX_SHORT_IDENTIFIER_LENGTH+1];
	lll_string_index_t* sm;
} import_module_data_t;



typedef struct __IDENTIFIER_DATA{
	lll_variable_index_t v;
	uint32_t c;
} identifier_data_t;



typedef struct __IDENTIFIER_MAP_DATA{
	identifier_data_t* s_im[LLL_MAX_SHORT_IDENTIFIER_LENGTH];
	identifier_data_t* l_im;
	lll_variable_index_t n_vi;
	lll_scope_t l_sc;
	lll_variable_index_t* sc_vi;
	lll_variable_index_t vc;
} identifier_map_data_t;



typedef struct __OPTIMIZER_DATA{
	lll_compilation_data_t* c_dt;
	identifier_map_data_t it;
	lll_runtime_object_t* v;
	lll_variable_index_t vi;
	uint8_t rm;
} optimizer_data_t;



typedef struct __ASSEMBLY_GENERATOR_DATA{
	lll_assembly_data_t* a_dt;
	const lll_compilation_data_t* c_dt;
	identifier_map_data_t it;
	assembly_instruction_label_t n_lbl;
} assembly_generator_data_t;



typedef struct __STRING_MAP_DATA{
	lll_string_index_t ml;
	uint64_t* m;
	lll_string_index_t* im;
} strint_map_data_t;



typedef struct __CALL_STACK_FRAME{
	lll_instruction_index_t ii;
	lll_stack_offset_t s;
} call_stack_frame_t;



typedef struct __CALL_STACK{
	call_stack_frame_t* dt;
	call_stack_size_t l;
} call_stack_t;



lll_object_offset_t _map_identifiers(const lll_object_t* o,const lll_compilation_data_t* c_dt,identifier_map_data_t* im);



#endif
