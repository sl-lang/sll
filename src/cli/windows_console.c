#include <windows.h>
#include <windows_shared.h>
#include <sll.h>



int main(int argc,const char*const* argv){
	HMODULE lh=_load_library();
	sll_return_code_t o=0;
	if (lh){
		sll_return_code_t (*cli)(sll_array_length_t argc,const sll_char_t*const* argv)=(void*)GetProcAddress(lh,"sll_cli_main");
		if (cli){
			o=cli(argc-1,argv+1);
		}
	}
	FreeLibrary(lh);
	return o;
}
