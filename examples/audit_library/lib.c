/*
Sll Audit Library Example
@krzem5, 2022-04-05
*/



#include <sll.h>
#include <stdio.h>



#ifdef _MSC_VER
#define EXPORT_SYMBOL __declspec(dllexport)
#else
#define EXPORT_SYMBOL __attribute__((visibility("default")))
#endif



EXPORT_SYMBOL void SLL_ABI_AUDIT_CALL(const sll_string_t* nm,const sll_array_t* args){
	sll_file_write_format(sll_stdout,SLL_CHAR("Event: %s\n"),NULL,nm->v);
}



EXPORT_SYMBOL void SLL_ABI_AUDIT_INIT(void){
	SLL_LOG("Load Audit library...");
}
