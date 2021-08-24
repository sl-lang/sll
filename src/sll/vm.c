#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/constants.h>
#include <sll/gc.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/types.h>



#define OPERATOR_INSTRUCTION_UNARY(nm) \
	{ \
		sll_runtime_object_t** a=(SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s+si-1); \
		sll_runtime_object_t* n=sll_operator_##nm(*a); \
		SLL_RELEASE(*a); \
		*a=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION_BINARY(nm) \
	{ \
		si--; \
		sll_runtime_object_t** a=(SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s+si-1); \
		sll_runtime_object_t* n=sll_operator_##nm(*a,*(s+si)); \
		SLL_RELEASE(*a); \
		SLL_RELEASE(*(s+si)); \
		*a=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION_TERNARY(nm) \
	{ \
		si-=2; \
		sll_runtime_object_t** a=(SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s+si-1); \
		sll_runtime_object_t* n=sll_operator_##nm(*a,*(s+si),*(s+si+1)); \
		SLL_RELEASE(*a); \
		SLL_RELEASE(*(s+si)); \
		SLL_RELEASE(*(s+si+1)); \
		*a=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION_QUATERNARY(nm) \
	{ \
		si-=3; \
		sll_runtime_object_t** a=(SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s+si-1); \
		sll_runtime_object_t* n=sll_operator_##nm(*a,*(s+si),*(s+si+1),*(s+si+2)); \
		SLL_RELEASE(*a); \
		SLL_RELEASE(*(s+si)); \
		SLL_RELEASE(*(s+si+1)); \
		SLL_RELEASE(*(s+si+2)); \
		*a=n; \
		break; \
	}



__SLL_FUNC __SLL_RETURN_CODE sll_execute_assembly(const sll_assembly_data_t* a_dt,const sll_stack_data_t* st,const sll_internal_function_table_t* i_ft,sll_input_data_stream_t* in,sll_output_data_stream_t* out,sll_error_t* e){
	sll_return_code_t rc=0;
	const sll_assembly_instruction_t* ai=a_dt->h;
	sll_runtime_object_t** v=(sll_runtime_object_t**)(st->ptr);
	for (sll_variable_index_t i=0;i<a_dt->vc;i++){
		*(v+i)=SLL_ACQUIRE_STATIC(int_zero);
	}
	call_stack_t c_st={
		(call_stack_frame_t*)(st->ptr+a_dt->vc*sizeof(sll_runtime_object_t*)),
		0
	};
	sll_runtime_object_t** s=(sll_runtime_object_t**)(st->ptr+a_dt->vc*sizeof(sll_runtime_object_t*)+CALL_STACK_SIZE*sizeof(call_stack_frame_t));
	sll_stack_offset_t si=0;
	sll_stack_offset_t s_sz=(st->sz-a_dt->vc*sizeof(sll_runtime_object_t*)-CALL_STACK_SIZE*sizeof(call_stack_frame_t))/sizeof(sll_runtime_object_t*);
	sll_instruction_index_t ii=0;
	sll_runtime_object_t* cs=malloc(a_dt->st.l*sizeof(sll_runtime_object_t));
	for (sll_string_index_t i=0;i<a_dt->st.l;i++){
		(cs+i)->rc=1;
		(cs+i)->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
		(cs+i)->dt.s=*(a_dt->st.dt+i);
	}
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
				SLL_RELEASE(*(s+si));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO:
				si-=2;
				SLL_RELEASE(*(s+si));
				SLL_RELEASE(*(s+si+1));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT:
				{
					sll_runtime_object_t* tmp=*(s+si-1);
					*(s+si-1)=*(s+si-2);
					*(s+si-2)=tmp;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP:
				si--;
				SLL_RELEASE(*(s+si));
				*(s+si-1)=*(s+si);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DUP:
				*(s+si)=*(s+si-1);
				SLL_ACQUIRE(*(s+si));
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
				{
					sll_runtime_object_t* n=SLL_CREATE();
					*(s+si)=n;
					n->rc=1;
					n->t=SLL_RUNTIME_OBJECT_TYPE_INT;
					n->dt.i=ai->dt.i;
					si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE:
				*(s+si)=SLL_ACQUIRE_STATIC(int_minus_one);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO:
				*(s+si)=SLL_ACQUIRE_STATIC(int_zero);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE:
				*(s+si)=SLL_ACQUIRE_STATIC(int_one);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO:
				*(s+si)=SLL_ACQUIRE_STATIC(int_two);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE:
				*(s+si)=SLL_ACQUIRE_STATIC(int_three);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR:
				*(s+si)=SLL_ACQUIRE_STATIC(int_four);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
				{
					sll_runtime_object_t* n=SLL_CREATE();
					*(s+si)=n;
					n->rc=1;
					n->t=SLL_RUNTIME_OBJECT_TYPE_FLOAT;
					n->dt.f=ai->dt.f;
					si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
				{
					sll_runtime_object_t* n=SLL_CREATE();
					*(s+si)=n;
					n->rc=1;
					n->t=SLL_RUNTIME_OBJECT_TYPE_CHAR;
					n->dt.c=ai->dt.c;
					si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD:
				*(s+si)=*(v+ai->dt.v);
				SLL_ACQUIRE(*(s+si));
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
				*(s+si)=cs+ai->dt.s;
				SLL_ACQUIRE(*(s+si));
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK:
				{
					sll_runtime_object_t* n=SLL_CREATE();
					n->rc=1;
					n->t=SLL_RUNTIME_OBJECT_TYPE_ARRAY;
					sll_array_t* a=&(n->dt.a);
					sll_array_create(ai->dt.al,a);
					si-=ai->dt.al;
					for (sll_array_length_t i=0;i<ai->dt.al;i++){
						a->v[i]=*(s+si+i);
					}
					*(s+si)=n;
					si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO:
				*(s+si)=SLL_ACQUIRE_STATIC(array_zero);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ONE:
				SLL_UNIMPLEMENTED();
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE:
				*(v+ai->dt.v)=*(s+si-1);
				SLL_ACQUIRE(*(s+si-1));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP:
				si--;
				*(v+ai->dt.v)=*(s+si);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE:
				*(v+ai->dt.v)=SLL_ACQUIRE_STATIC(int_minus_one);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ZERO:
				*(v+ai->dt.v)=SLL_ACQUIRE_STATIC(int_zero);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ONE:
				*(v+ai->dt.v)=SLL_ACQUIRE_STATIC(int_one);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_TWO:
				*(v+ai->dt.v)=SLL_ACQUIRE_STATIC(int_two);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_THREE:
				*(v+ai->dt.v)=SLL_ACQUIRE_STATIC(int_three);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_FOUR:
				*(v+ai->dt.v)=SLL_ACQUIRE_STATIC(int_four);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP:
_jump:
				ii=(SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?ii+ai->dt.rj:ai->dt.j);
				ai=a_dt->h+ii;
				continue;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JB:
				{
					if (sll_operator_compare(*(s+si-2),*(s+si-1))==SLL_COMPARE_RESULT_BELOW){
						si-=2;
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:
				{
					if (sll_operator_compare(*(s+si-2),*(s+si-1))!=SLL_COMPARE_RESULT_ABOVE){
						si-=2;
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JA:
				{
					if (sll_operator_compare(*(s+si-2),*(s+si-1))==SLL_COMPARE_RESULT_ABOVE){
						si-=2;
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:
				{
					if (sll_operator_compare(*(s+si-2),*(s+si-1))!=SLL_COMPARE_RESULT_BELOW){
						si-=2;
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JE:
				{
					if (sll_operator_compare(*(s+si-2),*(s+si-1))==SLL_COMPARE_RESULT_EQUAL){
						si-=2;
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:
				{
					if (sll_operator_compare(*(s+si-2),*(s+si-1))!=SLL_COMPARE_RESULT_EQUAL){
						si-=2;
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:
				{
					if (sll_operator_bool(*(s+si-1))==SLL_COMPARE_RESULT_ZERO){
						si--;
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ:
				{
					if (sll_operator_bool(*(s+si-1))==SLL_COMPARE_RESULT_NONZERO){
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
				OPERATOR_INSTRUCTION_BINARY(add);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_SUB:
				OPERATOR_INSTRUCTION_BINARY(sub);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MULT:
				OPERATOR_INSTRUCTION_BINARY(mult);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DIV:
				OPERATOR_INSTRUCTION_BINARY(div);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_FDIV:
				OPERATOR_INSTRUCTION_BINARY(floor_div);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MOD:
				OPERATOR_INSTRUCTION_BINARY(mod);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_AND:
				OPERATOR_INSTRUCTION_BINARY(and);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_OR:
				OPERATOR_INSTRUCTION_BINARY(or);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_XOR:
				OPERATOR_INSTRUCTION_BINARY(xor);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_INV:
				OPERATOR_INSTRUCTION_UNARY(inv);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LENGTH:
				OPERATOR_INSTRUCTION_UNARY(len);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_COPY:
				OPERATOR_INSTRUCTION_UNARY(dup);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS:
				OPERATOR_INSTRUCTION_BINARY(access);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_TWO:
				OPERATOR_INSTRUCTION_TERNARY(access_range);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_THREE:
				OPERATOR_INSTRUCTION_QUATERNARY(access_range_step);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT:
				{
					si--;
_print_from_stack:;
					sll_runtime_object_t* e=*(s+si);
					if (e->t==SLL_RUNTIME_OBJECT_TYPE_STRING){
						fwrite(e->dt.s.v,sizeof(sll_char_t),e->dt.s.l,stdout);
					}
					else{
						sll_string_t str;
						sll_object_to_string(&e,1,&str);
						fwrite(str.v,sizeof(sll_char_t),str.l,stdout);
						free(str.v);
					}
					SLL_RELEASE(e);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(out,ai->dt.c);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
				{
					sll_string_t* str=a_dt->st.dt+ai->dt.s;
					SLL_WRITE_TO_OUTPUT_DATA_STREAM(out,str->v,str->l*sizeof(sll_char_t));
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR:
				*(s+si)=*(v+ai->dt.v);
				SLL_ACQUIRE(*(s+si));
				goto _print_from_stack;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP:
				{
					si--;
					sll_runtime_object_t* e=*(s+si);
					if (e->dt.i<0){
						sll_function_index_t i=(sll_function_index_t)(~e->dt.i);
						if (i<i_ft->l){
							si-=ai->dt.ac;
							sll_runtime_object_t* n=(*(i_ft->dt+i))->p(s+si,ai->dt.ac);
							for (sll_arg_count_t j=0;j<ai->dt.ac;j++){
								SLL_RELEASE(*(s+si+j));
							}
							if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)!=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP){
								*(s+si)=n;
								si++;
							}
							break;
						}
					}
					else if (e->dt.i&&e->dt.i<=a_dt->ft.l){
						SLL_ASSERT(c_st.l<=CALL_STACK_SIZE);
						(c_st.dt+c_st.l)->ii=ii;
						(c_st.dt+c_st.l)->s=si-ai->dt.ac;
						c_st.l++;
						ii=*(a_dt->ft.dt+e->dt.i-1);
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
						*(s+si-1)=SLL_ACQUIRE_STATIC(int_zero);
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO:
				if (ai->dt.i<0){
					sll_function_index_t i=(sll_function_index_t)(~ai->dt.i);
					if (i<i_ft->l){
						sll_runtime_object_t* n=(*(i_ft->dt+i))->p(NULL,0);
						*(s+si)=n;
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
				*(s+si)=SLL_ACQUIRE_STATIC(int_zero);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:
				if (ai->dt.i<0){
					sll_function_index_t i=(sll_function_index_t)(~ai->dt.i);
					if (i<i_ft->l){
						sll_runtime_object_t* n=(*(i_ft->dt+i))->p(s+si-1,1);
						SLL_RELEASE(*(s+si-1));
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
				*(s+si-1)=SLL_ACQUIRE_STATIC(int_zero);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET:
_return:;
				sll_runtime_object_t* tmp=*(s+si-1);
				SLL_ASSERT(c_st.l);
				c_st.l--;
				ii=(c_st.dt+c_st.l)->ii;
				ai=a_dt->h+ii;
				si=(c_st.dt+c_st.l)->s;
				if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)!=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP){
					*(s+si)=tmp;
					si++;
				}
				ii++;
				ai++;
				continue;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO:
				*(s+si)=SLL_ACQUIRE_STATIC(int_zero);
				si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				{
					sll_runtime_object_t* n=SLL_CREATE();
					n->rc=1;
					n->t=SLL_RUNTIME_OBJECT_TYPE_INT;
					n->dt.i=ai->dt.i;
					*(s+si)=n;
					si++;
					goto _return;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				{
					sll_runtime_object_t* n=SLL_CREATE();
					n->rc=1;
					n->t=SLL_RUNTIME_OBJECT_TYPE_FLOAT;
					n->dt.f=ai->dt.f;
					*(s+si)=n;
					si++;
					goto _return;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				{
					sll_runtime_object_t* n=SLL_CREATE();
					n->rc=1;
					n->t=SLL_RUNTIME_OBJECT_TYPE_CHAR;
					n->dt.c=ai->dt.c;
					*(s+si)=n;
					si++;
					goto _return;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				*(s+si)=cs+ai->dt.s;
				SLL_ACQUIRE(*(s+si));
				si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR:
				*(s+si)=*(v+ai->dt.v);
				SLL_ACQUIRE(*(s+si));
				si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_END:
				{
					si--;
					if (s<v+a_dt->vc){
						e->t=SLL_ERROR_STACK_CORRUPTED;
						goto _clanup;
					}
					sll_runtime_object_t* e=*(s+si);
					switch (SLL_RUNTIME_OBJECT_GET_TYPE(e)){
						case SLL_RUNTIME_OBJECT_TYPE_INT:
							rc=(e->dt.i<INT32_MIN?INT32_MIN:(e->dt.i>INT32_MAX?INT32_MAX:(sll_return_code_t)e->dt.i));
							goto _clanup;
						case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
							rc=(e->dt.f<INT32_MIN?INT32_MIN:(e->dt.f>INT32_MAX?INT32_MAX:(sll_return_code_t)e->dt.f));
							goto _clanup;
						case SLL_RUNTIME_OBJECT_TYPE_CHAR:
							rc=e->dt.c;
							goto _clanup;
						case SLL_RUNTIME_OBJECT_TYPE_STRING:
							rc=(e->dt.s.l>INT32_MAX?INT32_MAX:e->dt.s.l);
							goto _clanup;
						case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
							rc=(e->dt.a.l>INT32_MAX?INT32_MAX:e->dt.a.l);
							goto _clanup;
						default:
							SLL_UNREACHABLE();
					}
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_END_ZERO:
				goto _clanup;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_END_ONE:
				rc=1;
				goto _clanup;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL:
				SLL_RELEASE(*(v+ai->dt.v));
				*(v+ai->dt.v)=SLL_ACQUIRE_STATIC(int_zero);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL:
				*(s+si)=*(v+ai->dt.v);
				*(v+ai->dt.v)=SLL_ACQUIRE_STATIC(int_zero);
				si++;
				break;
			default:
				e->t=SLL_ERROR_INVALID_INSTRUCTION;
				e->dt.it=SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai);
				goto _clanup;
		}
		ai++;
		ii++;
		if (si>=s_sz){
			e->t=SLL_ERROR_INVALID_STACK_INDEX;
			goto _clanup;
		}
	}
_clanup:;
	free(cs);
	return rc;
}
