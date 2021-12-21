#include <sll/api/string.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/node.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_debug_print_assembly(const sll_assembly_data_t* a_dt){
	sll_print_assembly(a_dt,sll_stdout);
	sll_file_write_char(sll_stdout,'\n');
}



__SLL_EXTERNAL void sll_debug_print_node(const sll_node_t* o,const sll_compilation_data_t* c_dt){
	sll_print_node(c_dt,NULL,o,sll_stdout);
	sll_file_write_char(sll_stdout,'\n');
}



__SLL_EXTERNAL void sll_debug_print_object(const sll_object_t* v){
	if (v->t==SLL_OBJECT_TYPE_STRING){
		sll_file_write(sll_stdout,v->dt.s.v,v->dt.s.l*sizeof(sll_char_t));
	}
	else{
		sll_string_t str;
		sll_api_string_convert((sll_object_t*const*)&v,1,&str);
		sll_file_write(sll_stdout,str.v,str.l*sizeof(sll_char_t));
		sll_free_string(&str);
	}
	sll_file_write_char(sll_stdout,'\n');
}
