#include <sll/_sll_internal.h>
#include <sll/api/string.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/ift.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <stdint.h>



#define DECREASE_PARENT(p) \
	do{ \
		if (p){ \
			if ((p)->t==SLL_NODE_TYPE_FUNC||(p)->t==SLL_NODE_TYPE_INTERNAL_FUNC){ \
				(p)->dt.fn.ac--; \
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
			if ((p)->t==SLL_NODE_TYPE_FUNC||(p)->t==SLL_NODE_TYPE_INTERNAL_FUNC){ \
				(p)->dt.fn.ac+=(v); \
			} \
			else{ \
				(p)->dt.ac+=(v); \
			} \
		} \
	} while (0)



static void _save_var_data(optimizer_data_t* o_dt,variable_assignment_data_t* o){
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		o->s_sm[i]=sll_allocate(o_dt->s_sm_l[i]*sizeof(uint64_t));
		for (uint32_t j=0;j<o_dt->s_sm_l[i];j++){
			*(o->s_sm[i]+j)=*(o_dt->va.s_sm[i]+j);
		}
	}
	o->l_sm=sll_allocate(o_dt->l_sm_l*sizeof(uint64_t));
	for (uint32_t i=0;i<o_dt->l_sm_l;i++){
		*(o->l_sm+i)=*(o_dt->va.l_sm+i);
	}
}



static void _restore_var_data(optimizer_data_t* o_dt,variable_assignment_data_t* va_dt,sll_bool_t s){
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		for (uint32_t j=0;j<o_dt->s_sm_l[i];j++){
			uint64_t v=(*(o_dt->va.s_sm[i]+j))^(*(va_dt->s_sm[i]+j));
			*(o_dt->va.s_sm[i]+j)=*(va_dt->s_sm[i]+j);
			if (!s){
				continue;
			}
			while (v){
				sll_variable_index_t k=(o_dt->it.s_im[i]+((j<<6)|FIND_FIRST_SET_BIT(v)))->v;
				sll_object_t* n=SLL_CREATE();
				n->t=OBJECT_TYPE_UNKNOWN;
				SLL_RELEASE(*(o_dt->v+k));
				*(o_dt->v+k)=n;
				v&=v-1;
			}
		}
		sll_deallocate(va_dt->s_sm[i]);
	}
	for (uint32_t i=0;i<o_dt->l_sm_l;i++){
		uint64_t v=(*(o_dt->va.l_sm+i))^(*(va_dt->l_sm+i));
		*(o_dt->va.l_sm+i)=*(va_dt->l_sm+i);
		if (!s){
			continue;
		}
		while (v){
			sll_variable_index_t j=(o_dt->it.l_im+((i<<6)|FIND_FIRST_SET_BIT(v)))->v;
			sll_object_t* n=SLL_CREATE();
			n->t=OBJECT_TYPE_UNKNOWN;
			SLL_RELEASE(*(o_dt->v+j));
			*(o_dt->v+j)=n;
			v&=v-1;
		}
	}
	sll_deallocate(va_dt->l_sm);
}



static sll_node_t* _remove_single_object(sll_node_t* o){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
		o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	sll_node_type_t t=o->t;
	o->t=SLL_NODE_TYPE_NOP;
	switch (t){
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
					o=_remove_single_object(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				o++;
				while (l){
					l--;
					o=_remove_single_object(o);
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
					o=_remove_single_object(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
			{
				sll_arg_count_t l=o->dt.l.ac;
				o++;
				while (l){
					l--;
					o=_remove_single_object(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_DECL:
			{
				sll_arg_count_t l=o->dt.d.ac;
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



static sll_node_t* _remove_up_to_end(sll_node_t* o,sll_node_offset_t off){
	while (off){
		off--;
		o=_remove_single_object(o);
	}
	return o;
}



static const sll_node_t* _map_identifiers(const sll_node_t* o,const sll_compilation_data_t* c_dt,identifier_map_data_t* im){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
		o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t l=o->dt.al;
				o++;
				while (l){
					l--;
					o=_map_identifiers(o,c_dt,im);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				o++;
				while (l){
					l--;
					o=_map_identifiers(o,c_dt,im);
				}
				return o;
			}
		case SLL_NODE_TYPE_IDENTIFIER:
			{
				sll_identifier_index_t i=SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id);
				uint8_t j=SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id);
				if (j==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
					if ((im->l_im+i)->v==SLL_MAX_VARIABLE_INDEX){
						(im->l_im+i)->v=im->n_vi;
						im->n_vi++;
						if (im->n_vi>im->vc){
							im->vc=im->n_vi;
						}
					}
				}
				else{
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
		case SLL_NODE_TYPE_FUNC:
			return sll_skip_node_const(o);
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					o=_map_identifiers(o,c_dt,im);
				}
				return o;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
			{
				sll_arg_count_t l=o->dt.l.ac;
				o++;
				while (l){
					l--;
					o=_map_identifiers(o,c_dt,im);
				}
				return o;
			}
		case SLL_NODE_TYPE_DECL:
			{
				sll_arg_count_t l=o->dt.d.ac;
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



static sll_object_t* _get_as_object(const sll_node_t* o,const optimizer_data_t* o_dt,uint8_t fl){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
		o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	NOT_FIELD(o);
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
			if (!(fl&OPTIMIZER_NEW_OBJECT)){
				return SLL_FROM_CHAR(o->dt.c);
			}
			else{
				sll_object_t* v=SLL_CREATE();
				v->t=SLL_OBJECT_TYPE_CHAR;
				v->dt.c=o->dt.c;
				return v;
			}
		case SLL_NODE_TYPE_INT:
			if (!(fl&OPTIMIZER_NEW_OBJECT)){
				return SLL_FROM_INT(o->dt.i);
			}
			else{
				sll_object_t* v=SLL_CREATE();
				v->t=SLL_OBJECT_TYPE_INT;
				v->dt.i=o->dt.i;
				return v;
			}
		case SLL_NODE_TYPE_FLOAT:
			if (!(fl&OPTIMIZER_NEW_OBJECT)){
				return SLL_FROM_FLOAT(o->dt.f);
			}
			else{
				sll_object_t* v=SLL_CREATE();
				v->t=SLL_OBJECT_TYPE_FLOAT;
				v->dt.f=o->dt.f;
				return v;
			}
		case SLL_NODE_TYPE_STRING:
			{
				sll_object_t* v=SLL_CREATE();
				v->t=SLL_OBJECT_TYPE_STRING|OBJECT_EXTERNAL_STRING;
				SLL_ASSERT(o->dt.s<o_dt->c_dt->st.l);
				v->dt.s=*(o_dt->c_dt->st.dt+o->dt.s);
				SLL_ASSERT(v->dt.s.v);
				return v;
			}
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t l=o->dt.al;
				sll_object_t* v=SLL_CREATE();
				v->t=SLL_OBJECT_TYPE_ARRAY;
				if (!sll_array_create(l,&(v->dt.a))){
					SLL_UNIMPLEMENTED();
				}
				o++;
				for (sll_array_length_t i=0;i<l;i++){
					sll_object_t* n=_get_as_object(o,o_dt,fl);
					o=sll_skip_node_const(o);
					v->dt.a.v[i]=n;
					if (n->t==OBJECT_TYPE_UNKNOWN){
						while (1){
							SLL_RELEASE(v->dt.a.v[i]);
							if (!i){
								break;
							}
							i--;
						}
						sll_deallocate(v->dt.a.v);
						v->t=OBJECT_TYPE_UNKNOWN;
						break;
					}
				}
				return v;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				sll_object_t* v=SLL_CREATE();
				v->t=SLL_OBJECT_TYPE_MAP;
				sll_map_create((l+1)>>1,&(v->dt.m));
				o++;
				for (sll_map_length_t i=0;i<l;i++){
					sll_object_t* n=_get_as_object(o,o_dt,fl);
					o=sll_skip_node_const(o);
					v->dt.m.v[i]=n;
					if (n->t==OBJECT_TYPE_UNKNOWN){
						while (1){
							SLL_RELEASE(v->dt.m.v[i]);
							if (!i){
								break;
							}
							i--;
						}
						sll_deallocate(v->dt.m.v);
						v->t=OBJECT_TYPE_UNKNOWN;
						break;
					}
				}
				if (l&1){
					v->dt.m.v[l]=SLL_ACQUIRE_STATIC_INT(0);
				}
				return v;
			}
		case SLL_NODE_TYPE_IDENTIFIER:
			if (fl&OPTIMIZER_NO_VARIABLES){
				break;
			}
			SLL_ACQUIRE(*(o_dt->v+GET_VARIABLE_INDEX(o,o_dt)));
			return *(o_dt->v+GET_VARIABLE_INDEX(o,o_dt));
		case SLL_NODE_TYPE_FUNCTION_ID:
			{
				sll_object_t* v=SLL_CREATE();
				v->t=OBJECT_TYPE_FUNCTION_ID;
				v->dt.i=o->dt.fn_id;
				return v;
			}
		case SLL_NODE_TYPE_PRINT:
		case SLL_NODE_TYPE_IF:
		case SLL_NODE_TYPE_SWITCH:
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_RETURN:
		case SLL_NODE_TYPE_OPERATION_LIST:
_return_zero:
			{
				if (!(fl&OPTIMIZER_NEW_OBJECT)){
					return SLL_ACQUIRE_STATIC_INT(0);
				}
				else{
					sll_object_t* v=SLL_CREATE();
					v->t=SLL_OBJECT_TYPE_INT;
					v->dt.i=0;
					return v;
				}
			}
		case SLL_NODE_TYPE_FUNC:
			{
				sll_object_t* v=SLL_CREATE();
				v->t=OBJECT_TYPE_FUNCTION_ID;
				v->dt.i=o->dt.fn.id;
				return v;
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			if (!(fl&OPTIMIZER_NEW_OBJECT)){
				return SLL_FROM_INT(~((sll_integer_t)(o->dt.fn.id)));
			}
			else{
				sll_object_t* v=SLL_CREATE();
				v->t=SLL_OBJECT_TYPE_INT;
				v->dt.i=~((sll_integer_t)(o->dt.fn.id));
				return v;
			}
		case SLL_NODE_TYPE_COMMA:
			{
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					goto _return_zero;
				}
				o++;
				while (l>1){
					l--;
					o=sll_skip_node_const(o);
				}
				return _get_as_object(o,o_dt,fl);
			}
	}
	sll_object_t* v=SLL_CREATE();
	v->t=OBJECT_TYPE_UNKNOWN;
	return v;
}



static sll_node_t* _object_to_node(sll_object_t* v,sll_node_t* o,optimizer_data_t* o_dt){
	if (o->t==NODE_TYPE_CHANGE_STACK){
		o=o->dt._p;
	}
	switch (SLL_OBJECT_GET_TYPE(v)){
		case SLL_OBJECT_TYPE_INT:
			o->t=SLL_NODE_TYPE_INT;
			o->dt.i=v->dt.i;
			return o;
		case SLL_OBJECT_TYPE_FLOAT:
			o->t=SLL_NODE_TYPE_FLOAT;
			o->dt.f=v->dt.f;
			return o;
		case SLL_OBJECT_TYPE_CHAR:
			o->t=SLL_NODE_TYPE_CHAR;
			o->dt.c=v->dt.c;
			return o;
		case SLL_OBJECT_TYPE_STRING:
			o->t=SLL_NODE_TYPE_STRING;
			o->dt.s=0;
			while (o_dt->c_dt->st.dt+o->dt.s!=&(v->dt.s)){
				o->dt.s++;
				if (o->dt.s==o_dt->c_dt->st.l){
					o->dt.s=sll_add_string_object(&(o_dt->c_dt->st),v);
					return o;
				}
			}
			SLL_ASSERT(o->dt.s<o_dt->c_dt->st.l);
			return o;
		case SLL_OBJECT_TYPE_ARRAY:
			o->t=SLL_NODE_TYPE_ARRAY;
			o->dt.al=v->dt.a.l;
			_shift_nodes(o,o_dt->c_dt,v->dt.a.l);
			for (sll_array_length_t i=0;i<v->dt.a.l;i++){
				o=_object_to_node(v->dt.a.v[i],o+1,o_dt);
			}
			return o;
		case SLL_OBJECT_TYPE_MAP:
			{
				o->t=SLL_NODE_TYPE_MAP;
				sll_map_length_t l=v->dt.m.l<<1;
				o->dt.ml=l;
				_shift_nodes(o,o_dt->c_dt,l);
				for (sll_map_length_t i=0;i<l;i++){
					o=_object_to_node(v->dt.m.v[i],o+1,o_dt);
				}
				return o;
			}
		case OBJECT_TYPE_FUNCTION_ID:
			o->t=SLL_NODE_TYPE_FUNCTION_ID;
			o->dt.fn_id=(sll_function_index_t)(v->dt.i);
			return o;
		case OBJECT_TYPE_UNKNOWN:
			SLL_UNIMPLEMENTED();
		default:
			SLL_UNIMPLEMENTED();
	}
}



static const sll_node_t* _mark_loop_vars(const sll_node_t* o,optimizer_data_t* o_dt){
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
					o=_mark_loop_vars(o,o_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				o++;
				while (l){
					l--;
					o=_mark_loop_vars(o,o_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_ASSIGN:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l>=2);
				o++;
				while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
					o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				SLL_ASSERT(o->t==SLL_NODE_TYPE_IDENTIFIER);
				sll_object_t* tmp=_get_as_object(o+1,o_dt,OPTIMIZER_NO_VARIABLES);
				if (tmp->t==OBJECT_TYPE_UNKNOWN){
					(*(o_dt->v+GET_VARIABLE_INDEX(o,o_dt)))->t|=OBJECT_CHANGE_IN_LOOP;
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
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					o=_mark_loop_vars(o,o_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
			{
				sll_arg_count_t l=o->dt.l.ac;
				o++;
				while (l){
					l--;
					o=_mark_loop_vars(o,o_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_INC:
		case SLL_NODE_TYPE_DEC:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				o++;
				while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
					o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				if (o->t==SLL_NODE_TYPE_IDENTIFIER){
					(*(o_dt->v+GET_VARIABLE_INDEX(o,o_dt)))->t|=OBJECT_CHANGE_IN_LOOP;
				}
				o++;
				l--;
				while (l){
					l--;
					o=_mark_loop_vars(o,o_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_DECL:
			{
				sll_arg_count_t l=o->dt.d.ac;
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



static sll_compare_result_t _get_cond_type(const sll_node_t* o,optimizer_data_t* o_dt,uint8_t inv,uint8_t lv){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
		o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	NOT_FIELD(o);
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
			return (((!!o->dt.c)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
		case SLL_NODE_TYPE_INT:
			return (((!!o->dt.i)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
		case SLL_NODE_TYPE_FLOAT:
			return (((!!o->dt.f)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
		case SLL_NODE_TYPE_STRING:
			return (((!!(o_dt->c_dt->st.dt+o->dt.s)->l)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
		case SLL_NODE_TYPE_ARRAY:
			return (((!!o->dt.al)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
		case SLL_NODE_TYPE_MAP:
			return (((!!o->dt.ml)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
		case SLL_NODE_TYPE_IDENTIFIER:
			{
				sll_object_t* v=*(o_dt->v+GET_VARIABLE_INDEX(o,o_dt));
				if (lv||!(v->t&OBJECT_CHANGE_IN_LOOP)){
					switch (SLL_OBJECT_GET_TYPE(v)){
						case SLL_OBJECT_TYPE_INT:
							return (((!!v->dt.i)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
						case SLL_OBJECT_TYPE_FLOAT:
							return (((!!v->dt.f)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
						case SLL_OBJECT_TYPE_CHAR:
							return (((!!v->dt.c)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
						case SLL_OBJECT_TYPE_STRING:
							return (((!!v->dt.s.l)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
						case SLL_OBJECT_TYPE_ARRAY:
							return (((!!v->dt.a.l)^inv)?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
					}
				}
				return COND_TYPE_UNKNOWN;
			}
		case SLL_NODE_TYPE_FUNCTION_ID:
		case SLL_NODE_TYPE_REF:
			return (((!o->dt.ac)^inv)?COND_TYPE_ALWAYS_FALSE:COND_TYPE_ALWAYS_TRUE);
		case SLL_NODE_TYPE_PRINT:
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC_LOAD:
		case SLL_NODE_TYPE_INLINE_FUNC:
		case SLL_NODE_TYPE_IF:
		case SLL_NODE_TYPE_SWITCH:
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_CONTINUE:
		case SLL_NODE_TYPE_BREAK:
		case SLL_NODE_TYPE_RETURN:
		case SLL_NODE_TYPE_OPERATION_LIST:
			return (inv?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
		case SLL_NODE_TYPE_ASSIGN:
			SLL_ASSERT(o->dt.ac>=2);
			return _get_cond_type(sll_skip_node_const(o+1),o_dt,inv,lv);
		case SLL_NODE_TYPE_INLINE_IF:
			SLL_UNIMPLEMENTED();
	}
	return COND_TYPE_UNKNOWN;
}



static sll_node_t* _inline_function(sll_node_t* o,optimizer_data_t* o_dt){
	SLL_ASSERT(o->t==SLL_NODE_TYPE_CALL);
	return sll_skip_node(o);
}



static sll_node_t* _check_remove(sll_node_t* o,sll_node_t* p,optimizer_data_t* o_dt){
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
			DECREASE_PARENT(p);
			o->t=SLL_NODE_TYPE_NOP;
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				o->t=SLL_NODE_TYPE_NOP;
				sll_array_length_t l=o->dt.al;
				INCREASE_PARENT(p,l-1);
				o++;
				while (l){
					l--;
					o=_check_remove(o,p,o_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				o->t=SLL_NODE_TYPE_NOP;
				sll_map_length_t l=o->dt.ml;
				INCREASE_PARENT(p,l-1);
				o++;
				while (l){
					l--;
					o=_check_remove(o,p,o_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				o->t=SLL_NODE_TYPE_NOP;
				sll_array_length_t l=o->dt.fn.ac;
				INCREASE_PARENT(p,l-1);
				o++;
				while (l){
					l--;
					o=_check_remove(o,p,o_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC_LOAD:
		case SLL_NODE_TYPE_NOT:
		case SLL_NODE_TYPE_BOOL:
		case SLL_NODE_TYPE_ADD:
		case SLL_NODE_TYPE_SUB:
		case SLL_NODE_TYPE_MULT:
		case SLL_NODE_TYPE_DIV:
		case SLL_NODE_TYPE_FLOOR_DIV:
		case SLL_NODE_TYPE_MOD:
		case SLL_NODE_TYPE_BIT_AND:
		case SLL_NODE_TYPE_BIT_OR:
		case SLL_NODE_TYPE_BIT_XOR:
		case SLL_NODE_TYPE_BIT_NOT:
		case SLL_NODE_TYPE_LESS:
		case SLL_NODE_TYPE_LESS_EQUAL:
		case SLL_NODE_TYPE_EQUAL:
		case SLL_NODE_TYPE_NOT_EQUAL:
		case SLL_NODE_TYPE_MORE:
		case SLL_NODE_TYPE_MORE_EQUAL:
		case SLL_NODE_TYPE_STRICT_EQUAL:
		case SLL_NODE_TYPE_STRICT_NOT_EQUAL:
		case SLL_NODE_TYPE_CAST:
		case SLL_NODE_TYPE_TYPEOF:
		case SLL_NODE_TYPE_NEW:
			{
				o->t=SLL_NODE_TYPE_NOP;
				sll_arg_count_t l=o->dt.ac;
				INCREASE_PARENT(p,l-1);
				o++;
				while (l){
					l--;
					o=_check_remove(o,p,o_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_INLINE_FUNC:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_CALL:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				sll_node_t* r=o;
				o++;
				while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
					o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				sll_object_t* rt=_get_as_object(o,o_dt,0);
				if (rt->t==SLL_OBJECT_TYPE_INT&&rt->dt.i<0){
					sll_function_index_t i=(sll_function_index_t)(~(rt->dt.i));
					if (i<o_dt->i_ft->l&&!((*(o_dt->i_ft->dt+i))->t&SLL_INTERNAL_FUNCTION_FLAG_REQUIRED)){
						r->t=SLL_NODE_TYPE_OPERATION_LIST;
						r->dt.ac=l;
						SLL_RELEASE(rt);
						return _check_remove(r,p,o_dt);
					}
				}
				SLL_RELEASE(rt);
				for (sll_arg_count_t i=1;i<l;i++){
					o=sll_skip_node(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_DECL:
			{
				o->t=SLL_NODE_TYPE_NOP;
				sll_arg_count_t l=o->dt.d.ac;
				INCREASE_PARENT(p,l-1);
				o++;
				while (l){
					l--;
					o=_check_remove(o,p,o_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_COMMA:
		case SLL_NODE_TYPE_OPERATION_LIST:
			{
				o->t=SLL_NODE_TYPE_NOP;
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
	return sll_skip_node(o);
}



static sll_node_t* _optimize(sll_node_t* o,sll_node_t* p,optimizer_data_t* o_dt,uint8_t fl){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
		o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	SLL_ASSERT(!o_dt->rm);
	if (o_dt->a_v&&o_dt->a_v!=OPTIMIZER_ASSIGN_VARIABLE_REMOVE){
		if (o_dt->a_v->t==SLL_NODE_TYPE_IDENTIFIER){
			sll_object_t** v;
			if (SLL_IDENTIFIER_GET_ARRAY_ID(o_dt->a_v->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
				v=o_dt->v+(o_dt->it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX(o_dt->a_v->dt.id))->v;
				(*(o_dt->va.l_sm+(SLL_IDENTIFIER_GET_ARRAY_INDEX(o_dt->a_v->dt.id)>>6)))|=1ull<<(SLL_IDENTIFIER_GET_ARRAY_INDEX(o_dt->a_v->dt.id)&63);
			}
			else{
				v=o_dt->v+(o_dt->it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID(o_dt->a_v->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(o_dt->a_v->dt.id))->v;
				(*(o_dt->va.s_sm[SLL_IDENTIFIER_GET_ARRAY_ID(o_dt->a_v->dt.id)]+(SLL_IDENTIFIER_GET_ARRAY_INDEX(o_dt->a_v->dt.id)>>6)))|=1ull<<(SLL_IDENTIFIER_GET_ARRAY_INDEX(o_dt->a_v->dt.id)&63);
			}
			SLL_RELEASE(*v);
			*v=_get_as_object(o,o_dt,OPTIMIZER_NEW_OBJECT);
			o_dt->a_v=NULL;
		}
		else if (o_dt->a_v->t==SLL_NODE_TYPE_ACCESS){
			SLL_ASSERT(o_dt->a_v->dt.ac>1);
			sll_object_t* v[3]={NULL,NULL,NULL};
			sll_arg_count_t vl=(o_dt->a_v->dt.ac>4?4:o_dt->a_v->dt.ac);
			sll_node_t* tmp=o_dt->a_v+1;
			while (tmp->t==SLL_NODE_TYPE_NOP||tmp->t==SLL_NODE_TYPE_DBG||tmp->t==NODE_TYPE_CHANGE_STACK){
				tmp=(tmp->t==NODE_TYPE_CHANGE_STACK?tmp->dt._p:tmp+1);
			}
			sll_object_t* var=NULL;
			if (tmp->t==SLL_NODE_TYPE_IDENTIFIER){
				if (SLL_IDENTIFIER_GET_ARRAY_ID(tmp->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
					var=*(o_dt->v+(o_dt->it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX(tmp->dt.id))->v);
					(*(o_dt->va.l_sm+(SLL_IDENTIFIER_GET_ARRAY_INDEX(tmp->dt.id)>>6)))|=1ull<<(SLL_IDENTIFIER_GET_ARRAY_INDEX(tmp->dt.id)&63);
				}
				else{
					var=*(o_dt->v+(o_dt->it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID(tmp->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(tmp->dt.id))->v);
					(*(o_dt->va.s_sm[SLL_IDENTIFIER_GET_ARRAY_ID(tmp->dt.id)]+(SLL_IDENTIFIER_GET_ARRAY_INDEX(tmp->dt.id)>>6)))|=1ull<<(SLL_IDENTIFIER_GET_ARRAY_INDEX(tmp->dt.id)&63);
				}
			}
			else{
				SLL_UNIMPLEMENTED();
			}
			if (SLL_OBJECT_GET_TYPE(var)==OBJECT_TYPE_UNKNOWN){
				o_dt->a_v=NULL;
				goto _keep_assignment;
			}
			tmp++;
			for (sll_arg_count_t i=1;i<vl;i++){
				v[i-1]=_get_as_object(tmp,o_dt,0);
				if (SLL_OBJECT_GET_TYPE(v[i-1])==OBJECT_TYPE_UNKNOWN||(v[i-1]->t&OBJECT_CHANGE_IN_LOOP)){
					do{
						i--;
						SLL_RELEASE(v[i]);
					} while (i);
					o_dt->a_v=NULL;
					goto _keep_assignment;
				}
				tmp=sll_skip_node(tmp);
			}
			sll_object_t* val=_get_as_object(o,o_dt,OPTIMIZER_NEW_OBJECT);
			if (SLL_OBJECT_GET_TYPE(val)==OBJECT_TYPE_UNKNOWN||(val->t&OBJECT_CHANGE_IN_LOOP)){
				SLL_RELEASE(val);
				SLL_UNIMPLEMENTED();
			}
			if (vl==2){
				sll_operator_assign(var,v[0],val);
			}
			else if (vl==3){
				sll_operator_assign_range(var,v[0],v[1],val);
			}
			else{
				SLL_ASSERT(vl==4);
				sll_operator_assign_range_step(var,v[0],v[1],v[2],val);
			}
			SLL_RELEASE(val);
			for (sll_arg_count_t i=0;i<vl-1;i++){
				SLL_RELEASE(v[i]);
			}
			o_dt->a_v=OPTIMIZER_ASSIGN_VARIABLE_REMOVE;
_keep_assignment:;
		}
		else{
			SLL_ASSERT(o_dt->a_v->t==SLL_NODE_TYPE_VAR_ACCESS);
			o_dt->a_v=NULL;
		}
	}
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FUNCTION_ID:
		case SLL_NODE_TYPE_FIELD:
			if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
				DECREASE_PARENT(p);
				o->t=SLL_NODE_TYPE_NOP;
			}
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
					o->dt.ac=o->dt.al;
					goto _optimize_operation_list_comma;
				}
				sll_node_t* r=o;
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
						r->t=SLL_NODE_TYPE_NOP;
						DECREASE_PARENT(p);
					}
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
					o->dt.ac=o->dt.ml;
					goto _optimize_operation_list_comma;
				}
				sll_node_t* r=o;
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
						r->t=SLL_NODE_TYPE_NOP;
						DECREASE_PARENT(p);
					}
				}
				return o;
			}
		case SLL_NODE_TYPE_IDENTIFIER:
			if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
				DECREASE_PARENT(p);
				o->t=SLL_NODE_TYPE_NOP;
			}
			else if (!(fl&OPTIMIZER_FLAG_ASSIGN)){
				sll_object_t* v=*(o_dt->v+GET_VARIABLE_INDEX(o,o_dt));
				if (SLL_OBJECT_GET_TYPE(v)!=OBJECT_TYPE_UNKNOWN&&!(v->t&OBJECT_CHANGE_IN_LOOP)){
					o=_object_to_node(v,o,o_dt);
				}
				else{
					DISABLE_REMOVE_VARIABLE(o,o_dt);
				}
			}
			return o+1;
		case SLL_NODE_TYPE_PRINT:
			{
				sll_arg_count_t l=o->dt.ac;
				sll_node_t* r=o;
				o++;
				while (l){
					l--;
					sll_node_t* a=o;
					o=_optimize(o,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
					if (o_dt->rm){
						SLL_UNIMPLEMENTED();
					}
					sll_object_t* v=_get_as_object(a,o_dt,0);
					if (SLL_OBJECT_GET_TYPE(v)!=OBJECT_TYPE_UNKNOWN&&!(v->t&OBJECT_CHANGE_IN_LOOP)){
						while (a->t==SLL_NODE_TYPE_NOP||a->t==SLL_NODE_TYPE_DBG||a->t==NODE_TYPE_CHANGE_STACK){
							a=(a->t==NODE_TYPE_CHANGE_STACK?a->dt._p:a+1);
						}
						if (a->t==SLL_NODE_TYPE_STRING){
							sll_string_t* s=o_dt->c_dt->st.dt+a->dt.s;
							if (!s->l){
								r->dt.ac--;
								a->t=SLL_NODE_TYPE_NOP;
								SLL_RELEASE(v);
								continue;
							}
							else if (s->l==1){
								a->t=SLL_NODE_TYPE_CHAR;
								a->dt.c=s->v[0];
							}
						}
						else if (a->t!=SLL_NODE_TYPE_CHAR){
							_shift_nodes(a,o_dt->c_dt,1);
							a->t=SLL_NODE_TYPE_COMMA;
							a->dt.ac=2;
							if (o->t==NODE_TYPE_CHANGE_STACK){
								o=o->dt._p;
							}
							o++;
							if (o->t==NODE_TYPE_CHANGE_STACK){
								o=o->dt._p;
							}
							_shift_nodes(o,o_dt->c_dt,1);
							sll_string_t s;
							sll_api_string_convert(&v,1,&s);
							o->t=SLL_NODE_TYPE_STRING;
							o->dt.i=sll_add_string(&(o_dt->c_dt->st),&s,1);
							o=_optimize(a,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
						}
					}
					SLL_RELEASE(v);
				}
				return o;
			}
		case SLL_NODE_TYPE_AND:
		case SLL_NODE_TYPE_OR:
			{
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					o->t=SLL_NODE_TYPE_INT;
					o->dt.i=0;
					return o+1;
				}
				if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
					SLL_UNIMPLEMENTED();
				}
				sll_node_t* r=o;
				o++;
				while (l){
					l--;
					sll_node_t* a=o;
					o=_optimize(o,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
					sll_object_t* v=_get_as_object(a,o_dt,0);
					if (v->t!=OBJECT_TYPE_UNKNOWN&&!(v->t&OBJECT_CHANGE_IN_LOOP)){
						SLL_UNIMPLEMENTED();
					}
					SLL_RELEASE(v);
				}
				return o;
			}
		case SLL_NODE_TYPE_NOT:
		case SLL_NODE_TYPE_BOOL:
		case SLL_NODE_TYPE_BIT_NOT:
		case SLL_NODE_TYPE_LENGTH:
		case SLL_NODE_TYPE_TYPEOF:
			{
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					o->t=SLL_NODE_TYPE_INT;
					o->dt.i=0;
					return o+1;
				}
				sll_node_t* r=o;
				o++;
				while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
					o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				sll_node_t* v=o;
				o=_optimize(v,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
				l--;
				while (l){
					o=_optimize(o,NULL,o_dt,fl&OPTIMIZER_FLAG_ASSIGN);
					l--;
				}
				sll_object_t* rt=_get_as_object(v,o_dt,0);
				if (rt->t!=OBJECT_TYPE_UNKNOWN&&!(rt->t&OBJECT_CHANGE_IN_LOOP)){
					_shift_nodes(o,o_dt->c_dt,1);
					if (r->t==SLL_NODE_TYPE_NOT){
						o->t=SLL_NODE_TYPE_INT;
						o->dt.i=!sll_operator_bool(rt);
					}
					else if (r->t==SLL_NODE_TYPE_BOOL){
						o->t=SLL_NODE_TYPE_INT;
						o->dt.i=sll_operator_bool(rt);
					}
					else if (r->t==SLL_NODE_TYPE_BIT_NOT){
						sll_object_t* nv=sll_operator_inv(rt);
						o=_object_to_node(nv,o,o_dt);
						SLL_RELEASE(nv);
					}
					else if (r->t==SLL_NODE_TYPE_LENGTH){
						sll_object_t* nv=sll_operator_len(rt);
						SLL_ASSERT(SLL_OBJECT_GET_TYPE(nv)==SLL_OBJECT_TYPE_INT);
						o->t=SLL_NODE_TYPE_INT;
						o->dt.i=nv->dt.i;
						SLL_RELEASE(nv);
					}
					else{
						o->t=SLL_NODE_TYPE_INT;
						o->dt.i=SLL_OBJECT_GET_TYPE(rt);
					}
					r->t=SLL_NODE_TYPE_COMMA;
					r->dt.ac++;
					o=r;
					SLL_RELEASE(rt);
					goto _optimize_operation_list_comma;
				}
				SLL_RELEASE(rt);
				return o;
			}
		case SLL_NODE_TYPE_ASSIGN:
			{
				sll_node_t* r=o;
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l>=2);
				o++;
				sll_node_t* id_o=o;
				o=_optimize(o,NULL,o_dt,OPTIMIZER_FLAG_ASSIGN|OPTIMIZER_FLAG_ARGUMENT);
				while (id_o->t==SLL_NODE_TYPE_NOP||id_o->t==SLL_NODE_TYPE_DBG||id_o->t==NODE_TYPE_CHANGE_STACK){
					id_o=(id_o->t==NODE_TYPE_CHANGE_STACK?id_o->dt._p:id_o+1);
				}
				if (id_o->t!=SLL_NODE_TYPE_IDENTIFIER&&id_o->t!=SLL_NODE_TYPE_ACCESS&&id_o->t!=SLL_NODE_TYPE_VAR_ACCESS){
					o=r;
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
					goto _optimize_operation_list_comma;
				}
				o_dt->a_v=id_o;
				o=_optimize(o,NULL,o_dt,OPTIMIZER_FLAG_ARGUMENT);
				if (o_dt->a_v==OPTIMIZER_ASSIGN_VARIABLE_REMOVE){
					o_dt->a_v=NULL;
					o=r;
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
					goto _optimize_operation_list_comma;
				}
				o_dt->a_v=NULL;
				if (o_dt->rm){
					SLL_UNIMPLEMENTED();
				}
				l-=2;
				while (l){
					l--;
					o=_optimize(o,r,o_dt,0);
					if (o_dt->rm){
						SLL_UNIMPLEMENTED();
					}
				}
				return o;
			}
		case SLL_NODE_TYPE_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				sll_node_t* r=o;
				o++;
				while (l){
					l--;
					o=_optimize(o,r,o_dt,0);
					if (o_dt->rm){
						r->dt.ac-=l;
						o=_remove_up_to_end(o,l);
						break;
					}
				}
				o_dt->rm=0;
				return o;
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
					o->dt.ac=o->dt.fn.ac;
					goto _optimize_operation_list_comma;
				}
				sll_node_t* r=o;
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					o=_optimize(o,r,o_dt,0);
					if (o_dt->rm){
						_remove_up_to_end(o,l);
						r->dt.fn.ac-=l;
						break;
					}
				}
				if (!r->dt.ac){
					r->t=SLL_NODE_TYPE_INT;
					r->dt.i=~((sll_integer_t)r->dt.fn.id);
				}
				return o;
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC_LOAD:
			{
				if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
					goto _optimize_operation_list_comma;
				}
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					o->t=SLL_NODE_TYPE_INT;
					o->dt.i=0;
					return o+1;
				}
				sll_node_t* r=o;
				o=_optimize(o+1,r,o_dt,OPTIMIZER_FLAG_ARGUMENT);
				l--;
				while (l){
					l--;
					o=_optimize(o,r,o_dt,0);
					if (o_dt->rm){
						_remove_up_to_end(o,l);
						r->dt.ac-=l;
						break;
					}
				}
				return o;
			}
		case SLL_NODE_TYPE_INLINE_FUNC:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_CALL:
			{
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					o->t=SLL_NODE_TYPE_NOP;
					SLL_UNIMPLEMENTED();
					return o+1;
				}
				sll_node_t* r=o;
				o++;
				while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
					o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				sll_node_t* fn=o;
				o=_optimize(fn,o,o_dt,OPTIMIZER_FLAG_ARGUMENT);
				if (o_dt->rm){
					_remove_up_to_end(o,l);
				}
				l--;
				while (l){
					l--;
					o=_optimize(o,r,o_dt,OPTIMIZER_FLAG_ARGUMENT);
					if (o_dt->rm){
						_remove_up_to_end(o,l);
						r->dt.ac-=l;
						break;
					}
				}
				if (fn->t==SLL_NODE_TYPE_FUNC){
					o=_inline_function(r,o_dt);
				}
				else if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
					sll_object_t* rt=_get_as_object(fn,o_dt,0);
					if (rt->t==SLL_OBJECT_TYPE_INT||rt->t==OBJECT_TYPE_FUNCTION_ID){
						if (rt->dt.i<0){
							sll_function_index_t i=(sll_function_index_t)(~(rt->dt.i));
							if (i<o_dt->i_ft->l&&!((*(o_dt->i_ft->dt+i))->t&SLL_INTERNAL_FUNCTION_FLAG_REQUIRED)){
								r->t=SLL_NODE_TYPE_OPERATION_LIST;
								_optimize(r,p,o_dt,fl);
							}
						}
						else if (rt->dt.i>=o_dt->c_dt->ft.l){
							r->t=SLL_NODE_TYPE_OPERATION_LIST;
							o=r;
							goto _optimize_operation_list_comma;
						}
					}
					SLL_RELEASE(rt);
				}
				else if (fn->t==SLL_NODE_TYPE_INT||fn->t==SLL_NODE_TYPE_FUNCTION_ID){
					if (fn->dt.i<0){
						sll_node_t* e=o;
						sll_function_index_t i=(sll_function_index_t)(~(fn->dt.i));
						if (i<o_dt->i_ft->l){
							if ((*(o_dt->i_ft->dt+i))->t&SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL){
								sll_object_t** al=sll_allocate((r->dt.ac-1)*sizeof(sll_object_t*));
								o=fn;
								for (sll_arg_count_t j=1;j<r->dt.ac;j++){
									o=sll_skip_node(o);
									sll_object_t* n=_get_as_object(o,o_dt,0);
									*(al+j-1)=n;
									if (n->t==OBJECT_TYPE_UNKNOWN){
										do{
											j--;
											SLL_RELEASE(*(al+j));
										} while (j);
										sll_deallocate(al);
										return e;
									}
								}
								if (al){
									sll_object_t* n=(*(o_dt->i_ft->dt+i))->p(al,r->dt.ac-1);
									for (sll_arg_count_t j=0;j<r->dt.ac-1;j++){
										SLL_RELEASE(*(al+j));
									}
									sll_deallocate(al);
									r->t=SLL_NODE_TYPE_COMMA;
									r->dt.ac++;
									o=sll_skip_node(o);
									_shift_nodes(o,o_dt->c_dt,1);
									_object_to_node(n,o,o_dt);
									SLL_RELEASE(n);
									return _optimize(r,p,o_dt,fl);
								}
							}
						}
						else{
							r->t=SLL_NODE_TYPE_OPERATION_LIST;
							o=r;
							goto _optimize_operation_list_comma;
						}
					}
					else if (fn->dt.i>=o_dt->c_dt->ft.l){
						r->t=SLL_NODE_TYPE_OPERATION_LIST;
						o=r;
						goto _optimize_operation_list_comma;
					}
				}
				return o;
			}
		case SLL_NODE_TYPE_IF:
		case SLL_NODE_TYPE_INLINE_IF:
			{
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
						DECREASE_PARENT(p);
						o->t=SLL_NODE_TYPE_NOP;
					}
					else{
						o->t=SLL_NODE_TYPE_INT;
						o->dt.i=0;
					}
					return o+1;
				}
				if (l==1){
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
					goto _optimize_operation_list_comma;
				}
				variable_assignment_data_t va_dt;
				_save_var_data(o_dt,&va_dt);
				sll_node_t* r=o;
				o++;
				if (l&1){
					l>>=1;
					while (l){
						l--;
						sll_node_t* cnd_o=o;
						o=_optimize(cnd_o,NULL,o_dt,OPTIMIZER_FLAG_ARGUMENT);
						if (o_dt->rm){
							SLL_UNIMPLEMENTED();
						}
						uint8_t cnd=_get_cond_type(cnd_o,o_dt,0,0);
						if (cnd==COND_TYPE_ALWAYS_TRUE){
							if (r->dt.ac>3){
								_shift_nodes(cnd_o,o_dt->c_dt,1);
								cnd_o->t=SLL_NODE_TYPE_COMMA;
								cnd_o->dt.ac=2;
								o++;
								if (o->t==NODE_TYPE_CHANGE_STACK){
									o=o->dt._p;
								}
								o=_optimize(o,cnd_o,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
								_remove_up_to_end(o,(l<<1)+1);
								r->dt.ac-=(l<<1)+2;
								if (r->dt.ac==1){
									_restore_var_data(o_dt,&va_dt,0);
									if (r->t==SLL_NODE_TYPE_IF){
										r->t=SLL_NODE_TYPE_OPERATION_LIST;
										r->dt.ac=1;
										o=r;
										goto _optimize_operation_list_comma;
									}
									r->t=SLL_NODE_TYPE_NOP;
								}
								else{
									_restore_var_data(o_dt,&va_dt,1);
								}
								o=cnd_o;
								goto _optimize_operation_list_comma;
							}
							r->t=(r->t==SLL_NODE_TYPE_INLINE_IF?SLL_NODE_TYPE_COMMA:SLL_NODE_TYPE_OPERATION_LIST);
							r->dt.ac--;
							_remove_single_object(_optimize(o,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT));
							_restore_var_data(o_dt,&va_dt,0);
							o=r;
							goto _optimize_operation_list_comma;
						}
						else if (cnd==COND_TYPE_ALWAYS_FALSE){
							if (r->dt.ac==3){
								r->t=(r->t==SLL_NODE_TYPE_INLINE_IF?SLL_NODE_TYPE_COMMA:SLL_NODE_TYPE_OPERATION_LIST);
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
					if (r->t!=SLL_NODE_TYPE_NOP){
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
						sll_node_t* cnd_o=o;
						o=_optimize(cnd_o,NULL,o_dt,OPTIMIZER_FLAG_ARGUMENT);
						if (o_dt->rm){
							SLL_UNIMPLEMENTED();
						}
						uint8_t cnd=_get_cond_type(cnd_o,o_dt,0,0);
						if (cnd==COND_TYPE_ALWAYS_TRUE){
							if (r->dt.ac>2){
								_shift_nodes(cnd_o,o_dt->c_dt,1);
								cnd_o->t=SLL_NODE_TYPE_COMMA;
								cnd_o->dt.ac=2;
								o++;
								if (o->t==NODE_TYPE_CHANGE_STACK){
									o=o->dt._p;
								}
								o=_optimize(o,cnd_o,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
								_remove_up_to_end(o,l<<1);
								r->dt.ac-=(l<<1)+1;
								if (r->dt.ac==1){
									_restore_var_data(o_dt,&va_dt,0);
									if (r->t==SLL_NODE_TYPE_IF){
										r->t=SLL_NODE_TYPE_OPERATION_LIST;
										r->dt.ac=1;
										o=r;
										goto _optimize_operation_list_comma;
									}
									r->t=SLL_NODE_TYPE_NOP;
								}
								else{
									_restore_var_data(o_dt,&va_dt,1);
								}
								o=cnd_o;
								goto _optimize_operation_list_comma;
							}
							r->t=(r->t==SLL_NODE_TYPE_INLINE_IF?SLL_NODE_TYPE_COMMA:SLL_NODE_TYPE_OPERATION_LIST);
							_optimize(o,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
							_restore_var_data(o_dt,&va_dt,0);
							o=r;
							goto _optimize_operation_list_comma;
						}
						else if (cnd==COND_TYPE_ALWAYS_FALSE){
							if (r->dt.ac==2){
								r->t=(r->t==SLL_NODE_TYPE_INLINE_IF?SLL_NODE_TYPE_COMMA:SLL_NODE_TYPE_OPERATION_LIST);
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
				_restore_var_data(o_dt,&va_dt,(r->t!=SLL_NODE_TYPE_NOP));
				return o;
			}
		case SLL_NODE_TYPE_SWITCH:
			{
				sll_arg_count_t l=o->dt.ac;
				if (l<3){
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
					goto _optimize_operation_list_comma;
				}
				sll_node_t* r=o;
				if (l==3){
					SLL_UNIMPLEMENTED();
				}
				if (l==4){
					SLL_UNIMPLEMENTED();
				}
				o=_optimize(o+1,r,o_dt,OPTIMIZER_FLAG_ARGUMENT);
				sll_node_t* cnd_n_o=o;
				sll_object_t* cnd=_get_as_object(r+1,o_dt,0);
				if ((SLL_OBJECT_GET_TYPE(cnd)!=SLL_OBJECT_TYPE_INT&&SLL_OBJECT_GET_TYPE(cnd)!=SLL_OBJECT_TYPE_CHAR)||(cnd->t&OBJECT_CHANGE_IN_LOOP)){
					SLL_RELEASE(cnd);
					cnd=NULL;
				}
				else if (SLL_OBJECT_GET_TYPE(cnd)==SLL_OBJECT_TYPE_CHAR){
					sll_char_t v=cnd->dt.c;
					SLL_RELEASE(cnd);
					cnd=SLL_FROM_INT(v);
				}
				SLL_ASSERT(!cnd||SLL_OBJECT_GET_TYPE(cnd)==SLL_OBJECT_TYPE_INT);
				l=(l-1)>>1;
				sll_integer_t* il=NULL;
				sll_array_length_t ill=0;
				do{
					l--;
					sll_node_t* a=o;
					o=_optimize(o,r,o_dt,OPTIMIZER_FLAG_ARGUMENT);
					while (a->t==SLL_NODE_TYPE_NOP||a->t==SLL_NODE_TYPE_DBG||a->t==NODE_TYPE_CHANGE_STACK){
						a=(a->t==NODE_TYPE_CHANGE_STACK?a->dt._p:a+1);
					}
					if (a->t!=SLL_NODE_TYPE_INT&&a->t!=SLL_NODE_TYPE_CHAR){
						SLL_UNIMPLEMENTED();
					}
					sll_integer_t i=(a->t==SLL_NODE_TYPE_INT?a->dt.i:a->dt.c);
					if (cnd&&i==cnd->dt.i){
						sll_deallocate(il);
						SLL_RELEASE(cnd);
						r->t=SLL_NODE_TYPE_OPERATION_LIST;
						_remove_up_to_end(cnd_n_o,((r->dt.ac-1)&0xfffffffe)-(l<<1)-1);
						o=_optimize(o,r,o_dt,0);
						_remove_up_to_end(o,(l<<1)+(!(r->dt.ac&1)));
						r->dt.ac=2;
						o=r;
						goto _optimize_operation_list_comma;
					}
					else{
						sll_array_length_t j=0;
						for (;j<ill;j++){
							if (*(il+j)==i){
								break;
							}
						}
						if (j==ill){
							o=_optimize(o,r,o_dt,OPTIMIZER_FLAG_ARGUMENT);
							ill++;
							il=sll_reallocate(il,ill*sizeof(sll_integer_t));
							*(il+ill-1)=i;
						}
						else{
							o=_remove_single_object(_remove_single_object(a));
						}
					}
				} while (l);
				sll_deallocate(il);
				if (!(o->dt.ac&1)){
					o=_optimize(o,r,o_dt,OPTIMIZER_FLAG_ARGUMENT);
				}
				if (cnd){
					SLL_RELEASE(cnd);
				}
				return o;
			}
		case SLL_NODE_TYPE_FOR:
			{
				sll_arg_count_t l=o->dt.l.ac;
				SLL_ASSERT(l);
				if (l==1){
					o->t=SLL_NODE_TYPE_NOP;
					return _optimize(o+1,p,o_dt,fl);
				}
				variable_assignment_data_t va_dt;
				_save_var_data(o_dt,&va_dt);
				sll_node_t* r=o;
				o=_optimize(o+1,NULL,o_dt,OPTIMIZER_FLAG_ARGUMENT);
				if (o_dt->rm){
					r->t=SLL_NODE_TYPE_NOP;
					_remove_up_to_end(o,l-1);
					return o;
				}
				sll_node_t* cnd_o=o;
				const sll_node_t* tmp=o;
				for (sll_arg_count_t i=1;i<l;i++){
					tmp=_mark_loop_vars(tmp,o_dt);
				}
				o=_optimize(cnd_o,NULL,o_dt,OPTIMIZER_FLAG_ARGUMENT);
				uint8_t cnd=_get_cond_type(cnd_o,o_dt,1,1);
				if (cnd==COND_TYPE_ALWAYS_TRUE){
					SLL_UNIMPLEMENTED();
				}
				else if (cnd==COND_TYPE_ALWAYS_FALSE){
					r->t=SLL_NODE_TYPE_WHILE;
				}
				l-=2;
				while (l){
					l--;
					o=_optimize(o,r,o_dt,0);
					if (o_dt->rm){
						_remove_up_to_end(o,l);
						r->dt.l.ac-=l;
						break;
					}
				}
				_restore_var_data(o_dt,&va_dt,1);
				return o;
			}
		case SLL_NODE_TYPE_WHILE:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_LOOP:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_INC:
		case SLL_NODE_TYPE_DEC:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				sll_node_t* r=o;
				o++;
				while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
					o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				if (o->t!=SLL_NODE_TYPE_IDENTIFIER){
					r->t=SLL_NODE_TYPE_OPERATION_LIST;
					o=r;
					goto _optimize_operation_list_comma;
				}
				sll_object_t* v=*(o_dt->v+GET_VARIABLE_INDEX(o,o_dt));
				if (SLL_OBJECT_GET_TYPE(v)!=OBJECT_TYPE_UNKNOWN&&!(v->t&OBJECT_CHANGE_IN_LOOP)){
					sll_object_t* nv=(r->t==SLL_NODE_TYPE_INC?sll_operator_inc(v):sll_operator_dec(v));
					SLL_RELEASE(v);
					*(o_dt->v+GET_VARIABLE_INDEX(o,o_dt))=nv;
					r->t=SLL_NODE_TYPE_OPERATION_LIST;
					o->t=SLL_NODE_TYPE_NOP;
					o=r;
					o->dt.ac--;
					goto _optimize_operation_list_comma;
				}
				l--;
				o++;
				while (l){
					l--;
					o=_optimize(o,r,o_dt,0);
				}
				return o;
			}
		case SLL_NODE_TYPE_LESS:
		case SLL_NODE_TYPE_LESS_EQUAL:
		case SLL_NODE_TYPE_EQUAL:
		case SLL_NODE_TYPE_NOT_EQUAL:
		case SLL_NODE_TYPE_MORE:
		case SLL_NODE_TYPE_MORE_EQUAL:
		case SLL_NODE_TYPE_STRICT_EQUAL:
		case SLL_NODE_TYPE_STRICT_NOT_EQUAL:
			if (o->dt.ac==1){
				o->t=SLL_NODE_TYPE_COMMA;
				o->dt.ac=2;
				sll_node_t* a=sll_skip_node(o+1);
				_shift_nodes(a,o_dt->c_dt,1);
				a->t=SLL_NODE_TYPE_INT;
				a->dt.i=1;
				goto _optimize_operation_list_comma;
			}
		case SLL_NODE_TYPE_ADD:
		case SLL_NODE_TYPE_SUB:
		case SLL_NODE_TYPE_MULT:
		case SLL_NODE_TYPE_DIV:
		case SLL_NODE_TYPE_FLOOR_DIV:
		case SLL_NODE_TYPE_MOD:
		case SLL_NODE_TYPE_BIT_AND:
		case SLL_NODE_TYPE_BIT_OR:
		case SLL_NODE_TYPE_BIT_XOR:
		case SLL_NODE_TYPE_BIT_RSHIFT:
		case SLL_NODE_TYPE_BIT_LSHIFT:
			{
				sll_arg_count_t l=o->dt.ac;
				if (l==1){
					o->t=SLL_NODE_TYPE_NOP;
					return _optimize(o+1,p,o_dt,fl);
				}
				sll_node_t* r=o;
				o++;
				sll_object_t* v=NULL;
				sll_node_t** arg=sll_allocate(l*sizeof(sll_node_t*));
				sll_arg_count_t i=0;
				sll_arg_count_t j=0;
				sll_node_t* pa=NULL;
				while (l){
					l--;
					sll_node_t* a=o;
					o=_optimize(o,r,o_dt,OPTIMIZER_FLAG_ARGUMENT);
					if (o_dt->rm){
						SLL_UNIMPLEMENTED();
					}
					sll_object_t* av=_get_as_object(a,o_dt,0);
					if (SLL_OBJECT_GET_TYPE(av)!=OBJECT_TYPE_UNKNOWN&&!(av->t&OBJECT_CHANGE_IN_LOOP)&&(!v||!i||SLL_OBJECT_IS_STORAGE(av)==SLL_OBJECT_IS_STORAGE(v))){
						*(arg+j)=o;
						if (!v){
							SLL_ACQUIRE(av);
							v=av;
							i=j;
						}
						else{
							sll_object_t* nv=NULL;
							switch (r->t){
								case SLL_NODE_TYPE_ADD:
									nv=sll_operator_add(v,av);
									break;
								case SLL_NODE_TYPE_SUB:
									nv=sll_operator_sub(v,av);
									break;
								case SLL_NODE_TYPE_MULT:
									nv=sll_operator_mult(v,av);
									break;
								case SLL_NODE_TYPE_DIV:
									nv=sll_operator_div(v,av);
									break;
								case SLL_NODE_TYPE_FLOOR_DIV:
									nv=sll_operator_floor_div(v,av);
									break;
								case SLL_NODE_TYPE_MOD:
									nv=sll_operator_mod(v,av);
									break;
								case SLL_NODE_TYPE_BIT_AND:
									nv=sll_operator_and(v,av);
									break;
								case SLL_NODE_TYPE_BIT_OR:
									nv=sll_operator_or(v,av);
									break;
								case SLL_NODE_TYPE_BIT_XOR:
									nv=sll_operator_xor(v,av);
									break;
								case SLL_NODE_TYPE_BIT_RSHIFT:
									nv=sll_operator_shr(v,av);
									break;
								case SLL_NODE_TYPE_BIT_LSHIFT:
									nv=sll_operator_shl(v,av);
									break;
								case SLL_NODE_TYPE_LESS:
								case SLL_NODE_TYPE_LESS_EQUAL:
								case SLL_NODE_TYPE_MORE:
								case SLL_NODE_TYPE_MORE_EQUAL:
									{
										sll_compare_result_t cmp=sll_operator_compare(v,av);
										if ((r->t==SLL_NODE_TYPE_LESS&&cmp==SLL_COMPARE_RESULT_BELOW)||(r->t==SLL_NODE_TYPE_LESS_EQUAL&&cmp!=SLL_COMPARE_RESULT_ABOVE)||(r->t==SLL_NODE_TYPE_MORE&&cmp==SLL_COMPARE_RESULT_ABOVE)||(r->t==SLL_NODE_TYPE_MORE_EQUAL&&cmp!=SLL_COMPARE_RESULT_BELOW)){
_join_cond:
											_shift_nodes(pa,o_dt->c_dt,1);
											r->dt.ac--;
											pa->t=SLL_NODE_TYPE_COMMA;
											pa->dt.ac=2;
											a++;
											if (a->t==NODE_TYPE_CHANGE_STACK){
												a=a->dt._p;
											}
											o++;
											if (o->t==NODE_TYPE_CHANGE_STACK){
												o=o->dt._p;
											}
											_optimize(pa,r,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
											SLL_ACQUIRE(av);
											nv=av;
											break;
										}
_remove_cond:
										sll_deallocate(arg);
										SLL_RELEASE(v);
										SLL_RELEASE(av);
										_remove_up_to_end(o,r->dt.ac-j-1);
										_shift_nodes(pa,o_dt->c_dt,1);
										pa->t=SLL_NODE_TYPE_COMMA;
										pa->dt.ac=2;
										r->dt.ac=j;
										_shift_nodes(r,o_dt->c_dt,1);
										r->t=SLL_NODE_TYPE_OPERATION_LIST;
										r->dt.ac=1;
										o=r;
										goto _optimize_operation_list_comma;
									}
								case SLL_NODE_TYPE_EQUAL:
								case SLL_NODE_TYPE_NOT_EQUAL:
									if ((!!(r->t==SLL_NODE_TYPE_NOT_EQUAL))^sll_operator_equal(v,av)){
										goto _join_cond;
									}
									goto _remove_cond;
								case SLL_NODE_TYPE_STRICT_EQUAL:
								case SLL_NODE_TYPE_STRICT_NOT_EQUAL:
									if ((!!(r->t==SLL_NODE_TYPE_NOT_EQUAL))^sll_operator_strict_equal(v,av)){
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
							sll_deallocate(arg);
							_shift_nodes(o,o_dt->c_dt,1);
							if (r->t>=SLL_NODE_TYPE_LESS){
								o->t=SLL_NODE_TYPE_INT;
								o->dt.i=1;
							}
							else{
								_object_to_node(v,o,o_dt);
							}
							SLL_RELEASE(v);
							r->t=SLL_NODE_TYPE_COMMA;
							r->dt.ac++;
							o=r;
							goto _optimize_operation_list_comma;
						}
						else{
							if (r->t>=SLL_NODE_TYPE_LESS){
								SLL_UNIMPLEMENTED();
							}
							r->dt.ac-=j-i-1;
							sll_node_t* tmp=*(arg+i);
							sll_deallocate(arg);
							_shift_nodes(tmp,o_dt->c_dt,1);
							tmp->t=SLL_NODE_TYPE_COMMA;
							tmp->dt.ac=j-i+1;
							o++;
							if (o->t==NODE_TYPE_CHANGE_STACK){
								o=o->dt._p;
							}
							_shift_nodes(o,o_dt->c_dt,1);
							_object_to_node(v,o,o_dt);
							SLL_RELEASE(v);
							o=tmp;
							p=r;
							fl|=OPTIMIZER_FLAG_ARGUMENT;
							goto _optimize_operation_list_comma;
						}
					}
					SLL_RELEASE(v);
				}
				sll_deallocate(arg);
				return o;
			}
		case SLL_NODE_TYPE_ACCESS:
			{
				if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
					goto _optimize_operation_list_comma;
				}
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					o->t=SLL_NODE_TYPE_INT;
					o->dt.i=0;
					return o+1;
				}
				sll_node_t* r=o;
				if (l==1){
					if (fl&OPTIMIZER_FLAG_ASSIGN){
						SLL_UNIMPLEMENTED();
					}
					o=_optimize(o+1,r,o_dt,OPTIMIZER_FLAG_ARGUMENT);
					sll_object_t* v=_get_as_object(r+1,o_dt,0);
					if (SLL_OBJECT_GET_TYPE(v)!=OBJECT_TYPE_UNKNOWN&&!(v->t&OBJECT_CHANGE_IN_LOOP)){
						r->t=SLL_NODE_TYPE_COMMA;
						r->dt.ac=2;
						_shift_nodes(o,o_dt->c_dt,1);
						_object_to_node(v,o,o_dt);
						SLL_RELEASE(v);
						o=r;
						goto _optimize_operation_list_comma;
					}
					SLL_RELEASE(v);
					return o;
				}
				o++;
				sll_object_t* v[4]={NULL,NULL,NULL,NULL};
				sll_arg_count_t i=0;
				if (l>4){
					l=4;
				}
				while (i<l){
					sll_node_t* a=o;
					o=_optimize(o,r,o_dt,(!i?fl:0)|OPTIMIZER_FLAG_ARGUMENT);
					v[i]=_get_as_object(a,o_dt,0);
					if (SLL_OBJECT_GET_TYPE(v[i])==OBJECT_TYPE_UNKNOWN||(v[i]->t&OBJECT_CHANGE_IN_LOOP)){
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
				else if (!(fl&OPTIMIZER_FLAG_ASSIGN)){
					sll_object_t* n;
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
						o=sll_skip_node(o);
						i++;
					}
					r->t=SLL_NODE_TYPE_COMMA;
					r->dt.ac++;
					_shift_nodes(o,o_dt->c_dt,1);
					o=_object_to_node(n,o,o_dt)+1;
					SLL_RELEASE(n);
					o=r;
					goto _optimize_operation_list_comma;
				}
				for (sll_arg_count_t j=0;j<i;j++){
					SLL_RELEASE(v[j]);
				}
				if (fl&OPTIMIZER_FLAG_ASSIGN){
					sll_node_t* tmp=r+1;
					while (tmp->t==SLL_NODE_TYPE_NOP||tmp->t==SLL_NODE_TYPE_DBG||tmp->t==NODE_TYPE_CHANGE_STACK){
						tmp=(tmp->t==NODE_TYPE_CHANGE_STACK?tmp->dt._p:tmp+1);
					}
					if (tmp->t==SLL_NODE_TYPE_IDENTIFIER){
						DISABLE_REMOVE_VARIABLE(tmp,o_dt);
					}
				}
				l=r->dt.ac;
				while (i<l){
					o=_optimize(o,r,o_dt,(i<5?OPTIMIZER_FLAG_ARGUMENT:0));
					if (o_dt->rm){
						SLL_UNIMPLEMENTED();
					}
					i++;
				}
				return o;
			}
		case SLL_NODE_TYPE_VAR_ACCESS:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l>1);
				o++;
				while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
					o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				SLL_ASSERT(o->t==SLL_NODE_TYPE_IDENTIFIER);
				DISABLE_REMOVE_VARIABLE(o,o_dt);
				o++;
				l--;
				do{
					l--;
					o=sll_skip_node(o);
				} while (l);
				return o;
			}
		case SLL_NODE_TYPE_CAST:
			{
				if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
					goto _optimize_operation_list_comma;
				}
				if (fl&OPTIMIZER_FLAG_ASSIGN){
					SLL_UNIMPLEMENTED();
				}
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					o->t=SLL_NODE_TYPE_INT;
					o->dt.i=0;
					return o+1;
				}
				sll_node_t* r=o;
				o=_optimize(o+1,r,o_dt,OPTIMIZER_FLAG_ARGUMENT);
				sll_object_t* v=_get_as_object(r+1,o_dt,0);
				l--;
				if (SLL_OBJECT_GET_TYPE(v)!=OBJECT_TYPE_UNKNOWN&&!(v->t&OBJECT_CHANGE_IN_LOOP)){
					while (l){
						l--;
						sll_node_t* a=o;
						o=_optimize(o,r,o_dt,OPTIMIZER_FLAG_ARGUMENT);
						sll_object_t* n=_get_as_object(a,o_dt,0);
						if (SLL_OBJECT_GET_TYPE(n)==OBJECT_TYPE_UNKNOWN||(n->t&OBJECT_CHANGE_IN_LOOP)){
							SLL_RELEASE(n);
							goto _unknown_cast;
						}
						sll_object_t* tmp=sll_operator_cast(v,n);
						SLL_RELEASE(v);
						SLL_RELEASE(n);
						v=tmp;
					}
					r->t=SLL_NODE_TYPE_COMMA;
					r->dt.ac++;
					_shift_nodes(o,o_dt->c_dt,1);
					_object_to_node(v,o,o_dt);
					SLL_RELEASE(v);
					o=r;
					goto _optimize_operation_list_comma;
				}
_unknown_cast:
				SLL_RELEASE(v);
				while (l){
					l--;
					o=_optimize(o,r,o_dt,OPTIMIZER_FLAG_ARGUMENT);
					if (o_dt->rm){
						SLL_UNIMPLEMENTED();
					}
				}
				return o;
			}
		case SLL_NODE_TYPE_DECL:
			{
				if (fl&OPTIMIZER_FLAG_ASSIGN){
					SLL_UNIMPLEMENTED();
				}
				sll_arg_count_t l=o->dt.ac;
				sll_node_t* r=o;
				o++;
				sll_bool_t e=l&1;
				l>>=1;
				sll_object_type_field_t* a=sll_allocate_stack(l*sizeof(sll_object_type_field_t));
				sll_arg_count_t i=0;
				while (l){
					l--;
					sll_node_t* v=o;
					o=_optimize(o,r,o_dt,OPTIMIZER_FLAG_ARGUMENT);
					if (a){
						sll_object_t* nv=_get_as_object(v,o_dt,0);
						if (SLL_OBJECT_GET_TYPE(nv)==OBJECT_TYPE_UNKNOWN||(nv->t&OBJECT_CHANGE_IN_LOOP)){
							sll_deallocate(a);
							a=NULL;
						}
						else{
							sll_object_t* iv=sll_operator_cast(nv,sll_static_int[SLL_OBJECT_TYPE_INT]);
							if (iv->dt.i<0){
								(a+i)->t=((sll_object_type_t)(~(iv->dt.i)))|SLL_OBJECT_FLAG_CONSTANT;
							}
							else{
								(a+i)->t=(sll_object_type_t)(iv->dt.i);
							}
							SLL_RELEASE(iv);
						}
						SLL_RELEASE(nv);
					}
					while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
						o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
					}
					SLL_ASSERT(o->t==SLL_NODE_TYPE_FIELD);
					if (a){
						(a+i)->f=o->dt.s;
						i++;
					}
					o++;
				}
				if (e){
					o=_optimize(o,r,o_dt,0);
				}
				if (r->dt.ac<2||!a){
					sll_deallocate(a);
					return o;
				}
				SLL_UNIMPLEMENTED();
			}
		case SLL_NODE_TYPE_NEW:
			{
				if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
					goto _optimize_operation_list_comma;
				}
				if (fl&OPTIMIZER_FLAG_ASSIGN){
					SLL_UNIMPLEMENTED();
				}
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					o->t=SLL_NODE_TYPE_INT;
					o->dt.i=0;
					return o+1;
				}
				sll_node_t* r=o;
				o=_optimize(o+1,r,o_dt,OPTIMIZER_FLAG_ARGUMENT);
				sll_object_t* v=_get_as_object(r+1,o_dt,0);
				l--;
				if (SLL_OBJECT_GET_TYPE(v)==OBJECT_TYPE_UNKNOWN||(v->t&OBJECT_CHANGE_IN_LOOP)){
					SLL_RELEASE(v);
					while (l){
						o=_optimize(o,r,o_dt,OPTIMIZER_FLAG_ARGUMENT);
						l--;
					}
					return o;
				}
				sll_object_t** a=sll_allocate(l*sizeof(sll_object_t*));
				sll_arg_count_t i=0;
				while (l){
					sll_node_t* tmp=o;
					o=_optimize(o,r,o_dt,OPTIMIZER_FLAG_ARGUMENT);
					if (a){
						sll_object_t* av=_get_as_object(tmp,o_dt,0);
						*(a+i)=av;
						i++;
						if (SLL_OBJECT_GET_TYPE(av)==OBJECT_TYPE_UNKNOWN||(av->t&OBJECT_CHANGE_IN_LOOP)){
							while (i){
								i--;
								SLL_RELEASE(*(a+i));
							}
							sll_deallocate(a);
							a=NULL;
						}
					}
					l--;
				}
				if (a){
					sll_object_t* iv=sll_operator_cast(v,sll_static_int[SLL_OBJECT_TYPE_INT]);
					SLL_RELEASE(v);
					if (iv->dt.i>SLL_MAX_OBJECT_TYPE){
						SLL_UNIMPLEMENTED();
					}
					sll_object_t* n=sll_create_object_type(NULL,(iv->dt.i<0?SLL_OBJECT_TYPE_INT:(sll_object_type_t)(iv->dt.i)),a,i);
					SLL_RELEASE(iv);
					while (i){
						i--;
						SLL_RELEASE(*(a+i));
					}
					sll_deallocate(a);
					SLL_ASSERT(n);
					r->t=SLL_NODE_TYPE_COMMA;
					r->dt.ac++;
					_shift_nodes(o,o_dt->c_dt,1);
					_object_to_node(n,o,o_dt);
					SLL_RELEASE(n);
					o=r;
					goto _optimize_operation_list_comma;
				}
				return o;
			}
		case SLL_NODE_TYPE_BREAK:
		case SLL_NODE_TYPE_CONTINUE:
			{
				sll_arg_count_t l=o->dt.ac;
				sll_node_t* r=o;
				o++;
				while (l){
					l--;
					o=_optimize(o,r,o_dt,0);
					if (o_dt->rm){
						SLL_UNIMPLEMENTED();
					}
				}
				return o;
			}
		case SLL_NODE_TYPE_REF:
			{
				if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
					o->t=SLL_NODE_TYPE_OPERATION_LIST;
					goto _optimize_operation_list_comma;
				}
				if (fl&OPTIMIZER_FLAG_ASSIGN){
					SLL_UNIMPLEMENTED();
				}
				sll_arg_count_t l=o->dt.ac;
				sll_node_t* r=o;
				o++;
				if (l){
					o=_optimize(o,r,o_dt,OPTIMIZER_FLAG_ARGUMENT);
					l--;
					while (l){
						l--;
						o=_optimize(o,r,o_dt,0);
					}
				}
				return o;
			}
		case SLL_NODE_TYPE_RETURN:
			{
				sll_arg_count_t l=o->dt.ac;
				sll_node_t* r=o;
				o++;
				while (l){
					l--;
					o=_optimize(o,r,o_dt,OPTIMIZER_FLAG_ARGUMENT);
					if (o_dt->rm){
						SLL_UNIMPLEMENTED();
					}
				}
				o_dt->rm=1;
				return o;
			}
		case SLL_NODE_TYPE_COMMA:
		case SLL_NODE_TYPE_OPERATION_LIST:
_optimize_operation_list_comma:
			{
				sll_bool_t rm=(p&&(p->t==SLL_NODE_TYPE_FUNC||p->t==SLL_NODE_TYPE_INLINE_FUNC||p->t==SLL_NODE_TYPE_COMMA||p->t==SLL_NODE_TYPE_OPERATION_LIST));
				sll_arg_count_t l=o->dt.ac;
				sll_node_t* r=o;
				o++;
				while (l){
					l--;
					o=_optimize(o,r,o_dt,(!rm&&!l&&r->t==SLL_NODE_TYPE_COMMA?OPTIMIZER_FLAG_ARGUMENT:0));
					if (o_dt->rm){
						o=_remove_up_to_end(o,l);
						sll_node_t* e=r+1;
						r->dt.ac=0;
						do{
							r->dt.ac++;
							e=sll_skip_node(e);
						} while (e!=o);
						break;
					}
				}
				if (!r->dt.ac){
					if (fl&OPTIMIZER_FLAG_ARGUMENT){
						r->t=SLL_NODE_TYPE_INT;
						r->dt.i=0;
					}
					else{
						r->t=SLL_NODE_TYPE_NOP;
						DECREASE_PARENT(p);
					}
				}
				else if (r->dt.ac==1&&(r->t==SLL_NODE_TYPE_COMMA||!(fl&OPTIMIZER_FLAG_ARGUMENT))){
					r->t=SLL_NODE_TYPE_NOP;
				}
				else if (p&&(p->t==SLL_NODE_TYPE_FUNC||p->t==SLL_NODE_TYPE_INLINE_FUNC||p->t==SLL_NODE_TYPE_COMMA||p->t==SLL_NODE_TYPE_OPERATION_LIST)){
					INCREASE_PARENT(p,r->dt.ac-1);
					r->t=SLL_NODE_TYPE_NOP;
				}
				return o;
			}
		default:
			SLL_UNREACHABLE();
			return 0;
	}
}



static sll_node_t* _merge_print_strings(sll_node_t* o,optimizer_data_t* o_dt){
	SLL_ASSERT(o->t==SLL_NODE_TYPE_PRINT);
	sll_arg_count_t l=o->dt.ac;
	sll_node_t* r=o;
	o++;
	sll_node_t* a=NULL;
	while (l){
		l--;
		while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
			o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
		}
		if (o->t==SLL_NODE_TYPE_CHAR||o->t==SLL_NODE_TYPE_STRING){
			if (a){
				sll_string_t tmp;
				if (a->t==SLL_NODE_TYPE_CHAR&&o->t==SLL_NODE_TYPE_CHAR){
					sll_string_concat_chars(a->dt.c,o->dt.c,&tmp);
				}
				else if (a->t==SLL_NODE_TYPE_CHAR&&o->t==SLL_NODE_TYPE_STRING){
					sll_string_prepend_char(o_dt->c_dt->st.dt+o->dt.s,a->dt.c,&tmp);
				}
				else if (a->t==SLL_NODE_TYPE_STRING&&o->t==SLL_NODE_TYPE_CHAR){
					sll_string_concat_char(o_dt->c_dt->st.dt+a->dt.s,o->dt.c,&tmp);
				}
				else{
					SLL_ASSERT(a->t==SLL_NODE_TYPE_STRING&&o->t==SLL_NODE_TYPE_STRING);
					sll_string_concat(o_dt->c_dt->st.dt+a->dt.s,o_dt->c_dt->st.dt+o->dt.s,&tmp);
				}
				a->t=SLL_NODE_TYPE_NOP;
				o->t=SLL_NODE_TYPE_STRING;
				o->dt.s=sll_add_string(&(o_dt->c_dt->st),&tmp,1);
				r->dt.ac--;
			}
			a=o;
			o++;
		}
		else{
			a=NULL;
			o=sll_skip_node(o);
		}
	}
	return o;
}



static sll_node_t* _remap_indexes_merge_print(sll_node_t* o,sll_node_t* p,optimizer_data_t* o_dt,sll_function_index_t* fn_m){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
		o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t l=o->dt.al;
				sll_node_t* r=o;
				o++;
				while (l){
					l--;
					o=_remap_indexes_merge_print(o,r,o_dt,fn_m);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				sll_node_t* r=o;
				o++;
				while (l){
					l--;
					o=_remap_indexes_merge_print(o,r,o_dt,fn_m);
				}
				return o;
			}
		case SLL_NODE_TYPE_IDENTIFIER:
			SLL_ASSERT(!GET_VARIABLE_REMOVE(o,o_dt));
			if (SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
				o->dt.id=*(o_dt->im.l+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id));
			}
			else{
				o->dt.id=*(o_dt->im.s[SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id));
			}
			return o+1;
		case SLL_NODE_TYPE_FUNCTION_ID:
			o->dt.fn_id=*(fn_m+o->dt.fn_id);
			return o+1;
		case SLL_NODE_TYPE_PRINT:
			{
				sll_arg_count_t l=o->dt.ac;
				sll_node_t* r=o;
				o++;
				while (l){
					l--;
					o=_remap_indexes_merge_print(o,r,o_dt,fn_m);
				}
				return _merge_print_strings(r,o_dt);
			}
		case SLL_NODE_TYPE_ASSIGN:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l>=2);
				sll_node_t* r=o;
				o++;
				while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
					o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				sll_bool_t rm=0;
				if (o->t==SLL_NODE_TYPE_IDENTIFIER){
					rm=GET_VARIABLE_REMOVE(o,o_dt);
					if (rm){
						r->t=SLL_NODE_TYPE_OPERATION_LIST;
						r->dt.ac=l-1;
						o->t=SLL_NODE_TYPE_NOP;
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
				}
				else{
					o=_remap_indexes_merge_print(o,r,o_dt,fn_m);
				}
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
		case SLL_NODE_TYPE_FUNC:
			o->dt.fn.id=*(fn_m+o->dt.fn.id);
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				sll_node_t* r=o;
				o++;
				while (l){
					l--;
					o=_remap_indexes_merge_print(o,r,o_dt,fn_m);
				}
				return o;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
			{
				sll_arg_count_t l=o->dt.l.ac;
				sll_node_t* r=o;
				o++;
				while (l){
					l--;
					o=_remap_indexes_merge_print(o,r,o_dt,fn_m);
				}
				return o;
			}
		case SLL_NODE_TYPE_DECL:
			{
				sll_arg_count_t l=o->dt.d.ac;
				sll_node_t* r=o;
				o++;
				while (l){
					l--;
					o=_remap_indexes_merge_print(o,r,o_dt,fn_m);
				}
				return o;
			}
		case SLL_NODE_TYPE_COMMA:
		case SLL_NODE_TYPE_OPERATION_LIST:
			{
				sll_arg_count_t l=o->dt.ac;
				sll_node_t* r=o;
				o++;
				while (l){
					l--;
					o=_remap_indexes_merge_print(o,r,o_dt,fn_m);
				}
				o=r+1;
				sll_node_t* a=NULL;
				sll_arg_count_t i=r->dt.ac;
				do{
					i--;
					while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
						o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
					}
					sll_node_t* n=sll_skip_node(o);
					if (o->t==SLL_NODE_TYPE_PRINT){
						if (a){
							a->dt.ac+=o->dt.ac;
							o->t=SLL_NODE_TYPE_NOP;
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
	sll_node_t* r=o;
	o++;
	while (l){
		l--;
		o=_remap_indexes_merge_print(o,r,o_dt,fn_m);
	}
	return o;
}



__SLL_EXTERNAL void sll_optimize_node(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft){
	optimizer_data_t o_dt={
		c_dt,
		i_ft,
		{
			.l_im=sll_allocate(c_dt->idt.ill*sizeof(identifier_data_t)),
			.n_vi=0,
			.l_sc=0,
			.sc_vi=sll_allocate(c_dt->_n_sc_id*sizeof(sll_variable_index_t)),
			.vc=0
		},
		.l_sm_l=(c_dt->idt.ill+63)>>6,
		.a_v=NULL,
		.rm=0
	};
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		o_dt.it.s_im[i]=sll_allocate(c_dt->idt.s[i].l*sizeof(identifier_data_t));
		o_dt.s_sm_l[i]=(c_dt->idt.s[i].l+63)>>6;
		o_dt.va.s_sm[i]=sll_zero_allocate(o_dt.s_sm_l[i]*sizeof(uint64_t));
		for (sll_identifier_list_length_t j=0;j<c_dt->idt.s[i].l;j++){
			(o_dt.it.s_im[i]+j)->v=SLL_MAX_VARIABLE_INDEX;
			(o_dt.it.s_im[i]+j)->rm=1;
		}
	}
	o_dt.va.l_sm=sll_zero_allocate(o_dt.l_sm_l*sizeof(uint64_t));
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
		const sll_node_t* fo=_get_node_at_offset(c_dt,fn->off);
		SLL_ASSERT(fo->t==SLL_NODE_TYPE_FUNC);
		sll_arg_count_t ac=fo->dt.fn.ac;
		fo++;
		while (ac){
			ac--;
			fo=_map_identifiers(fo,c_dt,&(o_dt.it));
		}
	}
	sll_deallocate(o_dt.it.sc_vi);
	for (sll_export_table_length_t i=0;i<c_dt->et.l;i++){
		sll_identifier_index_t j=*(c_dt->et.dt+i);
		if (SLL_IDENTIFIER_GET_ARRAY_ID(j)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
			(o_dt.it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX(j))->rm=0;
		}
		else{
			(o_dt.it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID(j)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(j))->rm=0;
		}
	}
	o_dt.v=sll_allocate(o_dt.it.vc*sizeof(sll_object_t*));
	for (sll_variable_index_t i=0;i<o_dt.it.vc;i++){
		sll_object_t* v=SLL_CREATE();
		v->t=OBJECT_TYPE_UNKNOWN;
		*(o_dt.v+i)=v;
	}
	_optimize(c_dt->h,NULL,&o_dt,0);
	sll_deallocate(o_dt.va.l_sm);
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		sll_deallocate(o_dt.va.s_sm[i]);
		o_dt.im.s[i]=sll_allocate(c_dt->idt.s[i].l*sizeof(sll_identifier_index_t));
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
			c_dt->idt.s[i].dt=sll_reallocate(c_dt->idt.s[i].dt,c_dt->idt.s[i].l*sizeof(sll_identifier_t));
		}
	}
	o_dt.im.l=sll_allocate(c_dt->idt.ill*sizeof(sll_identifier_index_t));
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
		c_dt->idt.il=sll_reallocate(c_dt->idt.il,c_dt->idt.ill*sizeof(sll_identifier_t));
	}
	for (sll_export_table_length_t i=0;i<c_dt->et.l;i++){
		if (SLL_IDENTIFIER_GET_ARRAY_ID(*(c_dt->et.dt+i))==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
			SLL_ASSERT(!(o_dt.it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX(*(c_dt->et.dt+i)))->rm);
			*(c_dt->et.dt+i)=*(o_dt.im.l+SLL_IDENTIFIER_GET_ARRAY_INDEX(*(c_dt->et.dt+i)));
		}
		else{
			SLL_ASSERT(!(o_dt.it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID(*(c_dt->et.dt+i))]+SLL_IDENTIFIER_GET_ARRAY_INDEX(*(c_dt->et.dt+i)))->rm);
			*(c_dt->et.dt+i)=*(o_dt.im.s[SLL_IDENTIFIER_GET_ARRAY_ID(*(c_dt->et.dt+i))]+SLL_IDENTIFIER_GET_ARRAY_INDEX(*(c_dt->et.dt+i)));
		}
	}
	sll_function_index_t fn_off=0;
	sll_function_index_t* fn_m=sll_allocate(c_dt->ft.l*sizeof(sll_function_index_t));
	for (sll_function_index_t i=0;i<c_dt->ft.l;i++){
		sll_function_t* f=*(c_dt->ft.dt+i);
		if (!f){
			SLL_ASSERT(0);
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
			sll_deallocate(c_dt->ft.dt);
			c_dt->ft.dt=NULL;
		}
		else{
			c_dt->ft.dt=sll_reallocate(c_dt->ft.dt,c_dt->ft.l*sizeof(sll_function_t*));
		}
	}
	o_dt.rm=0;
	_remap_indexes_merge_print(c_dt->h,NULL,&o_dt,fn_m);
	sll_deallocate(fn_m);
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		sll_deallocate(o_dt.it.s_im[i]);
		sll_deallocate(o_dt.im.s[i]);
	}
	sll_deallocate(o_dt.it.l_im);
	sll_deallocate(o_dt.im.l);
	for (sll_variable_index_t i=0;i<o_dt.it.vc;i++){
		SLL_RELEASE(*(o_dt.v+i));
	}
	sll_deallocate(o_dt.v);
}
