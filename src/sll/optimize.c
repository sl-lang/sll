#include <sll/_sll_internal.h>
#include <sll/api/string.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/ift.h>
#include <sll/integer_heap_queue.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/runtime_object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
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
				(p)->dt.ac--; \
			} \
			else{ \
				(p)->dt.ac--; \
			} \
		} \
	} while (0)
#define DISABLE_REMOVE_VARIABLE(o,o_dt) \
	do{ \
		if (SLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){ \
			((o_dt)->it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->rm=0; \
		} \
		else{ \
			((o_dt)->it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->rm=0; \
		} \
	} while (0)
#define GET_VARIABLE_INDEX(o,o_dt) (SLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH?((o_dt)->it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->v:((o_dt)->it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->v)
#define GET_VARIABLE_REMOVE(o,o_dt) (SLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH?((o_dt)->it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->rm:((o_dt)->it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->rm)
#define INCREASE_PARENT(p,v) \
	do{ \
		if (p){ \
			if ((p)->t==SLL_OBJECT_TYPE_FUNC||(p)->t==SLL_OBJECT_TYPE_INTERNAL_FUNC){ \
				(p)->dt.fn.ac+=(v); \
			} \
			else if ((p)->t==SLL_OBJECT_TYPE_COMMA||(p)->t==SLL_OBJECT_TYPE_OPERATION_LIST){ \
				(p)->dt.ac+=(v); \
			} \
			else{ \
				(p)->dt.ac+=(v); \
			} \
		} \
	} while (0)



static void _save_var_data(optimizer_data_t* o_dt,variable_assignment_data_t* o){
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		o->s_sm[i]=malloc(o_dt->s_sm_l[i]*sizeof(uint64_t));
		memcpy(o->s_sm[i],o_dt->va.s_sm[i],o_dt->s_sm_l[i]*sizeof(uint64_t));
	}
	o->l_sm=malloc(o_dt->l_sm_l*sizeof(uint64_t));
	memcpy(o->l_sm,o_dt->va.l_sm,o_dt->l_sm_l*sizeof(uint64_t));
}



static void _restore_var_data(optimizer_data_t* o_dt,variable_assignment_data_t* va_dt,uint8_t s){
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		if (s){
			for (uint32_t j=0;j<o_dt->s_sm_l[i];j++){
				uint64_t v=(*(o_dt->va.s_sm[i]+j))^(*(va_dt->s_sm[i]+j));
				while (v){
					sll_variable_index_t k=(o_dt->it.s_im[i]+((j<<6)|FIND_FIRST_SET_BIT(v)))->v;
					sll_runtime_object_t* n=SLL_CREATE();
					n->t=RUNTIME_OBJECT_TYPE_UNKNOWN;
					SLL_RELEASE(*(o_dt->v+k));
					*(o_dt->v+k)=n;
					v&=v-1;
				}
			}
		}
		memcpy(o_dt->va.s_sm[i],va_dt->s_sm[i],o_dt->s_sm_l[i]*sizeof(uint64_t));
		free(va_dt->s_sm[i]);
	}
	if (s){
		for (uint32_t i=0;i<o_dt->l_sm_l;i++){
			uint64_t v=(*(o_dt->va.l_sm+i))^(*(va_dt->l_sm+i));
			while (v){
				sll_variable_index_t j=(o_dt->it.l_im+((i<<6)|FIND_FIRST_SET_BIT(v)))->v;
				sll_runtime_object_t* n=SLL_CREATE();
				n->t=RUNTIME_OBJECT_TYPE_UNKNOWN;
				SLL_RELEASE(*(o_dt->v+j));
				*(o_dt->v+j)=n;
				v&=v-1;
			}
		}
	}
	memcpy(o_dt->va.l_sm,va_dt->l_sm,o_dt->l_sm_l*sizeof(uint64_t));
	free(va_dt->l_sm);
}



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
				sll_arg_count_t l=o->dt.ac;
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
		if (s->c==c&&s->l==al+bl&&!memcmp(a,s->v,al)&&!memcmp(b,s->v+al,bl)){
			return i;
		}
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
				sll_arg_count_t l=o->dt.ac;
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



static sll_runtime_object_t* _get_as_runtime_object(const sll_object_t* o,const optimizer_data_t* o_dt,sll_bool_t st){
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
		o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_CHAR:
			if (!st){
				return SLL_FROM_CHAR(o->dt.c);
			}
			else{
				sll_runtime_object_t* v=SLL_CREATE();
				v->t=SLL_RUNTIME_OBJECT_TYPE_CHAR;
				v->dt.c=o->dt.c;
				return v;
			}
		case SLL_OBJECT_TYPE_INT:
			if (!st){
				return SLL_FROM_INT(o->dt.i);
			}
			else{
				sll_runtime_object_t* v=SLL_CREATE();
				v->t=SLL_RUNTIME_OBJECT_TYPE_INT;
				v->dt.i=o->dt.i;
				return v;
			}
		case SLL_OBJECT_TYPE_FLOAT:
			if (!st){
				return SLL_FROM_FLOAT(o->dt.f);
			}
			else{
				sll_runtime_object_t* v=SLL_CREATE();
				v->t=SLL_RUNTIME_OBJECT_TYPE_FLOAT;
				v->dt.f=o->dt.f;
				return v;
			}
		case SLL_OBJECT_TYPE_STRING:
			{
				sll_runtime_object_t* v=SLL_CREATE();
				v->t=SLL_RUNTIME_OBJECT_TYPE_STRING|RUNTIME_OBJECT_EXTERNAL_STRING;
				SLL_ASSERT(o->dt.s<o_dt->c_dt->st.l);
				v->dt.s=*(o_dt->c_dt->st.dt+o->dt.s);
				return v;
			}
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_array_length_t l=o->dt.al;
				sll_runtime_object_t* v=SLL_CREATE();
				v->t=SLL_RUNTIME_OBJECT_TYPE_ARRAY;
				sll_array_create(l,&(v->dt.a));
				o++;
				for (sll_array_length_t i=0;i<l;i++){
					sll_runtime_object_t* n=_get_as_runtime_object(o,o_dt,st);
					o=sll_skip_object_const(o);
					v->dt.a.v[i]=n;
					if (n->t==RUNTIME_OBJECT_TYPE_UNKNOWN){
						while (1){
							SLL_RELEASE(v->dt.a.v[i]);
							if (!i){
								break;
							}
							i--;
						}
						free(v->dt.a.v);
						v->t=RUNTIME_OBJECT_TYPE_UNKNOWN;
						break;
					}
				}
				return v;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				sll_runtime_object_t* v=SLL_CREATE();
				v->t=SLL_RUNTIME_OBJECT_TYPE_MAP;
				sll_map_create((l+1)>>1,&(v->dt.m));
				o++;
				for (sll_map_length_t i=0;i<l;i++){
					sll_runtime_object_t* n=_get_as_runtime_object(o,o_dt,st);
					o=sll_skip_object_const(o);
					v->dt.a.v[i]=n;
					if (n->t==RUNTIME_OBJECT_TYPE_UNKNOWN){
						while (1){
							SLL_RELEASE(v->dt.a.v[i]);
							if (!i){
								break;
							}
							i--;
						}
						free(v->dt.a.v);
						v->t=RUNTIME_OBJECT_TYPE_UNKNOWN;
						break;
					}
				}
				if (l&1){
					v->dt.a.v[l]=SLL_ACQUIRE_STATIC_INT(0);
				}
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
		case SLL_OBJECT_TYPE_SWITCH:
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
			if (!st){
				return SLL_FROM_INT(~((sll_integer_t)(o->dt.fn.id)));
			}
			else{
				sll_runtime_object_t* v=SLL_CREATE();
				v->t=SLL_RUNTIME_OBJECT_TYPE_INT;
				v->dt.i=~((sll_integer_t)(o->dt.fn.id));
				return v;
			}
		case SLL_OBJECT_TYPE_CAST:
			{
				if (!o->dt.ac){
					return SLL_ACQUIRE_STATIC_INT(0);
				}
				sll_arg_count_t l=o->dt.ac-1;
				o++;
				if (o->t==OBJECT_TYPE_CHANGE_STACK){
					o=o->dt._p;
				}
				sll_runtime_object_t* v=_get_as_runtime_object(o,o_dt,st);
				if (SLL_RUNTIME_OBJECT_GET_TYPE(v)==RUNTIME_OBJECT_TYPE_UNKNOWN){
					return v;
				}
				while (l){
					l--;
					o=sll_skip_object_const(o);
					while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
						o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
					}
					sll_runtime_object_t* t=_get_as_runtime_object(o,o_dt,st);
					if (SLL_RUNTIME_OBJECT_GET_TYPE(t)==RUNTIME_OBJECT_TYPE_UNKNOWN){
						SLL_RELEASE(v);
						return t;
					}
					sll_runtime_object_t* n=sll_operator_cast(v,t);
					SLL_RELEASE(v);
					SLL_RELEASE(t);
					if (SLL_RUNTIME_OBJECT_GET_TYPE(n)==RUNTIME_OBJECT_TYPE_UNKNOWN){
						return n;
					}
					v=n;
				}
				return v;
			}
		case SLL_OBJECT_TYPE_COMMA:
			{
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					return SLL_ACQUIRE_STATIC_INT(0);
				}
				o++;
				while (l>1){
					l--;
					o=sll_skip_object_const(o);
				}
				return _get_as_runtime_object(o,o_dt,st);
			}
	}
	sll_runtime_object_t* v=SLL_CREATE();
	v->t=RUNTIME_OBJECT_TYPE_UNKNOWN;
	return v;
}



static sll_object_t* _runtime_object_to_object(sll_runtime_object_t* v,sll_object_t* o,optimizer_data_t* o_dt){
	if (o->t==OBJECT_TYPE_CHANGE_STACK){
		o=o->dt._p;
	}
	switch (SLL_RUNTIME_OBJECT_GET_TYPE(v)){
		case SLL_RUNTIME_OBJECT_TYPE_INT:
			o->t=SLL_OBJECT_TYPE_INT;
			o->dt.i=v->dt.i;
			return o;
		case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
			o->t=SLL_OBJECT_TYPE_FLOAT;
			o->dt.f=v->dt.f;
			return o;
		case SLL_RUNTIME_OBJECT_TYPE_CHAR:
			o->t=SLL_OBJECT_TYPE_CHAR;
			o->dt.c=v->dt.c;
			return o;
		case SLL_RUNTIME_OBJECT_TYPE_STRING:
			o->t=SLL_OBJECT_TYPE_STRING;
			o->dt.s=0;
			while (o_dt->c_dt->st.dt+o->dt.s!=&(v->dt.s)){
				o->dt.s++;
				if (o->dt.s==o_dt->c_dt->st.l){
					v->t|=RUNTIME_OBJECT_EXTERNAL_STRING;
					o->dt.s=sll_add_string(&(o_dt->c_dt->st),&(v->dt.s),0);
					return o;
				}
			}
			SLL_ASSERT(o->dt.s<o_dt->c_dt->st.l);
			return o;
		case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
			o->t=SLL_OBJECT_TYPE_ARRAY;
			o->dt.al=v->dt.a.l;
			_shift_objects(o,o_dt->c_dt,v->dt.a.l);
			for (sll_array_length_t i=0;i<v->dt.a.l;i++){
				o=_runtime_object_to_object(v->dt.a.v[i],o+1,o_dt);
			}
			return o;
		case SLL_RUNTIME_OBJECT_TYPE_HANDLE:
			SLL_UNIMPLEMENTED();
		case SLL_RUNTIME_OBJECT_TYPE_MAP:
			{
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_length_t l=v->dt.m.l<<1;
				o->dt.ml=l;
				_shift_objects(o,o_dt->c_dt,l);
				for (sll_map_length_t i=0;i<l;i++){
					o=_runtime_object_to_object(v->dt.m.v[i],o+1,o_dt);
				}
				return o;
			}
		case RUNTIME_OBJECT_TYPE_FUNCTION_ID:
			o->t=SLL_OBJECT_TYPE_FUNCTION_ID;
			o->dt.fn_id=(sll_function_index_t)(v->dt.i);
			return o;
		case RUNTIME_OBJECT_TYPE_UNKNOWN:
			SLL_UNIMPLEMENTED();
		default:
			SLL_UNREACHABLE();
	}
	return o;
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
				sll_runtime_object_t* tmp=_get_as_runtime_object(o+1,o_dt,0);
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
				sll_arg_count_t l=o->dt.ac;
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
		case SLL_OBJECT_TYPE_REF:
			return (((!o->dt.ac)^inv)?COND_TYPE_ALWAYS_FALSE:COND_TYPE_ALWAYS_TRUE);
		case SLL_OBJECT_TYPE_PRINT:
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
		case SLL_OBJECT_TYPE_IF:
		case SLL_OBJECT_TYPE_SWITCH:
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
		case SLL_OBJECT_TYPE_INLINE_IF:
			SLL_UNIMPLEMENTED();
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
			return COND_TYPE_UNKNOWN;
		default:
			SLL_UNREACHABLE();
	}
	return COND_TYPE_UNKNOWN;
}



static sll_object_t* _inline_function(sll_object_t* o,optimizer_data_t* o_dt){
	SLL_ASSERT(o->t==SLL_OBJECT_TYPE_CALL);
	return sll_skip_object(o);
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
				sll_runtime_object_t* rt=_get_as_runtime_object(o,o_dt,0);
				if (rt->t==SLL_RUNTIME_OBJECT_TYPE_INT&&rt->dt.i<0){
					sll_function_index_t i=(sll_function_index_t)(~(rt->dt.i));
					if (i<o_dt->i_ft->l&&!((*(o_dt->i_ft->dt+i))->t&SLL_INTERNAL_FUNCTION_FLAG_REQUIRED)){
						r->t=SLL_OBJECT_TYPE_OPERATION_LIST;
						r->dt.ac=l;
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
		case SLL_OBJECT_TYPE_CAST:
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
				sll_arg_count_t l=o->dt.ac;
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
	if (o_dt->a_v){
		SLL_RELEASE(*(o_dt->a_v));
		*(o_dt->a_v)=_get_as_runtime_object(o,o_dt,1);
		o_dt->a_v=NULL;
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
				if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
					o->t=SLL_OBJECT_TYPE_OPERATION_LIST;
					o->dt.ac=o->dt.al;
					goto _optimize_operation_list_comma;
				}
				sll_object_t* r=o;
				sll_array_length_t l=o->dt.al;
				o++;
				while (l){
					l--;
					o=_optimize(o,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
					if (o_dt->rm){
						r->dt.al-=l;
						o=_remove_up_to_end(o,l);
						break;
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
				if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
					o->t=SLL_OBJECT_TYPE_OPERATION_LIST;
					o->dt.ac=o->dt.ml;
					goto _optimize_operation_list_comma;
				}
				sll_object_t* r=o;
				sll_map_length_t l=o->dt.ml;
				o++;
				while (l){
					l--;
					o=_optimize(o,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
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
				o->t=SLL_OBJECT_TYPE_NOP;
			}
			else{
				sll_runtime_object_t* v=*(o_dt->v+GET_VARIABLE_INDEX(o,o_dt));
				if (SLL_RUNTIME_OBJECT_GET_TYPE(v)!=RUNTIME_OBJECT_TYPE_UNKNOWN&&(!(v->t&RUNTIME_OBJECT_CHANGE_IN_LOOP)||(fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG))){
					o=_runtime_object_to_object(v,o,o_dt);
				}
				else{
					DISABLE_REMOVE_VARIABLE(o,o_dt);
				}
			}
			return o+1;
		case SLL_OBJECT_TYPE_PRINT:
			{
				sll_arg_count_t l=o->dt.ac;
				sll_object_t* r=o;
				o++;
				while (l){
					l--;
					sll_object_t* a=o;
					o=_optimize(o,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
					if (o_dt->rm){
						SLL_UNIMPLEMENTED();
					}
					sll_runtime_object_t* v=_get_as_runtime_object(a,o_dt,0);
					if (SLL_RUNTIME_OBJECT_GET_TYPE(v)!=RUNTIME_OBJECT_TYPE_UNKNOWN&&!(v->t&RUNTIME_OBJECT_CHANGE_IN_LOOP)){
						while (a->t==SLL_OBJECT_TYPE_NOP||a->t==SLL_OBJECT_TYPE_DEBUG_DATA||a->t==OBJECT_TYPE_CHANGE_STACK){
							a=(a->t==OBJECT_TYPE_CHANGE_STACK?a->dt._p:a+1);
						}
						if (a->t==SLL_OBJECT_TYPE_STRING){
							sll_string_t* s=o_dt->c_dt->st.dt+a->dt.s;
							if (!s->l){
								r->dt.ac--;
								a->t=SLL_OBJECT_TYPE_NOP;
								SLL_RELEASE(v);
								continue;
							}
							else if (s->l==1){
								a->t=SLL_OBJECT_TYPE_CHAR;
								a->dt.c=s->v[0];
							}
						}
						else if (a->t!=SLL_OBJECT_TYPE_CHAR){
							_shift_objects(a,o_dt->c_dt,1);
							a->t=SLL_OBJECT_TYPE_COMMA;
							a->dt.ac=2;
							if (o->t==OBJECT_TYPE_CHANGE_STACK){
								o=o->dt._p;
							}
							o++;
							if (o->t==OBJECT_TYPE_CHANGE_STACK){
								o=o->dt._p;
							}
							_shift_objects(o,o_dt->c_dt,1);
							sll_string_t s;
							sll_object_to_string((const sll_runtime_object_t*const*)&v,1,&s);
							o->t=SLL_OBJECT_TYPE_STRING;
							o->dt.i=sll_add_string(&(o_dt->c_dt->st),&s,1);
							o=_optimize(a,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
						}
					}
					SLL_RELEASE(v);
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
				sll_runtime_object_t* rt=_get_as_runtime_object(v,o_dt,0);
				if (rt->t!=RUNTIME_OBJECT_TYPE_UNKNOWN){
					_shift_objects(o,o_dt->c_dt,1);
					if (r->t==SLL_OBJECT_TYPE_NOT){
						o->t=SLL_OBJECT_TYPE_INT;
						o->dt.i=!sll_operator_bool(rt);
					}
					else if (r->t==SLL_OBJECT_TYPE_BIT_NOT){
						sll_runtime_object_t* nv=sll_operator_inv(rt);
						o=_runtime_object_to_object(nv,o,o_dt);
						SLL_RELEASE(nv);
					}
					else{
						sll_runtime_object_t* nv=sll_operator_len(rt);
						SLL_ASSERT(SLL_RUNTIME_OBJECT_GET_TYPE(nv)==SLL_RUNTIME_OBJECT_TYPE_INT);
						o->t=SLL_OBJECT_TYPE_INT;
						o->dt.i=nv->dt.i;
						SLL_RELEASE(nv);
					}
					r->t=SLL_OBJECT_TYPE_COMMA;
					r->dt.ac++;
					o=r;
					SLL_RELEASE(rt);
					goto _optimize_operation_list_comma;
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
				sll_object_t* id_o=o;
				o=sll_skip_object(o);
				while (id_o->t==SLL_OBJECT_TYPE_NOP||id_o->t==SLL_OBJECT_TYPE_DEBUG_DATA||id_o->t==OBJECT_TYPE_CHANGE_STACK){
					id_o=(id_o->t==OBJECT_TYPE_CHANGE_STACK?id_o->dt._p:id_o+1);
				}
				if (id_o->t==SLL_OBJECT_TYPE_IDENTIFIER){
					if (SLL_IDENTIFIER_GET_ARRAY_ID(id_o->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
						o_dt->a_v=o_dt->v+(o_dt->it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX(id_o->dt.id))->v;
						(*(o_dt->va.l_sm+(SLL_IDENTIFIER_GET_ARRAY_INDEX(id_o->dt.id)>>6)))|=1ull<<(SLL_IDENTIFIER_GET_ARRAY_INDEX(id_o->dt.id)&63);
					}
					else{
						o_dt->a_v=o_dt->v+(o_dt->it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID(id_o->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(id_o->dt.id))->v;
						(*(o_dt->va.s_sm[SLL_IDENTIFIER_GET_ARRAY_ID(id_o->dt.id)]+(SLL_IDENTIFIER_GET_ARRAY_INDEX(id_o->dt.id)>>6)))|=1ull<<(SLL_IDENTIFIER_GET_ARRAY_INDEX(id_o->dt.id)&63);
					}
				}
				else{
					SLL_UNIMPLEMENTED();
				}
				o=_optimize(o,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
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
					o->dt.ac=o->dt.fn.ac;
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
				if (!r->dt.ac){
					r->t=SLL_OBJECT_TYPE_INT;
					r->dt.i=~((sll_integer_t)r->dt.fn.id);
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
					SLL_UNIMPLEMENTED();
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
					sll_runtime_object_t* rt=_get_as_runtime_object(fn,o_dt,0);
					if (rt->t==SLL_RUNTIME_OBJECT_TYPE_INT&&rt->dt.i<0){
						sll_function_index_t i=(sll_function_index_t)(~(rt->dt.i));
						if (i<o_dt->i_ft->l&&!((*(o_dt->i_ft->dt+i))->t&SLL_INTERNAL_FUNCTION_FLAG_REQUIRED)){
							r->t=SLL_OBJECT_TYPE_OPERATION_LIST;
							_optimize(r,p,o_dt,fl);
						}
					}
					SLL_RELEASE(rt);
				}
				else if (fn->t==SLL_OBJECT_TYPE_INT&&fn->dt.i<0){
					sll_object_t* e=o;
					sll_function_index_t i=(sll_function_index_t)(~(fn->dt.i));
					if (i<o_dt->i_ft->l&&((*(o_dt->i_ft->dt+i))->t&SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL)){
						sll_runtime_object_t** al=malloc((r->dt.ac-1)*sizeof(sll_runtime_object_t*));
						o=fn;
						for (sll_arg_count_t j=1;j<r->dt.ac;j++){
							o=sll_skip_object(o);
							sll_runtime_object_t* n=_get_as_runtime_object(o,o_dt,0);
							*(al+j-1)=n;
							if (n->t==RUNTIME_OBJECT_TYPE_UNKNOWN){
								do{
									j--;
									SLL_RELEASE(*(al+j));
								} while (j);
								free(al);
								return e;
							}
						}
						if (al){
							sll_runtime_object_t* n=(*(o_dt->i_ft->dt+i))->p((const sll_runtime_object_t*const*)al,r->dt.ac-1);
							for (sll_arg_count_t j=0;j<r->dt.ac-1;j++){
								SLL_RELEASE(*(al+j));
							}
							free(al);
							r->t=SLL_OBJECT_TYPE_COMMA;
							r->dt.ac++;
							o=sll_skip_object(o);
							_shift_objects(o,o_dt->c_dt,1);
							_runtime_object_to_object(n,o,o_dt);
							SLL_RELEASE(n);
							return _optimize(r,p,o_dt,fl);
						}
					}
				}
				return o;
			}
		case SLL_OBJECT_TYPE_IF:
		case SLL_OBJECT_TYPE_INLINE_IF:
			{
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
						DECREASE_PARENT(p);
						o->t=SLL_OBJECT_TYPE_NOP;
					}
					else{
						o->t=SLL_OBJECT_TYPE_INT;
						o->dt.i=0;
					}
					return o+1;
				}
				if (l==1){
					o->t=SLL_OBJECT_TYPE_OPERATION_LIST;
					goto _optimize_operation_list_comma;
				}
				variable_assignment_data_t va_dt;
				_save_var_data(o_dt,&va_dt);
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
							if (r->dt.ac>3){
								_shift_objects(cnd_o,o_dt->c_dt,1);
								cnd_o->t=SLL_OBJECT_TYPE_COMMA;
								cnd_o->dt.ac=2;
								o++;
								if (o->t==OBJECT_TYPE_CHANGE_STACK){
									o=o->dt._p;
								}
								o=_optimize(o,cnd_o,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
								_remove_up_to_end(o,(l<<1)+1);
								r->dt.ac-=(l<<1)+2;
								if (r->dt.ac==1){
									_restore_var_data(o_dt,&va_dt,0);
									if (r->t==SLL_OBJECT_TYPE_IF){
										r->t=SLL_OBJECT_TYPE_OPERATION_LIST;
										r->dt.ac=1;
										o=r;
										goto _optimize_operation_list_comma;
									}
									r->t=SLL_OBJECT_TYPE_NOP;
								}
								else{
									_restore_var_data(o_dt,&va_dt,1);
								}
								o=cnd_o;
								goto _optimize_operation_list_comma;
							}
							r->t=(r->t==SLL_OBJECT_TYPE_INLINE_IF?SLL_OBJECT_TYPE_COMMA:SLL_OBJECT_TYPE_OPERATION_LIST);
							r->dt.ac--;
							_remove_single_object(_optimize(o,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT));
							_restore_var_data(o_dt,&va_dt,0);
							o=r;
							goto _optimize_operation_list_comma;
						}
						else if (cnd==COND_TYPE_ALWAYS_FALSE){
							if (r->dt.ac==3){
								r->t=(r->t==SLL_OBJECT_TYPE_INLINE_IF?SLL_OBJECT_TYPE_COMMA:SLL_OBJECT_TYPE_OPERATION_LIST);
								r->dt.ac=2;
								_remove_single_object(o);
								_restore_var_data(o_dt,&va_dt,0);
								o=r;
								goto _optimize_operation_list_comma;
							}
							SLL_UNIMPLEMENTED();
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
							if (r->dt.ac>2){
								_shift_objects(cnd_o,o_dt->c_dt,1);
								cnd_o->t=SLL_OBJECT_TYPE_COMMA;
								cnd_o->dt.ac=2;
								o++;
								if (o->t==OBJECT_TYPE_CHANGE_STACK){
									o=o->dt._p;
								}
								o=_optimize(o,cnd_o,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
								_remove_up_to_end(o,l<<1);
								r->dt.ac-=(l<<1)+1;
								if (r->dt.ac==1){
									_restore_var_data(o_dt,&va_dt,0);
									if (r->t==SLL_OBJECT_TYPE_IF){
										r->t=SLL_OBJECT_TYPE_OPERATION_LIST;
										r->dt.ac=1;
										o=r;
										goto _optimize_operation_list_comma;
									}
									r->t=SLL_OBJECT_TYPE_NOP;
								}
								else{
									_restore_var_data(o_dt,&va_dt,1);
								}
								o=cnd_o;
								goto _optimize_operation_list_comma;
							}
							r->t=(r->t==SLL_OBJECT_TYPE_INLINE_IF?SLL_OBJECT_TYPE_COMMA:SLL_OBJECT_TYPE_OPERATION_LIST);
							_optimize(o,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
							_restore_var_data(o_dt,&va_dt,0);
							o=r;
							goto _optimize_operation_list_comma;
						}
						else if (cnd==COND_TYPE_ALWAYS_FALSE){
							if (r->dt.ac==2){
								r->t=(r->t==SLL_OBJECT_TYPE_INLINE_IF?SLL_OBJECT_TYPE_COMMA:SLL_OBJECT_TYPE_OPERATION_LIST);
								r->dt.ac=1;
								_remove_single_object(o);
								_restore_var_data(o_dt,&va_dt,0);
								o=r;
								goto _optimize_operation_list_comma;
							}
							SLL_UNIMPLEMENTED();
							continue;
						}
						o=_optimize(o,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
						o_dt->rm=0;
					}
				}
				_restore_var_data(o_dt,&va_dt,(r->t!=SLL_OBJECT_TYPE_NOP));
				return o;
			}
		case SLL_OBJECT_TYPE_SWITCH:
			{
				sll_arg_count_t l=o->dt.ac;
				if (l==1){
					o->t=SLL_OBJECT_TYPE_NOP;
					return _optimize(o+1,p,o_dt,fl);
				}
				if (l==2){
					o->t=SLL_OBJECT_TYPE_OPERATION_LIST;
					goto _optimize_operation_list_comma;
				}
				sll_object_t* r=o;
				if (l==3){
					SLL_UNIMPLEMENTED();
				}
				if (l==4){
					SLL_UNIMPLEMENTED();
				}
				o=_optimize(o+1,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
				sll_object_t* cnd_n_o=o;
				sll_runtime_object_t* cnd=_get_as_runtime_object(r+1,o_dt,0);
				if ((SLL_RUNTIME_OBJECT_GET_TYPE(cnd)!=SLL_RUNTIME_OBJECT_TYPE_INT&&SLL_RUNTIME_OBJECT_GET_TYPE(cnd)!=SLL_RUNTIME_OBJECT_TYPE_CHAR)||(cnd->t&RUNTIME_OBJECT_CHANGE_IN_LOOP)){
					SLL_RELEASE(cnd);
					cnd=NULL;
				}
				else if (SLL_RUNTIME_OBJECT_GET_TYPE(cnd)==SLL_RUNTIME_OBJECT_TYPE_CHAR){
					sll_char_t v=cnd->dt.c;
					SLL_RELEASE(cnd);
					cnd=SLL_FROM_INT(v);
				}
				SLL_ASSERT(!cnd||SLL_RUNTIME_OBJECT_GET_TYPE(cnd)==SLL_RUNTIME_OBJECT_TYPE_INT);
				l=(l-1)>>1;
				sll_integer_heap_queue_t il=SLL_ZERO_INTEGER_HEAP_QUEUE_STRUCT;
				do{
					l--;
					sll_object_t* a=o;
					o=_optimize(o,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
					while (a->t==SLL_OBJECT_TYPE_NOP||a->t==SLL_OBJECT_TYPE_DEBUG_DATA||a->t==OBJECT_TYPE_CHANGE_STACK){
						a=(a->t==OBJECT_TYPE_CHANGE_STACK?a->dt._p:a+1);
					}
					if (a->t!=SLL_OBJECT_TYPE_INT&&a->t!=SLL_OBJECT_TYPE_CHAR){
						SLL_UNIMPLEMENTED();
					}
					sll_integer_t i=(a->t==SLL_OBJECT_TYPE_INT?a->dt.i:a->dt.c);
					if (cnd&&i==cnd->dt.i){
						sll_free_integer_heap_queue(&il);
						SLL_RELEASE(cnd);
						r->t=SLL_OBJECT_TYPE_OPERATION_LIST;
						_remove_up_to_end(cnd_n_o,((r->dt.ac-1)&0xfffffffe)-(l<<1)-1);
						o=_optimize(o,r,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
						_remove_up_to_end(o,(l<<1)+(!(r->dt.ac&1)));
						r->dt.ac=2;
						o=r;
						goto _optimize_operation_list_comma;
					}
					else if (!sll_integer_heap_queue_add(&il,i)){
						o=_remove_single_object(_remove_single_object(a));
					}
					else{
						o=_optimize(o,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
					}
				} while (l);
				sll_free_integer_heap_queue(&il);
				if (!(o->dt.ac&1)){
					o=_optimize(o,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
				}
				if (cnd){
					SLL_RELEASE(cnd);
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
				variable_assignment_data_t va_dt;
				_save_var_data(o_dt,&va_dt);
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
				_restore_var_data(o_dt,&va_dt,1);
				return o;
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
			if (o->dt.ac==1){
				o->t=SLL_OBJECT_TYPE_COMMA;
				o->dt.ac=2;
				sll_object_t* a=sll_skip_object(o+1);
				_shift_objects(a,o_dt->c_dt,1);
				a->t=SLL_OBJECT_TYPE_INT;
				a->dt.i=1;
				goto _optimize_operation_list_comma;
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
		case SLL_OBJECT_TYPE_BIT_RSHIFT:
		case SLL_OBJECT_TYPE_BIT_LSHIFT:
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
				sll_object_t* pa=NULL;
				while (l){
					l--;
					sll_object_t* a=o;
					o=_optimize(o,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
					if (o_dt->rm){
						SLL_UNIMPLEMENTED();
					}
					sll_runtime_object_t* av=_get_as_runtime_object(a,o_dt,0);
					if (SLL_RUNTIME_OBJECT_GET_TYPE(av)!=RUNTIME_OBJECT_TYPE_UNKNOWN&&!(av->t&RUNTIME_OBJECT_CHANGE_IN_LOOP)&&(!v||!i||SLL_RUNTIME_OBJECT_IS_STORAGE(av)==SLL_RUNTIME_OBJECT_IS_STORAGE(v))){
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
								case SLL_OBJECT_TYPE_SUB:
									nv=sll_operator_sub(v,av);
									break;
								case SLL_OBJECT_TYPE_MULT:
									nv=sll_operator_mult(v,av);
									break;
								case SLL_OBJECT_TYPE_DIV:
									nv=sll_operator_div(v,av);
									break;
								case SLL_OBJECT_TYPE_FLOOR_DIV:
									nv=sll_operator_floor_div(v,av);
									break;
								case SLL_OBJECT_TYPE_MOD:
									nv=sll_operator_mod(v,av);
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
								case SLL_OBJECT_TYPE_BIT_RSHIFT:
									nv=sll_operator_shr(v,av);
									break;
								case SLL_OBJECT_TYPE_BIT_LSHIFT:
									nv=sll_operator_shl(v,av);
									break;
								case SLL_OBJECT_TYPE_LESS:
								case SLL_OBJECT_TYPE_LESS_EQUAL:
								case SLL_OBJECT_TYPE_MORE:
								case SLL_OBJECT_TYPE_MORE_EQUAL:
									{
										sll_compare_result_t cmp=sll_operator_compare(v,av);
										if ((r->t==SLL_OBJECT_TYPE_LESS&&cmp==SLL_COMPARE_RESULT_BELOW)||(r->t==SLL_OBJECT_TYPE_LESS_EQUAL&&cmp!=SLL_COMPARE_RESULT_ABOVE)||(r->t==SLL_OBJECT_TYPE_MORE&&cmp==SLL_COMPARE_RESULT_ABOVE)||(r->t==SLL_OBJECT_TYPE_MORE_EQUAL&&cmp!=SLL_COMPARE_RESULT_BELOW)){
_join_cond:
											_shift_objects(pa,o_dt->c_dt,1);
											r->dt.ac--;
											pa->t=SLL_OBJECT_TYPE_COMMA;
											pa->dt.ac=2;
											a++;
											if (a->t==OBJECT_TYPE_CHANGE_STACK){
												a=a->dt._p;
											}
											o++;
											if (o->t==OBJECT_TYPE_CHANGE_STACK){
												o=o->dt._p;
											}
											_optimize(pa,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
											SLL_ACQUIRE(av);
											nv=av;
											break;
										}
_remove_cond:
										SLL_RELEASE(v);
										SLL_RELEASE(av);
										free(arg);
										_remove_up_to_end(o,r->dt.ac-j-1);
										_shift_objects(pa,o_dt->c_dt,1);
										pa->t=SLL_OBJECT_TYPE_COMMA;
										pa->dt.ac=2;
										r->dt.ac=j;
										_shift_objects(r,o_dt->c_dt,1);
										r->t=SLL_OBJECT_TYPE_OPERATION_LIST;
										r->dt.ac=1;
										o=r;
										goto _optimize_operation_list_comma;
									}
								case SLL_OBJECT_TYPE_EQUAL:
								case SLL_OBJECT_TYPE_NOT_EQUAL:
									if ((!!(r->t==SLL_OBJECT_TYPE_NOT_EQUAL))^sll_operator_equal(v,av)){
										goto _join_cond;
									}
									goto _remove_cond;
								default:
									SLL_UNREACHABLE();
							}
							SLL_RELEASE(v);
							v=nv;
						}
						pa=a;
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
							_shift_objects(o,o_dt->c_dt,1);
							if (r->t>=SLL_OBJECT_TYPE_LESS){
								o->t=SLL_OBJECT_TYPE_INT;
								o->dt.i=1;
							}
							else{
								_runtime_object_to_object(v,o,o_dt);
							}
							SLL_RELEASE(v);
							r->t=SLL_OBJECT_TYPE_COMMA;
							r->dt.ac++;
							o=r;
							goto _optimize_operation_list_comma;
						}
						else{
							if (r->t>=SLL_OBJECT_TYPE_LESS){
								SLL_UNIMPLEMENTED();
							}
							r->dt.ac-=j-i-1;
							sll_object_t* tmp=*(arg+i);
							_shift_objects(tmp,o_dt->c_dt,1);
							tmp->t=SLL_OBJECT_TYPE_COMMA;
							tmp->dt.ac=j-i+1;
							o++;
							if (o->t==OBJECT_TYPE_CHANGE_STACK){
								o=o->dt._p;
							}
							_shift_objects(o,o_dt->c_dt,1);
							_runtime_object_to_object(v,o,o_dt);
							SLL_RELEASE(v);
							o=tmp;
							p=r;
							fl|=OPTIMIZER_FLAG_ARGUMENT;
							goto _optimize_operation_list_comma;
						}
					}
					SLL_RELEASE(v);
				}
				free(arg);
				return o;
			}
		case SLL_OBJECT_TYPE_ACCESS:
			{
				if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
					o->t=SLL_OBJECT_TYPE_OPERATION_LIST;
					goto _optimize_operation_list_comma;
				}
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					o->t=SLL_OBJECT_TYPE_INT;
					o->dt.i=0;
					return o+1;
				}
				sll_object_t* r=o;
				if (l==1){
					o=_optimize(o+1,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
					sll_runtime_object_t* v=_get_as_runtime_object(r+1,o_dt,0);
					if (SLL_RUNTIME_OBJECT_GET_TYPE(v)!=RUNTIME_OBJECT_TYPE_UNKNOWN&&!(v->t&RUNTIME_OBJECT_CHANGE_IN_LOOP)){
						r->t=SLL_OBJECT_TYPE_COMMA;
						r->dt.ac=2;
						_shift_objects(o,o_dt->c_dt,1);
						_runtime_object_to_object(v,o,o_dt);
						SLL_RELEASE(v);
						o=r;
						goto _optimize_operation_list_comma;
					}
					SLL_RELEASE(v);
					return o;
				}
				o++;
				sll_runtime_object_t* v[4]={NULL,NULL,NULL,NULL};
				sll_arg_count_t i=0;
				if (l>4){
					l=4;
				}
				while (i<l){
					sll_object_t* a=o;
					o=_optimize(o,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
					v[i]=_get_as_runtime_object(a,o_dt,0);
					if (SLL_RUNTIME_OBJECT_GET_TYPE(v[i])==RUNTIME_OBJECT_TYPE_UNKNOWN||(v[i]->t&RUNTIME_OBJECT_CHANGE_IN_LOOP)){
						break;
					}
					if (o_dt->rm){
						SLL_UNIMPLEMENTED();
					}
					i++;
				}
				if (i!=l){
					i++;
				}
				else{
					sll_runtime_object_t* n;
					SLL_ASSERT(i>1&&i<5);
					if (i==2){
						n=sll_operator_access(v[0],v[1]);
					}
					else if (i==3){
						n=sll_operator_access_range(v[0],v[1],v[2]);
					}
					else{
						n=sll_operator_access_range_step(v[0],v[1],v[2],v[3]);
					}
					while (i){
						i--;
						SLL_RELEASE(v[i]);
					}
					i=4;
					l=r->dt.ac;
					while (i<l){
						o=sll_skip_object(o);
						i++;
					}
					r->t=SLL_OBJECT_TYPE_COMMA;
					r->dt.ac++;
					_shift_objects(o,o_dt->c_dt,1);
					o=_runtime_object_to_object(n,o,o_dt)+1;
					SLL_RELEASE(n);
					o=r;
					goto _optimize_operation_list_comma;
				}
				for (sll_arg_count_t j=0;j<i;j++){
					SLL_RELEASE(v[j]);
				}
				l=r->dt.ac;
				while (i<l){
					o=_optimize(o,r,o_dt,(fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG)|(i<5?OPTIMIZER_FLAG_ARGUMENT:0));
					if (o_dt->rm){
						SLL_UNIMPLEMENTED();
					}
					i++;
				}
				return o;
			}
		case SLL_OBJECT_TYPE_CAST:
			{
				if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
					o->t=SLL_OBJECT_TYPE_OPERATION_LIST;
					goto _optimize_operation_list_comma;
				}
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					o->t=SLL_OBJECT_TYPE_INT;
					o->dt.i=0;
					return o+1;
				}
				sll_object_t* r=o;
				o=_optimize(o+1,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
				sll_runtime_object_t* v=_get_as_runtime_object(r+1,o_dt,0);
				l--;
				if (SLL_RUNTIME_OBJECT_GET_TYPE(v)!=RUNTIME_OBJECT_TYPE_UNKNOWN&&!(v->t&RUNTIME_OBJECT_CHANGE_IN_LOOP)){
					while (l){
						l--;
						sll_object_t* a=o;
						o=_optimize(o,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
						sll_runtime_object_t* n=_get_as_runtime_object(a,o_dt,0);
						if (SLL_RUNTIME_OBJECT_GET_TYPE(n)==RUNTIME_OBJECT_TYPE_UNKNOWN||(n->t&RUNTIME_OBJECT_CHANGE_IN_LOOP)){
							SLL_UNIMPLEMENTED();
						}
						sll_runtime_object_t* tmp=sll_operator_cast(v,n);
						SLL_RELEASE(v);
						SLL_RELEASE(n);
						v=tmp;
					}
					r->t=SLL_OBJECT_TYPE_COMMA;
					r->dt.ac++;
					_shift_objects(o,o_dt->c_dt,1);
					_runtime_object_to_object(v,o,o_dt);
					SLL_RELEASE(v);
					o=r;
					goto _optimize_operation_list_comma;
				}
				SLL_RELEASE(v);
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
		case SLL_OBJECT_TYPE_REF:
			{
				if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
					o->t=SLL_OBJECT_TYPE_OPERATION_LIST;
					goto _optimize_operation_list_comma;
				}
				sll_arg_count_t l=o->dt.ac;
				sll_object_t* r=o;
				o++;
				if (l){
					o=_optimize(o,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
					l--;
					while (l){
						l--;
						o=_optimize(o,r,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
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
_optimize_operation_list_comma:
			{
				sll_bool_t rm=(p&&(p->t==SLL_OBJECT_TYPE_FUNC||p->t==SLL_OBJECT_TYPE_INLINE_FUNC||p->t==SLL_OBJECT_TYPE_COMMA||p->t==SLL_OBJECT_TYPE_OPERATION_LIST));
				sll_arg_count_t l=o->dt.ac;
				sll_object_t* r=o;
				o++;
				while (l){
					l--;
					o=_optimize(o,r,o_dt,(fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG)|(!rm&&!l&&r->t==SLL_OBJECT_TYPE_COMMA?OPTIMIZER_FLAG_ARGUMENT:0));
					if (o_dt->rm){
						_remove_up_to_end(o,l);
						sll_object_t* e=r+1;
						r->dt.ac=0;
						do{
							r->dt.ac++;
							e=sll_skip_object(e);
						} while (e!=o);
						break;
					}
				}
				if (!r->dt.ac){
					if (fl&OPTIMIZER_FLAG_ARGUMENT){
						r->t=SLL_OBJECT_TYPE_INT;
						r->dt.i=0;
					}
					else{
						r->t=SLL_OBJECT_TYPE_NOP;
						DECREASE_PARENT(p);
					}
				}
				else if (r->dt.ac==1&&(r->t==SLL_OBJECT_TYPE_COMMA||!(fl&OPTIMIZER_FLAG_ARGUMENT))){
					r->t=SLL_OBJECT_TYPE_NOP;
				}
				else if ((p&&(p->t==SLL_OBJECT_TYPE_FUNC||p->t==SLL_OBJECT_TYPE_INLINE_FUNC||p->t==SLL_OBJECT_TYPE_COMMA||p->t==SLL_OBJECT_TYPE_OPERATION_LIST))){
					INCREASE_PARENT(p,r->dt.ac-1);
					r->t=SLL_OBJECT_TYPE_NOP;
				}
				return o;
			}
		default:
			SLL_UNREACHABLE();
			return 0;
	}
}



static sll_object_t* _merge_print_strings(sll_object_t* o,optimizer_data_t* o_dt){
	SLL_ASSERT(o->t==SLL_OBJECT_TYPE_PRINT);
	sll_arg_count_t l=o->dt.ac;
	sll_object_t* r=o;
	o++;
	sll_object_t* a=NULL;
	while (l){
		l--;
		while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
			o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
		}
		if (o->t==SLL_OBJECT_TYPE_CHAR||o->t==SLL_OBJECT_TYPE_STRING){
			if (a){
				if (a->t==SLL_OBJECT_TYPE_CHAR&&o->t==SLL_OBJECT_TYPE_CHAR){
					o->dt.s=_create_print_string(o_dt,&(a->dt.c),&(o->dt.c),1,1,SLL_STRING_COMBINE_CHECKSUMS(a->dt.c,1,o->dt.c));
				}
				else if (a->t==SLL_OBJECT_TYPE_CHAR&&o->t==SLL_OBJECT_TYPE_STRING){
					sll_string_t* sb=o_dt->c_dt->st.dt+o->dt.s;
					o->dt.s=_create_print_string(o_dt,&(a->dt.c),sb->v,1,sb->l,SLL_STRING_COMBINE_CHECKSUMS(a->dt.c,1,sb->c));
				}
				else if (a->t==SLL_OBJECT_TYPE_STRING&&o->t==SLL_OBJECT_TYPE_CHAR){
					sll_string_t* sa=o_dt->c_dt->st.dt+a->dt.s;
					o->dt.s=_create_print_string(o_dt,sa->v,&(o->dt.c),sa->l,1,SLL_STRING_COMBINE_CHECKSUMS(sa->c,sa->l,o->dt.c));
				}
				else{
					SLL_ASSERT(a->t==SLL_OBJECT_TYPE_STRING&&o->t==SLL_OBJECT_TYPE_STRING);
					sll_string_t* sa=o_dt->c_dt->st.dt+a->dt.s;
					sll_string_t* sb=o_dt->c_dt->st.dt+o->dt.s;
					o->dt.s=_create_print_string(o_dt,sa->v,sb->v,sa->l,sb->l,SLL_STRING_COMBINE_CHECKSUMS(sa->c,sa->l,sb->c));
				}
				a->t=SLL_OBJECT_TYPE_NOP;
				o->t=SLL_OBJECT_TYPE_STRING;
				r->dt.ac--;
			}
			a=o;
			o++;
		}
		else{
			a=NULL;
			o=sll_skip_object(o);
		}
	}
	return o;
}



static sll_object_t* _remap_indexes_merge_print(sll_object_t* o,sll_object_t* p,optimizer_data_t* o_dt,sll_function_index_t* fn_m){
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
					o=_remap_indexes_merge_print(o,r,o_dt,fn_m);
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
					o=_remap_indexes_merge_print(o,r,o_dt,fn_m);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_IDENTIFIER:
			SLL_ASSERT(!GET_VARIABLE_REMOVE(o,o_dt));
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
		case SLL_OBJECT_TYPE_PRINT:
			{
				sll_arg_count_t l=o->dt.ac;
				sll_object_t* r=o;
				o++;
				while (l){
					l--;
					o=_remap_indexes_merge_print(o,r,o_dt,fn_m);
				}
				return _merge_print_strings(r,o_dt);
			}
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
				uint8_t rm=GET_VARIABLE_REMOVE(o,o_dt);
				if (rm){
					r->t=SLL_OBJECT_TYPE_OPERATION_LIST;
					r->dt.ac=l-1;
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
					o=_remap_indexes_merge_print(o,r,o_dt,fn_m);
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
					o=_remap_indexes_merge_print(o,r,o_dt,fn_m);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_COMMA:
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_arg_count_t l=o->dt.ac;
				sll_object_t* r=o;
				o++;
				while (l){
					l--;
					o=_remap_indexes_merge_print(o,r,o_dt,fn_m);
				}
				o=r+1;
				sll_object_t* a=NULL;
				sll_arg_count_t i=r->dt.ac;
				do{
					i--;
					while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
						o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
					}
					sll_object_t* n=sll_skip_object(o);
					if (o->t==SLL_OBJECT_TYPE_PRINT){
						if (a){
							a->dt.ac+=o->dt.ac;
							o->t=SLL_OBJECT_TYPE_NOP;
							r->dt.ac--;
						}
						else{
							a=o;
						}
					}
					else{
						if (a){
							o=_merge_print_strings(a,o_dt);
						}
						a=NULL;
					}
					o=n;
				} while (i);
				if (a){
					o=_merge_print_strings(a,o_dt);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->dt.ac;
	sll_object_t* r=o;
	o++;
	while (l){
		l--;
		o=_remap_indexes_merge_print(o,r,o_dt,fn_m);
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
		.l_sm_l=(c_dt->idt.ill+63)>>6,
		.a_v=NULL,
		.rm=0
	};
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		o_dt.it.s_im[i]=malloc(c_dt->idt.s[i].l*sizeof(identifier_data_t));
		o_dt.s_sm_l[i]=(c_dt->idt.s[i].l+63)>>6;
		o_dt.va.s_sm[i]=calloc(sizeof(uint64_t),o_dt.s_sm_l[i]);
		for (sll_identifier_list_length_t j=0;j<c_dt->idt.s[i].l;j++){
			(o_dt.it.s_im[i]+j)->v=SLL_MAX_VARIABLE_INDEX;
			(o_dt.it.s_im[i]+j)->rm=1;
		}
	}
	o_dt.va.l_sm=calloc(sizeof(uint64_t),o_dt.l_sm_l);
	for (sll_identifier_list_length_t i=0;i<c_dt->idt.ill;i++){
		(o_dt.it.l_im+i)->v=SLL_MAX_VARIABLE_INDEX;
		(o_dt.it.l_im+i)->rm=1;
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
		for (sll_arg_count_t k=0;k<fn->al;k++){
			if (SLL_IDENTIFIER_GET_ARRAY_ID(fn->a[k])==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
				(o_dt.it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX(fn->a[k]))->rm=0;
			}
			else{
				(o_dt.it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID(fn->a[k])]+SLL_IDENTIFIER_GET_ARRAY_INDEX(fn->a[k]))->rm=0;
			}
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
			(o_dt.it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX(j))->rm=0;
		}
		else{
			(o_dt.it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID(j)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(j))->rm=0;
		}
	}
	o_dt.v=malloc(o_dt.it.vc*sizeof(sll_runtime_object_t*));
	for (sll_variable_index_t i=0;i<o_dt.it.vc;i++){
		sll_runtime_object_t* v=SLL_CREATE();
		v->t=RUNTIME_OBJECT_TYPE_UNKNOWN;
		*(o_dt.v+i)=v;
	}
	_optimize(c_dt->h,NULL,&o_dt,0);
	free(o_dt.va.l_sm);
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		free(o_dt.va.s_sm[i]);
		o_dt.im.s[i]=malloc(c_dt->idt.s[i].l*sizeof(sll_identifier_index_t));
		sll_identifier_list_length_t k=0;
		for (sll_identifier_list_length_t j=0;j<c_dt->idt.s[i].l;j++){
			*(o_dt.im.s[i]+j)=SLL_CREATE_IDENTIFIER(j-k,i);
			*(c_dt->idt.s[i].dt+j-k)=*(c_dt->idt.s[i].dt+j);
			if ((o_dt.it.s_im[i]+j)->rm){
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
		if ((o_dt.it.l_im+i)->rm){
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
				SLL_ASSERT(!(o_dt.it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX(f->a[k]))->rm);
				f->a[k]=*(o_dt.im.l+SLL_IDENTIFIER_GET_ARRAY_INDEX(f->a[k]));
			}
			else{
				SLL_ASSERT(!(o_dt.it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID(f->a[k])]+SLL_IDENTIFIER_GET_ARRAY_INDEX(f->a[k]))->rm);
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
	_remap_indexes_merge_print(c_dt->h,NULL,&o_dt,fn_m);
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
