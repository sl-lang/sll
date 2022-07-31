#include <sll/api/string.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/node.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_debug_print_assembly(const sll_assembly_data_t* assembly_data){
	sll_print_assembly(assembly_data,sll_stdout);
	sll_file_write_char(sll_stdout,'\n',NULL);
}



__SLL_EXTERNAL void sll_debug_print_node(sll_node_t node,const sll_source_file_t* source_file){
	sll_print_node(source_file,NULL,node,sll_stdout);
	sll_file_write_char(sll_stdout,'\n',NULL);
}



__SLL_EXTERNAL void sll_debug_print_object(sll_object_t object){
	if (object->type==SLL_OBJECT_TYPE_STRING){
		sll_file_write(sll_stdout,object->data.string.data,object->data.string.length*sizeof(sll_char_t),NULL);
	}
	else{
		sll_string_t str;
		sll_api_string_convert(&object,1,&str);
		sll_file_write(sll_stdout,str.data,str.length*sizeof(sll_char_t),NULL);
		sll_free_string(&str);
	}
	sll_file_write_char(sll_stdout,'\n',NULL);
}
