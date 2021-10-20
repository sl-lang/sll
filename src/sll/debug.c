#include <sll/api/string.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/object.h>
#include <sll/runtime_object.h>
#include <sll/stream.h>
#include <sll/types.h>
#include <stdio.h>
#include <stdlib.h>



__SLL_FUNC void sll_debug_print_assembly(const sll_assembly_data_t* a_dt){
	sll_output_data_stream_t os;
	sll_stream_create_output_from_file(stdout,&os);
	sll_print_assembly(a_dt,&os);
	putchar('\n');
}



__SLL_FUNC void sll_debug_print_object(const sll_object_t* o,const sll_compilation_data_t* c_dt){
	sll_output_data_stream_t os;
	sll_stream_create_output_from_file(stdout,&os);
	sll_print_object(c_dt,NULL,o,&os);
	putchar('\n');
}



__SLL_FUNC void sll_debug_print_runtime_object(const sll_runtime_object_t* v){
	if (v->t==SLL_RUNTIME_OBJECT_TYPE_STRING){
		fwrite(v->dt.s.v,sizeof(sll_char_t),v->dt.s.l,stdout);
	}
	else{
		sll_string_t str;
		sll_object_to_string((const sll_runtime_object_t*const*)&v,1,&str);
		fwrite(str.v,sizeof(sll_char_t),str.l,stdout);
		free(str.v);
	}
}
