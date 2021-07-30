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
	call_stack_t c_st={
		(call_stack_frame_t*)(st->ptr+a_dt->vc*sizeof(lll_runtime_object_t)),
		0
	};
	lll_runtime_object_t* s=(lll_runtime_object_t*)(st->ptr+a_dt->vc*sizeof(lll_runtime_object_t)+CALL_STACK_SIZE*sizeof(call_stack_frame_t));
	lll_stack_offset_t si=0;
	lll_stack_offset_t s_sz=(st->sz-a_dt->vc*sizeof(lll_runtime_object_t)-CALL_STACK_SIZE*sizeof(call_stack_frame_t))/sizeof(lll_runtime_object_t);
	IGNORE(s_sz);
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
				si--;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO:
				si-=2;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_ROT:
				{
					lll_runtime_object_t tmp=*(s+si-1);
					*(s+si-1)=*(s+si-2);
					*(s+si-2)=tmp;
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP:
				si--;
				*(s+si-1)=*(s+si);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_DUP:
				*(s+si)=*(s+si-1);
				si++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
				(s+si)->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				(s+si)->dt.i=ai->dt.i;
				si++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE:
				(s+si)->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				(s+si)->dt.i=-1;
				si++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO:
				(s+si)->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				(s+si)->dt.i=0;
				si++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE:
				(s+si)->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				(s+si)->dt.i=1;
				si++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO:
				(s+si)->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				(s+si)->dt.i=2;
				si++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE:
				(s+si)->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				(s+si)->dt.i=3;
				si++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR:
				(s+si)->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				(s+si)->dt.i=4;
				si++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
				(s+si)->t=LLL_RUNTIME_OBJECT_TYPE_FLOAT;
				(s+si)->dt.f=ai->dt.f;
				si++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
				(s+si)->t=LLL_RUNTIME_OBJECT_TYPE_CHAR;
				(s+si)->dt.c=ai->dt.c;
				si++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD:
				*(s+si)=*(v+ai->dt.v);
				si++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
				(s+si)->t=LLL_RUNTIME_OBJECT_TYPE_STRING;
				(s+si)->dt.s=*(a_dt->st.dt+ai->dt.s);
				si++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE:
				*(v+ai->dt.v)=*(s+si-1);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP:
				si--;
				*(v+ai->dt.v)=*(s+si);
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
					lll_compare_result_t cmp=lll_compare_runtime_object(s+si-2,s+si-1);
					if (cmp==LLL_COMPARE_RESULT_ERROR){
						e->t=LLL_ERROR_STACK_CORRUPTED;
						return 0;
					}
					if (cmp==LLL_COMPARE_RESULT_BELOW){
						si-=2;
						goto _jump;
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:
				{
					lll_compare_result_t cmp=lll_compare_runtime_object(s+si-2,s+si-1);
					if (cmp==LLL_COMPARE_RESULT_ERROR){
						e->t=LLL_ERROR_STACK_CORRUPTED;
						return 0;
					}
					if (cmp!=LLL_COMPARE_RESULT_ABOVE){
						si-=2;
						goto _jump;
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JA:
				{
					lll_compare_result_t cmp=lll_compare_runtime_object(s+si-2,s+si-1);
					if (cmp==LLL_COMPARE_RESULT_ERROR){
						e->t=LLL_ERROR_STACK_CORRUPTED;
						return 0;
					}
					if (cmp==LLL_COMPARE_RESULT_ABOVE){
						si-=2;
						goto _jump;
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:
				{
					lll_compare_result_t cmp=lll_compare_runtime_object(s+si-2,s+si-1);
					if (cmp==LLL_COMPARE_RESULT_ERROR){
						e->t=LLL_ERROR_STACK_CORRUPTED;
						return 0;
					}
					if (cmp!=LLL_COMPARE_RESULT_BELOW){
						si-=2;
						goto _jump;
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JE:
				{
					lll_compare_result_t cmp=lll_compare_runtime_object(s+si-2,s+si-1);
					if (cmp==LLL_COMPARE_RESULT_ERROR){
						e->t=LLL_ERROR_STACK_CORRUPTED;
						return 0;
					}
					if (cmp==LLL_COMPARE_RESULT_EQUAL){
						si-=2;
						goto _jump;
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:
				{
					lll_compare_result_t cmp=lll_compare_runtime_object(s+si-2,s+si-1);
					if (cmp==LLL_COMPARE_RESULT_ERROR){
						e->t=LLL_ERROR_STACK_CORRUPTED;
						return 0;
					}
					if (cmp!=LLL_COMPARE_RESULT_EQUAL){
						si-=2;
						goto _jump;
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:
				{
					lll_compare_result_t cmp=lll_runtime_object_nonzero(s+si-1);
					if (cmp==LLL_COMPARE_RESULT_ERROR){
						e->t=LLL_ERROR_STACK_CORRUPTED;
						return 0;
					}
					if (cmp==LLL_COMPARE_RESULT_ZERO){
						si--;
						goto _jump;
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ:
				{
					lll_compare_result_t cmp=lll_runtime_object_nonzero(s+si-1);
					if (cmp==LLL_COMPARE_RESULT_ERROR){
						e->t=LLL_ERROR_STACK_CORRUPTED;
						return 0;
					}
					if (cmp==LLL_COMPARE_RESULT_NONZERO){
						si--;
						goto _jump;
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_NOT:
				ASSERT(!"Unimplemented");
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
						default:
							UNREACHABLE();
					}
					break;
				}
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
						default:
							UNREACHABLE();
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_ADD:
				{
					si--;
					lll_runtime_object_t* a=(LLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s+si-1);
					lll_runtime_object_t* b=s+si;
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
						default:
							UNREACHABLE();
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_SUB:
				ASSERT(!"Unimplemented");
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_MULT:
				{
					si--;
					lll_runtime_object_t* a=(LLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s+si-1);
					lll_runtime_object_t* b=s+si;
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
				si--;
_print_from_stack:
				switch (LLL_RUNTIME_OBJECT_GET_TYPE(s+si)){
					case LLL_RUNTIME_OBJECT_TYPE_INT:
						_output_int(out,(s+si)->dt.i);
						break;
					case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
						ASSERT(!"Unimplemented");
					case LLL_RUNTIME_OBJECT_TYPE_CHAR:
						LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(out,(uint8_t)((s+si)->dt.c));
						break;
					case LLL_RUNTIME_OBJECT_TYPE_STRING:
						LLL_WRITE_TO_OUTPUT_DATA_STREAM(out,(uint8_t*)((s+si)->dt.s->v),(s+si)->dt.s->l*sizeof(lll_char_t));
						break;
					default:
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
				*(s+si)=*(v+ai->dt.v);
				goto _print_from_stack;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
				if ((s+si-1)->dt.i<0||(s+si-1)->dt.i>=a_dt->ft.l){
					si-=ai->dt.ac-1;
					(s+si-1)->t=LLL_RUNTIME_OBJECT_TYPE_INT;
					(s+si-1)->dt.i=0;
					break;
				}
				si--;
				ASSERT(c_st.l<=CALL_STACK_SIZE);
				(c_st.dt+c_st.l)->ii=ii+1;
				(c_st.dt+c_st.l)->s=si;
				c_st.l++;
				ii=*(a_dt->ft.dt+(s+si)->dt.i);
				ai=a_dt->h+ii;
				continue;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET:
_return:;
				lll_runtime_object_t tmp=*(s+si-1);
				ASSERT(c_st.l);
				c_st.l--;
				ii=(c_st.dt+c_st.l)->ii;
				ai=a_dt->h+ii;
				si=(c_st.dt+c_st.l)->s;
				*(s+si)=tmp;
				si++;
				continue;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO:
				(s+si)->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				(s+si)->dt.i=0;
				si++;
				goto _return;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				(s+si)->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				(s+si)->dt.i=ai->dt.i;
				si++;
				goto _return;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				(s+si)->t=LLL_RUNTIME_OBJECT_TYPE_FLOAT;
				(s+si)->dt.f=ai->dt.f;
				si++;
				goto _return;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				(s+si)->t=LLL_RUNTIME_OBJECT_TYPE_CHAR;
				(s+si)->dt.c=ai->dt.c;
				si++;
				goto _return;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				(s+si)->t=LLL_RUNTIME_OBJECT_TYPE_STRING;
				(s+si)->dt.s=*(a_dt->st.dt+ai->dt.s);
				si++;
				goto _return;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR:
				*(s+si)=*(v+ai->dt.v);
				si++;
				goto _return;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_END:
				si--;
				if (s<v+a_dt->vc){
					e->t=LLL_ERROR_STACK_CORRUPTED;
					return 0;
				}
				switch (LLL_RUNTIME_OBJECT_GET_TYPE(s)){
					case LLL_RUNTIME_OBJECT_TYPE_INT:
						return ((s+si)->dt.i<INT32_MIN?INT32_MIN:((s+si)->dt.i>INT32_MAX?INT32_MAX:(lll_return_code_t)(s+si)->dt.i));
					case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
						return ((s+si)->dt.f<INT32_MIN?INT32_MIN:((s+si)->dt.f>INT32_MAX?INT32_MAX:(lll_return_code_t)(s+si)->dt.f));
					case LLL_RUNTIME_OBJECT_TYPE_CHAR:
						return ((s+si)->dt.c>INT32_MAX?INT32_MAX:(s+si)->dt.c);// lgtm [cpp/constant-comparison]
					case LLL_RUNTIME_OBJECT_TYPE_STRING:
						return ((s+si)->dt.s->l>INT32_MAX?INT32_MAX:(s+si)->dt.s->l);
					default:
						UNREACHABLE();
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_END_ZERO:
				return 0;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_END_ONE:
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
