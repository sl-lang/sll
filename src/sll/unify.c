#include <sll/_internal/common.h>
#include <sll/_internal/stack.h>
#include <sll/_internal/unify.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/identifier.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>



static const sll_node_t* _clone_node(const sll_node_t* src,sll_source_file_t* o,source_file_mapping_data_t* sf_m_dt){
	while (src->t==SLL_NODE_TYPE_NOP||src->t==SLL_NODE_TYPE_DBG||src->t==SLL_NODE_TYPE_CHANGE_STACK){
		if (src->t==SLL_NODE_TYPE_CHANGE_STACK){
			src=src->dt._p;
		}
		else{
			sll_node_t* dst=_acquire_next_node(o);
			*dst=*src;
			if (sf_m_dt&&src->t==SLL_NODE_TYPE_DBG&&src->dt.s!=SLL_MAX_STRING_INDEX){
				dst->dt.s=*(sf_m_dt->sm+src->dt.s);
			}
			src++;
		}
	}
	sll_node_t* dst=_acquire_next_node(o);
	*dst=*src;
	switch (src->t){
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_COMPLEX:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return src+1;
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
			if (sf_m_dt){
				dst->dt.s=*(sf_m_dt->sm+dst->dt.s);
			}
			return src+1;
		case SLL_NODE_TYPE_IDENTIFIER:
			if (sf_m_dt){
				dst->dt.id=*(sf_m_dt->idx_off[SLL_IDENTIFIER_GET_ARRAY_ID(dst->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(dst->dt.id));
			}
			return src+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t l=src->dt.al;
				src++;
				while (l){
					l--;
					src=_clone_node(src,o,sf_m_dt);
				}
				return src;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=src->dt.ml;
				src++;
				while (l){
					l--;
					src=_clone_node(src,o,sf_m_dt);
				}
				return src;
			}
		case SLL_NODE_TYPE_FUNC:
			if (sf_m_dt){
				dst->dt.fn.id+=sf_m_dt->f_id_off;
				dst->dt.fn.sc+=sf_m_dt->sc_off;
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=src->dt.fn.ac;
				src++;
				while (l){
					l--;
					src=_clone_node(src,o,sf_m_dt);
				}
				return src;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			{
				if (sf_m_dt){
					dst->dt.l.sc+=sf_m_dt->sc_off;
				}
				sll_arg_count_t l=src->dt.l.ac;
				src++;
				while (l){
					l--;
					src=_clone_node(src,o,sf_m_dt);
				}
				return src;
			}
		case SLL_NODE_TYPE_DECL:
			{
				if (sf_m_dt&&dst->dt.dc.nm!=SLL_MAX_STRING_INDEX){
					dst->dt.dc.nm=*(sf_m_dt->sm+dst->dt.dc.nm);
				}
				sll_arg_count_t l=src->dt.dc.ac;
				src++;
				while (l){
					l--;
					src=_clone_node(src,o,sf_m_dt);
				}
				return src;
			}
	}
	sll_arg_count_t l=src->dt.ac;
	src++;
	while (l){
		l--;
		src=_clone_node(src,o,sf_m_dt);
	}
	return src;
}



__SLL_EXTERNAL void sll_unify_compilation_data(const sll_compilation_data_t* c_dt,sll_source_file_t* o){
	SLL_ASSERT(c_dt->l);
	_init_node_stack(o);
	o->dt=_acquire_next_node(o);
	o->dt->t=SLL_NODE_TYPE_OPERATION_LIST;
	o->dt->dt.ac=0;
	o->_n_sc_id=0;
	o->it.dt=NULL;
	o->it.l=0;
	o->fp_nm=0;
	sll_source_file_index_t idx=c_dt->l;
	sll_identifier_index_t** export_dt=sll_allocate_stack((idx-1)*sizeof(sll_identifier_index_t*));
	do{
		idx--;
		sll_source_file_t* sf=*(c_dt->dt+idx);
		if (!idx){
			o->tm=sf->tm;
			o->sz=sf->sz;
			o->h=sf->h;
			o->et.l=sf->et.l;
			o->et.dt=sll_allocate(sf->et.l*sizeof(sll_identifier_index_t));
			sll_copy_data(sf->et.dt,sf->et.l*sizeof(sll_identifier_index_t),o->et.dt);
		}
		if (!sf->dt){
			continue;
		}
		if (!o->dt->dt.ac){
			if (idx){
				sll_identifier_index_t* ex_dt=sll_allocate_stack(sf->et.l*sizeof(sll_identifier_index_t));
				*(export_dt+idx-1)=ex_dt;
				sll_copy_data(sf->et.dt,sf->et.l*sizeof(sll_identifier_index_t),ex_dt);
			}
			sll_node_t* dbg=_acquire_next_node(o);
			dbg->t=SLL_NODE_TYPE_DBG;
			dbg->dt.s=sf->fp_nm;
			for (unsigned int i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
				const sll_identifier_list_t* il=sf->idt.s+i;
				sll_identifier_list_t* oil=o->idt.s+i;
				oil->l=il->l;
				oil->dt=sll_allocate(il->l*sizeof(sll_identifier_t));
				sll_copy_data(il->dt,il->l*sizeof(sll_identifier_t),oil->dt);
			}
			o->idt.ill=sf->idt.ill;
			o->idt.il=sll_allocate(sf->idt.ill*sizeof(sll_identifier_t));
			sll_copy_data(sf->idt.il,sf->idt.ill*sizeof(sll_identifier_t),o->idt.il);
			o->ft.l=sf->ft.l;
			o->ft.dt=sll_allocate(sf->ft.l*sizeof(sll_function_t*));
			for (sll_function_index_t i=0;i<sf->ft.l;i++){
				const sll_function_t* s=*(sf->ft.dt+i);
				sll_function_t* d=sll_allocate(sizeof(sll_function_t)+s->al*sizeof(sll_identifier_index_t));
				*(o->ft.dt+i)=d;
				sll_copy_data(s,sizeof(sll_function_t)+s->al*sizeof(sll_identifier_index_t),d);
				d->off+=o->_s.off;
			}
			o->st.l=sf->st.l;
			o->st.dt=sll_allocate(sf->st.l*sizeof(sll_string_t));
			for (sll_string_index_t i=0;i<sf->st.l;i++){
				sll_string_clone(sf->st.dt+i,o->st.dt+i);
			}
			_clone_node(sf->dt,o,NULL);
		}
		else{
			source_file_mapping_data_t sf_m_dt={
				sll_allocate_stack(sf->st.l*sizeof(sll_string_index_t)),
				o->ft.l,
				o->_n_sc_id
			};
			for (sll_string_index_t i=0;i<sf->st.l;i++){
				sll_string_t tmp;
				sll_string_clone(sf->st.dt+i,&tmp);
				*(sf_m_dt.sm+i)=sll_add_string(&(o->st),&tmp,1);
			}
			sll_node_t* dbg=_acquire_next_node(o);
			dbg->t=SLL_NODE_TYPE_DBG;
			dbg->dt.s=*(sf_m_dt.sm+sf->fp_nm);
			if (!idx){
				o->fp_nm=dbg->dt.s;
			}
			for (unsigned int i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
				const sll_identifier_list_t* il=sf->idt.s+i;
				if (!il->l){
					sf_m_dt.idx_off[i]=NULL;
					continue;
				}
				sll_identifier_list_t* oil=o->idt.s+i;
				sll_identifier_list_length_t off=oil->l;
				sf_m_dt.idx_off[i]=sll_allocate_stack(il->l*sizeof(sll_identifier_index_t));
				oil->l+=il->l;
				oil->dt=sll_reallocate(oil->dt,oil->l*sizeof(sll_identifier_t));
				for (sll_identifier_list_length_t j=0;j<il->l;j++){
					*(sf_m_dt.idx_off[i]+j)=SLL_CREATE_IDENTIFIER(off+j,i);
					(oil->dt+off+j)->sc=(il->dt+j)->sc+o->_n_sc_id;
					SLL_IDENTIFIER_SET_STRING_INDEX(oil->dt+off+j,*(sf_m_dt.sm+SLL_IDENTIFIER_GET_STRING_INDEX(il->dt+j)),SLL_IDENTIFIER_IS_TLS(il->dt+j));
				}
			}
			if (sf->idt.ill){
				sll_identifier_list_length_t off=o->idt.ill;
				sf_m_dt.idx_off[SLL_MAX_SHORT_IDENTIFIER_LENGTH]=sll_allocate_stack(sf->idt.ill*sizeof(sll_identifier_index_t));
				o->idt.ill+=sf->idt.ill;
				o->idt.il=sll_reallocate(o->idt.il,o->idt.ill*sizeof(sll_identifier_t));
				for (sll_identifier_list_length_t i=0;i<sf->idt.ill;i++){
					*(sf_m_dt.idx_off[SLL_MAX_SHORT_IDENTIFIER_LENGTH]+i)=SLL_CREATE_IDENTIFIER(off+i,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
					(o->idt.il+off+i)->sc=(sf->idt.il+i)->sc+o->_n_sc_id;
					SLL_IDENTIFIER_SET_STRING_INDEX(o->idt.il+off+i,*(sf_m_dt.sm+SLL_IDENTIFIER_GET_STRING_INDEX(sf->idt.il+i)),SLL_IDENTIFIER_IS_TLS(sf->idt.il+i));
				}
			}
			else{
				sf_m_dt.idx_off[SLL_MAX_SHORT_IDENTIFIER_LENGTH]=NULL;
			}
			for (sll_import_index_t i=0;i<sf->it.l;i++){
				sll_import_file_t* k=*(sf->it.dt+i);
				SLL_ASSERT(k->sfi);
				sll_identifier_index_t* ex_dt=*(export_dt+k->sfi-1);
				for (sll_identifier_list_length_t j=0;j<k->l;j++){
					*(sf_m_dt.idx_off[SLL_IDENTIFIER_GET_ARRAY_ID(k->dt[j])]+SLL_IDENTIFIER_GET_ARRAY_INDEX(k->dt[j]))=*(ex_dt+j);
				}
			}
			if (idx){
				sll_identifier_index_t* ex_dt=sll_allocate_stack(sf->et.l*sizeof(sll_identifier_index_t));
				*(export_dt+idx-1)=ex_dt;
				for (sll_export_table_length_t i=0;i<sf->et.l;i++){
					*(ex_dt+i)=*(sf_m_dt.idx_off[SLL_IDENTIFIER_GET_ARRAY_ID(*(sf->et.dt+i))]+SLL_IDENTIFIER_GET_ARRAY_INDEX(*(sf->et.dt+i)));
				}
			}
			if (sf->ft.l){
				o->ft.l+=sf->ft.l;
				o->ft.dt=sll_reallocate(o->ft.dt,o->ft.l*sizeof(sll_function_t*));
				for (sll_function_index_t i=0;i<sf->ft.l;i++){
					const sll_function_t* s=*(sf->ft.dt+i);
					sll_function_t* d=sll_allocate(sizeof(sll_function_t)+s->al*sizeof(sll_identifier_index_t));
					*(o->ft.dt+sf_m_dt.f_id_off+i)=d;
					d->off=s->off+o->_s.off;
					d->nm=(s->nm==SLL_MAX_STRING_INDEX?SLL_MAX_STRING_INDEX:*(sf_m_dt.sm+s->nm));
					d->desc=(s->desc==SLL_MAX_STRING_INDEX?SLL_MAX_STRING_INDEX:*(sf_m_dt.sm+s->desc));
					d->al=s->al;
					for (sll_arg_count_t j=0;j<SLL_FUNCTION_GET_ARGUMENT_COUNT(s);j++){
						d->a[j]=*(sf_m_dt.idx_off[SLL_IDENTIFIER_GET_ARRAY_ID(s->a[j])]+SLL_IDENTIFIER_GET_ARRAY_INDEX(s->a[j]));
					}
				}
			}
			_clone_node(sf->dt,o,&sf_m_dt);
			for (unsigned int i=0;i<=SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
				sll_deallocate(sf_m_dt.idx_off[i]);
			}
			sll_deallocate(sf_m_dt.sm);
		}
		o->_n_sc_id+=sf->_n_sc_id;
		o->dt->dt.ac++;
	} while (idx);
	for (sll_source_file_index_t i=0;i<c_dt->l-1;i++){
		sll_deallocate(*(export_dt+i));
	}
	sll_deallocate(export_dt);
}
