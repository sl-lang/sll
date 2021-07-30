#include <lll_lib.h>
#include <_lll_lib_internal.h>
#include <stdint.h>
#include <stdlib.h>



void _output_int(lll_output_data_stream_t* os,int64_t v){
	char bf[20];
	uint8_t i=0;
	if (v<0){
		LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'-');
		v=-v;
	}
	do{
		bf[i]=v%10;
		i++;
		v/=10;
	} while(v);
	while (i){
		i--;
		LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,bf[i]+48);
	}
}



__LLL_IMPORT_EXPORT __LLL_RETURN_CODE lll_execute_assembly(const lll_assembly_data_t* a_dt,const lll_stack_data_t* st,lll_input_data_stream_t* in,lll_output_data_stream_t* out,lll_error_t* e){
	const lll_assembly_instruction_t* ai=a_dt->h;
	lll_runtime_object_t* v=(lll_runtime_object_t*)(st->ptr);
	lll_runtime_object_t* s=v+a_dt->vc;
	lll_instruction_index_t ii=0;
	while (1){
		if (ii>=a_dt->ic){
			e->t=LLL_ERROR_INVALID_INSTRUCTION_INDEX;
			return 0;
		}
		switch (LLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)){
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP:
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_POP:
				s--;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO:
				s-=2;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_ROT:
				{
					lll_runtime_object_t tmp=*(s-1);
					*(s-1)=*(s-2);
					*(s-2)=tmp;
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_DUP:
				*s=*(s-1);
				s++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
				s->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				s->dt.i=ai->dt.i;
				s++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE:
				s->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				s->dt.i=-1;
				s++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO:
				s->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				s->dt.i=0;
				s++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE:
				s->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				s->dt.i=1;
				s++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO:
				s->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				s->dt.i=2;
				s++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE:
				s->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				s->dt.i=3;
				s++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR:
				s->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				s->dt.i=4;
				s++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
				s->t=LLL_RUNTIME_OBJECT_TYPE_FLOAT;
				s->dt.f=ai->dt.f;
				s++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
				s->t=LLL_RUNTIME_OBJECT_TYPE_CHAR;
				s->dt.c=ai->dt.c;
				s++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_II:
				s->t=LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX;
				s->dt.ii=(LLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?ii+ai->dt.rj:ai->dt.j);
				s++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD:
				*s=*(v+ai->dt.v);
				s++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
				s->t=LLL_RUNTIME_OBJECT_TYPE_STRING;
				s->dt.s=*(a_dt->st.dt+ai->dt.s);
				s++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE:
				*(v+ai->dt.v)=*(s-1);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP:
				s--;
				*(v+ai->dt.v)=*s;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE:
				(v+ai->dt.v)->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				(v+ai->dt.v)->dt.i=-1;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ZERO:
				(v+ai->dt.v)->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				(v+ai->dt.v)->dt.i=0;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ONE:
				(v+ai->dt.v)->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				(v+ai->dt.v)->dt.i=1;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_TWO:
				(v+ai->dt.v)->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				(v+ai->dt.v)->dt.i=2;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_THREE:
				(v+ai->dt.v)->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				(v+ai->dt.v)->dt.i=3;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_FOUR:
				(v+ai->dt.v)->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				(v+ai->dt.v)->dt.i=4;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP:
_jump:
				ii=(LLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?ii+ai->dt.rj:ai->dt.j);
				ai=a_dt->h+ii;
				continue;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JB:
				{
					lll_compare_result_t cmp=lll_compare_runtime_object(s-2,s-1);
					if (cmp==LLL_COMPARE_RESULT_ERROR){
						e->t=LLL_ERROR_STACK_CORRUPTED;
						return 0;
					}
					if (cmp==LLL_COMPARE_RESULT_BELOW){
						s-=2;
						goto _jump;
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:
				{
					lll_compare_result_t cmp=lll_compare_runtime_object(s-2,s-1);
					if (cmp==LLL_COMPARE_RESULT_ERROR){
						e->t=LLL_ERROR_STACK_CORRUPTED;
						return 0;
					}
					if (cmp!=LLL_COMPARE_RESULT_ABOVE){
						s-=2;
						goto _jump;
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JA:
				{
					lll_compare_result_t cmp=lll_compare_runtime_object(s-2,s-1);
					if (cmp==LLL_COMPARE_RESULT_ERROR){
						e->t=LLL_ERROR_STACK_CORRUPTED;
						return 0;
					}
					if (cmp==LLL_COMPARE_RESULT_ABOVE){
						s-=2;
						goto _jump;
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:
				{
					lll_compare_result_t cmp=lll_compare_runtime_object(s-2,s-1);
					if (cmp==LLL_COMPARE_RESULT_ERROR){
						e->t=LLL_ERROR_STACK_CORRUPTED;
						return 0;
					}
					if (cmp!=LLL_COMPARE_RESULT_BELOW){
						s-=2;
						goto _jump;
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JE:
				{
					lll_compare_result_t cmp=lll_compare_runtime_object(s-2,s-1);
					if (cmp==LLL_COMPARE_RESULT_ERROR){
						e->t=LLL_ERROR_STACK_CORRUPTED;
						return 0;
					}
					if (cmp==LLL_COMPARE_RESULT_EQUAL){
						s-=2;
						goto _jump;
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:
				{
					lll_compare_result_t cmp=lll_compare_runtime_object(s-2,s-1);
					if (cmp==LLL_COMPARE_RESULT_ERROR){
						e->t=LLL_ERROR_STACK_CORRUPTED;
						return 0;
					}
					if (cmp!=LLL_COMPARE_RESULT_EQUAL){
						s-=2;
						goto _jump;
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:
				{
					lll_compare_result_t cmp=lll_runtime_object_nonzero(s-1);
					if (cmp==LLL_COMPARE_RESULT_ERROR){
						e->t=LLL_ERROR_STACK_CORRUPTED;
						return 0;
					}
					if (cmp==LLL_COMPARE_RESULT_ZERO){
						s--;
						goto _jump;
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ:
				{
					lll_compare_result_t cmp=lll_runtime_object_nonzero(s-1);
					if (cmp==LLL_COMPARE_RESULT_ERROR){
						e->t=LLL_ERROR_STACK_CORRUPTED;
						return 0;
					}
					if (cmp==LLL_COMPARE_RESULT_NONZERO){
						s--;
						goto _jump;
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_NOT:
				ASSERT(!"Unimplemented");
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_ADD:
				{
					s--;
					lll_runtime_object_t* a=(LLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s-1);
					lll_runtime_object_t* b=s;
					switch (LLL_RUNTIME_OBJECT_GET_TYPE(a)){
						case LLL_RUNTIME_OBJECT_TYPE_INT:
							switch (LLL_RUNTIME_OBJECT_GET_TYPE(b)){
								case LLL_RUNTIME_OBJECT_TYPE_INT:
									a->dt.i+=b->dt.i;
									break;
								case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
									a->t=LLL_RUNTIME_OBJECT_TYPE_FLOAT;
									a->dt.f=a->dt.i+b->dt.f;
									break;
								case LLL_RUNTIME_OBJECT_TYPE_CHAR:
									a->dt.i+=b->dt.c;
									break;
								case LLL_RUNTIME_OBJECT_TYPE_STRING:
									ASSERT(!"Unimplemented");
								case LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX:
									e->t=LLL_ERROR_STACK_CORRUPTED;
									return 0;
								default:
									UNREACHABLE();
							}
							break;
						case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
							ASSERT(!"Unimplemented");
						case LLL_RUNTIME_OBJECT_TYPE_CHAR:
							ASSERT(!"Unimplemented");
						case LLL_RUNTIME_OBJECT_TYPE_STRING:
							ASSERT(!"Unimplemented");
						case LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX:
							e->t=LLL_ERROR_STACK_CORRUPTED;
							return 0;
						default:
							UNREACHABLE();
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_INC:
				{
					lll_runtime_object_t* a=v+ai->dt.v;
					switch (LLL_RUNTIME_OBJECT_GET_TYPE(a)){
						case LLL_RUNTIME_OBJECT_TYPE_INT:
							a->dt.i++;
							break;
						case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
							a->dt.f++;
							break;
						case LLL_RUNTIME_OBJECT_TYPE_CHAR:
							a->dt.c++;
							break;
						case LLL_RUNTIME_OBJECT_TYPE_STRING:
							ASSERT(!"Unimplemented");
						case LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX:
							e->t=LLL_ERROR_STACK_CORRUPTED;
							return 0;
						default:
							UNREACHABLE();
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_SUB:
				ASSERT(!"Unimplemented");
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_DEC:
				{
					lll_runtime_object_t* a=v+ai->dt.v;
					switch (LLL_RUNTIME_OBJECT_GET_TYPE(a)){
						case LLL_RUNTIME_OBJECT_TYPE_INT:
							a->dt.i--;
							break;
						case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
							a->dt.f--;
							break;
						case LLL_RUNTIME_OBJECT_TYPE_CHAR:
							a->dt.c--;
							break;
						case LLL_RUNTIME_OBJECT_TYPE_STRING:
							ASSERT(!"Unimplemented");
						case LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX:
							e->t=LLL_ERROR_STACK_CORRUPTED;
							return 0;
						default:
							UNREACHABLE();
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_MULT:
				{
					s--;
					lll_runtime_object_t* a=(LLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s-1);
					lll_runtime_object_t* b=s;
					switch (LLL_RUNTIME_OBJECT_GET_TYPE(a)){
						case LLL_RUNTIME_OBJECT_TYPE_INT:
							switch (LLL_RUNTIME_OBJECT_GET_TYPE(b)){
								case LLL_RUNTIME_OBJECT_TYPE_INT:
									a->dt.i*=b->dt.i;
									break;
								case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
									a->t=LLL_RUNTIME_OBJECT_TYPE_FLOAT;
									a->dt.f=a->dt.i*b->dt.f;
									break;
								case LLL_RUNTIME_OBJECT_TYPE_CHAR:
									a->dt.i*=b->dt.c;
									break;
								case LLL_RUNTIME_OBJECT_TYPE_STRING:
									ASSERT(!"Unimplemented");
								case LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX:
									e->t=LLL_ERROR_STACK_CORRUPTED;
									return 0;
								default:
									UNREACHABLE();
							}
							break;
						case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
							ASSERT(!"Unimplemented");
						case LLL_RUNTIME_OBJECT_TYPE_CHAR:
							ASSERT(!"Unimplemented");
						case LLL_RUNTIME_OBJECT_TYPE_STRING:
							ASSERT(!"Unimplemented");
						case LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX:
							e->t=LLL_ERROR_STACK_CORRUPTED;
							return 0;
						default:
							UNREACHABLE();
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_DIV:
				ASSERT(!"Unimplemented");
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_FDIV:
				ASSERT(!"Unimplemented");
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_MOD:
				ASSERT(!"Unimplemented");
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_AND:
				ASSERT(!"Unimplemented");
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_OR:
				ASSERT(!"Unimplemented");
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_XOR:
				ASSERT(!"Unimplemented");
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_INV:
				ASSERT(!"Unimplemented");
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT:
				s--;
_print_from_stack:
				switch (LLL_RUNTIME_OBJECT_GET_TYPE(s)){
					case LLL_RUNTIME_OBJECT_TYPE_INT:
						_output_int(out,s->dt.i);
						break;
					case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
						ASSERT(!"Unimplemented");
					case LLL_RUNTIME_OBJECT_TYPE_CHAR:
						LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(out,(uint8_t)(s->dt.c));
						break;
					case LLL_RUNTIME_OBJECT_TYPE_STRING:
						LLL_WRITE_TO_OUTPUT_DATA_STREAM(out,(uint8_t*)(s->dt.s->v),s->dt.s->l*sizeof(lll_char_t));
						break;
					case LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX:
					default:
						e->t=LLL_ERROR_STACK_CORRUPTED;
						return 0;
						UNREACHABLE();
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(out,(uint8_t)(ai->dt.c));
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
				{
					lll_string_t* str=*(a_dt->st.dt+ai->dt.s);
					LLL_WRITE_TO_OUTPUT_DATA_STREAM(out,(uint8_t*)(str->v),str->l*sizeof(lll_char_t));
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR:
				*s=*(v+ai->dt.v);
				goto _print_from_stack;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
				if ((s-1)->dt.i<0||(s-1)->dt.i>=a_dt->ft.l){
					s-=ai->dt.ac+1;
					(s-1)->t=LLL_RUNTIME_OBJECT_TYPE_INT;
					(s-1)->dt.i=0;
					break;
				}
				s--;
				ii=*(a_dt->ft.dt+s->dt.i);
				ai=a_dt->h+ii;
				continue;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET:
				s--;
				ASSERT((s-1)->t==LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX);
				ii=(s-1)->dt.ii;
				ai=a_dt->h+ii;
				*(s-1)=*s;
				continue;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO:
				ASSERT((s-1)->t==LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX);
				ii=(s-1)->dt.ii;
				ai=a_dt->h+ii;
				(s-1)->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				(s-1)->dt.i=0;
				continue;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				ASSERT((s-1)->t==LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX);
				ii=(s-1)->dt.ii;
				ai=a_dt->h+ii;
				(s-1)->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				(s-1)->dt.i=ai->dt.i;
				continue;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				ASSERT((s-1)->t==LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX);
				ii=(s-1)->dt.ii;
				ai=a_dt->h+ii;
				(s-1)->t=LLL_RUNTIME_OBJECT_TYPE_FLOAT;
				(s-1)->dt.f=ai->dt.f;
				continue;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				ASSERT((s-1)->t==LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX);
				ii=(s-1)->dt.ii;
				ai=a_dt->h+ii;
				(s-1)->t=LLL_RUNTIME_OBJECT_TYPE_CHAR;
				(s-1)->dt.c=ai->dt.c;
				continue;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				ASSERT((s-1)->t==LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX);
				ii=(s-1)->dt.ii;
				ai=a_dt->h+ii;
				(s-1)->t=LLL_RUNTIME_OBJECT_TYPE_STRING;
				(s-1)->dt.s=*(a_dt->st.dt+ai->dt.s);
				continue;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR:
				ASSERT((s-1)->t==LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX);
				ii=(s-1)->dt.ii;
				ai=a_dt->h+ii;
				*(s-1)=*(v+ai->dt.v);
				continue;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_END:
				s--;
				if (s<v+a_dt->vc){
					e->t=LLL_ERROR_STACK_CORRUPTED;
					return 0;
				}
				switch (LLL_RUNTIME_OBJECT_GET_TYPE(s)){
					case LLL_RUNTIME_OBJECT_TYPE_INT:
						return (s->dt.i<INT32_MIN?INT32_MIN:(s->dt.i>INT32_MAX?INT32_MAX:(lll_return_code_t)s->dt.i));
					case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
						return (s->dt.f<INT32_MIN?INT32_MIN:(s->dt.f>INT32_MAX?INT32_MAX:(lll_return_code_t)s->dt.f));
					case LLL_RUNTIME_OBJECT_TYPE_CHAR:
						return (s->dt.c>INT32_MAX?INT32_MAX:s->dt.c);// lgtm [cpp/constant-comparison]
					case LLL_RUNTIME_OBJECT_TYPE_STRING:
						return (s->dt.s->l>INT32_MAX?INT32_MAX:s->dt.s->l);
					case LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX:
					default:
						e->t=LLL_ERROR_STACK_CORRUPTED;
						return 0;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_END_ZERO:
				if (s<v+a_dt->vc){
					e->t=LLL_ERROR_STACK_CORRUPTED;
				}
				return 0;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_END_ONE:
				if (s<v+a_dt->vc){
					e->t=LLL_ERROR_STACK_CORRUPTED;
				}
				return 1;
			default:
				e->t=LLL_ERROR_INVALID_INSTRUCTION;
				e->dt.it=LLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai);
				return 0;
		}
		ai++;
		ii++;
	}
}
