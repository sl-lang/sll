#ifndef __SLL__INTERNAL_STATIC_STRING_H__
#define __SLL__INTERNAL_STATIC_STRING_H__ 1
#include <sll/types.h>



#ifdef __SLL_BUILD_WINDOWS
#pragma section(".initstr$a",read)
#pragma section(".initstr$b",read)
#pragma section(".initstr$z",read)
#define __STATIC_STRING_DATA_SECTION __declspec(allocate(".initstr$b"))
#define INIT_STRING_SETUP static const __declspec(allocate(".initstr$a")) init_string_t* __init_string_start=0;static const __declspec(allocate(".initstr$z")) init_string_t* __init_string_end=0
#else
#ifdef __SLL_BUILD_DARWIN
#define __STATIC_STRING_DATA_SECTION __attribute__((used,__section__("__DATA,initstr")))
#define INIT_STRING_SETUP extern unsigned long long int __start_initstr __asm("section$start$__DATA$initstr");extern unsigned long long int __stop_initstr __asm("section$end$__DATA$initstr")
#else
#define __STATIC_STRING_DATA_SECTION __attribute__((used,section("initstr")))
#define INIT_STRING_SETUP extern const init_string_t* __start_initstr;extern const init_string_t* __stop_initstr
#endif
#define __init_string_start __start_initstr
#define __init_string_end __stop_initstr
#endif

#define _UNIQUE_NAME_JOIN2(a,b) a##_##b
#define _UNIQUE_NAME_JOIN(a,b) _UNIQUE_NAME_JOIN2(a,b)
#define _UNIQUE_NAME(a) _UNIQUE_NAME_JOIN(a,__LINE__)

#define __STATIC_STRING(nm,dt) sll_string_t nm=SLL_INIT_STRING_STRUCT;static const init_string_t _UNIQUE_NAME(__init_string)={&(nm),{.s=SLL_CHAR(dt)},sizeof(dt)/sizeof(char)-1};static const __STATIC_STRING_DATA_SECTION init_string_t* _UNIQUE_NAME(__init_string_ptr)=&_UNIQUE_NAME(__init_string)
#define __STATIC_STRING_CODE(nm,c) sll_string_t nm=SLL_INIT_STRING_STRUCT;static void _UNIQUE_NAME(__init_string_fn)(sll_string_t* out){c};static const init_string_t _UNIQUE_NAME(__init_string)={&(nm),{.fn=_UNIQUE_NAME(__init_string_fn)},SLL_MAX_STRING_LENGTH};static const __STATIC_STRING_DATA_SECTION init_string_t* _UNIQUE_NAME(__init_string_ptr)=&_UNIQUE_NAME(__init_string)



typedef union _STATIC_STRING_SOURCE{
	const sll_char_t* s;
	void (*fn)(sll_string_t*);
} static_string_source_t;



typedef struct _INIT_STRING{
	sll_string_t* p;
	static_string_source_t dt;
	sll_string_length_t dtl;
} init_string_t;



#endif
