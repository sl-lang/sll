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
#define UNREACHABLE() __assume(0)
static __inline __forceinline unsigned int FIND_FIRST_SET_BIT(unsigned __int64 m){
	unsigned long o;
	_BitScanForward64(&o,m);
	return o;
}
#define POPCOUNT(m) __popcnt64(m)
#else
#define UNREACHABLE() __builtin_unreachable()
#define FIND_FIRST_SET_BIT(m) (__builtin_ffsll((m))-1)
#define POPCOUNT(m) __builtin_popcountll(m)
#endif



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

#define CONSTRUCT_DWORD(a,b,c,d) ((((uint32_t)(d))<<24)|(((uint32_t)(c))<<16)|(((uint32_t)(b))<<8)|(a))

#define MAX(a,b) ((a)>(b)?(a):(b))

#define ASSEMBLY_FILE_MAGIC_NUMBER CONSTRUCT_DWORD('L','L','A',0)
#define COMPLIED_OBJECT_FILE_MAGIC_NUMBER CONSTRUCT_DWORD('L','L','C',0)

#define ERROR_DISPLAY_TAB_WIDTH 4

#define ASSEMBLY_INSTRUCTION_LABEL 128

#define RUNTIME_OBJECT_COMPARE_BELOW 0
#define RUNTIME_OBJECT_COMPARE_EQUAL 1
#define RUNTIME_OBJECT_COMPARE_ABOVE 2



typedef uint16_t scope_data_mask_length_t;



typedef lll_instruction_index_t assembly_instruction_label_t;



typedef struct __SCOPE_DATA{
	uint64_t* m;
	lll_scope_t l_sc;
	scope_data_mask_length_t ml;
} scope_data_t;



typedef struct __IMPORT_DATA{
	lll_import_index_t i;
	lll_stack_offset_t off;
	uint64_t b_off;
	uint8_t rm;
} import_data_t;



typedef struct __IMPORT_MODULE_DATA{
	lll_identifier_list_length_t off[LLL_MAX_SHORT_IDENTIFIER_LENGTH+1];
	lll_string_index_t* sm;
} import_module_data_t;



typedef struct __IDENTIFIER_MAP_DATA{
	lll_variable_index_t* s_im[LLL_MAX_SHORT_IDENTIFIER_LENGTH];
	lll_variable_index_t* l_im;
	lll_variable_index_t n_vi;
	lll_scope_t l_sc;
	lll_variable_index_t* sc_vi;
	lll_scope_t sc_vi_l;
} identifier_map_data_t;



typedef struct __LABEL_DATA{
	assembly_instruction_label_t nxt;
	lll_instruction_index_t* m;
	assembly_instruction_label_t ml;
} label_data_t;



typedef struct __ASSEMBLY_GENERATOR_DATA{
	lll_assembly_data_t* a_dt;
	const lll_compilation_data_t* c_dt;
	identifier_map_data_t im;
	label_data_t lbl;
} assembly_generator_data_t;



typedef struct __STRING_MAP_DATA{
	lll_string_index_t ml;
	uint64_t* m;
	lll_string_index_t* im;
} strint_map_data_t;



lll_string_index_t _create_string(lll_compilation_data_t* c_dt,const lll_char_t* dt,lll_string_length_t l);



lll_stack_offset_t _get_object_size(const lll_object_t* o);



#endif
