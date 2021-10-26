#include <sll/_sll_internal.h>
#include <sll/api/json.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/handle.h>
#include <sll/init.h>
#include <sll/platform.h>
#include <sll/types.h>
#include <sll/url.h>
#include <stdlib.h>


__SLL_FUNC void sll_deinit(void){
	_execute_cleanup();
	sll_platform_reset_console();
}



__SLL_FUNC void sll_deinit_array(sll_array_t* a){
	free(a->v);
	a->v=NULL;
	a->l=0;
}



__SLL_FUNC void sll_deinit_assembly_data(sll_assembly_data_t* a_dt){
	a_dt->tm=0;
	a_dt->h=NULL;
	a_dt->ic=0;
	a_dt->vc=0;
	sll_deinit_assembly_function_table(&(a_dt->ft));
	sll_deinit_string_table(&(a_dt->st));
	void* pg=a_dt->_s.s;
	sll_page_size_t sz=sll_platform_get_page_size()*ASSEMBLY_INSTRUCTION_STACK_PAGE_ALLOC_COUNT;
	while (pg){
		void* n=*((void**)pg);
		sll_platform_free_page(pg,sz);
		pg=n;
	}
	a_dt->_s.s=NULL;
	a_dt->_s.e=NULL;
	a_dt->_s.c=0;
	a_dt->_s.p=NULL;
}



__SLL_FUNC void sll_deinit_assembly_function_table(sll_assembly_function_table_t* ft){
	free(ft->dt);
	ft->dt=NULL;
	ft->l=0;
}



__SLL_FUNC void sll_deinit_compilation_data(sll_compilation_data_t* c_dt){
	c_dt->is=NULL;
	c_dt->tm=0;
	c_dt->h=NULL;
	sll_deinit_identifier_table(&(c_dt->idt));
	sll_deinit_export_table(&(c_dt->et));
	sll_deinit_function_table(&(c_dt->ft));
	sll_deinit_string_table(&(c_dt->st));
	void* pg=c_dt->_s.s;
	sll_page_size_t sz=sll_platform_get_page_size()*OBJECT_STACK_PAGE_ALLOC_COUNT;
	while (pg){
		void* n=*((void**)pg);
		sll_platform_free_page(pg,sz);
		pg=n;
	}
	c_dt->_s.s=NULL;
	c_dt->_s.e=NULL;
	c_dt->_s.c=0;
	c_dt->_s.p=NULL;
	c_dt->_s.off=0;
	c_dt->_n_sc_id=1;
}



__SLL_FUNC void sll_deinit_export_table(sll_export_table_t* et){
	free(et->dt);
	et->dt=NULL;
	et->l=0;
}



__SLL_FUNC void sll_deinit_function_table(sll_function_table_t* ft){
	for (sll_function_index_t i=0;i<ft->l;i++){
		free(*(ft->dt+i));
	}
	free(ft->dt);
	ft->dt=NULL;
	ft->l=0;
}



__SLL_FUNC void sll_deinit_handle_list(sll_handle_list_t* hl){
	for (sll_handle_type_t i=0;i<hl->dtl;i++){
		if ((*(hl->dt+i))->df){
			(*(hl->dt+i))->df(SLL_HANDLE_FREE);
		}
	}
	free(hl->dt);
	hl->dt=NULL;
	hl->dtl=0;
}



__SLL_FUNC void sll_deinit_header_list(sll_header_list_t* hl){
	for (sll_header_count_t i=0;i<hl->l;i++){
		sll_header_t* kv=*(hl->dt+i);
		free(kv->k.v);
		free(kv->v.v);
		free(*(hl->dt+i));
	}
	free(hl->dt);
	hl->dt=NULL;
	hl->l=0;
}



__SLL_FUNC void sll_deinit_http_response(sll_http_response_t* r){
	if (r->rc){
		free(r->rc->v);
		SLL_INIT_STRING(r->rc);
	}
	if (r->hl){
		sll_deinit_header_list(r->hl);
		SLL_INIT_HEADER_LIST(r->hl);
	}
	if (r->dt){
		free(r->dt->v);
		SLL_INIT_STRING(r->dt);
	}
}



__SLL_FUNC void sll_deinit_identifier_table(sll_identifier_table_t* idt){
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		sll_identifier_list_t* e=idt->s+i;
		free(e->dt);
		e->dt=NULL;
		e->l=0;
	}
	free(idt->il);
	idt->il=NULL;
	idt->ill=0;
}



__SLL_FUNC void sll_deinit_integer_heap_queue(sll_integer_heap_queue_t* il){
	free(il->v);
	il->v=NULL;
	il->l=0;
}



__SLL_FUNC void sll_deinit_internal_function_table(sll_internal_function_table_t* ift){
	for (sll_function_index_t i=0;i<ift->l;i++){
		free(*(ift->dt+i));
	}
	free(ift->dt);
	ift->dt=NULL;
	ift->l=0;
}



__SLL_FUNC void sll_deinit_json_object(sll_json_object_t* json){
	if (json->t==SLL_JSON_OBJECT_TYPE_STRING){
		free(json->dt.s.v);
	}
	else if (json->t==SLL_JSON_OBJECT_TYPE_ARRAY){
		for (sll_json_array_length_t i=0;i<json->dt.a.l;i++){
			sll_deinit_json_object(json->dt.a.dt+i);
		}
		free(json->dt.a.dt);
	}
	else if (json->t==SLL_JSON_OBJECT_TYPE_MAP){
		for (sll_json_map_length_t i=0;i<json->dt.m.l;i++){
			sll_json_map_keypair_t* e=json->dt.m.dt+i;
			free(e->k.v);
			sll_deinit_json_object(&(e->v));
		}
		free(json->dt.m.dt);
	}
}



__SLL_FUNC void sll_deinit_string_table(sll_string_table_t* st){
	for (sll_string_index_t i=0;i<st->l;i++){
		free((st->dt+i)->v);
	}
	free(st->dt);
	st->dt=NULL;
	st->l=0;
}
