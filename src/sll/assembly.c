#include <sll/_internal/assembly.h>
#include <sll/_internal/common.h>
#include <sll/_internal/intrinsics.h>
#include <sll/_internal/stack.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/gc.h>
#include <sll/generated/assembly_optimizer.h>
#include <sll/identifier.h>
#include <sll/location.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/object.h>
#include <sll/platform/memory.h>
#include <sll/platform/time.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>



#define DEFINE_LABEL(g_dt,id) \
	do{ \
		sll_assembly_instruction_t* __ai=_acquire_next_instruction((g_dt)->assembly_data); \
		__ai->type=ASSEMBLY_INSTRUCTION_TYPE_LABEL_TARGET; \
		ASSEMBLY_INSTRUCTION_MISC_FIELD_SET(__ai,(id)); \
	} while(0)
#define GENERATE_OPCODE(g_dt,op) (_acquire_next_instruction((g_dt)->assembly_data)->type=(op))
#define GENERATE_OPCODE_WITH_LABEL(g_dt,op,lbl) \
	do{ \
		sll_assembly_instruction_t* __ai=_acquire_next_instruction((g_dt)->assembly_data); \
		__ai->type=(op)|ASSEMBLY_INSTRUCTION_FLAG_LABEL; \
		ASSEMBLY_INSTRUCTION_MISC_FIELD_SET(__ai,(lbl)); \
	} while(0)
#define GENERATE_DEBUG_DATA(g_dt,o) \
	do{ \
		if ((o)->type==SLL_NODE_TYPE_DBG){ \
			sll_assembly_instruction_t* __ai=_acquire_next_instruction((g_dt)->assembly_data); \
			__ai->type=ASSEMBLY_INSTRUCTION_TYPE_DBG; \
			__ai->data.string_index=(o)->data.string_index; \
		} \
	} while (0)
#define GET_SIGN_ENCODED_INTEGER(x) ((x)<0?((~(x))<<1)|1:(x)<<1)
#define GET_VARIABLE_INDEX(o,g_dt) (SLL_IDENTIFIER_GET_ARRAY_ID((o)->data.identifier_index)==SLL_MAX_SHORT_IDENTIFIER_LENGTH?((g_dt)->identifier_map.long_identifier_map+SLL_IDENTIFIER_GET_ARRAY_INDEX((o)->data.identifier_index))->variable:((g_dt)->identifier_map.short_identifier_map[SLL_IDENTIFIER_GET_ARRAY_ID((o)->data.identifier_index)]+SLL_IDENTIFIER_GET_ARRAY_INDEX((o)->data.identifier_index))->variable)
#define NEXT_LABEL(g_dt) ((g_dt)->next_label++)

#define PUSH (g_dt->stack_offset++)
#define POP (g_dt->stack_offset--)
#define POP2 (g_dt->stack_offset-=2)
#define POP3 (g_dt->stack_offset-=3)
#define POP_N(n) (g_dt->stack_offset-=(n))



static sll_assembly_instruction_t _assembly_nop={
	ASSEMBLY_INSTRUCTION_TYPE_NOP
};



static const sll_node_t* _generate_on_stack(const sll_node_t* o,assembly_generator_data_t* g_dt);



static const sll_node_t* _generate(const sll_node_t* o,assembly_generator_data_t* g_dt);



static const sll_node_t* _map_identifiers(const sll_node_t* o,const sll_source_file_t* sf,assembly_generator_data_t* g_dt,sll_scope_t fn_sc){
	while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
		o=(o->type==SLL_NODE_TYPE_CHANGE_STACK?o->data._next_node:o+1);
	}
	switch (o->type){
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_COMPLEX:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_IDENTIFIER:
			{
				sll_identifier_index_t i=SLL_IDENTIFIER_GET_ARRAY_INDEX(o->data.identifier_index);
				sll_identifier_index_t j=SLL_IDENTIFIER_GET_ARRAY_ID(o->data.identifier_index);
				if (j==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
					sll_identifier_t* id=sf->identifier_table.long_data+i;
					if ((g_dt->identifier_map.long_identifier_map+i)->variable==SLL_MAX_VARIABLE_INDEX){
						if (SLL_IDENTIFIER_IS_TLS(id)){
							(g_dt->identifier_map.long_identifier_map+i)->variable=(g_dt->identifier_map.tls_variable_count<<1)|1;
							g_dt->identifier_map.tls_variable_count++;
						}
						else{
							(g_dt->identifier_map.long_identifier_map+i)->variable=g_dt->identifier_map.variable_count<<1;
							g_dt->identifier_map.variable_count++;
						}
					}
					if (*(g_dt->identifier_remove_data.long_+i)!=VARIABLE_OFFSET_NEVER_DELETE){
						if (fn_sc==SLL_MAX_SCOPE||id->scope>=fn_sc){
							*(g_dt->identifier_remove_data.long_+i)=PTR(o);
						}
						else{
							*(g_dt->identifier_remove_data.long_+i)=VARIABLE_OFFSET_NEVER_DELETE;
						}
					}
				}
				else{
					sll_identifier_t* id=sf->identifier_table.short_[j].data+i;
					if ((g_dt->identifier_map.short_identifier_map[j]+i)->variable==SLL_MAX_VARIABLE_INDEX){
						if (SLL_IDENTIFIER_IS_TLS(id)){
							(g_dt->identifier_map.short_identifier_map[j]+i)->variable=(g_dt->identifier_map.tls_variable_count<<1)|1;
							g_dt->identifier_map.tls_variable_count++;
						}
						else{
							(g_dt->identifier_map.short_identifier_map[j]+i)->variable=g_dt->identifier_map.variable_count<<1;
							g_dt->identifier_map.variable_count++;
						}
					}
					if (*(g_dt->identifier_remove_data.short_[j]+i)!=VARIABLE_OFFSET_NEVER_DELETE){
						if (fn_sc==SLL_MAX_SCOPE||id->scope>=fn_sc){
							*(g_dt->identifier_remove_data.short_[j]+i)=PTR(o);
						}
						else{
							*(g_dt->identifier_remove_data.short_[j]+i)=VARIABLE_OFFSET_NEVER_DELETE;
						}
					}
				}
				return o+1;
			}
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t l=o->data.array_length;
				o++;
				while (l){
					l--;
					o=_map_identifiers(o,sf,g_dt,fn_sc);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->data.map_length;
				o++;
				while (l){
					l--;
					o=_map_identifiers(o,sf,g_dt,fn_sc);
				}
				return o;
			}
		case SLL_NODE_TYPE_FUNC:
			return sll_skip_node_const(o);
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->data.function.arg_count;
				o++;
				while (l){
					l--;
					o=_map_identifiers(o,sf,g_dt,fn_sc);
				}
				return o;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			{
				sll_arg_count_t l=o->data.loop.arg_count;
				o++;
				while (l){
					l--;
					o=_map_identifiers(o,sf,g_dt,fn_sc);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->data.arg_count;
	o++;
	while (l){
		l--;
		o=_map_identifiers(o,sf,g_dt,fn_sc);
	}
	return o;
}



static const sll_node_t* _skip_with_dbg(const sll_node_t* o,assembly_generator_data_t* g_dt){
	while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
		GENERATE_DEBUG_DATA(g_dt,o);
		o=(o->type==SLL_NODE_TYPE_CHANGE_STACK?o->data._next_node:o+1);
	}
	switch (o->type){
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_COMPLEX:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t l=o->data.array_length;
				o++;
				while (l){
					l--;
					o=_skip_with_dbg(o,g_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->data.map_length;
				o++;
				while (l){
					l--;
					o=_skip_with_dbg(o,g_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->data.function.arg_count;
				o++;
				while (l){
					l--;
					o=_skip_with_dbg(o,g_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			{
				sll_arg_count_t l=o->data.loop.arg_count;
				o++;
				while (l){
					l--;
					o=_skip_with_dbg(o,g_dt);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->data.arg_count;
	o++;
	while (l){
		l--;
		o=_skip_with_dbg(o,g_dt);
	}
	return o;
}



static const sll_node_t* _generate_sequential_jump(const sll_node_t* o,assembly_generator_data_t* g_dt,sll_assembly_instruction_type_t t,sll_bool_t stack){
	sll_arg_count_t l=o->data.arg_count;
	o++;
	if (!l){
		if (stack){
			GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
			PUSH;
		}
		return o;
	}
	if (l==1){
		if (stack){
			GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
			PUSH;
		}
		return _generate(o,g_dt);
	}
	o=_generate_on_stack(o,g_dt);
	assembly_instruction_label_t e=NEXT_LABEL(g_dt);
	l-=2;
	while (l){
		l--;
		o=_generate_on_stack(o,g_dt);
		GENERATE_OPCODE_WITH_LABEL(g_dt,t,e);
		GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP);
		POP;
	}
	o=_generate_on_stack(o,g_dt);
	GENERATE_OPCODE_WITH_LABEL(g_dt,t,e);
	GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO);
	POP2;
	if (!stack){
		DEFINE_LABEL(g_dt,e);
	}
	else{
		GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE);
		assembly_instruction_label_t tmp=NEXT_LABEL(g_dt);
		GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,tmp);
		DEFINE_LABEL(g_dt,e);
		GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
		DEFINE_LABEL(g_dt,tmp);
		PUSH;
	}
	return o;
}



static const sll_node_t* _generate_cond_jump(const sll_node_t* o,assembly_generator_data_t* g_dt,assembly_instruction_label_t lbl,sll_assembly_instruction_type_t t){
	sll_arg_count_t l=o->data.arg_count;
	o++;
	if (!l){
		return o;
	}
	if (l==1){
		return _generate(o,g_dt);
	}
	o=_generate_on_stack(o,g_dt);
	l-=2;
	while (l){
		l--;
		o=_generate_on_stack(o,g_dt);
		GENERATE_OPCODE_WITH_LABEL(g_dt,t,lbl);
		GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP);
		POP;
	}
	o=_generate_on_stack(o,g_dt);
	GENERATE_OPCODE_WITH_LABEL(g_dt,t,lbl);
	GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO);
	POP2;
	return o;
}



static void _generate_identifier(const sll_node_t* n,assembly_generator_data_t* g_dt){
	sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
	ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD;
	if (SLL_IDENTIFIER_GET_ARRAY_ID(n->data.identifier_index)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
		if (*(g_dt->identifier_remove_data.long_+SLL_IDENTIFIER_GET_ARRAY_INDEX(n->data.identifier_index))==n){
			ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL;
		}
		ai->data.variable=(g_dt->identifier_map.long_identifier_map+SLL_IDENTIFIER_GET_ARRAY_INDEX(n->data.identifier_index))->variable;
	}
	else{
		if (*(g_dt->identifier_remove_data.short_[SLL_IDENTIFIER_GET_ARRAY_ID(n->data.identifier_index)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(n->data.identifier_index))==n){
			ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL;
		}
		ai->data.variable=(g_dt->identifier_map.short_identifier_map[SLL_IDENTIFIER_GET_ARRAY_ID(n->data.identifier_index)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(n->data.identifier_index))->variable;
	}
	PUSH;
}



static const sll_node_t* _generate_two_on_stack(const sll_node_t* o,assembly_generator_data_t* g_dt,sll_arg_count_t l){
	SLL_ASSERT(l);
	o=_generate_on_stack(o,g_dt);
	l--;
	if (!l){
		GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
		PUSH;
	}
	else{
		o=_generate_on_stack(o,g_dt);
		l--;
		while (l){
			l--;
			o=_generate(o,g_dt);
		}
	}
	return o;
}



static const sll_node_t* _generate_jump(const sll_node_t* o,assembly_generator_data_t* g_dt,assembly_instruction_label_t lbl,sll_bool_t inv){
	while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
		GENERATE_DEBUG_DATA(g_dt,o);
		o=(o->type==SLL_NODE_TYPE_CHANGE_STACK?o->data._next_node:o+1);
	}
	NOT_FIELD(o);
	switch (o->type){
		case SLL_NODE_TYPE_INT:
			if ((!!o->data.int_)^inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return o+1;
		case SLL_NODE_TYPE_FLOAT:
			if ((!!o->data.float_)^inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return o+1;
		case SLL_NODE_TYPE_CHAR:
			if ((!!o->data.char_)^inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return o+1;
		case SLL_NODE_TYPE_COMPLEX:
			if (((!!o->data.complex_.real)|(!!o->data.complex_.imag))^inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return o+1;
		case SLL_NODE_TYPE_STRING:
			if ((!!(g_dt->source_file->string_table.data+o->data.string_index)->length)^inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			if ((!!o->data.array_length)^inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return o+1;
		case SLL_NODE_TYPE_MAP:
			if ((!!o->data.map_length)^inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return o+1;
		case SLL_NODE_TYPE_IDENTIFIER:
			_generate_identifier(o,g_dt);
			GENERATE_OPCODE_WITH_LABEL(g_dt,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ),lbl);
			GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
			POP;
			return o+1;
		case SLL_NODE_TYPE_FUNCTION_ID:
			if (!inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return o+1;
		case SLL_NODE_TYPE_PRINT:
			o=_generate(o,g_dt);
			if (!inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return o;
		case SLL_NODE_TYPE_AND:
			{
				sll_arg_count_t l=o->data.arg_count;
				o++;
				if (!l){
					if (inv){
						GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
					}
					return o;
				}
				if (inv){
					while (l){
						l--;
						o=_generate_jump(o,g_dt,lbl,1);
					}
					return o;
				}
				assembly_instruction_label_t e=NEXT_LABEL(g_dt);
				l--;
				while (l){
					l--;
					o=_generate_jump(o,g_dt,e,1);
				}
				o=_generate_jump(o,g_dt,lbl,0);
				DEFINE_LABEL(g_dt,e);
				return o;
			}
		case SLL_NODE_TYPE_OR:
			{
				sll_arg_count_t l=o->data.arg_count;
				o++;
				if (!l){
					if (inv){
						GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
					}
					return o;
				}
				if (!inv){
					while (l){
						l--;
						o=_generate_jump(o,g_dt,lbl,0);
					}
					return o;
				}
				assembly_instruction_label_t e=NEXT_LABEL(g_dt);
				l--;
				while (l){
					l--;
					o=_generate_jump(o,g_dt,e,0);
				}
				o=_generate_jump(o,g_dt,lbl,1);
				DEFINE_LABEL(g_dt,e);
				return o;
			}
		case SLL_NODE_TYPE_NOT:
			{
				sll_arg_count_t l=o->data.arg_count;
				if (!l){
					if (!inv){
						GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
					}
					return o+1;
				}
				o++;
				if (l==1){
					return _generate_jump(o,g_dt,lbl,!inv);
				}
				SLL_UNIMPLEMENTED();
			}
		case SLL_NODE_TYPE_BOOL:
			{
				sll_arg_count_t l=o->data.arg_count;
				if (!l){
					if (!inv){
						GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
					}
					return o+1;
				}
				o++;
				if (l==1){
					return _generate_jump(o,g_dt,lbl,inv);
				}
				SLL_UNIMPLEMENTED();
			}
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			if (!inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return sll_skip_node_const(o);
		case SLL_NODE_TYPE_INTERNAL_FUNC_LOAD:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_INLINE_FUNC:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_IF:
		case SLL_NODE_TYPE_SWITCH:
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_RETURN:
		case SLL_NODE_TYPE_BREAK:
		case SLL_NODE_TYPE_CONTINUE:
		case SLL_NODE_TYPE_OPERATION_LIST:
			o=_generate(o,g_dt);
			if (inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return o;
		case SLL_NODE_TYPE_LESS:
			return _generate_cond_jump(o,g_dt,lbl,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:SLL_ASSEMBLY_INSTRUCTION_TYPE_JB));
		case SLL_NODE_TYPE_LESS_EQUAL:
			return _generate_cond_jump(o,g_dt,lbl,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JA:SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE));
		case SLL_NODE_TYPE_EQUAL:
			return _generate_cond_jump(o,g_dt,lbl,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:SLL_ASSEMBLY_INSTRUCTION_TYPE_JE));
		case SLL_NODE_TYPE_NOT_EQUAL:
			return _generate_cond_jump(o,g_dt,lbl,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JE:SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE));
		case SLL_NODE_TYPE_MORE:
			return _generate_cond_jump(o,g_dt,lbl,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:SLL_ASSEMBLY_INSTRUCTION_TYPE_JA));
		case SLL_NODE_TYPE_MORE_EQUAL:
			return _generate_cond_jump(o,g_dt,lbl,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JB:SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE));
		case SLL_NODE_TYPE_STRICT_EQUAL:
			return _generate_cond_jump(o,g_dt,lbl,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JSNE:SLL_ASSEMBLY_INSTRUCTION_TYPE_JSE));
		case SLL_NODE_TYPE_STRICT_NOT_EQUAL:
			return _generate_cond_jump(o,g_dt,lbl,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JSE:SLL_ASSEMBLY_INSTRUCTION_TYPE_JSNE));
		case SLL_NODE_TYPE_HAS:
			{
				if (!o->data.arg_count){
					if (inv){
						GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
					}
					return o+1;
				}
				o=_generate_two_on_stack(o+1,g_dt,o->data.arg_count);
				GENERATE_OPCODE_WITH_LABEL(g_dt,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JNI:SLL_ASSEMBLY_INSTRUCTION_TYPE_JI),lbl);
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO);
				POP2;
				return o;
			}
		case SLL_NODE_TYPE_REF:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_COMMA:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_THREAD_ID:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_THREAD_WAIT:
		case SLL_NODE_TYPE_THREAD_LOCK:
		case SLL_NODE_TYPE_THREAD_SEMAPHORE:
		case SLL_NODE_TYPE_THREAD_BARRIER_EQ:
		case SLL_NODE_TYPE_THREAD_BARRIER_GEQ:
		case SLL_NODE_TYPE_THREAD_EXIT:
		case SLL_NODE_TYPE_READ_BLOCKING:
		case SLL_NODE_TYPE_READ_BLOCKING_CHAR:
			SLL_UNIMPLEMENTED();
	}
	o=_generate_on_stack(o,g_dt);
	GENERATE_OPCODE_WITH_LABEL(g_dt,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ),lbl);
	GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
	POP;
	return o;
}



static const sll_node_t* _generate_call(const sll_node_t* o,assembly_generator_data_t* g_dt){
	SLL_ASSERT(o->type==SLL_NODE_TYPE_CALL);
	sll_arg_count_t ac=o->data.arg_count;
	const sll_node_t* fn=o+1;
	if (!ac){
		GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
		PUSH;
		return fn;
	}
	sll_arg_count_t l=ac;
	o=sll_skip_node_const(fn);
	l--;
	while (l){
		l--;
		o=_generate_on_stack(o,g_dt);
	}
	_generate_on_stack(fn,g_dt);
	sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
	ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL;
	ai->data.arg_count=ac-1;
	POP_N(ac-1);
	return o;
}



static const sll_node_t* _generate_call_array(const sll_node_t* o,assembly_generator_data_t* g_dt){
	SLL_ASSERT(o->type==SLL_NODE_TYPE_CALL_ARRAY);
	sll_arg_count_t ac=o->data.arg_count;
	o++;
	if (!ac){
		GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
		PUSH;
		return o;
	}
	sll_arg_count_t l=ac;
	const sll_node_t* fn=o;
	o=sll_skip_node_const(fn);
	l--;
	if (!l){
		GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO);
	}
	else{
		o=_generate_on_stack(o,g_dt);
		l--;
		while (l){
			l--;
			o=_generate(o,g_dt);
		}
	}
	_generate_on_stack(fn,g_dt);
	GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ARRAY);
	POP;
	return o;
}



static const sll_node_t* _generate_inline_function(const sll_node_t* o,assembly_generator_data_t* g_dt){
	SLL_ASSERT(o->type==SLL_NODE_TYPE_INLINE_FUNC);
	assembly_instruction_label_t e=NEXT_LABEL(g_dt);
	assembly_instruction_label_t p_rt=g_dt->return_label;
	g_dt->return_label=e;
	sll_arg_count_t l=o->data.arg_count;
	o++;
	while (l){
		l--;
		o=_generate(o,g_dt);
	}
	GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
	PUSH;
	DEFINE_LABEL(g_dt,e);
	g_dt->return_label=p_rt;
	return o;
}



static const sll_node_t* _generate_assign(const sll_node_t* o,assembly_generator_data_t* g_dt,sll_bool_t stack){
	sll_arg_count_t l=o->data.arg_count;
	o++;
	if (!l){
		return o;
	}
	if (l==1){
		return _generate(o,g_dt);
	}
	while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
		GENERATE_DEBUG_DATA(g_dt,o);
		o=(o->type==SLL_NODE_TYPE_CHANGE_STACK?o->data._next_node:o+1);
	}
	const sll_node_t* io=o;
	if (io->type==SLL_NODE_TYPE_IDENTIFIER){
		if ((SLL_IDENTIFIER_GET_ARRAY_ID(io->data.identifier_index)==SLL_MAX_SHORT_IDENTIFIER_LENGTH&&*(g_dt->identifier_remove_data.long_+SLL_IDENTIFIER_GET_ARRAY_INDEX(io->data.identifier_index))==io)||(SLL_IDENTIFIER_GET_ARRAY_ID(io->data.identifier_index)!=SLL_MAX_SHORT_IDENTIFIER_LENGTH&&*(g_dt->identifier_remove_data.short_[SLL_IDENTIFIER_GET_ARRAY_ID(io->data.identifier_index)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(io->data.identifier_index))==io)){
			o++;
			l--;
			do{
				o=_generate(o,g_dt);
				l--;
			} while (l);
			return o;
		}
		o=_generate_on_stack(o+1,g_dt);
		if (stack){
			SLL_UNIMPLEMENTED();
		}
		sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
		ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP;
		ai->data.variable=GET_VARIABLE_INDEX(io,g_dt);
		POP;
		l-=2;
	}
	else if (io->type==SLL_NODE_TYPE_VAR_ACCESS){
		sll_arg_count_t io_l=io->data.arg_count;
		SLL_ASSERT(io_l>1);
		io++;
		while (io->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||io->type==SLL_NODE_TYPE_CHANGE_STACK){
			GENERATE_DEBUG_DATA(g_dt,o);
			io=(io->type==SLL_NODE_TYPE_CHANGE_STACK?io->data._next_node:io+1);
		}
		SLL_ASSERT(io->type==SLL_NODE_TYPE_IDENTIFIER);
		sll_variable_index_t vi=GET_VARIABLE_INDEX(io,g_dt);
		io++;
		io_l--;
		sll_arg_count_t vl=io_l;
		do{
			while (io->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||io->type==SLL_NODE_TYPE_CHANGE_STACK){
				GENERATE_DEBUG_DATA(g_dt,o);
				io=(io->type==SLL_NODE_TYPE_CHANGE_STACK?io->data._next_node:io+1);
			}
			SLL_ASSERT(io->type==SLL_NODE_TYPE_FIELD);
			sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
			ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS;
			ai->data.string_index=io->data.string_index;
			PUSH;
			io++;
			io_l--;
		} while (io_l);
		o=_generate_on_stack(io,g_dt);
		if (stack){
			SLL_UNIMPLEMENTED();
		}
		sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
		ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR_ACCESS;
		ai->data.variable_access.variable=vi;
		ai->data.variable_access.arg_count=vl;
		POP_N(vl+1);
		l-=2;
	}
	else if (io->type==SLL_NODE_TYPE_ACCESS&&io->data.arg_count>=2){
		o++;
		while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
			GENERATE_DEBUG_DATA(g_dt,o);
			o=(o->type==SLL_NODE_TYPE_CHANGE_STACK?o->data._next_node:o+1);
		}
		sll_identifier_index_t id=SLL_MAX_IDENTIFIER_INDEX;
		if (o->type==SLL_NODE_TYPE_IDENTIFIER){
			id=GET_VARIABLE_INDEX(o,g_dt);
			o++;
		}
		else{
			o=_generate_on_stack(o,g_dt);
		}
		sll_arg_count_t io_l=io->data.arg_count-1;
		sll_assembly_instruction_type_t ai_t=SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN;
		sll_stack_offset_t pop=1;
		sll_arg_count_t i=io_l;
		if (io_l==2){
			ai_t=SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_TWO;
			pop=2;
		}
		else if (io_l>=3){
			ai_t=SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_THREE;
			pop=3;
			i=3;
		}
		io_l-=i;
		while (i){
			i--;
			o=_generate_on_stack(o,g_dt);
		}
		while (io_l){
			io_l--;
			o=_generate(o,g_dt);
		}
		o=_generate_on_stack(o,g_dt);
		if (stack){
			SLL_UNIMPLEMENTED();
		}
		sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
		ai->type=ai_t;
		if (id!=SLL_MAX_IDENTIFIER_INDEX){
			ai->type|=SLL_ASSEMBLY_INSTRUCTION_FLAG_INPLACE;
			ai->data.variable=id;
		}
		else{
			pop++;
		}
		POP_N(pop);
		l-=2;
	}
	else if (stack){
		GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
		PUSH;
	}
	while (l){
		l--;
		o=_generate(o,g_dt);
	}
	return o;
}



static const sll_node_t* _mark_loop_delete(const sll_node_t* o,const assembly_generator_data_t* g_dt,bitmap_t* v_st,sll_scope_t sc){
	while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
		o=(o->type==SLL_NODE_TYPE_CHANGE_STACK?o->data._next_node:o+1);
	}
	switch (o->type){
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_COMPLEX:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t l=o->data.array_length;
				o++;
				while (l){
					l--;
					o=_mark_loop_delete(o,g_dt,v_st,sc);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->data.map_length;
				o++;
				while (l){
					l--;
					o=_mark_loop_delete(o,g_dt,v_st,sc);
				}
				return o;
			}
		case SLL_NODE_TYPE_IDENTIFIER:
			{
				sll_identifier_index_t i=SLL_IDENTIFIER_GET_ARRAY_INDEX(o->data.identifier_index);
				if (SLL_IDENTIFIER_GET_ARRAY_ID(o->data.identifier_index)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
					sll_identifier_t* id=g_dt->source_file->identifier_table.long_data+i;
					if (id->scope<=sc&&*(g_dt->identifier_remove_data.long_+i)==o){
						*(g_dt->identifier_remove_data.long_+i)=VARIABLE_OFFSET_NEVER_DELETE;
						sll_variable_index_t k=(g_dt->identifier_map.long_identifier_map+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->data.identifier_index))->variable;
						*(v_st+(k>>6))|=1ull<<(k&63);
					}
				}
				else{
					sll_identifier_t* id=g_dt->source_file->identifier_table.short_[SLL_IDENTIFIER_GET_ARRAY_ID(o->data.identifier_index)].data+i;
					if (id->scope<=sc&&*(g_dt->identifier_remove_data.short_[SLL_IDENTIFIER_GET_ARRAY_ID(o->data.identifier_index)]+i)==o){
						*(g_dt->identifier_remove_data.short_[SLL_IDENTIFIER_GET_ARRAY_ID(o->data.identifier_index)]+i)=VARIABLE_OFFSET_NEVER_DELETE;
						sll_variable_index_t k=(g_dt->identifier_map.short_identifier_map[SLL_IDENTIFIER_GET_ARRAY_ID(o->data.identifier_index)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->data.identifier_index))->variable;
						*(v_st+(k>>6))|=1ull<<(k&63);
					}
				}
				return o+1;
			}
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->data.function.arg_count;
				o++;
				while (l){
					l--;
					o=_mark_loop_delete(o,g_dt,v_st,sc);
				}
				return o;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			{
				sll_arg_count_t l=o->data.loop.arg_count;
				o++;
				while (l){
					l--;
					o=_mark_loop_delete(o,g_dt,v_st,sc);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->data.arg_count;
	o++;
	while (l){
		l--;
		o=_mark_loop_delete(o,g_dt,v_st,sc);
	}
	return o;
}



static void _init_loop_data(assembly_generator_data_t* g_dt,assembly_loop_generator_data_t* lg_dt,assembly_instruction_label_t cnt,assembly_instruction_label_t brk){
	lg_dt->last_loop_data=g_dt->loop_data;
	g_dt->loop_data.continue_=cnt;
	g_dt->loop_data.break_=brk;
	lg_dt->variable_data=sll_zero_allocate(g_dt->loop_variable_count*sizeof(bitmap_t));
}



static const sll_node_t* _generate_loop_start(assembly_generator_data_t* g_dt,const sll_node_t* o,assembly_loop_generator_data_t* lg_dt,assembly_instruction_label_t cnt,assembly_instruction_label_t brk){
	_init_loop_data(g_dt,lg_dt,cnt,brk);
	SLL_ASSERT(o->data.loop.arg_count);
	sll_arg_count_t l=o->data.loop.arg_count-1;
	sll_scope_t sc=o->data.loop.scope;
	o=_generate(o+1,g_dt);
	const sll_node_t* cnd=o;
	for (sll_arg_count_t i=0;i<l;i++){
		o=_mark_loop_delete(o,g_dt,lg_dt->variable_data,sc);
	}
	return cnd;
}



static void _generate_loop_end(assembly_generator_data_t* g_dt,assembly_loop_generator_data_t* lg_dt){
	g_dt->loop_data=lg_dt->last_loop_data;
	for (sll_variable_index_t i=0;i<g_dt->loop_variable_count;i++){
		bitmap_t v=*(lg_dt->variable_data+i);
		while (v){
			sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
			ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL;
			ai->data.variable=FIND_FIRST_SET_BIT(v)|(i<<6);
			v&=v-1;
		}
	}
	sll_deallocate(lg_dt->variable_data);
}



static const sll_node_t* _generate_on_stack(const sll_node_t* o,assembly_generator_data_t* g_dt){
	while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
		GENERATE_DEBUG_DATA(g_dt,o);
		o=(o->type==SLL_NODE_TYPE_CHANGE_STACK?o->data._next_node:o+1);
	}
	NOT_FIELD(o);
	switch (o->type){
		case SLL_NODE_TYPE_INT:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
				ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT;
				ai->data.int_=o->data.int_;
				PUSH;
				return o+1;
			}
		case SLL_NODE_TYPE_FLOAT:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
				ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT;
				ai->data.float_=o->data.float_;
				PUSH;
				return o+1;
			}
		case SLL_NODE_TYPE_CHAR:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
				ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR;
				ai->data.char_=o->data.char_;
				PUSH;
				return o+1;
			}
		case SLL_NODE_TYPE_COMPLEX:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
				ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_COMPLEX;
				ai->data.complex_=o->data.complex_;
				PUSH;
				return o+1;
			}
		case SLL_NODE_TYPE_STRING:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
				ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS;
				ai->data.string_index=o->data.string_index;
				PUSH;
				return o+1;
			}
		case SLL_NODE_TYPE_ARRAY:
			{
				if (!o->data.array_length){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO);
					PUSH;
					return o+1;
				}
				sll_array_length_t al=o->data.array_length;
				if (al>255){
					SLL_UNIMPLEMENTED();
				}
				o++;
				for (sll_array_length_t i=0;i<al;i++){
					o=_generate_on_stack(o,g_dt);
				}
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
				if (al==1){
					ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ONE;
				}
				else{
					ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK;
					ai->data.array_length=al;
					POP_N(al-1);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				if (!o->data.map_length){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP_ZERO);
					PUSH;
					return o+1;
				}
				sll_map_length_t ml=o->data.map_length;
				o++;
				for (sll_map_length_t i=0;i<ml;i++){
					o=_generate_on_stack(o,g_dt);
				}
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
				ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP;
				ai->data.map_length=ml;
				POP_N(ml-1);
				return o;
			}
		case SLL_NODE_TYPE_IDENTIFIER:
			_generate_identifier(o,g_dt);
			return o+1;
		case SLL_NODE_TYPE_FUNCTION_ID:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
				ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT;
				ai->data.int_=o->data.function_index+1;
				PUSH;
				return o+1;
			}
		case SLL_NODE_TYPE_PRINT:
		case SLL_NODE_TYPE_IF:
		case SLL_NODE_TYPE_SWITCH:
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_INC:
		case SLL_NODE_TYPE_DEC:
		case SLL_NODE_TYPE_RETURN:
		case SLL_NODE_TYPE_BREAK:
		case SLL_NODE_TYPE_CONTINUE:
		case SLL_NODE_TYPE_THREAD_WAIT:
		case SLL_NODE_TYPE_THREAD_LOCK:
		case SLL_NODE_TYPE_THREAD_SEMAPHORE:
		case SLL_NODE_TYPE_THREAD_BARRIER_EQ:
		case SLL_NODE_TYPE_THREAD_BARRIER_GEQ:
		case SLL_NODE_TYPE_THREAD_EXIT:
		case SLL_NODE_TYPE_OPERATION_LIST:
			o=_generate(o,g_dt);
			GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
			PUSH;
			return o;
		case SLL_NODE_TYPE_AND:
			{
				sll_arg_count_t l=o->data.arg_count;
				o++;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o;
				}
				if (l==1){
					return _generate_on_stack(o,g_dt);
				}
				assembly_instruction_label_t e=NEXT_LABEL(g_dt);
				while (1){
					l--;
					o=_generate_on_stack(o,g_dt);
					if (!l){
						break;
					}
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_DUP);
					GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ,e);
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO);
					POP;
				}
				DEFINE_LABEL(g_dt,e);
				PUSH;
				return o;
			}
		case SLL_NODE_TYPE_OR:
			{
				sll_arg_count_t l=o->data.arg_count;
				o++;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o;
				}
				if (l==1){
					return _generate_on_stack(o,g_dt);
				}
				assembly_instruction_label_t e=NEXT_LABEL(g_dt);
				while (1){
					l--;
					o=_generate_on_stack(o,g_dt);
					if (!l){
						break;
					}
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_DUP);
					GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ,e);
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO);
					POP;
				}
				DEFINE_LABEL(g_dt,e);
				PUSH;
				return o;
			}
		case SLL_NODE_TYPE_NOT:
			{
				sll_arg_count_t l=o->data.arg_count;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o+1;
				}
				o=_generate_on_stack(o+1,g_dt);
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT);
				l--;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_BOOL:
			{
				sll_arg_count_t l=o->data.arg_count;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o+1;
				}
				o=_generate_on_stack(o+1,g_dt);
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_BOOL);
				l--;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_ASSIGN:
			return _generate_assign(o,g_dt,1);
		case SLL_NODE_TYPE_FUNC:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
				ai->type=ASSEMBLY_INSTRUCTION_TYPE_DBG_FUNC;
				ASSEMBLY_INSTRUCTION_MISC_FIELD_SET(ai,o->data.function.function_index);
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
				ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT;
				ai->data.int_=(o->type==SLL_NODE_TYPE_FUNC?o->data.function.function_index+1:~((sll_integer_t)o->data.function.function_index));
				PUSH;
				return _skip_with_dbg(o,g_dt);
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC_LOAD:
			{
				sll_arg_count_t l=o->data.arg_count;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o+1;
				}
				o++;
				while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
					GENERATE_DEBUG_DATA(g_dt,o);
					o=(o->type==SLL_NODE_TYPE_CHANGE_STACK?o->data._next_node:o+1);
				}
				if (o->type==SLL_NODE_TYPE_STRING){
					sll_string_index_t i=o->data.string_index;
					o++;
					l--;
					while (l){
						l--;
						o=_generate(o,g_dt);
					}
					sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
					ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR;
					ai->data.string_index=i;
					PUSH;
					return o;
				}
				o=_generate_on_stack(o,g_dt);
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP);
				l--;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_INLINE_FUNC:
			return _generate_inline_function(o,g_dt);
		case SLL_NODE_TYPE_CALL:
			return _generate_call(o,g_dt);
		case SLL_NODE_TYPE_CALL_ARRAY:
			return _generate_call_array(o,g_dt);
		case SLL_NODE_TYPE_INLINE_IF:
			{
				sll_arg_count_t l=o->data.arg_count;
				o++;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o;
				}
				if (l==1){
					o=_generate(o,g_dt);
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o;
				}
				assembly_instruction_label_t e=NEXT_LABEL(g_dt);
				sll_bool_t end=l&1;
				l>>=1;
				while (l){
					l--;
					assembly_instruction_label_t nxt=NEXT_LABEL(g_dt);
					o=_generate_jump(o,g_dt,nxt,1);
					o=_generate_on_stack(o,g_dt);
					GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,e);
					DEFINE_LABEL(g_dt,nxt);
					POP;
				}
				if (end){
					o=_generate_on_stack(o,g_dt);
				}
				else{
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
				}
				DEFINE_LABEL(g_dt,e);
				return o;
			}
		case SLL_NODE_TYPE_BIT_NOT:
			{
				sll_arg_count_t l=o->data.arg_count;
				o++;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o;
				}
				o=_generate_on_stack(o,g_dt);
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_INV);
				l--;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_LESS:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE,1);
		case SLL_NODE_TYPE_LESS_EQUAL:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JA,1);
		case SLL_NODE_TYPE_EQUAL:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE,1);
		case SLL_NODE_TYPE_NOT_EQUAL:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JE,1);
		case SLL_NODE_TYPE_MORE:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE,1);
		case SLL_NODE_TYPE_MORE_EQUAL:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JB,1);
		case SLL_NODE_TYPE_STRICT_EQUAL:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JSNE,1);
		case SLL_NODE_TYPE_STRICT_NOT_EQUAL:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JSE,1);
		case SLL_NODE_TYPE_LENGTH:
			{
				sll_arg_count_t l=o->data.arg_count;
				o++;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o;
				}
				o=_generate_on_stack(o,g_dt);
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_LENGTH);
				l--;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_ACCESS:
			{
				sll_arg_count_t ac=o->data.arg_count;
				o++;
				if (!ac){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o;
				}
				sll_arg_count_t l=ac;
				if (l>4){
					l=4;
				}
				while (l){
					l--;
					o=_generate_on_stack(o,g_dt);
				}
				if (ac==1){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_COPY);
				}
				else if (ac==2){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS);
					POP;
				}
				else if (ac==3){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_TWO);
					POP2;
				}
				else{
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_THREE);
					POP3;
				}
				if (ac>4){
					ac-=4;
					while (ac){
						ac--;
						o=_generate(o,g_dt);
					}
				}
				return o;
			}
		case SLL_NODE_TYPE_DEEP_COPY:
			{
				sll_arg_count_t l=o->data.arg_count;
				o++;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o;
				}
				o=_generate_on_stack(o,g_dt);
				l--;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_DEEP_COPY);
				return o;
			}
		case SLL_NODE_TYPE_VAR_ACCESS:
			{
				sll_arg_count_t l=o->data.arg_count;
				SLL_ASSERT(l>1);
				o++;
				while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
					GENERATE_DEBUG_DATA(g_dt,o);
					o=(o->type==SLL_NODE_TYPE_CHANGE_STACK?o->data._next_node:o+1);
				}
				SLL_ASSERT(o->type==SLL_NODE_TYPE_IDENTIFIER);
				sll_variable_index_t vi=GET_VARIABLE_INDEX(o,g_dt);
				o++;
				l--;
				sll_arg_count_t vl=l;
				do{
					while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
						GENERATE_DEBUG_DATA(g_dt,o);
						o=(o->type==SLL_NODE_TYPE_CHANGE_STACK?o->data._next_node:o+1);
					}
					SLL_ASSERT(o->type==SLL_NODE_TYPE_FIELD);
					sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
					ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS;
					ai->data.string_index=o->data.string_index;
					o++;
					l--;
				} while (l);
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
				ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_VAR;
				ai->data.variable_access.variable=vi;
				ai->data.variable_access.arg_count=vl;
				PUSH;
				return o;
			}
		case SLL_NODE_TYPE_HAS:
			{
				if (!o->data.arg_count){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o+1;
				}
				o=_generate_two_on_stack(o+1,g_dt,o->data.arg_count);
				assembly_instruction_label_t one=NEXT_LABEL(g_dt);
				assembly_instruction_label_t end=NEXT_LABEL(g_dt);
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JI,one);
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO);
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,end);
				DEFINE_LABEL(g_dt,one);
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE);
				DEFINE_LABEL(g_dt,end);
				POP;
				return o;
			}
		case SLL_NODE_TYPE_CAST:
			{
				sll_arg_count_t l=o->data.arg_count;
				o++;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o;
				}
				if (l==1){
					return _generate_on_stack(o,g_dt);
				}
				o=_generate_on_stack(o,g_dt);
				l--;
				while (l){
					l--;
					while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
						GENERATE_DEBUG_DATA(g_dt,o);
						o=(o->type==SLL_NODE_TYPE_CHANGE_STACK?o->data._next_node:o+1);
					}
					if (o->type==SLL_NODE_TYPE_INT&&o->data.int_>0&&o->data.int_<=SLL_MAX_OBJECT_TYPE){
						sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
						ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE;
						ai->data.type=(sll_object_type_t)(o->data.int_);
						o++;
					}
					else{
						o=_generate_on_stack(o,g_dt);
						GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST);
						POP;
					}
				}
				return o;
			}
		case SLL_NODE_TYPE_TYPEOF:
		case SLL_NODE_TYPE_NAMEOF:
		case SLL_NODE_TYPE_NAMEOF_TYPE:
			{
				sll_assembly_instruction_type_t ai_t=o->type-SLL_NODE_TYPE_TYPEOF+SLL_ASSEMBLY_INSTRUCTION_TYPE_TYPEOF;
				sll_arg_count_t l=o->data.arg_count;
				o++;
				if (!l){
					GENERATE_OPCODE(g_dt,(o->type==SLL_NODE_TYPE_TYPEOF?SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO:SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_EMPTY_STRING));
					PUSH;
					return o;
				}
				o=_generate_on_stack(o,g_dt);
				GENERATE_OPCODE(g_dt,ai_t);
				l--;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_DECL:
			{
				sll_arg_count_t l=o->data.declaration.arg_count;
				sll_assembly_instruction_type_t fl=SLL_ASSEMBLY_INSTRUCTION_FLAG_ANONYMOUS;
				if (o->data.declaration.name_string_index!=SLL_MAX_STRING_INDEX){
					sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
					ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS;
					ai->data.type=o->data.declaration.name_string_index;
					PUSH;
					fl=0;
				}
				o++;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL_ZERO|fl);
					if (!fl){
						POP;
					}
					PUSH;
					return o;
				}
				sll_arg_count_t v=l;
				l>>=1;
				while (l){
					l--;
					o=_generate_on_stack(o,g_dt);
					while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
						GENERATE_DEBUG_DATA(g_dt,o);
						o=(o->type==SLL_NODE_TYPE_CHANGE_STACK?o->data._next_node:o+1);
					}
					SLL_ASSERT(o->type==SLL_NODE_TYPE_FIELD);
					sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
					ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS;
					ai->data.string_index=o->data.string_index;
					PUSH;
					o++;
				}
				if (v&1){
					o=_generate(o,g_dt);
				}
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
				ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL|fl;
				ai->data.arg_count=v>>1;
				if (!fl){
					POP;
				}
				POP_N(ai->data.arg_count*2-1);
				return o;
			}
		case SLL_NODE_TYPE_NEW:
			{
				sll_arg_count_t l=o->data.arg_count;
				o++;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o;
				}
				sll_arg_count_t v=l-1;
				while (l){
					l--;
					o=_generate_on_stack(o,g_dt);
				}
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
				ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW;
				ai->data.arg_count=v;
				POP_N(v);
				return o;
			}
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			{
				GENERATE_OPCODE(g_dt,(o->type>=SLL_NODE_TYPE_FOR_MAP?SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP_ZERO:SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO));
				PUSH;
				sll_arg_count_t l=o->data.loop.arg_count;
				if (!l){
					return o+1;
				}
				if (l==1){
					return _generate(o+1,g_dt);
				}
				assembly_loop_generator_data_t lg_dt;
				assembly_instruction_label_t s=NEXT_LABEL(g_dt);
				assembly_instruction_label_t c=NEXT_LABEL(g_dt);
				assembly_instruction_label_t e=NEXT_LABEL(g_dt);
				const sll_node_t* cnd=_generate_loop_start(g_dt,o,&lg_dt,c,e);
				const sll_node_t* r=o;
				o=(r->type==SLL_NODE_TYPE_FOR_ARRAY||r->type==SLL_NODE_TYPE_FOR_MAP?_generate_jump(cnd,g_dt,e,1):sll_skip_node_const(cnd));
				DEFINE_LABEL(g_dt,s);
				l-=2;
				sll_bool_t extra=(l==(r->type>=SLL_NODE_TYPE_FOR_MAP));
				sll_arg_count_t end=(r->type>=SLL_NODE_TYPE_FOR_MAP);
				while (l){
					l--;
					if (l<=end){
						o=_generate_on_stack(o,g_dt);
					}
					else{
						o=_generate(o,g_dt);
					}
				}
				if (extra){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
				}
				if (r->type>=SLL_NODE_TYPE_FOR_MAP){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN);
					POP2;
				}
				else{
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ONE);
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD);
				}
				DEFINE_LABEL(g_dt,c);
				_generate_jump(cnd,g_dt,s,0);
				DEFINE_LABEL(g_dt,e);
				_generate_loop_end(g_dt,&lg_dt);
				return o;
			}
		case SLL_NODE_TYPE_REF:
		case SLL_NODE_TYPE_READ_BLOCKING_CHAR:
			{
				sll_assembly_instruction_type_t ai_t=(o->type==SLL_NODE_TYPE_REF?SLL_ASSEMBLY_INSTRUCTION_TYPE_REF:SLL_ASSEMBLY_INSTRUCTION_TYPE_READ_BLOCKING_CHAR);
				sll_arg_count_t l=o->data.arg_count;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o+1;
				}
				o=_generate_on_stack(o+1,g_dt);
				GENERATE_OPCODE(g_dt,ai_t);
				l--;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_COMMA:
			{
				sll_arg_count_t l=o->data.arg_count;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o+1;
				}
				l--;
				o++;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				return _generate_on_stack(o,g_dt);
			}
		case SLL_NODE_TYPE_THREAD_ID:
			{
				sll_arg_count_t l=o->data.arg_count;
				o++;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_ID);
				return o;
			}
		case SLL_NODE_TYPE_READ_BLOCKING:
			{
				if (!o->data.arg_count){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o+1;
				}
				o=_generate_two_on_stack(o+1,g_dt,o->data.arg_count);
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_READ_BLOCKING);
				POP;
				return o;
			}
	}
	sll_arg_count_t l=o->data.arg_count;
	sll_assembly_instruction_type_t t=o->type+(SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD-SLL_NODE_TYPE_ADD);
	o++;
	if (!l){
		GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
		PUSH;
		return o+1;
	}
	if (l==1){
		return _generate_on_stack(o,g_dt);
	}
	o=_generate_on_stack(o,g_dt);
	l--;
	while (l){
		l--;
		o=_generate_on_stack(o,g_dt);
		GENERATE_OPCODE(g_dt,t);
		POP;
	}
	return o;
}



static const sll_node_t* _generate(const sll_node_t* o,assembly_generator_data_t* g_dt){
	while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
		GENERATE_DEBUG_DATA(g_dt,o);
		o=(o->type==SLL_NODE_TYPE_CHANGE_STACK?o->data._next_node:o+1);
	}
	switch (o->type){
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_COMPLEX:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t al=o->data.array_length;
				o++;
				while (al){
					o=_generate(o,g_dt);
					al--;
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t ml=o->data.map_length;
				o++;
				while (ml){
					o=_generate(o,g_dt);
					ml--;
				}
				return o;
			}
		case SLL_NODE_TYPE_IDENTIFIER:
			{
				if (SLL_IDENTIFIER_GET_ARRAY_ID(o->data.identifier_index)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
					if (*(g_dt->identifier_remove_data.long_+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->data.identifier_index))==o){
						sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
						ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL;
						ai->data.variable=(g_dt->identifier_map.long_identifier_map+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->data.identifier_index))->variable;
					}
				}
				else{
					if (*(g_dt->identifier_remove_data.short_[SLL_IDENTIFIER_GET_ARRAY_ID(o->data.identifier_index)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->data.identifier_index))==o){
						sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
						ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL;
						ai->data.variable=(g_dt->identifier_map.short_identifier_map[SLL_IDENTIFIER_GET_ARRAY_ID(o->data.identifier_index)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->data.identifier_index))->variable;
					}
				}
				return o+1;
			}
		case SLL_NODE_TYPE_PRINT:
			{
				sll_arg_count_t l=o->data.arg_count;
				o++;
				while (l){
					l--;
					o=_generate_on_stack(o,g_dt);
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT);
					POP;
				}
				return o;
			}
		case SLL_NODE_TYPE_AND:
			{
				sll_arg_count_t l=o->data.arg_count;
				o++;
				if (!l){
					return o;
				}
				assembly_instruction_label_t end=NEXT_LABEL(g_dt);
				l--;
				while (l){
					l--;
					o=_generate_on_stack(o,g_dt);
					POP;
					GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ,end);
				}
				o=_generate(o,g_dt);
				DEFINE_LABEL(g_dt,end);
				return o;
			}
		case SLL_NODE_TYPE_OR:
			{
				sll_arg_count_t l=o->data.arg_count;
				o++;
				if (!l){
					return o;
				}
				assembly_instruction_label_t end=NEXT_LABEL(g_dt);
				l--;
				while (l){
					l--;
					o=_generate_on_stack(o,g_dt);
					POP;
					GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ,end);
				}
				o=_generate(o,g_dt);
				DEFINE_LABEL(g_dt,end);
				return o;
			}
		case SLL_NODE_TYPE_ASSIGN:
			return _generate_assign(o,g_dt,0);
		case SLL_NODE_TYPE_FUNC:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
				ai->type=ASSEMBLY_INSTRUCTION_TYPE_DBG_FUNC;
				ASSEMBLY_INSTRUCTION_MISC_FIELD_SET(ai,o->data.function.function_index);
				return _skip_with_dbg(o,g_dt);
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->data.function.arg_count;
				o++;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC_LOAD:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_INLINE_FUNC:
			o=_generate_inline_function(o,g_dt);
			GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
			POP;
			return o;
		case SLL_NODE_TYPE_CALL:
			o=_generate_call(o,g_dt);
			GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
			POP;
			return o;
		case SLL_NODE_TYPE_CALL_ARRAY:
			o=_generate_call_array(o,g_dt);
			GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
			POP;
			return o;
		case SLL_NODE_TYPE_IF:
		case SLL_NODE_TYPE_INLINE_IF:
			{
				sll_arg_count_t l=o->data.arg_count;
				o++;
				if (!l){
					return o;
				}
				if (l==1){
					return _generate(o,g_dt);
				}
				if (l&1){
					assembly_instruction_label_t e=NEXT_LABEL(g_dt);
					l>>=1;
					do{
						l--;
						assembly_instruction_label_t nxt=NEXT_LABEL(g_dt);
						o=_generate_jump(o,g_dt,nxt,1);
						o=_generate(o,g_dt);
						GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,e);
						DEFINE_LABEL(g_dt,nxt);
					} while (l);
					o=_generate(o,g_dt);
					DEFINE_LABEL(g_dt,e);
				}
				else{
					assembly_instruction_label_t e=NEXT_LABEL(g_dt);
					l>>=1;
					do{
						l--;
						assembly_instruction_label_t nxt=NEXT_LABEL(g_dt);
						o=_generate_jump(o,g_dt,nxt,1);
						o=_generate(o,g_dt);
						if (l){
							GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,e);
						}
						DEFINE_LABEL(g_dt,nxt);
					} while (l);
					DEFINE_LABEL(g_dt,e);
				}
				return o;
			}
		case SLL_NODE_TYPE_SWITCH:
			{
				sll_arg_count_t l=o->data.arg_count;
				const sll_node_t* r=o;
				sll_bool_t d=!(o->data.arg_count&1);
				o++;
				if (!l){
					return o;
				}
				if (l==1){
					return _generate(o,g_dt);
				}
				o=_generate_on_stack(o,g_dt);
				l=(l-1)>>1;
				node_label_t* m=sll_allocate((l+1)*sizeof(node_label_t));
				sll_arg_count_t i=0;
				while (l){
					l--;
					assembly_instruction_label_t j=NEXT_LABEL(g_dt);
					o=_generate_on_stack(o,g_dt);
					GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL,j);
					PUSH;
					(m+i)->node=o;
					(m+i)->label=j;
					i++;
					o=sll_skip_node_const(o);
				}
				assembly_instruction_label_t e=NEXT_LABEL(g_dt);
				if (d){
					assembly_instruction_label_t j=NEXT_LABEL(g_dt);
					GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL,j);
					PUSH;
					(m+i)->node=o;
					(m+i)->label=j;
				}
				else{
					GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL,e);
					PUSH;
					(m+i)->node=NULL;
					(m+i)->label=e;
				}
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
				ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_JT;
				ai->data.array_length=i;
				POP_N(i*2+1);
				o=r+1;
				for (sll_arg_count_t j=0;j<=i;j++){
					if ((m+j)->node){
						DEFINE_LABEL(g_dt,(m+j)->label);
						o=_generate((m+j)->node,g_dt);
						GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,e);
					}
				}
				sll_deallocate(m);
				DEFINE_LABEL(g_dt,e);
				return o;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_WHILE_MAP:
			{
				sll_arg_count_t l=o->data.loop.arg_count;
				if (!l){
					return o+1;
				}
				if (l==1){
					return _generate(o+1,g_dt);
				}
				assembly_instruction_label_t s=NEXT_LABEL(g_dt);
				assembly_instruction_label_t c=NEXT_LABEL(g_dt);
				assembly_instruction_label_t e=NEXT_LABEL(g_dt);
				assembly_loop_generator_data_t lg_dt;
				const sll_node_t* cnd=_generate_loop_start(g_dt,o,&lg_dt,c,e);
				o=(o->type>=SLL_NODE_TYPE_WHILE?sll_skip_node_const(cnd):_generate_jump(cnd,g_dt,e,1));
				DEFINE_LABEL(g_dt,s);
				l-=2;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				DEFINE_LABEL(g_dt,c);
				_generate_jump(cnd,g_dt,s,0);
				DEFINE_LABEL(g_dt,e);
				_generate_loop_end(g_dt,&lg_dt);
				return o;
			}
		case SLL_NODE_TYPE_LOOP:
			{
				sll_arg_count_t l=o->data.loop.arg_count;
				o++;
				assembly_instruction_label_t s=NEXT_LABEL(g_dt);
				if (!l){
					DEFINE_LABEL(g_dt,s);
					GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,s);
					return o;
				}
				assembly_instruction_label_t e=NEXT_LABEL(g_dt);
				assembly_loop_generator_data_t lg_dt;
				_init_loop_data(g_dt,&lg_dt,s,e);
				DEFINE_LABEL(g_dt,s);
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,s);
				DEFINE_LABEL(g_dt,e);
				_generate_loop_end(g_dt,&lg_dt);
				return o;
			}
		case SLL_NODE_TYPE_INC:
		case SLL_NODE_TYPE_DEC:
			{
				sll_assembly_instruction_type_t ai_t=(o->type==SLL_NODE_TYPE_INC?SLL_ASSEMBLY_INSTRUCTION_TYPE_INC:SLL_ASSEMBLY_INSTRUCTION_TYPE_DEC);
				sll_arg_count_t l=o->data.arg_count;
				o++;
				if (!l){
					return o;
				}
				while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
					GENERATE_DEBUG_DATA(g_dt,o);
					o=(o->type==SLL_NODE_TYPE_CHANGE_STACK?o->data._next_node:o+1);
				}
				if (o->type==SLL_NODE_TYPE_IDENTIFIER){
					sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
					ai->type=ai_t|SLL_ASSEMBLY_INSTRUCTION_FLAG_INPLACE;
					if (SLL_IDENTIFIER_GET_ARRAY_ID(o->data.identifier_index)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
						if (*(g_dt->identifier_remove_data.long_+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->data.identifier_index))==o){
							SLL_UNIMPLEMENTED();
						}
						ai->data.variable=(g_dt->identifier_map.long_identifier_map+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->data.identifier_index))->variable;
					}
					else{
						if (*(g_dt->identifier_remove_data.short_[SLL_IDENTIFIER_GET_ARRAY_ID(o->data.identifier_index)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->data.identifier_index))==o){
							SLL_UNIMPLEMENTED();
						}
						ai->data.variable=(g_dt->identifier_map.short_identifier_map[SLL_IDENTIFIER_GET_ARRAY_ID(o->data.identifier_index)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->data.identifier_index))->variable;
					}
					l--;
					o++;
				}
				else if (o->type==SLL_NODE_TYPE_ACCESS){
					SLL_UNIMPLEMENTED();
				}
				else if (o->type==SLL_NODE_TYPE_VAR_ACCESS){
					SLL_UNIMPLEMENTED();
				}
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_LESS:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE,0);
		case SLL_NODE_TYPE_LESS_EQUAL:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JA,0);
		case SLL_NODE_TYPE_EQUAL:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE,0);
		case SLL_NODE_TYPE_NOT_EQUAL:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JE,0);
		case SLL_NODE_TYPE_MORE:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE,0);
		case SLL_NODE_TYPE_MORE_EQUAL:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JB,0);
		case SLL_NODE_TYPE_STRICT_EQUAL:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JSNE,0);
		case SLL_NODE_TYPE_STRICT_NOT_EQUAL:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JSE,0);
		case SLL_NODE_TYPE_BREAK:
			{
				sll_arg_count_t l=o->data.arg_count;
				o++;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				if (g_dt->loop_data.break_==MAX_ASSEMBLY_INSTRUCTION_LABEL){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO);
					PUSH;
				}
				else{
					GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,g_dt->loop_data.break_);
				}
				return o;
			}
		case SLL_NODE_TYPE_CONTINUE:
			{
				sll_arg_count_t l=o->data.arg_count;
				o++;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				if (g_dt->loop_data.continue_==MAX_ASSEMBLY_INSTRUCTION_LABEL){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO);
					PUSH;
				}
				else{
					GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,g_dt->loop_data.continue_);
				}
				return o;
			}
		case SLL_NODE_TYPE_RETURN:
			{
				sll_arg_count_t l=o->data.arg_count;
				o++;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
				}
				else{
					o=_generate_on_stack(o,g_dt);
					l--;
					while (l){
						l--;
						o=_generate(o,g_dt);
					}
				}
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->assembly_data);
				if (g_dt->return_label==MAX_ASSEMBLY_INSTRUCTION_LABEL){
					ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET;
				}
				else{
					ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP|ASSEMBLY_INSTRUCTION_FLAG_LABEL;
					ASSEMBLY_INSTRUCTION_MISC_FIELD_SET(ai,g_dt->return_label);
				}
				return o;
			}
		case SLL_NODE_TYPE_THREAD_WAIT:
		case SLL_NODE_TYPE_THREAD_LOCK:
		case SLL_NODE_TYPE_THREAD_SEMAPHORE:
		case SLL_NODE_TYPE_THREAD_EXIT:
			{
				sll_assembly_instruction_type_t ai_t=(o->type-SLL_NODE_TYPE_THREAD_WAIT+SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_WAIT);
				sll_arg_count_t l=o->data.arg_count;
				o++;
				if (!l){
					return o;
				}
				o=_generate_on_stack(o,g_dt);
				l--;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				GENERATE_OPCODE(g_dt,ai_t);
				POP;
				return o;
			}
		case SLL_NODE_TYPE_THREAD_BARRIER_EQ:
		case SLL_NODE_TYPE_THREAD_BARRIER_GEQ:
		case SLL_NODE_TYPE_READ_BLOCKING:
			{
				sll_assembly_instruction_type_t ai_t=o->type-SLL_NODE_TYPE_THREAD_BARRIER_EQ+SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_BARRIER_EQ;
				sll_arg_count_t l=o->data.arg_count;
				o++;
				if (!l){
					return o;
				}
				if (l==1){
					return _generate(o,g_dt);
				}
				o=_generate_on_stack(_generate_on_stack(o,g_dt),g_dt);
				l-=2;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				GENERATE_OPCODE(g_dt,ai_t);
				POP2;
				return o;
			}
		case SLL_NODE_TYPE_READ_BLOCKING_CHAR:
			{
				sll_arg_count_t l=o->data.arg_count;
				o++;
				if (!l){
					return o;
				}
				o=_generate_on_stack(o,g_dt);
				l--;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_READ_BLOCKING_CHAR);
				POP;
				return o;
			}
	}
	sll_arg_count_t l=o->data.arg_count;
	o++;
	while (l){
		l--;
		o=_generate(o,g_dt);
	}
	return o;
}



__SLL_EXTERNAL void sll_free_assembly_data(sll_assembly_data_t* assembly_data){
	assembly_data->time=0;
	assembly_data->first_instruction=NULL;
	assembly_data->instruction_count=0;
	assembly_data->variable_count=0;
	sll_deallocate(assembly_data->function_table.data);
	assembly_data->function_table.data=NULL;
	assembly_data->function_table.length=0;
	for (sll_string_index_t i=0;i<assembly_data->string_table.length;i++){
		sll_free_string(assembly_data->string_table.data+i);
	}
	sll_deallocate(assembly_data->string_table.data);
	assembly_data->string_table.data=NULL;
	assembly_data->string_table.length=0;
	sll_deallocate(assembly_data->debug_data.data);
	assembly_data->debug_data.data=NULL;
	assembly_data->debug_data.length=0;
	assembly_stack_page_t* pg=assembly_data->_instruction_stack.start;
	while (pg){
		assembly_stack_page_t* n=pg->next;
		SLL_CRITICAL_ERROR(sll_platform_free_page(pg,SLL_ROUND_PAGE(ASSEMBLY_INSTRUCTION_STACK_ALLOC_SIZE)));
		pg=n;
	}
	assembly_data->_instruction_stack.start=NULL;
	assembly_data->_instruction_stack.end=NULL;
	assembly_data->_instruction_stack.count=0;
	assembly_data->_instruction_stack.next_instruction=NULL;
}



__SLL_EXTERNAL void sll_generate_assembly(const sll_source_file_t* source_file,sll_assembly_data_t* out){
	_init_assembly_stack(out);
	out->time=sll_platform_get_current_time();
	out->first_instruction=out->_instruction_stack.next_instruction;
	out->instruction_count=0;
	out->variable_count=0;
	out->tls_variable_count=0;
	if (!source_file->first_node){
		_acquire_next_instruction(out)->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO;
		out->function_table.length=0;
		out->function_table.data=NULL;
		out->string_table.length=0;
		out->string_table.data=NULL;
		return;
	}
	out->string_table.length=source_file->string_table.length;
	out->string_table.data=sll_allocate(out->string_table.length*sizeof(sll_string_t));
	for (sll_string_index_t i=0;i<out->string_table.length;i++){
		sll_string_clone(source_file->string_table.data+i,out->string_table.data+i);
	}
	assembly_generator_data_t g_dt={
		out,
		source_file,
		{
			.long_identifier_map=sll_allocate(source_file->identifier_table.long_data_length*sizeof(identifier_data_t)),
			.variable_count=0,
			.tls_variable_count=0
		},
		0,
		{
			.long_=sll_allocate(source_file->identifier_table.long_data_length*sizeof(void*))
		},
		{
			MAX_ASSEMBLY_INSTRUCTION_LABEL,
			MAX_ASSEMBLY_INSTRUCTION_LABEL
		},
		MAX_ASSEMBLY_INSTRUCTION_LABEL,
		0
	};
	for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		g_dt.identifier_map.short_identifier_map[i]=sll_allocate(source_file->identifier_table.short_[i].length*sizeof(identifier_data_t));
		g_dt.identifier_remove_data.short_[i]=sll_allocate(source_file->identifier_table.short_[i].length*sizeof(void*));
		for (sll_identifier_list_length_t j=0;j<source_file->identifier_table.short_[i].length;j++){
			(g_dt.identifier_map.short_identifier_map[i]+j)->variable=SLL_MAX_VARIABLE_INDEX;
			*(g_dt.identifier_remove_data.short_[i]+j)=NULL;
		}
	}
	for (sll_identifier_list_length_t i=0;i<source_file->identifier_table.long_data_length;i++){
		(g_dt.identifier_map.long_identifier_map+i)->variable=SLL_MAX_VARIABLE_INDEX;
		*(g_dt.identifier_remove_data.long_+i)=NULL;
	}
	_map_identifiers(source_file->first_node,source_file,&g_dt,SLL_MAX_SCOPE);
	sll_variable_index_t g_vc=g_dt.identifier_map.variable_count;
	sll_variable_index_t mx_vc=g_vc;
	sll_variable_index_t* fn_vc=sll_allocate_stack(source_file->function_table.length*sizeof(sll_variable_index_t));
	for (sll_function_index_t i=0;i<source_file->function_table.length;i++){
		g_dt.identifier_map.variable_count=g_vc;
		const sll_function_t* k=*(source_file->function_table.data+i);
		for (sll_arg_count_t j=0;j<SLL_FUNCTION_GET_ARGUMENT_COUNT(k);j++){
			sll_identifier_index_t ii=k->args[j];
			if (SLL_IDENTIFIER_GET_ARRAY_ID(ii)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
				(g_dt.identifier_map.long_identifier_map+SLL_IDENTIFIER_GET_ARRAY_INDEX(ii))->variable=g_dt.identifier_map.variable_count<<1;
			}
			else{
				(g_dt.identifier_map.short_identifier_map[SLL_IDENTIFIER_GET_ARRAY_ID(ii)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(ii))->variable=g_dt.identifier_map.variable_count<<1;
			}
			g_dt.identifier_map.variable_count++;
		}
		g_dt.stack_offset=0;
		const sll_node_t* fo=_get_node_at_offset(source_file,k->offset);
		SLL_ASSERT(fo->type==SLL_NODE_TYPE_FUNC);
		sll_arg_count_t ac=fo->data.function.arg_count;
		sll_scope_t sc=fo->data.function.scope;
		fo++;
		while (ac){
			ac--;
			fo=_map_identifiers(fo,source_file,&g_dt,sc);
		}
		*(fn_vc+i)=g_dt.identifier_map.variable_count-g_vc-SLL_FUNCTION_GET_ARGUMENT_COUNT(k);
		if (g_dt.identifier_map.variable_count>mx_vc){
			mx_vc=g_dt.identifier_map.variable_count;
		}
	}
	out->variable_count=g_vc;
	out->tls_variable_count=g_dt.identifier_map.tls_variable_count;
	g_dt.loop_variable_count=(out->tls_variable_count<<1)+1;
	if (g_dt.loop_variable_count<(mx_vc<<1)){
		g_dt.loop_variable_count=mx_vc<<1;
	}
	g_dt.loop_variable_count=(g_dt.loop_variable_count+63)>>6;
	g_dt.stack_offset=0;
	_generate(source_file->first_node,&g_dt);
	SLL_ASSERT(g_dt.return_label==MAX_ASSEMBLY_INSTRUCTION_LABEL);
	GENERATE_OPCODE(&g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO);
	out->function_table.length=source_file->function_table.length;
	out->function_table.data=sll_allocate(source_file->function_table.length*sizeof(sll_assembly_function_t));
	sll_function_index_t fn_n=0;
	sll_string_index_t* assembly_function_names=sll_allocate_stack(source_file->function_table.length*sizeof(sll_string_index_t));
	for (sll_function_index_t i=0;i<source_file->function_table.length;i++){
		const sll_function_t* k=*(source_file->function_table.data+i);
		sll_assembly_instruction_t* ai=_acquire_next_instruction(out);
		ai->type=ASSEMBLY_INSTRUCTION_TYPE_FUNC_START;
		ASSEMBLY_INSTRUCTION_MISC_FIELD_SET(ai,i);
		(out->function_table.data+i)->arg_count=k->arg_count;
		if (k->name_string_index==SLL_MAX_STRING_INDEX){
			sll_string_t str;
			sll_string_format(SLL_CHAR("@%u"),&str,fn_n);
			fn_n++;
			*(assembly_function_names+i)=sll_add_string(&(out->string_table),&str);
		}
		else{
			*(assembly_function_names+i)=k->name_string_index;
		}
		if (*(fn_vc+i)){
			ai=_acquire_next_instruction(out);
			ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_STACK;
			ai->data.stack_offset=*(fn_vc+i);
		}
		const sll_node_t* fo=_get_node_at_offset(source_file,k->offset);
		SLL_ASSERT(fo->type==SLL_NODE_TYPE_FUNC);
		sll_arg_count_t ac=fo->data.function.arg_count;
		g_dt.stack_offset=0;
		fo++;
		while (ac){
			ac--;
			fo=_generate(fo,&g_dt);
		}
		GENERATE_OPCODE(&g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO);
	}
	sll_deallocate(fn_vc);
	for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		sll_deallocate(g_dt.identifier_map.short_identifier_map[i]);
		sll_deallocate(g_dt.identifier_remove_data.short_[i]);
	}
	sll_deallocate(g_dt.identifier_map.long_identifier_map);
	sll_deallocate(g_dt.identifier_remove_data.long_);
	sll_assembly_instruction_t* ai=out->first_instruction;
	sll_assembly_instruction_t* stack[5]={
		&_assembly_nop,
		&_assembly_nop,
		&_assembly_nop,
		&_assembly_nop,
		&_assembly_nop
	};
	out->debug_data.length=0;
	for (sll_instruction_index_t i=0;i<out->instruction_count;i++){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==ASSEMBLY_INSTRUCTION_TYPE_NOP||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==ASSEMBLY_INSTRUCTION_TYPE_DBG||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==ASSEMBLY_INSTRUCTION_TYPE_DBG_FUNC||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
				ai=ai->data._next_instruction;
			}
			else{
				if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==ASSEMBLY_INSTRUCTION_TYPE_DBG){
					out->debug_data.length++;
				}
				else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==ASSEMBLY_INSTRUCTION_TYPE_DBG_FUNC){
					out->debug_data.length+=2;
				}
				ai++;
			}
			continue;
		}
		stack[4]=stack[3];
		stack[3]=stack[2];
		stack[2]=stack[1];
		stack[1]=stack[0];
		stack[0]=ai;
		_optimize_assembly(out,stack,&_assembly_nop);
		ai++;
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			ai=ai->data._next_instruction;
		}
	}
	strint_map_data_t sm={
		(out->string_table.length>>6)+1,
		sll_zero_allocate(((out->string_table.length>>6)+1)*sizeof(bitmap_t))
	};
	for (sll_function_index_t i=0;i<out->function_table.length;i++){
		sll_string_index_t j=*(assembly_function_names+i);
		*(sm.map_data+(j>>6))|=1ull<<(j&63);
	}
	ai=out->first_instruction;
	out->debug_data.data=sll_allocate(out->debug_data.length*sizeof(sll_debug_line_data_t));
	sll_debug_data_length_t dbg_i=0;
	sll_file_offset_t f_off=0;
	sll_string_index_t f_idx=0;
	sll_instruction_index_t l_dbg_ii=0;
	sll_assembly_instruction_t* src_ai=ai;
	sll_instruction_index_t* lbl=sll_allocate(g_dt.next_label*sizeof(sll_instruction_index_t));
	file_line_t* fn_ln=sll_allocate(source_file->function_table.length*sizeof(file_line_t));
	{
		sll_instruction_index_t i=0;
		while (i<out->instruction_count){
			if (src_ai->type==ASSEMBLY_INSTRUCTION_TYPE_NOP){
_remove_nop:
				src_ai++;
				if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(src_ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
					src_ai=src_ai->data._next_instruction;
				}
				out->instruction_count--;
				continue;
			}
			*ai=*src_ai;
			if (ai->type==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS||ai->type==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR||ai->type==SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR||ai->type==SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR){
				*(sm.map_data+(ai->data.string_index>>6))|=1ull<<(ai->data.string_index&63);
			}
			else if (ai->type==ASSEMBLY_INSTRUCTION_TYPE_DBG){
				if (ai->data.string_index==SLL_MAX_STRING_INDEX){
					f_off++;
					if (i!=l_dbg_ii||(dbg_i&&((out->debug_data.data+dbg_i-1)->line&SLL_DEBUG_LINE_DATA_FLAG_FILE))){
						(out->debug_data.data+dbg_i)->delta_instruction_index=i-l_dbg_ii;
						SLL_DEBUG_LINE_DATA_SET_DATA(out->debug_data.data+dbg_i,f_off,0);
						dbg_i++;
					}
				}
				else if (f_idx!=ai->data.string_index){
					(out->debug_data.data+dbg_i)->delta_instruction_index=i-l_dbg_ii;
					SLL_DEBUG_LINE_DATA_SET_DATA(out->debug_data.data+dbg_i,ai->data.string_index,SLL_DEBUG_LINE_DATA_FLAG_FILE);
					*(sm.map_data+(ai->data.string_index>>6))|=1ull<<(ai->data.string_index&63);
					f_off=0;
					f_idx=ai->data.string_index;
					dbg_i++;
				}
				l_dbg_ii=i;
				goto _remove_nop;
			}
			else if (ai->type==ASSEMBLY_INSTRUCTION_TYPE_DBG_FUNC){
				(fn_ln+ASSEMBLY_INSTRUCTION_MISC_FIELD_GET(ai))->line=f_off;
				(fn_ln+ASSEMBLY_INSTRUCTION_MISC_FIELD_GET(ai))->file_path_string_index=f_idx;
				goto _remove_nop;
			}
			else if (ai->type==ASSEMBLY_INSTRUCTION_TYPE_FUNC_START){
				(out->function_table.data+ASSEMBLY_INSTRUCTION_MISC_FIELD_GET(ai))->instruction_index=i;
				if (f_idx!=(fn_ln+ASSEMBLY_INSTRUCTION_MISC_FIELD_GET(ai))->file_path_string_index){
					f_idx=(fn_ln+ASSEMBLY_INSTRUCTION_MISC_FIELD_GET(ai))->file_path_string_index;
					(out->debug_data.data+dbg_i)->delta_instruction_index=i-l_dbg_ii;
					SLL_DEBUG_LINE_DATA_SET_DATA(out->debug_data.data+dbg_i,f_idx,SLL_DEBUG_LINE_DATA_FLAG_FILE);
					dbg_i++;
					l_dbg_ii=i;
				}
				(out->debug_data.data+dbg_i)->delta_instruction_index=i-l_dbg_ii;
				SLL_DEBUG_LINE_DATA_SET_DATA(out->debug_data.data+dbg_i,*(assembly_function_names+ASSEMBLY_INSTRUCTION_MISC_FIELD_GET(ai)),SLL_DEBUG_LINE_DATA_FLAG_FUNC);
				dbg_i++;
				f_off=(fn_ln+ASSEMBLY_INSTRUCTION_MISC_FIELD_GET(ai))->line;
				(out->debug_data.data+dbg_i)->delta_instruction_index=0;
				SLL_DEBUG_LINE_DATA_SET_DATA(out->debug_data.data+dbg_i,f_off,0);
				dbg_i++;
				l_dbg_ii=i;
				goto _remove_nop;
			}
			else if (ai->type==ASSEMBLY_INSTRUCTION_TYPE_LABEL_TARGET){
				sll_assembly_instruction_t* tmp=src_ai+1;
				while (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(tmp)==ASSEMBLY_INSTRUCTION_TYPE_NOP||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(tmp)==ASSEMBLY_INSTRUCTION_TYPE_DBG||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(tmp)==ASSEMBLY_INSTRUCTION_TYPE_DBG_FUNC||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(tmp)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
					tmp=(SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(tmp)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK?tmp->data._next_instruction:tmp+1);
				}
				if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(tmp)==SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP&&(tmp->type&ASSEMBLY_INSTRUCTION_FLAG_LABEL)){
					*(lbl+ASSEMBLY_INSTRUCTION_MISC_FIELD_GET(ai))=(sll_instruction_index_t)(ASSEMBLY_INSTRUCTION_MISC_FIELD_GET(tmp)|ASSEMBLY_INSTRUCTION_FLAG_LABEL_REF);
				}
				else{
					*(lbl+ASSEMBLY_INSTRUCTION_MISC_FIELD_GET(ai))=i;
				}
				goto _remove_nop;
			}
			ai++;
			if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
				ai=ai->data._next_instruction;
			}
			src_ai++;
			if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(src_ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
				src_ai=src_ai->data._next_instruction;
			}
			i++;
		}
	}
	sll_deallocate(fn_ln);
	sll_deallocate(assembly_function_names);
	out->debug_data.data=sll_reallocate(out->debug_data.data,dbg_i*sizeof(sll_debug_line_data_t));
	out->debug_data.length=dbg_i;
	sm.new_indicies=sll_allocate(out->string_table.length*sizeof(sll_string_index_t));
	sll_string_index_t k=0;
	sll_string_index_t l=0;
	for (sll_string_index_t i=0;i<sm.map_length;i++){
		bitmap_t v=~(*(sm.map_data+i));
		while (v){
			sll_string_index_t j=FIND_FIRST_SET_BIT(v)|(i<<6);
			if (j==out->string_table.length){
				break;
			}
			for (sll_string_index_t n=k;n<j;n++){
				*(out->string_table.data+n-l)=*(out->string_table.data+n);
				*(sm.new_indicies+n)=n-l;
			}
			sll_free_string(out->string_table.data+j);
			k=j+1;
			l++;
			v&=v-1;
		}
	}
	sll_deallocate(sm.map_data);
	for (sll_string_index_t i=k;i<out->string_table.length;i++){
		*(out->string_table.data+i-l)=*(out->string_table.data+i);
		*(sm.new_indicies+i)=i-l;
	}
	if (l){
		out->string_table.length-=l;
		out->string_table.data=sll_reallocate(out->string_table.data,out->string_table.length*sizeof(sll_string_t));
	}
	for (sll_debug_data_length_t i=0;i<out->debug_data.length;i++){
		if ((out->debug_data.data+i)->line&(SLL_DEBUG_LINE_DATA_FLAG_FILE|SLL_DEBUG_LINE_DATA_FLAG_FUNC)){
			SLL_DEBUG_LINE_DATA_SET_DATA(out->debug_data.data+i,*(sm.new_indicies+SLL_DEBUG_LINE_DATA_GET_DATA(out->debug_data.data+i)),(out->debug_data.data+i)->line&(SLL_DEBUG_LINE_DATA_FLAG_FILE|SLL_DEBUG_LINE_DATA_FLAG_FUNC));
		}
	}
	ai=out->first_instruction;
	for (sll_instruction_index_t i=0;i<out->instruction_count;i++){
		if ((SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL||(SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)>=SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP&&SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)<=SLL_ASSEMBLY_INSTRUCTION_TYPE_JNI))&&(ai->type&ASSEMBLY_INSTRUCTION_FLAG_LABEL)){
			ai->type&=~ASSEMBLY_INSTRUCTION_FLAG_LABEL;
			assembly_instruction_label_t lbl_i=(assembly_instruction_label_t)ASSEMBLY_INSTRUCTION_MISC_FIELD_GET(ai);
			sll_instruction_index_t j;
			while (1){
				j=*(lbl+lbl_i);
				if (!(j&ASSEMBLY_INSTRUCTION_FLAG_LABEL_REF)){
					break;
				}
				assembly_instruction_label_t n=ASSEMBLY_INSTRUCTION_GET_LABEL_DATA(j);
				j=*(lbl+n);
				*(lbl+lbl_i)=j;
				lbl_i=n;
			}
			ai->data.jump._instruction=_get_instruction_at_offset(out,j);
			if (j<128){
				ai->data.jump.target.abs=j;
			}
			else{
				sll_relative_instruction_index_t off=((sll_relative_instruction_index_t)j)-i;
				if (GET_SIGN_ENCODED_INTEGER((sll_integer_t)off)<(sll_integer_t)j){
					ai->type|=SLL_ASSEMBLY_INSTRUCTION_FLAG_RELATIVE;
					ai->data.jump.target.rel=off;
				}
				else{
					ai->data.jump.target.abs=j;
				}
			}
		}
		else if (ai->type==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS||ai->type==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR||ai->type==SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR||ai->type==SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR){
			ai->data.string_index=*(sm.new_indicies+ai->data.string_index);
		}
		ai++;
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			ai=ai->data._next_instruction;
		}
	}
	sll_deallocate(lbl);
	sll_deallocate(sm.new_indicies);
}
