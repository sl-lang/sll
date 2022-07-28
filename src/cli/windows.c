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
	char buffer[MAX_PATH+STRLEN(LIBRARY_NAME)];
	DWORD buffer_length=GetModuleFileNameA(NULL,buffer,MAX_PATH);
	while (buffer_length&&buffer[buffer_length]!='\\'&&buffer[buffer_length]!='/'){
		buffer_length--;
	}
	if (!buffer_length){
		buffer[0]='/';
		buffer_length=1;
	}
	buffer_length++;
	for (unsigned int i=0;i<STRLEN(LIBRARY_NAME);i++){
		buffer[buffer_length+i]=LIBRARY_NAME[i];
	}
	buffer[buffer_length+STRLEN(LIBRARY_NAME)]=0;
	HMODULE library_handle=LoadLibraryExA(buffer,NULL,0);
	if (!library_handle){
		return 0;
	}
	sll_version_t (*ver)(void)=(void*)GetProcAddress(library_handle,"sll_version");
	if (!ver||ver()!=SLL_VERSION){
		FreeLibrary(library_handle);
		return 0;
	}
	sll_return_code_t (*cli)(sll_array_length_t argc,const sll_char_t*const* argv)=(void*)GetProcAddress(library_handle,"sll_cli_main");
	sll_return_code_t out=(cli?cli(__argc-1,__argv+1):0);
	FreeLibrary(library_handle);
	return out;
}



int WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int d){
	return main();
}
