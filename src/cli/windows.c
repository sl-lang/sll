#include <windows.h>
#include <windows_shared.h>
#include <sll.h>



int WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int d){
	HMODULE lh=_load_library();
	sll_return_code_t o=0;
	if (lh){
		void* cli=GetProcAddress(lh,"sll_cli_main_raw");
		if (cli){
			o=((sll_return_code_t(*)(const sll_char_t*))cli)(GetCommandLineA());
		}
	}
	FreeLibrary(lh);
	return o;
}
