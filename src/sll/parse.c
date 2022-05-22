#include <sll/_internal/common.h>
#include <sll/_internal/file.h>
#include <sll/_internal/parse.h>
#include <sll/_internal/stack.h>
#include <sll/_internal/static_string.h>
#include <sll/_internal/string.h>
#include <sll/allocator.h>
#include <sll/api/hash.h>
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
#include <math.h>



#define ADD_DIGIT(n,d) \
	do{ \
		if (n_st.digit_count<19){ \
			n_st.hi=n_st.hi*n+d; \
		} \
		else if (n_st.digit_count<38){ \
			n_st.lo=n_st.lo*n+d; \
		} \
		n_st.digit_count++; \
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
		if (o&&o->type>=SLL_NODE_TYPE_IF&&o->type<=SLL_NODE_TYPE_SWITCH&&(ac&1)==1){ \
			SLL_ASSERT(n_l_sc.data); \
			sll_deallocate(n_l_sc.data); \
			n_l_sc.scope=sf->_next_scope; \
			n_l_sc.length=(n_l_sc.scope>>6)+1; \
			n_l_sc.data=sll_zero_allocate_stack(n_l_sc.length*sizeof(bitmap_t)); \
			sll_copy_data(e_c_dt->scope.data,e_c_dt->scope.length*sizeof(bitmap_t),n_l_sc.data); \
			n_l_sc.data[n_l_sc.scope>>6]|=1ull<<(n_l_sc.scope&63); \
			sf->_next_scope++; \
		} \
	} while (0)



static __STATIC_STRING(_parse_file_str,"@@file@@");
static __STATIC_STRING(_parse_line_str,"@@line@@");
static __STATIC_STRING(_parse_nil_str,"nil");
static __STATIC_STRING(_parse_true_str,"true");
static __STATIC_STRING(_parse_false_str,"false");



static sll_identifier_index_t _get_var_index(sll_source_file_t* sf,const extra_compilation_data_t* e_c_dt,const scope_data_t* l_sc,sll_string_t* str,void* arg,unsigned int fl){
	SLL_ASSERT(str->length);
	sll_identifier_index_t o=SLL_MAX_IDENTIFIER_INDEX;
	if (str->length<=SLL_MAX_SHORT_IDENTIFIER_LENGTH){
		sll_identifier_list_t* k=sf->identifier_table.short_+str->length-1;
		sll_scope_t mx_sc=SLL_MAX_SCOPE;
		sll_identifier_index_t mx_i=0;
		if (!(fl&GET_VAR_INDEX_FLAG_FUNC)){
			for (sll_identifier_list_length_t i=0;i<k->length;i++){
				sll_identifier_t* si=k->data+i;
				if (si->scope==SLL_MAX_SCOPE||l_sc->scope<si->scope||!STRING_EQUAL(sf->string_table.data+SLL_IDENTIFIER_GET_STRING_INDEX(si),str)){
					continue;
				}
				if (si->scope==l_sc->scope){
					o=SLL_CREATE_IDENTIFIER(i,str->length-1);
					goto _check_new_var;
				}
				else if ((mx_sc==SLL_MAX_SCOPE||si->scope>mx_sc)&&(l_sc->data[si->scope>>6]&(1ull<<(si->scope&0x3f)))){
					mx_sc=si->scope;
					mx_i=SLL_CREATE_IDENTIFIER(i,str->length-1);
				}
			}
			if (mx_sc!=SLL_MAX_SCOPE){
				o=mx_i;
				goto _check_new_var;
			}
			if (fl&GET_VAR_INDEX_FLAG_UNKNOWN){
				sll_free_string(str);
				return SLL_MAX_IDENTIFIER_INDEX;
			}
		}
		k->length++;
		k->data=sll_reallocate(k->data,k->length*sizeof(sll_identifier_t));
		(k->data+k->length-1)->scope=l_sc->scope;
		o=SLL_CREATE_IDENTIFIER(k->length-1,str->length-1);
		SLL_IDENTIFIER_SET_STRING_INDEX(k->data+k->length-1,sll_add_string(&(sf->string_table),str,1),fl&GET_VAR_INDEX_FLAG_TLS);
		if ((fl&GET_VAR_INDEX_FLAG_ASSIGN)&&arg){
			e_c_dt->new_variable_data->length++;
			e_c_dt->new_variable_data->data=sll_reallocate(e_c_dt->new_variable_data->data,e_c_dt->new_variable_data->length*sizeof(sll_node_t*));
			*(e_c_dt->new_variable_data->data+e_c_dt->new_variable_data->length-1)=arg;
		}
		return o;
	}
	sll_scope_t mx_sc=SLL_MAX_SCOPE;
	sll_identifier_index_t mx_i=0;
	if (!(fl&GET_VAR_INDEX_FLAG_FUNC)){
		for (sll_identifier_list_length_t i=0;i<sf->identifier_table.long_data_length;i++){
			sll_identifier_t* k=sf->identifier_table.long_data+i;
			if (k->scope==SLL_MAX_SCOPE||l_sc->scope<k->scope||!STRING_EQUAL(sf->string_table.data+SLL_IDENTIFIER_GET_STRING_INDEX(k),str)){
				continue;
			}
			if (k->scope==l_sc->scope){
				o=SLL_CREATE_IDENTIFIER(i,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
				goto _check_new_var;
			}
			else if ((mx_sc==SLL_MAX_SCOPE||k->scope>mx_sc)&&(l_sc->data[k->scope>>6]&(1ull<<(k->scope&0x3f)))){
				mx_sc=k->scope;
				mx_i=SLL_CREATE_IDENTIFIER(i,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
			}
		}
		if (mx_sc!=SLL_MAX_SCOPE){
			o=mx_i;
			goto _check_new_var;
		}
		if (fl&GET_VAR_INDEX_FLAG_UNKNOWN){
			sll_free_string(str);
			return SLL_MAX_IDENTIFIER_INDEX;
		}
	}
	sf->identifier_table.long_data_length++;
	sf->identifier_table.long_data=sll_reallocate(sf->identifier_table.long_data,sf->identifier_table.long_data_length*sizeof(sll_identifier_t));
	o=SLL_CREATE_IDENTIFIER(sf->identifier_table.long_data_length-1,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
	(sf->identifier_table.long_data+sf->identifier_table.long_data_length-1)->scope=l_sc->scope;
	SLL_IDENTIFIER_SET_STRING_INDEX(sf->identifier_table.long_data+sf->identifier_table.long_data_length-1,sll_add_string(&(sf->string_table),str,1),fl&GET_VAR_INDEX_FLAG_TLS);
	if ((fl&GET_VAR_INDEX_FLAG_ASSIGN)&&arg){
		e_c_dt->new_variable_data->length++;
		e_c_dt->new_variable_data->data=sll_reallocate(e_c_dt->new_variable_data->data,e_c_dt->new_variable_data->length*sizeof(sll_node_t*));
		*(e_c_dt->new_variable_data->data+e_c_dt->new_variable_data->length-1)=arg;
	}
	return o;
_check_new_var:;
	sll_free_string(str);
	for (sll_arg_count_t i=0;i<e_c_dt->new_variable_data->length;i++){
		if ((*(e_c_dt->new_variable_data->data+i))->data.identifier_index==o){
			return SLL_MAX_IDENTIFIER_INDEX;
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
			str->data[sz]=(sll_char_t)c;
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
	const scope_data_t* l_sc=&(e_c_dt->scope);
	scope_data_t n_l_sc={
		NULL
	};
	sll_node_t* o=NULL;
	sll_arg_count_t ac=0;
	sll_array_length_t al=0;
	sll_map_length_t ml=0;
	sll_node_offset_t f_off=0;
	sll_string_index_t desc=SLL_MAX_STRING_INDEX;
	while (c!=SLL_END_OF_DATA){
		if ((c>8&&c<14)||c==' '){
			do{
				if (c=='\n'){
					sll_node_t* dbg=_acquire_next_node(sf);
					dbg->type=SLL_NODE_TYPE_DBG;
					dbg->data.string_index=SLL_MAX_STRING_INDEX;
					(*(e_c_dt->file_line))++;
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
		else if (o&&o->type==NODE_TYPE_UNKNOWN){
			c=_operator_parser(o,c,&fl,sf,rf);
			const scope_data_t* src=l_sc;
			if (o->type==SLL_NODE_TYPE_FUNC){
				f_off=sf->_stack.offset-1;
				src=e_c_dt->non_function_scope;
				o->data.function.function_index=sf->function_table.length;
			}
			if ((o->type>=SLL_NODE_TYPE_IF&&o->type<=SLL_NODE_TYPE_LOOP)||o->type==SLL_NODE_TYPE_FUNC||o->type==SLL_NODE_TYPE_INLINE_FUNC||(o->type>=SLL_NODE_TYPE_FOR_ARRAY&&o->type<=SLL_NODE_TYPE_WHILE_MAP)){
				n_l_sc.scope=sf->_next_scope;
				n_l_sc.length=(n_l_sc.scope>>6)+1;
				n_l_sc.data=sll_zero_allocate_stack(n_l_sc.length*sizeof(bitmap_t));
				sll_copy_data(src->data,src->length*sizeof(bitmap_t),n_l_sc.data);
				n_l_sc.data[n_l_sc.scope>>6]|=1ull<<(n_l_sc.scope&63);
				sf->_next_scope++;
				l_sc=&n_l_sc;
			}
		}
		else if (c=='('||c=='{'||c=='['||c=='<'){
			if (!o){
				o=_acquire_next_node(sf);
				switch (c){
					case '(':
						o->type=NODE_TYPE_UNKNOWN;
						break;
					case '[':
						o->type=SLL_NODE_TYPE_ARRAY;
						break;
					case '<':
						o->type=SLL_NODE_TYPE_MAP;
						break;
					default:
						o->type=SLL_NODE_TYPE_OPERATION_LIST;
						break;
				}
			}
			else{
				if (o->type==SLL_NODE_TYPE_ASSIGN&&ac==1&&e_c_dt->new_variable_data->length){
					sll_node_t* id=o+1;
					while (id->type==SLL_NODE_TYPE_NOP||id->type==SLL_NODE_TYPE_DBG||id->type==SLL_NODE_TYPE_CHANGE_STACK){
						id=(id->type==SLL_NODE_TYPE_CHANGE_STACK?id->data._next_node:id+1);
					}
					if (id->type==SLL_NODE_TYPE_IDENTIFIER&&id==*(e_c_dt->new_variable_data->data+e_c_dt->new_variable_data->length-1)){
						e_c_dt->new_variable_data->length--;
					}
				}
				UPDATE_IF_SCOPE;
				if (o->type==NODE_TYPE_UNKNOWN){
					o->type=SLL_NODE_TYPE_OPERATION_LIST;
				}
				extra_compilation_data_t n_e_c_dt={
					*l_sc,
					e_c_dt->compilation_data,
					e_c_dt->internal_function_table,
					e_c_dt->import_resolver,
					e_c_dt->new_variable_data,
					NULL,
					SLL_MAX_STRING_INDEX,
					(o->type==SLL_NODE_TYPE_FUNC||e_c_dt->is_function),
					e_c_dt->file_line
				};
				n_e_c_dt.non_function_scope=(n_e_c_dt.is_function?e_c_dt->non_function_scope:l_sc);
				if (o->type==SLL_NODE_TYPE_ASSIGN&&ac==1){
					sll_node_t* a=o+1;
					while (a->type==SLL_NODE_TYPE_NOP||a->type==SLL_NODE_TYPE_DBG||a->type==SLL_NODE_TYPE_CHANGE_STACK){
						a=(a->type==SLL_NODE_TYPE_CHANGE_STACK?a->data._next_node:a+1);
					}
					if (a->type==SLL_NODE_TYPE_IDENTIFIER){
						n_e_c_dt.variable_assignment_name_string_index=SLL_IDENTIFIER_GET_STRING_INDEX(SLL_IDENTIFIER_GET_ARRAY_ID(a->data.identifier_index)==SLL_MAX_SHORT_IDENTIFIER_LENGTH?sf->identifier_table.long_data+SLL_IDENTIFIER_GET_ARRAY_INDEX(a->data.identifier_index):sf->identifier_table.short_[SLL_IDENTIFIER_GET_ARRAY_ID(a->data.identifier_index)].data+SLL_IDENTIFIER_GET_ARRAY_INDEX(a->data.identifier_index));
					}
				}
				_read_object_internal(rf,sf,c,&n_e_c_dt);
				if (o->type==SLL_NODE_TYPE_ARRAY){
					al++;
				}
				else if (o->type==SLL_NODE_TYPE_MAP){
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
			arg->type=SLL_NODE_TYPE_NOP;
			sll_char_t rewind_bf[255];
			sll_string_length_t rewind_bf_l=0;
			if (c=='\''){
				arg->type=SLL_NODE_TYPE_CHAR;
				arg->data.char_=0;
				unsigned int err=_read_single_char(rf,&(arg->data.char_));
				if (err!=1){
					if (err||arg->data.char_!='\''){
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
				arg->type=SLL_NODE_TYPE_STRING;
				sll_string_t s;
				STRING_INIT_STACK(&s);
				while (1){
					sll_string_increase(&s,1);
					unsigned int err=_read_single_char(rf,s.data+s.length);
					if (err==1||(!err&&s.data[s.length]=='"')){
						break;
					}
					s.length++;
				}
				SLL_STRING_FORMAT_PADDING(s.data,s.length);
				sll_allocator_move((void**)(&(s.data)),SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
				sll_string_calculate_checksum(&s);
				arg->data.string_index=sll_add_string(&(sf->string_table),&s,1);
				if (e_c_dt->is_function){
					desc=arg->data.string_index;
				}
				c=sll_file_read_char(rf,NULL);
			}
			else if (c=='`'){
				arg->type=SLL_NODE_TYPE_STRING;
				sll_string_t s;
				STRING_INIT_STACK(&s);
				while (1){
					c=sll_file_read_char(rf,NULL);
					if (c==SLL_END_OF_DATA){
						break;
					}
					if (c=='`'){
						if (sll_file_peek_char(rf,NULL)!='`'){
							break;
						}
						c=sll_file_read_char(rf,NULL);
					}
					sll_string_increase(&s,1);
					s.data[s.length]=(sll_char_t)c;
					s.length++;
				}
				SLL_STRING_FORMAT_PADDING(s.data,s.length);
				sll_allocator_move((void**)(&(s.data)),SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
				sll_string_calculate_checksum(&s);
				arg->data.string_index=sll_add_string(&(sf->string_table),&s,1);
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
					if (n_st.digit_count>=19){
						SLL_ASSERT(n_st.digit_count<=38);
						real=real*pow(10,n_st.digit_count-19)+n_st.lo;
					}
					real*=pow(10,(sll_float_t)n_st.exp);
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
						n_st.digit_count=0;
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
						arg->type=SLL_NODE_TYPE_COMPLEX;
						sll_float_t imag=(sll_float_t)n_st.hi;
						if (n_st.digit_count>=19){
							SLL_ASSERT(n_st.digit_count<=38);
							imag=imag*pow(10,n_st.digit_count-19)+n_st.lo;
						}
						arg->data.complex_.real=real;
						arg->data.complex_.imag=(neg?-imag:imag)*pow(10,(sll_float_t)n_st.exp);
					}
					else if (c=='i'){
						PUSH_REWIND_CHAR('i');
						c=sll_file_read_char(rf,NULL);
						if (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'&&c!='i'&&c!=SLL_END_OF_DATA)){
							goto _parse_identifier;
						}
						arg->type=SLL_NODE_TYPE_COMPLEX;
						arg->data.complex_.real=0;
						arg->data.complex_.imag=real;
					}
					else{
						arg->type=SLL_NODE_TYPE_FLOAT;
						arg->data.float_=real;
					}
				}
				else{
_create_int:
					arg->type=SLL_NODE_TYPE_INT;
					if (n_st.digit_count<19){
						arg->data.int_=n_st.hi;
					}
					else{
						SLL_ASSERT(n_st.digit_count<=38);
						n_st.digit_count-=19;
						arg->data.int_=n_st.hi;
						sll_size_t b=10;
						do{
							if (n_st.digit_count&1){
								arg->data.int_*=b;
							}
							n_st.digit_count>>=1;
							b*=b;
						} while (n_st.digit_count);
						arg->data.int_+=n_st.lo;
					}
					if (neg){
						arg->data.int_=-arg->data.int_;
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
					sll_copy_data(rewind_bf,rewind_bf_l,str.data);
				}
				c=_read_identifier(&str,rewind_bf_l,rf,c);
				if (o&&o->type==SLL_NODE_TYPE_DECL&&(ac&1)){
					arg->type=SLL_NODE_TYPE_FIELD;
					arg->data.string_index=sll_add_string(&(sf->string_table),&str,1);
				}
				else if (STRING_EQUAL(&str,&_parse_file_str)){
					sll_free_string(&str);
					arg->type=SLL_NODE_TYPE_STRING;
					arg->data.string_index=0;
				}
				else if (STRING_EQUAL(&str,&_parse_line_str)){
					sll_free_string(&str);
					arg->type=SLL_NODE_TYPE_INT;
					arg->data.int_=*(e_c_dt->file_line);
				}
				else if (STRING_EQUAL(&str,&_parse_nil_str)||STRING_EQUAL(&str,&_parse_false_str)){
					sll_free_string(&str);
					arg->type=SLL_NODE_TYPE_INT;
					arg->data.int_=0;
				}
				else if (STRING_EQUAL(&str,&_parse_true_str)){
					sll_free_string(&str);
					arg->type=SLL_NODE_TYPE_INT;
					arg->data.int_=1;
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
					if (ii==SLL_MAX_IDENTIFIER_INDEX){
						arg->type=SLL_NODE_TYPE_INT;
						arg->data.int_=0;
						while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}')){
							c=sll_file_read_char(rf,NULL);
							if (c==SLL_END_OF_DATA){
								break;
							}
						}
					}
					else{
						arg->type=SLL_NODE_TYPE_VAR_ACCESS;
						arg->data.arg_count=1;
						sll_node_t* v=_acquire_next_node(sf);
						v->type=SLL_NODE_TYPE_IDENTIFIER;
						v->data.identifier_index=ii;
						while (c!=SLL_END_OF_DATA){
							sll_string_create(255,&str);
							c=_read_identifier(&str,0,rf,c);
							arg->data.arg_count++;
							v=_acquire_next_node(sf);
							v->type=SLL_NODE_TYPE_FIELD;
							v->data.string_index=sll_add_string(&(sf->string_table),&str,1);
							if (c!='$'){
								break;
							}
							c=sll_file_read_char(rf,NULL);
						}
					}
				}
				else{
_normal_identifier:;
					unsigned int var_fl=((!o||o->type!=SLL_NODE_TYPE_ASSIGN||ac)&&!(fl&EXTRA_COMPILATION_DATA_VARIABLE_DEFINITION)?GET_VAR_INDEX_FLAG_UNKNOWN:0)|(o&&o->type==SLL_NODE_TYPE_ASSIGN?GET_VAR_INDEX_FLAG_ASSIGN:(o&&o->type==SLL_NODE_TYPE_FUNC?GET_VAR_INDEX_FLAG_FUNC:0));
					if (str.length&&str.data[0]=='!'){
						var_fl|=GET_VAR_INDEX_FLAG_TLS*(!e_c_dt->is_function);
						for (sll_string_length_t i=0;i<str.length-1;i++){
							str.data[i]=str.data[i+1];
						}
						sll_string_decrease(&str,str.length-1);
						sll_string_calculate_checksum(&str);
					}
					if (str.length){
						arg->type=SLL_NODE_TYPE_IDENTIFIER;
						arg->data.identifier_index=_get_var_index(sf,e_c_dt,l_sc,&str,arg,var_fl);
						if (arg->data.identifier_index==SLL_MAX_IDENTIFIER_INDEX){
							arg->type=SLL_NODE_TYPE_INT;
							arg->data.int_=0;
						}
					}
					else{
						arg->type=SLL_NODE_TYPE_INT;
						arg->data.int_=0;
					}
				}
			}
			if (arg->type==SLL_NODE_TYPE_NOP){
				continue;
			}
			if (!o){
				return;
			}
			if (o->type==SLL_NODE_TYPE_ARRAY){
				al++;
			}
			else if (o->type==SLL_NODE_TYPE_MAP){
				ml++;
			}
			else{
				if (o->type==SLL_NODE_TYPE_ASSIGN&&ac==1&&e_c_dt->new_variable_data->length){
					sll_node_t* id=o+1;
					while (id->type==SLL_NODE_TYPE_NOP||id->type==SLL_NODE_TYPE_DBG||id->type==SLL_NODE_TYPE_CHANGE_STACK){
						id=(id->type==SLL_NODE_TYPE_CHANGE_STACK?id->data._next_node:id+1);
					}
					if (id->type==SLL_NODE_TYPE_IDENTIFIER&&id==*(e_c_dt->new_variable_data->data+e_c_dt->new_variable_data->length-1)){
						e_c_dt->new_variable_data->length--;
					}
				}
				ac++;
				if ((fl&EXTRA_COMPILATION_DATA_IMPORT)&&arg->type==SLL_NODE_TYPE_STRING){
					sll_compilation_data_t im=SLL_INIT_COMPILATION_DATA_STRUCT;
					if (!e_c_dt->import_resolver(sf->string_table.data+arg->data.string_index,&im)){
						arg=_acquire_next_node(sf);
						arg->type=SLL_NODE_TYPE_INT;
						arg->data.int_=0;
						ac++;
						continue;
					}
					if (!(*(im.data))->first_node){
						if (im.length>1){
							SLL_UNIMPLEMENTED();
						}
						sll_free_compilation_data(&im);
						continue;
					}
					sll_source_file_index_t* if_m=sll_allocate_stack(e_c_dt->compilation_data->length*sizeof(sll_source_file_index_t));
					*if_m=0;
					sll_source_file_index_t i=1;
					for (sll_source_file_index_t j=1;j<e_c_dt->compilation_data->length;j++){
						sll_source_file_t* c_dt_s=*(e_c_dt->compilation_data->data+j);
						sll_source_file_index_t k=0;
						for (;k<im.length;k++){
							sll_source_file_t* im_s=*(im.data+k);
							if (im_s->file_size==c_dt_s->file_size&&sll_compare_data(&(im_s->file_hash),&(c_dt_s->file_hash),sizeof(sll_sha256_data_t))==SLL_COMPARE_RESULT_EQUAL){
								break;
							}
						}
						if (k==im.length){
							*(e_c_dt->compilation_data->data+i)=c_dt_s;
							*(if_m+j)=i;
							i++;
						}
						else{
							sll_free_source_file(c_dt_s);
							sll_deallocate(c_dt_s);
							*(if_m+j)=k+e_c_dt->compilation_data->length;
						}
					}
					for (sll_source_file_index_t j=1;j<e_c_dt->compilation_data->length;j++){
						if (*(if_m+j)>=e_c_dt->compilation_data->length){
							*(if_m+j)-=e_c_dt->compilation_data->length-i;
						}
					}
					for (sll_source_file_index_t j=0;j<i;j++){
						sll_source_file_t* n_sf=*(e_c_dt->compilation_data->data+j);
						for (sll_import_index_t k=0;k<n_sf->import_table.length;k++){
							(*(n_sf->import_table.data+k))->source_file_index=*(if_m+(*(n_sf->import_table.data+k))->source_file_index);
						}
					}
					sll_deallocate(if_m);
					sf->import_table.length++;
					sf->import_table.data=sll_reallocate(sf->import_table.data,sf->import_table.length*sizeof(sll_import_file_t*));
					sll_source_file_t* im_sf=*(im.data);
					sll_import_file_t* if_=sll_allocate(sizeof(sll_import_file_t)+im_sf->export_table.length*sizeof(sll_identifier_index_t));
					*(sf->import_table.data+sf->import_table.length-1)=if_;
					e_c_dt->compilation_data->length=i+im.length;
					e_c_dt->compilation_data->data=sll_reallocate(e_c_dt->compilation_data->data,e_c_dt->compilation_data->length*sizeof(sll_source_file_t*));
					for (sll_source_file_index_t j=0;j<im.length;j++){
						sll_source_file_t* n_im_sf=*(im.data+j);
						*(e_c_dt->compilation_data->data+i+j)=n_im_sf;
						for (sll_import_index_t k=0;k<n_im_sf->import_table.length;k++){
							(*(n_im_sf->import_table.data+k))->source_file_index+=i;
						}
					}
					if_->source_file_index=i;
					if_->length=im_sf->export_table.length;
					for (sll_export_table_length_t j=0;j<im_sf->export_table.length;j++){
						sll_identifier_index_t ii=*(im_sf->export_table.data+j);
						sll_string_t tmp;
						sll_string_clone(im_sf->string_table.data+SLL_IDENTIFIER_GET_STRING_INDEX((SLL_IDENTIFIER_GET_ARRAY_ID(ii)==SLL_MAX_SHORT_IDENTIFIER_LENGTH?im_sf->identifier_table.long_data:im_sf->identifier_table.short_[SLL_IDENTIFIER_GET_ARRAY_ID(ii)].data)+SLL_IDENTIFIER_GET_ARRAY_INDEX(ii)),&tmp);
						if_->data[j]=_get_var_index(sf,e_c_dt,l_sc,&tmp,NULL,GET_VAR_INDEX_FLAG_ASSIGN);
					}
					sll_deallocate(im.data);
				}
				else if ((fl&EXTRA_COMPILATION_DATA_EXPORT)&&arg->type==SLL_NODE_TYPE_IDENTIFIER){
					for (sll_export_table_length_t j=0;j<sf->export_table.length;j++){
						if (*(sf->export_table.data+j)==arg->data.identifier_index){
							goto _skip_export;
						}
					}
					sf->export_table.length++;
					sf->export_table.data=sll_reallocate(sf->export_table.data,sf->export_table.length*sizeof(sll_identifier_index_t));
					*(sf->export_table.data+sf->export_table.length-1)=arg->data.identifier_index;
_skip_export:;
				}
			}
		}
	}
	if (!o){
		o=_acquire_next_node(sf);
		o->type=SLL_NODE_TYPE_INT;
		o->data.int_=0;
		return;
	}
_return_node:;
	if (o->type==SLL_NODE_TYPE_ASSIGN&&ac==1&&e_c_dt->new_variable_data->length){
		sll_node_t* id=o+1;
		while (id->type==SLL_NODE_TYPE_NOP||id->type==SLL_NODE_TYPE_DBG||id->type==SLL_NODE_TYPE_CHANGE_STACK){
			id=(id->type==SLL_NODE_TYPE_CHANGE_STACK?id->data._next_node:id+1);
		}
		if (id->type==SLL_NODE_TYPE_IDENTIFIER&&id==*(e_c_dt->new_variable_data->data+e_c_dt->new_variable_data->length-1)){
			e_c_dt->new_variable_data->length--;
		}
	}
	if (o->type==SLL_NODE_TYPE_ARRAY){
		o->data.array_length=al;
	}
	else if (o->type==SLL_NODE_TYPE_MAP){
		o->data.map_length=ml;
	}
	else if (o->type==NODE_TYPE_UNKNOWN){
		o->type=SLL_NODE_TYPE_INT;
		o->data.int_=0;
	}
	else if (o->type==SLL_NODE_TYPE_ASSIGN){
		o->data.arg_count=ac;
		if (ac<2){
			o->type=SLL_NODE_TYPE_OPERATION_LIST;
		}
	}
	else if (o->type==SLL_NODE_TYPE_FUNC){
		sll_bool_t va=0;
		sll_arg_count_t i=0;
		sll_node_t* arg=o+1;
		while (i<ac){
			while (arg->type==SLL_NODE_TYPE_NOP||arg->type==SLL_NODE_TYPE_DBG||arg->type==SLL_NODE_TYPE_CHANGE_STACK){
				arg=(arg->type==SLL_NODE_TYPE_CHANGE_STACK?arg->data._next_node:arg+1);
			}
			if (arg->type!=SLL_NODE_TYPE_IDENTIFIER){
				break;
			}
			if (SLL_IDENTIFIER_GET_ARRAY_ID(arg->data.identifier_index)>3){
				sll_string_t* nm=sf->string_table.data+SLL_IDENTIFIER_GET_STRING_INDEX((SLL_IDENTIFIER_GET_ARRAY_ID(arg->data.identifier_index)==SLL_MAX_SHORT_IDENTIFIER_LENGTH?sf->identifier_table.long_data:sf->identifier_table.short_[SLL_IDENTIFIER_GET_ARRAY_ID(arg->data.identifier_index)].data)+SLL_IDENTIFIER_GET_ARRAY_INDEX(arg->data.identifier_index));
				SLL_ASSERT(nm->length>4);
				if (nm->data[0]=='@'&&nm->data[1]=='@'&&nm->data[nm->length-2]=='@'&&nm->data[nm->length-1]=='@'){
					i++;
					va=1;
					break;
				}
			}
			i++;
			arg++;
		}
		o->data.function.arg_count=ac-i;
		sf->function_table.length++;
		sf->function_table.data=sll_reallocate(sf->function_table.data,sf->function_table.length*sizeof(sll_function_t*));
		sll_function_t* f=sll_allocate(sizeof(sll_function_t)+i*sizeof(sll_identifier_index_t));
		f->offset=f_off;
		f->name_string_index=e_c_dt->variable_assignment_name_string_index;
		f->description_string_index=desc;
		f->arg_count=(i<<1)|va;
		arg=o+1;
		for (sll_arg_count_t j=0;j<i;j++){
			while (arg->type==SLL_NODE_TYPE_NOP||arg->type==SLL_NODE_TYPE_DBG||arg->type==SLL_NODE_TYPE_CHANGE_STACK){
				arg=(arg->type==SLL_NODE_TYPE_CHANGE_STACK?arg->data._next_node:arg+1);
			}
			SLL_ASSERT(arg->type==SLL_NODE_TYPE_IDENTIFIER);
			f->args[j]=arg->data.identifier_index;
			arg->type=SLL_NODE_TYPE_NOP;
			arg++;
		}
		*(sf->function_table.data+o->data.function.function_index)=f;
	}
	else if (o->type==SLL_NODE_TYPE_INTERNAL_FUNC){
		if (!ac){
			o->type=SLL_NODE_TYPE_OPERATION_LIST;
			o->data.arg_count=0;
		}
		else{
			sll_node_t* n=o+1;
			while (n->type==SLL_NODE_TYPE_NOP||n->type==SLL_NODE_TYPE_DBG||n->type==SLL_NODE_TYPE_CHANGE_STACK){
				n=(n->type==SLL_NODE_TYPE_CHANGE_STACK?n->data._next_node:n+1);
			}
			if (n->type!=SLL_NODE_TYPE_STRING){
				o->type=SLL_NODE_TYPE_OPERATION_LIST;
				o->data.arg_count=ac;
			}
			else{
				o->data.function.function_index=sll_lookup_internal_function(e_c_dt->internal_function_table,(sf->string_table.data+n->data.string_index)->data);
				if (o->data.function.function_index==SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX){
					o->type=SLL_NODE_TYPE_INTERNAL_FUNC_LOAD;
					o->data.arg_count=ac;
				}
				else{
					o->data.function.arg_count=ac-1;
					n->type=SLL_NODE_TYPE_NOP;
				}
			}
		}
	}
	else if ((o->type>=SLL_NODE_TYPE_FOR&&o->type<=SLL_NODE_TYPE_LOOP)||(o->type>=SLL_NODE_TYPE_FOR_ARRAY&&o->type<=SLL_NODE_TYPE_WHILE_MAP)){
		o->data.loop.arg_count=ac;
	}
	else if (o->type==SLL_NODE_TYPE_DECL){
		o->data.declaration.arg_count=ac;
		o->data.declaration.name_string_index=e_c_dt->variable_assignment_name_string_index;
	}
	else{
		o->data.arg_count=ac;
	}
	if (n_l_sc.data){
		sll_deallocate(n_l_sc.data);
	}
}



__SLL_EXTERNAL void sll_parse_nodes(sll_file_t* rf,sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft,sll_import_resolver_t ir){
	sll_source_file_t* sf=*(c_dt->data);
	SLL_ASSERT(c_dt->length==1&&!sf->first_node);
	sf->first_node=_acquire_next_node(sf);
	sf->first_node->type=SLL_NODE_TYPE_OPERATION_LIST;
	sf->first_node->data.arg_count=0;
	new_variable_data_t nv_dt={
		NULL,
		0
	};
	sll_file_offset_t ln=1;
	extra_compilation_data_t e_c_dt={
		{
			sll_allocate(sizeof(bitmap_t)),
			1,
			0
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
	e_c_dt.non_function_scope=&(e_c_dt.scope);
	e_c_dt.scope.data[0]=1;
	_file_start_hash(rf);
	sll_read_char_t c=sll_file_read_char(rf,NULL);
	while (c!=SLL_END_OF_DATA){
		_read_object_internal(rf,sf,c,&e_c_dt);
		sf->first_node->data.arg_count++;
		c=sll_file_read_char(rf,NULL);
	}
	sll_deallocate(e_c_dt.scope.data);
	sll_deallocate(nv_dt.data);
	_file_end_hash(rf);
	sf->file_size=SLL_FILE_GET_OFFSET(rf);
	sf->file_hash=rf->_hash.hash;
}
