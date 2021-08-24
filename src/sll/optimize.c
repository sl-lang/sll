#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/common.h>
#include <sll/constants.h>
#include <sll/core.h>
#include <sll/gc.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>



#define DECREASE_PARENT(p) \
	do{ \
		if (p){ \
			if ((p)->t==SLL_OBJECT_TYPE_FUNC||(p)->t==SLL_OBJECT_TYPE_INTERNAL_FUNC){ \
				(p)->dt.fn.ac--; \
			} \
			else if ((p)->t==SLL_OBJECT_TYPE_OPERATION_LIST){ \
				(p)->dt.sc--; \
			} \
			else{ \
				(p)->dt.ac--; \
			} \
		} \
	} while (0)
#define DECREASE_VARIABLE(o,o_dt) \
	do{ \
		if (SLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){ \
			((o_dt)->it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->c--; \
		} \
		else{ \
			((o_dt)->it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->c--; \
		} \
	} while (0)
#define GET_VARIABLE_INDEX(o,o_dt) (SLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH?((o_dt)->it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->v:((o_dt)->it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->v)
#define GET_VARIABLE_REF_COUNT(o,o_dt) (SLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH?((o_dt)->it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->c:((o_dt)->it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->c)



void _remove_up_to_end(sll_object_t* o,sll_object_offset_t off){
	sll_object_offset_t sz=sll_get_object_size(o);
	for (;off<sz;off++){
		(o+off)->t=SLL_OBJECT_TYPE_NOP;
	}
}



sll_string_index_t _create_print_string(optimizer_data_t* o_dt,const sll_char_t* a,const sll_char_t* b,sll_string_length_t al,sll_string_length_t bl,sll_string_checksum_t c){
	for (sll_string_index_t i=0;i<o_dt->c_dt->st.l;i++){
		sll_string_t* s=o_dt->c_dt->st.dt+i;
		if (s->c==c&&s->l==al+bl){
			for (sll_string_length_t j=0;j<al;j++){
				if (*(a+j)!=s->v[j]){
					goto _check_next_string;
				}
			}
			for (sll_string_length_t j=0;j<bl;j++){
				if (*(b+j)!=s->v[j+al]){
					goto _check_next_string;
				}
			}
			return i;
		}
_check_next_string:;
	}
	o_dt->c_dt->st.l++;
	o_dt->c_dt->st.dt=realloc(o_dt->c_dt->st.dt,o_dt->c_dt->st.l*sizeof(sll_string_t));
	sll_string_t* s=o_dt->c_dt->st.dt+o_dt->c_dt->st.l-1;
	sll_string_create(al+bl,s);
	s->c=c;
	memcpy(s->v,a,al);
	memcpy(s->v+al,b,bl);
	return o_dt->c_dt->st.l-1;
}



sll_object_offset_t _map_identifiers(const sll_object_t* o,const sll_compilation_data_t* c_dt,identifier_map_data_t* im){
	sll_object_offset_t eoff=0;
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA){
		eoff++;
		o++;
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
		case SLL_OBJECT_TYPE_CHAR:
		case SLL_OBJECT_TYPE_STRING:
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
			return eoff+1;
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_object_offset_t off=1;
				sll_array_length_t l=o->dt.al;
				while (l){
					l--;
					off+=_map_identifiers(o+off,c_dt,im);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_IDENTIFIER:
			{
				sll_identifier_index_t i=SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id);
				uint8_t j=SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id);
				if (j==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
					sll_identifier_t* id=c_dt->idt.il+i;
					if (im->l_sc!=id->sc){
						*(im->sc_vi+im->l_sc)=im->n_vi;
						im->l_sc=id->sc;
						if (*(im->sc_vi+im->l_sc)!=SLL_MAX_SCOPE){
							im->n_vi=*(im->sc_vi+im->l_sc);
						}
					}
					(im->l_im+i)->c++;
					if ((im->l_im+i)->v==SLL_MAX_VARIABLE_INDEX){
						(im->l_im+i)->v=im->n_vi;
						im->n_vi++;
						if (im->n_vi>im->vc){
							im->vc=im->n_vi;
						}
					}
				}
				else{
					sll_identifier_t* id=c_dt->idt.s[j].dt+i;
					if (im->l_sc!=id->sc){
						*(im->sc_vi+im->l_sc)=im->n_vi;
						im->l_sc=id->sc;
						if (*(im->sc_vi+im->l_sc)!=SLL_MAX_SCOPE){
							im->n_vi=*(im->sc_vi+im->l_sc);
						}
					}
					(im->s_im[j]+i)->c++;
					if ((im->s_im[j]+i)->v==SLL_MAX_VARIABLE_INDEX){
						(im->s_im[j]+i)->v=im->n_vi;
						im->n_vi++;
						if (im->n_vi>im->vc){
							im->vc=im->n_vi;
						}
					}
				}
				return eoff+1;
			}
		case SLL_OBJECT_TYPE_FUNC:
			return sll_get_object_size(o)+eoff;
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				sll_object_offset_t off=1;
				sll_arg_count_t l=o->dt.ac;
				while (l){
					l--;
					off+=_map_identifiers(o+off,c_dt,im);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_object_offset_t off=1;
				sll_statement_count_t l=o->dt.sc;
				while (l){
					l--;
					off+=_map_identifiers(o+off,c_dt,im);
				}
				return off+eoff;
			}
	}
	sll_object_offset_t off=1;
	sll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		off+=_map_identifiers(o+off,c_dt,im);
	}
	return off+eoff;
}



void _get_as_runtime_object(const sll_object_t* o,const optimizer_data_t* o_dt,sll_runtime_object_t* v){
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA){
		o++;
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_CHAR:
			v->t=SLL_RUNTIME_OBJECT_TYPE_CHAR;
			v->dt.c=o->dt.c;
			return;
		case SLL_OBJECT_TYPE_INT:
			v->t=SLL_RUNTIME_OBJECT_TYPE_INT;
			v->dt.i=o->dt.i;
			return;
		case SLL_OBJECT_TYPE_FLOAT:
			v->t=SLL_RUNTIME_OBJECT_TYPE_FLOAT;
			v->dt.f=o->dt.f;
			return;
		case SLL_OBJECT_TYPE_STRING:
			v->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
			v->dt.s=*(o_dt->c_dt->st.dt+o->dt.s);
			return;
		case SLL_OBJECT_TYPE_IDENTIFIER:
			*v=*(o_dt->v+GET_VARIABLE_INDEX(o,o_dt));
			return;
		case SLL_OBJECT_TYPE_PRINT:
		case SLL_OBJECT_TYPE_IF:
		case SLL_OBJECT_TYPE_FOR:
		case SLL_OBJECT_TYPE_RETURN:
		case SLL_OBJECT_TYPE_EXIT:
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			v->t=SLL_RUNTIME_OBJECT_TYPE_INT;
			v->dt.i=0;
			return;
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			v->t=SLL_RUNTIME_OBJECT_TYPE_INT;
			v->dt.i=(o->t==SLL_OBJECT_TYPE_FUNC?o->dt.fn.id+1:~((sll_integer_t)o->dt.fn.id));
			return;
		default:
			v->t=RUNTIME_OBJECT_TYPE_UNKNOWN;
	}
}



sll_object_offset_t _mark_loop_vars(const sll_object_t* o,optimizer_data_t* o_dt){
	sll_object_offset_t eoff=0;
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA){
		eoff++;
		o++;
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
		case SLL_OBJECT_TYPE_CHAR:
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
		case SLL_OBJECT_TYPE_STRING:
		case SLL_OBJECT_TYPE_IDENTIFIER:
			return eoff+1;
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_object_offset_t off=1;
				sll_array_length_t l=o->dt.al;
				while (l){
					l--;
					off+=_mark_loop_vars(o+off,o_dt);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_ASSIGN:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l>=2);
				sll_object_offset_t off=1;
				while ((o+off)->t==SLL_OBJECT_TYPE_NOP||(o+off)->t==SLL_OBJECT_TYPE_DEBUG_DATA){
					off++;
				}
				SLL_ASSERT((o+off)->t==SLL_OBJECT_TYPE_IDENTIFIER);
				sll_runtime_object_t tmp;
				_get_as_runtime_object(o+off+1,o_dt,&tmp);
				if (tmp.t==RUNTIME_OBJECT_TYPE_UNKNOWN){
					(o_dt->v+GET_VARIABLE_INDEX(o+off,o_dt))->t|=RUNTIME_OBJECT_CHANGE_IN_LOOP;
				}
				off++;
				l--;
				while (l){
					l--;
					off+=_mark_loop_vars(o+off,o_dt);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				sll_object_offset_t off=1;
				sll_arg_count_t l=o->dt.fn.ac;
				while (l){
					l--;
					off+=_mark_loop_vars(o+off,o_dt);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_object_offset_t off=1;
				sll_statement_count_t l=o->dt.sc;
				while (l){
					l--;
					off+=_mark_loop_vars(o+off,o_dt);
				}
				return off+eoff;
			}
	}
	sll_object_offset_t off=1;
	sll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		off+=_mark_loop_vars(o+off,o_dt);
	}
	return off+eoff;
}



uint8_t _get_cond_type(sll_object_t* o,optimizer_data_t* o_dt,uint8_t inv,uint8_t lv){
	sll_object_offset_t eoff=0;
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA){
		eoff++;
		o++;
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
			return COND_TYPE_UNKNOWN;
		case SLL_OBJECT_TYPE_CHAR:
			return (((!!o->dt.c)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
		case SLL_OBJECT_TYPE_INT:
			return (((!!o->dt.i)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
		case SLL_OBJECT_TYPE_FLOAT:
			return (((!!o->dt.f)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
		case SLL_OBJECT_TYPE_STRING:
			return (((!!(o_dt->c_dt->st.dt+o->dt.s)->l)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
		case SLL_OBJECT_TYPE_ARRAY:
			return (((!!o->dt.al)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
		case SLL_OBJECT_TYPE_IDENTIFIER:
			{
				sll_runtime_object_t* v=o_dt->v+GET_VARIABLE_INDEX(o,o_dt);
				DECREASE_VARIABLE(o,o_dt);
				if (lv||!(v->t&RUNTIME_OBJECT_CHANGE_IN_LOOP)){
					switch (SLL_RUNTIME_OBJECT_GET_TYPE(v)){
						case SLL_RUNTIME_OBJECT_TYPE_INT:
							return (((!!v->dt.i)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
						case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
							return (((!!v->dt.f)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
						case SLL_RUNTIME_OBJECT_TYPE_CHAR:
							return (((!!v->dt.c)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
						case SLL_RUNTIME_OBJECT_TYPE_STRING:
							return (((!!v->dt.s.l)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
						case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
							return (((!!v->dt.a.l)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
					}
				}
				return COND_TYPE_UNKNOWN;
			}
		case SLL_OBJECT_TYPE_PRINT:
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
		case SLL_OBJECT_TYPE_IF:
		case SLL_OBJECT_TYPE_FOR:
		case SLL_OBJECT_TYPE_WHILE:
		case SLL_OBJECT_TYPE_LOOP:
		case SLL_OBJECT_TYPE_RETURN:
		case SLL_OBJECT_TYPE_EXIT:
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			return (inv?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
		case SLL_OBJECT_TYPE_INPUT:
			SLL_UNIMPLEMENTED();
		case SLL_OBJECT_TYPE_AND:
			SLL_UNIMPLEMENTED();
		case SLL_OBJECT_TYPE_OR:
			SLL_UNIMPLEMENTED();
		case SLL_OBJECT_TYPE_NOT:
			SLL_ASSERT(o->dt.ac);
			return _get_cond_type(o+1,o_dt,!inv,lv);
		case SLL_OBJECT_TYPE_ASSIGN:
			SLL_ASSERT(o->dt.ac>=2);
			return _get_cond_type(o+sll_get_object_size(o+1)+1,o_dt,inv,lv);
		case SLL_OBJECT_TYPE_CALL:
		case SLL_OBJECT_TYPE_ADD:
		case SLL_OBJECT_TYPE_SUB:
		case SLL_OBJECT_TYPE_MULT:
		case SLL_OBJECT_TYPE_DIV:
		case SLL_OBJECT_TYPE_FLOOR_DIV:
		case SLL_OBJECT_TYPE_MOD:
		case SLL_OBJECT_TYPE_BIT_AND:
		case SLL_OBJECT_TYPE_BIT_OR:
		case SLL_OBJECT_TYPE_BIT_XOR:
		case SLL_OBJECT_TYPE_BIT_NOT:
		case SLL_OBJECT_TYPE_LESS:
		case SLL_OBJECT_TYPE_LESS_EQUAL:
		case SLL_OBJECT_TYPE_EQUAL:
		case SLL_OBJECT_TYPE_NOT_EQUAL:
		case SLL_OBJECT_TYPE_MORE:
		case SLL_OBJECT_TYPE_MORE_EQUAL:
			return COND_TYPE_UNKNOWN;
		default:
			SLL_UNREACHABLE();
			return COND_TYPE_UNKNOWN;
	}
}



sll_object_offset_t _optimize(sll_object_t* o,sll_object_t* p,optimizer_data_t* o_dt,uint8_t fl){
	sll_object_offset_t eoff=0;
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA){
		eoff++;
		o++;
	}
	SLL_ASSERT(!o_dt->rm);
	if (o_dt->vi!=SLL_MAX_VARIABLE_INDEX){
		_get_as_runtime_object(o,o_dt,o_dt->v+o_dt->vi);
		o_dt->vi=SLL_MAX_VARIABLE_INDEX;
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
			return eoff+1;
		case SLL_OBJECT_TYPE_CHAR:
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
		case SLL_OBJECT_TYPE_STRING:
			if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
				DECREASE_PARENT(p);
				o->t=SLL_OBJECT_TYPE_NOP;
			}
			return eoff+1;
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_object_offset_t off=1;
				sll_array_length_t l=o->dt.al;
				while (l){
					l--;
					off+=_optimize(o+off,NULL,o_dt,(fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG)|OPTIMIZER_FLAG_ARGUMENT);
					if (o_dt->rm){
						_remove_up_to_end(o,off);
						o->dt.al-=l;
						break;
					}
				}
				if (!o->dt.al){
					if (fl&OPTIMIZER_FLAG_ARGUMENT){
						o->t=SLL_OBJECT_TYPE_INT;
						o->dt.i=0;
					}
					else{
						o->t=SLL_OBJECT_TYPE_NOP;
						DECREASE_PARENT(p);
					}
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_IDENTIFIER:
			if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
				DECREASE_PARENT(p);
				DECREASE_VARIABLE(o,o_dt);
				o->t=SLL_OBJECT_TYPE_NOP;
			}
			else{
				sll_runtime_object_t* v=o_dt->v+GET_VARIABLE_INDEX(o,o_dt);
				if (SLL_RUNTIME_OBJECT_GET_TYPE(v)!=RUNTIME_OBJECT_TYPE_UNKNOWN&&(!(v->t&RUNTIME_OBJECT_CHANGE_IN_LOOP)||(fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG))){
					DECREASE_VARIABLE(o,o_dt);
					switch (SLL_RUNTIME_OBJECT_GET_TYPE(v)){
						case SLL_RUNTIME_OBJECT_TYPE_INT:
							o->t=SLL_OBJECT_TYPE_INT;
							o->dt.i=v->dt.i;
							break;
						case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
							o->t=SLL_OBJECT_TYPE_FLOAT;
							o->dt.f=v->dt.f;
							break;
						case SLL_RUNTIME_OBJECT_TYPE_CHAR:
							o->t=SLL_OBJECT_TYPE_CHAR;
							o->dt.c=v->dt.c;
							break;
						case SLL_RUNTIME_OBJECT_TYPE_STRING:
							o->t=SLL_OBJECT_TYPE_STRING;
							o->dt.s=0;
							while (!SLL_SAME_STRING(o_dt->c_dt->st.dt+o->dt.s,&(v->dt.s))){
								o->dt.s++;
							}
							break;
						case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
							SLL_UNIMPLEMENTED();
						default:
							SLL_UNREACHABLE();
					}
				}
			}
			return eoff+1;
		case SLL_OBJECT_TYPE_PRINT:
			{
				sll_object_offset_t off=1;
				sll_arg_count_t l=o->dt.ac;
				sll_object_t* a=NULL;
				while (l){
					l--;
					sll_object_t* b=o+off;
					off+=_optimize(b,o,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
					if (o_dt->rm){
						_remove_up_to_end(o,off);
						o->dt.ac-=l;
						break;
					}
					while (b->t==SLL_OBJECT_TYPE_NOP||b->t==SLL_OBJECT_TYPE_DEBUG_DATA){
						b++;
					}
					if (b->t==SLL_OBJECT_TYPE_INT||b->t==SLL_OBJECT_TYPE_FLOAT){
						sll_runtime_object_t rt;
						if (b->t==SLL_OBJECT_TYPE_INT){
							rt.t=SLL_RUNTIME_OBJECT_TYPE_INT;
							rt.dt.i=b->dt.i;
						}
						else{
							rt.t=SLL_RUNTIME_OBJECT_TYPE_FLOAT;
							rt.dt.f=b->dt.f;
						}
						sll_string_t s;
						const sll_runtime_object_t* rt_p=&rt;
						sll_object_to_string(&rt_p,1,&s);
						b->t=SLL_OBJECT_TYPE_STRING;
						b->dt.s=sll_add_string(&(o_dt->c_dt->st),&s);
					}
					if (a){
						if (a->t==SLL_OBJECT_TYPE_CHAR&&b->t==SLL_OBJECT_TYPE_CHAR){
							a->t=SLL_OBJECT_TYPE_NOP;
							b->t=SLL_OBJECT_TYPE_STRING;
							b->dt.s=_create_print_string(o_dt,&(a->dt.c),&(b->dt.c),1,1,a->dt.c^b->dt.c);
							o->dt.ac--;
						}
						else if (a->t==SLL_OBJECT_TYPE_CHAR&&b->t==SLL_OBJECT_TYPE_STRING){
							sll_string_t* sb=o_dt->c_dt->st.dt+b->dt.s;
							a->t=SLL_OBJECT_TYPE_NOP;
							b->t=SLL_OBJECT_TYPE_STRING;
							b->dt.s=_create_print_string(o_dt,&(a->dt.c),sb->v,1,sb->l,a->dt.c^sb->c);
							o->dt.ac--;
						}
						else if (a->t==SLL_OBJECT_TYPE_STRING&&b->t==SLL_OBJECT_TYPE_CHAR){
							sll_string_t* sa=o_dt->c_dt->st.dt+a->dt.s;
							a->t=SLL_OBJECT_TYPE_NOP;
							b->t=SLL_OBJECT_TYPE_STRING;
							b->dt.s=_create_print_string(o_dt,sa->v,&(b->dt.c),sa->l,1,sa->c^b->dt.c);
							o->dt.ac--;
						}
						else if (a->t==SLL_OBJECT_TYPE_STRING&&b->t==SLL_OBJECT_TYPE_STRING){
							sll_string_t* sa=o_dt->c_dt->st.dt+a->dt.s;
							sll_string_t* sb=o_dt->c_dt->st.dt+b->dt.s;
							a->t=SLL_OBJECT_TYPE_NOP;
							b->t=SLL_OBJECT_TYPE_STRING;
							b->dt.s=_create_print_string(o_dt,sa->v,sb->v,sa->l,sb->l,sa->c^sb->c);
							o->dt.ac--;
						}
					}
					if (b->t==SLL_OBJECT_TYPE_STRING){
						sll_string_t* s=o_dt->c_dt->st.dt+b->dt.s;
						if (!s->l){
							o->dt.ac--;
							b->t=SLL_OBJECT_TYPE_NOP;
							continue;
						}
						else if (s->l==1){
							b->t=SLL_OBJECT_TYPE_CHAR;
							b->dt.c=s->v[0];
						}
					}
					a=b;
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_ASSIGN:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l>=2);
				sll_object_offset_t off=1;
				while ((o+off)->t==SLL_OBJECT_TYPE_NOP||(o+off)->t==SLL_OBJECT_TYPE_DEBUG_DATA){
					off++;
				}
				sll_object_t* id_o=o+off;
				SLL_ASSERT(id_o->t==SLL_OBJECT_TYPE_IDENTIFIER);
				DECREASE_VARIABLE(id_o,o_dt);
				o_dt->vi=GET_VARIABLE_INDEX(id_o,o_dt);
				off++;
				off+=_optimize(o+off,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
				if (o_dt->rm){
					o->t=SLL_OBJECT_TYPE_NOP;
					id_o->t=SLL_OBJECT_TYPE_NOP;
					_remove_up_to_end(o,off);
					return off+eoff;
				}
				l-=2;
				while (l){
					l--;
					off+=_optimize(o+off,o,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
					if (o_dt->rm){
						_remove_up_to_end(o,off);
						o->dt.ac-=l;
						break;
					}
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_FUNC:
			{
				sll_object_offset_t off=1;
				sll_arg_count_t l=o->dt.fn.ac;
				while (l){
					l--;
					off+=_optimize(o+off,o,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
					if (o_dt->rm){
						_remove_up_to_end(o,off);
						o->dt.fn.ac-=l;
						break;
					}
				}
				o_dt->rm=0;
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
					o->t=SLL_OBJECT_TYPE_OPERATION_LIST;
					o->dt.sc=o->dt.fn.ac;
					return _optimize(o,p,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG)+eoff;
				}
				sll_object_offset_t off=1;
				sll_arg_count_t l=o->dt.fn.ac;
				while (l){
					l--;
					off+=_optimize(o+off,o,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
					if (o_dt->rm){
						_remove_up_to_end(o,off);
						o->dt.fn.ac-=l;
						break;
					}
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_IF:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				if (l==1){
					o->t=SLL_OBJECT_TYPE_NOP;
					return _optimize(o+1,p,o_dt,fl)+eoff;
				}
				sll_object_offset_t off=1;
				if (l&1){
					l>>=1;
					while (l){
						l--;
						sll_object_t* cnd_o=o+off;
						off+=_optimize(o+off,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
						if (o_dt->rm){
							SLL_UNIMPLEMENTED();
						}
						uint8_t cnd=_get_cond_type(cnd_o,o_dt,0,0);
						if (cnd==COND_TYPE_ALWAYS_TRUE){
							if (o->dt.ac-(l<<1)==3){
								sll_object_offset_t sz=sll_get_object_size(o);
								o->t=SLL_OBJECT_TYPE_NOP;
								off+=_optimize(o+off,p,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
								while (off<sz){
									(o+off)->t=SLL_OBJECT_TYPE_NOP;
									off++;
								}
							}
							else{
								off+=_optimize(o+off,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
								o_dt->rm=0;
								_remove_up_to_end(o,off);
								o->dt.ac-=(l<<1)+2;
							}
							while (cnd_o->t==SLL_OBJECT_TYPE_NOP||cnd_o->t==SLL_OBJECT_TYPE_DEBUG_DATA){
								cnd_o++;
							}
							cnd_o->t=SLL_OBJECT_TYPE_NOP;
							return off+eoff;
						}
						else if (cnd==COND_TYPE_ALWAYS_FALSE){
							o->dt.ac-=2;
							if (o->dt.ac==1){
								o->t=SLL_OBJECT_TYPE_NOP;
							}
							_remove_up_to_end(cnd_o,0);
							_remove_up_to_end(o+off,0);
							continue;
						}
						off+=_optimize(o+off,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
						o_dt->rm=0;
					}
					if (o->t!=SLL_OBJECT_TYPE_NOP){
						off+=_optimize(o+off,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
						o_dt->rm=0;
					}
					else{
						off+=_optimize(o+off,p,o_dt,fl);
					}
				}
				else{
					l>>=1;
					while (l){
						l--;
						sll_object_t* cnd_o=o+off;
						off+=_optimize(cnd_o,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
						if (o_dt->rm){
							SLL_UNIMPLEMENTED();
						}
						uint8_t cnd=_get_cond_type(cnd_o,o_dt,0,0);
						if (cnd==COND_TYPE_ALWAYS_TRUE){
							if (o->dt.ac-(l<<1)==2){
								sll_object_offset_t sz=sll_get_object_size(o);
								o->t=SLL_OBJECT_TYPE_NOP;
								off+=_optimize(o+off,p,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
								while (off<sz){
									(o+off)->t=SLL_OBJECT_TYPE_NOP;
									off++;
								}
							}
							else{
								o_dt->rm=0;
								off+=_optimize(o+off,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
								_remove_up_to_end(o,off);
							}
							o->dt.ac-=(l<<1)+1;
							_remove_up_to_end(cnd_o,0);
							break;
						}
						else if (cnd==COND_TYPE_ALWAYS_FALSE){
							o->dt.ac-=2;
							if (!o->dt.ac){
								o->t=SLL_OBJECT_TYPE_NOP;
								DECREASE_PARENT(p);
							}
							sll_object_offset_t sz=sll_get_object_size(cnd_o);
							for (sll_object_offset_t i=0;i<sz;i++){
								(cnd_o+i)->t=SLL_OBJECT_TYPE_NOP;
							}
							sz=sll_get_object_size(o+off);
							for (sll_object_offset_t i=0;i<sz;i++){
								(o+off+i)->t=SLL_OBJECT_TYPE_NOP;
							}
							off+=sz;
							continue;
						}
						off+=_optimize(o+off,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
						o_dt->rm=0;
					}
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_FOR:
			{
				sll_arg_count_t l=o->dt.l.ac;
				SLL_ASSERT(l);
				if (l==1){
					o->t=SLL_OBJECT_TYPE_NOP;
					return _optimize(o+1,p,o_dt,fl)+eoff;
				}
				sll_object_offset_t off=_optimize(o+1,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT)+1;
				if (o_dt->rm){
					o->t=SLL_OBJECT_TYPE_NOP;
					_remove_up_to_end(o,off);
					return off+eoff;
				}
				sll_object_t* cnd_o=o+off;
				sll_object_offset_t off2=off;
				for (sll_arg_count_t i=1;i<l;i++){
					off2+=_mark_loop_vars(o+off2,o_dt);
				}
				sll_object_offset_t cnd_sz=_optimize(cnd_o,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
				off+=cnd_sz;
				sll_object_t* tmp=malloc(cnd_sz*sizeof(sll_object_t));
				for (sll_object_offset_t i=0;i<cnd_sz;i++){
					*(tmp+i)=*(cnd_o+i);
				}
				_optimize(tmp,NULL,o_dt,OPTIMIZER_FLAG_ARGUMENT|OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
				if (o_dt->rm){
					SLL_UNIMPLEMENTED();
				}
				uint8_t cnd=_get_cond_type(tmp,o_dt,1,1);
				free(tmp);
				if (cnd==COND_TYPE_ALWAYS_TRUE){
					SLL_UNIMPLEMENTED();
				}
				else if (cnd==COND_TYPE_ALWAYS_FALSE){
					o->t=SLL_OBJECT_TYPE_WHILE;
				}
				l-=2;
				while (l){
					l--;
					off+=_optimize(o+off,o,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
					if (o_dt->rm){
						_remove_up_to_end(o,off);
						o->dt.l.ac-=l;
						break;
					}
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_WHILE:
			SLL_UNIMPLEMENTED();
		case SLL_OBJECT_TYPE_LOOP:
			SLL_UNIMPLEMENTED();
		case SLL_OBJECT_TYPE_LESS:
		case SLL_OBJECT_TYPE_LESS_EQUAL:
		case SLL_OBJECT_TYPE_EQUAL:
		case SLL_OBJECT_TYPE_NOT_EQUAL:
		case SLL_OBJECT_TYPE_MORE:
		case SLL_OBJECT_TYPE_MORE_EQUAL:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				sll_object_offset_t off=_optimize(o+1,o,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT)+1;
				if (o_dt->rm){
					_remove_up_to_end(o,off);
					o->t=SLL_OBJECT_TYPE_NOP;
					return off+eoff;
				}
				sll_runtime_object_t a;
				_get_as_runtime_object(o+1,o_dt,&a);
				l--;
				if (a.t!=RUNTIME_OBJECT_TYPE_UNKNOWN&&!(a.t&RUNTIME_OBJECT_CHANGE_IN_LOOP)){
					while (l){
						sll_object_t* arg=o+off;
						off+=_optimize(o+off,o,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
						if (o_dt->rm){
							_remove_up_to_end(o,off);
							o->dt.ac-=l-1;
							return off+eoff;
						}
						sll_runtime_object_t b;
						_get_as_runtime_object(arg,o_dt,&b);
						if (b.t==RUNTIME_OBJECT_TYPE_UNKNOWN||(b.t&RUNTIME_OBJECT_CHANGE_IN_LOOP)){
							break;
						}
						l--;
						sll_compare_result_t cmp=sll_operator_compare(&a,&b);
						SLL_ASSERT(cmp!=SLL_COMPARE_RESULT_ERROR);
						if ((o->t==SLL_OBJECT_TYPE_LESS&&cmp==SLL_COMPARE_RESULT_BELOW)||(o->t==SLL_OBJECT_TYPE_LESS_EQUAL&&cmp!=SLL_COMPARE_RESULT_ABOVE)||(o->t==SLL_OBJECT_TYPE_EQUAL&&cmp==SLL_COMPARE_RESULT_EQUAL)||(o->t==SLL_OBJECT_TYPE_NOT_EQUAL&&cmp!=SLL_COMPARE_RESULT_EQUAL)||(o->t==SLL_OBJECT_TYPE_MORE&&cmp==SLL_COMPARE_RESULT_ABOVE)||(o->t==SLL_OBJECT_TYPE_MORE_EQUAL&&cmp!=SLL_COMPARE_RESULT_BELOW)){
							a=b;
							continue;
						}
						_remove_up_to_end(o,off);
						o->t=SLL_OBJECT_TYPE_OPERATION_LIST;
						o->dt.sc=o->dt.ac-l;
						return _optimize(o,p,o_dt,fl)+eoff;
					}
					if (!l){
						_remove_up_to_end(o,1);
						o->t=SLL_OBJECT_TYPE_INT;
						o->dt.i=1;
						return eoff+1;
					}
					l--;
				}
				while (l){
					l--;
					off+=_optimize(o+off,o,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
					if (o_dt->rm){
						_remove_up_to_end(o,off);
						o->dt.ac-=l;
						break;
					}
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_RETURN:
		case SLL_OBJECT_TYPE_EXIT:
			{
				sll_object_offset_t off=1;
				sll_arg_count_t l=o->dt.ac;
				while (l){
					l--;
					off+=_optimize(o+off,o,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
					if (o_dt->rm){
						_remove_up_to_end(o,off);
						o->dt.ac-=l;
						break;
					}
				}
				o_dt->rm=1;
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_object_offset_t off=1;
				sll_statement_count_t l=o->dt.sc;
				while (l){
					l--;
					off+=_optimize(o+off,o,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
					if (o_dt->rm){
						_remove_up_to_end(o,off);
						o->dt.sc-=l;
						break;
					}
				}
				if (!o->dt.sc){
					if (fl&OPTIMIZER_FLAG_ARGUMENT){
						o->t=SLL_OBJECT_TYPE_INT;
						o->dt.i=0;
					}
					else{
						o->t=SLL_OBJECT_TYPE_NOP;
						DECREASE_PARENT(p);
					}
				}
				else if (o->dt.sc==1){
					o->t=SLL_OBJECT_TYPE_NOP;
				}
				else if (p&&(p->t==SLL_OBJECT_TYPE_FUNC||p->t==SLL_OBJECT_TYPE_OPERATION_LIST)){
					p->dt.sc+=o->dt.sc-1;
					o->t=SLL_OBJECT_TYPE_NOP;
				}
				return off+eoff;
			}
	}
	sll_object_offset_t off=1;
	sll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		off+=_optimize(o+off,o,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
		if (o_dt->rm){
			_remove_up_to_end(o,off);
			o->dt.ac-=l;
			break;
		}
	}
	return off+eoff;
}



sll_object_offset_t _remap_vars(sll_object_t* o,sll_object_t* p,optimizer_data_t* o_dt){
	sll_object_offset_t eoff=0;
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA){
		eoff++;
		o++;
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
		case SLL_OBJECT_TYPE_CHAR:
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
		case SLL_OBJECT_TYPE_STRING:
			return eoff+1;
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_object_offset_t off=1;
				sll_array_length_t l=o->dt.al;
				while (l){
					l--;
					off+=_remap_vars(o+off,o,o_dt);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_IDENTIFIER:
			SLL_ASSERT(GET_VARIABLE_REF_COUNT(o,o_dt));
			if (SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
				o->dt.id=*(o_dt->im.l+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id));
			}
			else{
				o->dt.id=*(o_dt->im.s[SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id));
			}
			return eoff+1;
		case SLL_OBJECT_TYPE_ASSIGN:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l>=2);
				sll_object_offset_t off=1;
				while ((o+off)->t==SLL_OBJECT_TYPE_NOP||(o+off)->t==SLL_OBJECT_TYPE_DEBUG_DATA){
					off++;
				}
				SLL_ASSERT((o+off)->t==SLL_OBJECT_TYPE_IDENTIFIER);
				uint8_t rm=!GET_VARIABLE_REF_COUNT(o+off,o_dt);
				if (rm){
					o->t=SLL_OBJECT_TYPE_OPERATION_LIST;
					o->dt.sc=o->dt.ac-1;
					(o+off)->t=SLL_OBJECT_TYPE_NOP;
				}
				else{
					if (SLL_IDENTIFIER_GET_ARRAY_ID((o+off)->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
						(o+off)->dt.id=*(o_dt->im.l+SLL_IDENTIFIER_GET_ARRAY_INDEX((o+off)->dt.id));
					}
					else{
						(o+off)->dt.id=*(o_dt->im.s[SLL_IDENTIFIER_GET_ARRAY_ID((o+off)->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX((o+off)->dt.id));
					}
				}
				off++;
				off+=_remap_vars(o+off,o,o_dt);
				l-=2;
				while (l){
					l--;
					off+=_remap_vars(o+off,o,o_dt);
				}
				if (rm){
					_optimize(o,p,o_dt,0);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				sll_object_offset_t off=1;
				sll_arg_count_t l=o->dt.fn.ac;
				while (l){
					l--;
					off+=_remap_vars(o+off,o,o_dt);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_object_offset_t off=1;
				sll_statement_count_t l=o->dt.sc;
				while (l){
					l--;
					off+=_remap_vars(o+off,o,o_dt);
				}
				return off+eoff;
			}
	}
	sll_object_offset_t off=1;
	sll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		off+=_remap_vars(o+off,o,o_dt);
	}
	return off+eoff;
}



__SLL_FUNC void sll_optimize_object(sll_compilation_data_t* c_dt,sll_object_t* o){
	optimizer_data_t o_dt={
		c_dt,
		{
			.l_im=malloc(c_dt->idt.ill*sizeof(identifier_data_t)),
			.n_vi=0,
			.l_sc=0,
			.sc_vi=malloc(c_dt->_n_sc_id*sizeof(sll_variable_index_t)),
			.vc=0
		},
		.vi=SLL_MAX_VARIABLE_INDEX,
		.rm=0
	};
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		o_dt.it.s_im[i]=malloc(c_dt->idt.s[i].l*sizeof(identifier_data_t));
		for (sll_identifier_list_length_t j=0;j<c_dt->idt.s[i].l;j++){
			(o_dt.it.s_im[i]+j)->v=SLL_MAX_VARIABLE_INDEX;
			(o_dt.it.s_im[i]+j)->c=0;
		}
	}
	for (sll_identifier_list_length_t i=0;i<c_dt->idt.ill;i++){
		(o_dt.it.l_im+i)->v=SLL_MAX_VARIABLE_INDEX;
		(o_dt.it.l_im+i)->c=0;
	}
	for (sll_scope_t i=0;i<c_dt->_n_sc_id;i++){
		*(o_dt.it.sc_vi+i)=SLL_MAX_VARIABLE_INDEX;
	}
	_map_identifiers(c_dt->h,c_dt,&(o_dt.it));
	for (sll_function_index_t i=0;i<c_dt->ft.l;i++){
		o_dt.it.n_vi=o_dt.it.vc;
		o_dt.it.l_sc=0;
		sll_object_t* fo=c_dt->h+(*(c_dt->ft.dt+i))->off;
		SLL_ASSERT(fo->t==SLL_OBJECT_TYPE_FUNC);
		sll_object_offset_t off=1;
		for (sll_arg_count_t j=0;j<fo->dt.fn.ac;j++){
			off+=_map_identifiers(fo+off,c_dt,&(o_dt.it));
		}
	}
	free(o_dt.it.sc_vi);
	for (sll_export_table_length_t i=0;i<c_dt->et.l;i++){
		sll_identifier_index_t j=*(c_dt->et.dt+i);
		if (SLL_IDENTIFIER_GET_ARRAY_ID(j)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
			(o_dt.it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX(j))->c++;
		}
		else{
			(o_dt.it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID(j)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(j))->c++;
		}
	}
	o_dt.v=malloc(o_dt.it.vc*sizeof(sll_runtime_object_t));
	for (sll_variable_index_t i=0;i<o_dt.it.vc;i++){
		(o_dt.v+i)->t=RUNTIME_OBJECT_TYPE_UNKNOWN;
	}
	_optimize(c_dt->h,NULL,&o_dt,0);
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		o_dt.im.s[i]=malloc(c_dt->idt.s[i].l*sizeof(sll_identifier_index_t));
		sll_identifier_list_length_t k=0;
		for (sll_identifier_list_length_t j=0;j<c_dt->idt.s[i].l;j++){
			*(o_dt.im.s[i]+j)=SLL_CREATE_IDENTIFIER(j-k,i);
			*(c_dt->idt.s[i].dt+j-k)=*(c_dt->idt.s[i].dt+j);
			if (!(o_dt.it.s_im[i]+j)->c){
				k++;
			}
		}
		if (k){
			c_dt->idt.s[i].l-=k;
			c_dt->idt.s[i].dt=realloc(c_dt->idt.s[i].dt,c_dt->idt.s[i].l*sizeof(sll_identifier_t));
		}
	}
	o_dt.im.l=malloc(c_dt->idt.ill*sizeof(sll_identifier_index_t));
	sll_identifier_list_length_t j=0;
	for (sll_identifier_list_length_t i=0;i<c_dt->idt.ill;i++){
		*(o_dt.im.l+i)=SLL_CREATE_IDENTIFIER(i-j,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
		*(c_dt->idt.il+i-j)=*(c_dt->idt.il+i);
		if (!(o_dt.it.l_im+i)->c){
			j++;
		}
	}
	if (j){
		c_dt->idt.ill-=j;
		c_dt->idt.il=realloc(c_dt->idt.il,c_dt->idt.ill*sizeof(sll_identifier_t));
	}
	o_dt.rm=0;
	_remap_vars(c_dt->h,NULL,&o_dt);
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		free(o_dt.it.s_im[i]);
		free(o_dt.im.s[i]);
	}
	free(o_dt.it.l_im);
	free(o_dt.im.l);
	free(o_dt.v);
}
