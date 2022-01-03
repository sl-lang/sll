#include <windows.h>
#include <sll.h>



#define LIBRARY_NAME "sll-"SLL_VERSION_STRING".dll"
#define STRLEN(x) (sizeof(x)/sizeof(char)-1)



int main(int argc,const char** argv){
	char bf[MAX_PATH+STRLEN(LIBRARY_NAME)];
	DWORD bfl=GetModuleFileNameA(NULL,bf,MAX_PATH);
	while (bfl&&bf[bfl]!='\\'&&bf[bfl]!='/'){
		bfl--;
	}
	if (!bfl){
		return -1;
	}
	memcpy(bf+bfl+1,LIBRARY_NAME,STRLEN(LIBRARY_NAME)+1);
	HMODULE lh=LoadLibraryExA(bf,NULL,0);
	if (!lh){
		return -1;
	}
	void* cli=GetProcAddress(lh,"sll_cli_main");
	if (!cli){
		FreeLibrary(lh);
		return -1;
	}
	sll_return_code_t o=((sll_return_code_t(*)(sll_array_length_t,const sll_char_t*const*))cli)(argc,argv);
	FreeLibrary(lh);
	return o;
}
