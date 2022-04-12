#include <sll/_internal/common.h>
#include <sll/_internal/file.h>
#include <sll/_internal/parse.h>
#include <sll/_internal/stack.h>
#include <sll/_internal/static_string.h>
#include <sll/_internal/string.h>
#include <sll/allocator.h>
#include <sll/api/math.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/file.h>
#include <sll/generated/operator_parser.h>
#include <sll/identifier.h>
#include <sll/ift.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>



#define ADD_DIGIT(n,d) \
	do{ \
		if (n_st.cnt<19){ \
			n_st.hi=n_st.hi*n+d; \
		} \
		else if (n_st.cnt<38){ \
			n_st.lo=n_st.lo*n+d; \
		} \
		n_st.cnt++; \
	} while (0)
#define PUSH_REWIND_CHAR(c) \
	do{ \
		if (rewind_bf_l<255){ \
			rewind_bf[rewind_bf_l]=(sll_char_t)(c); \
			rewind_bf_l++; \
		} \
	} while (0)
#define UPDATE_IF_SCOPE \
	do{ \
		if (o&&o->t>=SLL_NODE_TYPE_IF&&o->t<=SLL_NODE_TYPE_SWITCH&&(ac&1)==1){ \
			SLL_ASSERT(n_l_sc.m); \
			sll_deallocate(n_l_sc.m); \
			n_l_sc.l_sc=sf->_n_sc_id; \
			n_l_sc.ml=(n_l_sc.l_sc>>6)+1; \
			n_l_sc.m=sll_zero_allocate_stack(n_l_sc.ml*sizeof(bitmap_t)); \
			sll_copy_data(e_c_dt->sc.m,e_c_dt->sc.ml*sizeof(bitmap_t),n_l_sc.m); \
			n_l_sc.m[n_l_sc.l_sc>>6]|=1ull<<(n_l_sc.l_sc&63); \
			sf->_n_sc_id++; \
		} \
	} while (0)



static __STATIC_STRING(_parse_file_str,"@@file@@");
static __STATIC_STRING(_parse_line_str,"@@line@@");
static __STATIC_STRING(_parse_nil_str,"nil");
static __STATIC_STRING(_parse_true_str,"true");
static __STATIC_STRING(_parse_false_str,"false");



static sll_identifier_index_t _get_var_index(sll_source_file_t* sf,const extra_compilation_data_t* e_c_dt,const scope_data_t* l_sc,sll_string_t* str,void* arg,unsigned int fl){
	SLL_ASSERT(str->l);
	sll_identifier_index_t o=SLL_MAX_VARIABLE_INDEX;
	if (str->l<=SLL_MAX_SHORT_IDENTIFIER_LENGTH){
		sll_identifier_list_t* k=sf->idt.s+str->l-1;
		sll_scope_t mx_sc=SLL_MAX_SCOPE;
		sll_identifier_index_t mx_i=0;
		if (!(fl&GET_VAR_INDEX_FLAG_FUNC)){
			for (sll_identifier_list_length_t i=0;i<k->l;i++){
				sll_identifier_t* si=k->dt+i;
				if (si->sc==SLL_MAX_SCOPE||l_sc->l_sc<si->sc||!STRING_EQUAL(sf->st.dt+SLL_IDENTIFIER_GET_STRING_INDEX(si),str)){
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
		SLL_IDENTIFIER_SET_STRING_INDEX(k->dt+k->l-1,sll_add_string(&(sf->st),str,1),fl&GET_VAR_INDEX_FLAG_TLS);
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
		for (sll_identifier_list_length_t i=0;i<sf->idt.ill;i++){
			sll_identifier_t* k=sf->idt.il+i;
			if (k->sc==SLL_MAX_SCOPE||l_sc->l_sc<k->sc||!STRING_EQUAL(sf->st.dt+SLL_IDENTIFIER_GET_STRING_INDEX(k),str)){
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
	sf->idt.ill++;
	sf->idt.il=sll_reallocate(sf->idt.il,sf->idt.ill*sizeof(sll_identifier_t));
	o=SLL_CREATE_IDENTIFIER(sf->idt.ill-1,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
	(sf->idt.il+sf->idt.ill-1)->sc=l_sc->l_sc;
	SLL_IDENTIFIER_SET_STRING_INDEX(sf->idt.il+sf->idt.ill-1,sll_add_string(&(sf->st),str,1),fl&GET_VAR_INDEX_FLAG_TLS);
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
	sll_read_char_t c=sll_file_read_char(rf,NULL);
	if (c==SLL_END_OF_DATA){
		return 1;
	}
	if (c!='\\'){
		*o=(sll_char_t)c;
		return 0;
	}
	c=sll_file_peek_char(rf,NULL);
	if (c==SLL_END_OF_DATA){
		return 1;
	}
	if (c=='\''||c=='"'||c=='\\'){
		*o=(sll_char_t)sll_file_read_char(rf,NULL);
		return 2;
	}
	else if (c=='x'){
		sll_file_read_char(rf,NULL);
		c=sll_file_read_char(rf,NULL);
		if (c==SLL_END_OF_DATA){
			return 1;
		}
		if (c>96){
			c-=32;
		}
		if (c<48||(c>57&&c<65)||c>70){
			*o=0;
			return 0;
		}
		sll_char_t v=(c>64?c-55:c-48);
		c=sll_file_read_char(rf,NULL);
		if (c==SLL_END_OF_DATA){
			*o=v;
			return 0;
		}
		if (c>96){
			c-=32;
		}
		if (c<48||(c>57&&c<65)||c>70){
			*o=v;
			return 0;
		}
		*o=(v<<4)|(c>64?c-55:c-48);
		return 0;
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
		*o='\\';
		return 0;
	}
	sll_file_read_char(rf,NULL);
	*o=(sll_char_t)c;
	return 0;
}



static sll_read_char_t _read_identifier(sll_string_t* str,sll_string_length_t sz,sll_file_t* rf,sll_read_char_t c){
	do{
		if (sz<255){
			str->v[sz]=(sll_char_t)c;
			sz++;
		}
		c=sll_file_read_char(rf,NULL);
		if (c==SLL_END_OF_DATA){
			break;
		}
	} while (c<9||(c>13&&c!='$'&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'));
	sll_string_decrease(str,sz);
	sll_string_calculate_checksum(str);
	return c;
}



static void _read_object_internal(sll_file_t* rf,sll_source_file_t* sf,sll_read_char_t c,const extra_compilation_data_t* e_c_dt){
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
					sll_node_t* dbg=_acquire_next_node(sf);
					dbg->t=SLL_NODE_TYPE_DBG;
					dbg->dt.s=SLL_MAX_STRING_INDEX;
					(*(e_c_dt->ln))++;
				}
				c=sll_file_read_char(rf,NULL);
			} while ((c>8&&c<14)||c==' ');
		}
		else if (c==';'){
			do{
				c=sll_file_read_char(rf,NULL);
			} while (c!='\n'&&c!=SLL_END_OF_DATA);
		}
		else if (c=='|'&&sll_file_peek_char(rf,NULL)=='#'){
			c=sll_file_read_char(rf,NULL);
			SLL_ASSERT(c=='#');
			sll_read_char_t lc=c;
			do{
				lc=c;
				c=sll_file_read_char(rf,NULL);
			} while (c!=SLL_END_OF_DATA&&(c!='|'||lc!='#'));
			c=sll_file_read_char(rf,NULL);
		}
		else if (o&&o->t==NODE_TYPE_UNKNOWN){
			c=_operator_parser(o,c,&fl,sf,rf);
			const scope_data_t* src=l_sc;
			if (o->t==SLL_NODE_TYPE_FUNC){
				f_off=sf->_s.off-1;
				src=e_c_dt->not_fn_sc;
				o->dt.fn.id=sf->ft.l;
			}
			if ((o->t>=SLL_NODE_TYPE_IF&&o->t<=SLL_NODE_TYPE_LOOP)||o->t==SLL_NODE_TYPE_FUNC||o->t==SLL_NODE_TYPE_INLINE_FUNC||(o->t>=SLL_NODE_TYPE_FOR_ARRAY&&o->t<=SLL_NODE_TYPE_WHILE_MAP)){
				n_l_sc.l_sc=sf->_n_sc_id;
				n_l_sc.ml=(n_l_sc.l_sc>>6)+1;
				n_l_sc.m=sll_zero_allocate_stack(n_l_sc.ml*sizeof(bitmap_t));
				sll_copy_data(src->m,src->ml*sizeof(bitmap_t),n_l_sc.m);
				n_l_sc.m[n_l_sc.l_sc>>6]|=1ull<<(n_l_sc.l_sc&63);
				sf->_n_sc_id++;
				l_sc=&n_l_sc;
			}
		}
		else if (c=='('||c=='{'||c=='['||c=='<'){
			if (!o){
				o=_acquire_next_node(sf);
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
				UPDATE_IF_SCOPE;
				if (o->t==NODE_TYPE_UNKNOWN){
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
				}
				extra_compilation_data_t n_e_c_dt={
					*l_sc,
					e_c_dt->c_dt,
					e_c_dt->i_ft,
					e_c_dt->ir,
					e_c_dt->nv_dt,
					NULL,
					SLL_MAX_STRING_INDEX,
					(o->t==SLL_NODE_TYPE_FUNC||e_c_dt->fn),
					e_c_dt->ln
				};
				n_e_c_dt.not_fn_sc=(n_e_c_dt.fn?e_c_dt->not_fn_sc:l_sc);
				if (o->t==SLL_NODE_TYPE_ASSIGN&&ac==1){
					sll_node_t* a=o+1;
					while (a->t==SLL_NODE_TYPE_NOP||a->t==SLL_NODE_TYPE_DBG||a->t==SLL_NODE_TYPE_CHANGE_STACK){
						a=(a->t==SLL_NODE_TYPE_CHANGE_STACK?a->dt._p:a+1);
					}
					if (a->t==SLL_NODE_TYPE_IDENTIFIER){
						n_e_c_dt.a_nm=SLL_IDENTIFIER_GET_STRING_INDEX(SLL_IDENTIFIER_GET_ARRAY_ID(a->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH?sf->idt.il+SLL_IDENTIFIER_GET_ARRAY_INDEX(a->dt.id):sf->idt.s[SLL_IDENTIFIER_GET_ARRAY_ID(a->dt.id)].dt+SLL_IDENTIFIER_GET_ARRAY_INDEX(a->dt.id));
					}
				}
				_read_object_internal(rf,sf,c,&n_e_c_dt);
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
			c=sll_file_read_char(rf,NULL);
		}
		else if (c==')'||c=='}'||c==']'||c=='>'){
			break;
		}
		else{
			UPDATE_IF_SCOPE;
			sll_node_t* arg=_acquire_next_node(sf);
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
					c=sll_file_read_char(rf,NULL);
				}
			}
			else if (c=='"'){
				arg->t=SLL_NODE_TYPE_STRING;
				sll_string_t s;
				STRING_INIT_STACK(&s);
				while (1){
					sll_string_increase(&s,1);
					unsigned int err=_read_single_char(rf,s.v+s.l);
					if (err==1||(!err&&s.v[s.l]=='"')){
						break;
					}
					s.l++;
				}
				SLL_STRING_FORMAT_PADDING(s.v,s.l);
				sll_allocator_move((void**)(&(s.v)),SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
				sll_string_calculate_checksum(&s);
				arg->dt.s=sll_add_string(&(sf->st),&s,1);
				c=sll_file_read_char(rf,NULL);
			}
			else if (c=='`'){
				arg->t=SLL_NODE_TYPE_STRING;
				sll_string_t s;
				STRING_INIT_STACK(&s);
				while (1){
					c=sll_file_read_char(rf,NULL);
					if (c=='`'||c==SLL_END_OF_DATA){
						break;
					}
					sll_string_increase(&s,1);
					s.v[s.l]=(sll_char_t)c;
					s.l++;
				}
				SLL_STRING_FORMAT_PADDING(s.v,s.l);
				sll_allocator_move((void**)(&(s.v)),SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
				sll_string_calculate_checksum(&s);
				arg->dt.s=sll_add_string(&(sf->st),&s,1);
				c=sll_file_read_char(rf,NULL);
			}
			else if ((c>47&&c<58)||c=='-'||c=='+'){
				sll_bool_t neg=0;
				if (c=='-'||c=='+'){
					PUSH_REWIND_CHAR(c);
					neg=(c=='-');
					c=sll_file_read_char(rf,NULL);
				}
				number_parser_state_t n_st={
					0,
					0,
					0,
					0
				};
				if (c=='0'){
					PUSH_REWIND_CHAR('0');
					c=sll_file_read_char(rf,NULL);
					if (c==SLL_END_OF_DATA){
						goto _create_int;
					}
					PUSH_REWIND_CHAR(c);
					if (c=='x'||c=='X'){
						c=sll_file_read_char(rf,NULL);
						while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'&&c!='i'&&c!=SLL_END_OF_DATA)){
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
							c=sll_file_read_char(rf,NULL);
						};
					}
					else if (c=='o'||c=='O'){
						c=sll_file_read_char(rf,NULL);
						while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'&&c!='i'&&c!=SLL_END_OF_DATA)){
							if (c<48||(c>55&&c!='_')){
								goto _parse_identifier;
							}
							PUSH_REWIND_CHAR(c);
							if (c!='_'){
								ADD_DIGIT(8,c-48);
							}
							c=sll_file_read_char(rf,NULL);
						};
					}
					else if (c=='b'||c=='B'){
						c=sll_file_read_char(rf,NULL);
						while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'&&c!='i'&&c!=SLL_END_OF_DATA)){
							if (c!='0'&&c!='1'&&c!='_'){
								goto _parse_identifier;
							}
							PUSH_REWIND_CHAR(c);
							if (c!='_'){
								ADD_DIGIT(2,c-48);
							}
							c=sll_file_read_char(rf,NULL);
						};
					}
					if (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'&&c!='.'&&c!='e'&&c!='E'&&c!='i'&&(c<48||c>57))){
						goto _parse_identifier;
					}
				}
				if (c>47&&c<58){
					while (1){
						PUSH_REWIND_CHAR(c);
						if (c!='_'){
							ADD_DIGIT(10,c-48);
						}
						c=sll_file_read_char(rf,NULL);
						if ((c>8&&c<14)||c==' '||c=='('||c==')'||c==';'||c=='<'||c=='>'||c=='['||c==']'||c=='{'||c=='}'||c=='.'||c=='e'||c=='E'||c=='i'||c==SLL_END_OF_DATA){
							break;
						}
						if (c<48||(c>57&&c!='_')){
							goto _parse_identifier;
						}
					}
				}
				if (c=='.'||c=='e'||c=='E'||c=='i'){
					if (c=='.'){
						while (1){
							PUSH_REWIND_CHAR(c);
							c=sll_file_read_char(rf,NULL);
							if ((c>8&&c<14)||c==' '||c=='('||c==')'||c==';'||c=='<'||c=='>'||c=='['||c==']'||c=='{'||c=='}'||c=='e'||c=='E'||c=='+'||c=='-'||c=='i'||c==SLL_END_OF_DATA){
								break;
							}
							if (c<48||(c>57&&c!='_')){
								goto _parse_identifier;
							}
							n_st.exp--;
							if (c!='_'){
								ADD_DIGIT(10,c-48);
							}
						}
					}
					if (c=='e'||c=='E'){
						PUSH_REWIND_CHAR(c);
						sll_bool_t neg_e=0;
						sll_integer_t ev=0;
						c=sll_file_read_char(rf,NULL);
						if (c=='-'){
							PUSH_REWIND_CHAR(c);
							neg_e=1;
							c=sll_file_read_char(rf,NULL);
						}
						else if (c=='+'){
							PUSH_REWIND_CHAR(c);
							c=sll_file_read_char(rf,NULL);
						}
						while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'&&c!='+'&&c!='-'&&c!='i'&&c!=SLL_END_OF_DATA)){
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
							c=sll_file_read_char(rf,NULL);
						}
						n_st.exp+=(neg_e?-ev:ev);
					}
					if (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'&&c!='+'&&c!='-'&&c!='i')){
						goto _parse_identifier;
					}
					sll_float_t real=(sll_float_t)n_st.hi;
					if (n_st.cnt>=19){
						SLL_ASSERT(n_st.cnt<=38);
						real=real*sll_api_math_pow(10,n_st.cnt-19)+n_st.lo;
					}
					real*=sll_api_math_pow(10,(sll_float_t)n_st.exp);
					if (neg){
						real=-real;
					}
					if (c=='+'||c=='-'){
						neg=(c=='-');
						PUSH_REWIND_CHAR(c);
						c=sll_file_read_char(rf,NULL);
						if (c<48||c>57){
							goto _parse_identifier;
						}
						n_st.hi=0;
						n_st.lo=0;
						n_st.exp=0;
						n_st.cnt=0;
						while (1){
							PUSH_REWIND_CHAR(c);
							if (c!='_'){
								ADD_DIGIT(10,c-48);
							}
							c=sll_file_read_char(rf,NULL);
							if ((c>8&&c<14)||c==' '||c=='('||c==')'||c==';'||c=='<'||c=='>'||c=='['||c==']'||c=='{'||c=='}'||c=='.'||c=='e'||c=='E'||c=='i'||c==SLL_END_OF_DATA){
								break;
							}
							if (c<48||(c>57&&c!='_')){
								goto _parse_identifier;
							}
						}
						if (c=='.'){
							while (1){
								PUSH_REWIND_CHAR(c);
								c=sll_file_read_char(rf,NULL);
								if ((c>8&&c<14)||c==' '||c=='('||c==')'||c==';'||c=='<'||c=='>'||c=='['||c==']'||c=='{'||c=='}'||c=='e'||c=='E'||c=='i'||c==SLL_END_OF_DATA){
									break;
								}
								if (c<48||(c>57&&c!='_')){
									goto _parse_identifier;
								}
								n_st.exp--;
								if (c!='_'){
									ADD_DIGIT(10,c-48);
								}
							}
						}
						if (c=='e'||c=='E'){
							PUSH_REWIND_CHAR(c);
							sll_bool_t neg_e=0;
							sll_integer_t ev=0;
							c=sll_file_read_char(rf,NULL);
							if (c=='-'){
								PUSH_REWIND_CHAR(c);
								neg_e=1;
								c=sll_file_read_char(rf,NULL);
							}
							else if (c=='+'){
								PUSH_REWIND_CHAR(c);
								c=sll_file_read_char(rf,NULL);
							}
							while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'&&c!='i'&&c!=SLL_END_OF_DATA)){
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
								c=sll_file_read_char(rf,NULL);
							}
							n_st.exp+=(neg_e?-ev:ev);
						}
						if (c!='i'){
							goto _parse_identifier;
						}
						PUSH_REWIND_CHAR('i');
						c=sll_file_read_char(rf,NULL);
						if (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'&&c!='i'&&c!=SLL_END_OF_DATA)){
							goto _parse_identifier;
						}
						arg->t=SLL_NODE_TYPE_COMPLEX;
						sll_float_t imag=(sll_float_t)n_st.hi;
						if (n_st.cnt>=19){
							SLL_ASSERT(n_st.cnt<=38);
							imag=imag*sll_api_math_pow(10,n_st.cnt-19)+n_st.lo;
						}
						arg->dt.d.real=real;
						arg->dt.d.imag=(neg?-imag:imag)*sll_api_math_pow(10,(sll_float_t)n_st.exp);
					}
					else if (c=='i'){
						PUSH_REWIND_CHAR('i');
						c=sll_file_read_char(rf,NULL);
						if (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'&&c!='i'&&c!=SLL_END_OF_DATA)){
							goto _parse_identifier;
						}
						arg->t=SLL_NODE_TYPE_COMPLEX;
						arg->dt.d.real=0;
						arg->dt.d.imag=real;
					}
					else{
						arg->t=SLL_NODE_TYPE_FLOAT;
						arg->dt.f=real;
					}
				}
				else{
_create_int:
					arg->t=SLL_NODE_TYPE_INT;
					if (n_st.cnt<19){
						arg->dt.i=n_st.hi;
					}
					else{
						SLL_ASSERT(n_st.cnt<=38);
						n_st.cnt-=19;
						arg->dt.i=n_st.hi;
						sll_size_t b=10;
						do{
							if (n_st.cnt&1){
								arg->dt.i*=b;
							}
							n_st.cnt>>=1;
							b*=b;
						} while (n_st.cnt);
						arg->dt.i+=n_st.lo;
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
						c=sll_file_read_char(rf,NULL);
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
				if (o&&o->t==SLL_NODE_TYPE_DECL&&(ac&1)){
					arg->t=SLL_NODE_TYPE_FIELD;
					arg->dt.s=sll_add_string(&(sf->st),&str,1);
				}
				else if (STRING_EQUAL(&str,&_parse_file_str)){
					sll_free_string(&str);
					arg->t=SLL_NODE_TYPE_STRING;
					arg->dt.s=0;
				}
				else if (STRING_EQUAL(&str,&_parse_line_str)){
					sll_free_string(&str);
					arg->t=SLL_NODE_TYPE_INT;
					arg->dt.i=*(e_c_dt->ln);
				}
				else if (STRING_EQUAL(&str,&_parse_nil_str)||STRING_EQUAL(&str,&_parse_false_str)){
					sll_free_string(&str);
					arg->t=SLL_NODE_TYPE_INT;
					arg->dt.i=0;
				}
				else if (STRING_EQUAL(&str,&_parse_true_str)){
					sll_free_string(&str);
					arg->t=SLL_NODE_TYPE_INT;
					arg->dt.i=1;
				}
				else if (c=='$'){
					do{
						c=sll_file_read_char(rf,NULL);
						if (c==SLL_END_OF_DATA){
							sll_free_string(&str);
							break;
						}
					} while (c=='$');
					if (c>8&&(c<14||c==' '||c=='('||c==')'||c==';'||c=='<'||c=='>'||c=='['||c==']'||c=='{'||c=='}')){
						goto _normal_identifier;
					}
					sll_identifier_index_t ii=_get_var_index(sf,e_c_dt,l_sc,&str,arg,GET_VAR_INDEX_FLAG_UNKNOWN);
					if (ii==SLL_MAX_VARIABLE_INDEX){
						arg->t=SLL_NODE_TYPE_INT;
						arg->dt.i=0;
						while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}')){
							c=sll_file_read_char(rf,NULL);
							if (c==SLL_END_OF_DATA){
								break;
							}
						}
					}
					else{
						arg->t=SLL_NODE_TYPE_VAR_ACCESS;
						arg->dt.ac=1;
						sll_node_t* v=_acquire_next_node(sf);
						v->t=SLL_NODE_TYPE_IDENTIFIER;
						v->dt.id=ii;
						while (c!=SLL_END_OF_DATA){
							sll_string_create(255,&str);
							c=_read_identifier(&str,0,rf,c);
							arg->dt.ac++;
							v=_acquire_next_node(sf);
							v->t=SLL_NODE_TYPE_FIELD;
							v->dt.s=sll_add_string(&(sf->st),&str,1);
							if (c!='$'){
								break;
							}
							c=sll_file_read_char(rf,NULL);
						}
					}
				}
				else{
_normal_identifier:;
					unsigned int var_fl=((!o||o->t!=SLL_NODE_TYPE_ASSIGN||ac)&&!(fl&EXTRA_COMPILATION_DATA_VARIABLE_DEFINITION)?GET_VAR_INDEX_FLAG_UNKNOWN:0)|(o&&o->t==SLL_NODE_TYPE_ASSIGN?GET_VAR_INDEX_FLAG_ASSIGN:(o&&o->t==SLL_NODE_TYPE_FUNC?GET_VAR_INDEX_FLAG_FUNC:0));
					if (str.l&&str.v[0]=='!'){
						var_fl|=GET_VAR_INDEX_FLAG_TLS*(!e_c_dt->fn);
						for (sll_string_length_t i=0;i<str.l-1;i++){
							str.v[i]=str.v[i+1];
						}
						sll_string_decrease(&str,str.l-1);
						sll_string_calculate_checksum(&str);
					}
					if (str.l){
						arg->t=SLL_NODE_TYPE_IDENTIFIER;
						arg->dt.id=_get_var_index(sf,e_c_dt,l_sc,&str,arg,var_fl);
						if (arg->dt.i==SLL_MAX_VARIABLE_INDEX){
							arg->t=SLL_NODE_TYPE_INT;
							arg->dt.i=0;
						}
					}
					else{
						arg->t=SLL_NODE_TYPE_INT;
						arg->dt.i=0;
					}
				}
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
					if (!e_c_dt->ir(sf->st.dt+arg->dt.s,&im)){
						arg=_acquire_next_node(sf);
						arg->t=SLL_NODE_TYPE_INT;
						arg->dt.i=0;
						ac++;
						continue;
					}
					if (!(*(im.dt))->dt){
						if (im.l>1){
							SLL_UNIMPLEMENTED();
						}
						sll_free_compilation_data(&im);
						continue;
					}
					sll_source_file_index_t* if_m=sll_allocate_stack(e_c_dt->c_dt->l*sizeof(sll_source_file_index_t));
					*if_m=0;
					sll_source_file_index_t i=1;
					for (sll_source_file_index_t j=1;j<e_c_dt->c_dt->l;j++){
						sll_source_file_t* c_dt_s=*(e_c_dt->c_dt->dt+j);
						sll_source_file_index_t k=0;
						for (;k<im.l;k++){
							sll_source_file_t* im_s=*(im.dt+k);
							if (im_s->sz==c_dt_s->sz&&sll_compare_data(&(im_s->h),&(c_dt_s->h),sizeof(sll_sha256_data_t))==SLL_COMPARE_RESULT_EQUAL){
								break;
							}
						}
						if (k==im.l){
							*(e_c_dt->c_dt->dt+i)=c_dt_s;
							*(if_m+j)=i;
							i++;
						}
						else{
							sll_free_source_file(c_dt_s);
							sll_deallocate(c_dt_s);
							*(if_m+j)=k+e_c_dt->c_dt->l;
						}
					}
					for (sll_source_file_index_t j=1;j<e_c_dt->c_dt->l;j++){
						if (*(if_m+j)>=e_c_dt->c_dt->l){
							*(if_m+j)-=e_c_dt->c_dt->l-i;
						}
					}
					for (sll_source_file_index_t j=0;j<i;j++){
						sll_source_file_t* n_sf=*(e_c_dt->c_dt->dt+j);
						for (sll_import_index_t k=0;k<n_sf->it.l;k++){
							(*(n_sf->it.dt+k))->sfi=*(if_m+(*(n_sf->it.dt+k))->sfi);
						}
					}
					sll_deallocate(if_m);
					sf->it.l++;
					sf->it.dt=sll_reallocate(sf->it.dt,sf->it.l*sizeof(sll_import_file_t*));
					sll_source_file_t* im_sf=*(im.dt);
					sll_import_file_t* if_=sll_allocate(sizeof(sll_import_file_t)+im_sf->et.l*sizeof(sll_identifier_index_t));
					*(sf->it.dt+sf->it.l-1)=if_;
					e_c_dt->c_dt->l=i+im.l;
					e_c_dt->c_dt->dt=sll_reallocate(e_c_dt->c_dt->dt,e_c_dt->c_dt->l*sizeof(sll_source_file_t*));
					for (sll_source_file_index_t j=0;j<im.l;j++){
						sll_source_file_t* n_im_sf=*(im.dt+j);
						*(e_c_dt->c_dt->dt+i+j)=n_im_sf;
						for (sll_import_index_t k=0;k<n_im_sf->it.l;k++){
							(*(n_im_sf->it.dt+k))->sfi+=i;
						}
					}
					if_->sfi=i;
					if_->l=im_sf->et.l;
					for (sll_export_table_length_t j=0;j<im_sf->et.l;j++){
						sll_identifier_index_t ii=*(im_sf->et.dt+j);
						sll_string_t tmp;
						sll_string_clone(im_sf->st.dt+SLL_IDENTIFIER_GET_STRING_INDEX((SLL_IDENTIFIER_GET_ARRAY_ID(ii)==SLL_MAX_SHORT_IDENTIFIER_LENGTH?im_sf->idt.il:im_sf->idt.s[SLL_IDENTIFIER_GET_ARRAY_ID(ii)].dt)+SLL_IDENTIFIER_GET_ARRAY_INDEX(ii)),&tmp);
						if_->dt[j]=_get_var_index(sf,e_c_dt,l_sc,&tmp,NULL,GET_VAR_INDEX_FLAG_ASSIGN);
					}
					sll_deallocate(im.dt);
				}
				else if ((fl&EXTRA_COMPILATION_DATA_EXPORT)&&arg->t==SLL_NODE_TYPE_IDENTIFIER){
					for (sll_export_table_length_t j=0;j<sf->et.l;j++){
						if (*(sf->et.dt+j)==arg->dt.id){
							goto _skip_export;
						}
					}
					sf->et.l++;
					sf->et.dt=sll_reallocate(sf->et.dt,sf->et.l*sizeof(sll_identifier_index_t));
					*(sf->et.dt+sf->et.l-1)=arg->dt.id;
_skip_export:;
				}
			}
		}
	}
	if (!o){
		o=_acquire_next_node(sf);
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
		while (i<ac){
			while (arg->t==SLL_NODE_TYPE_NOP||arg->t==SLL_NODE_TYPE_DBG||arg->t==SLL_NODE_TYPE_CHANGE_STACK){
				arg=(arg->t==SLL_NODE_TYPE_CHANGE_STACK?arg->dt._p:arg+1);
			}
			if (arg->t!=SLL_NODE_TYPE_IDENTIFIER){
				break;
			}
			if (SLL_IDENTIFIER_GET_ARRAY_ID(arg->dt.id)>3){
				sll_string_t* nm=sf->st.dt+SLL_IDENTIFIER_GET_STRING_INDEX((SLL_IDENTIFIER_GET_ARRAY_ID(arg->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH?sf->idt.il:sf->idt.s[SLL_IDENTIFIER_GET_ARRAY_ID(arg->dt.id)].dt)+SLL_IDENTIFIER_GET_ARRAY_INDEX(arg->dt.id));
				SLL_ASSERT(nm->l>4);
				if (nm->v[0]=='@'&&nm->v[1]=='@'&&nm->v[nm->l-2]=='@'&&nm->v[nm->l-1]=='@'){
					i++;
					va=1;
					break;
				}
			}
			i++;
			arg++;
		}
		o->dt.fn.ac=ac-i;
		sf->ft.l++;
		sf->ft.dt=sll_reallocate(sf->ft.dt,sf->ft.l*sizeof(sll_function_t*));
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
		*(sf->ft.dt+o->dt.fn.id)=f;
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
				o->dt.fn.id=sll_lookup_internal_function(e_c_dt->i_ft,(sf->st.dt+n->dt.s)->v);
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
		o->dt.dc.ac=ac;
		o->dt.dc.nm=e_c_dt->a_nm;
	}
	else{
		o->dt.ac=ac;
	}
	if (n_l_sc.m){
		sll_deallocate(n_l_sc.m);
	}
}



__SLL_EXTERNAL void sll_parse_nodes(sll_file_t* rf,sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft,sll_import_resolver_t ir){
	sll_source_file_t* sf=*(c_dt->dt);
	SLL_ASSERT(c_dt->l==1&&!sf->dt);
	sf->dt=_acquire_next_node(sf);
	sf->dt->t=SLL_NODE_TYPE_OPERATION_LIST;
	sf->dt->dt.ac=0;
	new_variable_data_t nv_dt={
		NULL,
		0
	};
	sll_file_offset_t ln=1;
	extra_compilation_data_t e_c_dt={
		{
			sll_allocate(sizeof(bitmap_t)),
			0,
			1
		},
		c_dt,
		i_ft,
		ir,
		&nv_dt,
		NULL,
		SLL_MAX_STRING_INDEX,
		0,
		&ln
	};
	e_c_dt.not_fn_sc=&(e_c_dt.sc);
	e_c_dt.sc.m[0]=1;
	_file_start_hash(rf);
	sll_read_char_t c=sll_file_read_char(rf,NULL);
	while (c!=SLL_END_OF_DATA){
		_read_object_internal(rf,sf,c,&e_c_dt);
		sf->dt->dt.ac++;
		c=sll_file_read_char(rf,NULL);
	}
	_file_end_hash(rf);
	sf->sz=SLL_FILE_GET_OFFSET(rf);
	sf->h=rf->_h.h;
}
