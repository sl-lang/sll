#ifndef __LLL_INTERNAL_H__
#define __LLL_INTERNAL_H__ 1
#ifdef _MSC_VER
#include <windows.h>
#include <immintrin.h>
#endif
#include <lll.h>
#include <stdint.h>
#include <stdio.h>



#ifdef _MSC_VER
#pragma intrinsic(__movsb)
#define REPEAT_BYTE_COPY(d,s,sz) __movsb(d,s,sz)
#define UNREACHABLE() __assume(0)
#define ENABLE_COLOR() \
	uint32_t __tv; \
	do{ \
		SetConsoleOutputCP(CP_UTF8); \
		GetConsoleMode(GetStdHandle(-11),&__tv); \
		SetConsoleMode(GetStdHandle(-11),7); \
	} while (0)
#define DISABLE_COLOR() SetConsoleMode(GetStdHandle(-11),__tv)
#else
static inline void REPEAT_BYTE_COPY(unsigned char* d,unsigned char* s,size_t n){
	__asm__("rep movsb":"=D"(d),"=S"(s),"=c"(n):"0"(d),"1"(s),"2"(n):"memory");
}
#define UNREACHABLE() __builtin_unreachable()
#define ENABLE_COLOR()
#define DISABLE_COLOR()
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
				e->err[__i]=__tmp0[__j]; \
				__i++; \
			} \
			for (uint32_t __j=0;__j<sizeof(__func__)/sizeof(char)-1;__j++){ \
				e->err[__i]=__func__[__j]; \
				__i++; \
			} \
			const char __tmp1[]="): "_ASSERT_STR(x)": Assertion Failed"; \
			for (uint32_t __j=0;__j<sizeof(__tmp1)/sizeof(char);__j++){ \
				e->err[__i]=__tmp1[__j]; \
				__i++; \
			} \
			return r; \
		} \
	} while (0)
#define CONSTRUCT_CHAR(c) ((#c)[0])
#define CONSTRUCT_WORD(a,b) ((((uint16_t)(b))<<8)|(a))
#define CONSTRUCT_DWORD(a,b,c,d) ((((uint32_t)(d))<<24)|(((uint32_t)(c))<<16)|(((uint32_t)(b))<<8)|(a))
#define CONSTRUCT_QWORD(a,b,c,d,e,f,g,h) ((((uint64_t)(h))<<56)|(((uint64_t)(g))<<48)|(((uint64_t)(f))<<40)|(((uint64_t)(e))<<32)|(((uint64_t)(d))<<24)|(((uint64_t)(c))<<16)|(((uint64_t)(b))<<8)|(a))
#define ERROR_DISPLAY_TAB_WIDTH 4
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
#define HIGHLIGHT_COLOR "\x1b[31m"
#define HIGHLIGHT_COLOR_RESET "\x1b[0m"
#define READ_REF_FROM_STACK(o) ((lll_object_t*)(_bf+(*((uint32_t*)LLL_GET_OBJECT_WITH_OFFSET((o),sizeof(lll_object_t))))))
#define READ_SINGLE_CHAR_OK 0
#define READ_SINGLE_CHAR_END 1
#define READ_SINGLE_CHAR_ERROR 2



extern uint8_t* _bf;
extern uint32_t _bf_off;
extern uint32_t _bf_sz;



int _input_data_stream_file_read(lll_input_data_stream_t* is);



void _input_data_stream_file_restart_line(lll_input_data_stream_t* is,uint32_t lp);



uint8_t _output_data_stream_file_write_char(lll_output_data_stream_t* os,char c);



uint8_t _output_data_stream_file_write_string(lll_output_data_stream_t* os,char* s);



uint8_t _output_data_stream_file_write(lll_output_data_stream_t* os,uint8_t* bf,size_t sz);



uint32_t _print_object_internal(lll_object_t* o,FILE* f);



uint8_t _read_single_char(lll_input_data_stream_t* is,char t,uint32_t st,lll_error_t* e);



uint8_t _read_object_internal(lll_compilation_data_t* c_dt,int c,lll_error_t* e);



uint32_t _get_object_size(lll_object_t* o);



uint32_t _optimize_object_internal(lll_object_t* o,lll_error_t* e);



uint32_t _remove_debug_data_internal(lll_object_t* o);



uint32_t _remove_padding_internal(lll_object_t* o,uint32_t* rm);



#endif
