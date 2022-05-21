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
	sll_file_write_format(sll_stdout,SLL_CHAR("!!! {%s} "),NULL,nm->data);
	sll_object_t* tmp=sll_array_to_object(args);
	sll_string_t str;
	sll_api_string_convert(&tmp,1,&str);
	sll_release_object(tmp);
	sll_file_write(sll_stderr,str.data,str.length*sizeof(sll_char_t),NULL);
	sll_free_string(&str);
	sll_file_write_char(sll_stderr,'\n',NULL);
}



EXPORT_SYMBOL void SLL_ABI_AUDIT_INIT(void){
	SLL_LOG("Load Audit library...");
}
