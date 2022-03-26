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
#include <sll/platform/util.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>



#define DEFINE_LABEL(g_dt,id) \
	do{ \
		sll_assembly_instruction_t* __ai=_acquire_next_instruction((g_dt)->a_dt); \
		__ai->t=ASSEMBLY_INSTRUCTION_TYPE_LABEL_TARGET; \
		ASSEMBLY_INSTRUCTION_MISC_FIELD(__ai)=(id); \
	} while(0)
#define GENERATE_OPCODE(g_dt,op) (_acquire_next_instruction((g_dt)->a_dt)->t=(op))
#define GENERATE_OPCODE_WITH_LABEL(g_dt,op,lbl) \
	do{ \
		sll_assembly_instruction_t* __ai=_acquire_next_instruction((g_dt)->a_dt); \
		__ai->t=(op)|ASSEMBLY_INSTRUCTION_FLAG_LABEL; \
		ASSEMBLY_INSTRUCTION_MISC_FIELD(__ai)=(lbl); \
	} while(0)
#define GENERATE_DEBUG_DATA(g_dt,o) \
	do{ \
		if ((o)->t==SLL_NODE_TYPE_DBG){ \
			sll_assembly_instruction_t* __ai=_acquire_next_instruction((g_dt)->a_dt); \
			__ai->t=ASSEMBLY_INSTRUCTION_TYPE_DBG; \
			__ai->dt.s=(o)->dt.s; \
		} \
	} while (0)
#define GET_SIGN_ENCODED_INTEGER(x) ((x)<0?((~(x))<<1)|1:(x)<<1)
#define GET_VARIABLE_INDEX(o,g_dt) (SLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH?((g_dt)->it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->v:((g_dt)->it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->v)
#define NEXT_LABEL(g_dt) ((g_dt)->n_lbl++)

#define PUSH (g_dt->s_off++)
#define POP (g_dt->s_off--)
#define POP2 (g_dt->s_off-=2)
#define POP3 (g_dt->s_off-=3)
#define POP_N(n) (g_dt->s_off-=(n))



static sll_assembly_instruction_t _assembly_nop={
	ASSEMBLY_INSTRUCTION_TYPE_NOP
};



static const sll_node_t* _generate_on_stack(const sll_node_t* o,assembly_generator_data_t* g_dt);



static const sll_node_t* _generate(const sll_node_t* o,assembly_generator_data_t* g_dt);



static const sll_node_t* _map_identifiers(const sll_node_t* o,const sll_source_file_t* sf,assembly_generator_data_t* g_dt,sll_scope_t fn_sc){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
		o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_IDENTIFIER:
			{
				sll_identifier_index_t i=SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id);
				sll_identifier_index_t j=SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id);
				if (j==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
					sll_identifier_t* id=sf->idt.il+i;
					if ((g_dt->it.l_im+i)->v==SLL_MAX_VARIABLE_INDEX){
						if (SLL_IDENTIFIER_IS_TLS(id)){
							(g_dt->it.l_im+i)->v=(g_dt->it.tls_vc<<1)|1;
							g_dt->it.tls_vc++;
						}
						else{
							(g_dt->it.l_im+i)->v=g_dt->it.vc<<1;
							g_dt->it.vc++;
						}
					}
					if (*(g_dt->rm.l+i)!=VARIABLE_OFFSET_NEVER_DELETE){
						if (fn_sc==SLL_MAX_SCOPE||id->sc>=fn_sc){
							*(g_dt->rm.l+i)=PTR(o);
						}
						else{
							*(g_dt->rm.l+i)=VARIABLE_OFFSET_NEVER_DELETE;
						}
					}
				}
				else{
					sll_identifier_t* id=sf->idt.s[j].dt+i;
					if ((g_dt->it.s_im[j]+i)->v==SLL_MAX_VARIABLE_INDEX){
						if (SLL_IDENTIFIER_IS_TLS(id)){
							(g_dt->it.s_im[j]+i)->v=(g_dt->it.tls_vc<<1)|1;
							g_dt->it.tls_vc++;
						}
						else{
							(g_dt->it.s_im[j]+i)->v=g_dt->it.vc<<1;
							g_dt->it.vc++;
						}
					}
					if (*(g_dt->rm.s[j]+i)!=VARIABLE_OFFSET_NEVER_DELETE){
						if (fn_sc==SLL_MAX_SCOPE||id->sc>=fn_sc){
							*(g_dt->rm.s[j]+i)=PTR(o);
						}
						else{
							*(g_dt->rm.s[j]+i)=VARIABLE_OFFSET_NEVER_DELETE;
						}
					}
				}
				return o+1;
			}
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t l=o->dt.al;
				o++;
				while (l){
					l--;
					o=_map_identifiers(o,sf,g_dt,fn_sc);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
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
				sll_arg_count_t l=o->dt.fn.ac;
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
				sll_arg_count_t l=o->dt.l.ac;
				o++;
				while (l){
					l--;
					o=_map_identifiers(o,sf,g_dt,fn_sc);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_map_identifiers(o,sf,g_dt,fn_sc);
	}
	return o;
}



static const sll_node_t* _skip_with_dbg(const sll_node_t* o,assembly_generator_data_t* g_dt){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
		GENERATE_DEBUG_DATA(g_dt,o);
		o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
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
					o=_skip_with_dbg(o,g_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
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
				sll_arg_count_t l=o->dt.fn.ac;
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
				sll_arg_count_t l=o->dt.l.ac;
				o++;
				while (l){
					l--;
					o=_skip_with_dbg(o,g_dt);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_skip_with_dbg(o,g_dt);
	}
	return o;
}



static const sll_node_t* _generate_sequential_jump(const sll_node_t* o,assembly_generator_data_t* g_dt,sll_assembly_instruction_type_t t,sll_bool_t stack){
	sll_arg_count_t l=o->dt.ac;
	SLL_ASSERT(l);
	if (l==1){
		return _generate(o+1,g_dt);
	}
	o=_generate_on_stack(o+1,g_dt);
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
	sll_arg_count_t l=o->dt.ac;
	SLL_ASSERT(l);
	if (l==1){
		return _generate(o+1,g_dt);
	}
	o=_generate_on_stack(o+1,g_dt);
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
	sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
	ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD;
	if (SLL_IDENTIFIER_GET_ARRAY_ID(n->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
		if (*(g_dt->rm.l+SLL_IDENTIFIER_GET_ARRAY_INDEX(n->dt.id))==n){
			ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL;
		}
		ai->dt.v=(g_dt->it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX(n->dt.id))->v;
	}
	else{
		if (*(g_dt->rm.s[SLL_IDENTIFIER_GET_ARRAY_ID(n->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(n->dt.id))==n){
			ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL;
		}
		ai->dt.v=(g_dt->it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID(n->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(n->dt.id))->v;
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
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
		GENERATE_DEBUG_DATA(g_dt,o);
		o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	NOT_FIELD(o);
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
			if ((!!(o->dt.c))^inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return o+1;
		case SLL_NODE_TYPE_INT:
			if ((!!(o->dt.i))^inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return o+1;
		case SLL_NODE_TYPE_FLOAT:
			if ((!!(o->dt.f))^inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return o+1;
		case SLL_NODE_TYPE_STRING:
			if ((!!((g_dt->sf->st.dt+o->dt.s)->l))^inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_MAP:
			SLL_UNIMPLEMENTED();
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
				sll_arg_count_t l=o->dt.ac;
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
				sll_arg_count_t l=o->dt.ac;
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
				sll_arg_count_t l=o->dt.ac;
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
				sll_arg_count_t l=o->dt.ac;
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
				if (!o->dt.ac){
					if (inv){
						GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
					}
					return o+1;
				}
				o=_generate_two_on_stack(o+1,g_dt,o->dt.ac);
				GENERATE_OPCODE_WITH_LABEL(g_dt,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JNI:SLL_ASSEMBLY_INSTRUCTION_TYPE_JI),lbl);
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO);
				POP2;
				return o;
			}
		case SLL_NODE_TYPE_BREAK:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_CONTINUE:
			SLL_UNIMPLEMENTED();
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
	SLL_ASSERT(o->t==SLL_NODE_TYPE_CALL);
	sll_arg_count_t ac=o->dt.ac;
	sll_arg_count_t l=ac;
	SLL_ASSERT(l);
	const sll_node_t* fn=o+1;
	o=sll_skip_node_const(fn);
	l--;
	while (l){
		l--;
		o=_generate_on_stack(o,g_dt);
	}
	_generate_on_stack(fn,g_dt);
	sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
	ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL;
	ai->dt.ac=ac-1;
	POP_N(ac-1);
	return o;
}



static const sll_node_t* _generate_call_array(const sll_node_t* o,assembly_generator_data_t* g_dt){
	SLL_ASSERT(o->t==SLL_NODE_TYPE_CALL_ARRAY);
	sll_arg_count_t ac=o->dt.ac;
	sll_arg_count_t l=ac;
	SLL_ASSERT(l);
	const sll_node_t* fn=o+1;
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
	SLL_ASSERT(o->t==SLL_NODE_TYPE_INLINE_FUNC);
	assembly_instruction_label_t e=NEXT_LABEL(g_dt);
	assembly_instruction_label_t p_rt=g_dt->rt;
	g_dt->rt=e;
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_generate(o,g_dt);
	}
	GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
	PUSH;
	DEFINE_LABEL(g_dt,e);
	g_dt->rt=p_rt;
	return o;
}



static const sll_node_t* _generate_assign(const sll_node_t* o,assembly_generator_data_t* g_dt,sll_bool_t st){
	sll_arg_count_t l=o->dt.ac;
	SLL_ASSERT(l>=2);
	o++;
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
		GENERATE_DEBUG_DATA(g_dt,o);
		o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	const sll_node_t* io=o;
	if (io->t==SLL_NODE_TYPE_IDENTIFIER){
		if ((SLL_IDENTIFIER_GET_ARRAY_ID(io->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH&&*(g_dt->rm.l+SLL_IDENTIFIER_GET_ARRAY_INDEX(io->dt.id))==io)||(SLL_IDENTIFIER_GET_ARRAY_ID(io->dt.id)!=SLL_MAX_SHORT_IDENTIFIER_LENGTH&&*(g_dt->rm.s[SLL_IDENTIFIER_GET_ARRAY_ID(io->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(io->dt.id))==io)){
			o++;
			l--;
			do{
				o=_generate(o,g_dt);
				l--;
			} while (l);
			return o;
		}
		o=_generate_on_stack(o+1,g_dt);
		if (st){
			SLL_UNIMPLEMENTED();
		}
		sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
		ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP;
		ai->dt.v=GET_VARIABLE_INDEX(io,g_dt);
		POP;
		l-=2;
		while (l){
			l--;
			o=_generate(o,g_dt);
		}
		return o;
	}
	else if (io->t==SLL_NODE_TYPE_VAR_ACCESS){
		sll_arg_count_t io_l=io->dt.ac;
		SLL_ASSERT(io_l>1);
		io++;
		while (io->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||io->t==SLL_NODE_TYPE_CHANGE_STACK){
			GENERATE_DEBUG_DATA(g_dt,o);
			io=(io->t==SLL_NODE_TYPE_CHANGE_STACK?io->dt._p:io+1);
		}
		SLL_ASSERT(io->t==SLL_NODE_TYPE_IDENTIFIER);
		sll_variable_index_t vi=GET_VARIABLE_INDEX(io,g_dt);
		io++;
		io_l--;
		sll_arg_count_t vl=io_l;
		do{
			while (io->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||io->t==SLL_NODE_TYPE_CHANGE_STACK){
				GENERATE_DEBUG_DATA(g_dt,o);
				io=(io->t==SLL_NODE_TYPE_CHANGE_STACK?io->dt._p:io+1);
			}
			SLL_ASSERT(io->t==SLL_NODE_TYPE_FIELD);
			sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
			ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS;
			ai->dt.s=io->dt.s;
			PUSH;
			io++;
			io_l--;
		} while (io_l);
		o=_generate_on_stack(io,g_dt);
		if (st){
			SLL_UNIMPLEMENTED();
		}
		sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
		ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR_ACCESS;
		ai->dt.va.v=vi;
		ai->dt.va.l=vl;
		POP_N(vl+1);
		l-=2;
		while (l){
			l--;
			o=_generate(o,g_dt);
		}
		return o;
	}
	SLL_ASSERT(io->t==SLL_NODE_TYPE_ACCESS);
	SLL_ASSERT(io->dt.ac>=2);
	o++;
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
		GENERATE_DEBUG_DATA(g_dt,o);
		o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	sll_identifier_index_t id=SLL_MAX_IDENTIFIER_INDEX;
	if (o->t==SLL_NODE_TYPE_IDENTIFIER){
		id=GET_VARIABLE_INDEX(o,g_dt);
		o++;
	}
	else{
		o=_generate_on_stack(o,g_dt);
	}
	sll_arg_count_t io_l=io->dt.ac-1;
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
	if (st){
		SLL_UNIMPLEMENTED();
	}
	sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
	ai->t=ai_t;
	if (id!=SLL_MAX_IDENTIFIER_INDEX){
		ai->t|=SLL_ASSEMBLY_INSTRUCTION_FLAG_INPLACE;
		ai->dt.v=id;
	}
	else{
		pop++;
	}
	POP_N(pop);
	l-=2;
	while (l){
		l--;
		o=_generate(o,g_dt);
	}
	return o;
}



static const sll_node_t* _mark_loop_delete(const sll_node_t* o,const assembly_generator_data_t* g_dt,bitmap_t* v_st,sll_scope_t sc){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
		o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t l=o->dt.al;
				o++;
				while (l){
					l--;
					o=_mark_loop_delete(o,g_dt,v_st,sc);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				o++;
				while (l){
					l--;
					o=_mark_loop_delete(o,g_dt,v_st,sc);
				}
				return o;
			}
		case SLL_NODE_TYPE_IDENTIFIER:
			{
				sll_identifier_index_t i=SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id);
				if (SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
					sll_identifier_t* id=g_dt->sf->idt.il+i;
					if (id->sc<=sc&&*(g_dt->rm.l+i)==o){
						*(g_dt->rm.l+i)=VARIABLE_OFFSET_NEVER_DELETE;
						sll_variable_index_t k=(g_dt->it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id))->v;
						*(v_st+(k>>6))|=1ull<<(k&63);
					}
				}
				else{
					sll_identifier_t* id=g_dt->sf->idt.s[SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)].dt+i;
					if (id->sc<=sc&&*(g_dt->rm.s[SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)]+i)==o){
						*(g_dt->rm.s[SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)]+i)=VARIABLE_OFFSET_NEVER_DELETE;
						sll_variable_index_t k=(g_dt->it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id))->v;
						*(v_st+(k>>6))|=1ull<<(k&63);
					}
				}
				return o+1;
			}
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
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
				sll_arg_count_t l=o->dt.l.ac;
				o++;
				while (l){
					l--;
					o=_mark_loop_delete(o,g_dt,v_st,sc);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_mark_loop_delete(o,g_dt,v_st,sc);
	}
	return o;
}



static void _init_loop_data(assembly_generator_data_t* g_dt,assembly_loop_generator_data_t* lg_dt,assembly_instruction_label_t cnt,sll_assembly_instruction_type_t brk){
	lg_dt->p_l_dt=g_dt->l_dt;
	g_dt->l_dt.cnt=cnt;
	g_dt->l_dt.brk=brk;
	lg_dt->v_st=sll_zero_allocate(((g_dt->l_dt_vc>>6)+1)*sizeof(bitmap_t));
}



static const sll_node_t* _generate_loop_start(assembly_generator_data_t* g_dt,const sll_node_t* o,assembly_loop_generator_data_t* lg_dt,assembly_instruction_label_t cnt,sll_assembly_instruction_type_t brk){
	_init_loop_data(g_dt,lg_dt,cnt,brk);
	SLL_ASSERT(o->dt.l.ac);
	sll_arg_count_t l=o->dt.l.ac-1;
	sll_scope_t sc=o->dt.l.sc;
	o=_generate(o+1,g_dt);
	const sll_node_t* cnd=o;
	for (sll_arg_count_t i=0;i<l;i++){
		o=_mark_loop_delete(o,g_dt,lg_dt->v_st,sc);
	}
	return cnd;
}



static void _generate_loop_end(assembly_generator_data_t* g_dt,assembly_loop_generator_data_t* lg_dt){
	g_dt->l_dt=lg_dt->p_l_dt;
	for (sll_variable_index_t i=0;i<(g_dt->l_dt_vc>>6)+1;i++){
		bitmap_t v=*(lg_dt->v_st+i);
		while (v){
			sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
			ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL;
			ai->dt.v=FIND_FIRST_SET_BIT(v)|(i<<6);
			v&=v-1;
		}
	}
	sll_deallocate(lg_dt->v_st);
}



static const sll_node_t* _generate_on_stack(const sll_node_t* o,assembly_generator_data_t* g_dt){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
		GENERATE_DEBUG_DATA(g_dt,o);
		o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	NOT_FIELD(o);
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR;
				ai->dt.c=o->dt.c;
				PUSH;
				return o+1;
			}
		case SLL_NODE_TYPE_INT:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT;
				ai->dt.i=o->dt.i;
				PUSH;
				return o+1;
			}
		case SLL_NODE_TYPE_FLOAT:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT;
				ai->dt.f=o->dt.f;
				PUSH;
				return o+1;
			}
		case SLL_NODE_TYPE_STRING:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS;
				ai->dt.s=o->dt.s;
				PUSH;
				return o+1;
			}
		case SLL_NODE_TYPE_ARRAY:
			{
				if (!o->dt.al){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO);
					PUSH;
					return o+1;
				}
				sll_array_length_t al=o->dt.al;
				o++;
				for (sll_array_length_t i=0;i<al;i++){
					o=_generate_on_stack(o,g_dt);
				}
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				if (al==1){
					ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ONE;
				}
				else{
					ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK;
					ai->dt.al=al;
					POP_N(al-1);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				if (!o->dt.ml){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP_ZERO);
					PUSH;
					return o+1;
				}
				sll_map_length_t ml=o->dt.ml;
				o++;
				for (sll_map_length_t i=0;i<ml;i++){
					o=_generate_on_stack(o,g_dt);
				}
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP;
				ai->dt.ml=ml;
				POP_N(ml-1);
				return o;
			}
		case SLL_NODE_TYPE_IDENTIFIER:
			_generate_identifier(o,g_dt);
			return o+1;
		case SLL_NODE_TYPE_FUNCTION_ID:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT;
				ai->dt.i=o->dt.fn_id+1;
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
		case SLL_NODE_TYPE_OPERATION_LIST:
			o=_generate(o,g_dt);
			GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
			PUSH;
			return o;
		case SLL_NODE_TYPE_AND:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_OR:
			{
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o+1;
				}
				o++;
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
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o+1;
				}
				o++;
				if (l==1){
					o=_generate_on_stack(o,g_dt);
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT);
					return o;
				}
				SLL_UNIMPLEMENTED();
			}
		case SLL_NODE_TYPE_BOOL:
			{
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o+1;
				}
				o++;
				if (l==1){
					o=_generate_on_stack(o,g_dt);
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_BOOL);
					return o;
				}
				SLL_UNIMPLEMENTED();
			}
		case SLL_NODE_TYPE_ASSIGN:
			return _generate_assign(o,g_dt,1);
		case SLL_NODE_TYPE_FUNC:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=ASSEMBLY_INSTRUCTION_TYPE_DBG_FUNC;
				ASSEMBLY_INSTRUCTION_MISC_FIELD(ai)=o->dt.fn.id;
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT;
				ai->dt.i=(o->t==SLL_NODE_TYPE_FUNC?o->dt.fn.id+1:~((sll_integer_t)o->dt.fn.id));
				PUSH;
				return _skip_with_dbg(o,g_dt);
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC_LOAD:
			{
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o+1;
				}
				o++;
				while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
					GENERATE_DEBUG_DATA(g_dt,o);
					o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				if (o->t==SLL_NODE_TYPE_STRING){
					sll_string_index_t i=o->dt.s;
					o++;
					l--;
					while (l){
						l--;
						o=_generate(o,g_dt);
					}
					sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
					ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR;
					ai->dt.s=i;
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
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o+1;
				}
				if (l==1){
					o=_generate(o+1,g_dt);
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o;
				}
				o++;
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
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				o=_generate_on_stack(o+1,g_dt);
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
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				o=_generate_on_stack(o+1,g_dt);
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
				sll_arg_count_t ac=o->dt.ac;
				sll_arg_count_t l=ac;
				SLL_ASSERT(l);
				if (l>4){
					l=4;
				}
				o++;
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
				sll_arg_count_t l=o->dt.ac;
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
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l>1);
				o++;
				while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
					GENERATE_DEBUG_DATA(g_dt,o);
					o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				SLL_ASSERT(o->t==SLL_NODE_TYPE_IDENTIFIER);
				sll_variable_index_t vi=GET_VARIABLE_INDEX(o,g_dt);
				o++;
				l--;
				sll_arg_count_t vl=l;
				do{
					while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
						GENERATE_DEBUG_DATA(g_dt,o);
						o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
					}
					SLL_ASSERT(o->t==SLL_NODE_TYPE_FIELD);
					sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
					ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS;
					ai->dt.s=o->dt.s;
					o++;
					l--;
				} while (l);
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_VAR;
				ai->dt.va.v=vi;
				ai->dt.va.l=vl;
				PUSH;
				return o;
			}
		case SLL_NODE_TYPE_HAS:
			{
				if (!o->dt.ac){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o+1;
				}
				o=_generate_two_on_stack(o+1,g_dt,o->dt.ac);
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
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o+1;
				}
				if (l==1){
					return _generate(o+1,g_dt);
				}
				o=_generate_on_stack(o+1,g_dt);
				l--;
				while (l){
					l--;
					while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
						GENERATE_DEBUG_DATA(g_dt,o);
						o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
					}
					if (o->t==SLL_NODE_TYPE_INT&&o->dt.i>0&&o->dt.i<=SLL_MAX_OBJECT_TYPE){
						sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
						ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE;
						ai->dt.t=(sll_object_type_t)(o->dt.i);
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
				sll_assembly_instruction_type_t ai_t=o->t-SLL_NODE_TYPE_TYPEOF+SLL_ASSEMBLY_INSTRUCTION_TYPE_TYPEOF;
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				o=_generate_on_stack(o+1,g_dt);
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
				sll_arg_count_t l=o->dt.d.ac;
				sll_assembly_instruction_type_t fl=SLL_ASSEMBLY_INSTRUCTION_FLAG_ANONYMOUS;
				if (o->dt.d.nm!=SLL_MAX_STRING_INDEX){
					sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
					ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS;
					ai->dt.t=o->dt.d.nm;
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
					while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
						GENERATE_DEBUG_DATA(g_dt,o);
						o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
					}
					SLL_ASSERT(o->t==SLL_NODE_TYPE_FIELD);
					sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
					ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS;
					ai->dt.s=o->dt.s;
					PUSH;
					o++;
				}
				if (v&1){
					o=_generate(o,g_dt);
				}
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL|fl;
				ai->dt.ac=v>>1;
				if (!fl){
					POP;
				}
				POP_N(ai->dt.ac*2-1);
				return o;
			}
		case SLL_NODE_TYPE_NEW:
			{
				sll_arg_count_t l=o->dt.ac;
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
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW;
				ai->dt.ac=v;
				POP_N(v);
				return o;
			}
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			{
				GENERATE_OPCODE(g_dt,(o->t>=SLL_NODE_TYPE_FOR_MAP?SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP_ZERO:SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO));
				PUSH;
				sll_arg_count_t l=o->dt.l.ac;
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
				o=(r->t==SLL_NODE_TYPE_FOR_ARRAY||r->t==SLL_NODE_TYPE_FOR_MAP?_generate_jump(cnd,g_dt,e,1):sll_skip_node_const(cnd));
				DEFINE_LABEL(g_dt,s);
				l-=2;
				sll_bool_t extra=(l==(r->t>=SLL_NODE_TYPE_FOR_MAP));
				sll_arg_count_t end=(r->t>=SLL_NODE_TYPE_FOR_MAP);
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
				if (r->t>=SLL_NODE_TYPE_FOR_MAP){
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
		case SLL_NODE_TYPE_BREAK:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_CONTINUE:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_REF:
		case SLL_NODE_TYPE_READ_BLOCKING_CHAR:
			{
				sll_assembly_instruction_type_t ai_t=(o->t==SLL_NODE_TYPE_REF?SLL_ASSEMBLY_INSTRUCTION_TYPE_REF:SLL_ASSEMBLY_INSTRUCTION_TYPE_READ_BLOCKING_CHAR);
				sll_arg_count_t l=o->dt.ac;
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
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
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
				sll_arg_count_t l=o->dt.ac;
				o++;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_ID);
				return o;
			}
		case SLL_NODE_TYPE_THREAD_WAIT:
		case SLL_NODE_TYPE_THREAD_LOCK:
		case SLL_NODE_TYPE_THREAD_SEMAPHORE:
		case SLL_NODE_TYPE_THREAD_BARRIER_EQ:
		case SLL_NODE_TYPE_THREAD_BARRIER_GEQ:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_READ_BLOCKING:
			{
				if (!o->dt.ac){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					PUSH;
					return o+1;
				}
				o=_generate_two_on_stack(o+1,g_dt,o->dt.ac);
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_READ_BLOCKING);
				POP;
				return o;
			}
	}
	sll_arg_count_t l=o->dt.ac;
	sll_assembly_instruction_type_t t=o->t+(SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD-SLL_NODE_TYPE_ADD);
	SLL_ASSERT(l);
	if (l==1){
		return _generate_on_stack(o+1,g_dt);
	}
	o=_generate_on_stack(o+1,g_dt);
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
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
		GENERATE_DEBUG_DATA(g_dt,o);
		o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t al=o->dt.al;
				o++;
				while (al){
					o=_generate(o,g_dt);
					al--;
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t ml=o->dt.ml;
				o++;
				while (ml){
					o=_generate(o,g_dt);
					ml--;
				}
				return o;
			}
		case SLL_NODE_TYPE_IDENTIFIER:
			{
				if (SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
					if (*(g_dt->rm.l+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id))==o){
						sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
						ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL;
						ai->dt.v=(g_dt->it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id))->v;
					}
				}
				else{
					if (*(g_dt->rm.s[SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id))==o){
						sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
						ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL;
						ai->dt.v=(g_dt->it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id))->v;
					}
				}
				return o+1;
			}
		case SLL_NODE_TYPE_PRINT:
			{
				sll_arg_count_t l=o->dt.ac;
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
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_OR:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_ASSIGN:
			return _generate_assign(o,g_dt,0);
		case SLL_NODE_TYPE_FUNC:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=ASSEMBLY_INSTRUCTION_TYPE_DBG_FUNC;
				ASSEMBLY_INSTRUCTION_MISC_FIELD(ai)=o->dt.fn.id;
				return _skip_with_dbg(o,g_dt);
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
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
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				o++;
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
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				if (l==1){
					return _generate(o+1,g_dt);
				}
				const sll_node_t* r=o;
				sll_bool_t d=!(o->dt.ac&1);
				o=_generate_on_stack(o+1,g_dt);
				l=(l-1)>>1;
				node_label_t* m=sll_allocate((l+1)*sizeof(node_label_t));
				sll_arg_count_t i=0;
				while (l){
					l--;
					assembly_instruction_label_t j=NEXT_LABEL(g_dt);
					o=_generate_on_stack(o,g_dt);
					GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL,j);
					PUSH;
					(m+i)->o=o;
					(m+i)->l=j;
					i++;
					o=sll_skip_node_const(o);
				}
				assembly_instruction_label_t e=NEXT_LABEL(g_dt);
				if (d){
					assembly_instruction_label_t j=NEXT_LABEL(g_dt);
					GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL,j);
					PUSH;
					(m+i)->o=o;
					(m+i)->l=j;
				}
				else{
					GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL,e);
					PUSH;
					(m+i)->o=NULL;
					(m+i)->l=e;
				}
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_JT;
				ai->dt.al=i;
				POP_N(i*2+1);
				o=r+1;
				for (sll_arg_count_t j=0;j<=i;j++){
					if ((m+j)->o){
						DEFINE_LABEL(g_dt,(m+j)->l);
						o=_generate((m+j)->o,g_dt);
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
				sll_arg_count_t l=o->dt.l.ac;
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
				o=(o->t>=SLL_NODE_TYPE_WHILE?sll_skip_node_const(cnd):_generate_jump(cnd,g_dt,e,1));
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
				sll_arg_count_t l=o->dt.l.ac;
				assembly_instruction_label_t s=NEXT_LABEL(g_dt);
				assembly_instruction_label_t e=NEXT_LABEL(g_dt);
				assembly_loop_generator_data_t lg_dt;
				_init_loop_data(g_dt,&lg_dt,s,e);
				DEFINE_LABEL(g_dt,s);
				o++;
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
				sll_assembly_instruction_type_t ai_t=(o->t==SLL_NODE_TYPE_INC?SLL_ASSEMBLY_INSTRUCTION_TYPE_INC:SLL_ASSEMBLY_INSTRUCTION_TYPE_DEC);
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				o++;
				while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
					GENERATE_DEBUG_DATA(g_dt,o);
					o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				SLL_ASSERT(o->t==SLL_NODE_TYPE_IDENTIFIER);
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=ai_t|SLL_ASSEMBLY_INSTRUCTION_FLAG_INPLACE;
				if (SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
					if (*(g_dt->rm.l+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id))==o){
						SLL_UNIMPLEMENTED();
					}
					ai->dt.v=(g_dt->it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id))->v;
				}
				else{
					if (*(g_dt->rm.s[SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id))==o){
						SLL_UNIMPLEMENTED();
					}
					ai->dt.v=(g_dt->it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id))->v;
				}
				l--;
				o++;
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
				sll_arg_count_t l=o->dt.ac;
				o++;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				if (g_dt->l_dt.brk==MAX_ASSEMBLY_INSTRUCTION_LABEL){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO);
					PUSH;
				}
				else{
					GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,g_dt->l_dt.brk);
				}
				return o;
			}
		case SLL_NODE_TYPE_CONTINUE:
			{
				sll_arg_count_t l=o->dt.ac;
				o++;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				if (g_dt->l_dt.cnt==MAX_ASSEMBLY_INSTRUCTION_LABEL){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO);
					PUSH;
				}
				else{
					GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,g_dt->l_dt.cnt);
				}
				return o;
			}
		case SLL_NODE_TYPE_RETURN:
			{
				sll_arg_count_t l=o->dt.ac;
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
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				if (g_dt->rt==MAX_ASSEMBLY_INSTRUCTION_LABEL){
					ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET;
				}
				else{
					ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP|ASSEMBLY_INSTRUCTION_FLAG_LABEL;
					ASSEMBLY_INSTRUCTION_MISC_FIELD(ai)=g_dt->rt;
				}
				return o;
			}
		case SLL_NODE_TYPE_THREAD_WAIT:
		case SLL_NODE_TYPE_THREAD_LOCK:
		case SLL_NODE_TYPE_THREAD_SEMAPHORE:
			{
				sll_assembly_instruction_type_t ai_t=(o->t-SLL_NODE_TYPE_THREAD_WAIT+SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_WAIT);
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
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
				sll_assembly_instruction_type_t ai_t=o->t-SLL_NODE_TYPE_THREAD_BARRIER_EQ+SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_BARRIER_EQ;
				sll_arg_count_t l=o->dt.ac;
				o++;
				if (l<2){
					return (l?_generate(o,g_dt):o);
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
			SLL_UNIMPLEMENTED();
	}
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_generate(o,g_dt);
	}
	return o;
}



__SLL_EXTERNAL void sll_free_assembly_data(sll_assembly_data_t* a_dt){
	a_dt->tm=0;
	a_dt->h=NULL;
	a_dt->ic=0;
	a_dt->vc=0;
	sll_deallocate(a_dt->ft.dt);
	a_dt->ft.dt=NULL;
	a_dt->ft.l=0;
	for (sll_string_index_t i=0;i<a_dt->st.l;i++){
		sll_free_string(a_dt->st.dt+i);
	}
	sll_deallocate(a_dt->st.dt);
	a_dt->st.dt=NULL;
	a_dt->st.l=0;
	sll_deallocate(a_dt->dbg.dt);
	a_dt->dbg.dt=NULL;
	a_dt->dbg.l=0;
	void* pg=a_dt->_s.s;
	while (pg){
		void* n=*((void**)pg);
		sll_platform_free_page(pg,SLL_ROUND_PAGE(ASSEMBLY_INSTRUCTION_STACK_ALLOC_SIZE));
		pg=n;
	}
	a_dt->_s.s=NULL;
	a_dt->_s.e=NULL;
	a_dt->_s.c=0;
	a_dt->_s.p=NULL;
}



__SLL_EXTERNAL void sll_generate_assembly(const sll_source_file_t* sf,sll_assembly_data_t* o){
	_init_assembly_stack(o);
	o->tm=sll_platform_get_current_time();
	o->h=o->_s.p;
	o->ic=0;
	o->vc=0;
	o->tls_vc=0;
	if (!sf->dt){
		_acquire_next_instruction(o)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO;
		o->ft.l=0;
		o->ft.dt=NULL;
		o->st.l=0;
		o->st.dt=NULL;
		return;
	}
	o->st.l=sf->st.l;
	o->st.dt=sll_allocate(o->st.l*sizeof(sll_string_t));
	for (sll_string_index_t i=0;i<o->st.l;i++){
		sll_string_clone(sf->st.dt+i,o->st.dt+i);
	}
	assembly_generator_data_t g_dt={
		o,
		sf,
		{
			.l_im=sll_allocate(sf->idt.ill*sizeof(identifier_data_t)),
			.l_sc=0,
			.sc_vi=sll_allocate(sf->_n_sc_id*sizeof(sll_variable_index_t)),
			.vc=0,
			.tls_vc=0
		},
		0,
		{
			.l=sll_allocate(sf->idt.ill*sizeof(void*))
		},
		{
			MAX_ASSEMBLY_INSTRUCTION_LABEL,
			MAX_ASSEMBLY_INSTRUCTION_LABEL
		},
		MAX_ASSEMBLY_INSTRUCTION_LABEL,
		0
	};
	for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		g_dt.it.s_im[i]=sll_allocate(sf->idt.s[i].l*sizeof(identifier_data_t));
		g_dt.rm.s[i]=sll_allocate(sf->idt.s[i].l*sizeof(void*));
		for (sll_identifier_list_length_t j=0;j<sf->idt.s[i].l;j++){
			(g_dt.it.s_im[i]+j)->v=SLL_MAX_VARIABLE_INDEX;
			*(g_dt.rm.s[i]+j)=NULL;
		}
	}
	for (sll_identifier_list_length_t i=0;i<sf->idt.ill;i++){
		(g_dt.it.l_im+i)->v=SLL_MAX_VARIABLE_INDEX;
		*(g_dt.rm.l+i)=NULL;
	}
	for (sll_scope_t i=0;i<sf->_n_sc_id;i++){
		*(g_dt.it.sc_vi+i)=SLL_MAX_VARIABLE_INDEX;
	}
	_map_identifiers(sf->dt,sf,&g_dt,SLL_MAX_SCOPE);
	sll_variable_index_t g_vc=g_dt.it.vc;
	sll_variable_index_t* fn_vc=sll_allocate_stack(sf->ft.l*sizeof(sll_variable_index_t));
	for (sll_function_index_t i=0;i<sf->ft.l;i++){
		g_dt.it.vc=g_vc;
		const sll_function_t* k=*(sf->ft.dt+i);
		for (sll_arg_count_t j=0;j<SLL_FUNCTION_GET_ARGUMENT_COUNT(k);j++){
			sll_identifier_index_t ii=k->a[j];
			if (SLL_IDENTIFIER_GET_ARRAY_ID(ii)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
				(g_dt.it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX(ii))->v=g_dt.it.vc<<1;
			}
			else{
				(g_dt.it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID(ii)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(ii))->v=g_dt.it.vc<<1;
			}
			g_dt.it.vc++;
		}
		g_dt.s_off=0;
		g_dt.it.l_sc=0;
		const sll_node_t* fo=_get_node_at_offset(sf,k->off);
		SLL_ASSERT(fo->t==SLL_NODE_TYPE_FUNC);
		sll_arg_count_t ac=fo->dt.fn.ac;
		sll_scope_t sc=fo->dt.fn.sc;
		fo++;
		while (ac){
			ac--;
			fo=_map_identifiers(fo,sf,&g_dt,sc);
		}
		*(fn_vc+i)=g_dt.it.vc-g_vc-SLL_FUNCTION_GET_ARGUMENT_COUNT(k);
	}
	sll_deallocate(g_dt.it.sc_vi);
	o->vc=g_vc;
	o->tls_vc=g_dt.it.tls_vc;
	g_dt.l_dt_vc=(o->tls_vc<<1)+1;
	if (g_dt.l_dt_vc<(o->vc<<1)){
		g_dt.l_dt_vc=o->vc<<1;
	}
	g_dt.s_off=0;
	_generate(sf->dt,&g_dt);
	SLL_ASSERT(g_dt.rt==MAX_ASSEMBLY_INSTRUCTION_LABEL);
	GENERATE_OPCODE(&g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO);
	o->ft.l=sf->ft.l;
	o->ft.dt=sll_allocate(sf->ft.l*sizeof(sll_assembly_function_t));
	sll_function_index_t fn_n=0;
	for (sll_function_index_t i=0;i<sf->ft.l;i++){
		const sll_function_t* k=*(sf->ft.dt+i);
		sll_assembly_instruction_t* ai=_acquire_next_instruction(o);
		ai->t=ASSEMBLY_INSTRUCTION_TYPE_FUNC_START;
		ASSEMBLY_INSTRUCTION_MISC_FIELD(ai)=i;
		(o->ft.dt+i)->ac=k->al;
		if (k->nm==SLL_MAX_STRING_INDEX){
			sll_string_t str;
			sll_string_format(SLL_CHAR("@%u"),&str,fn_n);
			fn_n++;
			(o->ft.dt+i)->nm=sll_add_string(&(o->st),&str,0);
		}
		else{
			(o->ft.dt+i)->nm=k->nm;
		}
		if (*(fn_vc+i)){
			ai=_acquire_next_instruction(o);
			ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_STACK;
			ai->dt.so=*(fn_vc+i);
		}
		const sll_node_t* fo=_get_node_at_offset(sf,k->off);
		SLL_ASSERT(fo->t==SLL_NODE_TYPE_FUNC);
		sll_arg_count_t ac=fo->dt.fn.ac;
		g_dt.s_off=0;
		fo++;
		while (ac){
			ac--;
			fo=_generate(fo,&g_dt);
		}
		GENERATE_OPCODE(&g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO);
	}
	sll_deallocate(fn_vc);
	for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		sll_deallocate(g_dt.it.s_im[i]);
		sll_deallocate(g_dt.rm.s[i]);
	}
	sll_deallocate(g_dt.it.l_im);
	sll_deallocate(g_dt.rm.l);
	sll_assembly_instruction_t* ai=o->h;
	sll_assembly_instruction_t* st[5]={
		&_assembly_nop,
		&_assembly_nop,
		&_assembly_nop,
		&_assembly_nop,
		&_assembly_nop
	};
	o->dbg.l=0;
	for (sll_instruction_index_t i=0;i<o->ic;i++){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==ASSEMBLY_INSTRUCTION_TYPE_NOP||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==ASSEMBLY_INSTRUCTION_TYPE_DBG||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==ASSEMBLY_INSTRUCTION_TYPE_DBG_FUNC||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
				ai=ai->dt._p;
			}
			else{
				if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==ASSEMBLY_INSTRUCTION_TYPE_DBG){
					o->dbg.l++;
				}
				else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==ASSEMBLY_INSTRUCTION_TYPE_DBG_FUNC){
					o->dbg.l+=2;
				}
				ai++;
			}
			continue;
		}
		st[4]=st[3];
		st[3]=st[2];
		st[2]=st[1];
		st[1]=st[0];
		st[0]=ai;
		_optimize_assembly(st,&_assembly_nop);
		ai++;
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			ai=ai->dt._p;
		}
	}
	strint_map_data_t sm={
		(o->st.l>>6)+1,
		sll_allocate(((o->st.l>>6)+1)*sizeof(bitmap_t))
	};
	for (sll_string_index_t i=0;i<sm.ml;i++){
		*(sm.m+i)=0;
	}
	for (sll_function_index_t i=0;i<o->ft.l;i++){
		sll_string_index_t j=(o->ft.dt+i)->nm;
		*(sm.m+(j>>6))|=1ull<<(j&63);
	}
	ai=o->h;
	o->dbg.dt=sll_allocate(o->dbg.l*sizeof(sll_debug_line_data_t));
	sll_instruction_index_t dbg_i=0;
	sll_file_offset_t f_off=0;
	sll_string_index_t f_idx=0;
	sll_instruction_index_t l_dbg_ii=0;
	sll_assembly_instruction_t* s=ai;
	sll_instruction_index_t* lbl=sll_allocate(g_dt.n_lbl*sizeof(sll_instruction_index_t));
	file_line_t* fn_ln=sll_allocate(sf->ft.l*sizeof(file_line_t));
	{
		sll_instruction_index_t i=0;
		while (i<o->ic){
			if (s->t==ASSEMBLY_INSTRUCTION_TYPE_NOP){
_remove_nop:
				s++;
				if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(s)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
					s=s->dt._p;
				}
				o->ic--;
				continue;
			}
			*ai=*s;
			if (ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS||ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR||ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR||ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR){
				*(sm.m+(ai->dt.s>>6))|=1ull<<(ai->dt.s&63);
			}
			else if (ai->t==ASSEMBLY_INSTRUCTION_TYPE_DBG){
				if (ai->dt.s==SLL_MAX_STRING_INDEX){
					f_off++;
					if (i!=l_dbg_ii||(dbg_i&&((o->dbg.dt+dbg_i-1)->ln&SLL_DEBUG_LINE_DATA_FLAG_FILE))){
						(o->dbg.dt+dbg_i)->ii=i-l_dbg_ii;
						(o->dbg.dt+dbg_i)->ln=f_off;
						dbg_i++;
					}
				}
				else if (f_idx!=ai->dt.s){
					(o->dbg.dt+dbg_i)->ii=i-l_dbg_ii;
					(o->dbg.dt+dbg_i)->ln=ai->dt.s|SLL_DEBUG_LINE_DATA_FLAG_FILE;
					*(sm.m+(ai->dt.s>>6))|=1ull<<(ai->dt.s&63);
					f_off=0;
					f_idx=ai->dt.s;
					dbg_i++;
				}
				l_dbg_ii=i;
				goto _remove_nop;
			}
			else if (ai->t==ASSEMBLY_INSTRUCTION_TYPE_DBG_FUNC){
				(fn_ln+ASSEMBLY_INSTRUCTION_MISC_FIELD(ai))->ln=f_off;
				(fn_ln+ASSEMBLY_INSTRUCTION_MISC_FIELD(ai))->fp=f_idx;
				goto _remove_nop;
			}
			else if (ai->t==ASSEMBLY_INSTRUCTION_TYPE_FUNC_START){
				(o->ft.dt+ASSEMBLY_INSTRUCTION_MISC_FIELD(ai))->i=i;
				if (f_idx!=(fn_ln+ASSEMBLY_INSTRUCTION_MISC_FIELD(ai))->fp){
					f_idx=(fn_ln+ASSEMBLY_INSTRUCTION_MISC_FIELD(ai))->fp;
					(o->dbg.dt+dbg_i)->ii=i-l_dbg_ii;
					(o->dbg.dt+dbg_i)->ln=f_idx|SLL_DEBUG_LINE_DATA_FLAG_FILE;
					dbg_i++;
					l_dbg_ii=i;
				}
				(o->dbg.dt+dbg_i)->ii=i-l_dbg_ii;
				(o->dbg.dt+dbg_i)->ln=(o->ft.dt+ASSEMBLY_INSTRUCTION_MISC_FIELD(ai))->nm|SLL_DEBUG_LINE_DATA_FLAG_FUNC;
				dbg_i++;
				f_off=(fn_ln+ASSEMBLY_INSTRUCTION_MISC_FIELD(ai))->ln;
				(o->dbg.dt+dbg_i)->ii=0;
				(o->dbg.dt+dbg_i)->ln=f_off;
				dbg_i++;
				l_dbg_ii=i;
				goto _remove_nop;
			}
			else if (ai->t==ASSEMBLY_INSTRUCTION_TYPE_LABEL_TARGET){
				sll_assembly_instruction_t* tmp=s+1;
				while (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(tmp)==ASSEMBLY_INSTRUCTION_TYPE_NOP||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(tmp)==ASSEMBLY_INSTRUCTION_TYPE_DBG||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(tmp)==ASSEMBLY_INSTRUCTION_TYPE_DBG_FUNC||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(tmp)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
					tmp=(SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(tmp)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK?tmp->dt._p:tmp+1);
				}
				if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(tmp)==SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP&&(tmp->t&ASSEMBLY_INSTRUCTION_FLAG_LABEL)){
					*(lbl+ASSEMBLY_INSTRUCTION_MISC_FIELD(ai))=ASSEMBLY_INSTRUCTION_MISC_FIELD(tmp)|ASSEMBLY_INSTRUCTION_FLAG_LABEL_REF;
				}
				else{
					*(lbl+ASSEMBLY_INSTRUCTION_MISC_FIELD(ai))=i;
				}
				goto _remove_nop;
			}
			ai++;
			if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
				ai=ai->dt._p;
			}
			s++;
			if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(s)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
				s=s->dt._p;
			}
			i++;
		}
	}
	sll_deallocate(fn_ln);
	o->dbg.dt=sll_reallocate(o->dbg.dt,dbg_i*sizeof(sll_debug_line_data_t));
	o->dbg.l=dbg_i;
	sm.im=sll_allocate(o->st.l*sizeof(sll_string_t*));
	sll_string_index_t k=0;
	sll_string_index_t l=0;
	for (sll_string_index_t i=0;i<sm.ml;i++){
		bitmap_t v=~(*(sm.m+i));
		while (v){
			sll_string_index_t j=FIND_FIRST_SET_BIT(v)|(i<<6);
			if (j==o->st.l){
				break;
			}
			for (sll_string_index_t n=k;n<j;n++){
				*(o->st.dt+n-l)=*(o->st.dt+n);
				*(sm.im+n)=n-l;
			}
			sll_free_string(o->st.dt+j);
			k=j+1;
			l++;
			v&=v-1;
		}
	}
	sll_deallocate(sm.m);
	for (sll_string_index_t i=k;i<o->st.l;i++){
		*(o->st.dt+i-l)=*(o->st.dt+i);
		*(sm.im+i)=i-l;
	}
	if (l){
		o->st.l-=l;
		o->st.dt=sll_reallocate(o->st.dt,o->st.l*sizeof(sll_string_t));
	}
	for (sll_function_index_t i=0;i<o->ft.l;i++){
		(o->ft.dt+i)->nm=*(sm.im+(o->ft.dt+i)->nm);
	}
	for (sll_instruction_index_t i=0;i<o->dbg.l;i++){
		if ((o->dbg.dt+i)->ln&(SLL_DEBUG_LINE_DATA_FLAG_FILE|SLL_DEBUG_LINE_DATA_FLAG_FUNC)){
			(o->dbg.dt+i)->ln=(*(sm.im+SLL_DEBUG_LINE_DATA_GET_DATA(o->dbg.dt+i)))|((o->dbg.dt+i)->ln&(SLL_DEBUG_LINE_DATA_FLAG_FILE|SLL_DEBUG_LINE_DATA_FLAG_FUNC));
		}
	}
	ai=o->h;
	for (sll_instruction_index_t i=0;i<o->ic;i++){
		if ((SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL||(SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)>=SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP&&SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)<=SLL_ASSEMBLY_INSTRUCTION_TYPE_JNI))&&(ai->t&ASSEMBLY_INSTRUCTION_FLAG_LABEL)){
			ai->t&=~ASSEMBLY_INSTRUCTION_FLAG_LABEL;
			assembly_instruction_label_t lbl_i=ASSEMBLY_INSTRUCTION_MISC_FIELD(ai);
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
			if (j<128){
				ai->dt.j=j;
			}
			else{
				sll_relative_instruction_index_t off=((sll_relative_instruction_index_t)j)-i;
				if (GET_SIGN_ENCODED_INTEGER((sll_integer_t)off)<(sll_integer_t)j){
					ai->t|=SLL_ASSEMBLY_INSTRUCTION_FLAG_RELATIVE;
					ai->dt.rj=off;
				}
				else{
					ai->dt.j=j;
				}
			}
		}
		else if (ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS||ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR||ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR||ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR){
			ai->dt.s=*(sm.im+ai->dt.s);
		}
		ai++;
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			ai=ai->dt._p;
		}
	}
	sll_deallocate(lbl);
	sll_deallocate(sm.im);
}
