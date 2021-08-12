#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/constants.h>
#include <sll/core.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>



#define DEFINE_LABEL(g_dt,id) \
	do{ \
		sll_assembly_instruction_t* __ai=NEXT_INSTRUCTION((g_dt)); \
		__ai->t=ASSEMBLY_INSTRUCTION_TYPE_LABEL_TARGET; \
		ASSEMBLY_INSTRUCTION_MISC_FIELD(__ai)=(id); \
	} while(0)
#define GENERATE_OPCODE(g_dt,op) (NEXT_INSTRUCTION((g_dt))->t=(op))
#define GENERATE_OPCODE_WITH_LABEL(g_dt,op,lbl) \
	do{ \
		sll_assembly_instruction_t* __ai=NEXT_INSTRUCTION((g_dt)); \
		__ai->t=(op)|ASSEMBLY_INSTRUCTION_LABEL; \
		ASSEMBLY_INSTRUCTION_MISC_FIELD(__ai)=(lbl); \
	} while(0)
#define GET_SIGN_ENCODED_INTEGER(x) ((x)<0?((~(x))<<1)|1:(x)<<1)
#define GET_VARIABLE_INDEX(o,g_dt) (SLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)==SLL_MAX_SHORT_IDENTIFIER_LENGTH?((g_dt)->it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->v:((g_dt)->it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID((o)->dt.id)]+SLL_IDENTIFIER_GET_ARRAY_INDEX((o)->dt.id))->v)
#define NEXT_INSTRUCTION(g_dt) ((g_dt)->a_dt->h+((g_dt)->a_dt->ic++))
#define NEXT_LABEL(g_dt) ((g_dt)->n_lbl++)



sll_object_offset_t _generate_on_stack(const sll_object_t* o,assembly_generator_data_t* g_dt);



sll_object_offset_t _generate(const sll_object_t* o,assembly_generator_data_t* g_dt);



sll_object_offset_t _map_identifiers_extra(const sll_object_t* o,const sll_compilation_data_t* c_dt,assembly_generator_data_t* g_dt){
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
		case SLL_OBJECT_TYPE_IDENTIFIER:
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
					(g_dt->it.l_im+i)->c++;
					if (*(g_dt->rm.l+i)<(void*)o){
						*(g_dt->rm.l+i)=(void*)o;
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
					(g_dt->it.s_im[j]+i)->c++;
					if (*(g_dt->rm.s[j]+i)<(void*)o){
						*(g_dt->rm.s[j]+i)=(void*)o;
					}
					if ((g_dt->it.s_im[j]+i)->v==SLL_MAX_VARIABLE_INDEX){
						(g_dt->it.s_im[j]+i)->v=g_dt->it.n_vi;
						g_dt->it.n_vi++;
						if (g_dt->it.n_vi>g_dt->it.vc){
							g_dt->it.vc=g_dt->it.n_vi;
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
					off+=_map_identifiers_extra(o+off,c_dt,g_dt);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_object_offset_t off=1;
				sll_statement_count_t l=o->dt.sc;
				while (l){
					l--;
					off+=_map_identifiers_extra(o+off,c_dt,g_dt);
				}
				return off+eoff;
			}
	}
	sll_object_offset_t off=1;
	sll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		off+=_map_identifiers_extra(o+off,c_dt,g_dt);
	}
	return off+eoff;
}



sll_assembly_instruction_t* _get_previous_instruction(sll_assembly_instruction_t* ai,sll_instruction_index_t i){
	do{
		if (!i){
			return NULL;
		}
		ai--;
		i--;
	} while (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==ASSEMBLY_INSTRUCTION_TYPE_FUNC_START||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==ASSEMBLY_INSTRUCTION_TYPE_LABEL_TARGET);
	return ai;
}



sll_string_index_t _create_print_string(sll_assembly_data_t* a_dt,const sll_char_t* a,const sll_char_t* b,sll_string_length_t al,sll_string_length_t bl,sll_string_checksum_t c){
	for (sll_string_index_t i=0;i<a_dt->st.l;i++){
		sll_string_t* s=*(a_dt->st.dt+i);
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
	a_dt->st.l++;
	a_dt->st.dt=realloc(a_dt->st.dt,a_dt->st.l*sizeof(sll_string_t*));
	sll_string_t* s=sll_string_create(al+bl);
	s->rc=1;
	s->c=c;
	memcpy(s->v,a,al);
	memcpy(s->v+al,b,bl);
	*(a_dt->st.dt+a_dt->st.l-1)=s;
	return a_dt->st.l-1;
}



sll_object_offset_t _generate_sequential_jump(const sll_object_t* o,assembly_generator_data_t* g_dt,sll_assembly_instruction_type_t t,uint8_t st){
	sll_arg_count_t l=o->dt.ac;
	SLL_ASSERT(l);
	if (l==1){
		return _generate(o+1,g_dt);
	}
	sll_stack_offset_t off=_generate_on_stack(o+1,g_dt)+1;
	assembly_instruction_label_t e=NEXT_LABEL(g_dt);
	l-=2;
	while (l){
		l--;
		off+=_generate_on_stack(o+off,g_dt);
		GENERATE_OPCODE_WITH_LABEL(g_dt,t,e);
		GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP);
	}
	off+=_generate_on_stack(o+off,g_dt);
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
	return off;
}



sll_object_offset_t _generate_cond_jump(const sll_object_t* o,assembly_generator_data_t* g_dt,assembly_instruction_label_t lbl,sll_assembly_instruction_type_t t){
	sll_arg_count_t l=o->dt.ac;
	SLL_ASSERT(l);
	if (l==1){
		return _generate(o+1,g_dt);
	}
	sll_object_offset_t off=_generate_on_stack(o+1,g_dt)+1;
	l-=2;
	while (l){
		l--;
		off+=_generate_on_stack(o+off,g_dt);
		GENERATE_OPCODE_WITH_LABEL(g_dt,t,lbl);
		GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP);
	}
	off+=_generate_on_stack(o+off,g_dt);
	GENERATE_OPCODE_WITH_LABEL(g_dt,t,lbl);
	GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO);
	return off;
}



sll_object_offset_t _generate_jump(const sll_object_t* o,assembly_generator_data_t* g_dt,assembly_instruction_label_t lbl,uint8_t inv){
	sll_object_offset_t eoff=0;
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA){
		eoff++;
		o++;
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
			return eoff+1;
		case SLL_OBJECT_TYPE_CHAR:
			if ((!!(o->dt.c))^inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return eoff+1;
		case SLL_OBJECT_TYPE_INT:
			if ((!!(o->dt.i))^inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return eoff+1;
		case SLL_OBJECT_TYPE_FLOAT:
			if ((!!(o->dt.f))^inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return eoff+1;
		case SLL_OBJECT_TYPE_STRING:
			if ((!!((*(g_dt->c_dt->st.dt+o->dt.s))->l))^inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return eoff+1;
		case SLL_OBJECT_TYPE_IDENTIFIER:
			{
				sll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD;
				uint8_t i=SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id);
				sll_identifier_index_t j=SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id);
				if (i==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
					if (*(g_dt->rm.l+j)==o){
						ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL;
					}
					ai->dt.v=(g_dt->it.l_im+j)->v;
				}
				else{
					if (*(g_dt->rm.s[i]+j)==o){
						ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL;
					}
					ai->dt.v=(g_dt->it.s_im[i]+j)->v;
				}
				GENERATE_OPCODE_WITH_LABEL(g_dt,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ),lbl);
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
				return eoff+1;
			}
		case SLL_OBJECT_TYPE_PRINT:
			{
				sll_object_offset_t off=_generate(o,g_dt);
				if (!inv){
					GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_INPUT:
			SLL_ASSERT(!"Unimplemented");
		case SLL_OBJECT_TYPE_AND:
			SLL_ASSERT(!"Unimplemented");
		case SLL_OBJECT_TYPE_OR:
			SLL_ASSERT(!"Unimplemented");
		case SLL_OBJECT_TYPE_NOT:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				sll_object_offset_t off=1;
				l--;
				while (l){
					off+=_generate(o+off,g_dt);
				}
				return off+eoff+_generate_jump(o+off,g_dt,lbl,!inv);
			}
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			if (!inv){
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
			}
			return sll_get_object_size(o)+eoff;
		case SLL_OBJECT_TYPE_IF:
		case SLL_OBJECT_TYPE_FOR:
		case SLL_OBJECT_TYPE_RETURN:
		case SLL_OBJECT_TYPE_EXIT:
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_object_offset_t off=_generate(o,g_dt);
				if (inv){
					GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,lbl);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_LESS:
			return _generate_cond_jump(o,g_dt,lbl,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:SLL_ASSEMBLY_INSTRUCTION_TYPE_JB))+eoff;
		case SLL_OBJECT_TYPE_LESS_EQUAL:
			return _generate_cond_jump(o,g_dt,lbl,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JA:SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE))+eoff;
		case SLL_OBJECT_TYPE_EQUAL:
			return _generate_cond_jump(o,g_dt,lbl,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:SLL_ASSEMBLY_INSTRUCTION_TYPE_JE))+eoff;
		case SLL_OBJECT_TYPE_NOT_EQUAL:
			return _generate_cond_jump(o,g_dt,lbl,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JE:SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE))+eoff;
		case SLL_OBJECT_TYPE_MORE:
			return _generate_cond_jump(o,g_dt,lbl,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:SLL_ASSEMBLY_INSTRUCTION_TYPE_JA))+eoff;
		case SLL_OBJECT_TYPE_MORE_EQUAL:
			return _generate_cond_jump(o,g_dt,lbl,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JB:SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE))+eoff;
	}
	sll_object_offset_t off=_generate_on_stack(o,g_dt);
	GENERATE_OPCODE_WITH_LABEL(g_dt,(inv?SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ),lbl);
	GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
	return off+eoff;
}



sll_object_offset_t _generate_call(const sll_object_t* o,assembly_generator_data_t* g_dt){
	sll_arg_count_t l=o->dt.ac;
	SLL_ASSERT(l);
	sll_object_offset_t off=sll_get_object_size(o+1)+1;
	l--;
	while (l){
		l--;
		off+=_generate_on_stack(o+off,g_dt);
	}
	_generate_on_stack(o+1,g_dt);
	sll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
	ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL;
	ai->dt.ac=o->dt.ac-1;
	return off;
}



sll_object_offset_t _generate_on_stack(const sll_object_t* o,assembly_generator_data_t* g_dt){
	sll_object_offset_t eoff=0;
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA){
		eoff++;
		o++;
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
			return eoff+1;
		case SLL_OBJECT_TYPE_CHAR:
			{
				sll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR;
				ai->dt.c=o->dt.c;
				return eoff+1;
			}
		case SLL_OBJECT_TYPE_INT:
			{
				sll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT;
				ai->dt.i=o->dt.i;
				return eoff+1;
			}
		case SLL_OBJECT_TYPE_FLOAT:
			{
				sll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT;
				ai->dt.f=o->dt.f;
				return eoff+1;
			}
		case SLL_OBJECT_TYPE_STRING:
			{
				sll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS;
				ai->dt.s=o->dt.s;
				return eoff+1;
			}
		case SLL_OBJECT_TYPE_IDENTIFIER:
			{
				sll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD;
				uint8_t i=SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id);
				sll_identifier_index_t j=SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id);
				if (i==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
					if (*(g_dt->rm.l+j)==o){
						ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL;
					}
					ai->dt.v=(g_dt->it.l_im+j)->v;
				}
				else{
					if (*(g_dt->rm.s[i]+j)==o){
						ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL;
					}
					ai->dt.v=(g_dt->it.s_im[i]+j)->v;
				}
				return eoff+1;
			}
		case SLL_OBJECT_TYPE_PRINT:
		case SLL_OBJECT_TYPE_IF:
		case SLL_OBJECT_TYPE_FOR:
		case SLL_OBJECT_TYPE_RETURN:
		case SLL_OBJECT_TYPE_EXIT:
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_object_offset_t off=_generate(o,g_dt);
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_INPUT:
			SLL_ASSERT(!"Unimplemented");
		case SLL_OBJECT_TYPE_AND:
			SLL_ASSERT(!"Unimplemented");
		case SLL_OBJECT_TYPE_OR:
			SLL_ASSERT(!"Unimplemented");
		case SLL_OBJECT_TYPE_NOT:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				sll_object_offset_t off=1;
				l--;
				while (l){
					off+=_generate(o+off,g_dt);
				}
				off+=_generate_on_stack(o+off,g_dt);
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT);
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_ASSIGN:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l>=2);
				SLL_ASSERT((o+1)->t==SLL_OBJECT_TYPE_IDENTIFIER);
				sll_object_offset_t off=_generate_on_stack(o+2,g_dt)+2;
				sll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE;
				ai->dt.v=GET_VARIABLE_INDEX(o+1,g_dt);
				l-=2;
				while (l){
					l--;
					off+=_generate(o+off,g_dt);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				sll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT;
				ai->dt.i=(o->t==SLL_OBJECT_TYPE_FUNC?o->dt.fn.id:~((sll_integer_t)o->dt.fn.id));
				return sll_get_object_size(o)+eoff;
			}
		case SLL_OBJECT_TYPE_CALL:
			return _generate_call(o,g_dt)+eoff;
		case SLL_OBJECT_TYPE_LESS:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE,1)+eoff;
		case SLL_OBJECT_TYPE_LESS_EQUAL:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JA,1)+eoff;
		case SLL_OBJECT_TYPE_EQUAL:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE,1)+eoff;
		case SLL_OBJECT_TYPE_NOT_EQUAL:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JE,1)+eoff;
		case SLL_OBJECT_TYPE_MORE:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE,1)+eoff;
		case SLL_OBJECT_TYPE_MORE_EQUAL:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JB,1)+eoff;
	}
	sll_arg_count_t l=o->dt.ac;
	SLL_ASSERT(l);
	if (l==1){
		return _generate(o+1,g_dt)+eoff;
	}
	sll_object_offset_t off=_generate_on_stack(o+1,g_dt)+1;
	l--;
	while (l){
		l--;
		off+=_generate_on_stack(o+off,g_dt);
		GENERATE_OPCODE(g_dt,o->t+(SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD-SLL_OBJECT_TYPE_ADD));
	}
	return off+eoff;
}



sll_object_offset_t _generate(const sll_object_t* o,assembly_generator_data_t* g_dt){
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
		case SLL_OBJECT_TYPE_IDENTIFIER:
			{
				uint8_t i=SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id);
				sll_identifier_index_t j=SLL_IDENTIFIER_GET_ARRAY_INDEX(o->dt.id);
				if (i==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
					if (*(g_dt->rm.l+j)==o){
						sll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
						ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL;
						ai->dt.v=(g_dt->it.l_im+j)->v;
						GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
					}
				}
				else{
					if (*(g_dt->rm.s[i]+j)==o){
						sll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
						ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL;
						ai->dt.v=(g_dt->it.s_im[i]+j)->v;
						GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
					}
				}
				return eoff+1;
			}
		case SLL_OBJECT_TYPE_PRINT:
			{
				sll_object_offset_t off=1;
				sll_arg_count_t l=o->dt.ac;
				while (l){
					l--;
					off+=_generate_on_stack(o+off,g_dt);
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_INPUT:
			SLL_ASSERT(!"Unimplemented");
		case SLL_OBJECT_TYPE_AND:
			SLL_ASSERT(!"Unimplemented");
		case SLL_OBJECT_TYPE_OR:
			SLL_ASSERT(!"Unimplemented");
		case SLL_OBJECT_TYPE_ASSIGN:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l>=2);
				sll_object_offset_t off=1;
				while ((o+off)->t==SLL_OBJECT_TYPE_NOP||(o+off)->t==SLL_OBJECT_TYPE_DEBUG_DATA){
					off++;
				}
				SLL_ASSERT((o+off)->t==SLL_OBJECT_TYPE_IDENTIFIER);
				sll_variable_index_t vi=GET_VARIABLE_INDEX(o+off,g_dt);
				off+=_generate_on_stack(o+off+1,g_dt)+1;
				sll_assembly_instruction_t* ai=NEXT_INSTRUCTION(g_dt);
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP;
				ai->dt.v=vi;
				l-=2;
				while (l){
					l--;
					off+=_generate(o+off,g_dt);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_FUNC:
			return sll_get_object_size(o)+eoff;
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				sll_object_offset_t off=1;
				sll_arg_count_t l=o->dt.fn.ac;
				while (l){
					l--;
					off+=_generate(o+off,g_dt);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_CALL:
			{
				sll_object_offset_t off=_generate_call(o,g_dt);
				GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_POP);
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_IF:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				if (l==1){
					return _generate(o+1,g_dt)+eoff+1;
				}
				sll_object_offset_t off=1;
				if (l&1){
					assembly_instruction_label_t e=NEXT_LABEL(g_dt);
					l>>=1;
					while (l){
						l--;
						assembly_instruction_label_t nxt=NEXT_LABEL(g_dt);
						off+=_generate_jump(o+off,g_dt,nxt,1);
						off+=_generate(o+off,g_dt);
						GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,e);
						DEFINE_LABEL(g_dt,nxt);
					}
					off+=_generate(o+off,g_dt);
					DEFINE_LABEL(g_dt,e);
				}
				else{
					assembly_instruction_label_t e=NEXT_LABEL(g_dt);
					l>>=1;
					while (l){
						l--;
						assembly_instruction_label_t nxt=NEXT_LABEL(g_dt);
						off+=_generate_jump(o+off,g_dt,nxt,1);
						off+=_generate(o+off,g_dt);
						if (l){
							GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,e);
						}
						DEFINE_LABEL(g_dt,nxt);
					}
					DEFINE_LABEL(g_dt,e);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_FOR:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				if (l==1){
					return _generate(o+1,g_dt)+eoff+1;
				}
				sll_object_offset_t off=_generate(o+1,g_dt)+1;
				const sll_object_t* cnd=o+off;
				assembly_instruction_label_t s=NEXT_LABEL(g_dt);
				assembly_instruction_label_t e=NEXT_LABEL(g_dt);
				off+=_generate_jump(cnd,g_dt,e,1);
				DEFINE_LABEL(g_dt,s);
				l-=2;
				while (l){
					l--;
					off+=_generate(o+off,g_dt);
				}
				_generate_jump(cnd,g_dt,s,0);
				DEFINE_LABEL(g_dt,e);
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_WHILE:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l);
				if (l==1){
					return _generate(o+1,g_dt)+eoff+1;
				}
				sll_object_offset_t off=_generate(o+1,g_dt)+1;
				const sll_object_t* cnd=o+off;
				off+=sll_get_object_size(cnd);
				assembly_instruction_label_t s=NEXT_LABEL(g_dt);
				DEFINE_LABEL(g_dt,s);
				l-=2;
				while (l){
					l--;
					off+=_generate(o+off,g_dt);
				}
				_generate_jump(cnd,g_dt,s,0);
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_LOOP:
			{
				sll_arg_count_t l=o->dt.ac;
				assembly_instruction_label_t s=NEXT_LABEL(g_dt);
				DEFINE_LABEL(g_dt,s);
				sll_object_offset_t off=1;
				while (l){
					l--;
					off+=_generate(o+off,g_dt);
				}
				GENERATE_OPCODE_WITH_LABEL(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP,s);
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_LESS:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE,0)+eoff;
		case SLL_OBJECT_TYPE_LESS_EQUAL:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JA,0)+eoff;
		case SLL_OBJECT_TYPE_EQUAL:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE,0)+eoff;
		case SLL_OBJECT_TYPE_NOT_EQUAL:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JE,0)+eoff;
		case SLL_OBJECT_TYPE_MORE:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE,0)+eoff;
		case SLL_OBJECT_TYPE_MORE_EQUAL:
			return _generate_sequential_jump(o,g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_JB,0)+eoff;
		case SLL_OBJECT_TYPE_RETURN:
		case SLL_OBJECT_TYPE_EXIT:
			{
				sll_arg_count_t l=o->dt.ac;
				if (!l){
					GENERATE_OPCODE(g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO);
					GENERATE_OPCODE(g_dt,(o->t==SLL_OBJECT_TYPE_RETURN?SLL_ASSEMBLY_INSTRUCTION_TYPE_RET:SLL_ASSEMBLY_INSTRUCTION_TYPE_END));
					return eoff+1;
				}
				sll_object_offset_t off=_generate_on_stack(o+1,g_dt)+1;
				l--;
				while (l){
					l--;
					off+=_generate(o+off,g_dt);
				}
				GENERATE_OPCODE(g_dt,(o->t==SLL_OBJECT_TYPE_RETURN?SLL_ASSEMBLY_INSTRUCTION_TYPE_RET:SLL_ASSEMBLY_INSTRUCTION_TYPE_END));
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_object_offset_t off=1;
				sll_statement_count_t l=o->dt.sc;
				while (l){
					l--;
					off+=_generate(o+off,g_dt);
				}
				return off+eoff;
			}
	}
	sll_object_offset_t off=1;
	sll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		off+=_generate(o+off,g_dt);
	}
	return off+eoff;
}



__SLL_FUNC __SLL_RETURN sll_generate_assembly(const sll_compilation_data_t* c_dt,sll_assembly_data_t* o,sll_error_t* e){
	if (!o->_s.ptr||!c_dt->_s.ptr){
		e->t=SLL_ERROR_NO_STACK;
		return SLL_RETURN_ERROR;
	}
	o->tm=c_dt->tm;
	o->h=(sll_assembly_instruction_t*)(o->_s.ptr+o->_s.off);
	o->ic=0;
	o->vc=0;
	o->st.l=c_dt->st.l;
	o->st.dt=malloc(o->st.l*sizeof(sll_string_t*));
	for (sll_string_index_t i=0;i<o->st.l;i++){
		sll_string_t* s=*(c_dt->st.dt+i);
		sll_string_t* d=sll_string_create(s->l);
		d->rc=1;
		d->c=s->c;
		memcpy(d->v,s->v,s->l);
		*(o->st.dt+i)=d;
	}
	assembly_generator_data_t g_dt={
		o,
		c_dt,
		{
			.l_im=malloc(c_dt->idt.ill*sizeof(identifier_data_t)),
			.n_vi=0,
			.l_sc=0,
			.sc_vi=malloc(c_dt->_n_sc_id*sizeof(sll_variable_index_t)),
			.vc=0
		},
		0,
		{
			.l=malloc(c_dt->idt.ill*sizeof(identifier_data_t))
		}
	};
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		g_dt.it.s_im[i]=malloc(c_dt->idt.s[i].l*sizeof(identifier_data_t));
		g_dt.rm.s[i]=malloc(c_dt->idt.s[i].l*sizeof(identifier_data_t));
		for (sll_identifier_list_length_t j=0;j<c_dt->idt.s[i].l;j++){
			(g_dt.it.s_im[i]+j)->v=SLL_MAX_VARIABLE_INDEX;
			(g_dt.it.s_im[i]+j)->c=0;
			*(g_dt.rm.s[i]+j)=NULL;
		}
	}
	for (sll_identifier_list_length_t i=0;i<c_dt->idt.ill;i++){
		(g_dt.it.l_im+i)->v=SLL_MAX_VARIABLE_INDEX;
		(g_dt.it.l_im+i)->c=0;
		*(g_dt.rm.l+i)=NULL;
	}
	for (sll_scope_t i=0;i<c_dt->_n_sc_id;i++){
		*(g_dt.it.sc_vi+i)=SLL_MAX_VARIABLE_INDEX;
	}
	_map_identifiers_extra(c_dt->h,c_dt,&g_dt);
	for (sll_function_index_t i=0;i<c_dt->ft.l;i++){
		g_dt.it.n_vi=g_dt.it.vc;
		g_dt.it.l_sc=0;
		const sll_object_t* fo=c_dt->h+(*(c_dt->ft.dt+i))->off;
		SLL_ASSERT(fo->t==SLL_OBJECT_TYPE_FUNC);
		sll_object_offset_t off=1;
		for (sll_arg_count_t j=0;j<fo->dt.fn.ac;j++){
			off+=_map_identifiers_extra(fo+off,c_dt,&g_dt);
		}
	}
	free(g_dt.it.sc_vi);
	o->vc=g_dt.it.vc;
	_generate(c_dt->h,&g_dt);
	if ((o->h+o->ic-1)->t!=SLL_ASSEMBLY_INSTRUCTION_TYPE_END){
		GENERATE_OPCODE(&g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_END_ZERO);
	}
	o->ft.l=c_dt->ft.l;
	o->ft.dt=malloc(c_dt->ft.l*sizeof(sll_instruction_index_t));
	for (sll_function_index_t i=0;i<c_dt->ft.l;i++){
		const sll_function_t* k=*(c_dt->ft.dt+i);
		sll_arg_count_t j=k->al;
		sll_assembly_instruction_t* ai=NEXT_INSTRUCTION(&g_dt);
		ai->t=ASSEMBLY_INSTRUCTION_TYPE_FUNC_START;
		ASSEMBLY_INSTRUCTION_MISC_FIELD(ai)=i;
		while (j){
			j--;
			ai=NEXT_INSTRUCTION(&g_dt);
			ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP;
			ai->dt.v=(SLL_IDENTIFIER_GET_ARRAY_ID(k->a[j])==SLL_MAX_SHORT_IDENTIFIER_LENGTH?(g_dt.it.l_im+SLL_IDENTIFIER_GET_ARRAY_INDEX(k->a[j]))->v:(g_dt.it.s_im[SLL_IDENTIFIER_GET_ARRAY_ID(k->a[j])]+SLL_IDENTIFIER_GET_ARRAY_INDEX(k->a[j]))->v);
		}
		const sll_object_t* fo=c_dt->h+k->off;
		SLL_ASSERT(fo->t==SLL_OBJECT_TYPE_FUNC);
		sll_object_offset_t off=1;
		for (j=0;j<fo->dt.fn.ac;j++){
			off+=_generate(fo+off,&g_dt);
		}
		if ((o->h+o->ic-1)->t!=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET){
			GENERATE_OPCODE(&g_dt,SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO);
		}
	}
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		free(g_dt.it.s_im[i]);
		free(g_dt.rm.s[i]);
	}
	free(g_dt.it.l_im);
	free(g_dt.rm.l);
	sll_assembly_instruction_t* ai=o->h;
	for (sll_instruction_index_t i=0;i<o->ic;i++){
		if ((ai+1)->t!=SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT){
			if ((ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT&&!ai->dt.i)||(ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT&&!ai->dt.f)){
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO;
			}
			else if ((ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT&&ai->dt.i==1)||(ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT&&ai->dt.f==1)){
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE;
			}
			else if ((ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT&&ai->dt.i==2)||(ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT&&ai->dt.f==2)){
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO;
			}
			else if ((ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT&&ai->dt.i==3)||(ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT&&ai->dt.f==3)){
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE;
			}
			else if ((ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT&&ai->dt.i==4)||(ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT&&ai->dt.f==4)){
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR;
			}
			else if ((ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT&&ai->dt.i==-1)||(ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT&&ai->dt.f==-1)){
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE;
			}
		}
		if (ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_POP&&(ai-1)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT){
			(ai-1)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP;
			ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
		}
		else if (ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_POP&&(ai-1)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL){
			(ai-1)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP;
			ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
		}
		else if (ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP&&(ai-1)->t>=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE&&(ai-1)->t<=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR){
			ai->t=(ai-1)->t+SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE-SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE;
			(ai-1)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
		}
		else if (ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD&&(ai-1)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP&&ai->dt.v==(ai-1)->dt.v){
			(ai-1)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE;
			ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
		}
		else if (ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT){
			if ((ai-1)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT){
				int64_t v=(ai-1)->dt.i;
				sll_char_t bf[21];
				uint8_t j=0;
				if (v<0){
					bf[0]='-';
					j=1;
					v=-v;
				}
				sll_char_t bf_r[20];
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
				(ai-1)->dt.s=sll_create_string(&(o->st),bf,j);
				goto _print_str;
			}
			else if ((ai-1)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT){
				char bf[128];
				int sz=snprintf(bf,128,"%.16lg",(ai-1)->dt.f);
				(ai-1)->dt.s=sll_create_string(&(o->st),(sll_char_t*)bf,sz);
				goto _print_str;
			}
			else if ((ai-1)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR){
				(ai-1)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR;
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				sll_assembly_instruction_t* p_ai=_get_previous_instruction(ai-1,i-1);
				if (!p_ai){
					p_ai=ai;
				}
				if (i>2&&p_ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR){
					p_ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
					(ai-1)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR;
					(ai-1)->dt.s=_create_print_string(o,&(p_ai->dt.c),&((ai-1)->dt.c),1,1,p_ai->dt.c^(ai-1)->dt.c);
				}
				else if (i>2&&p_ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR){
					p_ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
					(ai-1)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR;
					sll_string_t* a=*(o->st.dt+p_ai->dt.s);
					(ai-1)->dt.s=_create_print_string(o,a->v,&((ai-1)->dt.c),a->l,1,a->c^(ai-1)->dt.c);
				}
			}
			else if ((ai-1)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS){
_print_str:
				(ai-1)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR;
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				sll_assembly_instruction_t* p_ai=_get_previous_instruction(ai-1,i-1);
				if (!p_ai){
					p_ai=ai;
				}
				if (p_ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR){
					p_ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
					(ai-1)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR;
					sll_string_t* b=*(o->st.dt+(ai-1)->dt.s);
					(ai-1)->dt.s=_create_print_string(o,&(p_ai->dt.c),b->v,1,b->l,p_ai->dt.c^b->c);
				}
				else if (p_ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR){
					p_ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
					(ai-1)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR;
					sll_string_t* a=*(o->st.dt+p_ai->dt.s);
					sll_string_t* b=*(o->st.dt+(ai-1)->dt.s);
					(ai-1)->dt.s=_create_print_string(o,a->v,b->v,a->l,b->l,a->c^b->c);
				}
				else{
					sll_string_t* s=*(o->st.dt+(ai-1)->dt.s);
					if (s->l==1){
						(ai-1)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR;
						(ai-1)->dt.v=s->v[0];
					}
				}
			}
			else if ((ai-1)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD){
				(ai-1)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR;
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			}
		}
		else if (ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL){
			if (ai->dt.ac<2){
				sll_instruction_index_t j=1;
				while (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai-j)==SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai-j)==ASSEMBLY_INSTRUCTION_TYPE_FUNC_START||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai-j)==ASSEMBLY_INSTRUCTION_TYPE_LABEL_TARGET){
					j++;
				}
				if ((ai-j)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT){
					ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
					(ai-j)->t=(ai->dt.ac?SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO);
				}
				else if ((ai-j)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE){
					ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
					(ai-j)->t=(ai->dt.ac?SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO);
					(ai-j)->dt.i=-1;
				}
				else if ((ai-j)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO){
					ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
					(ai-j)->t=(ai->dt.ac?SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO);
					(ai-j)->dt.i=0;
				}
				else if ((ai-j)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE){
					ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
					(ai-j)->t=(ai->dt.ac?SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO);
					(ai-j)->dt.i=1;
				}
				else if ((ai-j)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO){
					ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
					(ai-j)->t=(ai->dt.ac?SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO);
					(ai-j)->dt.i=2;
				}
				else if ((ai-j)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE){
					ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
					(ai-j)->t=(ai->dt.ac?SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO);
					(ai-j)->dt.i=3;
				}
				else if ((ai-j)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR){
					ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
					(ai-j)->t=(ai->dt.ac?SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO);
					(ai-j)->dt.i=4;
				}
			}
		}
		else if (ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_RET){
			if ((ai-1)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT){
				(ai-1)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT;
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			}
			else if ((ai-1)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT){
				(ai-1)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT;
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			}
			else if ((ai-1)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR){
				(ai-1)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR;
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			}
			else if ((ai-1)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS){
				(ai-1)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR;
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			}
			else if ((ai-1)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD){
				(ai-1)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR;
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			}
		}
		else if (ai->t>=SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD&&ai->t<=SLL_ASSEMBLY_INSTRUCTION_TYPE_XOR&&(ai+1)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP){
			if ((ai-2)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD&&(ai-2)->dt.v==(ai+1)->dt.v){
				*(ai-2)=*(ai-1);
				(ai-1)->dt.v=(ai+1)->dt.v;
				goto _compress_math_op;
			}
			if ((ai-1)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD&&(ai-1)->dt.v==(ai+1)->dt.v){
_compress_math_op:
				(ai-1)->t=ai->t|SLL_ASSEMBLY_INSTRUCTION_INPLACE;
				if ((ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD||ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_SUB)&&(ai-2)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE){
					(ai-2)->t=(ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD?SLL_ASSEMBLY_INSTRUCTION_TYPE_INC:SLL_ASSEMBLY_INSTRUCTION_TYPE_DEC)|SLL_ASSEMBLY_INSTRUCTION_INPLACE;
					(ai-2)->dt.v=(ai-1)->dt.v;
					(ai-1)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				}
				ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
				(ai+1)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
			}
		}
		else if (ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_END&&(ai-1)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO){
			(ai-1)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_END_ZERO;
			ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
		}
		else if (ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_END&&(ai-1)->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE){
			(ai-1)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_END_ONE;
			ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP;
		}
		ai++;
	}
	strint_map_data_t sm={
		(o->st.l>>6)+1,
		malloc(((o->st.l>>6)+1)*sizeof(uint64_t))
	};
	for (sll_string_index_t i=0;i<sm.ml;i++){
		*(sm.m+i)=0;
	}
	ai=o->h;
	sll_instruction_index_t* lbl=malloc(g_dt.n_lbl*sizeof(sll_instruction_index_t));
	sll_instruction_index_t off=0;
	for (sll_instruction_index_t i=0;i<o->ic;i++){
		if (off){
			*ai=*(ai+off);
		}
		if (ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP){
_handle_nop:;
			off++;
			i--;
			o->ic--;
			continue;
		}
		else if (ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS||ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR||ai->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR){
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
	sm.im=malloc(o->st.l*sizeof(sll_string_t*));
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
			sll_string_release(*(o->st.dt+j));
			k=j+1;
			l++;
			v&=v-1;
		}
	}
	free(sm.m);
	for (sll_string_index_t i=k;i<o->st.l;i++){
		*(o->st.dt+i-l)=*(o->st.dt+i);
		*(sm.im+i)=i-l;
	}
	if (l){
		o->st.l-=l;
		o->st.dt=realloc(o->st.dt,o->st.l*sizeof(sll_string_t*));
	}
	ai=o->h;
	for (sll_instruction_index_t i=0;i<o->ic;i++){
		if ((SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)>=SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP&&SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)<=SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ)&&(ai->t&ASSEMBLY_INSTRUCTION_LABEL)){
			ai->t&=~ASSEMBLY_INSTRUCTION_LABEL;
			sll_instruction_index_t j=*(lbl+ASSEMBLY_INSTRUCTION_MISC_FIELD(ai));
			if (j<128){
				ai->dt.j=j;
			}
			else{
				off=((sll_relative_instruction_index_t)j)-i;
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
	}
	free(lbl);
	free(sm.im);
	return SLL_RETURN_NO_ERROR;
}
