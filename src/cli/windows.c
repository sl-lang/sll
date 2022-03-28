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
	sll_return_code_t o=-1;
	HANDLE lib_fh=CreateFileA(bf,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	HMODULE lh=NULL;
	if (lib_fh==INVALID_HANDLE_VALUE){
		return -1;
	}
	LARGE_INTEGER lib_sz;
	if (!GetFileSizeEx(lib_fh,&lib_sz)){
		goto _cleanup;
	}
	LockFile(lib_fh,0,0,lib_sz.QuadPart&0xffffffff,lib_sz.QuadPart>>32);
	lh=LoadLibraryExA(bf,NULL,0);
	if (!lh){
		goto _cleanup;
	}
	UnlockFile(lib_fh,0,0,lib_sz.QuadPart&0xffffffff,lib_sz.QuadPart>>32);
	CloseHandle(lib_fh);
	lib_fh=INVALID_HANDLE_VALUE;
	sll_version_t (*ver)(void)=(void*)GetProcAddress(lh,"sll_version");
	if (!ver||ver()!=SLL_VERSION){
		goto _cleanup;
	}
	sll_return_code_t (*cli)(sll_array_length_t argc,const sll_char_t*const* argv)=(void*)GetProcAddress(lh,"sll_cli_main");
	if (cli){
		o=cli(__argc-1,__argv+1);
	}
_cleanup:
	if (lib_fh!=INVALID_HANDLE_VALUE){
		CloseHandle(lib_fh);
	}
	if (lh){
		FreeLibrary(lh);
	}
	return o;
}



int WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int d){
	return main();
}
