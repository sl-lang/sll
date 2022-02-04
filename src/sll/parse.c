#include <sll/_sll_internal.h>
#include <sll/api/math.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/file.h>
#include <sll/identifier.h>
#include <sll/ift.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>



#define ADD_DIGIT(n,d) \
	do{ \
		if (vc<19){ \
			v=v*n+d; \
		} \
		else if (vc<38){ \
			v2=v2*n+d; \
		} \
		vc++; \
	} while (0)
#define PUSH_REWIND_CHAR(c) \
	do{ \
		if (rewind_bf_l<255){ \
			rewind_bf[rewind_bf_l]=(sll_char_t)(c); \
			rewind_bf_l++; \
		} \
	} while (0)



static sll_node_t* _patch_module(sll_node_t* mo,const import_module_data_t* im_dt){
	sll_node_t* o=_acquire_next_node(im_dt->c_dt);
	while (mo->t==SLL_NODE_TYPE_NOP||mo->t==SLL_NODE_TYPE_DBG||mo->t==SLL_NODE_TYPE_CHANGE_STACK){
		if (mo->t==SLL_NODE_TYPE_CHANGE_STACK){
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
			o->dt.fn.sc+=im_dt->sc_off;
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
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
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
	}
	sll_arg_count_t l=o->dt.ac;
	mo++;
	while (l){
		l--;
		mo=_patch_module(mo,im_dt);
	}
	return mo;
}



static sll_identifier_index_t _get_var_index(sll_compilation_data_t* c_dt,const extra_compilation_data_t* e_c_dt,const scope_data_t* l_sc,sll_string_t* str,void* arg,unsigned int fl){
	SLL_ASSERT(str->l);
	sll_identifier_index_t o=SLL_MAX_VARIABLE_INDEX;
	if (str->l<=SLL_MAX_SHORT_IDENTIFIER_LENGTH){
		sll_identifier_list_t* k=c_dt->idt.s+str->l-1;
		sll_scope_t mx_sc=SLL_MAX_SCOPE;
		sll_identifier_index_t mx_i=0;
		if (!(fl&GET_VAR_INDEX_FLAG_FUNC)){
			for (sll_identifier_list_length_t i=0;i<k->l;i++){
				sll_identifier_t* si=k->dt+i;
				if (si->sc==SLL_MAX_SCOPE||l_sc->l_sc<si->sc||!sll_string_equal(c_dt->st.dt+si->i,str)){
					continue;
				}
				if (si->sc==l_sc->l_sc){
					o=SLL_CREATE_IDENTIFIER(i,str->l-1);
					goto _check_new_var;
				}
				else if ((mx_sc==SLL_MAX_SCOPE||si->sc>mx_sc)&&(l_sc->m[si->sc>>6]&(1ull<<(si->sc&0x3f)))){
					mx_sc=si->sc;
					mx_i=SLL_CREATE_IDENTIFIER(i,str->l-1);
				}
			}
			if (mx_sc!=SLL_MAX_SCOPE){
				o=mx_i;
				goto _check_new_var;
			}
			if (fl&GET_VAR_INDEX_FLAG_UNKNOWN){
				sll_free_string(str);
				return SLL_MAX_VARIABLE_INDEX;
			}
		}
		k->l++;
		k->dt=sll_reallocate(k->dt,k->l*sizeof(sll_identifier_t));
		(k->dt+k->l-1)->sc=l_sc->l_sc;
		o=SLL_CREATE_IDENTIFIER(k->l-1,str->l-1);
		(k->dt+k->l-1)->i=sll_add_string(&(c_dt->st),str,1);
		if ((fl&GET_VAR_INDEX_FLAG_ASSIGN)&&arg){
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
			if (k->sc==SLL_MAX_SCOPE||l_sc->l_sc<k->sc||!sll_string_equal(s,str)){
				continue;
			}
			if (k->sc==l_sc->l_sc){
				o=SLL_CREATE_IDENTIFIER(i,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
				goto _check_new_var;
			}
			else if ((mx_sc==SLL_MAX_SCOPE||k->sc>mx_sc)&&(l_sc->m[k->sc>>6]&(1ull<<(k->sc&0x3f)))){
				mx_sc=k->sc;
				mx_i=SLL_CREATE_IDENTIFIER(i,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
			}
		}
		if (mx_sc!=SLL_MAX_SCOPE){
			o=mx_i;
			goto _check_new_var;
		}
		if (fl&GET_VAR_INDEX_FLAG_UNKNOWN){
			sll_free_string(str);
			return SLL_MAX_VARIABLE_INDEX;
		}
	}
	c_dt->idt.ill++;
	c_dt->idt.il=sll_reallocate(c_dt->idt.il,c_dt->idt.ill*sizeof(sll_identifier_t));
	o=SLL_CREATE_IDENTIFIER(c_dt->idt.ill-1,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
	(c_dt->idt.il+c_dt->idt.ill-1)->sc=l_sc->l_sc;
	(c_dt->idt.il+c_dt->idt.ill-1)->i=sll_add_string(&(c_dt->st),str,1);
	if ((fl&GET_VAR_INDEX_FLAG_ASSIGN)&&arg){
		e_c_dt->nv_dt->sz++;
		e_c_dt->nv_dt->dt=sll_reallocate(e_c_dt->nv_dt->dt,e_c_dt->nv_dt->sz*sizeof(sll_node_t*));
		*(e_c_dt->nv_dt->dt+e_c_dt->nv_dt->sz-1)=arg;
	}
	return o;
_check_new_var:;
	sll_free_string(str);
	for (sll_arg_count_t i=0;i<e_c_dt->nv_dt->sz;i++){
		if ((*(e_c_dt->nv_dt->dt+i))->dt.id==o){
			return SLL_MAX_VARIABLE_INDEX;
		}
	}
	return o;
}



static unsigned int _read_single_char(sll_file_t* rf,sll_char_t* o){
	sll_read_char_t c=sll_file_read_char(rf);
	if (c==SLL_END_OF_DATA){
		return 1;
	}
	if (c!='\\'){
		*o=(sll_char_t)c;
		return 0;
	}
	c=sll_file_read_char(rf);
	if (c==SLL_END_OF_DATA){
		return 1;
	}
	if (c=='\''||c=='"'||c=='\\'){
		*o=(sll_char_t)c;
		return 2;
	}
	else if (c=='x'){
		c=sll_file_read_char(rf);
		if (c==SLL_END_OF_DATA){
			return 1;
		}
		if (c>96){
			c-=32;
		}
		if (c<48||(c>57&&c<65)||c>70){
			SLL_UNIMPLEMENTED();
		}
		sll_char_t v=(c>64?c-55:c-48);
		c=sll_file_read_char(rf);
		if (c==SLL_END_OF_DATA){
			return 1;
		}
		if (c>96){
			c-=32;
		}
		if (c<48||(c>57&&c<65)||c>70){
			SLL_UNIMPLEMENTED();
		}
		c=(v<<4)|(c>64?c-55:c-48);
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
	*o=(sll_char_t)c;
	return 0;
}



static sll_read_char_t _read_identifier(sll_string_t* str,sll_string_length_t sz,sll_file_t* rf,sll_read_char_t c){
	do{
		if (sz<255){
			str->v[sz]=(sll_char_t)c;
			sz++;
		}
		c=sll_file_read_char(rf);
		if (c==SLL_END_OF_DATA){
			break;
		}
	} while (c<9||(c>13&&c!='$'&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'));
	str->l=sz;
	sll_string_calculate_checksum(str);
	return c;
}



static void _read_object_internal(sll_file_t* rf,sll_compilation_data_t* c_dt,sll_read_char_t c,const extra_compilation_data_t* e_c_dt){
	unsigned int fl=0;
	const scope_data_t* l_sc=&(e_c_dt->sc);
	scope_data_t n_l_sc={
		NULL
	};
	sll_node_t* o=NULL;
	sll_arg_count_t ac=0;
	sll_array_length_t al=0;
	sll_map_length_t ml=0;
	sll_node_offset_t f_off=0;
	while (c!=SLL_END_OF_DATA){
		if ((c>8&&c<14)||c==' '){
			do{
				if (c=='\n'){
					sll_node_t* dbg=_acquire_next_node(c_dt);
					dbg->t=SLL_NODE_TYPE_DBG;
					dbg->dt.s=SLL_MAX_STRING_INDEX;
				}
				c=sll_file_read_char(rf);
			} while ((c>8&&c<14)||c==' ');
		}
		else if (c==';'){
			do{
				c=sll_file_read_char(rf);
			} while (c!='\n'&&c!=SLL_END_OF_DATA);
		}
		else if (o&&o->t==NODE_TYPE_UNKNOWN){
			sll_char_t str[256];
			sll_string_length_t sz=0;
			do{
				if (sz<255){
					*(str+sz)=(sll_char_t)c;
					sz++;
				}
				c=sll_file_read_char(rf);
			} while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='{'&&c!='}'&&c!=SLL_END_OF_DATA));
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
				else if (*str=='|'&&*(str+1)==':'){
					o->t=SLL_NODE_TYPE_HAS;
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
				else if (*str=='['&&*(str+1)=='>'){
					o->t=SLL_NODE_TYPE_FOR_ARRAY;
					o->dt.l.sc=c_dt->_n_sc_id;
				}
				else if (*str=='['&&*(str+1)=='<'){
					o->t=SLL_NODE_TYPE_WHILE_ARRAY;
					o->dt.l.sc=c_dt->_n_sc_id;
				}
				else if (*str=='{'&&*(str+1)=='>'){
					o->t=SLL_NODE_TYPE_FOR_MAP;
					o->dt.l.sc=c_dt->_n_sc_id;
				}
				else if (*str=='{'&&*(str+1)=='<'){
					o->t=SLL_NODE_TYPE_WHILE_MAP;
					o->dt.l.sc=c_dt->_n_sc_id;
				}
				else if (*str=='%'&&*(str+1)=='%'){
					o->t=SLL_NODE_TYPE_REF;
				}
				else if (*str=='@'&&*(str+1)=='@'){
					o->t=SLL_NODE_TYPE_RETURN;
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
					o->dt.fn.id=c_dt->ft.l;
					o->dt.fn.sc=c_dt->_n_sc_id;
				}
				else if (*str=='.'&&*(str+1)=='.'&&*(str+2)=='.'){
					o->t=SLL_NODE_TYPE_INTERNAL_FUNC;
				}
				else if (*str=='*'&&*(str+1)=='*'&&*(str+2)=='*'){
					o->t=SLL_NODE_TYPE_INLINE_FUNC;
					o->dt.fn.sc=c_dt->_n_sc_id;
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
				else if (*str=='-'&&*(str+1)=='-'&&*(str+2)=='-'){
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
					fl|=EXTRA_COMPILATION_DATA_IMPORT;
				}
			}
			if ((o->t>=SLL_NODE_TYPE_FOR&&o->t<=SLL_NODE_TYPE_LOOP)||o->t==SLL_NODE_TYPE_FUNC||o->t==SLL_NODE_TYPE_INLINE_FUNC||(o->t>=SLL_NODE_TYPE_FOR_ARRAY&&o->t<=SLL_NODE_TYPE_WHILE_MAP)){
				n_l_sc.l_sc=c_dt->_n_sc_id;
				n_l_sc.ml=(n_l_sc.l_sc>>6)+1;
				n_l_sc.m=sll_zero_allocate(n_l_sc.ml*sizeof(bitmap_t));
				sll_copy_data(l_sc->m,l_sc->ml*sizeof(bitmap_t),n_l_sc.m);
				n_l_sc.m[n_l_sc.l_sc>>6]|=1ull<<(n_l_sc.l_sc&63);
				c_dt->_n_sc_id++;
				l_sc=&n_l_sc;
			}
		}
		else if (c=='('||c=='{'||c=='['||c=='<'){
			if (!o){
				o=_acquire_next_node(c_dt);
				switch (c){
					case '(':
						o->t=NODE_TYPE_UNKNOWN;
						break;
					case '[':
						o->t=SLL_NODE_TYPE_ARRAY;
						break;
					case '<':
						o->t=SLL_NODE_TYPE_MAP;
						break;
					default:
						o->t=SLL_NODE_TYPE_OPERATION_LIST;
						break;
				}
			}
			else{
				if (o->t==SLL_NODE_TYPE_ASSIGN&&ac==1&&e_c_dt->nv_dt->sz){
					sll_node_t* id=o+1;
					while (id->t==SLL_NODE_TYPE_NOP||id->t==SLL_NODE_TYPE_DBG||id->t==SLL_NODE_TYPE_CHANGE_STACK){
						id=(id->t==SLL_NODE_TYPE_CHANGE_STACK?id->dt._p:id+1);
					}
					if (id->t==SLL_NODE_TYPE_IDENTIFIER&&id==*(e_c_dt->nv_dt->dt+e_c_dt->nv_dt->sz-1)){
						e_c_dt->nv_dt->sz--;
					}
				}
				if (o->t==NODE_TYPE_UNKNOWN){
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
				}
				extra_compilation_data_t n_e_c_dt={
					*l_sc,
					e_c_dt->i_ft,
					e_c_dt->ir,
					e_c_dt->nv_dt,
					SLL_MAX_STRING_INDEX
				};
				if (o->t==SLL_NODE_TYPE_ASSIGN&&ac==1){
					sll_node_t* a=o+1;
					while (a->t==SLL_NODE_TYPE_NOP||a->t==SLL_NODE_TYPE_DBG||a->t==SLL_NODE_TYPE_CHANGE_STACK){
						a=(a->t==SLL_NODE_TYPE_CHANGE_STACK?a->dt._p:a+1);
					}
					if (a->t==SLL_NODE_TYPE_IDENTIFIER){
						n_e_c_dt.a_nm=(SLL_IDENTIFIER_GET_ARRAY_ID(a->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH?c_dt->idt.il+SLL_IDENTIFIER_GET_ARRAY_INDEX(a->dt.id):c_dt->idt.s[SLL_IDENTIFIER_GET_ARRAY_ID(a->dt.id)].dt+SLL_IDENTIFIER_GET_ARRAY_INDEX(a->dt.id))->i;
					}
				}
				_read_object_internal(rf,c_dt,c,&n_e_c_dt);
				if (o->t==SLL_NODE_TYPE_ARRAY){
					al++;
				}
				else if (o->t==SLL_NODE_TYPE_MAP){
					ml++;
				}
				else{
					ac++;
				}
			}
			c=sll_file_read_char(rf);
		}
		else if (c==')'||c=='}'||c==']'||c=='>'){
			break;
		}
		else if (c=='|'&&sll_file_peek_char(rf)=='#'){
			c=sll_file_read_char(rf);
			SLL_ASSERT(c=='#');
			sll_read_char_t lc=c;
			do{
				lc=c;
				c=sll_file_read_char(rf);
			} while (c!=SLL_END_OF_DATA&&(c!='|'||lc!='#'));
			c=sll_file_read_char(rf);
		}
		else{
			sll_node_t* arg=_acquire_next_node(c_dt);
			arg->t=SLL_NODE_TYPE_NOP;
			sll_char_t rewind_bf[255];
			sll_string_length_t rewind_bf_l=0;
			if (c=='\''){
				arg->t=SLL_NODE_TYPE_CHAR;
				arg->dt.c=0;
				unsigned int err=_read_single_char(rf,&(arg->dt.c));
				if (err!=1){
					if (err||arg->dt.c!='\''){
						sll_char_t tmp;
						do{
							err=_read_single_char(rf,&tmp);
							if (err==1){
								break;
							}
						} while (err||tmp!='\'');
					}
					c=sll_file_read_char(rf);
				}
			}
			else if (c=='"'){
				arg->t=SLL_NODE_TYPE_STRING;
				sll_string_t s;
				sll_string_create(0,&s);
				s.v=sll_memory_move(s.v,SLL_MEMORY_MOVE_DIRECTION_TO_STACK);
				while (1){
					sll_string_increase(&s,1);
					unsigned int err=_read_single_char(rf,s.v+s.l);
					if (err==1||(!err&&s.v[s.l]=='"')){
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
				sll_bool_t neg=0;
				if (c=='-'){
					PUSH_REWIND_CHAR('-');
					neg=1;
					c=sll_file_read_char(rf);
				}
				sll_integer_t v=0;
				unsigned int vc=0;
				sll_integer_t v2=0;
				if (c=='0'){
					PUSH_REWIND_CHAR('0');
					c=sll_file_read_char(rf);
					if (c==SLL_END_OF_DATA){
						goto _create_int;
					}
					PUSH_REWIND_CHAR(c);
					if (c=='x'||c=='X'){
						c=sll_file_read_char(rf);
						while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'&&c!=SLL_END_OF_DATA)){
							PUSH_REWIND_CHAR(c);
							if (c>96){
								c-=32;
							}
							if (c<48||(c>57&&c<65)||(c>70&&c!='_')){
								rewind_bf_l--;
								goto _parse_identifier;
							}
							if (c!='_'){
								ADD_DIGIT(16,(c>64?c-55:c-48));
							}
							c=sll_file_read_char(rf);
						};
					}
					else if (c=='o'||c=='O'){
						c=sll_file_read_char(rf);
						while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'&&c!=SLL_END_OF_DATA)){
							if (c<48||(c>55&&c!='_')){
								goto _parse_identifier;
							}
							PUSH_REWIND_CHAR(c);
							if (c!='_'){
								ADD_DIGIT(8,c-48);
							}
							c=sll_file_read_char(rf);
						};
					}
					else if (c=='b'||c=='B'){
						c=sll_file_read_char(rf);
						while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'&&c!=SLL_END_OF_DATA)){
							if (c!='0'&&c!='1'&&c!='_'){
								goto _parse_identifier;
							}
							PUSH_REWIND_CHAR(c);
							if (c!='_'){
								ADD_DIGIT(2,c-48);
							}
							c=sll_file_read_char(rf);
						};
					}
					if (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'&&c!='.'&&c!='e'&&c!='E'&&(c<48||c>57))){
						goto _parse_identifier;
					}
				}
				if (c>47&&c<58){
					while (1){
						PUSH_REWIND_CHAR(c);
						if (c!='_'){
							ADD_DIGIT(10,c-48);
						}
						c=sll_file_read_char(rf);
						if ((c>8&&c<14)||c==' '||c=='('||c==')'||c==';'||c=='<'||c=='>'||c=='['||c==']'||c=='{'||c=='}'||c=='.'||c=='e'||c=='E'||c==SLL_END_OF_DATA){
							break;
						}
						if (c<48||(c>57&&c!='_')){
							goto _parse_identifier;
						}
					}
				}
				if (c=='.'||c=='e'||c=='E'){
					sll_integer_t ex=0;
					if (c=='.'){
						while (1){
							PUSH_REWIND_CHAR(c);
							c=sll_file_read_char(rf);
							if ((c>8&&c<14)||c==' '||c=='('||c==')'||c==';'||c=='<'||c=='>'||c=='['||c==']'||c=='{'||c=='}'||c=='e'||c=='E'||c==SLL_END_OF_DATA){
								break;
							}
							if (c<48||(c>57&&c!='_')){
								goto _parse_identifier;
							}
							ex--;
							if (c!='_'){
								ADD_DIGIT(10,c-48);
							}
						}
					}
					if (c=='e'||c=='E'){
						PUSH_REWIND_CHAR(c);
						sll_bool_t neg_e=0;
						sll_integer_t ev=0;
						c=sll_file_read_char(rf);
						if (c=='-'){
							PUSH_REWIND_CHAR(c);
							neg_e=1;
							c=sll_file_read_char(rf);
						}
						else if (c=='+'){
							PUSH_REWIND_CHAR(c);
							c=sll_file_read_char(rf);
						}
						while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'&&c!=SLL_END_OF_DATA)){
							if (c<48||(c>57&&c!='_')){
								goto _parse_identifier;
							}
							PUSH_REWIND_CHAR(c);
							if (c!='_'){
								ev=ev*10+(c-48);
								if (ev>308){
									ev=308;
								}
							}
							c=sll_file_read_char(rf);
						}
						ex+=(neg_e?-ev:ev);
					}
					if (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}')){
						goto _parse_identifier;
					}
					arg->t=SLL_NODE_TYPE_FLOAT;
					if (vc<19){
						arg->dt.f=(sll_float_t)v;
					}
					else{
						SLL_ASSERT(vc<=38);
						arg->dt.f=((sll_float_t)v)*sll_api_math_pow(10,vc-19)+v2;
					}
					arg->dt.f*=sll_api_math_pow(10,(sll_float_t)ex);
					if (neg){
						arg->dt.f=-arg->dt.f;
					}
				}
				else{
_create_int:
					arg->t=SLL_NODE_TYPE_INT;
					if (vc<19){
						arg->dt.i=v;
					}
					else{
						SLL_ASSERT(vc<=38);
						vc-=19;
						arg->dt.i=v;
						sll_size_t b=10;
						do{
							if (vc&1){
								arg->dt.i*=b;
							}
							vc>>=1;
							b*=b;
						} while (vc);
						arg->dt.i+=v2;
					}
					if (neg){
						arg->dt.i=-arg->dt.i;
					}
				}
			}
			else{
_parse_identifier:
				if (!rewind_bf_l){
					while (c=='$'){
						c=sll_file_read_char(rf);
						if (c==SLL_END_OF_DATA){
							goto _return_node;
						}
					}
				}
				sll_string_t str;
				sll_string_create(255,&str);
				if (rewind_bf_l){
					sll_copy_data(rewind_bf,rewind_bf_l,str.v);
				}
				c=_read_identifier(&str,rewind_bf_l,rf,c);
				if (o&&(o->t!=SLL_NODE_TYPE_DECL||!(ac&1))&&c=='$'){
					do{
						c=sll_file_read_char(rf);
						if (c==SLL_END_OF_DATA){
							sll_free_string(&str);
							break;
						}
					} while (c=='$');
					if (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}')){
						sll_identifier_index_t ii=_get_var_index(c_dt,e_c_dt,l_sc,&str,arg,GET_VAR_INDEX_FLAG_UNKNOWN);
						if (ii==SLL_MAX_VARIABLE_INDEX){
							arg->t=SLL_NODE_TYPE_INT;
							arg->dt.i=0;
							while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}')){
								c=sll_file_read_char(rf);
								if (c==SLL_END_OF_DATA){
									break;
								}
							}
						}
						else{
							arg->t=SLL_NODE_TYPE_VAR_ACCESS;
							arg->dt.ac=1;
							sll_node_t* v=_acquire_next_node(c_dt);
							v->t=SLL_NODE_TYPE_IDENTIFIER;
							v->dt.id=ii;
							while (c!=SLL_END_OF_DATA){
								sll_string_create(255,&str);
								c=_read_identifier(&str,0,rf,c);
								arg->dt.ac++;
								v=_acquire_next_node(c_dt);
								v->t=SLL_NODE_TYPE_FIELD;
								v->dt.s=sll_add_string(&(c_dt->st),&str,1);
								if (c!='$'){
									break;
								}
								c=sll_file_read_char(rf);
							}
						}
						goto _identifier_end;
					}
				}
				if (o&&o->t==SLL_NODE_TYPE_DECL&&(ac&1)){
					sll_string_calculate_checksum(&str);
					arg->t=SLL_NODE_TYPE_FIELD;
					arg->dt.s=sll_add_string(&(c_dt->st),&str,1);
				}
				else if ((str.l==3&&sll_compare_data(str.v,"nil",3)==SLL_COMPARE_RESULT_EQUAL)||(str.l==5&&sll_compare_data(str.v,"false",5)==SLL_COMPARE_RESULT_EQUAL)){
					sll_free_string(&str);
					arg->t=SLL_NODE_TYPE_INT;
					arg->dt.i=0;
				}
				else if (str.l==4&&sll_compare_data(str.v,"true",4)==SLL_COMPARE_RESULT_EQUAL){
					sll_free_string(&str);
					arg->t=SLL_NODE_TYPE_INT;
					arg->dt.i=1;
				}
				else{
					sll_string_calculate_checksum(&str);
					arg->t=SLL_NODE_TYPE_IDENTIFIER;
					sll_char_t tmp[256];sll_copy_data(str.v,str.l+1,tmp);
					arg->dt.id=_get_var_index(c_dt,e_c_dt,l_sc,&str,arg,((!o||o->t!=SLL_NODE_TYPE_ASSIGN||ac)&&!(fl&EXTRA_COMPILATION_DATA_VARIABLE_DEFINITION)?GET_VAR_INDEX_FLAG_UNKNOWN:0)|(o&&o->t==SLL_NODE_TYPE_ASSIGN?GET_VAR_INDEX_FLAG_ASSIGN:(o&&o->t==SLL_NODE_TYPE_FUNC?GET_VAR_INDEX_FLAG_FUNC:0)));
					if (arg->dt.i==SLL_MAX_VARIABLE_INDEX){
						arg->t=SLL_NODE_TYPE_INT;
						arg->dt.i=0;
					}
				}
_identifier_end:;
			}
			if (arg->t==SLL_NODE_TYPE_NOP){
				continue;
			}
			if (!o){
				return;
			}
			if (o->t==SLL_NODE_TYPE_ARRAY){
				al++;
			}
			else if (o->t==SLL_NODE_TYPE_MAP){
				ml++;
			}
			else{
				if (o->t==SLL_NODE_TYPE_ASSIGN&&ac==1&&e_c_dt->nv_dt->sz){
					sll_node_t* id=o+1;
					while (id->t==SLL_NODE_TYPE_NOP||id->t==SLL_NODE_TYPE_DBG||id->t==SLL_NODE_TYPE_CHANGE_STACK){
						id=(id->t==SLL_NODE_TYPE_CHANGE_STACK?id->dt._p:id+1);
					}
					if (id->t==SLL_NODE_TYPE_IDENTIFIER&&id==*(e_c_dt->nv_dt->dt+e_c_dt->nv_dt->sz-1)){
						e_c_dt->nv_dt->sz--;
					}
				}
				ac++;
				if ((fl&EXTRA_COMPILATION_DATA_IMPORT)&&arg->t==SLL_NODE_TYPE_STRING){
					sll_compilation_data_t im=SLL_INIT_COMPILATION_DATA_STRUCT;
					if (!e_c_dt->ir(c_dt->st.dt+arg->dt.s,&im)){
						arg=_acquire_next_node(c_dt);
						arg->t=SLL_NODE_TYPE_INT;
						arg->dt.i=0;
						ac++;
						continue;
					}
					if (!im.h){
						sll_free_compilation_data(&im);
						arg=_acquire_next_node(c_dt);
						arg->t=SLL_NODE_TYPE_INT;
						arg->dt.i=0;
						ac++;
						continue;
					}
					sll_node_t* dbg=_acquire_next_node(c_dt);
					dbg->t=SLL_NODE_TYPE_DBG;
					dbg->dt.s=c_dt->fpt.l;
					ac++;
					import_module_data_t im_dt={
						.sm=sll_allocate(im.st.l*sizeof(sll_string_index_t)),
						.f_off=c_dt->ft.l,
						.eiml=im.et.l,
						.sc_off=c_dt->_n_sc_id,
						.c_dt=c_dt,
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
					for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
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
						sll_string_t tmp;
						sll_string_clone(im.st.dt+((SLL_IDENTIFIER_GET_ARRAY_ID(eii)==SLL_MAX_SHORT_IDENTIFIER_LENGTH?im.idt.il:im.idt.s[SLL_IDENTIFIER_GET_ARRAY_ID(eii)].dt)+SLL_IDENTIFIER_GET_ARRAY_INDEX(eii))->i,&tmp);
						(im_dt.eim+i)->b=_get_var_index(c_dt,e_c_dt,l_sc,&tmp,NULL,GET_VAR_INDEX_FLAG_ASSIGN);
					}
					sll_function_index_t j=c_dt->ft.l;
					c_dt->ft.l+=im.ft.l;
					c_dt->ft.dt=sll_reallocate(c_dt->ft.dt,c_dt->ft.l*sizeof(sll_function_t*));
					for (sll_function_index_t i=0;i<im.ft.l;i++){
						sll_function_t* f=*(im.ft.dt+i);
						sll_function_t* nf=sll_allocate(sizeof(sll_function_t)+SLL_FUNCTION_GET_ARGUMENT_COUNT(f)*sizeof(sll_identifier_index_t));
						nf->off=(sll_node_offset_t)(f->off+c_dt->_s.off);
						nf->al=f->al;
						nf->nm=(f->nm==SLL_MAX_STRING_INDEX?SLL_MAX_STRING_INDEX:*(im_dt.sm+f->nm));
						for (sll_arg_count_t k=0;k<SLL_FUNCTION_GET_ARGUMENT_COUNT(f);k++){
							nf->a[k]=SLL_IDENTIFIER_ADD_INDEX(f->a[k],im_dt.off[SLL_IDENTIFIER_GET_ARRAY_ID(f->a[k])]);
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
		return;
	}
_return_node:;
	if (o->t==SLL_NODE_TYPE_ASSIGN&&ac==1&&e_c_dt->nv_dt->sz){
		sll_node_t* id=o+1;
		while (id->t==SLL_NODE_TYPE_NOP||id->t==SLL_NODE_TYPE_DBG||id->t==SLL_NODE_TYPE_CHANGE_STACK){
			id=(id->t==SLL_NODE_TYPE_CHANGE_STACK?id->dt._p:id+1);
		}
		if (id->t==SLL_NODE_TYPE_IDENTIFIER&&id==*(e_c_dt->nv_dt->dt+e_c_dt->nv_dt->sz-1)){
			e_c_dt->nv_dt->sz--;
		}
	}
	if (o->t==SLL_NODE_TYPE_ARRAY){
		o->dt.al=al;
	}
	else if (o->t==SLL_NODE_TYPE_MAP){
		o->dt.ml=ml;
	}
	else if (o->t==NODE_TYPE_UNKNOWN){
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
		sll_bool_t va=0;
		sll_arg_count_t i=0;
		sll_node_t* arg=o+1;
		for (;i<ac;i++){
			while (arg->t==SLL_NODE_TYPE_NOP||arg->t==SLL_NODE_TYPE_DBG||arg->t==SLL_NODE_TYPE_CHANGE_STACK){
				arg=(arg->t==SLL_NODE_TYPE_CHANGE_STACK?arg->dt._p:arg+1);
			}
			if (arg->t!=SLL_NODE_TYPE_IDENTIFIER){
				break;
			}
			if (SLL_IDENTIFIER_GET_ARRAY_ID(arg->dt.id)>3){
				sll_string_t* nm=c_dt->st.dt+((SLL_IDENTIFIER_GET_ARRAY_ID(arg->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH?c_dt->idt.il:c_dt->idt.s[SLL_IDENTIFIER_GET_ARRAY_ID(arg->dt.id)].dt)+SLL_IDENTIFIER_GET_ARRAY_INDEX(arg->dt.id))->i;
				SLL_ASSERT(nm->l>4);
				if (nm->v[0]=='@'&&nm->v[1]=='@'&&nm->v[nm->l-2]=='@'&&nm->v[nm->l-1]=='@'){
					i++;
					va=1;
					break;
				}
			}
			arg++;
		}
		o->dt.fn.ac=ac-i;
		c_dt->ft.l++;
		c_dt->ft.dt=sll_reallocate(c_dt->ft.dt,c_dt->ft.l*sizeof(sll_function_t*));
		sll_function_t* f=sll_allocate(sizeof(sll_function_t)+i*sizeof(sll_identifier_index_t));
		f->off=f_off;
		f->al=(i<<1)|va;
		f->nm=e_c_dt->a_nm;
		arg=o+1;
		for (sll_arg_count_t j=0;j<i;j++){
			while (arg->t==SLL_NODE_TYPE_NOP||arg->t==SLL_NODE_TYPE_DBG||arg->t==SLL_NODE_TYPE_CHANGE_STACK){
				arg=(arg->t==SLL_NODE_TYPE_CHANGE_STACK?arg->dt._p:arg+1);
			}
			SLL_ASSERT(arg->t==SLL_NODE_TYPE_IDENTIFIER);
			f->a[j]=arg->dt.id;
			arg->t=SLL_NODE_TYPE_NOP;
			arg++;
		}
		*(c_dt->ft.dt+o->dt.fn.id)=f;
	}
	else if (o->t==SLL_NODE_TYPE_INTERNAL_FUNC){
		if (!ac){
			o->t=SLL_NODE_TYPE_OPERATION_LIST;
			o->dt.ac=0;
		}
		else{
			sll_node_t* n=o+1;
			while (n->t==SLL_NODE_TYPE_NOP||n->t==SLL_NODE_TYPE_DBG||n->t==SLL_NODE_TYPE_CHANGE_STACK){
				n=(n->t==SLL_NODE_TYPE_CHANGE_STACK?n->dt._p:n+1);
			}
			if (n->t!=SLL_NODE_TYPE_STRING){
				o->t=SLL_NODE_TYPE_OPERATION_LIST;
				o->dt.ac=ac;
			}
			else{
				o->dt.fn.id=sll_lookup_internal_function(e_c_dt->i_ft,(c_dt->st.dt+n->dt.s)->v);
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
	else if ((o->t>=SLL_NODE_TYPE_FOR&&o->t<=SLL_NODE_TYPE_LOOP)||(o->t>=SLL_NODE_TYPE_FOR_ARRAY&&o->t<=SLL_NODE_TYPE_WHILE_MAP)){
		o->dt.l.ac=ac;
	}
	else if (o->t==SLL_NODE_TYPE_DECL){
		o->dt.d.ac=ac;
		o->dt.d.nm=e_c_dt->a_nm;
	}
	else{
		o->dt.ac=ac;
	}
	if (n_l_sc.m){
		sll_deallocate(n_l_sc.m);
	}
}



__SLL_EXTERNAL void sll_parse_all_nodes(sll_file_t* rf,sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft,sll_import_resolver_t il){
	c_dt->h=_acquire_next_node(c_dt);
	c_dt->h->t=SLL_NODE_TYPE_OPERATION_LIST;
	c_dt->h->dt.ac=0;
	new_variable_data_t nv_dt={
		NULL,
		0
	};
	extra_compilation_data_t e_c_dt={
		{
			sll_allocate(sizeof(bitmap_t)),
			0,
			1
		},
		i_ft,
		il,
		&nv_dt,
		SLL_MAX_STRING_INDEX
	};
	e_c_dt.sc.m[0]=1;
	sll_read_char_t c=sll_file_read_char(rf);
	while (c!=SLL_END_OF_DATA){
		_read_object_internal(rf,c_dt,c,&e_c_dt);
		c_dt->h->dt.ac++;
		c=sll_file_read_char(rf);
	}
}
