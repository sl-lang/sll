#include <windows.h>
#include <sll.h>



#define LIBRARY_NAME "sll-"SLL_VERSION_STRING".dll"
#define STRLEN(x) (sizeof(x)/sizeof(char)-1)



int main(int argc,const char** argv){
	char bf[256+STRLEN(LIBRARY_NAME)];
	DWORD bfl=GetModuleFileNameA(NULL,bf,256);
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
	void* cli=GetProcAddress(lh,"sll_cli_main");
	if (!cli){
		FreeLibrary(lh);
		return 0;
	}
	sll_return_code_t o=((sll_return_code_t(*)(sll_array_length_t,const sll_char_t*const*))cli)(argc,argv);
	FreeLibrary(lh);
	return o;
}
