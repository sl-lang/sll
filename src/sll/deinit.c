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
	for (sll_array_length_t i=0;i<a->l;i++){
		sll_release_object(a->v[i]);
	}
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
	sll_deinit_assembly_stack_data(&(a_dt->_s));
}



__SLL_FUNC void sll_deinit_assembly_function_table(sll_assembly_function_table_t* ft){
	free(ft->dt);
	ft->dt=NULL;
	ft->l=0;
}



__SLL_FUNC void sll_deinit_assembly_instruction(sll_assembly_instruction_t* ai){
	SLL_INIT_ASSEMBLY_INSTRUCTION(ai);
}



__SLL_FUNC void sll_deinit_assembly_instruction_data(sll_assembly_instruction_data_t* ai_dt){
	SLL_INIT_ASSEMBLY_INSTRUCTION_DATA(ai_dt);
}



__SLL_FUNC void sll_deinit_assembly_stack_data(sll_assembly_stack_data_t* a_st){
	void* pg=a_st->s;
	sll_page_size_t sz=sll_platform_get_page_size()*ASSEMBLY_INSTRUCTION_STACK_PAGE_ALLOC_COUNT;
	while (pg){
		void* n=*((void**)pg);
		sll_platform_free_page(pg,sz);
		pg=n;
	}
	a_st->s=NULL;
	a_st->e=NULL;
	a_st->c=0;
	a_st->p=NULL;
}



__SLL_FUNC void sll_deinit_compilation_data(sll_compilation_data_t* c_dt){
	c_dt->is=NULL;
	c_dt->tm=0;
	c_dt->h=NULL;
	sll_deinit_identifier_table(&(c_dt->idt));
	sll_deinit_export_table(&(c_dt->et));
	sll_deinit_function_table(&(c_dt->ft));
	sll_deinit_string_table(&(c_dt->st));
	sll_deinit_compilation_stack_data(&(c_dt->_s));
	c_dt->_n_sc_id=1;
}



__SLL_FUNC void sll_deinit_compilation_stack_data(sll_compilation_stack_data_t* c_st){
	void* pg=c_st->s;
	sll_page_size_t sz=sll_platform_get_page_size()*OBJECT_STACK_PAGE_ALLOC_COUNT;
	while (pg){
		void* n=*((void**)pg);
		sll_platform_free_page(pg,sz);
		pg=n;
	}
	c_st->s=NULL;
	c_st->e=NULL;
	c_st->c=0;
	c_st->p=NULL;
	c_st->off=0;
}



__SLL_FUNC void sll_deinit_debug_object_data(sll_debug_object_data_t* d_dt){
	SLL_INIT_DEBUG_OBJECT_DATA(d_dt);
}



__SLL_FUNC void sll_deinit_error_data_range(sll_error_data_range_t* e_dt_r){
	SLL_INIT_ERROR_DATA_RANGE(e_dt_r);
}



__SLL_FUNC void sll_deinit_error_data(sll_error_data_t* e_dt){
	SLL_INIT_ERROR_DATA(e_dt);
}



__SLL_FUNC void sll_deinit_error(sll_error_t* e){
	SLL_INIT_ERROR(e);
}



__SLL_FUNC void sll_deinit_export_table(sll_export_table_t* et){
	free(et->dt);
	et->dt=NULL;
	et->l=0;
}



__SLL_FUNC void sll_deinit_function_object_data(sll_function_object_data_t* f_dt){
	SLL_INIT_FUNCTION_OBJECT_DATA(f_dt);
}



__SLL_FUNC void sll_deinit_function(sll_function_t* f){
	SLL_INIT_FUNCTION(f);
}



__SLL_FUNC void sll_deinit_function_table(sll_function_table_t* ft){
	for (sll_function_index_t i=0;i<ft->l;i++){
		free(*(ft->dt+i));
	}
	free(ft->dt);
	ft->dt=NULL;
	ft->l=0;
}



__SLL_FUNC void sll_deinit_handle_data(sll_handle_data_t* h_dt){
	SLL_INIT_HANDLE_DATA(h_dt);
}



__SLL_FUNC void sll_deinit_handle_descriptor(sll_handle_descriptor_t* hd){
	SLL_INIT_HANDLE_DESCRIPTOR(hd);
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



__SLL_FUNC void sll_deinit_header(sll_header_t* h){
	SLL_INIT_HEADER(h);
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



__SLL_FUNC void sll_deinit_identifier(sll_identifier_t* i){
	SLL_INIT_IDENTIFIER(i);
}



__SLL_FUNC void sll_deinit_identifier_list(sll_identifier_list_t* il){
	SLL_INIT_IDENTIFIER_LIST(il);
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



__SLL_FUNC void sll_deinit_input_buffer(sll_input_buffer_t* i_bf){
	SLL_INIT_INPUT_BUFFER(i_bf);
}



__SLL_FUNC void sll_deinit_input_data_stream(sll_input_data_stream_t* is){
	SLL_INIT_INPUT_DATA_STREAM(is);
}



__SLL_FUNC void sll_deinit_binary_heap(sll_binary_heap_t* il){
	free(il->v);
	il->v=NULL;
	il->l=0;
}



__SLL_FUNC void sll_deinit_internal_function(sll_internal_function_t* i_f){
	SLL_INIT_INTERNAL_FUNCTION(i_f);
}



__SLL_FUNC void sll_deinit_internal_function_table(sll_internal_function_table_t* ift){
	for (sll_function_index_t i=0;i<ift->l;i++){
		free(*(ift->dt+i));
	}
	free(ift->dt);
	ift->dt=NULL;
	ift->l=0;
}



__SLL_FUNC void sll_deinit_json_array(sll_json_array_t* json_a){
	for (sll_json_array_length_t i=0;i<json_a->l;i++){
		sll_deinit_json_object(json_a->dt+i);
	}
	free(json_a->dt);
}



__SLL_FUNC void sll_deinit_json_map_keypair(sll_json_map_keypair_t* json_kp){
	sll_deinit_string(&(json_kp->k));
	sll_deinit_json_object(&(json_kp->v));
}



__SLL_FUNC void sll_deinit_json_map(sll_json_map_t* json_m){
	for (sll_json_map_length_t i=0;i<json_m->l;i++){
		sll_json_map_keypair_t* e=json_m->dt+i;
		sll_deinit_string(&(e->k));
		sll_deinit_json_object(&(e->v));
	}
	free(json_m->dt);
}



__SLL_FUNC void sll_deinit_json_object_data(sll_json_object_data_t* json_dt){
	SLL_INIT_JSON_OBJECT_DATA(json_dt);
}



__SLL_FUNC void sll_deinit_json_object(sll_json_object_t* json){
	if (json->t==SLL_JSON_OBJECT_TYPE_STRING){
		sll_deinit_string(&(json->dt.s));
	}
	else if (json->t==SLL_JSON_OBJECT_TYPE_ARRAY){
		sll_deinit_json_array(&(json->dt.a));
	}
	else if (json->t==SLL_JSON_OBJECT_TYPE_MAP){
		sll_deinit_json_map(&(json->dt.m));
	}
}



__SLL_FUNC void sll_deinit_loop_data(sll_loop_data_t* l_dt){
	SLL_INIT_LOOP_DATA(l_dt);
}



__SLL_FUNC void sll_deinit_map(sll_map_t* m){
	for (sll_map_length_t j=0;j<(m->l<<1);j++){
		sll_release_object(*(m->v+j));
	}
	free(m->v);
	m->v=NULL;
	m->l=0;
}



__SLL_FUNC void sll_deinit_object_data(sll_object_data_t* o_dt){
	SLL_INIT_OBJECT_DATA(o_dt);
}



__SLL_FUNC void sll_deinit_object(sll_object_t* o){
	SLL_INIT_OBJECT(o);
}



__SLL_FUNC void sll_deinit_output_data_stream(sll_output_data_stream_t* os){
	SLL_INIT_OUTPUT_DATA_STREAM(os);
}



__SLL_FUNC void sll_deinit_runtime_data(sll_runtime_data_t* r_dt){
	SLL_INIT_RUNTIME_DATA(r_dt);
}



__SLL_FUNC void sll_deinit_runtime_object_data(sll_runtime_object_data_t* ro_dt){
	SLL_INIT_RUNTIME_OBJECT_DATA(ro_dt);
}



__SLL_FUNC void sll_deinit_runtime_object(sll_runtime_object_t* o){
	sll_remove_debug_data(o);
	if (SLL_RUNTIME_OBJECT_GET_TYPE(o)==SLL_RUNTIME_OBJECT_TYPE_STRING){
		if (!(o->t&RUNTIME_OBJECT_EXTERNAL_STRING)){
			sll_deinit_string(&(o->dt.s));
		}
	}
	else if (SLL_RUNTIME_OBJECT_GET_TYPE(o)==SLL_RUNTIME_OBJECT_TYPE_ARRAY){
		sll_deinit_array(&(o->dt.a));
	}
	else if (SLL_RUNTIME_OBJECT_GET_TYPE(o)==SLL_RUNTIME_OBJECT_TYPE_HANDLE){
		if (sll_current_runtime_data){
			sll_handle_descriptor_t* hd=SLL_HANDLE_LOOKUP_DESCRIPTOR(sll_current_runtime_data->hl,o->dt.h.t);
			if (hd&&hd->df){
				hd->df(o->dt.h.h);
			}
		}
	}
	else if (SLL_RUNTIME_OBJECT_GET_TYPE(o)==SLL_RUNTIME_OBJECT_TYPE_MAP){
		sll_deinit_map(&(o->dt.m));
	}
	o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
	o->dt.i=0;
}



__SLL_FUNC void sll_deinit_stack_data(sll_stack_data_t* s_dt){
	SLL_INIT_STACK_DATA(s_dt);
}



__SLL_FUNC void sll_deinit_string(sll_string_t* s){
	s->l=0;
	s->c=0;
	free(s->v);
	s->v=NULL;
}



__SLL_FUNC void sll_deinit_string_table(sll_string_table_t* st){
	for (sll_string_index_t i=0;i<st->l;i++){
		free((st->dt+i)->v);
	}
	free(st->dt);
	st->dt=NULL;
	st->l=0;
}
