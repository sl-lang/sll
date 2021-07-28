#include <lll_lib.h>
#include <_lll_lib_internal.h>
#include <stdint.h>
#include <stdlib.h>



#define DEFINE_LABEL(g_dt,id) \
	do{ \
		lll_assembly_instruction_t* __ai=NEXT_INSTRUCTION((g_dt)); \
		__ai->t=ASSEMBLY_INSTRUCTION_TYPE_LABEL_TARGET; \
		ASSEMBLY_INSTRUCTION_MISC_FIELD(__ai)=(id); \
	} while(0)
#define GENERATE_OPCODE(g_dt,op) (NEXT_INSTRUCTION((g_dt))->t=(op))
#define GENERATE_OPCODE_WITH_LABEL(g_dt,op,lbl) \
	do{ \
		lll_assembly_instruction_t* __ai=NEXT_INSTRUCTION((g_dt)); \
		__ai->t=(op)|ASSEMBLY_INSTRUCTION_LABEL; \
		ASSEMBLY_INSTRUCTION_MISC_FIELD(__ai)=(lbl); \
	} while(0)
#define GET_SIGN_ENCODED_INTEGER(x) ((x)<0?((~(x))<<1)|1:(x)<<1)
#define NEXT_INSTRUCTION(g_dt) ((g_dt)->a_dt->h+((g_dt)->a_dt->ic++))
#define NEXT_LABEL(g_dt) ((g_dt)->n_lbl++)



lll_stack_offset_t _generate(const lll_object_t* o,assembly_generator_data_t* g_dt);



lll_stack_offset_t _generate_on_stack(const lll_object_t* o,assembly_generator_data_t* g_dt);



void _get_as_runtime_object(const lll_object_t* o,const assembly_generator_data_t* g_dt,lll_runtime_object_t* v){
	while (o->t==LLL_OBJECT_TYPE_NOP||LLL_GET_OBJECT_TYPE(o)==LLL_OBJECT_TYPE_DEBUG_DATA){
		if (o->t==LLL_OBJECT_TYPE_NOP){
			o=LLL_GET_OBJECT_AFTER_NOP(o);
		}
		else{
			o=LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)o);
		}
	}
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_NIL:
		case LLL_OBJECT_TYPE_PRINT:
		case LLL_OBJECT_TYPE_IF:
		case LLL_OBJECT_TYPE_FOR:
		case LLL_OBJECT_TYPE_RETURN:
		case LLL_OBJECT_TYPE_EXIT:
		case LLL_OBJECT_TYPE_IMPORT:
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			v->t=LLL_RUNTIME_OBJECT_TYPE_INT;
			v->dt.i=0;
			break;
		case LLL_OBJECT_TYPE_CHAR:
			v->t=LLL_RUNTIME_OBJECT_TYPE_CHAR;
			v->dt.c=((lll_char_object_t*)o)->v;
			break;
		case LLL_OBJECT_TYPE_INT:
			v->t=LLL_RUNTIME_OBJECT_TYPE_INT;
			v->dt.i=((lll_integer_object_t*)o)->v;
			break;
		case LLL_OBJECT_TYPE_FLOAT:
			v->t=LLL_RUNTIME_OBJECT_TYPE_FLOAT;
			v->dt.f=((lll_float_object_t*)o)->v;
			break;
		case LLL_OBJECT_TYPE_STRING:
			v->t=LLL_RUNTIME_OBJECT_TYPE_STRING;
			v->dt.s=*(g_dt->c_dt->st.dt+((lll_string_object_t*)o)->i);
			break;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			{
				lll_identifier_index_t idx=((lll_identifier_object_t*)o)->idx;
				*v=*(g_dt->v+(LLL_IDENTIFIER_GET_ARRAY_ID(idx)==LLL_MAX_SHORT_IDENTIFIER_LENGTH?*(g_dt->im.l_im+LLL_IDENTIFIER_GET_ARRAY_INDEX(idx)):*(g_dt->im.s_im[LLL_IDENTIFIER_GET_ARRAY_ID(idx)]+LLL_IDENTIFIER_GET_ARRAY_INDEX(idx))));
				break;
			}
		case LLL_OBJECT_TYPE_FUNC:
			v->t=LLL_RUNTIME_OBJECT_TYPE_INT;
			v->dt.i=((lll_function_object_t*)o)->id;
		default:
			v->t=RUNTIME_OBJECT_TYPE_UNKNOWN;
	}
}



lll_stack_offset_t _generate_sequential_jump(const lll_object_t* o,assembly_generator_data_t* g_dt,lll_assembly_instruction_type_t t,uint8_t st){
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	ASSERT(l);
	if (l==1){
		return _generate(LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t)),g_dt);
	}
	lll_stack_offset_t off=sizeof(lll_operator_object_t);
	lll_runtime_object_t a;
	_get_as_runtime_object(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt,&a);
	l--;
	while (l){
		lll_stack_offset_t n_off=off+_get_object_size(LLL_GET_OBJECT_ARGUMENT(o,off));
		lll_runtime_object_t b;
		_get_as_runtime_object(LLL_GET_OBJECT_ARGUMENT(o,n_off),g_dt,&b);
		if (a.t==RUNTIME_OBJECT_TYPE_UNKNOWN||b.t==RUNTIME_OBJECT_TYPE_UNKNOWN){
			break;
		}
		_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
		l--;
		off=n_off;
		uint8_t cmp=_compare_runtime_object(&a,&b);
		ASSERT(cmp!=RUNTIME_OBJECT_COMPARE_ERROR);
		switch (t){
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JB:
				if (cmp==RUNTIME_OBJECT_COMPARE_BELOW){
					break;
				}
				_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
				if (st){
					GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
				}
				return _get_object_size(o);
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:
				if (cmp!=RUNTIME_OBJECT_COMPARE_ABOVE){
					break;
				}
				_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
				if (st){
					GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
				}
				return _get_object_size(o);
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JA:
				if (cmp==RUNTIME_OBJECT_COMPARE_ABOVE){
					break;
				}
				_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
				if (st){
					GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
				}
				return _get_object_size(o);
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:
				if (cmp!=RUNTIME_OBJECT_COMPARE_BELOW){
					break;
				}
				_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
				if (st){
					GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
				}
				return _get_object_size(o);
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JE:
				if (cmp==RUNTIME_OBJECT_COMPARE_EQUAL){
					break;
				}
				_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
				if (st){
					GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
				}
				return _get_object_size(o);
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:
				if (cmp!=RUNTIME_OBJECT_COMPARE_EQUAL){
					break;
				}
				_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
				if (st){
					GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
				}
				return _get_object_size(o);
			default:
				UNREACHABLE();
		}
		a=b;
	}
	if (!l){
		if (st){
			GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE);
		}
		return off+_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
	}
	switch (t){
		case LLL_ASSEMBLY_INSTRUCTION_TYPE_JB:
			t=LLL_ASSEMBLY_INSTRUCTION_TYPE_JAE;
			break;
		case LLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:
			t=LLL_ASSEMBLY_INSTRUCTION_TYPE_JA;
			break;
		case LLL_ASSEMBLY_INSTRUCTION_TYPE_JA:
			t=LLL_ASSEMBLY_INSTRUCTION_TYPE_JBE;
			break;
		case LLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:
			t=LLL_ASSEMBLY_INSTRUCTION_TYPE_JB;
			break;
		case LLL_ASSEMBLY_INSTRUCTION_TYPE_JE:
			t=LLL_ASSEMBLY_INSTRUCTION_TYPE_JNE;
			break;
		case LLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:
			t=LLL_ASSEMBLY_INSTRUCTION_TYPE_JE;
			break;
		default:
			UNREACHABLE();
	}
	off+=_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
	assembly_instruction_label_t e=NEXT_LABEL(g_dt);
	l--;
	while (l){
		l--;
		off+=_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
		GENERATE_OPCODE_WITH_LABEL(g_dt,t,e);
		GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_ROT);
		GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
	}
	off+=_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
	GENERATE_OPCODE_WITH_LABEL(g_dt,t,e);
	GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO);
	if (!st){
		DEFINE_LABEL(g_dt,e);
	}
	else{
		GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE);
		assembly_instruction_label_t tmp=NEXT_LABEL(g_dt);
		GENERATE_OPCODE_WITH_LABEL(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,tmp);
		DEFINE_LABEL(g_dt,e);
		GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
		DEFINE_LABEL(g_dt,tmp);
	}
	return off;
}



lll_stack_offset_t _generate_cond_jump(const lll_object_t* o,assembly_generator_data_t* g_dt,assembly_instruction_label_t lbl,lll_assembly_instruction_type_t t,uint8_t inv){
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	ASSERT(l);
	if (l==1){
		return _generate(LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t)),g_dt);
	}
	lll_stack_offset_t off=sizeof(lll_operator_object_t);
	lll_runtime_object_t a;
	_get_as_runtime_object(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt,&a);
	l--;
	while (l){
		lll_stack_offset_t n_off=off+_get_object_size(LLL_GET_OBJECT_ARGUMENT(o,off));
		lll_runtime_object_t b;
		_get_as_runtime_object(LLL_GET_OBJECT_ARGUMENT(o,n_off),g_dt,&b);
		if (a.t==RUNTIME_OBJECT_TYPE_UNKNOWN||b.t==RUNTIME_OBJECT_TYPE_UNKNOWN){
			break;
		}
		_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
		l--;
		off=n_off;
		uint8_t cmp=_compare_runtime_object(&a,&b);
		ASSERT(cmp!=RUNTIME_OBJECT_COMPARE_ERROR);
		switch (t){
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JB:
				if (cmp==RUNTIME_OBJECT_COMPARE_BELOW){
					break;
				}
				if (inv){
					GENERATE_OPCODE_WITH_LABEL(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
				}
				_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
				return _get_object_size(o);
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:
				if (cmp!=RUNTIME_OBJECT_COMPARE_ABOVE){
					break;
				}
				if (inv){
					GENERATE_OPCODE_WITH_LABEL(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
				}
				_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
				return _get_object_size(o);
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JA:
				if (cmp==RUNTIME_OBJECT_COMPARE_ABOVE){
					break;
				}
				if (inv){
					GENERATE_OPCODE_WITH_LABEL(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
				}
				_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
				return _get_object_size(o);
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:
				if (cmp!=RUNTIME_OBJECT_COMPARE_BELOW){
					break;
				}
				if (inv){
					GENERATE_OPCODE_WITH_LABEL(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
				}
				_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
				return _get_object_size(o);
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JE:
				if (cmp==RUNTIME_OBJECT_COMPARE_EQUAL){
					break;
				}
				if (inv){
					GENERATE_OPCODE_WITH_LABEL(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
				}
				_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
				return _get_object_size(o);
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:
				if (cmp!=RUNTIME_OBJECT_COMPARE_EQUAL){
					break;
				}
				if (inv){
					GENERATE_OPCODE_WITH_LABEL(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
				}
				_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
				return _get_object_size(o);
			default:
				UNREACHABLE();
		}
		a=b;
	}
	if (!l){
		if (!inv){
			GENERATE_OPCODE_WITH_LABEL(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
		}
		return off+_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
	}
	if (inv){
		switch (t){
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JB:
				t=LLL_ASSEMBLY_INSTRUCTION_TYPE_JAE;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:
				t=LLL_ASSEMBLY_INSTRUCTION_TYPE_JA;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JA:
				t=LLL_ASSEMBLY_INSTRUCTION_TYPE_JBE;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:
				t=LLL_ASSEMBLY_INSTRUCTION_TYPE_JB;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JE:
				t=LLL_ASSEMBLY_INSTRUCTION_TYPE_JNE;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:
				t=LLL_ASSEMBLY_INSTRUCTION_TYPE_JE;
				break;
			default:
				UNREACHABLE();
		}
	}
	off+=_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
	l--;
	while (l){
		l--;
		off+=_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
		GENERATE_OPCODE_WITH_LABEL(g_dt,t,lbl);
		GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_ROT);
		GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
	}
	off+=_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
	GENERATE_OPCODE_WITH_LABEL(g_dt,t,lbl);
	GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO);
	return off;
}



lll_stack_offset_t _generate_jump(const lll_object_t* o,assembly_generator_data_t* g_dt,assembly_instruction_label_t lbl,uint8_t inv){
	lll_stack_offset_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_NIL:
			if (inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return sizeof(lll_integer_object_t)+eoff;
		case LLL_OBJECT_TYPE_CHAR:
			if ((!!(((lll_char_object_t*)o)->v))^inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return sizeof(lll_char_object_t)+eoff;
		case LLL_OBJECT_TYPE_INT:
			if ((!!(((lll_integer_object_t*)o)->v))^inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return sizeof(lll_integer_object_t)+eoff;
		case LLL_OBJECT_TYPE_FLOAT:
			if ((!!(((lll_float_object_t*)o)->v))^inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return sizeof(lll_float_object_t)+eoff;
		case LLL_OBJECT_TYPE_STRING:
			if ((!!((*(g_dt->c_dt->st.dt+((lll_string_object_t*)o)->i))->l))^inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return sizeof(lll_string_object_t)+eoff;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			ASSERT(!"Unimplemented");
		case LLL_OBJECT_TYPE_PRINT:
			{
				lll_stack_offset_t off=_generate(o,g_dt);
				if (!inv){
					GENERATE_OPCODE_WITH_LABEL(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_INPUT:
			ASSERT(!"Unimplemented");
		case LLL_OBJECT_TYPE_AND:
			ASSERT(!"Unimplemented");
		case LLL_OBJECT_TYPE_OR:
			ASSERT(!"Unimplemented");
		case LLL_OBJECT_TYPE_NOT:
			return _generate_jump(o,g_dt,lbl,!inv)+eoff;
		case LLL_OBJECT_TYPE_FUNC:
			{
				if (!inv){
					GENERATE_OPCODE_WITH_LABEL(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
				}
				return _get_object_size(o)+eoff;
			}
		case LLL_OBJECT_TYPE_IF:
		case LLL_OBJECT_TYPE_FOR:
		case LLL_OBJECT_TYPE_RETURN:
		case LLL_OBJECT_TYPE_EXIT:
		case LLL_OBJECT_TYPE_IMPORT:
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_stack_offset_t off=_generate(o,g_dt);
				if (inv){
					GENERATE_OPCODE_WITH_LABEL(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_LESS:
			return _generate_cond_jump(o,g_dt,lbl,LLL_ASSEMBLY_INSTRUCTION_TYPE_JB,inv)+eoff;
		case LLL_OBJECT_TYPE_LESS_EQUAL:
			return _generate_cond_jump(o,g_dt,lbl,LLL_ASSEMBLY_INSTRUCTION_TYPE_JBE,inv)+eoff;
		case LLL_OBJECT_TYPE_EQUAL:
			return _generate_cond_jump(o,g_dt,lbl,LLL_ASSEMBLY_INSTRUCTION_TYPE_JE,inv)+eoff;
		case LLL_OBJECT_TYPE_NOT_EQUAL:
			return _generate_cond_jump(o,g_dt,lbl,LLL_ASSEMBLY_INSTRUCTION_TYPE_JNE,inv)+eoff;
		case LLL_OBJECT_TYPE_MORE:
			return _generate_cond_jump(o,g_dt,lbl,LLL_ASSEMBLY_INSTRUCTION_TYPE_JA,inv)+eoff;
		case LLL_OBJECT_TYPE_MORE_EQUAL:
			return _generate_cond_jump(o,g_dt,lbl,LLL_ASSEMBLY_INSTRUCTION_TYPE_JAE,inv)+eoff;
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			return sizeof(lll_debug_object_t)+eoff+_generate_jump(LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)o),g_dt,lbl,inv);
	}
	lll_stack_offset_t off=_generate_on_stack(o,g_dt);
	GENERATE_OPCODE_WITH_LABEL(g_dt,(inv?LLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:LLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ),lbl);
	GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
	return off+eoff;
}



lll_stack_offset_t _invalidate_loop_variables(const lll_object_t* o,assembly_generator_data_t* g_dt){
	lll_stack_offset_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_NIL:
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_CHAR:
			return sizeof(lll_char_object_t)+eoff;
		case LLL_OBJECT_TYPE_STRING:
			return sizeof(lll_string_object_t)+eoff;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			return sizeof(lll_identifier_object_t)+eoff;
		case LLL_OBJECT_TYPE_INT:
			return sizeof(lll_integer_object_t)+eoff;
		case LLL_OBJECT_TYPE_FLOAT:
			return sizeof(lll_float_object_t)+eoff;
		case LLL_OBJECT_TYPE_ASSIGN:
			{
				lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
				ASSERT(l>=2);
				lll_identifier_object_t* io=(lll_identifier_object_t*)LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t));
				ASSERT(LLL_GET_OBJECT_TYPE(io)==LLL_OBJECT_TYPE_IDENTIFIER);
				lll_stack_offset_t off=sizeof(lll_operator_object_t)+sizeof(lll_identifier_object_t);
				lll_runtime_object_t tmp;
				_get_as_runtime_object(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt,&tmp);
				if (tmp.t==RUNTIME_OBJECT_TYPE_UNKNOWN){
					(g_dt->v+(LLL_IDENTIFIER_GET_ARRAY_ID(io->idx)==LLL_MAX_SHORT_IDENTIFIER_LENGTH?*(g_dt->im.l_im+LLL_IDENTIFIER_GET_ARRAY_INDEX(io->idx)):*(g_dt->im.s_im[LLL_IDENTIFIER_GET_ARRAY_ID(io->idx)]+LLL_IDENTIFIER_GET_ARRAY_INDEX(io->idx))))->t=RUNTIME_OBJECT_TYPE_UNKNOWN;
				}
				l--;
				while (l){
					l--;
					off+=_invalidate_loop_variables(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_FUNC:
			{
				lll_stack_offset_t off=sizeof(lll_function_object_t);
				lll_arg_count_t l=((lll_function_object_t*)o)->ac;
				while (l){
					l--;
					off+=_invalidate_loop_variables(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_IMPORT:
			return sizeof(lll_import_object_t)+sizeof(lll_import_index_t)*((lll_import_object_t*)o)->ac+eoff;
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_stack_offset_t off=sizeof(lll_operation_list_object_t);
				lll_statement_count_t l=((lll_operation_list_object_t*)o)->sc;
				while (l){
					l--;
					off+=_invalidate_loop_variables(LLL_GET_OBJECT_STATEMENT(o,off),g_dt);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			return sizeof(lll_debug_object_t)+eoff+_invalidate_loop_variables(LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)o),g_dt);
	}
	lll_stack_offset_t off=sizeof(lll_operator_object_t);
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	while (l){
		l--;
		off+=_invalidate_loop_variables(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
	}
	return off+eoff;
}



lll_stack_offset_t _generate_for_loop(const lll_object_t* o,assembly_generator_data_t* g_dt){
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	ASSERT(l);
	if (l==1){
		return sizeof(lll_operator_object_t)+_generate(LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t)),g_dt);
	}
	lll_stack_offset_t off=sizeof(lll_operator_object_t)+_generate(LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t)),g_dt);
	lll_object_t* cnd=LLL_GET_OBJECT_ARGUMENT(o,off);
	assembly_instruction_label_t s=NEXT_LABEL(g_dt);
	assembly_instruction_label_t e=NEXT_LABEL(g_dt);
	off+=_generate_jump(cnd,g_dt,e,1);
	if (LLL_ASSEMBLY_INSTRUCTION_GET_TYPE(g_dt->a_dt->h+g_dt->a_dt->ic-1)!=LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP){
		l-=2;
		lll_stack_offset_t off2=off;
		for (lll_arg_count_t i=0;i<l;i++){
			off2+=_invalidate_loop_variables(LLL_GET_OBJECT_ARGUMENT(o,off2),g_dt);
		}
		DEFINE_LABEL(g_dt,s);
		while (l){
			l--;
			off+=_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
			_generate_jump(cnd,g_dt,s,0);
		}
		DEFINE_LABEL(g_dt,e);
	}
	else{
		(g_dt->a_dt->h+g_dt->a_dt->ic-1)->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
		l-=2;
		while (l){
			l--;
			off+=_get_object_size(LLL_GET_OBJECT_ARGUMENT(o,off));
		}
	}
	return off;
}



lll_stack_offset_t _generate_if(const lll_object_t* o,assembly_generator_data_t* g_dt){
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	ASSERT(l);
	if (l==1){
		return sizeof(lll_operator_object_t)+_generate(LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t)),g_dt);
	}
	lll_stack_offset_t off=sizeof(lll_operator_object_t);
	if (l&1){
		assembly_instruction_label_t e=NEXT_LABEL(g_dt);
		l>>=1;
		while (l){
			l--;
			assembly_instruction_label_t nxt=NEXT_LABEL(g_dt);
			off+=_generate_jump(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt,nxt,1);
			if (LLL_ASSEMBLY_INSTRUCTION_GET_TYPE(g_dt->a_dt->h+g_dt->a_dt->ic-1)!=LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP){
				off+=_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
				GENERATE_OPCODE_WITH_LABEL(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,e);
				DEFINE_LABEL(g_dt,nxt);
			}
			else{
				(g_dt->a_dt->h+g_dt->a_dt->ic-1)->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				off+=_get_object_size(LLL_GET_OBJECT_ARGUMENT(o,off));
			}
		}
		off+=_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
		DEFINE_LABEL(g_dt,e);
	}
	else{
		assembly_instruction_label_t e=NEXT_LABEL(g_dt);
		l>>=1;
		while (l){
			l--;
			assembly_instruction_label_t nxt=NEXT_LABEL(g_dt);
			off+=_generate_jump(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt,nxt,1);
			if (LLL_ASSEMBLY_INSTRUCTION_GET_TYPE(g_dt->a_dt->h+g_dt->a_dt->ic-1)!=LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP){
				off+=_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
				if (l){
					GENERATE_OPCODE_WITH_LABEL(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,e);
				}
				DEFINE_LABEL(g_dt,nxt);
			}
			else{
				(g_dt->a_dt->h+g_dt->a_dt->ic-1)->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				off+=_get_object_size(LLL_GET_OBJECT_ARGUMENT(o,off));
			}
		}
		DEFINE_LABEL(g_dt,e);
	}
	return off;
}



lll_stack_offset_t _generate_call(const lll_object_t* o,assembly_generator_data_t* g_dt){
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	ASSERT(l);
	lll_stack_offset_t off=sizeof(lll_operator_object_t)+_get_object_size(LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t)));
	l--;
	assembly_instruction_label_t e=NEXT_LABEL(g_dt);
	GENERATE_OPCODE_WITH_LABEL(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_II,e);
	while (l){
		l--;
		off+=_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
	}
	_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t)),g_dt);
	lll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
	ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_CALL;
	ai->dt.ac=((lll_operator_object_t*)o)->ac-1;
	DEFINE_LABEL(g_dt,e);
	return off;
}



lll_stack_offset_t _generate_chain(const lll_object_t* o,lll_assembly_instruction_type_t t,assembly_generator_data_t* g_dt){
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	ASSERT(l);
	if (l==1){
		return _generate(LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t)),g_dt);
	}
	lll_stack_offset_t off=sizeof(lll_operator_object_t)+_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t)),g_dt);
	l--;
	while (l){
		l--;
		off+=_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
		GENERATE_OPCODE(g_dt,t);
	}
	return off;
}



lll_stack_offset_t _generate_on_stack(const lll_object_t* o,assembly_generator_data_t* g_dt){
	lll_stack_offset_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	if (g_dt->vi!=LLL_MAX_VARIABLE_INDEX){
		_get_as_runtime_object(o,g_dt,g_dt->v+g_dt->vi);
		g_dt->vi=LLL_MAX_VARIABLE_INDEX;
	}
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_NIL:
			{
				lll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT;
				ai->dt.i=0;
				return sizeof(lll_float_object_t)+eoff;
			}
		case LLL_OBJECT_TYPE_CHAR:
			{
				lll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR;
				ai->dt.c=((lll_char_object_t*)o)->v;
				return sizeof(lll_char_object_t)+eoff;
			}
		case LLL_OBJECT_TYPE_INT:
			{
				lll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT;
				ai->dt.i=((lll_integer_object_t*)o)->v;
				return sizeof(lll_integer_object_t)+eoff;
			}
		case LLL_OBJECT_TYPE_FLOAT:
			{
				lll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT;
				ai->dt.f=((lll_float_object_t*)o)->v;
				return sizeof(lll_float_object_t)+eoff;
			}
		case LLL_OBJECT_TYPE_STRING:
			{
				lll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS;
				ai->dt.s=((lll_string_object_t*)o)->i;
				return sizeof(lll_string_object_t)+eoff;
			}
		case LLL_OBJECT_TYPE_IDENTIFIER:
			{
				lll_identifier_index_t idx=((lll_identifier_object_t*)o)->idx;
				lll_variable_index_t vi=(LLL_IDENTIFIER_GET_ARRAY_ID(idx)==LLL_MAX_SHORT_IDENTIFIER_LENGTH?*(g_dt->im.l_im+LLL_IDENTIFIER_GET_ARRAY_INDEX(idx)):*(g_dt->im.s_im[LLL_IDENTIFIER_GET_ARRAY_ID(idx)]+LLL_IDENTIFIER_GET_ARRAY_INDEX(idx)));
				lll_runtime_object_t* v=g_dt->v+vi;
				lll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				switch (v->t){
					case LLL_RUNTIME_OBJECT_TYPE_INT:
						ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT;
						ai->dt.i=v->dt.i;
						break;
					case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
						ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT;
						ai->dt.f=v->dt.f;
						break;
					case LLL_RUNTIME_OBJECT_TYPE_CHAR:
						ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR;
						ai->dt.c=v->dt.c;
						break;
					case LLL_RUNTIME_OBJECT_TYPE_STRING:
						ASSERT(!"Unimplemented");
					case RUNTIME_OBJECT_TYPE_UNKNOWN:
						ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD;
						ai->dt.v=vi;
						break;
					default:
						UNREACHABLE();
				}
				return sizeof(lll_identifier_object_t)+eoff;
			}
		case LLL_OBJECT_TYPE_PRINT:
		case LLL_OBJECT_TYPE_IF:
		case LLL_OBJECT_TYPE_FOR:
		case LLL_OBJECT_TYPE_RETURN:
		case LLL_OBJECT_TYPE_EXIT:
		case LLL_OBJECT_TYPE_IMPORT:
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_stack_offset_t off=_generate(o,g_dt);
				GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_INPUT:
			ASSERT(!"Unimplemented");
		case LLL_OBJECT_TYPE_AND:
			ASSERT(!"Unimplemented");
		case LLL_OBJECT_TYPE_OR:
			ASSERT(!"Unimplemented");
		case LLL_OBJECT_TYPE_NOT:
			{
				lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
				ASSERT(l);
				lll_stack_offset_t off=sizeof(lll_operator_object_t);
				l--;
				while (l){
					off+=_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
				}
				off+=_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
				GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_NOT);
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_ASSIGN:
			{
				lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
				ASSERT(l>=2);
				lll_identifier_object_t* io=(lll_identifier_object_t*)LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t));
				ASSERT(LLL_GET_OBJECT_TYPE(io)==LLL_OBJECT_TYPE_IDENTIFIER);
				lll_stack_offset_t off=sizeof(lll_operator_object_t)+sizeof(lll_identifier_object_t)+_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t)+sizeof(lll_identifier_object_t)),g_dt);
				lll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE;
				ai->dt.v=(LLL_IDENTIFIER_GET_ARRAY_ID(io->idx)==LLL_MAX_SHORT_IDENTIFIER_LENGTH?*(g_dt->im.l_im+LLL_IDENTIFIER_GET_ARRAY_INDEX(io->idx)):*(g_dt->im.s_im[LLL_IDENTIFIER_GET_ARRAY_ID(io->idx)]+LLL_IDENTIFIER_GET_ARRAY_INDEX(io->idx)));
				l-=2;
				while (l){
					l--;
					off+=_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_FUNC:
			{
				lll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT;
				ai->dt.i=((lll_function_object_t*)o)->id;
				return _get_object_size(o)+eoff;
			}
		case LLL_OBJECT_TYPE_CALL:
			return _generate_call(o,g_dt)+eoff;
		case LLL_OBJECT_TYPE_ADD:
			return _generate_chain(o,LLL_ASSEMBLY_INSTRUCTION_TYPE_ADD,g_dt)+eoff;
		case LLL_OBJECT_TYPE_SUB:
			return _generate_chain(o,LLL_ASSEMBLY_INSTRUCTION_TYPE_SUB,g_dt)+eoff;
		case LLL_OBJECT_TYPE_MULT:
			return _generate_chain(o,LLL_ASSEMBLY_INSTRUCTION_TYPE_MULT,g_dt)+eoff;
		case LLL_OBJECT_TYPE_DIV:
			return _generate_chain(o,LLL_ASSEMBLY_INSTRUCTION_TYPE_DIV,g_dt)+eoff;
		case LLL_OBJECT_TYPE_FLOOR_DIV:
			return _generate_chain(o,LLL_ASSEMBLY_INSTRUCTION_TYPE_FDIV,g_dt)+eoff;
		case LLL_OBJECT_TYPE_MOD:
			return _generate_chain(o,LLL_ASSEMBLY_INSTRUCTION_TYPE_MOD,g_dt)+eoff;
		case LLL_OBJECT_TYPE_BIT_AND:
			return _generate_chain(o,LLL_ASSEMBLY_INSTRUCTION_TYPE_AND,g_dt)+eoff;
		case LLL_OBJECT_TYPE_BIT_OR:
			return _generate_chain(o,LLL_ASSEMBLY_INSTRUCTION_TYPE_OR,g_dt)+eoff;
		case LLL_OBJECT_TYPE_BIT_XOR:
			return _generate_chain(o,LLL_ASSEMBLY_INSTRUCTION_TYPE_XOR,g_dt)+eoff;
		case LLL_OBJECT_TYPE_BIT_NOT:
			return _generate_chain(o,LLL_ASSEMBLY_INSTRUCTION_TYPE_INV,g_dt)+eoff;
		case LLL_OBJECT_TYPE_LESS:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JB,1)+eoff;
		case LLL_OBJECT_TYPE_LESS_EQUAL:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JBE,1)+eoff;
		case LLL_OBJECT_TYPE_EQUAL:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JE,1)+eoff;
		case LLL_OBJECT_TYPE_NOT_EQUAL:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JNE,1)+eoff;
		case LLL_OBJECT_TYPE_MORE:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JA,1)+eoff;
		case LLL_OBJECT_TYPE_MORE_EQUAL:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JAE,1)+eoff;
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			return sizeof(lll_debug_object_t)+eoff+_generate_on_stack(LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)o),g_dt);
		default:
			UNREACHABLE();
	}
}



lll_stack_offset_t _generate(const lll_object_t* o,assembly_generator_data_t* g_dt){
	lll_stack_offset_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_NIL:
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_CHAR:
			return sizeof(lll_char_object_t)+eoff;
		case LLL_OBJECT_TYPE_INT:
			return sizeof(lll_integer_object_t)+eoff;
		case LLL_OBJECT_TYPE_FLOAT:
			return sizeof(lll_float_object_t)+eoff;
		case LLL_OBJECT_TYPE_STRING:
			return sizeof(lll_string_object_t)+eoff;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			return sizeof(lll_identifier_object_t)+eoff;
		case LLL_OBJECT_TYPE_PRINT:
			{
				lll_stack_offset_t off=sizeof(lll_operator_object_t);
				lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
				while (l){
					l--;
					off+=_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
					GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_INPUT:
			ASSERT(!"Unimplemented");
		case LLL_OBJECT_TYPE_AND:
			ASSERT(!"Unimplemented");
		case LLL_OBJECT_TYPE_OR:
			ASSERT(!"Unimplemented");
		case LLL_OBJECT_TYPE_ASSIGN:
			{
				lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
				ASSERT(l>=2);
				lll_identifier_object_t* io=(lll_identifier_object_t*)LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t));
				ASSERT(LLL_GET_OBJECT_TYPE(io)==LLL_OBJECT_TYPE_IDENTIFIER);
				lll_variable_index_t v=(LLL_IDENTIFIER_GET_ARRAY_ID(io->idx)==LLL_MAX_SHORT_IDENTIFIER_LENGTH?*(g_dt->im.l_im+LLL_IDENTIFIER_GET_ARRAY_INDEX(io->idx)):*(g_dt->im.s_im[LLL_IDENTIFIER_GET_ARRAY_ID(io->idx)]+LLL_IDENTIFIER_GET_ARRAY_INDEX(io->idx)));
				g_dt->vi=v;
				lll_stack_offset_t off=sizeof(lll_operator_object_t)+sizeof(lll_identifier_object_t)+_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t)+sizeof(lll_identifier_object_t)),g_dt);
				lll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP;
				ai->dt.v=v;
				l-=2;
				while (l){
					l--;
					off+=_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_FUNC:
			return _get_object_size(o)+eoff;
		case LLL_OBJECT_TYPE_CALL:
			{
				lll_stack_offset_t off=_generate_call(o,g_dt);
				GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_IF:
			return _generate_if(o,g_dt)+eoff;
		case LLL_OBJECT_TYPE_FOR:
			return _generate_for_loop(o,g_dt)+eoff;
		case LLL_OBJECT_TYPE_LESS:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JB,0)+eoff;
		case LLL_OBJECT_TYPE_LESS_EQUAL:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JBE,0)+eoff;
		case LLL_OBJECT_TYPE_EQUAL:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JE,0)+eoff;
		case LLL_OBJECT_TYPE_NOT_EQUAL:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JNE,0)+eoff;
		case LLL_OBJECT_TYPE_MORE:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JA,0)+eoff;
		case LLL_OBJECT_TYPE_MORE_EQUAL:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JAE,0)+eoff;
		case LLL_OBJECT_TYPE_IMPORT:
			return sizeof(lll_import_object_t)+sizeof(lll_import_index_t)*((lll_import_object_t*)o)->ac+eoff;
		case LLL_OBJECT_TYPE_RETURN:
		case LLL_OBJECT_TYPE_EXIT:
			{
				lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
				if (!l){
					GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					GENERATE_OPCODE(g_dt,(LLL_GET_OBJECT_TYPE(o)==LLL_OBJECT_TYPE_RETURN?LLL_ASSEMBLY_INSTRUCTION_TYPE_RET:LLL_ASSEMBLY_INSTRUCTION_TYPE_END));
					return sizeof(lll_operator_object_t);
				}
				lll_stack_offset_t off=sizeof(lll_operator_object_t)+_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t)),g_dt);
				l--;
				while (l){
					l--;
					off+=_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
				}
				GENERATE_OPCODE(g_dt,(LLL_GET_OBJECT_TYPE(o)==LLL_OBJECT_TYPE_RETURN?LLL_ASSEMBLY_INSTRUCTION_TYPE_RET:LLL_ASSEMBLY_INSTRUCTION_TYPE_END));
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_stack_offset_t off=sizeof(lll_operation_list_object_t);
				lll_statement_count_t l=((lll_operation_list_object_t*)o)->sc;
				while (l){
					l--;
					off+=_generate(LLL_GET_OBJECT_STATEMENT(o,off),g_dt);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			return sizeof(lll_debug_object_t)+eoff+_generate(LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)o),g_dt);
	}
	lll_stack_offset_t off=sizeof(lll_operator_object_t);
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	while (l){
		l--;
		off+=_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
	}
	return off+eoff;
}



lll_stack_offset_t _map_identifiers(const lll_object_t* o,assembly_generator_data_t* g_dt){
	lll_stack_offset_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_NIL:
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_CHAR:
			return sizeof(lll_char_object_t)+eoff;
		case LLL_OBJECT_TYPE_STRING:
			return sizeof(lll_string_object_t)+eoff;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			{
				lll_identifier_index_t i=((lll_identifier_object_t*)o)->idx;
				lll_identifier_index_t j=LLL_IDENTIFIER_GET_ARRAY_INDEX(i);
				uint8_t k=LLL_IDENTIFIER_GET_ARRAY_ID(i);
				if (k==LLL_MAX_SHORT_IDENTIFIER_LENGTH){
					lll_identifier_t* id=g_dt->c_dt->i_dt.il+j;
					if (g_dt->im.l_sc<id->sc){
						*(g_dt->im.sc_vi+g_dt->im.l_sc)=g_dt->im.n_vi;
						g_dt->im.l_sc=id->sc;
						if (id->sc<=g_dt->im.sc_vi_l){
							g_dt->im.n_vi=*(g_dt->im.sc_vi+g_dt->im.l_sc);
						}
						else{
							g_dt->im.sc_vi_l=id->sc;
						}
					}
					else if (g_dt->im.l_sc>id->sc){
						*(g_dt->im.sc_vi+g_dt->im.l_sc)=g_dt->im.n_vi;
						g_dt->im.l_sc=id->sc;
						g_dt->im.n_vi=*(g_dt->im.sc_vi+g_dt->im.l_sc);
					}
					if (*(g_dt->im.l_im+j)==LLL_MAX_VARIABLE_INDEX){
						*(g_dt->im.l_im+j)=g_dt->im.n_vi;
						g_dt->im.n_vi++;
						if (g_dt->im.n_vi>g_dt->a_dt->vc){
							g_dt->a_dt->vc=g_dt->im.n_vi;
						}
					}
				}
				else{
					lll_identifier_t* id=g_dt->c_dt->i_dt.s[k].dt+j;
					if (g_dt->im.l_sc<id->sc){
						*(g_dt->im.sc_vi+g_dt->im.l_sc)=g_dt->im.n_vi;
						g_dt->im.l_sc=id->sc;
						if (id->sc<=g_dt->im.sc_vi_l){
							g_dt->im.n_vi=*(g_dt->im.sc_vi+g_dt->im.l_sc);
						}
						else{
							g_dt->im.sc_vi_l=id->sc;
						}
					}
					else if (g_dt->im.l_sc>id->sc){
						*(g_dt->im.sc_vi+g_dt->im.l_sc)=g_dt->im.n_vi;
						g_dt->im.l_sc=id->sc;
						g_dt->im.n_vi=*(g_dt->im.sc_vi+g_dt->im.l_sc);
					}
					if (*(g_dt->im.s_im[k]+j)==LLL_MAX_VARIABLE_INDEX){
						*(g_dt->im.s_im[k]+j)=g_dt->im.n_vi;
						g_dt->im.n_vi++;
						if (g_dt->im.n_vi>g_dt->a_dt->vc){
							g_dt->a_dt->vc=g_dt->im.n_vi;
						}
					}
				}
				return sizeof(lll_identifier_object_t)+eoff;
			}
		case LLL_OBJECT_TYPE_INT:
			return sizeof(lll_integer_object_t)+eoff;
		case LLL_OBJECT_TYPE_FLOAT:
			return sizeof(lll_float_object_t)+eoff;
		case LLL_OBJECT_TYPE_FUNC:
			return _get_object_size(o)+eoff;
		case LLL_OBJECT_TYPE_IMPORT:
			return sizeof(lll_import_object_t)+sizeof(lll_import_index_t)*((lll_import_object_t*)o)->ac+eoff;
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_stack_offset_t off=sizeof(lll_operation_list_object_t);
				lll_statement_count_t l=((lll_operation_list_object_t*)o)->sc;
				while (l){
					l--;
					off+=_map_identifiers(LLL_GET_OBJECT_STATEMENT(o,off),g_dt);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			return sizeof(lll_debug_object_t)+eoff+_map_identifiers(LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)o),g_dt);
	}
	lll_stack_offset_t off=sizeof(lll_operator_object_t);
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	while (l){
		l--;
		off+=_map_identifiers(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
	}
	return off+eoff;
}



__LLL_IMPORT_EXPORT __LLL_RETURN lll_generate_assembly(const lll_compilation_data_t* c_dt,lll_assembly_data_t* o,lll_error_t* e){
	if (!o->_s.ptr||!c_dt->_s.ptr){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	o->tm=c_dt->tm;
	o->h=(lll_assembly_instruction_t*)(o->_s.ptr+o->_s.off);
	o->ic=0;
	o->vc=0;
	o->st.l=c_dt->st.l;
	o->st.dt=malloc(o->st.l*sizeof(lll_string_t*));
	for (lll_string_index_t i=0;i<o->st.l;i++){
		lll_string_t* s=*(c_dt->st.dt+i);
		lll_string_t* d=malloc(sizeof(lll_string_t)+(s->l+1)*sizeof(lll_char_t));
		d->l=s->l;
		d->c=s->c;
		for (lll_string_length_t l=0;l<s->l;l++){
			d->v[l]=s->v[l];
		}
		d->v[d->l]=0;
		*(o->st.dt+i)=d;
	}
	assembly_generator_data_t g_dt={
		o,
		c_dt,
		{
			.l_im=malloc(c_dt->i_dt.ill*sizeof(lll_variable_index_t)),
			.n_vi=0,
			.l_sc=0,
			.sc_vi=malloc(c_dt->_n_sc_id*sizeof(lll_variable_index_t)),
			.sc_vi_l=0
		},
		0,
		.vi=LLL_MAX_VARIABLE_INDEX
	};
	for (uint8_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		g_dt.im.s_im[i]=malloc(c_dt->i_dt.s[i].l*sizeof(lll_variable_index_t));
		for (lll_identifier_list_length_t j=0;j<c_dt->i_dt.s[i].l;j++){
			*(g_dt.im.s_im[i]+j)=LLL_MAX_VARIABLE_INDEX;
		}
	}
	for (lll_identifier_list_length_t i=0;i<c_dt->i_dt.ill;i++){
		*(g_dt.im.l_im+i)=LLL_MAX_VARIABLE_INDEX;
	}
	_map_identifiers(c_dt->h,&g_dt);
	for (lll_function_index_t i=0;i<c_dt->f_dt.l;i++){
		g_dt.im.n_vi=o->vc;
		g_dt.im.l_sc=0;
		g_dt.im.sc_vi_l=0;
		const lll_function_object_t* fo=(const lll_function_object_t*)(c_dt->_s.ptr+(*(c_dt->f_dt.dt+i))->off);
		ASSERT(LLL_GET_OBJECT_TYPE(fo)==LLL_OBJECT_TYPE_FUNC);
		lll_stack_offset_t off=sizeof(lll_function_object_t);
		for (lll_arg_count_t j=0;j<fo->ac;j++){
			off+=_map_identifiers(LLL_GET_OBJECT_ARGUMENT(fo,off),&g_dt);
		}
	}
	free(g_dt.im.sc_vi);
	g_dt.v=malloc(o->vc*sizeof(lll_runtime_object_t));
	for (lll_variable_index_t i=0;i<o->vc;i++){
		(g_dt.v+i)->t=RUNTIME_OBJECT_TYPE_UNKNOWN;
	}
	_generate(c_dt->h,&g_dt);
	if ((o->h+o->ic-1)->t!=LLL_ASSEMBLY_INSTRUCTION_TYPE_END){
		GENERATE_OPCODE(&g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
		GENERATE_OPCODE(&g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_END);
	}
	o->ft.l=c_dt->f_dt.l;
	o->ft.dt=malloc(c_dt->f_dt.l*sizeof(lll_instruction_index_t));
	for (lll_function_index_t i=0;i<c_dt->f_dt.l;i++){
		const lll_function_t* k=*(c_dt->f_dt.dt+i);
		lll_arg_count_t j=k->al;
		lll_assembly_instruction_t* ai=NEXT_INSTRUCTION(&g_dt);
		ai->t=ASSEMBLY_INSTRUCTION_TYPE_FUNC_START;
		ASSEMBLY_INSTRUCTION_MISC_FIELD(ai)=i;
		while (j){
			j--;
			lll_assembly_instruction_t* ai=NEXT_INSTRUCTION(&g_dt);
			ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP;
			ai->dt.v=(LLL_IDENTIFIER_GET_ARRAY_ID(k->a[j])==LLL_MAX_SHORT_IDENTIFIER_LENGTH?*(g_dt.im.l_im+LLL_IDENTIFIER_GET_ARRAY_INDEX(k->a[j])):*(g_dt.im.s_im[LLL_IDENTIFIER_GET_ARRAY_ID(k->a[j])]+LLL_IDENTIFIER_GET_ARRAY_INDEX(k->a[j])));
		}
		const lll_function_object_t* fo=(const lll_function_object_t*)(c_dt->_s.ptr+k->off);
		ASSERT(LLL_GET_OBJECT_TYPE(fo)==LLL_OBJECT_TYPE_FUNC);
		lll_stack_offset_t off=sizeof(lll_function_object_t);
		for (lll_arg_count_t j=0;j<fo->ac;j++){
			off+=_generate(LLL_GET_OBJECT_ARGUMENT(fo,off),&g_dt);
		}
		if ((o->h+o->ic-1)->t!=LLL_ASSEMBLY_INSTRUCTION_TYPE_RET){
			GENERATE_OPCODE(&g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO);
		}
	}
	for (uint8_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		free(g_dt.im.s_im[i]);
	}
	free(g_dt.im.l_im);
	free(g_dt.v);
	lll_assembly_instruction_t* ai=o->h;
	for (lll_instruction_index_t i=0;i<o->ic;i++){
		if ((ai+1)->t!=LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT){
			if ((ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT&&!ai->dt.i)||(ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT&&!ai->dt.f)){
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO;
			}
			else if ((ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT&&ai->dt.i==1)||(ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT&&ai->dt.f==1)){
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE;
			}
			else if ((ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT&&ai->dt.i==2)||(ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT&&ai->dt.f==2)){
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO;
			}
			else if ((ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT&&ai->dt.i==3)||(ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT&&ai->dt.f==3)){
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE;
			}
			else if ((ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT&&ai->dt.i==4)||(ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT&&ai->dt.f==4)){
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR;
			}
			else if ((ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT&&ai->dt.i==-1)||(ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT&&ai->dt.f==-1)){
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE;
			}
		}
		if (ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_POP&&(ai-1)->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_ROT){
			(ai-1)->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP;
			ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
		}
		else if (ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD&&(ai-1)->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP&&ai->dt.v==(ai-1)->dt.v){
			(ai-1)->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE;
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
		}
		else if (ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT){
			if ((ai-1)->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT){
				int64_t v=(ai-1)->dt.i;
				lll_char_t bf[21];
				uint8_t j=0;
				if (v<0){
					bf[0]='-';
					j=1;
					v=-v;
				}
				lll_char_t bf_r[20];
				uint8_t k=0;
				do{
					bf_r[k]=v%10;
					k++;
					v/=10;
				} while (v);
				while (k){
					k--;
					bf[j]=bf_r[k]+48;
					j++;
				}
				(ai-1)->dt.s=_create_string(&(o->st),bf,j);
				goto _print_str;
			}
			else if ((ai-1)->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT){
				ASSERT(!"Unimplemented");
			}
			else if ((ai-1)->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR){
				(ai-1)->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR;
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			}
			else if ((ai-1)->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS){
_print_str:
				(ai-1)->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR;
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				if (i>2&&(ai-3)->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR){
					(ai-3)->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
					lll_char_t a=(ai-3)->dt.c;
					lll_string_t* b=*(o->st.dt+(ai-1)->dt.s);
					lll_string_checksum_t c=a^b->c;
					for (lll_string_index_t i=0;i<o->st.l;i++){
						lll_string_t* s=*(o->st.dt+i);
						if (s->c==c&&s->l==b->l+1){
							if (s->v[0]!=a){
								continue;
							}
							for (lll_string_length_t j=0;j<b->l;j++){
								if (b->v[j]!=s->v[j+1]){
									goto _check_next_string;
								}
							}
							(ai-1)->dt.s=i;
							goto _next_instruction;
						}
_check_next_string:;
					}
					o->st.l++;
					o->st.dt=realloc(o->st.dt,o->st.l*sizeof(lll_string_t*));
					lll_string_t* s=malloc(sizeof(lll_string_t)+(b->l+2)*sizeof(lll_char_t));
					s->l=b->l+1;
					s->c=c;
					s->v[0]=a;
					for (lll_string_length_t j=0;j<b->l;j++){
						s->v[j+1]=b->v[j];
					}
					s->v[s->l]=0;
					*(o->st.dt+o->st.l-1)=s;
					(ai-1)->dt.s=o->st.l-1;
				}
				else if (i>2&&(ai-3)->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR){
					(ai-3)->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
					lll_string_t* a=*(o->st.dt+(ai-3)->dt.s);
					lll_string_t* b=*(o->st.dt+(ai-1)->dt.s);
					lll_string_checksum_t c=a->c^b->c;
					for (lll_string_index_t i=0;i<o->st.l;i++){
						lll_string_t* s=*(o->st.dt+i);
						if (s->c==c&&s->l==a->l+b->l){
							for (lll_string_length_t j=0;j<a->l;j++){
								if (a->v[j]!=s->v[j]){
									goto _check_next_string2;
								}
							}
							for (lll_string_length_t j=a->l;j<a->l+b->l;j++){
								if (b->v[j-a->l]!=s->v[j]){
									goto _check_next_string2;
								}
							}
							(ai-1)->dt.s=i;
							goto _next_instruction;
						}
_check_next_string2:;
					}
					o->st.l++;
					o->st.dt=realloc(o->st.dt,o->st.l*sizeof(lll_string_t*));
					lll_string_t* s=malloc(sizeof(lll_string_t)+(a->l+b->l+1)*sizeof(lll_char_t));
					s->l=a->l+b->l;
					s->c=c;
					for (lll_string_length_t j=0;j<a->l;j++){
						s->v[j]=a->v[j];
					}
					for (lll_string_length_t j=a->l;j<a->l+b->l;j++){
						s->v[j]=b->v[j-a->l];
					}
					s->v[s->l]=0;
					*(o->st.dt+o->st.l-1)=s;
					(ai-1)->dt.s=o->st.l-1;
				}
			}
			else if ((ai-1)->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD){
				(ai-1)->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR;
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			}
		}
		else if (ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_RET){
			if ((ai-1)->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT){
				(ai-1)->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT;
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			}
			else if ((ai-1)->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT){
				(ai-1)->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT;
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			}
			else if ((ai-1)->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR){
				(ai-1)->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR;
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			}
			else if ((ai-1)->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS){
				(ai-1)->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR;
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			}
			else if ((ai-1)->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD){
				(ai-1)->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR;
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			}
		}
		else if (ai->t>=LLL_ASSEMBLY_INSTRUCTION_TYPE_ADD&&ai->t<=LLL_ASSEMBLY_INSTRUCTION_TYPE_XOR&&(ai+1)->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP){
			if ((ai-2)->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD&&(ai-2)->dt.v==(ai+1)->dt.v){
				*(ai-2)=*(ai-1);
				(ai-1)->dt.v=(ai+1)->dt.v;
				goto _compress_math_op;
			}
			if ((ai-1)->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD&&(ai-1)->dt.v==(ai+1)->dt.v){
_compress_math_op:
				(ai-1)->t=ai->t|LLL_ASSEMBLY_INSTRUCTION_INPLACE;
				if ((ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_ADD||ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_SUB)&&(ai-2)->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE){
					(ai-2)->t=(ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_ADD?LLL_ASSEMBLY_INSTRUCTION_TYPE_INC:LLL_ASSEMBLY_INSTRUCTION_TYPE_DEC);
					(ai-2)->dt.v=(ai-1)->dt.v;
					(ai-1)->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				}
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				(ai+1)->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			}
		}
_next_instruction:
		ai++;
	}
	strint_map_data_t sm={
		(o->st.l>>6)+1,
		malloc(((o->st.l>>6)+1)*sizeof(uint64_t))
	};
	for (lll_string_index_t i=0;i<sm.ml;i++){
		*(sm.m+i)=0;
	}
	ai=o->h;
	lll_instruction_index_t* lbl=malloc(g_dt.n_lbl*sizeof(lll_instruction_index_t));
	lll_instruction_index_t off=0;
	for (lll_instruction_index_t i=0;i<o->ic;i++){
		if (off){
			*ai=*(ai+off);
		}
		if (ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP){
_handle_nop:;
			off++;
			i--;
			o->ic--;
			continue;
		}
		else if (ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS||ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR||ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR){
			*(sm.m+(ai->dt.s>>6))|=1ull<<(ai->dt.s&63);
		}
		else if (ai->t==ASSEMBLY_INSTRUCTION_TYPE_FUNC_START){
			*(o->ft.dt+ASSEMBLY_INSTRUCTION_MISC_FIELD(ai))=i;
			goto _handle_nop;
		}
		else if (ai->t==ASSEMBLY_INSTRUCTION_TYPE_LABEL_TARGET){
			*(lbl+ASSEMBLY_INSTRUCTION_MISC_FIELD(ai))=i;
			goto _handle_nop;
		}
		ai++;
	}
	sm.im=malloc(o->st.l*sizeof(lll_string_t*));
	lll_string_index_t k=0;
	lll_string_index_t l=0;
	for (lll_string_index_t i=0;i<sm.ml;i++){
		uint64_t v=~(*(sm.m+i));
		while (v){
			lll_string_index_t j=FIND_FIRST_SET_BIT(v)|(i<<6);
			if (j==o->st.l){
				break;
			}
			for (lll_string_index_t n=k;n<j;n++){
				*(o->st.dt+n-l)=*(o->st.dt+n);
				*(sm.im+n)=n-l;
			}
			free(*(o->st.dt+j));
			k=j+1;
			l++;
			v&=v-1;
		}
	}
	free(sm.m);
	for (lll_string_index_t i=k;i<o->st.l;i++){
		*(o->st.dt+i-l)=*(o->st.dt+i);
		*(sm.im+i)=i-l;
	}
	if (l){
		o->st.l-=l;
		o->st.dt=realloc(o->st.dt,o->st.l*sizeof(lll_string_t*));
	}
	ai=o->h;
	for (lll_instruction_index_t i=0;i<o->ic;i++){
		if ((LLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_II||(LLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)>=LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP&&LLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)<=LLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ))&&(ai->t&ASSEMBLY_INSTRUCTION_LABEL)){
			ai->t&=~ASSEMBLY_INSTRUCTION_LABEL;
			lll_instruction_index_t j=*(lbl+ASSEMBLY_INSTRUCTION_MISC_FIELD(ai));
			if (j<128){
				ai->dt.j=j;
			}
			else{
				lll_relative_instruction_index_t off=((lll_relative_instruction_index_t)j)-i;
				if (GET_SIGN_ENCODED_INTEGER((int64_t)off)<(int64_t)j){
					ai->t|=LLL_ASSEMBLY_INSTRUCTION_RELATIVE;
					ai->dt.rj=off;
				}
				else{
					ai->dt.j=j;
				}
			}
		}
		else if (ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS||ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR||ai->t==LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR){
			ai->dt.s=*(sm.im+ai->dt.s);
		}
		ai++;
	}
	free(lbl);
	free(sm.im);
	return LLL_RETURN_NO_ERROR;
}
