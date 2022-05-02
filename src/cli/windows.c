#include <windows.h>
#include <sll.h>
#include <stdlib.h>
#include <stdio.h>



#define LIBRARY_NAME "sll-"SLL_VERSION_STRING".dll"
#define STRLEN(x) (sizeof(x)/sizeof(char)-1)



int main(void){
	if (!__argc){
		return 0;
	}
	char bf[PATH_MAX+STRLEN(LIBRARY_NAME)];
	DWORD bfl=GetModuleFileNameA(NULL,bf,PATH_MAX);
	while (bfl&&bf[bfl]!='\\'&&bf[bfl]!='/'){
		bfl--;
	}
	if (!bfl){
		bf[0]='/';
		bfl=1;
	}
	bfl++;
	for (unsigned int i=0;i<STRLEN(LIBRARY_NAME);i++){
		bf[bfl+i]=LIBRARY_NAME[i];
	}
	bf[bfl+STRLEN(LIBRARY_NAME)]=0;
	HMODULE lh=LoadLibraryExA(bf,NULL,0);
	if (!lh){
		return 0;
	}
	sll_version_t (*ver)(void)=(void*)GetProcAddress(lh,"sll_version");
	if (!ver||ver()!=SLL_VERSION){
		FreeLibrary(lh);
		return 0;
	}
	sll_return_code_t (*cli)(sll_array_length_t argc,const sll_char_t*const* argv)=(void*)GetProcAddress(lh,"sll_cli_main");
	sll_return_code_t o=(cli?cli(__argc-1,__argv+1):0);
	FreeLibrary(lh);
	return o;
}



int WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int d){
	return main();
}
