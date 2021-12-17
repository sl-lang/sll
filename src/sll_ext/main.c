#include <sll_ext/common.h>
#include <sll_ext/version.h>
#include <sll.h>



__SLL_EXT_EXTERNAL sll_bool_t __sll_load(sll_version_t v){
	if (v!=SLL_EXT_VERSION){
		sll_file_write_string(sll_stdout,SLL_CHAR("<ext-lib> Invalid version!\n"));
		return 0;
	}
	sll_file_write_string(sll_stdout,SLL_CHAR("<ext-lib> Loaded extension library for v"SLL_EXT_VERSION_STRING"!\n"));
	return 1;
}



__SLL_EXT_EXTERNAL void __sll_unload(void){
	sll_file_write_string(sll_stdout,SLL_CHAR("<ext-lib> Successfully unloaded extension library\n"));
}
