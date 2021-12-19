#include <sll_ext/_sll_ext_internal.h>
#include <sll_ext/common.h>
#include <sll_ext/version.h>
#include <sll.h>



__SLL_EXT_EXTERNAL sll_bool_t __sll_load(sll_version_t v){
	if (v!=SLL_EXT_VERSION){
		SLL_LOG("Version mismatch!");
		return 0;
	}
	_register_debug_functions();
	return 1;
}



__SLL_EXT_EXTERNAL void __sll_unload(void){
}
