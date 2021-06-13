#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif
#include <lll_lib.h>
#include <_lll_internal.h>
#include <stdint.h>
#include <stdio.h>



#ifdef _MSC_VER
extern void _CRT_INIT(void);
#endif



__LLL_IMPORT_EXPORT void __lll_api_init(void){
#ifdef _MSC_VER
	_CRT_INIT();
#endif
}



__LLL_IMPORT_EXPORT void __lll_api_putchar(int c){
	putchar(c);
}



__LLL_IMPORT_EXPORT void __lll_api_print_string(char* s){
	do{
		putchar(*s);
		s++;
	} while (*s);
}



__LLL_IMPORT_EXPORT void __lll_api_print_int32(int32_t v){
	if (!v){
		putchar('0');
		return;
	}
	if (v<0){
		v=-v;
		putchar('-');
	}
	char bf[10];
	uint8_t i=0;
	while (v){
		bf[i]=v%10;
		v/=10;
		i++;
	};
	while (i){
		i--;
		putchar(bf[i]+48);
	}
}



__LLL_IMPORT_EXPORT void __lll_api_deinit(void){
	fflush(stdout);
	fflush(stderr);
#ifdef _MSC_VER
	ExitProcess(0);
#else
	__asm__ volatile("mov $60,%rax\nxor %edi,%edi\nsyscall\n");
#endif
	UNREACHABLE();
}
