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
#pragma intrinsic(__movsb)
#pragma intrinsic(__stosb)
#pragma intrinsic(_BitScanForward)
#define FORCE_INLINE __inline __forceinline
#define UNREACHABLE() __assume(0)
#define PACKED(s) __pragma(pack(push,1)) s __pragma(pack(pop))
#define REPEAT_BYTE_COPY(d,s,sz) __movsb((d),(s),(sz))
#define REPEAT_BYTE_SET(d,v,sz) __stosb(d,v,sz)
static FORCE_INLINE unsigned int FIND_FIRST_SET_BIT(unsigned int m){
	unsigned long o;
	_BitScanForward	(&o,m);
	return o;
}
#else
#define FORCE_INLINE inline __attribute__((always_inline))
#define UNREACHABLE() __builtin_unreachable()
#define PACKED(s) s __attribute__((__packed__))
static FORCE_INLINE void REPEAT_BYTE_COPY(unsigned char* d,unsigned char* s,size_t n){
	__asm__ volatile("rep movsb":"=D"(d),"=S"(s),"=c"(n):"0"(d),"1"(s),"2"(n):"memory");
}
static FORCE_INLINE void REPEAT_BYTE_SET(unsigned char* d,uint8_t v,size_t n){
	__asm__ volatile("rep stosb":"=D"(d),"=A"(v),"=c"(n):"0"(d),"1"(v),"2"(n):"memory");
}
#define FIND_FIRST_SET_BIT(m) (__builtin_ffs((m))-1)
#endif



#define _ASSERT_STR_(l) #l
#define _ASSERT_STR(l) _ASSERT_STR_(l)
#define ASSERT(x,e,r) \
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
#define ASSERT_EXIT(x) \
	do{ \
		if (!(x)){ \
			printf("File \"%s\", Line %u (%s): %s: Assertion Failed\n",__FILE__,__LINE__,__func__,_ASSERT_STR(x)); \
			raise(SIGABRT); \
		} \
	} while (0)

#define CONSTRUCT_CHAR(c) ((#c)[0])
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
#define FAST_COMPARE_STR_6(a,b) (*((uint32_t*)(a))==*((uint32_t*)(b))&&*((uint16_t*)(a)+4)==*((uint16_t*)(b)+4))
#define FAST_COMPARE_STR_7(a,b) (*((uint32_t*)(a))==*((uint32_t*)(b))&&*((uint16_t*)(a)+4)==*((uint16_t*)(b)+4)&&*((a)+6)==*((b)+6))
#define FAST_COMPARE_STR_8(a,b) (*((uint64_t*)(a))==*((uint64_t*)(b)))
#define FAST_COMPARE_STR_9(a,b) (*((uint64_t*)(a))==*((uint64_t*)(b))&&*((a)+8)==*((b)+8))
#define FAST_COMPARE_STR_10(a,b) (*((uint64_t*)(a))==*((uint64_t*)(b))&&*((uint16_t*)(a)+8)==*((uint16_t*)(b)+8))
#define FAST_COMPARE_STR_11(a,b) (*((uint64_t*)(a))==*((uint64_t*)(b))&&*((uint16_t*)(a)+8)==*((uint16_t*)(b)+8)&&*((a)+10)==*((b)+10))
#define FAST_COMPARE_STR_12(a,b) (*((uint64_t*)(a))==*((uint64_t*)(b))&&*((uint32_t*)(a)+8)==*((uint32_t*)(b)+8))
#define FAST_COMPARE_STR_13(a,b) (*((uint64_t*)(a))==*((uint64_t*)(b))&&*((uint32_t*)(a)+8)==*((uint32_t*)(b)+8)&&*((a)+12)==*((b)+12))
#define FAST_COMPARE_STR_14(a,b) (*((uint64_t*)(a))==*((uint64_t*)(b))&&*((uint32_t*)(a)+8)==*((uint32_t*)(b)+8)&&*((uint16_t*)(a)+12)==*((uint16_t*)(b)+12))
#define FAST_COMPARE_STR_15(a,b) (*((uint64_t*)(a))==*((uint64_t*)(b))&&*((uint32_t*)(a)+8)==*((uint32_t*)(b)+8)&&*((uint16_t*)(a)+12)==*((uint16_t*)(b)+12)&&*((a)+14)==*((b)+14))

#define COMPLIED_OBJECT_FILE_MAGIC_NUMBER CONSTRUCT_DWORD('L','L','L',0)

#define ERROR_DISPLAY_TAB_WIDTH 4

#define HIGHLIGHT_COLOR "\x1b[31m"
#define HIGHLIGHT_COLOR_RESET "\x1b[0m"

#define READ_SINGLE_CHAR_OK 0
#define READ_SINGLE_CHAR_END 1
#define READ_SINGLE_CHAR_ERROR 2

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
#define REGISER_SIZE_MASK 0x30
#define REGISTER_NONE 0xff
#define MIN_REGISTER REGISTER_A
#define MAX_REGISTER REGISTER_R15
#define ALL_REGISTER_AVAIBLE_MASK ((1<<(MAX_REGISTER-MIN_REGISTER+1))-1)
#define REGISTER_TO_MASK(r) (1<<((r)-MIN_REGISTER))
#define REGISTER_FROM_BIT_INDER(r) ((r)+MIN_REGISTER)

#define IDENTIFIER_INDEX_TO_MAP_OFFSET(i,im) (LLL_IDENTIFIER_GET_ARRAY_INDEX((i))+(im)->off[LLL_IDENTIFIER_GET_ARRAY_ID((i))])

#define COMPARE_ALWAYS_TRUE 2
#define COMPARE_ALWAYS_FALSE 3



typedef uint8_t cpu_register_t;


typedef uint32_t label_t;



typedef union __IDENTIFIER_DATA_EXTRA{
	int64_t v;
	uint32_t st;
} identifier_data_extra_t;



typedef struct __IDENTIFIER_DATA{
	cpu_register_t r;
	identifier_data_extra_t e;
} identifier_data_t;



typedef struct __IDENTIFIER_MAP{
	uint32_t off[LLL_MAX_SHORT_IDENTIFIER_LENGTH+1];
	identifier_data_t* dt;
	uint16_t rm;
	label_t nl;
} identifier_map_t;



typedef PACKED(struct __COMPILED_OBJECT_FILE{
	uint32_t m;
	uint32_t sz;
	uint64_t t;
	uint16_t fpl;
	uint32_t sil[LLL_MAX_SHORT_IDENTIFIER_LENGTH];
	uint32_t ill;
}) compiled_object_file_t;



extern uint8_t* _bf;
extern uint32_t _bf_off;
extern uint32_t _bf_sz;



int _input_data_stream_file_read(lll_input_data_stream_t* is);



void _input_data_stream_file_restart_line(lll_input_data_stream_t* is,uint32_t lp);



uint8_t _output_data_stream_file_write_char(lll_output_data_stream_t* os,char c);



uint8_t _output_data_stream_file_write_string(lll_output_data_stream_t* os,char* s);



uint8_t _output_data_stream_file_write(lll_output_data_stream_t* os,uint8_t* bf,size_t sz);



uint32_t _print_object_internal(lll_compilation_data_t* c_dt,lll_object_t* o,FILE* f);



uint8_t _read_single_char(lll_input_data_stream_t* is,char t,uint32_t st,lll_error_t* e);



uint8_t _read_object_internal(lll_compilation_data_t* c_dt,int c,uint32_t l_sc,lll_error_t* e);



uint32_t _get_object_size(lll_object_t* o);



uint32_t _write_object_as_assembly(lll_output_data_stream_t* os,lll_object_t* o,identifier_map_t* im,lll_error_t* e);



uint32_t _optimize_object_internal(lll_object_t* o,lll_error_t* e);



uint32_t _remove_debug_data_internal(lll_object_t* o);



uint32_t _remove_padding_internal(lll_object_t* o,uint32_t* rm);



#endif
