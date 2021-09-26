#include <sll/_sll_internal.h>
#include <sll/api/string.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/constants.h>
#include <sll/core.h>
#include <sll/gc.h>
#include <sll/handle.h>
#include <sll/map.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <stdlib.h>



#define OPERATOR_INSTRUCTION_UNARY(nm) \
	{ \
		sll_runtime_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s+si-1); \
		sll_runtime_object_t* n=sll_operator_##nm(*tos); \
		SLL_RELEASE(*tos); \
		*tos=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION_BINARY(nm) \
	{ \
		si--; \
		sll_runtime_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s+si-1); \
		sll_runtime_object_t* n=sll_operator_##nm(*tos,*(s+si)); \
		SLL_RELEASE(*tos); \
		SLL_RELEASE(*(s+si)); \
		*tos=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION_TERNARY(nm) \
	{ \
		si-=2; \
		sll_runtime_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s+si-1); \
		sll_runtime_object_t* n=sll_operator_##nm(*tos,*(s+si),*(s+si+1)); \
		SLL_RELEASE(*tos); \
		SLL_RELEASE(*(s+si)); \
		SLL_RELEASE(*(s+si+1)); \
		*tos=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION_QUATERNARY(nm) \
	{ \
		si-=3; \
		sll_runtime_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s+si-1); \
		sll_runtime_object_t* n=sll_operator_##nm(*tos,*(s+si),*(s+si+1),*(s+si+2)); \
		SLL_RELEASE(*tos); \
		SLL_RELEASE(*(s+si)); \
		SLL_RELEASE(*(s+si+1)); \
		SLL_RELEASE(*(s+si+2)); \
		*tos=n; \
		break; \
	}



sll_integer_t sll_current_instruction_count=0;



const sll_runtime_data_t* sll_current_runtime_data=NULL;



__SLL_FUNC __SLL_CHECK_OUTPUT sll_return_code_t sll_execute_assembly(const sll_assembly_data_t* a_dt,const sll_stack_data_t* st,const sll_runtime_data_t* r_dt,sll_error_t* e){
	e->t=SLL_ERROR_UNKNOWN;
	sll_current_instruction_count=0;
	sll_current_runtime_data=r_dt;
	sll_handle_type_t hl_l=r_dt->hl->dtl;
	sll_internal_function_table_t* i_ft=r_dt->ift;
	sll_output_data_stream_t* out=r_dt->os;
	sll_return_code_t rc=0;
	const sll_assembly_instruction_t* ai=a_dt->h;
	sll_buffer_t ptr=st->ptr;
	sll_runtime_object_t** v=(sll_runtime_object_t**)ptr;
	sll_static_int[0]->rc+=a_dt->vc;
	for (sll_variable_index_t i=0;i<a_dt->vc;i++){
		*(v+i)=sll_static_int[0];
	}
	ptr+=a_dt->vc*sizeof(sll_runtime_object_t*);
	sll_runtime_object_t* cs=(sll_runtime_object_t*)ptr;
	for (sll_string_index_t i=0;i<a_dt->st.l;i++){
		(cs+i)->rc=1;
		(cs+i)->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
		(cs+i)->dt.s=*(a_dt->st.dt+i);
	}
	ptr+=a_dt->st.l*sizeof(sll_runtime_object_t);
	call_stack_t c_st={
		(call_stack_frame_t*)ptr,
		0
	};
	ptr+=CALL_STACK_SIZE*sizeof(call_stack_frame_t);
	sll_runtime_object_t** s=(sll_runtime_object_t**)ptr;
	sll_stack_offset_t si=0;
	SLL_ASSERT(st->ptr+st->sz>=ptr);
	sll_stack_offset_t s_sz=(sll_stack_offset_t)(st->ptr+st->sz-ptr);
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
				SLL_RELEASE(*(s+si));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO:
				si-=2;
				SLL_RELEASE(*(s+si));
				SLL_RELEASE(*(s+si+1));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT:
				{
					sll_runtime_object_t* tos=*(s+si-1);
					*(s+si-1)=*(s+si-2);
					*(s+si-2)=tos;
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
					sll_runtime_object_t* tos=SLL_CREATE();
					*(s+si)=tos;
					tos->t=SLL_RUNTIME_OBJECT_TYPE_INT;
					tos->dt.i=ai->dt.i;
					si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE:
				*(s+si)=SLL_ACQUIRE_STATIC_INT(-1);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO:
				*(s+si)=SLL_ACQUIRE_STATIC_INT(0);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE:
				*(s+si)=SLL_ACQUIRE_STATIC_INT(1);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO:
				*(s+si)=SLL_ACQUIRE_STATIC_INT(2);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE:
				*(s+si)=SLL_ACQUIRE_STATIC_INT(3);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR:
				*(s+si)=SLL_ACQUIRE_STATIC_INT(4);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
				{
					sll_runtime_object_t* tos=SLL_CREATE();
					*(s+si)=tos;
					tos->t=SLL_RUNTIME_OBJECT_TYPE_FLOAT;
					tos->dt.f=ai->dt.f;
					si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
				*(s+si)=SLL_ACQUIRE_STATIC_CHAR(ai->dt.c);
				si++;
				break;
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
					sll_runtime_object_t* tos=SLL_CREATE();
					tos->t=SLL_RUNTIME_OBJECT_TYPE_ARRAY;
					sll_array_t* a=&(tos->dt.a);
					sll_array_create(ai->dt.al,a);
					si-=ai->dt.al;
					for (sll_array_length_t i=0;i<ai->dt.al;i++){
						a->v[i]=*(s+si+i);
					}
					*(s+si)=tos;
					si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO:
				*(s+si)=SLL_ACQUIRE_STATIC(array_zero);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ONE:
				{
					sll_runtime_object_t* tos=SLL_CREATE();
					tos->t=SLL_RUNTIME_OBJECT_TYPE_ARRAY;
					sll_array_create(1,&(tos->dt.a));
					tos->dt.a.v[0]=*(s+si-1);
					*(s+si-1)=tos;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP:
				{
					sll_runtime_object_t* tos=SLL_CREATE();
					tos->t=SLL_RUNTIME_OBJECT_TYPE_MAP;
					sll_map_t* m=&(tos->dt.m);
					sll_map_create((ai->dt.ml+1)>>1,m);
					si-=ai->dt.ml;
					for (sll_map_length_t i=0;i<ai->dt.ml;i++){
						m->v[i]=*(s+si+i);
					}
					if (ai->dt.ml&1){
						m->v[ai->dt.ml]=SLL_ACQUIRE_STATIC_INT(0);
					}
					*(s+si)=tos;
					si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP_ZERO:
				*(s+si)=SLL_ACQUIRE_STATIC(map_zero);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE:
				SLL_RELEASE(*(v+ai->dt.v));
				*(v+ai->dt.v)=*(s+si-1);
				SLL_ACQUIRE(*(s+si-1));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP:
				SLL_RELEASE(*(v+ai->dt.v));
				si--;
				*(v+ai->dt.v)=*(s+si);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE:
				SLL_RELEASE(*(v+ai->dt.v));
				*(v+ai->dt.v)=SLL_ACQUIRE_STATIC_INT(-1);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ZERO:
				SLL_RELEASE(*(v+ai->dt.v));
				*(v+ai->dt.v)=SLL_ACQUIRE_STATIC_INT(0);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ONE:
				SLL_RELEASE(*(v+ai->dt.v));
				*(v+ai->dt.v)=SLL_ACQUIRE_STATIC_INT(1);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_TWO:
				SLL_RELEASE(*(v+ai->dt.v));
				*(v+ai->dt.v)=SLL_ACQUIRE_STATIC_INT(2);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_THREE:
				SLL_RELEASE(*(v+ai->dt.v));
				*(v+ai->dt.v)=SLL_ACQUIRE_STATIC_INT(3);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_FOUR:
				SLL_RELEASE(*(v+ai->dt.v));
				*(v+ai->dt.v)=SLL_ACQUIRE_STATIC_INT(4);
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
						SLL_RELEASE(*(s+si));
						SLL_RELEASE(*(s+si+1));
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:
				{
					if (sll_operator_compare(*(s+si-2),*(s+si-1))!=SLL_COMPARE_RESULT_ABOVE){
						si-=2;
						SLL_RELEASE(*(s+si));
						SLL_RELEASE(*(s+si+1));
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JA:
				{
					if (sll_operator_compare(*(s+si-2),*(s+si-1))==SLL_COMPARE_RESULT_ABOVE){
						si-=2;
						SLL_RELEASE(*(s+si));
						SLL_RELEASE(*(s+si+1));
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:
				{
					if (sll_operator_compare(*(s+si-2),*(s+si-1))!=SLL_COMPARE_RESULT_BELOW){
						si-=2;
						SLL_RELEASE(*(s+si));
						SLL_RELEASE(*(s+si+1));
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JE:
				{
					if (sll_operator_equal(*(s+si-2),*(s+si-1))){
						si-=2;
						SLL_RELEASE(*(s+si));
						SLL_RELEASE(*(s+si+1));
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:
				{
					if (!sll_operator_equal(*(s+si-2),*(s+si-1))){
						si-=2;
						SLL_RELEASE(*(s+si));
						SLL_RELEASE(*(s+si+1));
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:
				{
					if (!sll_operator_bool(*(s+si-1))){
						si--;
						SLL_RELEASE(*(s+si));
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ:
				{
					if (sll_operator_bool(*(s+si-1))){
						si--;
						SLL_RELEASE(*(s+si));
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
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_SHR:
				OPERATOR_INSTRUCTION_BINARY(shr);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_SHL:
				OPERATOR_INSTRUCTION_BINARY(shl);
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
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST:
				OPERATOR_INSTRUCTION_BINARY(cast);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE:
				{
					sll_runtime_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s+si-1);
					sll_runtime_object_t* t=SLL_CREATE();
					t->t=SLL_RUNTIME_OBJECT_TYPE_INT;
					t->dt.i=ai->dt.t;
					sll_runtime_object_t* n=sll_operator_cast(*tos,t);
					SLL_RELEASE(*tos);
					SLL_RELEASE(t);
					*tos=n;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT:
				{
					si--;
_print_from_stack:;
					sll_runtime_object_t* tos=*(s+si);
					if (tos->t==SLL_RUNTIME_OBJECT_TYPE_STRING){
						SLL_WRITE_TO_OUTPUT_DATA_STREAM(r_dt->os,tos->dt.s.v,tos->dt.s.l*sizeof(sll_char_t));
					}
					else{
						sll_string_t str;
						sll_object_to_string((const sll_runtime_object_t*const*)&tos,1,&str);
						SLL_WRITE_TO_OUTPUT_DATA_STREAM(r_dt->os,str.v,str.l*sizeof(sll_char_t));
						free(str.v);
					}
					SLL_RELEASE(tos);
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
					sll_runtime_object_t* tos=*(s+si);
					if (SLL_RUNTIME_OBJECT_GET_TYPE(tos)==SLL_RUNTIME_OBJECT_TYPE_INT){
						sll_integer_t i=tos->dt.i;
						SLL_RELEASE(tos);
						if (i<0){
							sll_function_index_t j=(sll_function_index_t)(~i);
							if (j<i_ft->l){
								si-=ai->dt.ac;
								sll_runtime_object_t* n=(*(i_ft->dt+j))->p((const sll_runtime_object_t*const*)(s+si),ai->dt.ac);
								for (sll_arg_count_t k=0;k<ai->dt.ac;k++){
									SLL_RELEASE(*(s+si+k));
								}
								if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)!=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP){
									*(s+si)=n;
									si++;
								}
								else{
									SLL_RELEASE(n);
								}
								break;
							}
						}
						else if (i&&i<=a_dt->ft.l){
							SLL_ASSERT(c_st.l<=CALL_STACK_SIZE);
							(c_st.dt+c_st.l)->ii=ii;
							(c_st.dt+c_st.l)->s=si-ai->dt.ac;
							c_st.l++;
							ii=*(a_dt->ft.dt+i-1);
							ai=a_dt->h+ii;
							continue;
						}
					}
					SLL_RELEASE(tos);
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
						*(s+si-1)=SLL_ACQUIRE_STATIC_INT(0);
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO:
				if (ai->dt.i<0){
					sll_function_index_t i=(sll_function_index_t)(~ai->dt.i);
					if (i<i_ft->l){
						*(s+si)=(*(i_ft->dt+i))->p(NULL,0);
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
				*(s+si)=SLL_ACQUIRE_STATIC_INT(0);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:
				if (ai->dt.i<0){
					sll_function_index_t i=(sll_function_index_t)(~ai->dt.i);
					if (i<i_ft->l){
						sll_runtime_object_t* n=(*(i_ft->dt+i))->p((const sll_runtime_object_t*const*)(s+si-1),1);
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
				*(s+si-1)=SLL_ACQUIRE_STATIC_INT(0);
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
				else{
					SLL_RELEASE(tmp);
				}
				ii++;
				ai++;
				continue;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO:
				*(s+si)=SLL_ACQUIRE_STATIC_INT(0);
				si++;
				goto _return;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				{
					sll_runtime_object_t* tos=SLL_CREATE();
					tos->t=SLL_RUNTIME_OBJECT_TYPE_INT;
					tos->dt.i=ai->dt.i;
					*(s+si)=tos;
					si++;
					goto _return;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				{
					sll_runtime_object_t* tos=SLL_CREATE();
					tos->t=SLL_RUNTIME_OBJECT_TYPE_FLOAT;
					tos->dt.f=ai->dt.f;
					*(s+si)=tos;
					si++;
					goto _return;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				*(s+si)=SLL_ACQUIRE_STATIC_CHAR(ai->dt.c);
				si++;
				goto _return;
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
						goto _end;
					}
					sll_runtime_object_t* tos=*(s+si);
					switch (SLL_RUNTIME_OBJECT_GET_TYPE(tos)){
						case SLL_RUNTIME_OBJECT_TYPE_INT:
							rc=(tos->dt.i<INT32_MIN?INT32_MIN:(tos->dt.i>INT32_MAX?INT32_MAX:(sll_return_code_t)tos->dt.i));
							goto _end;
						case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
							rc=(tos->dt.f<INT32_MIN?INT32_MIN:(tos->dt.f>INT32_MAX?INT32_MAX:(sll_return_code_t)tos->dt.f));
							goto _end;
						case SLL_RUNTIME_OBJECT_TYPE_CHAR:
							rc=tos->dt.c;
							goto _end;
						case SLL_RUNTIME_OBJECT_TYPE_STRING:
							rc=(tos->dt.s.l>INT32_MAX?INT32_MAX:tos->dt.s.l);
							goto _end;
						case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
							rc=(tos->dt.a.l>INT32_MAX?INT32_MAX:tos->dt.a.l);
							goto _end;
						default:
							SLL_UNREACHABLE();
					}
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_END_ZERO:
				goto _end;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_END_ONE:
				rc=1;
				goto _end;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL:
				SLL_RELEASE(*(v+ai->dt.v));
				*(v+ai->dt.v)=SLL_ACQUIRE_STATIC_INT(0);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL:
				*(s+si)=*(v+ai->dt.v);
				*(v+ai->dt.v)=SLL_ACQUIRE_STATIC_INT(0);
				si++;
				break;
			default:
				e->t=SLL_ERROR_INVALID_INSTRUCTION;
				e->dt.it=SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai);
				goto _end;
		}
		sll_current_instruction_count++;
		ai++;
		if (ai->t==ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			ai=ai->dt._p;
		}
		ii++;
		if (si>=s_sz){
			e->t=SLL_ERROR_INVALID_STACK_INDEX;
		}
	}
_end:
	for (sll_variable_index_t i=0;i<a_dt->vc;i++){
		SLL_RELEASE(*(v+i));
	}
	sll_cleanup_handles(r_dt->hl,hl_l);
	sll_current_runtime_data=NULL;
	return rc;
}
