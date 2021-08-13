#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/core.h>
#include <sll/operator.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



#define OPERATOR_INSTRUCTION_UNARY(nm) \
	{ \
		sll_runtime_object_t* a=(SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s+si-1); \
		sll_runtime_object_t n={0}; \
		sll_operator_##nm(a,&n); \
		if (a->t==SLL_RUNTIME_OBJECT_TYPE_STRING){ \
			sll_string_release(a->dt.s); \
		} \
		else if (a->t==SLL_RUNTIME_OBJECT_TYPE_ARRAY){ \
			sll_array_release(a->dt.a); \
		} \
		*a=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION(nm) \
	{ \
		si--; \
		sll_runtime_object_t* a=(SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s+si-1); \
		sll_runtime_object_t* b=s+si; \
		sll_runtime_object_t n={0}; \
		sll_operator_##nm(a,b,&n); \
		if (a->t==SLL_RUNTIME_OBJECT_TYPE_STRING){ \
			sll_string_release(a->dt.s); \
		} \
		else if (a->t==SLL_RUNTIME_OBJECT_TYPE_ARRAY){ \
			sll_array_release(a->dt.a); \
		} \
		if (b->t==SLL_RUNTIME_OBJECT_TYPE_STRING){ \
			sll_string_release(b->dt.s); \
		} \
		else if (b->t==SLL_RUNTIME_OBJECT_TYPE_ARRAY){ \
			sll_array_release(b->dt.a); \
		} \
		*a=n; \
		break; \
	}



__SLL_FUNC __SLL_RETURN_CODE sll_execute_assembly(const sll_assembly_data_t* a_dt,const sll_stack_data_t* st,sll_internal_function_table_t* i_ft,sll_input_data_stream_t* in,sll_output_data_stream_t* out,sll_error_t* e){
	const sll_assembly_instruction_t* ai=a_dt->h;
	sll_runtime_object_t* v=(sll_runtime_object_t*)(st->ptr);
	for (sll_variable_index_t i=0;i<a_dt->vc;i++){
		(v+i)->t=SLL_RUNTIME_OBJECT_TYPE_INT;
		(v+i)->dt.i=0;
	}
	call_stack_t c_st={
		(call_stack_frame_t*)(st->ptr+a_dt->vc*sizeof(sll_runtime_object_t)),
		0
	};
	sll_runtime_object_t* s=(sll_runtime_object_t*)(st->ptr+a_dt->vc*sizeof(sll_runtime_object_t)+CALL_STACK_SIZE*sizeof(call_stack_frame_t));
	sll_stack_offset_t si=0;
	sll_stack_offset_t s_sz=(st->sz-a_dt->vc*sizeof(sll_runtime_object_t)-CALL_STACK_SIZE*sizeof(call_stack_frame_t))/sizeof(sll_runtime_object_t);
	sll_instruction_index_t ii=0;
	while (1){
		if (ii>=a_dt->ic){
			e->t=SLL_ERROR_INVALID_INSTRUCTION_INDEX;
			return 0;
		}
		switch (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)){
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP:
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_POP:
				si--;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO:
				si-=2;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT:
				{
					sll_runtime_object_t tmp=*(s+si-1);
					*(s+si-1)=*(s+si-2);
					*(s+si-2)=tmp;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP:
				si--;
				*(s+si-1)=*(s+si);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DUP:
				*(s+si)=*(s+si-1);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
				(s+si)->t=SLL_RUNTIME_OBJECT_TYPE_INT;
				(s+si)->dt.i=ai->dt.i;
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE:
				(s+si)->t=SLL_RUNTIME_OBJECT_TYPE_INT;
				(s+si)->dt.i=-1;
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO:
				(s+si)->t=SLL_RUNTIME_OBJECT_TYPE_INT;
				(s+si)->dt.i=0;
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE:
				(s+si)->t=SLL_RUNTIME_OBJECT_TYPE_INT;
				(s+si)->dt.i=1;
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO:
				(s+si)->t=SLL_RUNTIME_OBJECT_TYPE_INT;
				(s+si)->dt.i=2;
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE:
				(s+si)->t=SLL_RUNTIME_OBJECT_TYPE_INT;
				(s+si)->dt.i=3;
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR:
				(s+si)->t=SLL_RUNTIME_OBJECT_TYPE_INT;
				(s+si)->dt.i=4;
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
				(s+si)->t=SLL_RUNTIME_OBJECT_TYPE_FLOAT;
				(s+si)->dt.f=ai->dt.f;
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
				(s+si)->t=SLL_RUNTIME_OBJECT_TYPE_CHAR;
				(s+si)->dt.c=ai->dt.c;
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD:
				*(s+si)=*(v+ai->dt.v);
				if ((s+si)->t==SLL_RUNTIME_OBJECT_TYPE_STRING){
					(s+si)->dt.s->rc++;
				}
				else if ((s+si)->t==SLL_RUNTIME_OBJECT_TYPE_ARRAY){
					(s+si)->dt.a->rc++;
				}
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
				(s+si)->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
				(s+si)->dt.s=*(a_dt->st.dt+ai->dt.s);
				(s+si)->dt.s->rc++;
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK:
				{
					sll_array_t* a=sll_array_create(ai->dt.al);
					a->rc=1;
					si-=ai->dt.al;
					for (sll_array_length_t i=0;i<ai->dt.al;i++){
						a->v[i]=*(s+si+i);
					}
					(s+si)->t=SLL_RUNTIME_OBJECT_TYPE_ARRAY;
					(s+si)->dt.a=a;
					si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO:
				SLL_UNIMPLEMENTED();
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ONE:
				SLL_UNIMPLEMENTED();
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE:
				*(v+ai->dt.v)=*(s+si-1);
				if ((s+si-1)->t==SLL_RUNTIME_OBJECT_TYPE_STRING){
					(s+si-1)->dt.s->rc++;
				}
				else if ((s+si-1)->t==SLL_RUNTIME_OBJECT_TYPE_ARRAY){
					(s+si-1)->dt.a->rc++;
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP:
				si--;
				*(v+ai->dt.v)=*(s+si);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE:
				(v+ai->dt.v)->t=SLL_RUNTIME_OBJECT_TYPE_INT;
				(v+ai->dt.v)->dt.i=-1;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ZERO:
				(v+ai->dt.v)->t=SLL_RUNTIME_OBJECT_TYPE_INT;
				(v+ai->dt.v)->dt.i=0;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ONE:
				(v+ai->dt.v)->t=SLL_RUNTIME_OBJECT_TYPE_INT;
				(v+ai->dt.v)->dt.i=1;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_TWO:
				(v+ai->dt.v)->t=SLL_RUNTIME_OBJECT_TYPE_INT;
				(v+ai->dt.v)->dt.i=2;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_THREE:
				(v+ai->dt.v)->t=SLL_RUNTIME_OBJECT_TYPE_INT;
				(v+ai->dt.v)->dt.i=3;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_FOUR:
				(v+ai->dt.v)->t=SLL_RUNTIME_OBJECT_TYPE_INT;
				(v+ai->dt.v)->dt.i=4;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP:
_jump:
				ii=(SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?ii+ai->dt.rj:ai->dt.j);
				ai=a_dt->h+ii;
				continue;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JB:
				{
					if (sll_operator_compare(s+si-2,s+si-1)==SLL_COMPARE_RESULT_BELOW){
						si-=2;
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:
				{
					if (sll_operator_compare(s+si-2,s+si-1)!=SLL_COMPARE_RESULT_ABOVE){
						si-=2;
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JA:
				{
					if (sll_operator_compare(s+si-2,s+si-1)==SLL_COMPARE_RESULT_ABOVE){
						si-=2;
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:
				{
					if (sll_operator_compare(s+si-2,s+si-1)!=SLL_COMPARE_RESULT_BELOW){
						si-=2;
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JE:
				{
					if (sll_operator_compare(s+si-2,s+si-1)==SLL_COMPARE_RESULT_EQUAL){
						si-=2;
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:
				{
					if (sll_operator_compare(s+si-2,s+si-1)!=SLL_COMPARE_RESULT_EQUAL){
						si-=2;
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:
				{
					if (sll_operator_bool(s+si-1)==SLL_COMPARE_RESULT_ZERO){
						si--;
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ:
				{
					if (sll_operator_bool(s+si-1)==SLL_COMPARE_RESULT_NONZERO){
						si--;
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT:
				SLL_UNIMPLEMENTED();
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_INC:
				OPERATOR_INSTRUCTION_UNARY(inc);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DEC:
				OPERATOR_INSTRUCTION_UNARY(dec);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD:
				OPERATOR_INSTRUCTION(add);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_SUB:
				OPERATOR_INSTRUCTION(sub);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MULT:
				OPERATOR_INSTRUCTION(mult);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DIV:
				OPERATOR_INSTRUCTION(div);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_FDIV:
				OPERATOR_INSTRUCTION(floor_div);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MOD:
				OPERATOR_INSTRUCTION(mod);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_AND:
				OPERATOR_INSTRUCTION(and);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_OR:
				OPERATOR_INSTRUCTION(or);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_XOR:
				OPERATOR_INSTRUCTION(xor);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_INV:
				OPERATOR_INSTRUCTION_UNARY(inv);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT:
				{
					si--;
_print_from_stack:;
					if ((s+si)->t==SLL_RUNTIME_OBJECT_TYPE_STRING){
						fwrite((s+si)->dt.s->v,sizeof(sll_char_t),(s+si)->dt.s->l,stdout);
						sll_string_release((s+si)->dt.s);
					}
					else{
						sll_string_t* str=sll_object_to_string(s+si,1);
						fwrite(str->v,sizeof(sll_char_t),str->l,stdout);
						sll_string_release(str);
						if ((s+si)->t==SLL_RUNTIME_OBJECT_TYPE_ARRAY){
							sll_array_release((s+si)->dt.a);
						}
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(out,ai->dt.c);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
				{
					sll_string_t* ps=*(a_dt->st.dt+ai->dt.s);
					SLL_WRITE_TO_OUTPUT_DATA_STREAM(out,ps->v,ps->l*sizeof(sll_char_t));
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR:
				*(s+si)=*(v+ai->dt.v);
				if ((s+si)->t==SLL_RUNTIME_OBJECT_TYPE_STRING){
					(s+si)->dt.s->rc++;
				}
				else if ((s+si)->t==SLL_RUNTIME_OBJECT_TYPE_ARRAY){
					(s+si)->dt.a->rc++;
				}
				goto _print_from_stack;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP:
				si--;
				if ((s+si)->dt.i<0){
					sll_function_index_t i=(sll_function_index_t)(~(s+si)->dt.i);
					if (i<i_ft->l){
						si-=ai->dt.ac;
						sll_runtime_object_t n={0};
						(*(i_ft->dt+i))->p(&n,ai->dt.ac,s+si);
						for (sll_arg_count_t j=0;j<ai->dt.ac;j++){
							if ((s+si+j)->t==SLL_RUNTIME_OBJECT_TYPE_STRING){
								sll_string_release((s+si+j)->dt.s);
							}
							else if ((s+si+j)->t==SLL_RUNTIME_OBJECT_TYPE_ARRAY){
								sll_array_release((s+si+j)->dt.a);
							}
						}
						if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)!=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP){
							*(s+si)=n;
							si++;
						}
						break;
					}
				}
				else if ((s+si)->dt.i&&(s+si)->dt.i<=a_dt->ft.l){
					SLL_ASSERT(c_st.l<=CALL_STACK_SIZE);
					(c_st.dt+c_st.l)->ii=ii;
					(c_st.dt+c_st.l)->s=si-ai->dt.ac;
					c_st.l++;
					ii=*(a_dt->ft.dt+(s+si)->dt.i-1);
					ai=a_dt->h+ii;
					continue;
				}
				if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP){
					si-=ai->dt.ac;
				}
				else{
					if (!ai->dt.ac){
						si++;
					}
					else{
						si-=ai->dt.ac-1;
					}
					(s+si-1)->t=SLL_RUNTIME_OBJECT_TYPE_INT;
					(s+si-1)->dt.i=0;
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO:
				if (ai->dt.i<0){
					sll_function_index_t i=(sll_function_index_t)(~ai->dt.i);
					if (i<i_ft->l){
						(*(i_ft->dt+i))->p(s+si,0,NULL);
						si++;
						break;
					}
				}
				else if (ai->dt.i&&ai->dt.i<=a_dt->ft.l){
					SLL_ASSERT(c_st.l<=CALL_STACK_SIZE);
					(c_st.dt+c_st.l)->ii=ii;
					(c_st.dt+c_st.l)->s=si;
					c_st.l++;
					ii=*(a_dt->ft.dt+ai->dt.i-1);
					ai=a_dt->h+ii;
					continue;
				}
				(s+si)->t=SLL_RUNTIME_OBJECT_TYPE_INT;
				(s+si)->dt.i=0;
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:
				if (ai->dt.i<0){
					sll_function_index_t i=(sll_function_index_t)(~ai->dt.i);
					if (i<i_ft->l){
						sll_runtime_object_t n={0};
						(*(i_ft->dt+i))->p(&n,1,s+si-1);
						if ((s+si-1)->t==SLL_RUNTIME_OBJECT_TYPE_STRING){
							sll_string_release((s+si-1)->dt.s);
						}
						else if ((s+si-1)->t==SLL_RUNTIME_OBJECT_TYPE_ARRAY){
							sll_array_release((s+si-1)->dt.a);
						}
						*(s+si-1)=n;
						break;
					}
				}
				else if (ai->dt.i&&ai->dt.i<=a_dt->ft.l){
					SLL_ASSERT(c_st.l<=CALL_STACK_SIZE);
					(c_st.dt+c_st.l)->ii=ii;
					(c_st.dt+c_st.l)->s=si-1;
					c_st.l++;
					ii=*(a_dt->ft.dt+ai->dt.i-1);
					ai=a_dt->h+ii;
					continue;
				}
				(s+si-1)->t=SLL_RUNTIME_OBJECT_TYPE_INT;
				(s+si-1)->dt.i=0;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET:
_return:;
				sll_runtime_object_t tmp=*(s+si-1);
				SLL_ASSERT(c_st.l);
				c_st.l--;
				ii=(c_st.dt+c_st.l)->ii;
				ai=a_dt->h+ii;
				si=(c_st.dt+c_st.l)->s;
				if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)!=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP){
					*(s+si)=tmp;
					si++;
				}
				ai++;
				continue;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO:
				(s+si)->t=SLL_RUNTIME_OBJECT_TYPE_INT;
				(s+si)->dt.i=0;
				si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				(s+si)->t=SLL_RUNTIME_OBJECT_TYPE_INT;
				(s+si)->dt.i=ai->dt.i;
				si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				(s+si)->t=SLL_RUNTIME_OBJECT_TYPE_FLOAT;
				(s+si)->dt.f=ai->dt.f;
				si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				(s+si)->t=SLL_RUNTIME_OBJECT_TYPE_CHAR;
				(s+si)->dt.c=ai->dt.c;
				si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				(s+si)->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
				(s+si)->dt.s=*(a_dt->st.dt+ai->dt.s);
				(s+si)->dt.s->rc++;
				si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR:
				*(s+si)=*(v+ai->dt.v);
				si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_END:
				si--;
				if (s<v+a_dt->vc){
					e->t=SLL_ERROR_STACK_CORRUPTED;
					return 0;
				}
				switch (SLL_RUNTIME_OBJECT_GET_TYPE(s)){
					case SLL_RUNTIME_OBJECT_TYPE_INT:
						return ((s+si)->dt.i<INT32_MIN?INT32_MIN:((s+si)->dt.i>INT32_MAX?INT32_MAX:(sll_return_code_t)(s+si)->dt.i));
					case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
						return ((s+si)->dt.f<INT32_MIN?INT32_MIN:((s+si)->dt.f>INT32_MAX?INT32_MAX:(sll_return_code_t)(s+si)->dt.f));
					case SLL_RUNTIME_OBJECT_TYPE_CHAR:
						return ((s+si)->dt.c>INT32_MAX?INT32_MAX:(s+si)->dt.c);// lgtm [cpp/constant-comparison]
					case SLL_RUNTIME_OBJECT_TYPE_STRING:
						return ((s+si)->dt.s->l>INT32_MAX?INT32_MAX:(s+si)->dt.s->l);
					case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
						SLL_UNIMPLEMENTED();
					default:
						UNREACHABLE();
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_END_ZERO:
				return 0;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_END_ONE:
				return 1;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL:
				*(s+si)=*(v+ai->dt.v);
				(v+ai->dt.v)->t=SLL_RUNTIME_OBJECT_TYPE_INT;
				(v+ai->dt.v)->dt.i=0;
				si++;
				break;
			default:
				e->t=SLL_ERROR_INVALID_INSTRUCTION;
				e->dt.it=SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai);
				return 0;
		}
		ai++;
		ii++;
		if (si>=s_sz){
			e->t=SLL_ERROR_INVALID_STACK_INDEX;
			return 0;
		}
	}
}
