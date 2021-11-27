#include <sll/_sll_internal.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/object.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <stdint.h>



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
		__ai->t=(op)|ASSEMBLY_INSTRUCTION_LABEL; \
		ASSEMBLY_INSTRUCTION_MISC_FIELD(__ai)=(lbl); \
	} while(0)
#define GET_SIGN_ENCODED_INTEGER(x) ((x)<0?((~(x))<<1)|1:(x)<<1)
#define GET_VARIABLE_INDEX(o,g_dt) (SLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH?((g_dt)->it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->v:((g_dt)->it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->v)
#define NEXT_LABEL(g_dt) ((g_dt)->n_lbl++)
#define SHIFT_UP(st) \
	do{ \
		(st)[0]=(st)[1]; \
		(st)[1]=(st)[2]; \
		(st)[2]=(st)[3]; \
		(st)[3]=(st)[4]; \
	} while(0)
#define SHIFT_UP_TWO(st) \
	do{ \
		(st)[0]=(st)[2]; \
		(st)[1]=(st)[3]; \
		(st)[2]=(st)[4]; \
		(st)[3]=&_assembly_nop; \
	} while(0)



static sll_assembly_instruction_t _assembly_nop={
	SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP
};


static const sll_node_t* _generate_on_stack(const sll_node_t* o,assembly_generator_data_t* g_dt);



static const sll_node_t* _generate(const sll_node_t* o,assembly_generator_data_t* g_dt);



static const sll_node_t* _map_identifiers(const sll_node_t* o,const sll_compilation_data_t* c_dt,assembly_generator_data_t* g_dt,sll_scope_t fn_sc){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DEBUG_DATA||o->t==NODE_TYPE_CHANGE_STACK){
		o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_NODE_TYPE_UNKNOWN:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t l=o->dt.al;
				o++;
				while (l){
					l--;
					o=_map_identifiers(o,c_dt,g_dt,fn_sc);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				o++;
				while (l){
					l--;
					o=_map_identifiers(o,c_dt,g_dt,fn_sc);
				}
				return o;
			}
		case SLL_NODE_TYPE_IDENTIFIER:
			{
				sll_identifier_index_t i=SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id);
				uint8_t j=SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id);
				if (j==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
					sll_identifier_t* id=c_dt->idt.il+i;
					if (g_dt->it.l_sc!=id->sc){
						*(g_dt->it.sc_vi+g_dt->it.l_sc)=g_dt->it.n_vi;
						g_dt->it.l_sc=id->sc;
						if (*(g_dt->it.sc_vi+g_dt->it.l_sc)!=SLL_MAX_SCOPE){
							g_dt->it.n_vi=*(g_dt->it.sc_vi+g_dt->it.l_sc);
						}
					}
					if (*(g_dt->rm.l+i)!=VARIABLE_OFFSET_NEVER_DELETE){
						if (fn_sc==SLL_MAX_SCOPE||id->sc>=fn_sc){
							*(g_dt->rm.l+i)=(void*)o;
						}
						else{
							*(g_dt->rm.l+i)=VARIABLE_OFFSET_NEVER_DELETE;
						}
					}
					if ((g_dt->it.l_im+i)->v==SLL_MAX_VARIABLE_INDEX){
						(g_dt->it.l_im+i)->v=g_dt->it.n_vi;
						g_dt->it.n_vi++;
						if (g_dt->it.n_vi>g_dt->it.vc){
							g_dt->it.vc=g_dt->it.n_vi;
						}
					}
				}
				else{
					sll_identifier_t* id=c_dt->idt.s[j].dt+i;
					if (g_dt->it.l_sc!=id->sc){
						*(g_dt->it.sc_vi+g_dt->it.l_sc)=g_dt->it.n_vi;
						g_dt->it.l_sc=id->sc;
						if (*(g_dt->it.sc_vi+g_dt->it.l_sc)!=SLL_MAX_SCOPE){
							g_dt->it.n_vi=*(g_dt->it.sc_vi+g_dt->it.l_sc);
						}
					}
					if (*(g_dt->rm.s[j]+i)!=VARIABLE_OFFSET_NEVER_DELETE){
						if (fn_sc==SLL_MAX_SCOPE||id->sc>=fn_sc){
							*(g_dt->rm.s[j]+i)=(void*)o;
						}
						else{
							*(g_dt->rm.s[j]+i)=VARIABLE_OFFSET_NEVER_DELETE;
						}
					}
					if ((g_dt->it.s_im[j]+i)->v==SLL_MAX_VARIABLE_INDEX){
						(g_dt->it.s_im[j]+i)->v=g_dt->it.n_vi;
						g_dt->it.n_vi++;
						if (g_dt->it.n_vi>g_dt->it.vc){
							g_dt->it.vc=g_dt->it.n_vi;
						}
					}
				}
				return o+1;
			}
		case SLL_NODE_TYPE_FUNC:
			return sll_skip_node_const(o);
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.ac;
				o++;
				while (l){
					l--;
					o=_map_identifiers(o,c_dt,g_dt,fn_sc);
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
					o=_map_identifiers(o,c_dt,g_dt,fn_sc);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_map_identifiers(o,c_dt,g_dt,fn_sc);
	}
	return o;
}



static sll_assembly_instruction_t* _get_previous_instruction(assembly_generator_data_t* g_dt){
	sll_assembly_instruction_t* o=g_dt->a_dt->_s.p;
	sll_instruction_index_t i=g_dt->a_dt->ic;
	do{
		if (!i){
			return &_assembly_nop;
		}
		o=(SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(o)==ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK?o->dt._p:o-1);
		i--;
	} while (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(o)==SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(o)==ASSEMBLY_INSTRUCTION_TYPE_FUNC_START||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(o)==ASSEMBLY_INSTRUCTION_TYPE_LABEL_TARGET||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(o)==ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK);
	return o;
}



static const sll_node_t* _generate_sequential_jump(const sll_node_t* o,assembly_generator_data_t* g_dt,sll_assembly_instruction_type_t t,uint8_t st){
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
	}
	o=_generate_on_stack(o,g_dt);
	GENERATE_OPCODE_WITH_LABEL(g_dt,t,e);
	GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO);
	if (!st){
		DEFINE_LABEL(g_dt,e);
	}
	else{
		GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE);
		assembly_instruction_label_t tmp=NEXT_LABEL(g_dt);
		GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,tmp);
		DEFINE_LABEL(g_dt,e);
		GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
		DEFINE_LABEL(g_dt,tmp);
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
	}
	o=_generate_on_stack(o,g_dt);
	GENERATE_OPCODE_WITH_LABEL(g_dt,t,lbl);
	GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO);
	return o;
}



static const sll_node_t* _generate_jump(const sll_node_t* o,assembly_generator_data_t* g_dt,assembly_instruction_label_t lbl,uint8_t inv){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DEBUG_DATA||o->t==NODE_TYPE_CHANGE_STACK){
		o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_NODE_TYPE_UNKNOWN:
			return o+1;
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
			if ((!!((g_dt->c_dt->st.dt+o->dt.s)->l))^inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_MAP:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_IDENTIFIER:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD;
				uint8_t i=SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id);
				sll_identifier_index_t j=SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id);
				if (i==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
					if (*(g_dt->rm.l+j)==o){
						ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL;
					}
					ai->dt.v=(g_dt->it.l_im+j)->v;
				}
				else{
					if (*(g_dt->rm.s[i]+j)==o){
						ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL;
					}
					ai->dt.v=(g_dt->it.s_im[i]+j)->v;
				}
				GENERATE_OPCODE_WITH_LABEL(g_dt,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ),lbl);
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
				return o+1;
			}
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
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_OR:
			SLL_UNIMPLEMENTED();
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
		case SLL_NODE_TYPE_INLINE_FUNC:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_IF:
		case SLL_NODE_TYPE_SWITCH:
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_RETURN:
		case SLL_NODE_TYPE_EXIT:
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
		case SLL_NODE_TYPE_BREAK:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_CONTINUE:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_REF:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_COMMA:
			SLL_UNIMPLEMENTED();
	}
	o=_generate_on_stack(o,g_dt);
	GENERATE_OPCODE_WITH_LABEL(g_dt,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ),lbl);
	GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
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
	return o;
}



static const sll_node_t* _generate_inline_function(const sll_node_t* o,assembly_generator_data_t* g_dt){
	SLL_ASSERT(o->t==SLL_NODE_TYPE_INLINE_FUNC);
	return_table_t rt={
		sll_allocate((g_dt->rt->sz+1)*sizeof(assembly_instruction_label_t)),
		g_dt->rt->sz+1
	};
	for (uint32_t i=0;i<g_dt->rt->sz;i++){
		*(rt.dt+i)=*(g_dt->rt->dt+i);
	}
	assembly_instruction_label_t e=NEXT_LABEL(g_dt);
	*(rt.dt+rt.sz-1)=e;
	return_table_t* p_rt=g_dt->rt;
	g_dt->rt=&rt;
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_generate(o,g_dt);
	}
	GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
	DEFINE_LABEL(g_dt,e);
	sll_deallocate(rt.dt);
	g_dt->rt=p_rt;
	return o;
}



static const sll_node_t* _generate_on_stack(const sll_node_t* o,assembly_generator_data_t* g_dt){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DEBUG_DATA||o->t==NODE_TYPE_CHANGE_STACK){
		o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_NODE_TYPE_UNKNOWN:
			return o+1;
		case SLL_NODE_TYPE_CHAR:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR;
				ai->dt.c=o->dt.c;
				return o+1;
			}
		case SLL_NODE_TYPE_INT:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT;
				ai->dt.i=o->dt.i;
				return o+1;
			}
		case SLL_NODE_TYPE_FLOAT:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT;
				ai->dt.f=o->dt.f;
				return o+1;
			}
		case SLL_NODE_TYPE_STRING:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS;
				ai->dt.s=o->dt.s;
				return o+1;
			}
		case SLL_NODE_TYPE_ARRAY:
			{
				if (!o->dt.al){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO);
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
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				if (!o->dt.ml){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP_ZERO);
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
				return o;
			}
		case SLL_NODE_TYPE_IDENTIFIER:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD;
				uint8_t i=SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id);
				sll_identifier_index_t j=SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id);
				if (i==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
					if (*(g_dt->rm.l+j)==o){
						ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL;
					}
					ai->dt.v=(g_dt->it.l_im+j)->v;
				}
				else{
					if (*(g_dt->rm.s[i]+j)==o){
						ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL;
					}
					ai->dt.v=(g_dt->it.s_im[i]+j)->v;
				}
				return o+1;
			}
		case SLL_NODE_TYPE_FUNCTION_ID:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT;
				ai->dt.i=o->dt.fn_id+1;
				return o+1;
			}
		case SLL_NODE_TYPE_PRINT:
		case SLL_NODE_TYPE_IF:
		case SLL_NODE_TYPE_SWITCH:
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_RETURN:
		case SLL_NODE_TYPE_EXIT:
		case SLL_NODE_TYPE_OPERATION_LIST:
			o=_generate(o,g_dt);
			GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
			return o;
		case SLL_NODE_TYPE_AND:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_OR:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_NOT:
			{
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
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
			{
				sll_arg_count_t l=o->dt.ac;
				o++;
				SLL_ASSERT(l>=2);
				const sll_node_t* io=o;
				SLL_ASSERT(io->t==SLL_NODE_TYPE_IDENTIFIER);
				o=sll_skip_node_const(o);
				o=_generate_on_stack(o,g_dt);
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE;
				uint8_t i=SLL_IDENTIFIER_GET_ARRAY_ID(io->dt.id);
				sll_identifier_index_t j=SLL_IDENTIFIER_GET_ARRAY_INDEX(io->dt.id);
				if (i==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
					if (*(g_dt->rm.l+j)==io){
						ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
					}
					else{
						ai->dt.v=(g_dt->it.l_im+j)->v;
					}
				}
				else{
					if (*(g_dt->rm.s[i]+j)==io){
						ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
					}
					else{
						ai->dt.v=(g_dt->it.s_im[i]+j)->v;
					}
				}
				l-=2;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT;
				ai->dt.i=(o->t==SLL_NODE_TYPE_FUNC?o->dt.fn.id+1:~((sll_integer_t)o->dt.fn.id));
				return sll_skip_node_const(o);
			}
		case SLL_NODE_TYPE_INLINE_FUNC:
			return _generate_inline_function(o,g_dt);
		case SLL_NODE_TYPE_CALL:
			return _generate_call(o,g_dt);
		case SLL_NODE_TYPE_INLINE_IF:
			{
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
					ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT;
					ai->dt.i=0;
					return o+1;
				}
				if (l==1){
					o=_generate(o+1,g_dt);
					sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
					ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT;
					ai->dt.i=0;
					return o;
				}
				o++;
				assembly_instruction_label_t e=NEXT_LABEL(g_dt);
				if (l&1){
					l>>=1;
					while (l){
						l--;
						assembly_instruction_label_t nxt=NEXT_LABEL(g_dt);
						o=_generate_jump(o,g_dt,nxt,1);
						o=_generate_on_stack(o,g_dt);
						sll_assembly_instruction_t* p_ai=_get_previous_instruction(g_dt);
						if (p_ai->t!=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET&&p_ai->t!=SLL_ASSEMBLY_INSTRUCTION_TYPE_END){
							GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,e);
						}
						DEFINE_LABEL(g_dt,nxt);
					}
					o=_generate_on_stack(o,g_dt);
				}
				else{
					l>>=1;
					while (l){
						l--;
						assembly_instruction_label_t nxt=NEXT_LABEL(g_dt);
						o=_generate_jump(o,g_dt,nxt,1);
						o=_generate_on_stack(o,g_dt);
						sll_assembly_instruction_t* p_ai=_get_previous_instruction(g_dt);
						if (p_ai->t!=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET&&p_ai->t!=SLL_ASSEMBLY_INSTRUCTION_TYPE_END){
							GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,e);
						}
						DEFINE_LABEL(g_dt,nxt);
					}
					sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
					ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT;
					ai->dt.i=0;
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
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_STRICT_NOT_EQUAL:
			SLL_UNIMPLEMENTED();
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
				}
				else if (ac==3){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_TWO);
				}
				else{
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_THREE);
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
		case SLL_NODE_TYPE_CAST:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				if (l==1){
					return _generate(o+1,g_dt);
				}
				o=_generate_on_stack(o+1,g_dt);
				l--;
				while (l){
					l--;
					while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DEBUG_DATA||o->t==NODE_TYPE_CHANGE_STACK){
						o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
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
					}
				}
				return o;
			}
		case SLL_NODE_TYPE_TYPEOF:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				o=_generate_on_stack(o+1,g_dt);
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_TYPEOF);
				l--;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_DECL:
			{
				sll_arg_count_t l=o->dt.ac;
				o++;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL_ZERO);
					return o;
				}
				sll_arg_count_t v=l;
				l&=0xfffffffe;
				while (l){
					l--;
					o=_generate_on_stack(o,g_dt);
				}
				if (v&1){
					o=_generate(o,g_dt);
				}
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL;
				ai->dt.ac=v>>1;
				return o;
			}
		case SLL_NODE_TYPE_NEW:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_BREAK:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_CONTINUE:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_REF:
			{
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_NULL);
					return o+1;
				}
				o=_generate_on_stack(o+1,g_dt);
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_REF);
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
	}
	sll_arg_count_t l=o->dt.ac;
	sll_assembly_instruction_type_t t=o->t+(SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD-SLL_NODE_TYPE_ADD);
	SLL_ASSERT(l);
	if (l==1){
		return _generate(o+1,g_dt);
	}
	o=_generate_on_stack(o+1,g_dt);
	l--;
	while (l){
		l--;
		o=_generate_on_stack(o,g_dt);
		GENERATE_OPCODE(g_dt,t);
	}
	return o;
}



static const sll_node_t* _mark_loop_delete(const sll_node_t* o,const assembly_generator_data_t* g_dt,uint64_t* v_st,sll_scope_t sc){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DEBUG_DATA||o->t==NODE_TYPE_CHANGE_STACK){
		o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_NODE_TYPE_UNKNOWN:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_MAP:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_IDENTIFIER:
			{
				sll_identifier_index_t i=SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id);
				uint8_t j=SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id);
				if (j==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
					sll_identifier_t* id=g_dt->c_dt->idt.il+i;
					if (id->sc<=sc&&*(g_dt->rm.l+i)==o){
						*(g_dt->rm.l+i)=VARIABLE_OFFSET_NEVER_DELETE;
						sll_variable_index_t k=(g_dt->it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id))->v;
						*(v_st+(k>>6))|=1ull<<(k&63);
					}
				}
				else{
					sll_identifier_t* id=g_dt->c_dt->idt.s[j].dt+i;
					if (id->sc<=sc&&*(g_dt->rm.s[j]+i)==o){
						*(g_dt->rm.s[j]+i)=VARIABLE_OFFSET_NEVER_DELETE;
						sll_variable_index_t k=(g_dt->it.s_im[j]+SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id))->v;
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
		case SLL_NODE_TYPE_INLINE_FUNC:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
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



static const sll_node_t* _generate(const sll_node_t* o,assembly_generator_data_t* g_dt){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DEBUG_DATA||o->t==NODE_TYPE_CHANGE_STACK){
		o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_NODE_TYPE_UNKNOWN:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t al=o->dt.al;
				o++;
				for (sll_array_length_t i=0;i<al;i++){
					o=_generate(o,g_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_IDENTIFIER:
			{
				uint8_t i=SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id);
				sll_identifier_index_t j=SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id);
				if (i==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
					if (*(g_dt->rm.l+j)==o){
						sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
						ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL;
						ai->dt.v=(g_dt->it.l_im+j)->v;
					}
				}
				else{
					if (*(g_dt->rm.s[i]+j)==o){
						sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
						ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL;
						ai->dt.v=(g_dt->it.s_im[i]+j)->v;
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
				}
				return o;
			}
		case SLL_NODE_TYPE_AND:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_OR:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_ASSIGN:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l>=2);
				o++;
				while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DEBUG_DATA||o->t==NODE_TYPE_CHANGE_STACK){
					o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				const sll_node_t* io=o;
				SLL_ASSERT(io->t==SLL_NODE_TYPE_IDENTIFIER);
				uint8_t i=SLL_IDENTIFIER_GET_ARRAY_ID(io->dt.id);
				sll_identifier_index_t j=SLL_IDENTIFIER_GET_ARRAY_INDEX(io->dt.id);
				if ((i==SLL_MAX_SHORT_IDENTIFIER_LENGTH&&*(g_dt->rm.l+j)==io)||(i!=SLL_MAX_SHORT_IDENTIFIER_LENGTH&&*(g_dt->rm.s[i]+j)==io)){
					o++;
					l--;
					while (l){
						l--;
						o=_generate(o,g_dt);
					}
					return o;
				}
				o=_generate_on_stack(o+1,g_dt);
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP;
				ai->dt.v=GET_VARIABLE_INDEX(io,g_dt);
				l-=2;
				while (l){
					l--;
					o=_generate(o,g_dt);
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
					o=_generate(o,g_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_INLINE_FUNC:
			o=_generate_inline_function(o,g_dt);
			GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
			return o;
		case SLL_NODE_TYPE_CALL:
			o=_generate_call(o,g_dt);
			GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
			return o;
		case SLL_NODE_TYPE_IF:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				if (l==1){
					return _generate(o+1,g_dt);
				}
				o++;
				if (l&1){
					assembly_instruction_label_t e=NEXT_LABEL(g_dt);
					l>>=1;
					while (l){
						l--;
						assembly_instruction_label_t nxt=NEXT_LABEL(g_dt);
						o=_generate_jump(o,g_dt,nxt,1);
						o=_generate(o,g_dt);
						sll_assembly_instruction_t* p_ai=_get_previous_instruction(g_dt);
						if (p_ai->t!=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET&&p_ai->t!=SLL_ASSEMBLY_INSTRUCTION_TYPE_END){
							GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,e);
						}
						DEFINE_LABEL(g_dt,nxt);
					}
					o=_generate(o,g_dt);
					DEFINE_LABEL(g_dt,e);
				}
				else{
					assembly_instruction_label_t e=NEXT_LABEL(g_dt);
					l>>=1;
					while (l){
						l--;
						assembly_instruction_label_t nxt=NEXT_LABEL(g_dt);
						o=_generate_jump(o,g_dt,nxt,1);
						o=_generate(o,g_dt);
						if (l){
							sll_assembly_instruction_t* p_ai=_get_previous_instruction(g_dt);
							if (p_ai->t!=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET&&p_ai->t!=SLL_ASSEMBLY_INSTRUCTION_TYPE_END){
								GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,e);
							}
						}
						DEFINE_LABEL(g_dt,nxt);
					}
					DEFINE_LABEL(g_dt,e);
				}
				return o;
			}
		case SLL_NODE_TYPE_INLINE_IF:
			o=_generate_on_stack(o,g_dt);
			GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
			return o;
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
				object_label_t* m=sll_allocate((l+1)*sizeof(object_label_t));
				sll_arg_count_t i=0;
				while (l){
					l--;
					assembly_instruction_label_t j=NEXT_LABEL(g_dt);
					o=_generate_on_stack(o,g_dt);
					GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL,j);
					(m+i)->o=o;
					(m+i)->l=j;
					i++;
					o=sll_skip_node_const(o);
				}
				assembly_instruction_label_t e=NEXT_LABEL(g_dt);
				if (d){
					assembly_instruction_label_t j=NEXT_LABEL(g_dt);
					GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL,j);
					(m+i)->o=o;
					(m+i)->l=j;
				}
				else{
					GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL,e);
					(m+i)->o=NULL;
					(m+i)->l=e;
				}
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_JT;
				ai->dt.al=i;
				o=r+1;
				for (sll_arg_count_t j=0;j<=i;j++){
					if ((m+j)->o){
						DEFINE_LABEL(g_dt,(m+j)->l);
						o=_generate((m+j)->o,g_dt);
						if (j<i-(d?1:0)){
							GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,e);
						}
					}
				}
				sll_deallocate(m);
				DEFINE_LABEL(g_dt,e);
				return o;
			}
		case SLL_NODE_TYPE_FOR:
			{
				sll_arg_count_t l=o->dt.l.ac;
				SLL_ASSERT(l);
				if (l==1){
					return _generate(o+1,g_dt);
				}
				sll_scope_t sc=o->dt.l.sc;
				o=_generate(o+1,g_dt);
				const sll_node_t* cnd=o;
				loop_table_t lt={
					sll_allocate((g_dt->lt->sz+1)*sizeof(loop_t)),
					g_dt->lt->sz+1
				};
				for (uint32_t i=0;i<g_dt->lt->sz;i++){
					*(lt.dt+i)=*(g_dt->lt->dt+i);
				}
				loop_table_t* p_lt=g_dt->lt;
				g_dt->lt=&lt;
				assembly_instruction_label_t s=NEXT_LABEL(g_dt);
				assembly_instruction_label_t c=NEXT_LABEL(g_dt);
				assembly_instruction_label_t e=NEXT_LABEL(g_dt);
				(lt.dt+lt.sz-1)->s=c;
				(lt.dt+lt.sz-1)->e=e;
				uint64_t* v_st=sll_zero_allocate(((g_dt->a_dt->vc>>6)+1)*sizeof(uint64_t));
				for (sll_arg_count_t i=1;i<l;i++){
					o=_mark_loop_delete(o,g_dt,v_st,sc);
				}
				o=_generate_jump(cnd,g_dt,e,1);
				DEFINE_LABEL(g_dt,s);
				l-=2;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				DEFINE_LABEL(g_dt,c);
				_generate_jump(cnd,g_dt,s,0);
				sll_deallocate(lt.dt);
				g_dt->lt=p_lt;
				DEFINE_LABEL(g_dt,e);
				for (sll_variable_index_t i=0;i<(g_dt->a_dt->vc>>6)+1;i++){
					uint64_t v=*(v_st+i);
					while (v){
						sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
						ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL;
						ai->dt.v=FIND_FIRST_SET_BIT(v)|(i<<6);
						v&=v-1;
					}
				}
				sll_deallocate(v_st);
				return o;
			}
		case SLL_NODE_TYPE_WHILE:
			{
				sll_arg_count_t l=o->dt.l.ac;
				SLL_ASSERT(l);
				if (l==1){
					return _generate(o+1,g_dt);
				}
				sll_scope_t sc=o->dt.l.sc;
				o=_generate(o+1,g_dt);
				const sll_node_t* cnd=o;
				loop_table_t lt={
					sll_allocate((g_dt->lt->sz+1)*sizeof(loop_t)),
					g_dt->lt->sz+1
				};
				for (uint32_t i=0;i<g_dt->lt->sz;i++){
					*(lt.dt+i)=*(g_dt->lt->dt+i);
				}
				loop_table_t* p_lt=g_dt->lt;
				g_dt->lt=&lt;
				assembly_instruction_label_t s=NEXT_LABEL(g_dt);
				assembly_instruction_label_t c=NEXT_LABEL(g_dt);
				assembly_instruction_label_t e=NEXT_LABEL(g_dt);
				(lt.dt+lt.sz-1)->s=c;
				(lt.dt+lt.sz-1)->e=e;
				uint64_t* v_st=sll_zero_allocate(((g_dt->a_dt->vc>>6)+1)*sizeof(uint64_t));
				o=_mark_loop_delete(cnd,g_dt,v_st,o->dt.l.sc);
				l-=2;
				const sll_node_t* j=o;
				for (sll_arg_count_t i=0;i<l;i++){
					j=_mark_loop_delete(j,g_dt,v_st,sc);
				}
				DEFINE_LABEL(g_dt,s);
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				DEFINE_LABEL(g_dt,c);
				_generate_jump(cnd,g_dt,s,0);
				sll_deallocate(lt.dt);
				g_dt->lt=p_lt;
				DEFINE_LABEL(g_dt,e);
				for (sll_variable_index_t i=0;i<(g_dt->a_dt->vc>>6)+1;i++){
					uint64_t v=*(v_st+i);
					while (v){
						sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
						ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL;
						ai->dt.v=FIND_FIRST_SET_BIT(v)|(i<<6);
						v&=v-1;
					}
				}
				sll_deallocate(v_st);
				return o;
			}
		case SLL_NODE_TYPE_LOOP:
			SLL_UNIMPLEMENTED();
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
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_STRICT_NOT_EQUAL:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_BREAK:
			{
				SLL_ASSERT(g_dt->lt->sz);
				sll_arg_count_t l=o->dt.ac;
				o++;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,(g_dt->lt->dt+g_dt->lt->sz-1)->e);
				return o;
			}
		case SLL_NODE_TYPE_CONTINUE:
			{
				SLL_ASSERT(g_dt->lt->sz);
				sll_arg_count_t l=o->dt.ac;
				o++;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,(g_dt->lt->dt+g_dt->lt->sz-1)->s);
				return o;
			}
		case SLL_NODE_TYPE_REF:
			{
				sll_arg_count_t l=o->dt.ac;
				o++;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				return o;
			}
		case SLL_NODE_TYPE_RETURN:
			{
				sll_arg_count_t l=o->dt.ac;
				o++;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
				}
				else{
					o=_generate_on_stack(o,g_dt);
					l--;
					while (l){
						l--;
						o=_generate(o,g_dt);
					}
				}
				SLL_ASSERT(g_dt->rt->sz);
				assembly_instruction_label_t t=*(g_dt->rt->dt+g_dt->rt->sz-1);
				sll_assembly_instruction_t* ai=_acquire_next_instruction(g_dt->a_dt);
				if (t==MAX_ASSEMBLY_INSTRUCTION_LABEL){
					ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET;
				}
				else{
					ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP|ASSEMBLY_INSTRUCTION_LABEL;
					ASSEMBLY_INSTRUCTION_MISC_FIELD(ai)=t;
				}
				return o;
			}
		case SLL_NODE_TYPE_EXIT:
			{
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_END);
					return o+1;
				}
				o=_generate_on_stack(o+1,g_dt);
				l--;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_END);
				return o;
			}
		case SLL_NODE_TYPE_COMMA:
		case SLL_NODE_TYPE_OPERATION_LIST:
			{
				sll_arg_count_t l=o->dt.ac;
				o++;
				while (l){
					l--;
					o=_generate(o,g_dt);
				}
				return o;
			}
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
	sll_free_string_table(&(a_dt->st));
	void* pg=a_dt->_s.s;
	sll_page_size_t sz=sll_platform_get_page_size()*ASSEMBLY_INSTRUCTION_STACK_PAGE_ALLOC_COUNT;
	while (pg){
		void* n=*((void**)pg);
		sll_platform_free_page(pg,sz);
		pg=n;
	}
	a_dt->_s.s=NULL;
	a_dt->_s.e=NULL;
	a_dt->_s.c=0;
	a_dt->_s.p=NULL;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_generate_assembly(const sll_compilation_data_t* c_dt,sll_assembly_data_t* o,sll_error_t* e){
	if (!c_dt->_s.s){
		e->t=SLL_ERROR_NO_STACK;
		return 0;
	}
	_init_assembly_stack(o);
	o->tm=sll_platform_get_current_time();
	o->h=o->_s.p;
	o->ic=0;
	o->vc=0;
	o->st.l=c_dt->st.l;
	o->st.dt=sll_allocate(o->st.l*sizeof(sll_string_t));
	for (sll_string_index_t i=0;i<o->st.l;i++){
		sll_string_clone(c_dt->st.dt+i,o->st.dt+i);
	}
	loop_table_t g_dt_lt={
		NULL,
		0
	};
	return_table_t g_dt_rt={
		NULL,
		0
	};
	assembly_generator_data_t g_dt={
		o,
		c_dt,
		{
			.l_im=sll_allocate(c_dt->idt.ill*sizeof(identifier_data_t)),
			.n_vi=0,
			.l_sc=0,
			.sc_vi=sll_allocate(c_dt->_n_sc_id*sizeof(sll_variable_index_t)),
			.vc=0
		},
		0,
		{
			.l=sll_allocate(c_dt->idt.ill*sizeof(identifier_data_t))
		},
		&g_dt_lt,
		&g_dt_rt
	};
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		g_dt.it.s_im[i]=sll_allocate(c_dt->idt.s[i].l*sizeof(identifier_data_t));
		g_dt.rm.s[i]=sll_allocate(c_dt->idt.s[i].l*sizeof(identifier_data_t));
		for (sll_identifier_list_length_t j=0;j<c_dt->idt.s[i].l;j++){
			(g_dt.it.s_im[i]+j)->v=SLL_MAX_VARIABLE_INDEX;
			*(g_dt.rm.s[i]+j)=NULL;
		}
	}
	for (sll_identifier_list_length_t i=0;i<c_dt->idt.ill;i++){
		(g_dt.it.l_im+i)->v=SLL_MAX_VARIABLE_INDEX;
		*(g_dt.rm.l+i)=NULL;
	}
	for (sll_scope_t i=0;i<c_dt->_n_sc_id;i++){
		*(g_dt.it.sc_vi+i)=SLL_MAX_VARIABLE_INDEX;
	}
	_map_identifiers(c_dt->h,c_dt,&g_dt,SLL_MAX_SCOPE);
	for (sll_function_index_t i=0;i<c_dt->ft.l;i++){
		const sll_function_t* k=*(c_dt->ft.dt+i);
		for (sll_arg_count_t j=0;j<k->al;j++){
			sll_identifier_index_t ii=k->a[j];
			if (SLL_IDENTIFIER_GET_ARRAY_ID(ii)==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
				(g_dt.it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX(ii))->v=g_dt.it.vc;
			}
			else{
				(g_dt.it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID(ii)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(ii))->v=g_dt.it.vc;
			}
			g_dt.it.vc++;
		}
		g_dt.it.n_vi=g_dt.it.vc;
		g_dt.it.l_sc=0;
		const sll_node_t* fo=_get_node_at_offset(c_dt,k->off);
		SLL_ASSERT(fo->t==SLL_NODE_TYPE_FUNC);
		sll_arg_count_t ac=fo->dt.fn.ac;
		fo++;
		while (ac){
			ac--;
			fo=_map_identifiers(fo,c_dt,&g_dt,fo->dt.fn.sc);
		}
	}
	sll_deallocate(g_dt.it.sc_vi);
	o->vc=g_dt.it.vc;
	_generate(c_dt->h,&g_dt);
	if (_get_previous_instruction(&g_dt)->t!=SLL_ASSEMBLY_INSTRUCTION_TYPE_END){
		GENERATE_OPCODE(&g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_END_ZERO);
	}
	o->ft.l=c_dt->ft.l;
	o->ft.dt=sll_allocate(c_dt->ft.l*sizeof(sll_instruction_index_t));
	g_dt_rt.dt=sll_allocate(sizeof(assembly_instruction_label_t));
	*(g_dt_rt.dt)=MAX_ASSEMBLY_INSTRUCTION_LABEL;
	g_dt_rt.sz=1;
	for (sll_function_index_t i=0;i<c_dt->ft.l;i++){
		const sll_function_t* k=*(c_dt->ft.dt+i);
		sll_arg_count_t j=k->al;
		sll_assembly_instruction_t* ai=_acquire_next_instruction(o);
		ai->t=ASSEMBLY_INSTRUCTION_TYPE_FUNC_START;
		ASSEMBLY_INSTRUCTION_MISC_FIELD(ai)=i;
		while (j){
			j--;
			ai=_acquire_next_instruction(o);
			ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP;
			ai->dt.v=(SLL_IDENTIFIER_GET_ARRAY_ID(k->a[j])==SLL_MAX_SHORT_IDENTIFIER_LENGTH?(g_dt.it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX(k->a[j]))->v:(g_dt.it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID(k->a[j])]+SLL_IDENTIFIER_GET_ARRAY_INDEX(k->a[j]))->v);
		}
		const sll_node_t* fo=_get_node_at_offset(c_dt,k->off);
		SLL_ASSERT(fo->t==SLL_NODE_TYPE_FUNC);
		sll_arg_count_t ac=fo->dt.fn.ac;
		fo++;
		while (ac){
			ac--;
			fo=_generate(fo,&g_dt);
		}
		if (_get_previous_instruction(&g_dt)->t!=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET){
			GENERATE_OPCODE(&g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO);
		}
	}
	sll_deallocate(g_dt_rt.dt);
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
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
	for (sll_instruction_index_t i=0;i<o->ic;i++){
		if (ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP){
			ai++;
			if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
				ai=ai->dt._p;
			}
			continue;
		}
		st[4]=st[3];
		st[3]=st[2];
		st[2]=st[1];
		st[1]=st[0];
		st[0]=ai;
		if (st[0]->t!=SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT){
			if ((st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT&&!st[1]->dt.i)||(st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT&&!st[1]->dt.f)){
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO;
			}
			else if ((st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT&&st[1]->dt.i==1)||(st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT&&st[1]->dt.f==1)){
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE;
			}
			else if ((st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT&&st[1]->dt.i==2)||(st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT&&st[1]->dt.f==2)){
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO;
			}
			else if ((st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT&&st[1]->dt.i==3)||(st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT&&st[1]->dt.f==3)){
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE;
			}
			else if ((st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT&&st[1]->dt.i==4)||(st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT&&st[1]->dt.f==4)){
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR;
			}
			else if ((st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT&&st[1]->dt.i==-1)||(st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT&&st[1]->dt.f==-1)){
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE;
			}
		}
		if (st[0]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_POP&&st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT){
			st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP;
			SHIFT_UP(st);
		}
		else if (st[0]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_POP&&st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL){
			st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP;
			SHIFT_UP(st);
		}
		else if (st[0]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD&&st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP&&st[0]->dt.v==st[1]->dt.v){
			st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE;
			SHIFT_UP(st);
		}
		else if (st[0]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL&&st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP&&st[0]->dt.v==st[1]->dt.v){
			st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			SHIFT_UP_TWO(st);
		}
		else if (st[0]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP&&st[1]->t>=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE&&st[1]->t<=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR){
			st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			st[1]->t+=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE-SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE;
			st[1]->dt.v=st[0]->dt.v;
			SHIFT_UP(st);
		}
		else if (st[0]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP&&st[1]->t>=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT&&st[1]->t<=SLL_ASSEMBLY_INSTRUCTION_TYPE_INV&&st[2]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD&&st[0]->dt.v==st[2]->dt.v){
			st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			st[2]->t=st[1]->t|SLL_ASSEMBLY_INSTRUCTION_INPLACE;
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			SHIFT_UP_TWO(st);
		}
		else if (st[0]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT&&st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR){
			st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR;
			SHIFT_UP(st);
		}
		else if (st[0]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT&&st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD){
			st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR;
			SHIFT_UP(st);
		}
		else if (st[0]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT&&st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS){
			st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR;
			SHIFT_UP(st);
		}
		else if (st[0]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL&&st[0]->dt.ac<2){
			if (st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT){
				st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				st[1]->t=(st[0]->dt.ac?SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO);
				SHIFT_UP(st);
			}
			else if (st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE){
				st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				st[1]->t=(st[0]->dt.ac?SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO);
				st[1]->dt.i=-1;
				SHIFT_UP(st);
			}
			else if (st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO){
				st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				st[1]->t=(st[0]->dt.ac?SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO);
				st[1]->dt.i=0;
				SHIFT_UP(st);
			}
			else if (st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE){
				st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				st[1]->t=(st[0]->dt.ac?SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO);
				st[1]->dt.i=1;
				SHIFT_UP(st);
			}
			else if (st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO){
				st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				st[1]->t=(st[0]->dt.ac?SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO);
				st[1]->dt.i=2;
				SHIFT_UP(st);
			}
			else if (st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE){
				st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				st[1]->t=(st[0]->dt.ac?SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO);
				st[1]->dt.i=3;
				SHIFT_UP(st);
			}
			else if (st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR){
				st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				st[1]->t=(st[0]->dt.ac?SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO);
				st[1]->dt.i=4;
				SHIFT_UP(st);
			}
		}
		else if ((st[0]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD||st[0]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_SUB)&&(st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE||st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE)){
			st[1]->t=(((st[0]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD)^(st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE))?SLL_ASSEMBLY_INSTRUCTION_TYPE_INC:SLL_ASSEMBLY_INSTRUCTION_TYPE_DEC);
			st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			SHIFT_UP(st);
		}
		else if (st[0]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_RET){
			if (st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT){
				st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT;
				SHIFT_UP(st);
			}
			else if (st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE){
				st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT;
				st[1]->dt.i=-1;
				SHIFT_UP(st);
			}
			else if (st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO){
				st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT;
				st[1]->dt.i=0;
				SHIFT_UP(st);
			}
			else if (st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE){
				st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT;
				st[1]->dt.i=1;
				SHIFT_UP(st);
			}
			else if (st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO){
				st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT;
				st[1]->dt.i=2;
				SHIFT_UP(st);
			}
			else if (st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE){
				st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT;
				st[1]->dt.i=3;
				SHIFT_UP(st);
			}
			else if (st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR){
				st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT;
				st[1]->dt.i=4;
				SHIFT_UP(st);
			}
			else if (st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT){
				st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT;
				SHIFT_UP(st);
			}
			else if (st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR){
				st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR;
				SHIFT_UP(st);
			}
			else if (st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS){
				st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR;
				SHIFT_UP(st);
			}
			else if (st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD){
				st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR;
				SHIFT_UP(st);
			}
		}
		else if (st[0]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_END&&(st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO||st[1]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE)){
			st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			st[1]->t=(st[0]->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO?SLL_ASSEMBLY_INSTRUCTION_TYPE_END_ZERO:SLL_ASSEMBLY_INSTRUCTION_TYPE_END_ONE);
			SHIFT_UP(st);
		}
		ai++;
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			ai=ai->dt._p;
		}
	}
	strint_map_data_t sm={
		(o->st.l>>6)+1,
		sll_allocate(((o->st.l>>6)+1)*sizeof(uint64_t))
	};
	for (sll_string_index_t i=0;i<sm.ml;i++){
		*(sm.m+i)=0;
	}
	ai=o->h;
	sll_assembly_instruction_t* s=ai;
	sll_instruction_index_t* lbl=sll_allocate(g_dt.n_lbl*sizeof(sll_instruction_index_t));
	for (sll_instruction_index_t i=0;i<o->ic;i++){
		if (s->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP){
_handle_nop:;
			s++;
			if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(s)==ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
				s=s->dt._p;
			}
			i--;
			o->ic--;
			continue;
		}
		*ai=*s;
		if (ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS||ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR||ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR){
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
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			ai=ai->dt._p;
		}
		s++;
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(s)==ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			s=s->dt._p;
		}
	}
	sm.im=sll_allocate(o->st.l*sizeof(sll_string_t*));
	sll_string_index_t k=0;
	sll_string_index_t l=0;
	for (sll_string_index_t i=0;i<sm.ml;i++){
		uint64_t v=~(*(sm.m+i));
		while (v){
			sll_string_index_t j=FIND_FIRST_SET_BIT(v)|(i<<6);
			if (j==o->st.l){
				break;
			}
			for (sll_string_index_t n=k;n<j;n++){
				*(o->st.dt+n-l)=*(o->st.dt+n);
				*(sm.im+n)=n-l;
			}
			sll_deallocate((o->st.dt+j)->v);
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
	ai=o->h;
	for (sll_instruction_index_t i=0;i<o->ic;i++){
		if ((SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL||(SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)>=SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP&&SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)<=SLL_ASSEMBLY_INSTRUCTION_TYPE_JSNE))&&(ai->t&ASSEMBLY_INSTRUCTION_LABEL)){
			ai->t&=~ASSEMBLY_INSTRUCTION_LABEL;
			sll_instruction_index_t j=*(lbl+ASSEMBLY_INSTRUCTION_MISC_FIELD(ai));
			if (j<128){
				ai->dt.j=j;
			}
			else{
				sll_relative_instruction_index_t off=((sll_relative_instruction_index_t)j)-i;
				if (GET_SIGN_ENCODED_INTEGER((int64_t)off)<(int64_t)j){
					ai->t|=SLL_ASSEMBLY_INSTRUCTION_RELATIVE;
					ai->dt.rj=off;
				}
				else{
					ai->dt.j=j;
				}
			}
		}
		else if (ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS||ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR||ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR){
			ai->dt.s=*(sm.im+ai->dt.s);
		}
		ai++;
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			ai=ai->dt._p;
		}
	}
	sll_deallocate(lbl);
	sll_deallocate(sm.im);
	return 1;
}
