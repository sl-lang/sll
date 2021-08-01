#include <lll_lib.h>
#include <lll_lib_api.h>
#include <_lll_lib_internal.h>
#include <stdint.h>
#include <stdlib.h>



#define DECREASE_PARENT(p) \
	do{ \
		if (p){ \
			if ((p)->t==LLL_OBJECT_TYPE_FUNC||(p)->t==LLL_OBJECT_TYPE_INTERNAL_FUNC){ \
				(p)->dt.fn.ac--; \
			} \
			else if ((p)->t==LLL_OBJECT_TYPE_OPERATION_LIST){ \
				(p)->dt.sc--; \
			} \
			else{ \
				(p)->dt.ac--; \
			} \
		} \
	} while (0)
#define DECREASE_VARIABLE(o,o_dt) \
	do{ \
		if (LLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)==LLL_MAX_SHORT_IDENTIFIER_LENGTH){ \
			((o_dt)->it.l_im+LLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->c--; \
		} \
		else{ \
			((o_dt)->it.s_im[LLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)]+LLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->c--; \
		} \
	} while (0)
#define GET_VARIABLE_INDEX(o,o_dt) (LLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)==LLL_MAX_SHORT_IDENTIFIER_LENGTH?((o_dt)->it.l_im+LLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->v:((o_dt)->it.s_im[LLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)]+LLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->v)
#define GET_VARIABLE_REF_COUNT(o,o_dt) (LLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)==LLL_MAX_SHORT_IDENTIFIER_LENGTH?((o_dt)->it.l_im+LLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->c:((o_dt)->it.s_im[LLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)]+LLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->c)



void _remove_up_to_end(lll_object_t* o,lll_object_offset_t off){
	lll_object_offset_t sz=lll_get_object_size(o);
	for (;off<sz;off++){
		(o+off)->t=LLL_OBJECT_TYPE_NOP;
	}
}



lll_object_offset_t _map_identifiers(const lll_object_t* o,const lll_compilation_data_t* c_dt,identifier_map_data_t* im){
	lll_object_offset_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP||o->t==LLL_OBJECT_TYPE_DEBUG_DATA){
		eoff++;
		o++;
	}
	switch (o->t){
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_CHAR:
		case LLL_OBJECT_TYPE_STRING:
		case LLL_OBJECT_TYPE_INT:
		case LLL_OBJECT_TYPE_FLOAT:
			return eoff+1;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			{
				lll_identifier_index_t i=LLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id);
				uint8_t j=LLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id);
				if (j==LLL_MAX_SHORT_IDENTIFIER_LENGTH){
					lll_identifier_t* id=c_dt->idt.il+i;
					if (im->l_sc!=id->sc){
						*(im->sc_vi+im->l_sc)=im->n_vi;
						im->l_sc=id->sc;
						if (*(im->sc_vi+im->l_sc)!=LLL_MAX_SCOPE){
							im->n_vi=*(im->sc_vi+im->l_sc);
						}
					}
					(im->l_im+i)->c++;
					if ((im->l_im+i)->v==LLL_MAX_VARIABLE_INDEX){
						(im->l_im+i)->v=im->n_vi;
						im->n_vi++;
						if (im->n_vi>im->vc){
							im->vc=im->n_vi;
						}
					}
				}
				else{
					lll_identifier_t* id=c_dt->idt.s[j].dt+i;
					if (im->l_sc!=id->sc){
						*(im->sc_vi+im->l_sc)=im->n_vi;
						im->l_sc=id->sc;
						if (*(im->sc_vi+im->l_sc)!=LLL_MAX_SCOPE){
							im->n_vi=*(im->sc_vi+im->l_sc);
						}
					}
					(im->s_im[j]+i)->c++;
					if ((im->s_im[j]+i)->v==LLL_MAX_VARIABLE_INDEX){
						(im->s_im[j]+i)->v=im->n_vi;
						im->n_vi++;
						if (im->n_vi>im->vc){
							im->vc=im->n_vi;
						}
					}
				}
				return eoff+1;
			}
		case LLL_OBJECT_TYPE_FUNC:
			return lll_get_object_size(o)+eoff;
		case LLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				lll_object_offset_t off=1;
				lll_arg_count_t l=o->dt.ac;
				while (l){
					l--;
					off+=_map_identifiers(o+off,c_dt,im);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_object_offset_t off=1;
				lll_statement_count_t l=o->dt.sc;
				while (l){
					l--;
					off+=_map_identifiers(o+off,c_dt,im);
				}
				return off+eoff;
			}
	}
	lll_object_offset_t off=1;
	lll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		off+=_map_identifiers(o+off,c_dt,im);
	}
	return off+eoff;
}



void _get_as_runtime_object(const lll_object_t* o,const optimizer_data_t* o_dt,lll_runtime_object_t* v){
	while (o->t==LLL_OBJECT_TYPE_NOP||o->t==LLL_OBJECT_TYPE_DEBUG_DATA){
		o++;
	}
	switch (o->t){
		case LLL_OBJECT_TYPE_PRINT:
		case LLL_OBJECT_TYPE_IF:
		case LLL_OBJECT_TYPE_FOR:
		case LLL_OBJECT_TYPE_RETURN:
		case LLL_OBJECT_TYPE_EXIT:
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			v->t=LLL_RUNTIME_OBJECT_TYPE_INT;
			v->dt.i=0;
			return;
		case LLL_OBJECT_TYPE_CHAR:
			v->t=LLL_RUNTIME_OBJECT_TYPE_CHAR;
			v->dt.c=o->dt.c;
			return;
		case LLL_OBJECT_TYPE_INT:
			v->t=LLL_RUNTIME_OBJECT_TYPE_INT;
			v->dt.i=o->dt.i;
			return;
		case LLL_OBJECT_TYPE_FLOAT:
			v->t=LLL_RUNTIME_OBJECT_TYPE_FLOAT;
			v->dt.f=o->dt.f;
			return;
		case LLL_OBJECT_TYPE_STRING:
			v->t=LLL_RUNTIME_OBJECT_TYPE_STRING;
			v->dt.s=*(o_dt->c_dt->st.dt+o->dt.s);
			return;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			*v=*(o_dt->v+GET_VARIABLE_INDEX(o,o_dt));
			return;
		case LLL_OBJECT_TYPE_FUNC:
		case LLL_OBJECT_TYPE_INTERNAL_FUNC:
			v->t=LLL_RUNTIME_OBJECT_TYPE_INT;
			v->dt.i=(o->t==LLL_OBJECT_TYPE_FUNC?o->dt.fn.id:~((lll_integer_t)o->dt.fn.id));
			return;
		default:
			v->t=RUNTIME_OBJECT_TYPE_UNKNOWN;
	}
}



lll_object_offset_t _mark_loop_vars(const lll_object_t* o,optimizer_data_t* o_dt){
	lll_object_offset_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP||o->t==LLL_OBJECT_TYPE_DEBUG_DATA){
		eoff++;
		o++;
	}
	switch (o->t){
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_CHAR:
		case LLL_OBJECT_TYPE_STRING:
		case LLL_OBJECT_TYPE_IDENTIFIER:
		case LLL_OBJECT_TYPE_INT:
		case LLL_OBJECT_TYPE_FLOAT:
			return eoff+1;
		case LLL_OBJECT_TYPE_ASSIGN:
			{
				lll_arg_count_t l=o->dt.ac;
				ASSERT(l>=2);
				lll_object_offset_t off=1;
				while ((o+off)->t==LLL_OBJECT_TYPE_NOP||(o+off)->t==LLL_OBJECT_TYPE_DEBUG_DATA){
					off++;
				}
				ASSERT((o+off)->t==LLL_OBJECT_TYPE_IDENTIFIER);
				lll_runtime_object_t tmp;
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
		case LLL_OBJECT_TYPE_FUNC:
		case LLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				lll_object_offset_t off=1;
				lll_arg_count_t l=o->dt.fn.ac;
				while (l){
					l--;
					off+=_mark_loop_vars(o+off,o_dt);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_object_offset_t off=1;
				lll_statement_count_t l=o->dt.sc;
				while (l){
					l--;
					off+=_mark_loop_vars(o+off,o_dt);
				}
				return off+eoff;
			}
	}
	lll_object_offset_t off=1;
	lll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		off+=_mark_loop_vars(o+off,o_dt);
	}
	return off+eoff;
}



uint8_t _get_cond_type(lll_object_t* o,optimizer_data_t* o_dt,uint8_t inv,uint8_t lv){
	lll_object_offset_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP||o->t==LLL_OBJECT_TYPE_DEBUG_DATA){
		eoff++;
		o++;
	}
	switch (o->t){
		case LLL_OBJECT_TYPE_UNKNOWN:
			return COND_TYPE_UNKNOWN;
		case LLL_OBJECT_TYPE_CHAR:
			return ((!!o->dt.c)^inv?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
		case LLL_OBJECT_TYPE_INT:
			return ((!!o->dt.i)^inv?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
		case LLL_OBJECT_TYPE_FLOAT:
			return ((!!o->dt.f)^inv?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
		case LLL_OBJECT_TYPE_STRING:
			return ((!!(*(o_dt->c_dt->st.dt+o->dt.s))->l)^inv?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
		case LLL_OBJECT_TYPE_IDENTIFIER:
			{
				lll_runtime_object_t* v=o_dt->v+GET_VARIABLE_INDEX(o,o_dt);
				DECREASE_VARIABLE(o,o_dt);
				if (lv||!(v->t&RUNTIME_OBJECT_CHANGE_IN_LOOP)){
					switch (LLL_RUNTIME_OBJECT_GET_TYPE(v)){
						case LLL_RUNTIME_OBJECT_TYPE_INT:
							return ((!!v->dt.i)^inv?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
						case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
							return ((!!v->dt.f)^inv?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
						case LLL_RUNTIME_OBJECT_TYPE_CHAR:
							return ((!!v->dt.c)^inv?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
						case LLL_RUNTIME_OBJECT_TYPE_STRING:
							return ((!!v->dt.s->l)^inv?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
					}
				}
				return COND_TYPE_UNKNOWN;
			}
		case LLL_OBJECT_TYPE_PRINT:
		case LLL_OBJECT_TYPE_FUNC:
		case LLL_OBJECT_TYPE_INTERNAL_FUNC:
		case LLL_OBJECT_TYPE_IF:
		case LLL_OBJECT_TYPE_FOR:
		case LLL_OBJECT_TYPE_WHILE:
		case LLL_OBJECT_TYPE_LOOP:
		case LLL_OBJECT_TYPE_RETURN:
		case LLL_OBJECT_TYPE_EXIT:
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			return (inv?COND_TYPE_ALWAYS_TRUE:COND_TYPE_ALWAYS_FALSE);
		case LLL_OBJECT_TYPE_INPUT:
			ASSERT(!"Unimplemented");
		case LLL_OBJECT_TYPE_AND:
			ASSERT(!"Unimplemented");
		case LLL_OBJECT_TYPE_OR:
			ASSERT(!"Unimplemented");
		case LLL_OBJECT_TYPE_NOT:
			ASSERT(o->dt.ac);
			return _get_cond_type(o+1,o_dt,!inv,lv);
		case LLL_OBJECT_TYPE_ASSIGN:
			ASSERT(o->dt.ac>=2);
			return _get_cond_type(o+lll_get_object_size(o+1)+1,o_dt,inv,lv);
		case LLL_OBJECT_TYPE_CALL:
		case LLL_OBJECT_TYPE_ADD:
		case LLL_OBJECT_TYPE_SUB:
		case LLL_OBJECT_TYPE_MULT:
		case LLL_OBJECT_TYPE_DIV:
		case LLL_OBJECT_TYPE_FLOOR_DIV:
		case LLL_OBJECT_TYPE_MOD:
		case LLL_OBJECT_TYPE_BIT_AND:
		case LLL_OBJECT_TYPE_BIT_OR:
		case LLL_OBJECT_TYPE_BIT_XOR:
		case LLL_OBJECT_TYPE_BIT_NOT:
		case LLL_OBJECT_TYPE_LESS:
		case LLL_OBJECT_TYPE_LESS_EQUAL:
		case LLL_OBJECT_TYPE_EQUAL:
		case LLL_OBJECT_TYPE_NOT_EQUAL:
		case LLL_OBJECT_TYPE_MORE:
		case LLL_OBJECT_TYPE_MORE_EQUAL:
			return COND_TYPE_UNKNOWN;
		default:
			UNREACHABLE();
			return COND_TYPE_UNKNOWN;
	}
}



lll_object_offset_t _optimize(lll_object_t* o,lll_object_t* p,optimizer_data_t* o_dt,uint8_t fl){
	lll_object_offset_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP||o->t==LLL_OBJECT_TYPE_DEBUG_DATA){
		eoff++;
		o++;
	}
	ASSERT(!o_dt->rm);
	if (o_dt->vi!=LLL_MAX_VARIABLE_INDEX){
		_get_as_runtime_object(o,o_dt,o_dt->v+o_dt->vi);
		o_dt->vi=LLL_MAX_VARIABLE_INDEX;
	}
	switch (o->t){
		case LLL_OBJECT_TYPE_UNKNOWN:
			return eoff+1;
		case LLL_OBJECT_TYPE_CHAR:
		case LLL_OBJECT_TYPE_STRING:
		case LLL_OBJECT_TYPE_INT:
		case LLL_OBJECT_TYPE_FLOAT:
			if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
				DECREASE_PARENT(p);
				o->t=LLL_OBJECT_TYPE_NOP;
			}
			return eoff+1;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
				DECREASE_PARENT(p);
				o->t=LLL_OBJECT_TYPE_NOP;
			}
			else{
				lll_runtime_object_t* v=o_dt->v+GET_VARIABLE_INDEX(o,o_dt);
				if (LLL_RUNTIME_OBJECT_GET_TYPE(v)!=RUNTIME_OBJECT_TYPE_UNKNOWN&&(!(v->t&RUNTIME_OBJECT_CHANGE_IN_LOOP)||(fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG))){
					DECREASE_VARIABLE(o,o_dt);
					switch (LLL_RUNTIME_OBJECT_GET_TYPE(v)){
						case LLL_RUNTIME_OBJECT_TYPE_INT:
							o->t=LLL_OBJECT_TYPE_INT;
							o->dt.i=v->dt.i;
							break;
						case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
							o->t=LLL_OBJECT_TYPE_FLOAT;
							o->dt.f=v->dt.f;
							break;
						case LLL_RUNTIME_OBJECT_TYPE_CHAR:
							o->t=LLL_OBJECT_TYPE_CHAR;
							o->dt.c=v->dt.c;
							break;
						case LLL_RUNTIME_OBJECT_TYPE_STRING:
							ASSERT(!"Unimplemented");
						default:
							UNREACHABLE();
					}
				}
			}
			return eoff+1;
		case LLL_OBJECT_TYPE_ASSIGN:
			{
				lll_arg_count_t l=o->dt.ac;
				ASSERT(l>=2);
				lll_object_offset_t off=1;
				while ((o+off)->t==LLL_OBJECT_TYPE_NOP||(o+off)->t==LLL_OBJECT_TYPE_DEBUG_DATA){
					off++;
				}
				lll_object_t* id_o=o+off;
				ASSERT(id_o->t==LLL_OBJECT_TYPE_IDENTIFIER);
				DECREASE_VARIABLE(id_o,o_dt);
				o_dt->vi=GET_VARIABLE_INDEX(id_o,o_dt);
				off++;
				off+=_optimize(o+off,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
				if (o_dt->rm){
					o->t=LLL_OBJECT_TYPE_NOP;
					id_o->t=LLL_OBJECT_TYPE_NOP;
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
		case LLL_OBJECT_TYPE_FUNC:
			return lll_get_object_size(o)+eoff;
		case LLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				if (!(fl&OPTIMIZER_FLAG_ARGUMENT)){
					o->t=LLL_OBJECT_TYPE_OPERATION_LIST;
					o->dt.sc=o->dt.fn.ac;
					return _optimize(o,p,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG)+eoff;
				}
				lll_object_offset_t off=1;
				lll_arg_count_t l=o->dt.fn.ac;
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
		case LLL_OBJECT_TYPE_IF:
			{
				lll_arg_count_t l=o->dt.ac;
				ASSERT(l);
				if (l==1){
					o->t=LLL_OBJECT_TYPE_NOP;
					return _optimize(o+1,p,o_dt,fl)+eoff;
				}
				lll_object_offset_t off=1;
				if (l&1){
					l>>=1;
					while (l){
						l--;
						lll_object_t* cnd_o=o+off;
						off+=_optimize(o+off,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
						if (o_dt->rm){
							ASSERT(!"Unimplemented");
						}
						uint8_t cnd=_get_cond_type(cnd_o,o_dt,0,0);
						if (cnd==COND_TYPE_ALWAYS_TRUE){
							if (o->dt.ac-(l<<1)==3){
								lll_object_offset_t sz=lll_get_object_size(o);
								o->t=LLL_OBJECT_TYPE_NOP;
								off+=_optimize(o+off,p,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
								while (off<sz){
									(o+off)->t=LLL_OBJECT_TYPE_NOP;
									off++;
								}
							}
							else{
								off+=_optimize(o+off,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
								o_dt->rm=0;
								_remove_up_to_end(o,off);
								o->dt.ac-=(l<<1)+2;
							}
							while (cnd_o->t==LLL_OBJECT_TYPE_NOP||cnd_o->t==LLL_OBJECT_TYPE_DEBUG_DATA){
								cnd_o++;
							}
							cnd_o->t=LLL_OBJECT_TYPE_NOP;
							return off+eoff;
						}
						else if (cnd==COND_TYPE_ALWAYS_FALSE){
							o->dt.ac-=2;
							if (o->dt.ac==1){
								o->t=LLL_OBJECT_TYPE_NOP;
							}
							_remove_up_to_end(cnd_o,0);
							_remove_up_to_end(o+off,0);
							continue;
						}
						off+=_optimize(o+off,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
						o_dt->rm=0;
					}
					if (o->t!=LLL_OBJECT_TYPE_NOP){
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
						lll_object_t* cnd_o=o+off;
						off+=_optimize(cnd_o,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
						if (o_dt->rm){
							ASSERT(!"Unimplemented");
						}
						uint8_t cnd=_get_cond_type(cnd_o,o_dt,0,0);
						if (cnd==COND_TYPE_ALWAYS_TRUE){
							if (o->dt.ac-(l<<1)==2){
								lll_object_offset_t sz=lll_get_object_size(o);
								o->t=LLL_OBJECT_TYPE_NOP;
								off+=_optimize(o+off,p,o_dt,fl&OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
								while (off<sz){
									(o+off)->t=LLL_OBJECT_TYPE_NOP;
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
								o->t=LLL_OBJECT_TYPE_NOP;
								DECREASE_PARENT(p);
							}
							lll_object_offset_t sz=lll_get_object_size(cnd_o);
							for (lll_object_offset_t i=0;i<sz;i++){
								(cnd_o+i)->t=LLL_OBJECT_TYPE_NOP;
							}
							sz=lll_get_object_size(o+off);
							for (lll_object_offset_t i=0;i<sz;i++){
								(o+off+i)->t=LLL_OBJECT_TYPE_NOP;
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
		case LLL_OBJECT_TYPE_FOR:
			{
				lll_arg_count_t l=o->dt.ac;
				ASSERT(l);
				if (l==1){
					o->t=LLL_OBJECT_TYPE_NOP;
					return _optimize(o+1,p,o_dt,fl)+eoff;
				}
				lll_object_offset_t off=_optimize(o+1,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT)+1;
				if (o_dt->rm){
					o->t=LLL_OBJECT_TYPE_NOP;
					_remove_up_to_end(o,off);
					return off+eoff;
				}
				lll_object_t* cnd_o=o+off;
				lll_object_offset_t off2=off;
				for (lll_arg_count_t i=1;i<l;i++){
					off2+=_mark_loop_vars(o+off2,o_dt);
				}
				lll_object_offset_t cnd_sz=_optimize(cnd_o,NULL,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
				off+=cnd_sz;
				lll_object_t* tmp=malloc(cnd_sz*sizeof(lll_object_t));
				for (lll_object_offset_t i=0;i<cnd_sz;i++){
					*(tmp+i)=*(cnd_o+i);
				}
				_optimize(tmp,NULL,o_dt,OPTIMIZER_FLAG_ARGUMENT|OPTIMIZER_FLAG_IGNORE_LOOP_FLAG);
				if (o_dt->rm){
					ASSERT(!"Unimplemented");
				}
				uint8_t cnd=_get_cond_type(tmp,o_dt,1,1);
				free(tmp);
				if (cnd==COND_TYPE_ALWAYS_TRUE){
					ASSERT(!"Unimplemented");
				}
				else if (cnd==COND_TYPE_ALWAYS_FALSE){
					o->t=LLL_OBJECT_TYPE_WHILE;
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
		case LLL_OBJECT_TYPE_WHILE:
			ASSERT(!"Unimplemented");
		case LLL_OBJECT_TYPE_LOOP:
			ASSERT(!"Unimplemented");
		case LLL_OBJECT_TYPE_LESS:
		case LLL_OBJECT_TYPE_LESS_EQUAL:
		case LLL_OBJECT_TYPE_EQUAL:
		case LLL_OBJECT_TYPE_NOT_EQUAL:
		case LLL_OBJECT_TYPE_MORE:
		case LLL_OBJECT_TYPE_MORE_EQUAL:
			{
				lll_arg_count_t l=o->dt.ac;
				ASSERT(l);
				lll_object_offset_t off=_optimize(o+1,o,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT)+1;
				if (o_dt->rm){
					_remove_up_to_end(o,off);
					o->t=LLL_OBJECT_TYPE_NOP;
					return off+eoff;
				}
				lll_runtime_object_t a;
				_get_as_runtime_object(o+1,o_dt,&a);
				l--;
				if (a.t!=RUNTIME_OBJECT_TYPE_UNKNOWN&&!(a.t&RUNTIME_OBJECT_CHANGE_IN_LOOP)){
					while (l){
						lll_object_t* arg=o+off;
						off+=_optimize(o+off,o,o_dt,fl|OPTIMIZER_FLAG_ARGUMENT);
						if (o_dt->rm){
							_remove_up_to_end(o,off);
							o->dt.ac-=l-1;
							return off+eoff;
						}
						lll_runtime_object_t b;
						_get_as_runtime_object(arg,o_dt,&b);
						if (b.t==RUNTIME_OBJECT_TYPE_UNKNOWN||(b.t&RUNTIME_OBJECT_CHANGE_IN_LOOP)){
							break;
						}
						l--;
						lll_compare_result_t cmp=lll_compare_runtime_object(&a,&b);
						ASSERT(cmp!=LLL_COMPARE_RESULT_ERROR);
						if ((o->t==LLL_OBJECT_TYPE_LESS&&cmp==LLL_COMPARE_RESULT_BELOW)||(o->t==LLL_OBJECT_TYPE_LESS_EQUAL&&cmp!=LLL_COMPARE_RESULT_ABOVE)||(o->t==LLL_OBJECT_TYPE_EQUAL&&cmp==LLL_COMPARE_RESULT_EQUAL)||(o->t==LLL_OBJECT_TYPE_NOT_EQUAL&&cmp!=LLL_COMPARE_RESULT_EQUAL)||(o->t==LLL_OBJECT_TYPE_MORE&&cmp==LLL_COMPARE_RESULT_ABOVE)||(o->t==LLL_OBJECT_TYPE_MORE_EQUAL&&cmp!=LLL_COMPARE_RESULT_BELOW)){
							a=b;
							continue;
						}
						_remove_up_to_end(o,off);
						o->t=LLL_OBJECT_TYPE_OPERATION_LIST;
						o->dt.sc=o->dt.ac-l;
						return _optimize(o,p,o_dt,fl)+eoff;
					}
					if (!l){
						_remove_up_to_end(o,1);
						o->t=LLL_OBJECT_TYPE_INT;
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
		case LLL_OBJECT_TYPE_RETURN:
		case LLL_OBJECT_TYPE_EXIT:
			{
				lll_object_offset_t off=1;
				lll_arg_count_t l=o->dt.ac;
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
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_object_offset_t off=1;
				lll_statement_count_t l=o->dt.sc;
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
						o->t=LLL_OBJECT_TYPE_INT;
						o->dt.i=0;
					}
					else{
						o->t=LLL_OBJECT_TYPE_NOP;
						DECREASE_PARENT(p);
					}
				}
				else if (o->dt.sc==1){
					o->t=LLL_OBJECT_TYPE_NOP;
				}
				else if (p&&p->t==LLL_OBJECT_TYPE_OPERATION_LIST){
					p->dt.sc+=o->dt.sc-1;
					o->t=LLL_OBJECT_TYPE_NOP;
				}
				return off+eoff;
			}
	}
	lll_object_offset_t off=1;
	lll_arg_count_t l=o->dt.ac;
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



lll_object_offset_t _remove_const_var(lll_object_t* o,lll_object_t* p,optimizer_data_t* o_dt){
	lll_object_offset_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP||o->t==LLL_OBJECT_TYPE_DEBUG_DATA){
		eoff++;
		o++;
	}
	switch (o->t){
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_CHAR:
		case LLL_OBJECT_TYPE_STRING:
		case LLL_OBJECT_TYPE_INT:
		case LLL_OBJECT_TYPE_FLOAT:
			return eoff+1;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			ASSERT(GET_VARIABLE_REF_COUNT(o,o_dt));
			return eoff+1;
		case LLL_OBJECT_TYPE_ASSIGN:
			{
				lll_arg_count_t l=o->dt.ac;
				ASSERT(l>=2);
				lll_object_offset_t off=1;
				while ((o+off)->t==LLL_OBJECT_TYPE_NOP||(o+off)->t==LLL_OBJECT_TYPE_DEBUG_DATA){
					off++;
				}
				ASSERT((o+off)->t==LLL_OBJECT_TYPE_IDENTIFIER);
				uint8_t rm=!GET_VARIABLE_REF_COUNT(o+off,o_dt);
				if (rm){
					o->t=LLL_OBJECT_TYPE_OPERATION_LIST;
					o->dt.sc=o->dt.ac-1;
					(o+off)->t=LLL_OBJECT_TYPE_NOP;
				}
				off++;
				off+=_remove_const_var(o+off,o,o_dt);
				l-=2;
				while (l){
					l--;
					off+=_remove_const_var(o+off,o,o_dt);
				}
				if (rm){
					_optimize(o,p,o_dt,0);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_FUNC:
		case LLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				lll_object_offset_t off=1;
				lll_arg_count_t l=o->dt.fn.ac;
				while (l){
					l--;
					off+=_remove_const_var(o+off,o,o_dt);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_object_offset_t off=1;
				lll_statement_count_t l=o->dt.sc;
				while (l){
					l--;
					off+=_remove_const_var(o+off,o,o_dt);
				}
				return off+eoff;
			}
	}
	lll_object_offset_t off=1;
	lll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		off+=_remove_const_var(o+off,o,o_dt);
	}
	return off+eoff;
}



__LLL_IMPORT_EXPORT void lll_optimize_object(lll_compilation_data_t* c_dt,lll_object_t* o){
	optimizer_data_t o_dt={
		c_dt,
		{
			.l_im=malloc(c_dt->idt.ill*sizeof(identifier_data_t)),
			.n_vi=0,
			.l_sc=0,
			.sc_vi=malloc(c_dt->_n_sc_id*sizeof(lll_variable_index_t)),
			.vc=0
		},
		.vi=LLL_MAX_VARIABLE_INDEX,
		.rm=0
	};
	for (uint8_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		o_dt.it.s_im[i]=malloc(c_dt->idt.s[i].l*sizeof(identifier_data_t));
		for (lll_identifier_list_length_t j=0;j<c_dt->idt.s[i].l;j++){
			(o_dt.it.s_im[i]+j)->v=LLL_MAX_VARIABLE_INDEX;
			(o_dt.it.s_im[i]+j)->c=0;
		}
	}
	for (lll_identifier_list_length_t i=0;i<c_dt->idt.ill;i++){
		(o_dt.it.l_im+i)->v=LLL_MAX_VARIABLE_INDEX;
		(o_dt.it.l_im+i)->c=0;
	}
	for (lll_scope_t i=0;i<c_dt->_n_sc_id;i++){
		*(o_dt.it.sc_vi+i)=LLL_MAX_VARIABLE_INDEX;
	}
	_map_identifiers(c_dt->h,c_dt,&(o_dt.it));
	for (lll_function_index_t i=0;i<c_dt->ft.l;i++){
		o_dt.it.n_vi=o_dt.it.vc;
		o_dt.it.l_sc=0;
		lll_object_t* fo=c_dt->h+(*(c_dt->ft.dt+i))->off;
		ASSERT(fo->t==LLL_OBJECT_TYPE_FUNC);
		lll_object_offset_t off=1;
		for (lll_arg_count_t j=0;j<fo->dt.fn.ac;j++){
			off+=_map_identifiers(fo+off,c_dt,&(o_dt.it));
		}
	}
	free(o_dt.it.sc_vi);
	for (lll_export_table_length_t i=0;i<c_dt->et.l;i++){
		lll_identifier_index_t j=*(c_dt->et.dt+i);
		if (LLL_IDENTIFIER_GET_ARRAY_ID(j)==LLL_MAX_SHORT_IDENTIFIER_LENGTH){
			(o_dt.it.l_im+LLL_IDENTIFIER_GET_ARRAY_INDEX(j))->c++;
		}
		else{
			(o_dt.it.s_im[LLL_IDENTIFIER_GET_ARRAY_ID(j)]+LLL_IDENTIFIER_GET_ARRAY_INDEX(j))->c++;
		}
	}
	o_dt.v=malloc(o_dt.it.vc*sizeof(lll_runtime_object_t));
	for (lll_variable_index_t i=0;i<o_dt.it.vc;i++){
		(o_dt.v+i)->t=RUNTIME_OBJECT_TYPE_UNKNOWN;
	}
	_optimize(c_dt->h,NULL,&o_dt,0);
	o_dt.rm=0;
	_remove_const_var(c_dt->h,NULL,&o_dt);
	for (uint8_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		lll_identifier_list_length_t k=0;
		for (lll_identifier_list_length_t j=0;j<c_dt->idt.s[i].l;j++){
			*(c_dt->idt.s[i].dt+j-k)=*(c_dt->idt.s[i].dt+j);
			if (!(o_dt.it.s_im[i]+j)->c){
				k++;
			}
		}
		if (k){
			c_dt->idt.s[i].l-=k;
			c_dt->idt.s[i].dt=realloc(c_dt->idt.s[i].dt,c_dt->idt.s[i].l*sizeof(lll_identifier_t));
		}
		free(o_dt.it.s_im[i]);
	}
	lll_identifier_list_length_t j=0;
	for (lll_identifier_list_length_t i=0;i<c_dt->idt.ill;i++){
		*(c_dt->idt.il+i-j)=*(c_dt->idt.il+i);
		if (!(o_dt.it.l_im+i)->c){
			j++;
		}
	}
	if (j){
		c_dt->idt.ill-=j;
		c_dt->idt.il=realloc(c_dt->idt.il,c_dt->idt.ill*sizeof(lll_identifier_t));
	}
	free(o_dt.it.l_im);
	free(o_dt.v);
}
