#include <sll/common.h>
#include <sll/constants.h>
#include <sll/gc.h>
#include <sll/types.h>
#include <stdlib.h>



__SLL_FUNC __SLL_RETURN_HANDLE_TYPE sll_create_handle(sll_handle_list_t* hl,const sll_char_t* nm,sll_handle_destructor_t df){
	hl->dtl++;
	hl->dt=realloc(hl->dt,hl->dtl*sizeof(sll_handle_descriptor_t*));
	sll_handle_descriptor_t* hd=malloc(sizeof(sll_handle_descriptor_t));
	hd->nml=0;
	hd->c=0;
	while (*(nm+hd->nml)){
		hd->nm[hd->nml]=*(nm+hd->nml);
		hd->c^=hd->nm[hd->nml];
		hd->nml++;
	}
	hd->df=df;
	*(hl->dt+hl->dtl-1)=hd;
	return hl->dtl;
}



__SLL_FUNC void sll_free_handle_list(sll_handle_list_t* hl){
	for (sll_handle_type_t i=0;i<hl->dtl;i++){
		(*(hl->dt+i))->df(SLL_HANDLE_FREE);
		free(*(hl->dt+i));
	}
	free(hl->dt);
	hl->dt=NULL;
	hl->dtl=0;
}



__SLL_FUNC void sll_init_handle_list(sll_handle_list_t* o){
	o->dt=NULL;
	o->dtl=0;
}



__SLL_FUNC sll_runtime_object_t* sll_handle_to_object(sll_handle_type_t t,sll_handle_t h){
	sll_runtime_object_t* o=sll_create_object();
	o->t=SLL_RUNTIME_OBJECT_TYPE_HANDLE;
	o->dt.h.t=t;
	o->dt.h.h=h;
	return o;
}
