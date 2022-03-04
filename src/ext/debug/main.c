#include <sll.h>



#ifdef _MSC_VER
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif



EXPORT sll_bool_t __sll_load(sll_version_t v){
	if (v!=SLL_VERSION){
		SLL_LOG("Version mismatch!");
		return 0;
	}
	SLL_LOG("Extension loaded!");
	return 1;
}



EXPORT void __sll_unload(void){
	SLL_LOG("Extension unloaded!");
}
