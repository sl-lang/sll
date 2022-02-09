#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_unify_compilation_data(const sll_compilation_data_t* c_dt,sll_source_file_t* o){
	o->dt=NULL;
	for (sll_source_file_index_t i=0;i<c_dt->l;i++){
		sll_source_file_t* sf=*(c_dt->dt+i);
		if (!sf->dt){
			continue;
		}
		if (!o->dt){
			SLL_UNIMPLEMENTED();
		}
		else{
			SLL_UNIMPLEMENTED();
		}
	}
}
