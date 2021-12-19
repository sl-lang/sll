#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/ift.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <math.h>
#include <stdint.h>



static sll_node_t* _patch_module(sll_node_t* mo,const import_module_data_t* im_dt){
	sll_node_t* o=_acquire_next_node(im_dt->c_dt);
	while (mo->t==SLL_NODE_TYPE_NOP||mo->t==SLL_NODE_TYPE_DBG||mo->t==NODE_TYPE_CHANGE_STACK){
		if (mo->t==NODE_TYPE_CHANGE_STACK){
			mo=mo->dt._p;
		}
		else{
			o->t=mo->t;
			if (mo->t==SLL_NODE_TYPE_DBG){
				o->dt.s=(mo->dt.s==SLL_MAX_STRING_INDEX?SLL_MAX_STRING_INDEX:mo->dt.s+im_dt->fp_off);
			}
			o=_acquire_next_node(im_dt->c_dt);
			mo++;
		}
	}
	*o=*mo;
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return mo+1;
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
			o->dt.s=*(im_dt->sm+o->dt.s);
			return mo+1;
		case SLL_NODE_TYPE_IDENTIFIER:
			for (sll_export_table_length_t i=0;i<im_dt->eiml;i++){
				if ((im_dt->eim+i)->a==o->dt.id){
					o->dt.id=(im_dt->eim+i)->b;
					return mo+1;
				}
			}
			o->dt.id=SLL_IDENTIFIER_ADD_INDEX(o->dt.id,im_dt->off[SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)]);
			return mo+1;
		case SLL_NODE_TYPE_FUNC:
			o->dt.fn.id+=im_dt->f_off;
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				mo++;
				while (l){
					l--;
					mo=_patch_module(mo,im_dt);
				}
				return mo;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
			{
				o->dt.l.sc+=im_dt->sc_off;
				sll_arg_count_t l=o->dt.l.ac;
				mo++;
				while (l){
					l--;
					mo=_patch_module(mo,im_dt);
				}
				return mo;
			}
		case SLL_NODE_TYPE_DECL:
			{
				sll_arg_count_t l=o->dt.d.ac;
				if (o->dt.d.nm!=SLL_MAX_STRING_INDEX){
					o->dt.d.nm=*(im_dt->sm+o->dt.d.nm);
				}
				mo++;
				while (l){
					l--;
					mo=_patch_module(mo,im_dt);
				}
				return mo;
			}
		case SLL_NODE_TYPE_DECL_COPY:
			o->dt.dc.t+=im_dt->oi_off;
			if (o->dt.dc.nm!=SLL_MAX_STRING_INDEX){
				o->dt.dc.nm=*(im_dt->sm+o->dt.dc.nm);
			}
			return mo+1;
	}
	sll_arg_count_t l=o->dt.ac;
	mo++;
	while (l){
		l--;
		mo=_patch_module(mo,im_dt);
	}
	return mo;
}



static sll_identifier_index_t _get_var_index(sll_compilation_data_t* c_dt,const extra_compilation_data_t* e_c_dt,const scope_data_t* l_sc,sll_string_t* str,void* arg,uint8_t fl,sll_error_t* e){
	sll_identifier_index_t o=SLL_MAX_VARIABLE_INDEX;
	if (str->l<=SLL_MAX_SHORT_IDENTIFIER_LENGTH){
		sll_identifier_list_t* k=c_dt->idt.s+str->l-1;
		sll_scope_t mx_sc=SLL_MAX_SCOPE;
		sll_identifier_index_t mx_i=0;
		if (!(fl&GET_VAR_INDEX_FLAG_FUNC)){
			for (sll_identifier_list_length_t i=0;i<k->l;i++){
				sll_identifier_t* si=k->dt+i;
				if (si->sc==SLL_MAX_SCOPE||!sll_string_equal(c_dt->st.dt+si->i,str)){
					continue;
				}
				if (si->sc==l_sc->l_sc){
					o=SLL_CREATE_IDENTIFIER(i,str->l-1);
					goto _check_new_var;
				}
				else if ((l_sc->m[si->sc>>6]&(1ull<<(si->sc&0x3f)))&&(mx_sc==SLL_MAX_SCOPE||si->sc>mx_sc)){
					mx_sc=si->sc;
					mx_i=SLL_CREATE_IDENTIFIER(i,str->l-1);
				}
			}
			if (mx_sc!=SLL_MAX_SCOPE){
				o=mx_i;
				goto _check_new_var;
			}
			if (fl&GET_VAR_INDEX_FLAG_UNKNOWN){
				return SLL_MAX_VARIABLE_INDEX;
			}
		}
		k->l++;
		k->dt=sll_reallocate(k->dt,k->l*sizeof(sll_identifier_t));
		(k->dt+k->l-1)->sc=l_sc->l_sc;
		o=SLL_CREATE_IDENTIFIER(k->l-1,str->l-1);
		(k->dt+k->l-1)->i=sll_add_string(&(c_dt->st),str,1);
		if (fl&GET_VAR_INDEX_FLAG_ASSIGN){
			e_c_dt->nv_dt->sz++;
			e_c_dt->nv_dt->dt=sll_reallocate(e_c_dt->nv_dt->dt,e_c_dt->nv_dt->sz*sizeof(sll_node_t*));
			*(e_c_dt->nv_dt->dt+e_c_dt->nv_dt->sz-1)=arg;
		}
		return o;
	}
	sll_scope_t mx_sc=SLL_MAX_SCOPE;
	sll_identifier_index_t mx_i=0;
	if (!(fl&GET_VAR_INDEX_FLAG_FUNC)){
		for (sll_identifier_list_length_t i=0;i<c_dt->idt.ill;i++){
			sll_identifier_t* k=c_dt->idt.il+i;
			sll_string_t* s=c_dt->st.dt+k->i;
			if (k->sc==SLL_MAX_SCOPE||!sll_string_equal(s,str)){
				continue;
			}
			if (k->sc==l_sc->l_sc){
				o=SLL_CREATE_IDENTIFIER(i,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
				goto _check_new_var;
			}
			else if ((l_sc->m[k->sc>>6]&(1ull<<(k->sc&0x3f)))&&(mx_sc==SLL_MAX_SCOPE||k->sc>mx_sc)){
				mx_sc=k->sc;
				mx_i=SLL_CREATE_IDENTIFIER(i,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
			}
		}
		if (mx_sc!=SLL_MAX_SCOPE){
			o=mx_i;
			goto _check_new_var;
		}
		if (fl&GET_VAR_INDEX_FLAG_UNKNOWN){
			return SLL_MAX_VARIABLE_INDEX;
		}
	}
	c_dt->idt.ill++;
	c_dt->idt.il=sll_reallocate(c_dt->idt.il,c_dt->idt.ill*sizeof(sll_identifier_t));
	o=SLL_CREATE_IDENTIFIER(c_dt->idt.ill-1,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
	(c_dt->idt.il+c_dt->idt.ill-1)->sc=l_sc->l_sc;
	(c_dt->idt.il+c_dt->idt.ill-1)->i=sll_add_string(&(c_dt->st),str,1);
	if (fl&GET_VAR_INDEX_FLAG_ASSIGN){
		e_c_dt->nv_dt->sz++;
		e_c_dt->nv_dt->dt=sll_reallocate(e_c_dt->nv_dt->dt,e_c_dt->nv_dt->sz*sizeof(sll_node_t*));
		*(e_c_dt->nv_dt->dt+e_c_dt->nv_dt->sz-1)=arg;
	}
	return o;
_check_new_var:;
	sll_free_string(str);
	for (uint32_t i=0;i<e_c_dt->nv_dt->sz;i++){
		if ((*(e_c_dt->nv_dt->dt+i))->dt.id==o){
			return SLL_MAX_VARIABLE_INDEX-1;
		}
	}
	return o;
}



static uint8_t _read_single_char(sll_file_t* rf,sll_file_offset_t st,sll_error_t* e,sll_char_t* o){
	sll_read_char_t c=sll_file_read_char(rf);
	if (c=='\r'||c=='\n'||c==SLL_END_OF_DATA){
		e->t=SLL_ERROR_UNMATCHED_QUOTES;
		e->dt.r.off=st;
		e->dt.r.sz=SLL_FILE_GET_OFFSET(rf)-st-1;
		return 1;
	}
	if (c=='\\'){
		c=sll_file_read_char(rf);
		if (c=='\r'||c=='\n'){
			e->t=SLL_ERROR_UNMATCHED_QUOTES;
			e->dt.r.off=st;
			e->dt.r.sz=SLL_FILE_GET_OFFSET(rf)-st-1;
			return 1;
		}
		if (c==SLL_END_OF_DATA){
			e->t=SLL_ERROR_UNMATCHED_QUOTES;
			e->dt.r.off=st;
			e->dt.r.sz=SLL_FILE_GET_OFFSET(rf)-st-1;
			return 1;
		}
		if (c=='\''||c=='"'||c=='\\'){
			*o=(sll_char_t)c;
			return 2;
		}
		else if (c=='x'){
			c=sll_file_read_char(rf);
			if (c==SLL_END_OF_DATA){
				e->t=SLL_ERROR_UNMATCHED_QUOTES;
				e->dt.r.off=st;
				e->dt.r.sz=SLL_FILE_GET_OFFSET(rf)-st-1;
				return 1;
			}
			if (c>96){
				c-=32;
			}
			if (c<48||(c>57&&c<65)||c>70){
				if (c=='\r'||c=='\n'){
					e->t=SLL_ERROR_UNMATCHED_QUOTES;
					e->dt.r.off=st;
					e->dt.r.sz=SLL_FILE_GET_OFFSET(rf)-st-1;
					return 1;
				}
				e->t=SLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER;
				e->dt.r.off=SLL_FILE_GET_OFFSET(rf)-1;
				e->dt.r.sz=1;
				return 1;
			}
			sll_char_t v=(c>64?c-55:c-48)<<4;
			c=sll_file_read_char(rf);
			if (c==SLL_END_OF_DATA){
				e->t=SLL_ERROR_UNMATCHED_QUOTES;
				e->dt.r.off=st;
				e->dt.r.sz=SLL_FILE_GET_OFFSET(rf)-st-1;
				return 1;
			}
			if (c>96){
				c-=32;
			}
			if (c<48||(c>57&&c<65)||c>70){
				if (c=='\r'||c=='\n'){
					e->t=SLL_ERROR_UNMATCHED_QUOTES;
					e->dt.r.off=st;
					e->dt.r.sz=SLL_FILE_GET_OFFSET(rf)-st-1;
					return 1;
				}
				e->t=SLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER;
				e->dt.r.off=SLL_FILE_GET_OFFSET(rf)-1;
				e->dt.r.sz=1;
				return 1;
			}
			c=(c>64?c-55:c-48)|v;
		}
		else if (c=='t'){
			c='\t';
		}
		else if (c=='n'){
			c='\n';
		}
		else if (c=='v'){
			c='\v';
		}
		else if (c=='f'){
			c='\f';
		}
		else if (c=='r'){
			c='\r';
		}
		else{
			SLL_UNIMPLEMENTED();
		}
	}
	*o=(sll_char_t)c;
	return 0;
}



static sll_bool_t _read_object_internal(sll_compilation_data_t* c_dt,sll_read_char_t c,const extra_compilation_data_t* e_c_dt,sll_error_t* e){
	uint8_t fl=e_c_dt->fl;
	const scope_data_t* l_sc=&(e_c_dt->sc);
	scope_data_t n_l_sc={
		NULL
	};
	const scope_data_t* b_l_sc=NULL;
	sll_file_t* rf=c_dt->rf;
	sll_file_offset_t st_off=0;
	int ec=-1;
	sll_node_t* o=NULL;
	sll_arg_count_t ac=0;
	sll_array_length_t al=0;
	sll_map_length_t ml=0;
	sll_node_offset_t f_off=0;
	while (c!=SLL_END_OF_DATA){
		if ((c>8&&c<14)||c==' '){
			while ((c>8&&c<14)||c==' '){
				if (c=='\n'){
					sll_node_t* dbg=_acquire_next_node(c_dt);
					dbg->t=SLL_NODE_TYPE_DBG;
					dbg->dt.s=SLL_MAX_STRING_INDEX;
				}
				c=sll_file_read_char(rf);
			}
		}
		else if (c==';'){
			while (c!='\n'&&c!='\r'&&c!=SLL_END_OF_DATA){
				c=sll_file_read_char(rf);
			}
		}
		else if (c==')'){
			if (n_l_sc.m){
				sll_deallocate(n_l_sc.m);
			}
			if (!o||ec!=')'){
				e->t=SLL_ERROR_UNMATCHED_CLOSE_PARENTHESES;
				e->dt.r.off=SLL_FILE_GET_OFFSET(rf)-1;
				e->dt.r.sz=1;
				return 0;
			}
			if (o->t==SLL_NODE_TYPE_ASSIGN&&ac==1&&e_c_dt->nv_dt->sz){
				sll_node_t* id=o+1;
				while (id->t==SLL_NODE_TYPE_NOP||id->t==SLL_NODE_TYPE_DBG||id->t==NODE_TYPE_CHANGE_STACK){
					id=(id->t==NODE_TYPE_CHANGE_STACK?id->dt._p:id+1);
				}
				if (id->t==SLL_NODE_TYPE_IDENTIFIER&&id==*(e_c_dt->nv_dt->dt+e_c_dt->nv_dt->sz-1)){
					e_c_dt->nv_dt->sz--;
				}
			}
			if (o->t==NODE_TYPE_UNKNOWN){
				o->t=SLL_NODE_TYPE_INT;
				o->dt.i=0;
			}
			else if (o->t==SLL_NODE_TYPE_ASSIGN){
				o->dt.ac=ac;
				if (ac<2){
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
				}
			}
			else if (o->t==SLL_NODE_TYPE_FUNC){
				if (!ac){
					o->t=SLL_NODE_TYPE_INT;
					o->dt.i=0;
				}
				else{
					sll_arg_count_t i=0;
					sll_node_t* arg=o+1;
					for (;i<ac;i++){
						while (arg->t==SLL_NODE_TYPE_NOP||arg->t==SLL_NODE_TYPE_DBG||arg->t==NODE_TYPE_CHANGE_STACK){
							arg=(arg->t==NODE_TYPE_CHANGE_STACK?arg->dt._p:arg+1);
						}
						if (arg->t!=SLL_NODE_TYPE_IDENTIFIER){
							break;
						}
						arg++;
					}
					o->dt.fn.ac=ac-i;
					o->dt.fn.id=c_dt->ft.l;
					c_dt->ft.l++;
					void* tmp=sll_reallocate(c_dt->ft.dt,c_dt->ft.l*sizeof(sll_function_t*));
					if (!tmp){
						SLL_ASSERT(!"Unable to sll_reallocateate Function Array");
					}
					c_dt->ft.dt=tmp;
					sll_function_t* f=sll_allocate(sizeof(sll_function_t)+i*sizeof(sll_identifier_index_t));
					f->off=f_off;
					f->al=i;
					f->nm=e_c_dt->a_nm;
					arg=o+1;
					for (sll_arg_count_t j=0;j<i;j++){
						while (arg->t==SLL_NODE_TYPE_NOP||arg->t==SLL_NODE_TYPE_DBG||arg->t==NODE_TYPE_CHANGE_STACK){
							arg=(arg->t==NODE_TYPE_CHANGE_STACK?arg->dt._p:arg+1);
						}
						SLL_ASSERT(arg->t==SLL_NODE_TYPE_IDENTIFIER);
						f->a[j]=arg->dt.id;
						arg->t=SLL_NODE_TYPE_NOP;
						arg++;
					}
					*(c_dt->ft.dt+c_dt->ft.l-1)=f;
				}
			}
			else if (o->t==SLL_NODE_TYPE_INTERNAL_FUNC){
				if (!ac){
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
					o->dt.ac=0;
				}
				else{
					sll_node_t* n=o+1;
					while (n->t==SLL_NODE_TYPE_NOP||n->t==SLL_NODE_TYPE_DBG||n->t==NODE_TYPE_CHANGE_STACK){
						n=(n->t==NODE_TYPE_CHANGE_STACK?n->dt._p:n+1);
					}
					if (n->t!=SLL_NODE_TYPE_STRING){
						o->t=SLL_NODE_TYPE_OPERATION_LIST;
						o->dt.ac=ac;
					}
					else{
						sll_string_t* s=c_dt->st.dt+n->dt.s;
						if (!s->l||s->l>SLL_INTERNAL_FUNCTION_MAX_LENGTH){
							sll_string_t ns;
							sll_string_select(s,0,SLL_INTERNAL_FUNCTION_MAX_LENGTH,1,&ns);
							n->dt.s=sll_add_string(&(c_dt->st),&ns,1);
							s=c_dt->st.dt+n->dt.s;
						}
						o->dt.fn.id=sll_lookup_internal_function(e_c_dt->i_ft,s->v);
						if (o->dt.fn.id==SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX){
							o->t=SLL_NODE_TYPE_INTERNAL_FUNC_LOAD;
							o->dt.ac=ac;
						}
						else{
							o->dt.fn.ac=ac-1;
							n->t=SLL_NODE_TYPE_NOP;
						}
					}
				}
			}
			else if (o->t==SLL_NODE_TYPE_FOR||o->t==SLL_NODE_TYPE_WHILE||o->t==SLL_NODE_TYPE_LOOP){
				if (!ac){
					o->t=SLL_NODE_TYPE_INT;
					o->dt.i=0;
				}
				else if (ac==1){
					o->t=SLL_NODE_TYPE_NOP;
				}
				else{
					o->dt.l.ac=ac;
				}
			}
			else if (o->t==SLL_NODE_TYPE_DECL){
				o->dt.d.ac=ac;
				o->dt.d.nm=e_c_dt->a_nm;
			}
			else{
				o->dt.ac=ac;
			}
			return 1;
		}
		else if (c=='}'){
			if (!o||ec!='}'){
				e->t=SLL_ERROR_UNMATCHED_CURLY_CLOSE_BRACKETS;
				e->dt.r.off=SLL_FILE_GET_OFFSET(rf)-1;
				e->dt.r.sz=1;
				return 0;
			}
			o->dt.ac=ac;
			return 1;
		}
		else if (c=='('||c=='{'){
			if (!o){
				st_off=SLL_FILE_GET_OFFSET(rf)-1;
				o=_acquire_next_node(c_dt);
				if (c=='{'){
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
					ec='}';
				}
				else{
					o->t=NODE_TYPE_UNKNOWN;
					ec=')';
				}
			}
			else{
				if (SLL_IS_OBJECT_TYPE_IF(o)&&(ac&1)){
					SLL_ASSERT(n_l_sc.m);
					sll_deallocate(n_l_sc.m);
					n_l_sc.l_sc=c_dt->_n_sc_id;
					n_l_sc.ml=(n_l_sc.l_sc+65)>>6;
					n_l_sc.m=sll_allocate(n_l_sc.ml*sizeof(uint64_t));
					n_l_sc.m[n_l_sc.ml-1]=0;
					for (scope_data_mask_length_t i=0;i<b_l_sc->ml;i++){
						*(n_l_sc.m+i)=*(b_l_sc->m+i);
					}
					n_l_sc.m[n_l_sc.ml-1]|=1ull<<(n_l_sc.l_sc&63);
					c_dt->_n_sc_id++;
					l_sc=&n_l_sc;
				}
_recurse_array_or_map:;
				if (o->t==SLL_NODE_TYPE_ASSIGN&&ac==1&&e_c_dt->nv_dt->sz){
					sll_node_t* id=o+1;
					while (id->t==SLL_NODE_TYPE_NOP||id->t==SLL_NODE_TYPE_DBG||id->t==NODE_TYPE_CHANGE_STACK){
						id=(id->t==NODE_TYPE_CHANGE_STACK?id->dt._p:id+1);
					}
					if (id->t==SLL_NODE_TYPE_IDENTIFIER&&id==*(e_c_dt->nv_dt->dt+e_c_dt->nv_dt->sz-1)){
						e_c_dt->nv_dt->sz--;
					}
				}
				sll_file_offset_t arg_s=SLL_FILE_GET_OFFSET(rf)-1;
				if (o->t==NODE_TYPE_UNKNOWN){
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
				}
				extra_compilation_data_t n_e_c_dt={
					fl&EXTRA_COMPILATION_DATA_INSIDE_FUNCTION,
					*l_sc,
					e_c_dt->i_ft,
					e_c_dt->il,
					e_c_dt->nv_dt,
					SLL_MAX_STRING_INDEX
				};
				if (o->t==SLL_NODE_TYPE_ASSIGN&&ac==1){
					sll_node_t* a=o+1;
					while (a->t==SLL_NODE_TYPE_NOP||a->t==SLL_NODE_TYPE_DBG||a->t==NODE_TYPE_CHANGE_STACK){
						a=(a->t==NODE_TYPE_CHANGE_STACK?a->dt._p:a+1);
					}
					if (a->t==SLL_NODE_TYPE_IDENTIFIER){
						n_e_c_dt.a_nm=(SLL_IDENTIFIER_GET_ARRAY_ID(a->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH?c_dt->idt.il+SLL_IDENTIFIER_GET_ARRAY_INDEX(a->dt.id):c_dt->idt.s[SLL_IDENTIFIER_GET_ARRAY_ID(a->dt.id)].dt+SLL_IDENTIFIER_GET_ARRAY_INDEX(a->dt.id))->i;
					}
				}
				if (!_read_object_internal(c_dt,c,&n_e_c_dt,e)){
					if (n_l_sc.m){
						sll_deallocate(n_l_sc.m);
					}
					return 0;
				}
				if (o->t==SLL_NODE_TYPE_ARRAY){
					if (al==SLL_MAX_ARRAY_LENGTH){
						e->t=SLL_ERROR_ARRAY_TOO_LONG;
						e->dt.r.off=arg_s;
						e->dt.r.sz=SLL_FILE_GET_OFFSET(rf)-arg_s-1;
						return 0;
					}
					al++;
				}
				else if (o->t==SLL_NODE_TYPE_MAP){
					if (ml==SLL_MAX_ARRAY_LENGTH){
						e->t=SLL_ERROR_MAP_TOO_LONG;
						e->dt.r.off=arg_s;
						e->dt.r.sz=SLL_FILE_GET_OFFSET(rf)-arg_s-1;
						return 0;
					}
					ml++;
				}
				else{
					if (ac==SLL_MAX_ARG_COUNT){
						e->t=SLL_ERROR_TOO_MANY_ARGUMENTS;
						e->dt.r.off=arg_s;
						e->dt.r.sz=SLL_FILE_GET_OFFSET(rf)-arg_s-1;
						if (n_l_sc.m){
							sll_deallocate(n_l_sc.m);
						}
						return 0;
					}
					ac++;
				}
			}
			c=sll_file_read_char(rf);
		}
		else if (o&&o->t==NODE_TYPE_UNKNOWN){
			char str[256];
			sll_string_length_t sz=0;
			do{
				if (sz<255){
					*(str+sz)=(sll_char_t)c;
					sz++;
				}
				c=sll_file_read_char(rf);
				if (c==SLL_END_OF_DATA){
					break;
				}
			} while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='{'&&c!='}'));
			o->t=SLL_NODE_TYPE_OPERATION_LIST;
			if (sz==1){
				if (*str=='!'){
					o->t=SLL_NODE_TYPE_NOT;
				}
				else if (*str=='='){
					o->t=SLL_NODE_TYPE_ASSIGN;
				}
				else if (*str=='?'){
					o->t=SLL_NODE_TYPE_IF;
				}
				else if (*str=='+'){
					o->t=SLL_NODE_TYPE_ADD;
				}
				else if (*str=='-'){
					o->t=SLL_NODE_TYPE_SUB;
				}
				else if (*str=='*'){
					o->t=SLL_NODE_TYPE_MULT;
				}
				else if (*str=='/'){
					o->t=SLL_NODE_TYPE_DIV;
				}
				else if (*str=='%'){
					o->t=SLL_NODE_TYPE_MOD;
				}
				else if (*str=='&'){
					o->t=SLL_NODE_TYPE_BIT_AND;
				}
				else if (*str=='|'){
					o->t=SLL_NODE_TYPE_BIT_OR;
				}
				else if (*str=='^'){
					o->t=SLL_NODE_TYPE_BIT_XOR;
				}
				else if (*str=='~'){
					o->t=SLL_NODE_TYPE_BIT_NOT;
				}
				else if (*str=='<'){
					o->t=SLL_NODE_TYPE_LESS;
				}
				else if (*str=='>'){
					o->t=SLL_NODE_TYPE_MORE;
				}
				else if (*str=='$'){
					o->t=SLL_NODE_TYPE_LENGTH;
				}
				else if (*str==':'){
					o->t=SLL_NODE_TYPE_ACCESS;
				}
				else if (*str=='.'){
					o->t=SLL_NODE_TYPE_NEW;
				}
				else if (*str=='@'){
					o->t=SLL_NODE_TYPE_BREAK;
				}
				else if (*str==','){
					o->t=SLL_NODE_TYPE_COMMA;
				}
				else if (*str=='#'){
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
					fl|=EXTRA_COMPILATION_DATA_VARIABLE_DEFINITION;
				}
			}
			else if (sz==2){
				if (*str==':'&&*(str+1)=='>'){
					o->t=SLL_NODE_TYPE_PRINT;
				}
				else if (*str=='&'&&*(str+1)=='&'){
					o->t=SLL_NODE_TYPE_AND;
				}
				else if (*str=='|'&&*(str+1)=='|'){
					o->t=SLL_NODE_TYPE_OR;
				}
				else if (*str=='!'&&*(str+1)=='!'){
					o->t=SLL_NODE_TYPE_BOOL;
				}
				else if (*str=='<'&&*(str+1)=='-'){
					o->t=SLL_NODE_TYPE_CALL;
				}
				else if (*str=='?'&&*(str+1)==':'){
					o->t=SLL_NODE_TYPE_INLINE_IF;
				}
				else if (*str=='?'&&*(str+1)=='?'){
					o->t=SLL_NODE_TYPE_SWITCH;
				}
				else if (*str=='-'&&*(str+1)=='>'){
					o->t=SLL_NODE_TYPE_FOR;
					o->dt.l.sc=c_dt->_n_sc_id;
				}
				else if (*str=='>'&&*(str+1)=='-'){
					o->t=SLL_NODE_TYPE_WHILE;
					o->dt.l.sc=c_dt->_n_sc_id;
				}
				else if (*str=='>'&&*(str+1)=='<'){
					o->t=SLL_NODE_TYPE_LOOP;
				}
				else if (*str=='+'&&*(str+1)=='+'){
					o->t=SLL_NODE_TYPE_INC;
				}
				else if (*str=='-'&&*(str+1)=='-'){
					o->t=SLL_NODE_TYPE_DEC;
				}
				else if (*str=='/'&&*(str+1)=='/'){
					o->t=SLL_NODE_TYPE_FLOOR_DIV;
				}
				else if (*str=='>'&&*(str+1)=='>'){
					o->t=SLL_NODE_TYPE_BIT_RSHIFT;
				}
				else if (*str=='<'&&*(str+1)=='<'){
					o->t=SLL_NODE_TYPE_BIT_LSHIFT;
				}
				else if (*str=='<'&&*(str+1)=='='){
					o->t=SLL_NODE_TYPE_LESS_EQUAL;
				}
				else if (*str=='='&&*(str+1)=='='){
					o->t=SLL_NODE_TYPE_EQUAL;
				}
				else if (*str=='!'&&*(str+1)=='='){
					o->t=SLL_NODE_TYPE_NOT_EQUAL;
				}
				else if (*str=='>'&&*(str+1)=='='){
					o->t=SLL_NODE_TYPE_MORE_EQUAL;
				}
				else if (*str==':'&&*(str+1)==':'){
					o->t=SLL_NODE_TYPE_CAST;
				}
				else if (*str==':'&&*(str+1)=='?'){
					o->t=SLL_NODE_TYPE_TYPEOF;
				}
				else if (*str=='&'&&*(str+1)==':'){
					o->t=SLL_NODE_TYPE_DECL;
				}
				else if (*str=='%'&&*(str+1)=='%'){
					o->t=SLL_NODE_TYPE_REF;
				}
				else if (*str=='@'&&*(str+1)=='@'){
					o->t=((fl&EXTRA_COMPILATION_DATA_INSIDE_FUNCTION)?SLL_NODE_TYPE_RETURN:SLL_NODE_TYPE_EXIT);
				}
				else if (*str=='#'&&*(str+1)=='#'){
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
					fl|=EXTRA_COMPILATION_DATA_EXPORT;
				}
			}
			else if (sz==3){
				if (*str==','&&*(str+1)==','&&*(str+2)==','){
					o->t=SLL_NODE_TYPE_FUNC;
					f_off=c_dt->_s.off-1;
					o->dt.fn.sc=c_dt->_n_sc_id;
					fl|=EXTRA_COMPILATION_DATA_INSIDE_FUNCTION;
				}
				else if (*str=='.'&&*(str+1)=='.'&&*(str+2)=='.'){
					o->t=SLL_NODE_TYPE_INTERNAL_FUNC;
				}
				else if (*str=='*'&&*(str+1)=='*'&&*(str+2)=='*'){
					o->t=SLL_NODE_TYPE_INLINE_FUNC;
					o->dt.fn.sc=c_dt->_n_sc_id;
					fl|=EXTRA_COMPILATION_DATA_INSIDE_FUNCTION;
				}
				else if (*str=='='&&*(str+1)=='='&&*(str+2)=='='){
					o->t=SLL_NODE_TYPE_STRICT_EQUAL;
				}
				else if (*str=='!'&&*(str+1)=='='&&*(str+2)=='='){
					o->t=SLL_NODE_TYPE_STRICT_NOT_EQUAL;
				}
				else if (*str=='<'&&*(str+1)=='<'&&*(str+2)=='<'){
					o->t=SLL_NODE_TYPE_CONTINUE;
				}
				else if (*str=='@'&&*(str+1)=='@'&&*(str+2)=='@'){
					o->t=SLL_NODE_TYPE_EXIT;
				}
				else if (*str=='-'&&*(str+1)=='-'&&*(str+2)=='-'){
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
					fl|=EXTRA_COMPILATION_DATA_IMPORT;
				}
			}
			if (SLL_IS_OBJECT_TYPE_IF(o)||o->t==SLL_NODE_TYPE_FOR||o->t==SLL_NODE_TYPE_WHILE||o->t==SLL_NODE_TYPE_LOOP||o->t==SLL_NODE_TYPE_FUNC||o->t==SLL_NODE_TYPE_INLINE_FUNC){
				n_l_sc.l_sc=c_dt->_n_sc_id;
				n_l_sc.ml=(n_l_sc.l_sc+65)>>6;
				n_l_sc.m=sll_allocate(n_l_sc.ml*sizeof(uint64_t));
				n_l_sc.m[n_l_sc.ml-1]=0;
				sll_copy_data(l_sc->m,l_sc->ml*sizeof(uint64_t),n_l_sc.m);
				n_l_sc.m[n_l_sc.l_sc>>6]|=1ull<<(n_l_sc.l_sc&63);
				c_dt->_n_sc_id++;
				b_l_sc=l_sc;
				l_sc=&n_l_sc;
			}
		}
		else if (c=='['||c=='<'){
			if (o){
				goto _recurse_array_or_map;
			}
			st_off=SLL_FILE_GET_OFFSET(rf)-1;
			o=_acquire_next_node(c_dt);
			o->t=(c=='['?SLL_NODE_TYPE_ARRAY:SLL_NODE_TYPE_MAP);
			c=sll_file_read_char(rf);
		}
		else if (c==']'){
			if (!o||o->t!=SLL_NODE_TYPE_ARRAY){
				e->t=SLL_ERROR_UNMATCHED_ARRAY_CLOSE_BRACKETS;
				e->dt.r.off=SLL_FILE_GET_OFFSET(rf)-1;
				e->dt.r.sz=1;
				return 0;
			}
			o->dt.al=al;
			return 1;
		}
		else if (c=='>'){
			if (!o||o->t!=SLL_NODE_TYPE_MAP){
				e->t=SLL_ERROR_UNMATCHED_MAP_CLOSE_BRACKETS;
				e->dt.r.off=SLL_FILE_GET_OFFSET(rf)-1;
				e->dt.r.sz=1;
				return 0;
			}
			o->dt.ml=ml;
			return 1;
		}
		else if (c=='|'){
			c=sll_file_read_char(rf);
			if (c!='#'){
				e->t=SLL_ERROR_UNEXPECTED_CHARACTER;
				e->dt.r.off=SLL_FILE_GET_OFFSET(rf)-3;
				e->dt.r.sz=1;
				if (n_l_sc.m){
					sll_deallocate(n_l_sc.m);
				}
				return 0;
			}
			int lc=c;
			do{
				lc=c;
				c=sll_file_read_char(rf);
				if (c==SLL_END_OF_DATA){
					e->t=SLL_ERROR_UNMATCHED_OPEN_QUOTE;
					e->dt.r.off=SLL_FILE_GET_OFFSET(rf)-3;
					e->dt.r.sz=1;
					if (n_l_sc.m){
						sll_deallocate(n_l_sc.m);
					}
					return 0;
				}
			} while (c!='|'||lc!='#');
			c=sll_file_read_char(rf);
		}
		else{
			sll_file_offset_t arg_s=SLL_FILE_GET_OFFSET(rf)-1;
			sll_node_t* arg=_acquire_next_node(c_dt);
			if (c=='\''){
				arg->t=SLL_NODE_TYPE_CHAR;
				uint8_t err=_read_single_char(rf,arg_s,e,&(arg->dt.c));
				if (err==1){
					if (n_l_sc.m){
						sll_deallocate(n_l_sc.m);
					}
					return 0;
				}
				if (!err&&arg->dt.c=='\''){
					arg->dt.c=0;
				}
				else{
					sll_char_t tmp;
					do{
						err=_read_single_char(rf,arg_s,e,&tmp);
						if (err==1){
							if (n_l_sc.m){
								sll_deallocate(n_l_sc.m);
							}
							return 0;
						}
					} while (err||tmp!='\'');
				}
				c=sll_file_read_char(rf);
			}
			else if (c=='"'){
				arg->t=SLL_NODE_TYPE_STRING;
				sll_string_t s;
				sll_string_create(0,&s);
				s.v=sll_memory_move(s.v,SLL_MEMORY_MOVE_DIRECTION_TO_STACK);
				while (1){
					sll_string_increase(&s,1);
					uint8_t err=_read_single_char(rf,arg_s,e,s.v+s.l);
					if (err==1){
						sll_deallocate(s.v);
						if (n_l_sc.m){
							sll_deallocate(n_l_sc.m);
						}
						return 0;
					}
					if (!err&&s.v[s.l]=='"'){
						break;
					}
					s.l++;
				}
				SLL_STRING_FORMAT_PADDING(s.v,s.l);
				s.v=sll_memory_move(s.v,SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
				sll_string_calculate_checksum(&s);
				arg->dt.s=sll_add_string(&(c_dt->st),&s,1);
				c=sll_file_read_char(rf);
			}
			else if ((c>47&&c<58)||c=='-'){
				int8_t m=1;
				if (c=='-'){
					m=-1;
					c=sll_file_read_char(rf);
					if (c==SLL_END_OF_DATA){
						break;
					}
				}
				sll_integer_t v=0;
				if (c=='0'){
					c=sll_file_read_char(rf);
					if (c==SLL_END_OF_DATA){
						break;
					}
					if (c=='x'||c=='X'){
						c=sll_file_read_char(rf);
						if (c==SLL_END_OF_DATA){
							break;
						}
						do{
							if (c>96){
								c-=32;
							}
							if (c<48||(c>57&&c<65)||(c>70&&c!='_')){
								e->t=SLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER;
								e->dt.r.off=SLL_FILE_GET_OFFSET(rf)-1;
								e->dt.r.sz=1;
								if (n_l_sc.m){
									sll_deallocate(n_l_sc.m);
								}
								return 0;
							}
							if (c!='_'){
								v=(v<<4)+(c>64?c-55:c-48);
							}
							c=sll_file_read_char(rf);
							if (c==SLL_END_OF_DATA){
								goto _return_error;
							}
						} while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'));
					}
					else if (c=='o'||c=='O'){
						c=sll_file_read_char(rf);
						if (c==SLL_END_OF_DATA){
							break;
						}
						do{
							if (c<48||(c>55&&c!='_')){
								e->t=SLL_ERROR_UNKNOWN_OCTAL_CHARCTER;
								e->dt.r.off=SLL_FILE_GET_OFFSET(rf)-1;
								e->dt.r.sz=1;
								if (n_l_sc.m){
									sll_deallocate(n_l_sc.m);
								}
								return 0;
							}
							if (c!='_'){
								v=(v<<3)+(c-48);
							}
							c=sll_file_read_char(rf);
							if (c==SLL_END_OF_DATA){
								goto _return_error;
							}
						} while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'));
					}
					else if (c=='b'||c=='B'){
						c=sll_file_read_char(rf);
						if (c==SLL_END_OF_DATA){
							break;
						}
						do{
							if (c!='0'&&c!='1'&&c!='_'){
								e->t=SLL_ERROR_UNKNOWN_BINARY_CHARCTER;
								e->dt.r.off=SLL_FILE_GET_OFFSET(rf)-1;
								e->dt.r.sz=1;
								if (n_l_sc.m){
									sll_deallocate(n_l_sc.m);
								}
								return 0;
							}
							if (c!='_'){
								v=(v<<1)+(c-48);
							}
							c=sll_file_read_char(rf);
							if (c==SLL_END_OF_DATA){
								goto _return_error;
							}
						} while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'));
					}
					if (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'&&c!='.'&&c!='e'&&c!='E'&&(c<48||c>57))){
						e->t=SLL_ERROR_UNKNOWN_DECIMAL_CHARCTER;
						e->dt.r.off=SLL_FILE_GET_OFFSET(rf)-1;
						e->dt.r.sz=1;
						if (n_l_sc.m){
							sll_deallocate(n_l_sc.m);
						}
						return 0;
					}
				}
				if (c>47&&c<58){
					while (1){
						if (c!='_'){
							v=v*10+(c-48);
						}
						c=sll_file_read_char(rf);
						if (c==SLL_END_OF_DATA){
							goto _return_error;
						}
						if ((c>8&&c<14)||c==' '||c=='('||c==')'||c==';'||c=='<'||c=='>'||c=='['||c==']'||c=='{'||c=='}'||c=='.'||c=='e'||c=='E'){
							break;
						}
						if (c<48||(c>57&&c!='_')){
							e->t=SLL_ERROR_UNKNOWN_DECIMAL_CHARCTER;
							e->dt.r.off=SLL_FILE_GET_OFFSET(rf)-1;
							e->dt.r.sz=1;
							if (n_l_sc.m){
								sll_deallocate(n_l_sc.m);
							}
							return 0;
						}
					}
				}
				if (c=='.'||c=='e'||c=='E'){
					int16_t ex=0;
					if (c=='.'){
						while (1){
							c=sll_file_read_char(rf);
							if (c==SLL_END_OF_DATA){
								break;
							}
							if ((c>8&&c<14)||c==' '||c=='('||c==')'||c==';'||c=='<'||c=='>'||c=='['||c==']'||c=='{'||c=='}'||c=='e'||c=='E'){
								break;
							}
							if (c<48||(c>57&&c!='_')){
								e->t=SLL_ERROR_UNKNOWN_DECIMAL_CHARCTER;
								e->dt.r.off=SLL_FILE_GET_OFFSET(rf)-1;
								e->dt.r.sz=1;
								if (n_l_sc.m){
									sll_deallocate(n_l_sc.m);
								}
								return 0;
							}
							ex--;
							if (c!='_'){
								v=v*10+(c-48);
							}
						}
					}
					if (c=='e'||c=='E'){
						int8_t em=1;
						int16_t ev=0;
						c=sll_file_read_char(rf);
						if (c==SLL_END_OF_DATA){
							break;
						}
						if (c=='-'){
							em=-1;
							c=sll_file_read_char(rf);
							if (c==SLL_END_OF_DATA){
								break;
							}
						}
						else if (c=='+'){
							c=sll_file_read_char(rf);
							if (c==SLL_END_OF_DATA){
								break;
							}
						}
						while (1){
							if ((c>8&&c<14)||c==' '||c=='('||c==')'||c==';'||c=='<'||c=='>'||c=='['||c==']'||c=='{'||c=='}'){
								break;
							}
							if (c<48||(c>57&&c!='_')){
								e->t=SLL_ERROR_UNKNOWN_DECIMAL_CHARCTER;
								e->dt.r.off=SLL_FILE_GET_OFFSET(rf)-1;
								e->dt.r.sz=1;
								if (n_l_sc.m){
									sll_deallocate(n_l_sc.m);
								}
								return 0;
							}
							if (c!='_'){
								ev=ev*10+(c-48);
							}
							c=sll_file_read_char(rf);
							if (c==SLL_END_OF_DATA){
								break;
							}
						}
						ex+=ev*em;
					}
					if (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}')){
						e->t=SLL_ERROR_UNKNOWN_DECIMAL_CHARCTER;
						e->dt.r.off=SLL_FILE_GET_OFFSET(rf)-1;
						e->dt.r.sz=1;
						if (n_l_sc.m){
							sll_deallocate(n_l_sc.m);
						}
						return 0;
					}
					arg->t=SLL_NODE_TYPE_FLOAT;
					arg->dt.f=((sll_float_t)v)*pow(5,ex)*pow(2,ex)*m;
				}
				else{
					arg->t=SLL_NODE_TYPE_INT;
					arg->dt.i=v*m;
				}
			}
			else if ((c>64&&c<91)||c=='_'||(c>96&&c<123)){
				sll_string_t str;
				sll_string_create(255,&str);
				uint8_t	sz=0;
				do{
					if (sz<255){
						str.v[sz]=(sll_char_t)c;
						sz++;
					}
					c=sll_file_read_char(rf);
					if (c==SLL_END_OF_DATA){
						sll_free_string(&str);
						goto _return_error;
					}
				} while ((c>47&&c<58)||(c>64&&c<91)||c=='_'||(c>96&&c<123));
				str.l=sz;
				if (o&&(o->t!=SLL_NODE_TYPE_DECL||!(ac&1))&&c=='$'){
					c=sll_file_read_char(rf);
					if (c==SLL_END_OF_DATA){
						sll_free_string(&str);
						goto _return_error;
					}
					if (c<48||(c>57&&c<65)||(c>90&&c<96&&c!='_')||c>122){
						goto _unknown_identifier_char;
					}
					sll_string_calculate_checksum(&str);
					arg->t=SLL_NODE_TYPE_VAR_ACCESS;
					arg->dt.ac=1;
					sll_node_t* v=_acquire_next_node(c_dt);
					v->t=SLL_NODE_TYPE_IDENTIFIER;
					v->dt.id=_get_var_index(c_dt,e_c_dt,l_sc,&str,arg,GET_VAR_INDEX_FLAG_UNKNOWN,e);
					if (v->dt.i==SLL_MAX_VARIABLE_INDEX){
						sll_free_string(&str);
						e->t=SLL_ERROR_UNKNOWN_IDENTIFIER;
						e->dt.r.off=arg_s;
						e->dt.r.sz=SLL_FILE_GET_OFFSET(rf)-arg_s-2;
						if (n_l_sc.m){
							sll_deallocate(n_l_sc.m);
						}
						return 0;
					}
					if (v->dt.i==SLL_MAX_VARIABLE_INDEX-1){
						arg->t=SLL_NODE_TYPE_INT;
						arg->dt.i=0;
						v->t=SLL_NODE_TYPE_NOP;
					}
					else{
						while (1){
							sll_string_create(255,&str);
							sz=0;
							do{
								if (sz<255){
									str.v[sz]=(sll_char_t)c;
									sz++;
								}
								c=sll_file_read_char(rf);
								if (c==SLL_END_OF_DATA){
									sll_free_string(&str);
									goto _return_error;
								}
							} while ((c>47&&c<58)||(c>64&&c<91)||c=='_'||(c>96&&c<123));
							if (c<9||(c>13&&c!='$'&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}')){
								goto _unknown_identifier_char;
							}
							str.l=sz;
							sll_string_calculate_checksum(&str);
							arg->dt.ac++;
							v=_acquire_next_node(c_dt);
							v->t=SLL_NODE_TYPE_FIELD;
							v->dt.s=sll_add_string(&(c_dt->st),&str,1);
							if (c!='$'){
								break;
							}
							c=sll_file_read_char(rf);
							if (c==SLL_END_OF_DATA){
								goto _return_error;
							}
						}
					}
				}
				else if (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}')){
_unknown_identifier_char:
					sll_free_string(&str);
					e->t=SLL_ERROR_UNKNOWN_IDENTIFIER_CHARACTER;
					e->dt.r.off=SLL_FILE_GET_OFFSET(rf)-1;
					e->dt.r.sz=1;
					if (n_l_sc.m){
						sll_deallocate(n_l_sc.m);
					}
					return 0;
				}
				else if (o&&o->t==SLL_NODE_TYPE_DECL&&(ac&1)){
					sll_string_calculate_checksum(&str);
					arg->t=SLL_NODE_TYPE_FIELD;
					arg->dt.s=sll_add_string(&(c_dt->st),&str,1);
				}
				else if ((sz==3&&sll_compare_data(str.v,"nil",3)==SLL_COMPARE_RESULT_EQUAL)||(sz==5&&sll_compare_data(str.v,"false",5)==SLL_COMPARE_RESULT_EQUAL)){
					sll_free_string(&str);
					arg->t=SLL_NODE_TYPE_INT;
					arg->dt.i=0;
				}
				else if (sz==4&&sll_compare_data(str.v,"true",4)==SLL_COMPARE_RESULT_EQUAL){
					sll_free_string(&str);
					arg->t=SLL_NODE_TYPE_INT;
					arg->dt.i=1;
				}
				else{
					sll_string_calculate_checksum(&str);
					arg->t=SLL_NODE_TYPE_IDENTIFIER;
					arg->dt.id=_get_var_index(c_dt,e_c_dt,l_sc,&str,arg,((!o||o->t!=SLL_NODE_TYPE_ASSIGN||ac)&&!(fl&EXTRA_COMPILATION_DATA_VARIABLE_DEFINITION)?GET_VAR_INDEX_FLAG_UNKNOWN:0)|(o&&o->t==SLL_NODE_TYPE_ASSIGN?GET_VAR_INDEX_FLAG_ASSIGN:(o&&o->t==SLL_NODE_TYPE_FUNC?GET_VAR_INDEX_FLAG_FUNC:0)),e);
					if (arg->dt.i==SLL_MAX_VARIABLE_INDEX){
						sll_free_string(&str);
						e->t=SLL_ERROR_UNKNOWN_IDENTIFIER;
						e->dt.r.off=arg_s;
						e->dt.r.sz=SLL_FILE_GET_OFFSET(rf)-arg_s-1;
						if (n_l_sc.m){
							sll_deallocate(n_l_sc.m);
						}
						return 0;
					}
					if (arg->dt.i==SLL_MAX_VARIABLE_INDEX-1){
						arg->t=SLL_NODE_TYPE_INT;
						arg->dt.i=0;
					}
				}
			}
			else{
				e->t=SLL_ERROR_UNEXPECTED_CHARACTER;
				e->dt.r.off=SLL_FILE_GET_OFFSET(rf)-1;
				e->dt.r.sz=1;
				if (n_l_sc.m){
					sll_deallocate(n_l_sc.m);
				}
				return 0;
			}
			if (!o){
				return 1;
			}
			if (o->t==SLL_NODE_TYPE_ARRAY){
				if (al==SLL_MAX_ARRAY_LENGTH){
					e->t=SLL_ERROR_ARRAY_TOO_LONG;
					e->dt.r.off=arg_s;
					e->dt.r.sz=SLL_FILE_GET_OFFSET(rf)-arg_s-1;
					return 0;
				}
				al++;
			}
			else if (o->t==SLL_NODE_TYPE_MAP){
				if (ml==SLL_MAX_ARRAY_LENGTH){
					e->t=SLL_ERROR_MAP_TOO_LONG;
					e->dt.r.off=arg_s;
					e->dt.r.sz=SLL_FILE_GET_OFFSET(rf)-arg_s-1;
					return 0;
				}
				ml++;
			}
			else{
				if (SLL_IS_OBJECT_TYPE_IF(o)&&(ac&1)){
					SLL_ASSERT(n_l_sc.m);
					sll_deallocate(n_l_sc.m);
					n_l_sc.l_sc=c_dt->_n_sc_id;
					n_l_sc.ml=(n_l_sc.l_sc+65)>>6;
					n_l_sc.m=sll_allocate(n_l_sc.ml*sizeof(uint64_t));
					n_l_sc.m[n_l_sc.ml-1]=0;
					for (scope_data_mask_length_t i=0;i<b_l_sc->ml;i++){
						*(n_l_sc.m+i)=*(b_l_sc->m+i);
					}
					n_l_sc.m[n_l_sc.ml-1]|=1ull<<(n_l_sc.l_sc&63);
					c_dt->_n_sc_id++;
					l_sc=&n_l_sc;
				}
				else if (o->t==SLL_NODE_TYPE_ASSIGN&&ac==1&&e_c_dt->nv_dt->sz){
					sll_node_t* id=o+1;
					while (id->t==SLL_NODE_TYPE_NOP||id->t==SLL_NODE_TYPE_DBG||id->t==NODE_TYPE_CHANGE_STACK){
						id=(id->t==NODE_TYPE_CHANGE_STACK?id->dt._p:id+1);
					}
					if (id->t==SLL_NODE_TYPE_IDENTIFIER&&id==*(e_c_dt->nv_dt->dt+e_c_dt->nv_dt->sz-1)){
						e_c_dt->nv_dt->sz--;
					}
				}
				if (ac==SLL_MAX_ARG_COUNT){
					e->t=SLL_ERROR_TOO_MANY_ARGUMENTS;
					e->dt.r.off=arg_s;
					e->dt.r.sz=SLL_FILE_GET_OFFSET(rf)-arg_s-1;
					if (n_l_sc.m){
						sll_deallocate(n_l_sc.m);
					}
					return 0;
				}
				ac++;
				if ((fl&EXTRA_COMPILATION_DATA_IMPORT)&&arg->t==SLL_NODE_TYPE_STRING){
					if (ac==SLL_MAX_ARG_COUNT){
						e->t=SLL_ERROR_TOO_MANY_ARGUMENTS;
						e->dt.r.off=arg_s;
						e->dt.r.sz=SLL_FILE_GET_OFFSET(rf)-arg_s-1;
						return 0;
					}
					sll_node_t* dbg=_acquire_next_node(c_dt);
					dbg->t=SLL_NODE_TYPE_DBG;
					dbg->dt.s=c_dt->fpt.l;
					ac++;
					sll_compilation_data_t im=SLL_INIT_COMPILATION_DATA_STRUCT;
					if (e_c_dt->il(c_dt->st.dt+arg->dt.s,&im,e)==0){
						return 0;
					}
					import_module_data_t im_dt={
						.sm=sll_allocate(im.st.l*sizeof(sll_string_index_t)),
						.f_off=c_dt->ft.l,
						.eiml=im.et.l,
						.sc_off=c_dt->_n_sc_id,
						.c_dt=c_dt,
						.oi_off=c_dt->ot_it.l,
						.fp_off=c_dt->fpt.l
					};
					for (sll_string_index_t i=0;i<im.st.l;i++){
						sll_string_t* s=im.st.dt+i;
						for (sll_string_index_t j=0;j<c_dt->st.l;j++){
							if (sll_string_equal(s,c_dt->st.dt+j)){
								*(im_dt.sm+i)=j;
								goto _merge_next_string;
							}
						}
						*(im_dt.sm+i)=c_dt->st.l;
						c_dt->st.l++;
						c_dt->st.dt=sll_reallocate(c_dt->st.dt,c_dt->st.l*sizeof(sll_string_t));
						sll_string_clone(s,c_dt->st.dt+c_dt->st.l-1);
_merge_next_string:;
					}
					c_dt->fpt.l+=im.fpt.l;
					c_dt->fpt.dt=sll_reallocate(c_dt->fpt.dt,c_dt->fpt.l*sizeof(sll_string_index_t));
					for (sll_string_index_t i=0;i<im.fpt.l;i++){
						*(c_dt->fpt.dt+im_dt.fp_off+i)=*(im_dt.sm+(*(im.fpt.dt+i)));
					}
					if (im.ot_it.l){
						c_dt->ot_it.l+=im.ot_it.l;
						c_dt->ot_it.dt=sll_reallocate(c_dt->ot_it.dt,c_dt->ot_it.l*sizeof(sll_object_type_initializer_t*));
						for (sll_object_type_t i=0;i<im.ot_it.l;i++){
							const sll_object_type_initializer_t* oi=*(im.ot_it.dt+i);
							sll_object_type_initializer_t* n=sll_allocate(sizeof(sll_object_type_initializer_t)+oi->l*sizeof(sll_object_type_field_t));
							n->l=oi->l;
							SLL_ASSERT(n->l);
							for (sll_arg_count_t j=0;j<n->l;j++){
								n->dt[j].t=oi->dt[j].t;
								n->dt[j].f=*(im_dt.sm+oi->dt[j].f);
							}
							*(c_dt->ot_it.dt+im_dt.oi_off+i)=n;
						}
					}
					for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
						sll_identifier_list_t* il=c_dt->idt.s+i;
						sll_identifier_list_t mil=im.idt.s[i];
						im_dt.off[i]=il->l;
						if (mil.l){
							il->l+=mil.l;
							il->dt=sll_reallocate(il->dt,il->l*sizeof(sll_identifier_t));
							for (sll_identifier_list_length_t j=0;j<mil.l;j++){
								(il->dt+im_dt.off[i]+j)->sc=(mil.dt+j)->sc+c_dt->_n_sc_id;
								(il->dt+im_dt.off[i]+j)->i=*(im_dt.sm+(mil.dt+j)->i);
							}
						}
					}
					im_dt.off[SLL_MAX_SHORT_IDENTIFIER_LENGTH]=c_dt->idt.ill;
					sll_identifier_list_length_t si=c_dt->idt.ill;
					if (im.idt.ill){
						c_dt->idt.ill+=im.idt.ill;
						c_dt->idt.il=sll_reallocate(c_dt->idt.il,c_dt->idt.ill*sizeof(sll_identifier_t));
						for (sll_identifier_list_length_t j=0;j<im.idt.ill;j++){
							(c_dt->idt.il+si+j)->sc=(im.idt.il+j)->sc+c_dt->_n_sc_id;
							(c_dt->idt.il+si+j)->i=*(im_dt.sm+(im.idt.il+j)->i);
						}
					}
					im_dt.eim=sll_allocate(im.et.l*sizeof(identifier_pair_t));
					for (sll_export_table_length_t i=0;i<im.et.l;i++){
						sll_identifier_index_t eii=*(im.et.dt+i);
						(im_dt.eim+i)->a=eii;
						sll_scope_t mx_sc=SLL_MAX_SCOPE;
						sll_identifier_index_t mx_i=0;
						sll_identifier_t* ei;
						uint8_t j=SLL_IDENTIFIER_GET_ARRAY_ID(eii);
						if (j==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
							ei=c_dt->idt.il+si+SLL_IDENTIFIER_GET_ARRAY_INDEX(eii);
							for (sll_identifier_list_length_t k=0;k<c_dt->idt.ill;k++){
								sll_identifier_t* n=c_dt->idt.il+k;
								if (k==si+SLL_IDENTIFIER_GET_ARRAY_INDEX(eii)||n->i!=ei->i){
									continue;
								}
								if (n->sc==l_sc->l_sc){
									ei->sc=SLL_MAX_SCOPE;
									(im_dt.eim+i)->b=SLL_CREATE_IDENTIFIER(k,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
									goto _export_identifier_found;
								}
								if ((l_sc->m[n->sc>>6]&(1ull<<(n->sc&0x3f)))&&(mx_sc==SLL_MAX_SCOPE||n->sc>mx_sc)){
									mx_sc=n->sc;
									mx_i=SLL_CREATE_IDENTIFIER(k,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
								}
							}
						}
						else{
							sll_identifier_list_t* il=c_dt->idt.s+j;
							ei=il->dt+im_dt.off[j]+SLL_IDENTIFIER_GET_ARRAY_INDEX(eii);
							for (sll_identifier_list_length_t k=0;k<il->l;k++){
								sll_identifier_t* n=il->dt+k;
								if (k==im_dt.off[j]+SLL_IDENTIFIER_GET_ARRAY_INDEX(eii)||n->i!=ei->i){
									continue;
								}
								if (n->sc==l_sc->l_sc){
									ei->sc=SLL_MAX_SCOPE;
									(im_dt.eim+i)->b=SLL_CREATE_IDENTIFIER(k,j);
									goto _export_identifier_found;
								}
								if ((l_sc->m[n->sc>>6]&(1ull<<(n->sc&0x3f)))&&(mx_sc==SLL_MAX_SCOPE||n->sc>mx_sc)){
									mx_sc=n->sc;
									mx_i=SLL_CREATE_IDENTIFIER(k,j);
								}
							}
						}
						if (mx_sc!=SLL_MAX_SCOPE){
							ei->sc=SLL_MAX_SCOPE;
							(im_dt.eim+i)->b=mx_i;
						}
						else{
							ei->sc=l_sc->l_sc;
							(im_dt.eim+i)->a=SLL_MAX_IDENTIFIER_INDEX;
						}
_export_identifier_found:;
					}
					sll_function_index_t j=c_dt->ft.l;
					c_dt->ft.l+=im.ft.l;
					c_dt->ft.dt=sll_reallocate(c_dt->ft.dt,c_dt->ft.l*sizeof(sll_function_t*));
					for (sll_function_index_t i=0;i<im.ft.l;i++){
						sll_function_t* f=*(im.ft.dt+i);
						sll_function_t* nf=sll_allocate(sizeof(sll_function_t)+f->al*sizeof(sll_identifier_index_t));
						nf->off=(sll_node_offset_t)(f->off+c_dt->_s.off);
						nf->al=f->al;
						nf->nm=(f->nm==SLL_MAX_STRING_INDEX?SLL_MAX_STRING_INDEX:*(im_dt.sm+f->nm));
						for (sll_arg_count_t k=0;k<f->al;k++){
							nf->a[k]=f->a[k];
						}
						*(c_dt->ft.dt+i+j)=nf;
					}
					c_dt->_n_sc_id+=im._n_sc_id;
					_patch_module(im.h,&im_dt);
					sll_deallocate(im_dt.sm);
					sll_deallocate(im_dt.eim);
					sll_free_compilation_data(&im);
					dbg=_acquire_next_node(c_dt);
					dbg->t=SLL_NODE_TYPE_DBG;
					dbg->dt.s=0;
				}
				else if ((fl&EXTRA_COMPILATION_DATA_EXPORT)&&arg->t==SLL_NODE_TYPE_IDENTIFIER){
					for (sll_export_table_length_t j=0;j<c_dt->et.l;j++){
						if (*(c_dt->et.dt+j)==arg->dt.id){
							goto _skip_export;
						}
					}
					c_dt->et.l++;
					c_dt->et.dt=sll_reallocate(c_dt->et.dt,c_dt->et.l*sizeof(sll_identifier_index_t));
					*(c_dt->et.dt+c_dt->et.l-1)=arg->dt.id;
_skip_export:;
				}
			}
		}
	}
	if (!o){
		o=_acquire_next_node(c_dt);
		o->t=SLL_NODE_TYPE_INT;
		o->dt.i=0;
		return 1;
	}
_return_error:;
	if (o->t==SLL_NODE_TYPE_ARRAY){
		e->t=SLL_ERROR_UNMATCHED_ARRAY_OPEN_BRACKETS;
	}
	else if (o->t==SLL_NODE_TYPE_MAP){
		e->t=SLL_ERROR_UNMATCHED_MAP_OPEN_BRACKETS;
	}
	else if (o->t==SLL_NODE_TYPE_OPERATION_LIST){
		e->t=SLL_ERROR_UNMATCHED_CURLY_OPEN_BRACKETS;
	}
	else{
		e->t=SLL_ERROR_UNMATCHED_OPEN_PARENTHESES;
	}
	e->dt.r.off=st_off;
	e->dt.r.sz=SLL_FILE_GET_OFFSET(rf)-st_off;
	if (n_l_sc.m){
		sll_deallocate(n_l_sc.m);
	}
	return 0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_parse_node(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft,sll_import_loader_t il,sll_error_t* e,sll_node_t** o){
	sll_node_t* a=c_dt->_s.p;
	SLL_ASSERT(a);
	sll_read_char_t c=sll_file_read_char(c_dt->rf);
	if (c==SLL_END_OF_DATA){
		a->t=SLL_NODE_TYPE_INT;
		a->dt.i=0;
		return 1;
	}
	new_variable_data_t nv_dt={
		NULL,
		0
	};
	extra_compilation_data_t e_c_dt={
		0,
		{
			sll_allocate(sizeof(uint64_t)),
			0,
			1
		},
		i_ft,
		il,
		&nv_dt,
		SLL_MAX_STRING_INDEX
	};
	e_c_dt.sc.m[0]=1;
	if (!_read_object_internal(c_dt,c,&e_c_dt,e)){
		sll_deallocate(e_c_dt.sc.m);
		sll_deallocate(nv_dt.dt);
		return 0;
	}
	sll_deallocate(e_c_dt.sc.m);
	sll_deallocate(nv_dt.dt);
	*o=a;
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_parse_all_nodes(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft,sll_import_loader_t il,sll_error_t* e){
	c_dt->h=_acquire_next_node(c_dt);
	c_dt->h->t=SLL_NODE_TYPE_OPERATION_LIST;
	c_dt->h->dt.ac=0;
	new_variable_data_t nv_dt={
		NULL,
		0
	};
	extra_compilation_data_t e_c_dt={
		0,
		{
			sll_allocate(sizeof(uint64_t)),
			0,
			1
		},
		i_ft,
		il,
		&nv_dt,
		SLL_MAX_STRING_INDEX
	};
	e_c_dt.sc.m[0]=1;
	while (1){
		sll_read_char_t c=sll_file_read_char(c_dt->rf);
		if (c==SLL_END_OF_DATA){
			sll_deallocate(e_c_dt.sc.m);
			sll_deallocate(nv_dt.dt);
			return 1;
		}
		sll_file_offset_t off=SLL_FILE_GET_OFFSET(c_dt->rf);
		if (!_read_object_internal(c_dt,c,&e_c_dt,e)){
			sll_deallocate(e_c_dt.sc.m);
			sll_deallocate(nv_dt.dt);
			return 0;
		}
		if (c_dt->h->dt.ac==SLL_MAX_ARG_COUNT){
			e->t=SLL_ERROR_TOO_MANY_ARGUMENTS;
			e->dt.r.off=off-1;
			e->dt.r.sz=SLL_FILE_GET_OFFSET(c_dt->rf)-off-1;
			sll_deallocate(e_c_dt.sc.m);
			sll_deallocate(nv_dt.dt);
			return 0;
		}
		c_dt->h->dt.ac++;
	}
}
