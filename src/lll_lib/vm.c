#include <lll_lib.h>
#include <_lll_lib_internal.h>
#include <stdlib.h>



uint8_t _compare_runtime_object(const lll_runtime_object_t* a,const lll_runtime_object_t* b,const lll_assembly_data_t* a_dt){
	switch (a->t){
		case LLL_RUNTIME_OBJECT_TYPE_NIL:
		case LLL_RUNTIME_OBJECT_TYPE_FALSE:
			switch (b->t){
				case LLL_RUNTIME_OBJECT_TYPE_NIL:
				case LLL_RUNTIME_OBJECT_TYPE_FALSE:
					return RUNTIME_OBJECT_COMPARE_EQUAL;
				case LLL_RUNTIME_OBJECT_TYPE_INT:
					return (b->dt.i>0?RUNTIME_OBJECT_COMPARE_BELOW:(b->dt.i<0?RUNTIME_OBJECT_COMPARE_ABOVE:RUNTIME_OBJECT_COMPARE_EQUAL));
				case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
					return (b->dt.f>0?RUNTIME_OBJECT_COMPARE_BELOW:(b->dt.f<0?RUNTIME_OBJECT_COMPARE_ABOVE:RUNTIME_OBJECT_COMPARE_EQUAL));
				case LLL_RUNTIME_OBJECT_TYPE_CHAR:
					return (b->dt.c?RUNTIME_OBJECT_COMPARE_BELOW:RUNTIME_OBJECT_COMPARE_EQUAL);
				case LLL_RUNTIME_OBJECT_TYPE_TRUE:
					return RUNTIME_OBJECT_COMPARE_BELOW;
				case LLL_RUNTIME_OBJECT_TYPE_STRING:
					return (b->dt.s->l?RUNTIME_OBJECT_COMPARE_BELOW:RUNTIME_OBJECT_COMPARE_EQUAL);
				case LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX:
					ASSERT(!"Stack Corruption");
				default:
					UNREACHABLE();
			}
		case LLL_RUNTIME_OBJECT_TYPE_INT:
			{
				int64_t v=0;
				switch (b->t){
					case LLL_RUNTIME_OBJECT_TYPE_INT:
						v=b->dt.i;
						break;
					case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
						return (b->dt.f>a->dt.i?RUNTIME_OBJECT_COMPARE_BELOW:(b->dt.f<a->dt.i?RUNTIME_OBJECT_COMPARE_ABOVE:RUNTIME_OBJECT_COMPARE_EQUAL));
					case LLL_RUNTIME_OBJECT_TYPE_CHAR:
						v=b->dt.c;
						break;
					case LLL_RUNTIME_OBJECT_TYPE_TRUE:
						v=1;
						break;
					case LLL_RUNTIME_OBJECT_TYPE_STRING:
						v=b->dt.s->l;
						break;
					case LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX:
						ASSERT(!"Stack Corruption");
				}
				return (v>a->dt.i?RUNTIME_OBJECT_COMPARE_BELOW:(v<a->dt.i?RUNTIME_OBJECT_COMPARE_ABOVE:RUNTIME_OBJECT_COMPARE_EQUAL));
			}
		case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
			ASSERT(!"Unimplemented");
		case LLL_RUNTIME_OBJECT_TYPE_CHAR:
			ASSERT(!"Unimplemented");
		case LLL_RUNTIME_OBJECT_TYPE_TRUE:
			ASSERT(!"Unimplemented");
		case LLL_RUNTIME_OBJECT_TYPE_STRING:
			ASSERT(!"Unimplemented");
		case LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX:
			ASSERT(!"Stack Corruption");
		default:
			UNREACHABLE();
	}
}



uint8_t _runtime_object_zero(const lll_runtime_object_t* o,const lll_assembly_data_t* a_dt){
	switch (o->t){
		case LLL_RUNTIME_OBJECT_TYPE_NIL:
		case LLL_RUNTIME_OBJECT_TYPE_FALSE:
			return 1;
		case LLL_RUNTIME_OBJECT_TYPE_INT:
			return !!(o->dt.i);
		case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
			return !!(o->dt.f);
		case LLL_RUNTIME_OBJECT_TYPE_CHAR:
			return !!(o->dt.c);
		case LLL_RUNTIME_OBJECT_TYPE_TRUE:
			return 0;
		case LLL_RUNTIME_OBJECT_TYPE_STRING:
			return !!(o->dt.s->l);
		default:
			UNREACHABLE();
	}
}



__LLL_IMPORT_EXPORT __LLL_RETURN_CODE lll_run_assembly(const lll_assembly_data_t* a_dt,lll_stack_data_t* st,lll_input_data_stream_t* in,lll_output_data_stream_t* out){
	lll_assembly_instruction_t* ai=a_dt->h;
	lll_runtime_object_t* s=(lll_runtime_object_t*)(st->ptr);
	lll_runtime_object_t* v=malloc(a_dt->vc*sizeof(lll_runtime_object_t));
	lll_instruction_index_t ii=0;
	while (1){
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
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_NIL:
				s->t=LLL_RUNTIME_OBJECT_TYPE_NIL;
				s++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
				s->t=LLL_RUNTIME_OBJECT_TYPE_INT;
				s->dt.i=ai->dt.i;
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
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TRUE:
				s->t=LLL_RUNTIME_OBJECT_TYPE_TRUE;
				s++;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FALSE:
				s->t=LLL_RUNTIME_OBJECT_TYPE_FALSE;
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
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP:
_jump:
				ii=(LLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?ii+ai->dt.rj:ai->dt.j);
				ai=a_dt->h+ii;
				continue;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JB:
				if (_compare_runtime_object(s-2,s-1,a_dt)==RUNTIME_OBJECT_COMPARE_BELOW){
					s-=2;
					goto _jump;
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:
				{
					uint8_t cmp=_compare_runtime_object(s-2,s-1,a_dt);
					if (cmp==RUNTIME_OBJECT_COMPARE_BELOW||cmp==RUNTIME_OBJECT_COMPARE_EQUAL){
						s-=2;
						goto _jump;
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JA:
				if (_compare_runtime_object(s-2,s-1,a_dt)==RUNTIME_OBJECT_COMPARE_ABOVE){
					s-=2;
					goto _jump;
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:
				{
					uint8_t cmp=_compare_runtime_object(s-2,s-1,a_dt);
					if (cmp==RUNTIME_OBJECT_COMPARE_ABOVE||cmp==RUNTIME_OBJECT_COMPARE_EQUAL){
						s-=2;
						goto _jump;
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JE:
				if (_compare_runtime_object(s-2,s-1,a_dt)==RUNTIME_OBJECT_COMPARE_EQUAL){
					s-=2;
					goto _jump;
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:
				if (_compare_runtime_object(s-2,s-1,a_dt)!=RUNTIME_OBJECT_COMPARE_EQUAL){
					s-=2;
					goto _jump;
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:
				if (_runtime_object_zero(s-1,a_dt)){
					s--;
					goto _jump;
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ:
				if (!_runtime_object_zero(s-1,a_dt)){
					s--;
					goto _jump;
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_NOT:
				ASSERT(!"Unimplemented");
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_ADD:
				{
					s--;
					lll_runtime_object_t* a=s-1;
					lll_runtime_object_t* b=s;
					switch (a->t){
						case LLL_RUNTIME_OBJECT_TYPE_NIL:
						case LLL_RUNTIME_OBJECT_TYPE_FALSE:
							*(s-1)=*b;
							break;
						case LLL_RUNTIME_OBJECT_TYPE_INT:
							switch (b->t){
								case LLL_RUNTIME_OBJECT_TYPE_NIL:
								case LLL_RUNTIME_OBJECT_TYPE_FALSE:
									break;
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
								case LLL_RUNTIME_OBJECT_TYPE_TRUE:
									a->dt.i++;
									break;
								case LLL_RUNTIME_OBJECT_TYPE_STRING:
									ASSERT(!"Unimplemented");
								case LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX:
									ASSERT(!"Stack Corruption");
								default:
									UNREACHABLE();
							}
							break;
						case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
							ASSERT(!"Unimplemented");
						case LLL_RUNTIME_OBJECT_TYPE_CHAR:
							ASSERT(!"Unimplemented");
						case LLL_RUNTIME_OBJECT_TYPE_TRUE:
							ASSERT(!"Unimplemented");
						case LLL_RUNTIME_OBJECT_TYPE_STRING:
							ASSERT(!"Unimplemented");
						case LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX:
							ASSERT(!"Stack Corruption");
						default:
							UNREACHABLE();
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_SUB:
				ASSERT(!"Unimplemented");
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_MULT:
				{
					s--;
					lll_runtime_object_t* a=s-1;
					lll_runtime_object_t* b=s;
					switch (a->t){
						case LLL_RUNTIME_OBJECT_TYPE_NIL:
						case LLL_RUNTIME_OBJECT_TYPE_FALSE:
							break;
						case LLL_RUNTIME_OBJECT_TYPE_INT:
							switch (b->t){
								case LLL_RUNTIME_OBJECT_TYPE_NIL:
								case LLL_RUNTIME_OBJECT_TYPE_FALSE:
									a->t=LLL_RUNTIME_OBJECT_TYPE_NIL;
									break;
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
								case LLL_RUNTIME_OBJECT_TYPE_TRUE:
									break;
								case LLL_RUNTIME_OBJECT_TYPE_STRING:
									ASSERT(!"Unimplemented");
								case LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX:
									ASSERT(!"Stack Corruption");
								default:
									UNREACHABLE();
							}
							break;
						case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
							ASSERT(!"Unimplemented");
						case LLL_RUNTIME_OBJECT_TYPE_CHAR:
							ASSERT(!"Unimplemented");
						case LLL_RUNTIME_OBJECT_TYPE_TRUE:
							ASSERT(!"Unimplemented");
						case LLL_RUNTIME_OBJECT_TYPE_STRING:
							ASSERT(!"Unimplemented");
						case LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX:
							ASSERT(!"Stack Corruption");
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
				switch (s->t){
					case LLL_RUNTIME_OBJECT_TYPE_NIL:
						LLL_WRITE_TO_OUTPUT_DATA_STREAM(out,(uint8_t*)"nil",3*sizeof(char));
						break;
					case LLL_RUNTIME_OBJECT_TYPE_INT:
						{
							char bf[20];
							uint8_t i=0;
							int64_t v=s->dt.i;
							if (v<0){
								LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(out,'-');
								v=-v;
							}
							do{
								bf[i]=v%10;
								i++;
								v/=10;
							} while(v);
							while (i){
								i--;
								LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(out,bf[i]+48);
							}
							break;
						}
					case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
						ASSERT(!"Unimplemented");
					case LLL_RUNTIME_OBJECT_TYPE_CHAR:
						LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(out,(uint8_t)(s->dt.c));
						break;
					case LLL_RUNTIME_OBJECT_TYPE_TRUE:
						LLL_WRITE_TO_OUTPUT_DATA_STREAM(out,(uint8_t*)"true",4*sizeof(char));
						break;
					case LLL_RUNTIME_OBJECT_TYPE_FALSE:
						LLL_WRITE_TO_OUTPUT_DATA_STREAM(out,(uint8_t*)"false",5*sizeof(char));
						break;
					case LLL_RUNTIME_OBJECT_TYPE_STRING:
						LLL_WRITE_TO_OUTPUT_DATA_STREAM(out,(uint8_t*)(s->dt.s->v),s->dt.s->l*sizeof(lll_char_t));
						break;
					case LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX:
						ASSERT(!"Stack Corruption");
					default:
						UNREACHABLE();
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
				if ((s-1)->dt.i<0||(s-1)->dt.i>=a_dt->ft.l){
					s-=ai->dt.ac+1;
					(s-1)->t=LLL_RUNTIME_OBJECT_TYPE_NIL;
					break;
				}
				s--;
				ii=*(a_dt->ft.dt+s->dt.i);
				ai=a_dt->h+ii;
				continue;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET:
				ASSERT((s-2)->t=LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX);
				ii=(s-2)->dt.ii;
				ai=a_dt->h+ii;
				s--;
				*(s-1)=*s;
				continue;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_NIL:
				ASSERT((s-1)->t=LLL_RUNTIME_OBJECT_TYPE_INSTRUCTION_INDEX);
				ii=(s-1)->dt.ii;
				ai=a_dt->h+ii;
				(s-1)->t=LLL_RUNTIME_OBJECT_TYPE_NIL;
				continue;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_END:
			default:
				goto _end;
		}
		ai++;
		ii++;
	}
_end:;
	free(v);
	ASSERT(((uint8_t*)s)==st->ptr);
	return 0;
}
