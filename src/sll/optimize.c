#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/common.h>
#include <sll/constants.h>
#include <sll/core.h>
#include <sll/gc.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
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
			else if ((p)->t==SLL_OBJECT_TYPE_COMMA||(p)->t==SLL_OBJECT_TYPE_OPERATION_LIST){ \
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
#define INCREASE_PARENT(p,v) \
	do{ \
		if (p){ \
			if ((p)->t==SLL_OBJECT_TYPE_FUNC||(p)->t==SLL_OBJECT_TYPE_INTERNAL_FUNC){ \
				(p)->dt.fn.ac+=(v); \
			} \
			else if ((p)->t==SLL_OBJECT_TYPE_COMMA||(p)->t==SLL_OBJECT_TYPE_OPERATION_LIST){ \
				(p)->dt.sc+=(v); \
			} \
			else{ \
				(p)->dt.ac+=(v); \
			} \
		} \
	} while (0)



static sll_object_t* _remove_single_object(sll_object_t* o){
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
		o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	sll_object_type_t t=o->t;
	o->t=SLL_OBJECT_TYPE_NOP;
	switch (t){
		case SLL_OBJECT_TYPE_UNKNOWN:
		case SLL_OBJECT_TYPE_CHAR:
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
		case SLL_OBJECT_TYPE_STRING:
		case SLL_OBJECT_TYPE_IDENTIFIER:
		case SLL_OBJECT_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_array_length_t l=o->dt.al;
				o++;
				while (l){
					l--;
					o=_remove_single_object(o);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				o++;
				while (l){
					l--;
					o=_remove_single_object(o);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					o=_remove_single_object(o);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_FOR:
		case SLL_OBJECT_TYPE_WHILE:
		case SLL_OBJECT_TYPE_LOOP:
			{
				sll_arg_count_t l=o->dt.l.ac;
				o++;
				while (l){
					l--;
					o=_remove_single_object(o);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_COMMA:
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_statement_count_t l=o->dt.sc;
				o++;
				while (l){
					l--;
					o=_remove_single_object(o);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_remove_single_object(o);
	}
	return o;
}



static sll_object_t* _remove_up_to_end(sll_object_t* o,sll_object_offset_t off){
	while (off){
		off--;
		o=_remove_single_object(o);
	}
	return o;
}



static sll_string_index_t _create_print_string(optimizer_data_t* o_dt,const sll_char_t* a,const sll_char_t* b,sll_string_length_t al,sll_string_length_t bl,sll_string_checksum_t c){
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



static const sll_object_t* _map_identifiers(const sll_object_t* o,const sll_compilation_data_t* c_dt,identifier_map_data_t* im){
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
		o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
		case SLL_OBJECT_TYPE_CHAR:
		case SLL_OBJECT_TYPE_STRING:
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
		case SLL_OBJECT_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_array_length_t l=o->dt.al;
				o++;
				while (l){
					l--;
					o=_map_identifiers(o,c_dt,im);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				o++;
				while (l){
					l--;
					o=_map_identifiers(o,c_dt,im);
				}
				return o;
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
				return o+1;
			}
		case SLL_OBJECT_TYPE_FUNC:
			return sll_skip_object_const(o);
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					o=_map_identifiers(o,c_dt,im);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_COMMA:
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_statement_count_t l=o->dt.sc;
				o++;
				while (l){
					l--;
					o=_map_identifiers(o,c_dt,im);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_map_identifiers(o,c_dt,im);
	}
	return o;
}



static sll_runtime_object_t* _get_as_runtime_object(const sll_object_t* o,const optimizer_data_t* o_dt){
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
		o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_CHAR:
			return SLL_FROM_CHAR(o->dt.c);
		case SLL_OBJECT_TYPE_INT:
			return SLL_FROM_INT(o->dt.i);
		case SLL_OBJECT_TYPE_FLOAT:
			return SLL_FROM_FLOAT(o->dt.f);
		case SLL_OBJECT_TYPE_STRING:
			{
				sll_runtime_object_t* v=SLL_CREATE();
				v->t=SLL_RUNTIME_OBJECT_TYPE_STRING|RUNTIME_OBJECT_EXTERNAL_STRING;
				v->dt.s=*(o_dt->c_dt->st.dt+o->dt.s);
				return v;
			}
		case SLL_OBJECT_TYPE_IDENTIFIER:
			SLL_ACQUIRE(*(o_dt->v+GET_VARIABLE_INDEX(o,o_dt)));
			return *(o_dt->v+GET_VARIABLE_INDEX(o,o_dt));
		case SLL_OBJECT_TYPE_FUNCTION_ID:
			{
				sll_runtime_object_t* v=SLL_CREATE();
				v->t=RUNTIME_OBJECT_TYPE_FUNCTION_ID;
				v->dt.i=o->dt.fn_id;
				return v;
			}
		case SLL_OBJECT_TYPE_PRINT:
		case SLL_OBJECT_TYPE_IF:
		case SLL_OBJECT_TYPE_FOR:
		case SLL_OBJECT_TYPE_RETURN:
		case SLL_OBJECT_TYPE_EXIT:
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			return SLL_ACQUIRE_STATIC_INT(0);
		case SLL_OBJECT_TYPE_FUNC:
			{
				sll_runtime_object_t* v=SLL_CREATE();
				v->t=RUNTIME_OBJECT_TYPE_FUNCTION_ID;
				v->dt.i=o->dt.fn.id;
				return v;
			}
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			return SLL_FROM_INT(~((sll_integer_t)o->dt.fn.id));
		case SLL_OBJECT_TYPE_COMMA:
			SLL_UNIMPLEMENTED();
	}
	sll_runtime_object_t* v=SLL_CREATE();
	v->t=RUNTIME_OBJECT_TYPE_UNKNOWN;
	return v;
}



static sll_runtime_object_t* _get_as_runtime_object_nonstatic(const sll_object_t* o,const optimizer_data_t* o_dt){
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
		o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	sll_runtime_object_t* v=SLL_CREATE();
	v->t=RUNTIME_OBJECT_TYPE_UNKNOWN;
	switch (o->t){
		case SLL_OBJECT_TYPE_CHAR:
			v->t=SLL_RUNTIME_OBJECT_TYPE_CHAR;
			v->dt.c=o->dt.c;
			break;
		case SLL_OBJECT_TYPE_INT:
			v->t=SLL_RUNTIME_OBJECT_TYPE_INT;
			v->dt.i=o->dt.i;
			break;
		case SLL_OBJECT_TYPE_FLOAT:
			v->t=SLL_RUNTIME_OBJECT_TYPE_FLOAT;
			v->dt.f=o->dt.f;
			break;
		case SLL_OBJECT_TYPE_STRING:
			v->t=SLL_RUNTIME_OBJECT_TYPE_STRING|RUNTIME_OBJECT_EXTERNAL_STRING;
			v->dt.s=*(o_dt->c_dt->st.dt+o->dt.s);
			break;
		case SLL_OBJECT_TYPE_IDENTIFIER:
			SLL_RELEASE(v);
			SLL_ACQUIRE(*(o_dt->v+GET_VARIABLE_INDEX(o,o_dt)));
			return *(o_dt->v+GET_VARIABLE_INDEX(o,o_dt));
		case SLL_OBJECT_TYPE_FUNCTION_ID:
			v->t=RUNTIME_OBJECT_TYPE_FUNCTION_ID;
			v->dt.i=o->dt.fn_id;
			break;
		case SLL_OBJECT_TYPE_PRINT:
		case SLL_OBJECT_TYPE_IF:
		case SLL_OBJECT_TYPE_FOR:
		case SLL_OBJECT_TYPE_RETURN:
		case SLL_OBJECT_TYPE_EXIT:
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			v->t=SLL_RUNTIME_OBJECT_TYPE_INT;
			v->dt.i=0;
			break;
		case SLL_OBJECT_TYPE_FUNC:
			v->t=RUNTIME_OBJECT_TYPE_FUNCTION_ID;
			v->dt.i=o->dt.fn.id;
			break;
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			v->t=SLL_RUNTIME_OBJECT_TYPE_INT;
			v->dt.i=~((sll_integer_t)o->dt.fn.id);
			break;
		case SLL_OBJECT_TYPE_COMMA:
			SLL_UNIMPLEMENTED();
	}
	return v;
}



static void _runtime_object_to_object(sll_runtime_object_t* v,sll_object_t* o,optimizer_data_t* o_dt){
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
				if (o->dt.s==o_dt->c_dt->st.l){
					v->t|=RUNTIME_OBJECT_EXTERNAL_STRING;
					o->dt.s=sll_add_string(&(o_dt->c_dt->st),&(v->dt.s));
					break;
				}
			}
			SLL_ASSERT(o->dt.s<o_dt->c_dt->st.l);
			break;
		case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
			SLL_UNIMPLEMENTED();
		case SLL_RUNTIME_OBJECT_TYPE_HANDLE:
			SLL_UNIMPLEMENTED();
		case SLL_RUNTIME_OBJECT_TYPE_MAP:
			SLL_UNIMPLEMENTED();
		case RUNTIME_OBJECT_TYPE_FUNCTION_ID:
			o->t=SLL_OBJECT_TYPE_FUNCTION_ID;
			o->dt.fn_id=(sll_function_index_t)(v->dt.i);
			break;
		case RUNTIME_OBJECT_TYPE_UNKNOWN:
			SLL_UNIMPLEMENTED();
		default:
			SLL_UNREACHABLE();
	}
}



static const sll_object_t* _mark_loop_vars(const sll_object_t* o,optimizer_data_t* o_dt){
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
		o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
		case SLL_OBJECT_TYPE_CHAR:
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
		case SLL_OBJECT_TYPE_STRING:
		case SLL_OBJECT_TYPE_IDENTIFIER:
		case SLL_OBJECT_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_array_length_t l=o->dt.al;
				o++;
				while (l){
					l--;
					o=_mark_loop_vars(o,o_dt);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				o++;
				while (l){
					l--;
					o=_mark_loop_vars(o,o_dt);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_ASSIGN:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l>=2);
				o++;
				while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
					o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				SLL_ASSERT(o->t==SLL_OBJECT_TYPE_IDENTIFIER);
				sll_runtime_object_t* tmp=_get_as_runtime_object(o+1,o_dt);
				if (tmp->t==RUNTIME_OBJECT_TYPE_UNKNOWN){
					(*(o_dt->v+GET_VARIABLE_INDEX(o,o_dt)))->t|=RUNTIME_OBJECT_CHANGE_IN_LOOP;
				}
				SLL_RELEASE(tmp);
				o++;
				l--;
				while (l){
					l--;
					o=_mark_loop_vars(o,o_dt);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					o=_mark_loop_vars(o,o_dt);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_COMMA:
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_statement_count_t l=o->dt.sc;
				o++;
				while (l){
					l--;
					o=_mark_loop_vars(o,o_dt);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_mark_loop_vars(o,o_dt);
	}
	return o;
}



static uint8_t _get_cond_type(const sll_object_t* o,optimizer_data_t* o_dt,uint8_t inv,uint8_t lv){
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
		o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
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
		case SLL_OBJECT_TYPE_MAP:
			return (((!!o->dt.ml)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
		case SLL_OBJECT_TYPE_IDENTIFIER:
			{
				sll_runtime_object_t* v=*(o_dt->v+GET_VARIABLE_INDEX(o,o_dt));
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
		case SLL_OBJECT_TYPE_FUNCTION_ID:
			return (inv?COND_TYPE_ALWAYS_FALSE:COND_TYPE_ALWAYS_TRUE);
		case SLL_OBJECT_TYPE_PRINT:
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
		case SLL_OBJECT_TYPE_IF:
		case SLL_OBJECT_TYPE_FOR:
		case SLL_OBJECT_TYPE_WHILE:
		case SLL_OBJECT_TYPE_LOOP:
		case SLL_OBJECT_TYPE_CONTINUE:
		case SLL_OBJECT_TYPE_BREAK:
		case SLL_OBJECT_TYPE_RETURN:
		case SLL_OBJECT_TYPE_EXIT:
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			return (inv?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
		case SLL_OBJECT_TYPE_INLINE_FUNC:
			SLL_UNIMPLEMENTED();
		case SLL_OBJECT_TYPE_INPUT:
			SLL_UNIMPLEMENTED();
		case SLL_OBJECT_TYPE_AND:
			SLL_UNIMPLEMENTED();
		case SLL_OBJECT_TYPE_OR:
			SLL_UNIMPLEMENTED();
		case SLL_OBJECT_TYPE_NOT:
			return (!o->dt.ac?COND_TYPE_ALWAYS_FALSE:_get_cond_type(o+1,o_dt,!inv,lv));
		case SLL_OBJECT_TYPE_ASSIGN:
			SLL_ASSERT(o->dt.ac>=2);
			return _get_cond_type(sll_skip_object_const(o+1),o_dt,inv,lv);
		case SLL_OBJECT_TYPE_CALL:
			return COND_TYPE_UNKNOWN;
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
		case SLL_OBJECT_TYPE_COMMA:
			// SLL_UNIMPLEMENTED();
			return COND_TYPE_UNKNOWN;
		default:
			SLL_UNREACHABLE();
	}
	return COND_TYPE_UNKNOWN;
}



static sll_object_t* _inline_function(sll_object_t* o,optimizer_data_t* o_dt){
	// SLL_UNIMPLEMENTED();
	return sll_skip_object(o);
	// SLL_ASSERT(o->t==SLL_OBJECT_TYPE_CALL);
	// o->t=SLL_OBJECT_TYPE_INLINE_FUNC;
	// SLL_ASSERT(o->dt.ac);
	// sll_object_offset_t off=1;
	// while ((o+off)->t==SLL_OBJECT_TYPE_NOP||(o+off)->t==SLL_OBJECT_TYPE_DEBUG_DATA){
	// 	off++;
	// }
	// sll_object_t* fn=o+off;
	// sll_runtime_object_t* fn_id=_get_as_runtime_object(fn,o_dt);
	// SLL_ASSERT(fn_id->t==RUNTIME_OBJECT_TYPE_FUNCTION_ID);
	// sll_function_t* fn_dt=*(o_dt->c_dt->ft.dt+fn_id->dt.i);
	// *(o_dt->c_dt->ft.dt+fn_id->dt.i)=NULL;
	// SLL_RELEASE(fn_id);
	// if (fn_dt->al!=o->dt.ac-1){
	// 	SLL_UNIMPLEMENTED();
	// }
	// sll_object_t* tmp=((sll_object_t*)(o_dt->c_dt->_s.ptr))+fn_dt->off;
	// sll_object_offset_t fn_src_sz=sll_get_object_size(tmp);
	// sll_object_t* fn_src=malloc(fn_src_sz*sizeof(sll_object_t));
	// memcpy(fn_src,tmp,fn_src_sz*sizeof(sll_object_t));
	// sll_object_offset_t sz=off+sll_get_object_size(fn);
	// while (off<sz){
	// 	(o+off)->t=SLL_OBJECT_TYPE_NOP;
	// 	off++;
	// }
	// tmp=o+off;
	// sll_object_offset_t fn_arg_sz=0;
	// sll_object_offset_t* fn_arg_sz_l=malloc(fn_dt->al*sizeof(sll_object_offset_t));
	// for (sll_arg_count_t i=0;i<fn_dt->al;i++){
	// 	sz=sll_get_object_size(tmp+fn_arg_sz);
	// 	*(fn_arg_sz_l+i)=sz;
	// 	fn_arg_sz+=sz;
	// }
	// sll_object_t* fn_arg=malloc(fn_arg_sz*sizeof(sll_object_t));
	// memcpy(fn_arg,tmp,fn_arg_sz*sizeof(sll_object_t));
	// for (sll_object_offset_t i=0;i<fn_arg_sz;i++){
	// 	(tmp+i)->t=SLL_OBJECT_TYPE_NOP;
	// }
	// off+=fn_arg_sz;
	// sll_object_offset_t n_sz=(fn_dt->al<<1)+fn_arg_sz+fn_src_sz;
	// sll_object_offset_t out=0;
	// if (n_sz>off-1){
	// 	out=n_sz-(off-1);
	// 	_shift_objects(o+off,o_dt,out);
	// }
	// off=1;
	// sll_object_offset_t fn_arg_off=0;
	// for (sll_arg_count_t i=0;i<o->dt.ac-1;i++){
	// 	(o+off)->t=SLL_OBJECT_TYPE_ASSIGN;
	// 	(o+off)->dt.ac=2;
	// 	off++;
	// 	(o+off)->t=SLL_OBJECT_TYPE_IDENTIFIER;
	// 	(o+off)->dt.i=fn_dt->a[i];
	// 	off++;
	// 	sz=*(fn_arg_sz_l+i);
	// 	memcpy(o+off,fn_arg+fn_arg_off,sz*sizeof(sll_object_t));
	// 	sz+=off;
	// 	while (off<sz){
	// 		if ((o+off)->t==SLL_OBJECT_TYPE_FUNC){
	// 			(*(o_dt->c_dt->ft.dt+(o+off)->dt.fn.id))->off=(sll_object_offset_t)(o+off-((sll_object_t*)(o_dt->c_dt->_s.ptr)));
	// 		}
	// 		off++;
	// 	}
	// }
	// free(fn_arg);
	// free(fn_arg_sz_l);
	// free(fn_dt);
	// memcpy(o+off,fn_src,fn_src_sz*sizeof(sll_object_t));
	// free(fn_src);
	// while ((o+off)->t!=SLL_OBJECT_TYPE_FUNC){
	// 	(o+off)->t=SLL_OBJECT_TYPE_NOP;
	// 	off++;
	// }
	// (o+off)->t=SLL_OBJECT_TYPE_NOP;
	// o->dt.ac+=(o+off)->dt.fn.ac-1;
	// return out;
}



static sll_object_t* _check_remove(sll_object_t* o,sll_object_t* p,optimizer_data_t* o_dt){
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
		o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_CHAR:
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
		case SLL_OBJECT_TYPE_STRING:
		case SLL_OBJECT_TYPE_IDENTIFIER:
		case SLL_OBJECT_TYPE_FUNCTION_ID:
			DECREASE_PARENT(p);
			o->t=SLL_OBJECT_TYPE_NOP;
			return o+1;
		case SLL_OBJECT_TYPE_ARRAY:
			{
				o->t=SLL_OBJECT_TYPE_NOP;
				sll_array_length_t l=o->dt.al;
				INCREASE_PARENT(p,l-1);
				o++;
				while (l){
					l--;
					o=_check_remove(o,p,o_dt);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				o->t=SLL_OBJECT_TYPE_NOP;
				sll_map_length_t l=o->dt.ml;
				INCREASE_PARENT(p,l-1);
				o++;
				while (l){
					l--;
					o=_check_remove(o,p,o_dt);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				o->t=SLL_OBJECT_TYPE_NOP;
				sll_array_length_t l=o->dt.fn.ac;
				INCREASE_PARENT(p,l-1);
				o++;
				while (l){
					l--;
					o=_check_remove(o,p,o_dt);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_INLINE_FUNC:
			SLL_UNIMPLEMENTED();
		case SLL_OBJECT_TYPE_CALL:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				sll_object_t* r=o;
				o++;
				while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
					o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				sll_runtime_object_t* rt=_get_as_runtime_object(o,o_dt);
				if (rt->t==SLL_RUNTIME_OBJECT_TYPE_INT&&rt->dt.i<0){
					sll_function_index_t i=(sll_function_index_t)(~(rt->dt.i));
					if (i<o_dt->i_ft->l&&(*(o_dt->i_ft->dt+i))->t==SLL_INTERNAL_FUNCTION_TYPE_DEFAULT){
						r->t=SLL_OBJECT_TYPE_OPERATION_LIST;
						r->dt.sc=l;
						SLL_RELEASE(rt);
						return _check_remove(r,p,o_dt);
					}
				}
				SLL_RELEASE(rt);
				for (sll_arg_count_t i=1;i<l;i++){
					o=sll_skip_object(o);
				}
				return o;
			}
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
			{
				o->t=SLL_OBJECT_TYPE_NOP;
				sll_arg_count_t l=o->dt.ac;
				INCREASE_PARENT(p,l-1);
				o++;
				while (l){
					l--;
					o=_check_remove(o,p,o_dt);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_COMMA:
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				o->t=SLL_OBJECT_TYPE_NOP;
				sll_statement_count_t l=o->dt.sc;
				INCREASE_PARENT(p,l-1);
				o++;
				while (l){
					l--;
					o=_check_remove(o,p,o_dt);
				}
				return o;
			}
	}
	return sll_skip_object(o);
}



static sll_object_t* _optimize(sll_object_t* o,sll_object_t* p,optimizer_data_t* o_dt,uint8_t fl){
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
		o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	SLL_ASSERT(!o_dt->rm);
	if (o_dt->vi!=SLL_MAX_VARIABLE_INDEX){
		SLL_RELEASE(*(o_dt->v+o_dt->vi));
		*(o_dt->v+o_dt->vi)=_get_as_runtime_object_nonstatic(o,o_dt);
		o_dt->vi=SLL_MAX_VARIABLE_INDEX;
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
			return o+1;
		case SLL_OBJECT_TYPE_CHAR:
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
		case SLL_OBJECT_TYPE_STRING:
		case SLL_OBJECT_TYPE_FUNCTION_ID:
			if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
				DECREASE_PARENT(p);
				o->t=SLL_OBJECT_TYPE_NOP;
			}
			return o+1;
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_object_t* r=o;
				sll_array_length_t l=o->dt.al;
				o++;
				while (l){
					l--;
					o=_optimize(o,NULL,o_dt,(fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG)|OPTIMIZER_FLAG_ARGUMENT);
					if (o_dt->rm){
						SLL_UNIMPLEMENTED();
					}
				}
				if (!r->dt.al){
					if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
						r->t=SLL_OBJECT_TYPE_NOP;
						DECREASE_PARENT(p);
					}
				}
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_object_t* r=o;
				sll_map_length_t l=o->dt.ml;
				o++;
				while (l){
					l--;
					o=_optimize(o,NULL,o_dt,(fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG)|OPTIMIZER_FLAG_ARGUMENT);
					if (o_dt->rm){
						SLL_UNIMPLEMENTED();
					}
				}
				if (!r->dt.ml){
					if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
						r->t=SLL_OBJECT_TYPE_NOP;
						DECREASE_PARENT(p);
					}
				}
				return o;
			}
		case SLL_OBJECT_TYPE_IDENTIFIER:
			if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
				DECREASE_PARENT(p);
				DECREASE_VARIABLE(o,o_dt);
				o->t=SLL_OBJECT_TYPE_NOP;
			}
			else{
				sll_runtime_object_t* v=*(o_dt->v+GET_VARIABLE_INDEX(o,o_dt));
				if (SLL_RUNTIME_OBJECT_GET_TYPE(v)!=RUNTIME_OBJECT_TYPE_UNKNOWN&&(!(v->t&RUNTIME_OBJECT_CHANGE_IN_LOOP)||(fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG))){
					DECREASE_VARIABLE(o,o_dt);
					_runtime_object_to_object(v,o,o_dt);
				}
			}
			return o+1;
		case SLL_OBJECT_TYPE_PRINT:
			{
				sll_arg_count_t l=o->dt.ac;
				sll_object_t* r=o;
				o++;
				sll_object_t* a=NULL;
				while (l){
					l--;
					sll_object_t* b=o;
					o=_optimize(b,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
					if (o_dt->rm){
						SLL_UNIMPLEMENTED();
					}
					while (b->t==SLL_OBJECT_TYPE_NOP||b->t==SLL_OBJECT_TYPE_DEBUG_DATA||b->t==OBJECT_TYPE_CHANGE_STACK){
						b=(b->t==OBJECT_TYPE_CHANGE_STACK?b->dt._p:b+1);
					}
					if (b->t==SLL_OBJECT_TYPE_INT||b->t==SLL_OBJECT_TYPE_FLOAT){
						sll_runtime_object_t rt=SLL_RUNTIME_OBJECT_STRUCT;
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
							r->dt.ac--;
						}
						else if (a->t==SLL_OBJECT_TYPE_CHAR&&b->t==SLL_OBJECT_TYPE_STRING){
							sll_string_t* sb=o_dt->c_dt->st.dt+b->dt.s;
							a->t=SLL_OBJECT_TYPE_NOP;
							b->t=SLL_OBJECT_TYPE_STRING;
							b->dt.s=_create_print_string(o_dt,&(a->dt.c),sb->v,1,sb->l,a->dt.c^sb->c);
							r->dt.ac--;
						}
						else if (a->t==SLL_OBJECT_TYPE_STRING&&b->t==SLL_OBJECT_TYPE_CHAR){
							sll_string_t* sa=o_dt->c_dt->st.dt+a->dt.s;
							a->t=SLL_OBJECT_TYPE_NOP;
							b->t=SLL_OBJECT_TYPE_STRING;
							b->dt.s=_create_print_string(o_dt,sa->v,&(b->dt.c),sa->l,1,sa->c^b->dt.c);
							r->dt.ac--;
						}
						else if (a->t==SLL_OBJECT_TYPE_STRING&&b->t==SLL_OBJECT_TYPE_STRING){
							sll_string_t* sa=o_dt->c_dt->st.dt+a->dt.s;
							sll_string_t* sb=o_dt->c_dt->st.dt+b->dt.s;
							a->t=SLL_OBJECT_TYPE_NOP;
							b->t=SLL_OBJECT_TYPE_STRING;
							b->dt.s=_create_print_string(o_dt,sa->v,sb->v,sa->l,sb->l,sa->c^sb->c);
							r->dt.ac--;
						}
					}
					if (b->t==SLL_OBJECT_TYPE_STRING){
						sll_string_t* s=o_dt->c_dt->st.dt+b->dt.s;
						if (!s->l){
							r->dt.ac--;
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
				return o;
			}
		case SLL_OBJECT_TYPE_INPUT:
			SLL_UNIMPLEMENTED();
		case SLL_OBJECT_TYPE_AND:
			SLL_UNIMPLEMENTED();
		case SLL_OBJECT_TYPE_OR:
			SLL_UNIMPLEMENTED();
		case SLL_OBJECT_TYPE_NOT:
		case SLL_OBJECT_TYPE_BIT_NOT:
		case SLL_OBJECT_TYPE_LENGTH:
			{
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					o->t=SLL_OBJECT_TYPE_INT;
					o->dt.i=0;
					return o+1;
				}
				sll_object_t* r=o;
				o++;
				while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
					o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				sll_object_t* v=o;
				o=_optimize(v,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
				l--;
				while (l){
					o=_optimize(o,NULL,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
					l--;
				}
				sll_runtime_object_t* rt=_get_as_runtime_object(v,o_dt);
				if (rt->t!=RUNTIME_OBJECT_TYPE_UNKNOWN){
					_check_remove(v,r,o_dt);
					if (rt->t==RUNTIME_OBJECT_TYPE_FUNCTION_ID){
						rt->t=SLL_RUNTIME_OBJECT_TYPE_INT;
						rt->dt.i++;
					}
					if (r->dt.ac){
						SLL_UNIMPLEMENTED();
					}
					r->t=SLL_OBJECT_TYPE_INT;
					if (r->t==SLL_OBJECT_TYPE_NOT){
						r->dt.i=!sll_operator_bool(rt);
					}
					else if (r->t==SLL_OBJECT_TYPE_BIT_NOT){
						sll_runtime_object_t* nv=sll_operator_inv(rt);
						SLL_ASSERT(SLL_RUNTIME_OBJECT_GET_TYPE(nv)==SLL_RUNTIME_OBJECT_TYPE_INT);
						r->dt.i=nv->dt.i;
						SLL_RELEASE(nv);
					}
					else{
						sll_runtime_object_t* nv=sll_operator_len(rt);
						SLL_ASSERT(SLL_RUNTIME_OBJECT_GET_TYPE(nv)==SLL_RUNTIME_OBJECT_TYPE_INT);
						r->dt.i=nv->dt.i;
						SLL_RELEASE(nv);
					}
				}
				SLL_RELEASE(rt);
				return o;
			}
		case SLL_OBJECT_TYPE_ASSIGN:
			{
				sll_object_t* r=o;
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l>=2);
				o++;
				while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
					o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				sll_object_t* id_o=o;
				SLL_ASSERT(id_o->t==SLL_OBJECT_TYPE_IDENTIFIER);
				DECREASE_VARIABLE(id_o,o_dt);
				o_dt->vi=GET_VARIABLE_INDEX(id_o,o_dt);
				o=_optimize(o+1,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
				if (o_dt->rm){
					SLL_UNIMPLEMENTED();
				}
				l-=2;
				while (l){
					l--;
					o=_optimize(o,r,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
					if (o_dt->rm){
						SLL_UNIMPLEMENTED();
					}
				}
				return o;
			}
		case SLL_OBJECT_TYPE_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				sll_object_t* r=o;
				o++;
				while (l){
					l--;
					o=_optimize(o,r,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
					if (o_dt->rm){
						r->dt.ac-=l;
						o=_remove_up_to_end(o,l);
						break;
					}
				}
				o_dt->rm=0;
				return o;
			}
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
					o->t=SLL_OBJECT_TYPE_OPERATION_LIST;
					o->dt.sc=o->dt.fn.ac;
					return _optimize(o,p,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
				}
				sll_object_t* r=o;
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					o=_optimize(o,r,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
					if (o_dt->rm){
						_remove_up_to_end(o,l);
						r->dt.fn.ac-=l;
						break;
					}
				}
				return o;
			}
		case SLL_OBJECT_TYPE_INLINE_FUNC:
			SLL_UNIMPLEMENTED();
		case SLL_OBJECT_TYPE_CALL:
			{
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					o->t=SLL_OBJECT_TYPE_NOP;
					return o+1;
				}
				sll_object_t* r=o;
				o++;
				while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
					o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				sll_object_t* fn=o;
				o=_optimize(fn,o,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
				if (o_dt->rm){
					_remove_up_to_end(o,l);
				}
				l--;
				while (l){
					l--;
					o=_optimize(o,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
					if (o_dt->rm){
						_remove_up_to_end(o,l);
						r->dt.ac-=l;
						break;
					}
				}
				if (fn->t==SLL_OBJECT_TYPE_FUNC){
					o=_inline_function(r,o_dt);
				}
				else if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
					sll_runtime_object_t* rt=_get_as_runtime_object(fn,o_dt);
					if (rt->t==SLL_RUNTIME_OBJECT_TYPE_INT&&rt->dt.i<0){
						sll_function_index_t i=(sll_function_index_t)(~(rt->dt.i));
						if (i<o_dt->i_ft->l&&(*(o_dt->i_ft->dt+i))->t==SLL_INTERNAL_FUNCTION_TYPE_DEFAULT){
							r->t=SLL_OBJECT_TYPE_OPERATION_LIST;
							r->dt.sc=r->dt.ac;
							_optimize(r,p,o_dt,fl);
						}
					}
					SLL_RELEASE(rt);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_IF:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				if (l==1){
					o->t=SLL_OBJECT_TYPE_NOP;
					return _optimize(o+1,p,o_dt,fl);
				}
				sll_object_t* r=o;
				o++;
				if (l&1){
					l>>=1;
					while (l){
						l--;
						sll_object_t* cnd_o=o;
						o=_optimize(cnd_o,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
						if (o_dt->rm){
							SLL_UNIMPLEMENTED();
						}
						uint8_t cnd=_get_cond_type(cnd_o,o_dt,0,0);
						if (cnd==COND_TYPE_ALWAYS_TRUE){
							if (r->dt.ac-(l<<1)==3){
								r->t=SLL_OBJECT_TYPE_NOP;
								o=_optimize(o,p,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
								o=_remove_up_to_end(o,r->dt.ac-(l<<1));
							}
							else{
								o=_optimize(o,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
								o_dt->rm=0;
								_remove_up_to_end(o,l);
								r->dt.ac-=(l<<1)+2;
							}
							while (cnd_o->t==SLL_OBJECT_TYPE_NOP||cnd_o->t==SLL_OBJECT_TYPE_DEBUG_DATA||cnd_o->t==OBJECT_TYPE_CHANGE_STACK){
								cnd_o=(cnd_o->t==OBJECT_TYPE_CHANGE_STACK?cnd_o->dt._p:cnd_o+1);
							}
							cnd_o->t=SLL_OBJECT_TYPE_NOP;
							return o;
						}
						else if (cnd==COND_TYPE_ALWAYS_FALSE){
							r->dt.ac-=2;
							if (r->dt.ac==1){
								r->t=SLL_OBJECT_TYPE_NOP;
							}
							_remove_single_object(cnd_o);
							_remove_up_to_end(o,l);
							continue;
						}
						o=_optimize(o,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
						o_dt->rm=0;
					}
					if (r->t!=SLL_OBJECT_TYPE_NOP){
						o=_optimize(o,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
						o_dt->rm=0;
					}
					else{
						o=_optimize(o,p,o_dt,fl);
					}
				}
				else{
					l>>=1;
					while (l){
						l--;
						sll_object_t* cnd_o=o;
						o=_optimize(cnd_o,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
						if (o_dt->rm){
							SLL_UNIMPLEMENTED();
						}
						uint8_t cnd=_get_cond_type(cnd_o,o_dt,0,0);
						if (cnd==COND_TYPE_ALWAYS_TRUE){
							if (r->dt.ac-(l<<1)==2){
								r->t=SLL_OBJECT_TYPE_NOP;
								o=_optimize(o,p,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
								o=_remove_up_to_end(o,r->dt.ac-(l<<1)-2);
							}
							else{
								o_dt->rm=0;
								o=_optimize(o,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
								_remove_up_to_end(o,l);
							}
							r->dt.ac-=(l<<1)+1;
							_remove_single_object(cnd_o);
							break;
						}
						else if (cnd==COND_TYPE_ALWAYS_FALSE){
							r->dt.ac-=2;
							if (!r->dt.ac){
								r->t=SLL_OBJECT_TYPE_NOP;
								DECREASE_PARENT(p);
							}
							_remove_single_object(cnd_o);
							o=_remove_single_object(o);
							continue;
						}
						o=_optimize(o,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
						o_dt->rm=0;
					}
				}
				return o;
			}
		case SLL_OBJECT_TYPE_FOR:
			{
				sll_arg_count_t l=o->dt.l.ac;
				SLL_ASSERT(l);
				if (l==1){
					o->t=SLL_OBJECT_TYPE_NOP;
					return _optimize(o+1,p,o_dt,fl);
				}
				sll_object_t* r=o;
				o=_optimize(o+1,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
				if (o_dt->rm){
					r->t=SLL_OBJECT_TYPE_NOP;
					_remove_up_to_end(o,l-1);
					return o;
				}
				sll_object_t* cnd_o=o;
				const sll_object_t* tmp=o;
				for (sll_arg_count_t i=1;i<l;i++){
					tmp=_mark_loop_vars(tmp,o_dt);
				}
				o=_optimize(cnd_o,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
				uint8_t cnd=_get_cond_type(cnd_o,o_dt,1,1);
				if (cnd==COND_TYPE_ALWAYS_TRUE){
					SLL_UNIMPLEMENTED();
				}
				else if (cnd==COND_TYPE_ALWAYS_FALSE){
					r->t=SLL_OBJECT_TYPE_WHILE;
				}
				l-=2;
				while (l){
					l--;
					o=_optimize(o,r,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
					if (o_dt->rm){
						_remove_up_to_end(o,l);
						r->dt.l.ac-=l;
						break;
					}
				}
				return o;
			}
		case SLL_OBJECT_TYPE_WHILE:
			SLL_UNIMPLEMENTED();
		case SLL_OBJECT_TYPE_LOOP:
			SLL_UNIMPLEMENTED();
		case SLL_OBJECT_TYPE_ADD:
		case SLL_OBJECT_TYPE_MULT:
		case SLL_OBJECT_TYPE_BIT_AND:
		case SLL_OBJECT_TYPE_BIT_OR:
		case SLL_OBJECT_TYPE_BIT_XOR:
			{
				sll_arg_count_t l=o->dt.ac;
				if (l==1){
					o->t=SLL_OBJECT_TYPE_NOP;
					return _optimize(o+1,p,o_dt,fl);
				}
				sll_object_t* r=o;
				o++;
				sll_runtime_object_t* v=NULL;
				sll_object_t** arg=malloc(l*sizeof(sll_object_t*));
				sll_arg_count_t i=0;
				sll_arg_count_t j=0;
				while (l){
					l--;
					sll_object_t* a=o;
					o=_optimize(o,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
					if (o_dt->rm){
						SLL_UNIMPLEMENTED();
					}
					sll_runtime_object_t* av=_get_as_runtime_object(a,o_dt);
					if (SLL_RUNTIME_OBJECT_GET_TYPE(av)!=RUNTIME_OBJECT_TYPE_UNKNOWN&&!(av->t&RUNTIME_OBJECT_CHANGE_IN_LOOP)){
						*(arg+j)=o;
						if (!v){
							SLL_ACQUIRE(av);
							v=av;
							i=j;
						}
						else{
							sll_runtime_object_t* nv=NULL;
							switch (r->t){
								case SLL_OBJECT_TYPE_ADD:
									nv=sll_operator_add(v,av);
									break;
								case SLL_OBJECT_TYPE_MULT:
									nv=sll_operator_mult(v,av);
									break;
								case SLL_OBJECT_TYPE_BIT_AND:
									nv=sll_operator_and(v,av);
									break;
								case SLL_OBJECT_TYPE_BIT_OR:
									nv=sll_operator_or(v,av);
									break;
								case SLL_OBJECT_TYPE_BIT_XOR:
									nv=sll_operator_xor(v,av);
									break;
								default:
									SLL_UNREACHABLE();
							}
							SLL_RELEASE(v);
							v=nv;
						}
					}
					else{
						if (v){
							if (j-i>1){
								SLL_UNIMPLEMENTED();
							}
							SLL_RELEASE(v);
							v=NULL;
						}
					}
					SLL_RELEASE(av);
					j++;
				}
				if (v){
					if (j-i>1){
						if (!i){
							r->t=SLL_OBJECT_TYPE_COMMA;
							r->dt.ac++;
							_shift_objects(o,o_dt->c_dt,1);
							_runtime_object_to_object(v,o,o_dt);
							o++;
							_optimize(r,p,o_dt,fl);
						}
						else{
							SLL_UNIMPLEMENTED();
						}
					}
					SLL_RELEASE(v);
				}
				free(arg);
				return o;
			}
		case SLL_OBJECT_TYPE_SUB:
		case SLL_OBJECT_TYPE_DIV:
		case SLL_OBJECT_TYPE_FLOOR_DIV:
		case SLL_OBJECT_TYPE_MOD:
		case SLL_OBJECT_TYPE_BIT_RSHIFT:
		case SLL_OBJECT_TYPE_BIT_LSHIFT:
			{
				sll_arg_count_t l=o->dt.ac;
				sll_object_t* r=o;
				o++;
				while (l){
					l--;
					o=_optimize(o,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
					if (o_dt->rm){
						r->dt.ac-=l;
						_remove_up_to_end(o,l);
						break;
					}
				}
				return o;
			}
		case SLL_OBJECT_TYPE_LESS:
		case SLL_OBJECT_TYPE_LESS_EQUAL:
		case SLL_OBJECT_TYPE_EQUAL:
		case SLL_OBJECT_TYPE_NOT_EQUAL:
		case SLL_OBJECT_TYPE_MORE:
		case SLL_OBJECT_TYPE_MORE_EQUAL:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				sll_object_t* r=o;
				o=_optimize(o+1,o,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
				if (o_dt->rm){
					_remove_up_to_end(o,l-1);
					r->t=SLL_OBJECT_TYPE_NOP;
					return o;
				}
				sll_runtime_object_t* a=_get_as_runtime_object(r+1,o_dt);
				l--;
				if (a->t!=RUNTIME_OBJECT_TYPE_UNKNOWN&&!(a->t&RUNTIME_OBJECT_CHANGE_IN_LOOP)){
					while (l){
						sll_object_t* arg=o;
						o=_optimize(o,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
						if (o_dt->rm){
							_remove_up_to_end(o,l-1);
							r->dt.ac-=l-1;
							return o;
						}
						sll_runtime_object_t* b=_get_as_runtime_object(arg,o_dt);
						if (b->t==RUNTIME_OBJECT_TYPE_UNKNOWN||(b->t&RUNTIME_OBJECT_CHANGE_IN_LOOP)){
							SLL_RELEASE(b);
							break;
						}
						l--;
						sll_compare_result_t cmp=sll_operator_compare(a,b);
						SLL_RELEASE(a);
						if ((o->t==SLL_OBJECT_TYPE_LESS&&cmp==SLL_COMPARE_RESULT_BELOW)||(o->t==SLL_OBJECT_TYPE_LESS_EQUAL&&cmp!=SLL_COMPARE_RESULT_ABOVE)||(o->t==SLL_OBJECT_TYPE_EQUAL&&cmp==SLL_COMPARE_RESULT_EQUAL)||(o->t==SLL_OBJECT_TYPE_NOT_EQUAL&&cmp!=SLL_COMPARE_RESULT_EQUAL)||(o->t==SLL_OBJECT_TYPE_MORE&&cmp==SLL_COMPARE_RESULT_ABOVE)||(o->t==SLL_OBJECT_TYPE_MORE_EQUAL&&cmp!=SLL_COMPARE_RESULT_BELOW)){
							a=b;
							continue;
						}
						SLL_RELEASE(b);
						_remove_up_to_end(o,l);
						r->t=SLL_OBJECT_TYPE_OPERATION_LIST;
						r->dt.sc=r->dt.ac-l;
						return _optimize(r,p,o_dt,fl);
					}
					if (!l){
						SLL_RELEASE(a);
						_remove_up_to_end(o,1);
						r->t=SLL_OBJECT_TYPE_INT;
						r->dt.i=1;
						return r+1;
					}
					l--;
				}
				SLL_RELEASE(a);
				while (l){
					l--;
					o=_optimize(o,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
					if (o_dt->rm){
						_remove_up_to_end(o,l);
						r->dt.ac-=l;
						break;
					}
				}
				return o;
			}
		case SLL_OBJECT_TYPE_ACCESS:
		case SLL_OBJECT_TYPE_CAST:
			{
				sll_arg_count_t l=o->dt.ac;
				sll_object_t* r=o;
				o++;
				while (l){
					l--;
					o=_optimize(o,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
					if (o_dt->rm){
						SLL_UNIMPLEMENTED();
					}
				}
				return o;
			}
		case SLL_OBJECT_TYPE_BREAK:
		case SLL_OBJECT_TYPE_CONTINUE:
			{
				sll_arg_count_t l=o->dt.ac;
				sll_object_t* r=o;
				o++;
				while (l){
					l--;
					o=_optimize(o,r,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
					if (o_dt->rm){
						SLL_UNIMPLEMENTED();
					}
				}
				return o;
			}
		case SLL_OBJECT_TYPE_RETURN:
		case SLL_OBJECT_TYPE_EXIT:
			{
				sll_arg_count_t l=o->dt.ac;
				sll_object_t* r=o;
				o++;
				while (l){
					l--;
					o=_optimize(o,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
					if (o_dt->rm){
						SLL_UNIMPLEMENTED();
					}
				}
				o_dt->rm=1;
				return o;
			}
		case SLL_OBJECT_TYPE_COMMA:
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_statement_count_t l=o->dt.sc;
				sll_object_t* r=o;
				o++;
				while (l){
					l--;
					o=_optimize(o,r,o_dt,(fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG)|(!l&&r->t==SLL_OBJECT_TYPE_COMMA?OPTIMIZER_FLAG_ARGUMENT:0));
					if (o_dt->rm){
						o=_remove_up_to_end(o,l);
						r->dt.sc-=l;
						break;
					}
				}
				if (!r->dt.sc){
					if (fl&OPTIMIZER_FLAG_ARGUMENT){
						r->t=SLL_OBJECT_TYPE_INT;
						r->dt.i=0;
					}
					else{
						r->t=SLL_OBJECT_TYPE_NOP;
						DECREASE_PARENT(p);
					}
				}
				else if (r->dt.sc==1){
					r->t=SLL_OBJECT_TYPE_NOP;
				}
				else if (r->t==SLL_OBJECT_TYPE_OPERATION_LIST&&p&&(p->t==SLL_OBJECT_TYPE_FUNC||p->t==SLL_OBJECT_TYPE_INLINE_FUNC||p->t==SLL_OBJECT_TYPE_OPERATION_LIST)){
					p->dt.sc+=r->dt.sc-1;
					r->t=SLL_OBJECT_TYPE_NOP;
				}
				return o;
			}
		default:
			SLL_UNREACHABLE();
			return 0;
	}
}



static sll_object_t* _remap_indexes(sll_object_t* o,sll_object_t* p,optimizer_data_t* o_dt,sll_function_index_t* fn_m){
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
		o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
		case SLL_OBJECT_TYPE_CHAR:
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
		case SLL_OBJECT_TYPE_STRING:
			return o+1;
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_array_length_t l=o->dt.al;
				sll_object_t* r=o;
				o++;
				while (l){
					l--;
					o=_remap_indexes(o,r,o_dt,fn_m);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				sll_object_t* r=o;
				o++;
				while (l){
					l--;
					o=_remap_indexes(o,r,o_dt,fn_m);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_IDENTIFIER:
			SLL_ASSERT(GET_VARIABLE_REF_COUNT(o,o_dt));
			if (SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
				o->dt.id=*(o_dt->im.l+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id));
			}
			else{
				o->dt.id=*(o_dt->im.s[SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id));
			}
			return o+1;
		case SLL_OBJECT_TYPE_FUNCTION_ID:
			o->dt.fn_id=*(fn_m+o->dt.fn_id);
			return o+1;
		case SLL_OBJECT_TYPE_ASSIGN:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l>=2);
				sll_object_t* r=o;
				o++;
				while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
					o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				SLL_ASSERT(o->t==SLL_OBJECT_TYPE_IDENTIFIER);
				uint8_t rm=!GET_VARIABLE_REF_COUNT(o,o_dt);
				if (rm){
					r->t=SLL_OBJECT_TYPE_OPERATION_LIST;
					r->dt.sc=l-1;
					o->t=SLL_OBJECT_TYPE_NOP;
				}
				else{
					if (SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
						o->dt.id=*(o_dt->im.l+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id));
					}
					else{
						o->dt.id=*(o_dt->im.s[SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id));
					}
				}
				o++;
				l--;
				while (l){
					l--;
					o=_remap_indexes(o,r,o_dt,fn_m);
				}
				if (rm){
					_check_remove(r,p,o_dt);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_FUNC:
			o->dt.fn.id=*(fn_m+o->dt.fn.id);
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				sll_object_t* r=o;
				o++;
				while (l){
					l--;
					o=_remap_indexes(o,r,o_dt,fn_m);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_COMMA:
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_statement_count_t l=o->dt.sc;
				sll_object_t* r=o;
				o++;
				while (l){
					l--;
					o=_remap_indexes(o,r,o_dt,fn_m);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->dt.ac;
	sll_object_t* r=o;
	o++;
	while (l){
		l--;
		o=_remap_indexes(o,r,o_dt,fn_m);
	}
	return o;
}



__SLL_FUNC void sll_optimize_object(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft){
	optimizer_data_t o_dt={
		c_dt,
		i_ft,
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
		sll_function_t* fn=*(c_dt->ft.dt+i);
		if (!fn){
			continue;
		}
		o_dt.it.n_vi=o_dt.it.vc;
		o_dt.it.l_sc=0;
		const sll_object_t* fo=_get_object_at_offset(c_dt,fn->off);
		SLL_ASSERT(fo->t==SLL_OBJECT_TYPE_FUNC);
		sll_arg_count_t ac=fo->dt.fn.ac;
		fo++;
		while (ac){
			ac--;
			fo=_map_identifiers(fo,c_dt,&(o_dt.it));
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
	o_dt.v=malloc(o_dt.it.vc*sizeof(sll_runtime_object_t*));
	for (sll_variable_index_t i=0;i<o_dt.it.vc;i++){
		sll_runtime_object_t* v=SLL_CREATE();
		v->t=RUNTIME_OBJECT_TYPE_UNKNOWN;
		*(o_dt.v+i)=v;
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
	sll_function_index_t fn_off=0;
	sll_function_index_t* fn_m=malloc(c_dt->ft.l*sizeof(sll_function_index_t));
	for (sll_function_index_t i=0;i<c_dt->ft.l;i++){
		sll_function_t* f=*(c_dt->ft.dt+i);
		if (!f){
			fn_off++;
			continue;
		}
		*(fn_m+i)=i-fn_off;
		*(c_dt->ft.dt+i-fn_off)=f;
		for (sll_arg_count_t k=0;k<f->al;k++){
			if (SLL_IDENTIFIER_GET_ARRAY_ID(f->a[k])==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
				SLL_ASSERT((o_dt.it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX(f->a[k]))->c);
				f->a[k]=*(o_dt.im.l+SLL_IDENTIFIER_GET_ARRAY_INDEX(f->a[k]));
			}
			else{
				SLL_ASSERT((o_dt.it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID(f->a[k])]+SLL_IDENTIFIER_GET_ARRAY_INDEX(f->a[k]))->c);
				f->a[k]=*(o_dt.im.s[SLL_IDENTIFIER_GET_ARRAY_ID(f->a[k])]+SLL_IDENTIFIER_GET_ARRAY_INDEX(f->a[k]));
			}
		}
	}
	if (fn_off){
		c_dt->ft.l-=fn_off;
		if (!c_dt->ft.l){
			free(c_dt->ft.dt);
			c_dt->ft.dt=NULL;
		}
		else{
			c_dt->ft.dt=realloc(c_dt->ft.dt,c_dt->ft.l*sizeof(sll_function_t*));
		}
	}
	_remap_indexes(c_dt->h,NULL,&o_dt,fn_m);
	free(fn_m);
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		free(o_dt.it.s_im[i]);
		free(o_dt.im.s[i]);
	}
	free(o_dt.it.l_im);
	free(o_dt.im.l);
	for (sll_variable_index_t i=0;i<o_dt.it.vc;i++){
		SLL_RELEASE(*(o_dt.v+i));
	}
	free(o_dt.v);
}
