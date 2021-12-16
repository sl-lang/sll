#include <sll.h>
#include <sll_ext/common.h>



__SLL_EXT_EXTERNAL void __sll_initialize(void){
	sll_file_write_string(sll_stdout,SLL_CHAR("Extension library loaded!\n"));
}



__SLL_EXT_EXTERNAL void __sll_deinitialize(void){
	sll_file_write_string(sll_stdout,SLL_CHAR("Extension library unloaded!\n"));
}
