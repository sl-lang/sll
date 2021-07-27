#include <lll_lib.h>
#include <_lll_lib_internal.h>
#include <stdint.h>
#include <stdlib.h>



#define DEFINE_LABEL(g_dt,lbl) \
	do{ \
		if ((g_dt)->_lm_l<=(lbl)){ \
			(g_dt)->_lm_l=(lbl)+1; \
			(g_dt)->_lm=realloc((g_dt)->_lm,(g_dt)->_lm_l*sizeof(lll_instruction_index_t)); \
		} \
		*((g_dt)->_lm+(lbl))=(g_dt)->a_dt->lc; \
	} while(0)
#define GENERATE_OPCODE(g_dt,op) (NEXT_INSTRUCTION((g_dt))->t=(op))
#define GENERATE_OPCODE_JUMP(g_dt,op,lbl) \
	do{ \
		lll_assembly_instruction_t* __ai=NEXT_INSTRUCTION((g_dt)); \
		__ai->t=(op)|ASSEMBLY_INSTRUCTION_LABEL; \
		__ai->dt._lbl=(lbl); \
	} while(0)
#define NEXT_INSTRUCTION(g_dt) ((g_dt)->a_dt->h+((g_dt)->a_dt->lc++))
#define NEXT_LABEL(g_dt) ((g_dt)->n_lbl++)



lll_stack_offset_t _generate(const lll_object_t* o,assembly_generator_data_t* g_dt);



lll_stack_offset_t _generate_on_stack(const lll_object_t* o,assembly_generator_data_t* g_dt);



lll_stack_offset_t _generate_sequential_jump(const lll_object_t* o,assembly_generator_data_t* g_dt,lll_assembly_instruction_type_t t,uint8_t st){
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	ASSERT(l);
	if (l==1){
		return sizeof(lll_operator_object_t)+_generate(LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t)),g_dt);
	}
	lll_stack_offset_t off=sizeof(lll_operator_object_t)+_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t)),g_dt);
	_lll_assembly_instruction_label_t e=NEXT_LABEL(g_dt);
	l-=2;
	while (l){
		l--;
		off+=_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
		GENERATE_OPCODE_JUMP(g_dt,t,e);
		if (l){
			GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_ROT);
			GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
		}
	}
	GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO);
	off+=_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
	if (!st){
		DEFINE_LABEL(g_dt,e);
	}
	else{
		GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TRUE);
		_lll_assembly_instruction_label_t tmp=NEXT_LABEL(g_dt);
		GENERATE_OPCODE_JUMP(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,tmp);
		DEFINE_LABEL(g_dt,e);
		GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FALSE);
		DEFINE_LABEL(g_dt,tmp);
	}
	return off;
}



lll_stack_offset_t _generate_cond_jump(const lll_object_t* o,assembly_generator_data_t* g_dt,_lll_assembly_instruction_label_t lbl,lll_assembly_instruction_type_t t){
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	ASSERT(l);
	if (l==1){
		return _generate(LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t)),g_dt);
	}
	lll_stack_offset_t off=sizeof(lll_operator_object_t)+_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t)),g_dt);
	l-=2;
	while (l){
		l--;
		off+=_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
		GENERATE_OPCODE_JUMP(g_dt,t,lbl);
		GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_ROT);
		GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
	}
	off+=_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
	GENERATE_OPCODE_JUMP(g_dt,t,lbl);
	GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO);
	return off;
}



lll_stack_offset_t _generate_jump(const lll_object_t* o,assembly_generator_data_t* g_dt,_lll_assembly_instruction_label_t lbl,uint8_t inv){
	lll_stack_offset_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_CHAR:
			if ((!!(((lll_char_object_t*)o)->v))^inv){
				GENERATE_OPCODE_JUMP(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return sizeof(lll_char_object_t)+eoff;
		case LLL_OBJECT_TYPE_INT:
			if ((!!(((lll_integer_object_t*)o)->v))^inv){
				GENERATE_OPCODE_JUMP(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return sizeof(lll_integer_object_t)+eoff;
		case LLL_OBJECT_TYPE_FLOAT:
			if ((!!(((lll_float_object_t*)o)->v))^inv){
				GENERATE_OPCODE_JUMP(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return sizeof(lll_float_object_t)+eoff;
		case LLL_OBJECT_TYPE_NIL:
		case LLL_OBJECT_TYPE_FALSE:
			if (inv){
				GENERATE_OPCODE_JUMP(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_TRUE:
			if (!inv){
				GENERATE_OPCODE_JUMP(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_STRING:
			return sizeof(lll_string_object_t)+eoff;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			return sizeof(lll_identifier_object_t)+eoff;
		case LLL_OBJECT_TYPE_PRINT:
			{
				lll_stack_offset_t off=_generate(o,g_dt);
				if (!inv){
					GENERATE_OPCODE_JUMP(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
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
					GENERATE_OPCODE_JUMP(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
				}
				return _get_object_size(o)+eoff;
			}
		case LLL_OBJECT_TYPE_IF:
		case LLL_OBJECT_TYPE_FOR:
			{
				lll_stack_offset_t off=_generate(o,g_dt);
				if (inv){
					GENERATE_OPCODE_JUMP(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_LESS:
			return _generate_cond_jump(o,g_dt,lbl,(inv?LLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:LLL_ASSEMBLY_INSTRUCTION_TYPE_JB))+eoff;
		case LLL_OBJECT_TYPE_LESS_EQUAL:
			return _generate_cond_jump(o,g_dt,lbl,(inv?LLL_ASSEMBLY_INSTRUCTION_TYPE_JA:LLL_ASSEMBLY_INSTRUCTION_TYPE_JBE))+eoff;
		case LLL_OBJECT_TYPE_EQUAL:
			return _generate_cond_jump(o,g_dt,lbl,(inv?LLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:LLL_ASSEMBLY_INSTRUCTION_TYPE_JE))+eoff;
		case LLL_OBJECT_TYPE_NOT_EQUAL:
			return _generate_cond_jump(o,g_dt,lbl,(inv?LLL_ASSEMBLY_INSTRUCTION_TYPE_JE:LLL_ASSEMBLY_INSTRUCTION_TYPE_JNE))+eoff;
		case LLL_OBJECT_TYPE_MORE:
			return _generate_cond_jump(o,g_dt,lbl,(inv?LLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:LLL_ASSEMBLY_INSTRUCTION_TYPE_JA))+eoff;
		case LLL_OBJECT_TYPE_MORE_EQUAL:
			return _generate_cond_jump(o,g_dt,lbl,(inv?LLL_ASSEMBLY_INSTRUCTION_TYPE_JB:LLL_ASSEMBLY_INSTRUCTION_TYPE_JAE))+eoff;
		case LLL_OBJECT_TYPE_IMPORT:
			if (inv){
				GENERATE_OPCODE_JUMP(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return sizeof(lll_import_object_t)+sizeof(lll_import_index_t)*((lll_import_object_t*)o)->ac+eoff;
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_stack_offset_t off=sizeof(lll_operation_list_object_t);
				lll_statement_count_t l=((lll_operation_list_object_t*)o)->sc;
				while (l){
					l--;
					off+=_generate(LLL_GET_OBJECT_STATEMENT(o,off),g_dt);
				}
				if (inv){
					GENERATE_OPCODE_JUMP(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			return sizeof(lll_debug_object_t)+eoff+_generate_jump(LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)o),g_dt,lbl,inv);
	}
	lll_stack_offset_t off=_generate_on_stack(o,g_dt);
	GENERATE_OPCODE_JUMP(g_dt,(inv?LLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:LLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ),lbl);
	GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
	return off+eoff;
}



lll_stack_offset_t _generate_for_loop(const lll_object_t* o,assembly_generator_data_t* g_dt){
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	ASSERT(l>=2);
	lll_stack_offset_t off=sizeof(lll_operator_object_t)+_generate(LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t)),g_dt);
	lll_object_t* cnd=LLL_GET_OBJECT_ARGUMENT(o,off);
	_lll_assembly_instruction_label_t s=NEXT_LABEL(g_dt);
	_lll_assembly_instruction_label_t e=NEXT_LABEL(g_dt);
	off+=_generate_jump(cnd,g_dt,e,1);
	l-=2;
	DEFINE_LABEL(g_dt,s);
	while (l){
		l--;
		off+=_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
		_generate_jump(cnd,g_dt,s,0);
	}
	DEFINE_LABEL(g_dt,e);
	return off;
}



lll_stack_offset_t _generate_call(const lll_object_t* o,assembly_generator_data_t* g_dt){
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	ASSERT(l);
	lll_stack_offset_t* al=malloc(l*sizeof(lll_stack_offset_t));
	lll_stack_offset_t off=sizeof(lll_operator_object_t);
	for (lll_arg_count_t i=0;i<l-1;i++){
		*(al+i)=off;
		off+=_get_object_size(LLL_GET_OBJECT_ARGUMENT(o,off));
	}
	*(al+l-1)=off;
	off+=_get_object_size(LLL_GET_OBJECT_ARGUMENT(o,off));
	while (l){
		l--;
		_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,*(al+l)),g_dt);
	}
	free(al);
	GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_CALL);
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
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
			return sizeof(lll_object_t)+eoff;
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
		case LLL_OBJECT_TYPE_NIL:
			GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_NIL);
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_TRUE:
			GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TRUE);
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_FALSE:
			GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FALSE);
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_STRING:
			{
				lll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS;
				ai->dt.s=((lll_string_object_t*)o)->i;
				return sizeof(lll_string_object_t)+eoff;
			}
		case LLL_OBJECT_TYPE_IDENTIFIER:
			{
				lll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD;
				lll_identifier_index_t idx=((lll_identifier_object_t*)o)->idx;
				ai->dt.v=(LLL_IDENTIFIER_GET_ARRAY_ID(idx)==LLL_MAX_SHORT_IDENTIFIER_LENGTH?*(g_dt->l_im+LLL_IDENTIFIER_GET_ARRAY_INDEX(idx)):*(g_dt->s_im[LLL_IDENTIFIER_GET_ARRAY_ID(idx)]+LLL_IDENTIFIER_GET_ARRAY_INDEX(idx)));
				return sizeof(lll_identifier_object_t)+eoff;
			}
		case LLL_OBJECT_TYPE_PRINT:
			{
				lll_stack_offset_t off=_generate(o,g_dt);
				GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_NIL);
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
				GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_DUP);
				lll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE;
				ai->dt.v=(LLL_IDENTIFIER_GET_ARRAY_ID(io->idx)==LLL_MAX_SHORT_IDENTIFIER_LENGTH?*(g_dt->l_im+LLL_IDENTIFIER_GET_ARRAY_INDEX(io->idx)):*(g_dt->s_im[LLL_IDENTIFIER_GET_ARRAY_ID(io->idx)]+LLL_IDENTIFIER_GET_ARRAY_INDEX(io->idx)));
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
		case LLL_OBJECT_TYPE_IF:
		case LLL_OBJECT_TYPE_FOR:
			{
				lll_stack_offset_t off=_generate(o,g_dt);
				GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_NIL);
				return off+eoff;
			}
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
			return _generate_chain(o,LLL_ASSEMBLY_INSTRUCTION_TYPE_BIT_AND,g_dt)+eoff;
		case LLL_OBJECT_TYPE_BIT_OR:
			return _generate_chain(o,LLL_ASSEMBLY_INSTRUCTION_TYPE_BIT_OR,g_dt)+eoff;
		case LLL_OBJECT_TYPE_BIT_XOR:
			return _generate_chain(o,LLL_ASSEMBLY_INSTRUCTION_TYPE_BIT_XOR,g_dt)+eoff;
		case LLL_OBJECT_TYPE_BIT_NOT:
			return _generate_chain(o,LLL_ASSEMBLY_INSTRUCTION_TYPE_BIT_NOT,g_dt)+eoff;
		case LLL_OBJECT_TYPE_LESS:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JAE,1)+eoff;
		case LLL_OBJECT_TYPE_LESS_EQUAL:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JA,1)+eoff;
		case LLL_OBJECT_TYPE_EQUAL:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JNE,1)+eoff;
		case LLL_OBJECT_TYPE_NOT_EQUAL:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JE,1)+eoff;
		case LLL_OBJECT_TYPE_MORE:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JBE,1)+eoff;
		case LLL_OBJECT_TYPE_MORE_EQUAL:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JB,1)+eoff;
		case LLL_OBJECT_TYPE_IMPORT:
			GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_NIL);
			return sizeof(lll_import_object_t)+sizeof(lll_import_index_t)*((lll_import_object_t*)o)->ac+eoff;
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_stack_offset_t off=sizeof(lll_operation_list_object_t);
				lll_statement_count_t l=((lll_operation_list_object_t*)o)->sc;
				while (l){
					l--;
					off+=_generate(LLL_GET_OBJECT_STATEMENT(o,off),g_dt);
				}
				GENERATE_OPCODE(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_NIL);
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			return sizeof(lll_debug_object_t)+eoff+_generate_on_stack(LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)o),g_dt);
	}
	lll_stack_offset_t off=sizeof(lll_operator_object_t);
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	while (l){
		l--;
		off+=_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
	}
	return off+eoff;
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
		case LLL_OBJECT_TYPE_TRUE:
		case LLL_OBJECT_TYPE_FALSE:
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
		case LLL_OBJECT_TYPE_NOT:
			ASSERT(!"Unimplemented");
		case LLL_OBJECT_TYPE_ASSIGN:
			{
				lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
				ASSERT(l>=2);
				lll_identifier_object_t* io=(lll_identifier_object_t*)LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t));
				ASSERT(LLL_GET_OBJECT_TYPE(io)==LLL_OBJECT_TYPE_IDENTIFIER);
				lll_stack_offset_t off=sizeof(lll_operator_object_t)+sizeof(lll_identifier_object_t)+_generate_on_stack(LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t)+sizeof(lll_identifier_object_t)),g_dt);
				lll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				ai->t=LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE;
				ai->dt.v=(LLL_IDENTIFIER_GET_ARRAY_ID(io->idx)==LLL_MAX_SHORT_IDENTIFIER_LENGTH?*(g_dt->l_im+LLL_IDENTIFIER_GET_ARRAY_INDEX(io->idx)):*(g_dt->s_im[LLL_IDENTIFIER_GET_ARRAY_ID(io->idx)]+LLL_IDENTIFIER_GET_ARRAY_INDEX(io->idx)));
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
			{
				lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
				ASSERT(l);
				if (l==1){
					return sizeof(lll_operator_object_t)+_generate(LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t)),g_dt)+eoff;
				}
				lll_stack_offset_t off=sizeof(lll_operator_object_t);
				if (l&1){
					_lll_assembly_instruction_label_t e=NEXT_LABEL(g_dt);
					l>>=1;
					while (l){
						l--;
						_lll_assembly_instruction_label_t nxt=NEXT_LABEL(g_dt);
						off+=_generate_jump(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt,nxt,1);
						off+=_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
						GENERATE_OPCODE_JUMP(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,e);
						DEFINE_LABEL(g_dt,nxt);
					}
					off+=_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
					DEFINE_LABEL(g_dt,e);
				}
				else{
					_lll_assembly_instruction_label_t e=NEXT_LABEL(g_dt);
					l>>=1;
					while (l){
						l--;
						_lll_assembly_instruction_label_t nxt=NEXT_LABEL(g_dt);
						off+=_generate_jump(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt,nxt,1);
						off+=_generate(LLL_GET_OBJECT_ARGUMENT(o,off),g_dt);
						if (l){
							GENERATE_OPCODE_JUMP(g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,e);
						}
						DEFINE_LABEL(g_dt,nxt);
					}
					DEFINE_LABEL(g_dt,e);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_FOR:
			return _generate_for_loop(o,g_dt)+eoff;
		case LLL_OBJECT_TYPE_LESS:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JAE,0)+eoff;
		case LLL_OBJECT_TYPE_LESS_EQUAL:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JA,0)+eoff;
		case LLL_OBJECT_TYPE_EQUAL:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JNE,0)+eoff;
		case LLL_OBJECT_TYPE_NOT_EQUAL:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JE,0)+eoff;
		case LLL_OBJECT_TYPE_MORE:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JBE,0)+eoff;
		case LLL_OBJECT_TYPE_MORE_EQUAL:
			return _generate_sequential_jump(o,g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_JB,0)+eoff;
		case LLL_OBJECT_TYPE_IMPORT:
			return sizeof(lll_import_object_t)+sizeof(lll_import_index_t)*((lll_import_object_t*)o)->ac+eoff;
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
		case LLL_OBJECT_TYPE_TRUE:
		case LLL_OBJECT_TYPE_FALSE:
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
					if (g_dt->_l_sc<id->sc){
						*(g_dt->_sc_vi+g_dt->_l_sc)=g_dt->_n_vi;
						g_dt->_l_sc=id->sc;
						if (id->sc<=g_dt->_sc_vi_l){
							g_dt->_n_vi=*(g_dt->_sc_vi+g_dt->_l_sc);
						}
						else{
							g_dt->_sc_vi_l=id->sc;
						}
					}
					else if (g_dt->_l_sc>id->sc){
						*(g_dt->_sc_vi+g_dt->_l_sc)=g_dt->_n_vi;
						g_dt->_l_sc=id->sc;
						g_dt->_n_vi=*(g_dt->_sc_vi+g_dt->_l_sc);
					}
					if (*(g_dt->l_im+j)==LLL_MAX_VARIABLE_INDEX){
						*(g_dt->l_im+j)=g_dt->_n_vi;
						g_dt->_n_vi++;
						if (g_dt->_n_vi>g_dt->a_dt->vc){
							g_dt->a_dt->vc=g_dt->_n_vi;
						}
					}
				}
				else{
					lll_identifier_t* id=g_dt->c_dt->i_dt.s[k].dt+j;
					if (g_dt->_l_sc<id->sc){
						*(g_dt->_sc_vi+g_dt->_l_sc)=g_dt->_n_vi;
						g_dt->_l_sc=id->sc;
						if (id->sc<=g_dt->_sc_vi_l){
							g_dt->_n_vi=*(g_dt->_sc_vi+g_dt->_l_sc);
						}
						else{
							g_dt->_sc_vi_l=id->sc;
						}
					}
					else if (g_dt->_l_sc>id->sc){
						*(g_dt->_sc_vi+g_dt->_l_sc)=g_dt->_n_vi;
						g_dt->_l_sc=id->sc;
						g_dt->_n_vi=*(g_dt->_sc_vi+g_dt->_l_sc);
					}
					if (*(g_dt->s_im[k]+j)==LLL_MAX_VARIABLE_INDEX){
						*(g_dt->s_im[k]+j)=g_dt->_n_vi;
						g_dt->_n_vi++;
						if (g_dt->_n_vi>g_dt->a_dt->vc){
							g_dt->a_dt->vc=g_dt->_n_vi;
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
	o->st.l=c_dt->st.l;
	o->st.dt=malloc(o->st.l*sizeof(lll_string_t*));
	for (lll_string_index_t i=0;i<o->st.l;i++){
		const lll_string_t* s=*(c_dt->st.dt+i);
		lll_string_t* d=malloc(sizeof(lll_string_t)+(s->l+1)*sizeof(lll_char_t));
		d->l=s->l;
		d->c=s->c;
		for (lll_string_length_t j=0;j<s->l;j++){
			d->v[j]=s->v[j];
		}
		d->v[d->l]=0;
		*(o->st.dt+i)=d;
	}
	o->lc=0;
	o->vc=0;
	assembly_generator_data_t g_dt={
		o,
		c_dt,
		0,
		.l_im=malloc(c_dt->i_dt.ill*sizeof(lll_variable_index_t)),
		._n_vi=0,
		._l_sc=0,
		._sc_vi=malloc(c_dt->_n_sc_id*sizeof(lll_variable_index_t)),
		._sc_vi_l=0,
		._lm=NULL,
		._lm_l=0
	};
	for (uint8_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		g_dt.s_im[i]=malloc(c_dt->i_dt.s[i].l*sizeof(lll_variable_index_t));
		for (lll_identifier_list_length_t j=0;j<c_dt->i_dt.s[i].l;j++){
			*(g_dt.s_im[i]+j)=LLL_MAX_VARIABLE_INDEX;
		}
	}
	for (lll_identifier_list_length_t i=0;i<c_dt->i_dt.ill;i++){
		*(g_dt.l_im+i)=LLL_MAX_VARIABLE_INDEX;
	}
	_map_identifiers(c_dt->h,&g_dt);
	for (lll_function_index_t i=0;i<c_dt->f_dt.l;i++){
		const lll_function_t* k=*(c_dt->f_dt.dt+i);
		g_dt._n_vi=o->vc;
		g_dt._l_sc=0;
		g_dt._sc_vi_l=0;
		const lll_function_object_t* fo=(const lll_function_object_t*)(c_dt->_s.ptr+k->off);
		ASSERT(LLL_GET_OBJECT_TYPE(fo)==LLL_OBJECT_TYPE_FUNC);
		lll_stack_offset_t off=sizeof(lll_function_object_t);
		for (lll_arg_count_t j=0;j<fo->ac;j++){
			off+=_map_identifiers(LLL_GET_OBJECT_ARGUMENT(fo,off),&g_dt);
		}
	}
	free(g_dt._sc_vi);
	_generate(c_dt->h,&g_dt);
	GENERATE_OPCODE(&g_dt,LLL_ASSEMBLY_INSTRUCTION_TYPE_END);
	lll_assembly_instruction_t* ai=o->h;
	for (lll_instruction_index_t i=0;i<o->lc;i++){
		if (ai->t&ASSEMBLY_INSTRUCTION_LABEL){
			ai->t&=~ASSEMBLY_INSTRUCTION_LABEL;
			ai->dt.j=*(g_dt._lm+ai->dt._lbl);
		}
		ai++;
	}
	for (uint8_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		free(g_dt.s_im[i]);
	}
	free(g_dt.l_im);
	free(g_dt._lm);
	return LLL_RETURN_NO_ERROR;
}
