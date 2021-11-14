#include <sll/api/string.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/init.h>
#include <sll/object.h>
#include <sll/runtime_object.h>
#include <sll/types.h>
#include <stdio.h>



__SLL_EXTERNAL void sll_debug_print_assembly(const sll_assembly_data_t* a_dt){
	sll_print_assembly(a_dt,sll_stdout);
	putchar('\n');
}



__SLL_EXTERNAL void sll_debug_print_object(const sll_object_t* o,const sll_compilation_data_t* c_dt){
	sll_print_object(c_dt,NULL,o,sll_stdout);
	putchar('\n');
}



__SLL_EXTERNAL void sll_debug_print_runtime_object(const sll_runtime_object_t* v){
	if (v->t==SLL_RUNTIME_OBJECT_TYPE_STRING){
		sll_file_write(sll_stdout,v->dt.s.v,v->dt.s.l*sizeof(sll_char_t));
	}
	else{
		sll_string_t str;
		sll_object_to_string((const sll_runtime_object_t*const*)&v,1,&str);
		sll_file_write(sll_stdout,str.v,str.l*sizeof(sll_char_t));
		sll_deinit_string(&str);
	}
	putchar('\n');
}
