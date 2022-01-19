#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/platform.h>
#include <sll/string_table.h>
#include <sll/types.h>



static const sll_node_t* _get_node_size(const sll_node_t* o,sll_node_offset_t* sz){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
		if (o->t==NODE_TYPE_CHANGE_STACK){
			o=o->dt._p;
		}
		else{
			(*sz)++;
			o++;
		}
	}
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			(*sz)++;
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t l=o->dt.al;
				(*sz)++;
				o++;
				while (l){
					l--;
					o=_get_node_size(o,sz);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				(*sz)++;
				o++;
				while (l){
					l--;
					o=_get_node_size(o,sz);
				}
				return o;
			}
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				(*sz)++;
				o++;
				while (l){
					l--;
					o=_get_node_size(o,sz);
				}
				return o;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			{
				sll_arg_count_t l=o->dt.l.ac;
				(*sz)++;
				o++;
				while (l){
					l--;
					o=_get_node_size(o,sz);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->dt.ac;
	(*sz)++;
	o++;
	while (l){
		l--;
		o=_get_node_size(o,sz);
	}
	return o;
}



static sll_node_t* _remove_debug_data(sll_node_t* o){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
		if (o->t==SLL_NODE_TYPE_DBG){
			o->t=SLL_NODE_TYPE_NOP;
		}
		o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t l=o->dt.al;
				o++;
				while (l){
					l--;
					o=_remove_debug_data(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				o++;
				while (l){
					l--;
					o=_remove_debug_data(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					o=_remove_debug_data(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			{
				sll_arg_count_t l=o->dt.l.ac;
				o++;
				while (l){
					l--;
					o=_remove_debug_data(o);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_remove_debug_data(o);
	}
	return o;
}



__SLL_EXTERNAL void sll_free_compilation_data(sll_compilation_data_t* c_dt){
	c_dt->tm=0;
	c_dt->h=NULL;
	for (unsigned int i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		sll_identifier_list_t* e=c_dt->idt.s+i;
		sll_deallocate(e->dt);
		e->dt=NULL;
		e->l=0;
	}
	sll_deallocate(c_dt->idt.il);
	c_dt->idt.il=NULL;
	c_dt->idt.ill=0;
	sll_deallocate(c_dt->et.dt);
	c_dt->et.dt=NULL;
	c_dt->et.l=0;
	for (sll_function_index_t i=0;i<c_dt->ft.l;i++){
		sll_deallocate(*(c_dt->ft.dt+i));
	}
	sll_deallocate(c_dt->ft.dt);
	c_dt->ft.dt=NULL;
	c_dt->ft.l=0;
	for (sll_string_index_t i=0;i<c_dt->st.l;i++){
		sll_free_string(c_dt->st.dt+i);
	}
	sll_deallocate(c_dt->st.dt);
	c_dt->st.dt=NULL;
	c_dt->st.l=0;
	void* pg=c_dt->_s.s;
	while (pg){
		void* n=*((void**)pg);
		sll_platform_free_page(pg,SLL_ROUND_PAGE(NODE_STACK_ALLOC_SIZE));
		pg=n;
	}
	sll_deallocate(c_dt->fpt.dt);
	c_dt->fpt.dt=NULL;
	c_dt->fpt.l=0;
	c_dt->_s.s=NULL;
	c_dt->_s.e=NULL;
	c_dt->_s.c=0;
	c_dt->_s.p=NULL;
	c_dt->_s.off=0;
	c_dt->_n_sc_id=1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_node_offset_t sll_get_node_size(const sll_node_t* o){
	sll_node_offset_t sz=0;
	_get_node_size(o,&sz);
	return sz;
}



__SLL_EXTERNAL void sll_init_compilation_data(const sll_char_t* fp,sll_compilation_data_t* o){
	o->tm=sll_platform_get_current_time();
	o->h=NULL;
	for (unsigned int i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		o->idt.s[i].dt=NULL;
		o->idt.s[i].l=0;
	}
	o->idt.il=NULL;
	o->idt.ill=0;
	o->et.dt=NULL;
	o->et.l=0;
	o->ft.dt=NULL;
	o->ft.l=0;
	o->st.dt=NULL;
	o->st.l=0;
	o->fpt.dt=sll_allocate(sizeof(sll_string_index_t));
	o->fpt.l=1;
	*(o->fpt.dt)=0;
	_init_node_stack(o);
	o->_n_sc_id=1;
	IGNORE_RESULT(sll_create_string(&(o->st),fp,sll_string_length_unaligned(fp)));
}



__SLL_EXTERNAL void sll_remove_debug_data(sll_compilation_data_t* c_dt){
	_remove_debug_data(c_dt->h);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_node_t* sll_skip_node(sll_node_t* o){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
		o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t l=o->dt.al;
				o++;
				while (l){
					l--;
					o=sll_skip_node(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				o++;
				while (l){
					l--;
					o=sll_skip_node(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					o=sll_skip_node(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			{
				sll_arg_count_t l=o->dt.l.ac;
				o++;
				while (l){
					l--;
					o=sll_skip_node(o);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=sll_skip_node(o);
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT const sll_node_t* sll_skip_node_const(const sll_node_t* o){
	return sll_skip_node((sll_node_t*)o);
}
