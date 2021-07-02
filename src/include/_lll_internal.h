#ifndef __LLL_INTERNAL_H__
#define __LLL_INTERNAL_H__ 1
#ifdef _MSC_VER
#include <intrin.h>
#include <immintrin.h>
#endif
#include <lll_lib.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>



#ifdef _MSC_VER
#pragma intrinsic(__popcnt16)
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanReverse)
#pragma intrinsic(_BitScanReverse64)
#define __LLL_API_FUNCTION
#define FORCE_INLINE __inline __forceinline
#define UNREACHABLE() __assume(0)
#define PACKED(s) __pragma(pack(push,1)) s __pragma(pack(pop))
static FORCE_INLINE unsigned int FIND_FIRST_SET_BIT(unsigned int m){
	unsigned long o;
	_BitScanForward(&o,m);
	return o;
}
static FORCE_INLINE unsigned int FIND_LAST_SET_BIT(unsigned int m){
	unsigned long o;
	_BitScanReverse(&o,m);
	return o;
}
static FORCE_INLINE unsigned int FIND_LAST_SET_BIT64(unsigned __int64 m){
	unsigned long o;
	_BitScanReverse64(&o,m);
	return o;
}
#define PARITY16(x) (__popcnt16((x))&1)
#else
#define __LLL_API_FUNCTION __attribute__((ms_abi))
#define FORCE_INLINE inline __attribute__((always_inline))
#define UNREACHABLE() __builtin_unreachable()
#define PACKED(s) s __attribute__((__packed__))
#define FIND_FIRST_SET_BIT(m) (__builtin_ffs((m))-1)
#define FIND_LAST_SET_BIT(m) (31-__builtin_clz((m)))
#define FIND_LAST_SET_BIT64(m) (63-__builtin_clzll((m)))
#define PARITY16(x) __builtin_parity((x))
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

#define _CONSTRUCT_CHAR_JOIN_(l) _CONSTRUCT_CHAR_##l
#define _CONSTRUCT_CHAR_JOIN(l) _CONSTRUCT_CHAR_JOIN_(l)
#define _CONSTRUCT_CHAR_COUNT_ARGS(_1,_2,n,...) n
#define CONSTRUCT_CHAR(...) _CONSTRUCT_CHAR_JOIN(_CONSTRUCT_CHAR_COUNT_ARGS(__VA_ARGS__,2,1))(__VA_ARGS__)
#define _CONSTRUCT_CHAR_1(c) ((#c)[0])
#define _CONSTRUCT_CHAR_2(...) (',')
#define CONSTRUCT_WORD(a,b) ((((uint16_t)(b))<<8)|(a))
#define CONSTRUCT_DWORD(a,b,c,d) ((((uint32_t)(d))<<24)|(((uint32_t)(c))<<16)|(((uint32_t)(b))<<8)|(a))
#define CONSTRUCT_QWORD(a,b,c,d,e,f,g,h) ((((uint64_t)(h))<<56)|(((uint64_t)(g))<<48)|(((uint64_t)(f))<<40)|(((uint64_t)(e))<<32)|(((uint64_t)(d))<<24)|(((uint64_t)(c))<<16)|(((uint64_t)(b))<<8)|(a))

#define _FAST_COMPARE_JOIN_(l) FAST_COMPARE_##l
#define _FAST_COMPARE_JOIN(l) _FAST_COMPARE_JOIN_(l)
#define _FAST_COMPARE_COUNT_ARGS(_1,_2,_3,_4,_5,_6,_7,_8,n,...) n
#define FAST_COMPARE(s,...) _FAST_COMPARE_JOIN(_FAST_COMPARE_COUNT_ARGS(__VA_ARGS__,8,7,6,5,4,3,2,1))(s,__VA_ARGS__)
#define FAST_COMPARE_1(s,a) (*(s)==CONSTRUCT_CHAR(a))
#define FAST_COMPARE_2(s,a,b) (*((uint16_t*)(s))==CONSTRUCT_WORD(CONSTRUCT_CHAR(a),CONSTRUCT_CHAR(b)))
#define FAST_COMPARE_3(s,a,b,c) (*((uint16_t*)(s))==CONSTRUCT_WORD(CONSTRUCT_CHAR(a),CONSTRUCT_CHAR(b))&&*((s)+2)==CONSTRUCT_CHAR(c))
#define FAST_COMPARE_4(s,a,b,c,d) (*((uint32_t*)(s))==CONSTRUCT_DWORD(CONSTRUCT_CHAR(a),CONSTRUCT_CHAR(b),CONSTRUCT_CHAR(c),CONSTRUCT_CHAR(d)))
#define FAST_COMPARE_5(s,a,b,c,d,e) (*((uint32_t*)(s))==CONSTRUCT_DWORD(CONSTRUCT_CHAR(a),CONSTRUCT_CHAR(b),CONSTRUCT_CHAR(c),CONSTRUCT_CHAR(d))&&*((s)+4)==CONSTRUCT_CHAR(e))
#define FAST_COMPARE_6(s,a,b,c,d,e,f) (*((uint32_t*)(s))==CONSTRUCT_DWORD(CONSTRUCT_CHAR(a),CONSTRUCT_CHAR(b),CONSTRUCT_CHAR(c),CONSTRUCT_CHAR(d))&&*((uint16_t*)(s+4))==CONSTRUCT_WORD(CONSTRUCT_CHAR(e),CONSTRUCT_CHAR(f)))
#define FAST_COMPARE_7(s,a,b,c,d,e,f,g) (*((uint32_t*)(s))==CONSTRUCT_DWORD(CONSTRUCT_CHAR(a),CONSTRUCT_CHAR(b),CONSTRUCT_CHAR(c),CONSTRUCT_CHAR(d))&&*((uint16_t*)(s+4))==CONSTRUCT_WORD(CONSTRUCT_CHAR(e),CONSTRUCT_CHAR(f))&&*((s)+6)==CONSTRUCT_CHAR(g))
#define FAST_COMPARE_8(s,a,b,c,d,e,f,g,h) (*((uint64_t*)(s))==CONSTRUCT_QWORD(CONSTRUCT_CHAR(a),CONSTRUCT_CHAR(b),CONSTRUCT_CHAR(c),CONSTRUCT_CHAR(d),CONSTRUCT_CHAR(e),CONSTRUCT_CHAR(f),CONSTRUCT_CHAR(g),CONSTRUCT_CHAR(h)))
#define _FAST_COMPARE_STR_JOIN_(l) FAST_COMPARE_STR_##l
#define _FAST_COMPARE_STR_JOIN(l) _FAST_COMPARE_STR_JOIN_(l)
#define FAST_COMPARE_STR(a,b,l) _FAST_COMPARE_STR_JOIN(l)(a,b)
#define FAST_COMPARE_STR_1(a,b) (*(a)==*(b))
#define FAST_COMPARE_STR_2(a,b) (*((uint16_t*)(a))==*((uint16_t*)(b)))
#define FAST_COMPARE_STR_3(a,b) (*((uint16_t*)(a))==*((uint16_t*)(b))&&*((a)+2)==*((b)+2))
#define FAST_COMPARE_STR_4(a,b) (*((uint32_t*)(a))==*((uint32_t*)(b)))
#define FAST_COMPARE_STR_5(a,b) (*((uint32_t*)(a))==*((uint32_t*)(b))&&*((a)+4)==*((b)+4))
#define FAST_COMPARE_STR_6(a,b) (*((uint32_t*)(a))==*((uint32_t*)(b))&&*((uint16_t*)((a)+4))==*((uint16_t*)((b)+4)))
#define FAST_COMPARE_STR_7(a,b) (*((uint32_t*)(a))==*((uint32_t*)(b))&&*((uint16_t*)(a)+4)==*((uint16_t*)(b)+4)&&*((a)+6)==*((b)+6))
#define FAST_COMPARE_STR_8(a,b) (*((uint64_t*)(a))==*((uint64_t*)(b)))
#define FAST_COMPARE_STR_9(a,b) (*((uint64_t*)(a))==*((uint64_t*)(b))&&*((a)+8)==*((b)+8))
#define FAST_COMPARE_STR_10(a,b) (*((uint64_t*)(a))==*((uint64_t*)(b))&&*((uint16_t*)(a)+8)==*((uint16_t*)(b)+8))
#define FAST_COMPARE_STR_11(a,b) (*((uint64_t*)(a))==*((uint64_t*)(b))&&*((uint16_t*)(a)+8)==*((uint16_t*)(b)+8)&&*((a)+10)==*((b)+10))
#define FAST_COMPARE_STR_12(a,b) (*((uint64_t*)(a))==*((uint64_t*)(b))&&*((uint32_t*)(a)+8)==*((uint32_t*)(b)+8))
#define FAST_COMPARE_STR_13(a,b) (*((uint64_t*)(a))==*((uint64_t*)(b))&&*((uint32_t*)(a)+8)==*((uint32_t*)(b)+8)&&*((a)+12)==*((b)+12))
#define FAST_COMPARE_STR_14(a,b) (*((uint64_t*)(a))==*((uint64_t*)(b))&&*((uint32_t*)(a)+8)==*((uint32_t*)(b)+8)&&*((uint16_t*)(a)+12)==*((uint16_t*)(b)+12))
#define FAST_COMPARE_STR_15(a,b) (*((uint64_t*)(a))==*((uint64_t*)(b))&&*((uint32_t*)(a)+8)==*((uint32_t*)(b)+8)&&*((uint16_t*)(a)+12)==*((uint16_t*)(b)+12)&&*((a)+14)==*((b)+14))
#define FAST_COMPARE_COMMA ,

#define COMPLIED_OBJECT_FILE_MAGIC_NUMBER CONSTRUCT_DWORD('L','L','L',0)

#define ERROR_DISPLAY_TAB_WIDTH 4

#define HIGHLIGHT_COLOR "\x1b[31m"
#define HIGHLIGHT_COLOR_RESET "\x1b[0m"

#define READ_SINGLE_CHAR_OK 0
#define READ_SINGLE_CHAR_ERROR 1

#define REGISTER_CONST 0
#define REGISTER_A 1
#define REGISTER_B 2
#define REGISTER_C 3
#define REGISTER_D 4
#define REGISTER_SI 5
#define REGISTER_DI 6
#define REGISTER_R8 7
#define REGISTER_R9 8
#define REGISTER_R10 9
#define REGISTER_R11 10
#define REGISTER_R12 11
#define REGISTER_R13 12
#define REGISTER_R14 13
#define REGISTER_R15 14
#define REGISTER_STACK 15
#define REGISTER_8BIT 0x00
#define REGISTER_16BIT 0x10
#define REGISTER_32BIT 0x20
#define REGISTER_64BIT 0x30
#define REGISTER_SIZE_MASK 0x30
#define REGISTER_TEMPORARY 0x40
#define REGISTER_KNOWN_VALUE 0x80
#define REGISTER_COPY_IDENTIFIER 0xe0
#define REGISTER_NONE 0xf0
#define MIN_REGISTER REGISTER_A
#define MAX_REGISTER REGISTER_R15
#define GET_BASE_REGISTER(r) ((r)&0xf)
#define ALL_REGISTER_AVAIBLE_MASK ((1<<(MAX_REGISTER-MIN_REGISTER+1))-1)
#define REGISTER_TO_MASK(r) (1<<((r)-MIN_REGISTER))
#define REGISTER_FROM_BIT_INDEX(r) ((r)+MIN_REGISTER)

#define IDENTIFIER_INDEX_TO_MAP_OFFSET(i,im) (LLL_IDENTIFIER_GET_ARRAY_INDEX((i))+(im)->off[LLL_IDENTIFIER_GET_ARRAY_ID((i))])

#define FUNCTION_NON_VOLATILE_REGISTERS (REGISTER_TO_MASK(REGISTER_A)|REGISTER_TO_MASK(REGISTER_C)|REGISTER_TO_MASK(REGISTER_D)|REGISTER_TO_MASK(REGISTER_R8)|REGISTER_TO_MASK(REGISTER_R9)|REGISTER_TO_MASK(REGISTER_R10)|REGISTER_TO_MASK(REGISTER_R11))
#define FUNCTION_CALL_REGISTER_COUNT (sizeof(FUNCTION_CALL_REGISTERS)/sizeof(cpu_register_t))

#define COMPARE_UNKNOWN 1
#define COMPARE_ALWAYS_TRUE 2
#define COMPARE_ALWAYS_FALSE 3
#define COMPARE_FIRST_TRUE 4

#define IDENTIFIER_DATA_TYPE_CHAR 1
#define IDENTIFIER_DATA_TYPE_INT8 2
#define IDENTIFIER_DATA_TYPE_INT16 4
#define IDENTIFIER_DATA_TYPE_INT32 8
#define IDENTIFIER_DATA_TYPE_INT64 16
#define IDENTIFIER_DATA_TYPE_UINT8 32
#define IDENTIFIER_DATA_TYPE_UINT16 64
#define IDENTIFIER_DATA_TYPE_UINT32 128
#define IDENTIFIER_DATA_TYPE_UINT64 256
#define IDENTIFIER_DATA_TYPE_FLOAT32 512
#define IDENTIFIER_DATA_TYPE_FLOAT64 1024
#define IDENTIFIER_DATA_TYPE_STRING 2048
#define IDENTIFIER_DATA_TYPE_NIL 4096
#define IDENTIFIER_DATA_TYPE_FUNCTION 8192
#define IS_IDENTIFIER_DATA_TYPE_SINGLE(t) (!((t)&((t)-1)))



typedef uint8_t cpu_register_t;



typedef uint16_t identifier_data_type_t;



typedef uint32_t label_t;



typedef struct __IDENTIFIER_DATA_EXTRA_STRING{
	uint32_t l;
	char* ptr;
} identifier_data_extra_string_t;



typedef union __IDENTIFIER_DATA_EXTRA{
	int64_t v;
	uint32_t st;
	identifier_data_extra_string_t str;
	lll_function_index_t fn;
} identifier_data_extra_t;



typedef struct __IDENTIFIER_DATA{
	cpu_register_t r;
	identifier_data_type_t t;
	identifier_data_extra_t e;
	uint32_t sc;
} identifier_data_t;



typedef struct __IDENTIFIER_MAP{
	uint32_t off[LLL_MAX_SHORT_IDENTIFIER_LENGTH+1];
	identifier_data_t* dt;
	uint32_t dtl;
	uint16_t rm;
	label_t nl;
} identifier_map_t;



typedef struct __STRING_TABLE_ENTRY{
	uint32_t sz;
	uint8_t nb;
	char v[];
} string_table_entry_t;



typedef struct __STRING_TABLE{
	string_table_entry_t** dt;
	uint32_t l;
} string_table_t;



typedef struct __ASSEMBLY_GENERATOR_DATA{
	identifier_map_t im;
	string_table_t st;
	uint32_t n_sc;
} assembly_generator_data_t;



typedef struct __EXTENDED_ASSEMBLY_GENERATOR_DATA{
	assembly_generator_data_t* agd;
	uint32_t sc;
} extended_assembly_generator_data_t;



typedef PACKED(struct __COMPILED_OBJECT_FILE{
	uint32_t m;
	uint32_t sz;
	uint64_t t;
	uint32_t sil[LLL_MAX_SHORT_IDENTIFIER_LENGTH];
	uint32_t ill;
	uint32_t iml;
	uint16_t fp_dtl;
	uint16_t f_dtl;
}) compiled_object_file_t;



typedef struct __SCOPE_DATA{
	uint64_t* m;
	uint32_t l_sc;
	uint16_t ml;
} scope_data_t;



typedef struct __IMPORT_DATA{
	uint32_t i;
	uint32_t off;
	uint64_t b_off;
	uint8_t rm;
} import_data_t;



typedef struct __IMPORT_IDENTIFIER_OFFSET_LIST{
	uint32_t off[LLL_MAX_SHORT_IDENTIFIER_LENGTH+1];
	uint16_t dbg_off;
} import_identifier_offset_list_t;



extern uint8_t* _bf;
extern uint32_t _bf_off;
extern uint32_t _bf_sz;



const static cpu_register_t FUNCTION_CALL_REGISTERS[]={REGISTER_C,REGISTER_D,REGISTER_R8,REGISTER_R9};



uint32_t _get_object_size(lll_object_t* o);



#endif
