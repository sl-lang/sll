#include <sll/_sll_internal.h>
#include <sll/api/memory.h>
#include <sll/api/string.h>
#include <sll/array.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/handle.h>
#include <sll/map.h>
#include <sll/operator.h>
#include <sll/platform.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <sll/util.h>



#define OPERATOR_INSTRUCTION_UNARY(nm) \
	{ \
		sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s+si-1); \
		sll_object_t* n=nm(*tos); \
		SLL_RELEASE(*tos); \
		*tos=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION_BINARY(nm) \
	{ \
		si--; \
		sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s+si-1); \
		sll_object_t* n=nm(*tos,*(s+si)); \
		SLL_RELEASE(*tos); \
		SLL_RELEASE(*(s+si)); \
		*tos=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION_TERNARY(nm) \
	{ \
		si-=2; \
		sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s+si-1); \
		sll_object_t* n=nm(*tos,*(s+si),*(s+si+1)); \
		SLL_RELEASE(*tos); \
		SLL_RELEASE(*(s+si)); \
		SLL_RELEASE(*(s+si+1)); \
		*tos=n; \
		break; \
	}
#define OPERATOR_INSTRUCTION_QUATERNARY(nm) \
	{ \
		si-=3; \
		sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s+si-1); \
		sll_object_t* n=nm(*tos,*(s+si),*(s+si+1),*(s+si+2)); \
		SLL_RELEASE(*tos); \
		SLL_RELEASE(*(s+si)); \
		SLL_RELEASE(*(s+si+1)); \
		SLL_RELEASE(*(s+si+2)); \
		*tos=n; \
		break; \
	}



__SLL_EXTERNAL sll_integer_t sll_current_instruction_count=0;
__SLL_EXTERNAL const sll_runtime_data_t* sll_current_runtime_data=NULL;



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_code_t sll_execute_assembly(const sll_assembly_data_t* a_dt,sll_page_size_t s_sz,const sll_runtime_data_t* r_dt,sll_error_t* e){
	e->t=SLL_ERROR_UNKNOWN;
	sll_current_instruction_count=0;
	sll_current_runtime_data=r_dt;
	const sll_assembly_instruction_t* ai=a_dt->h;
	sll_page_size_t sz=sll_platform_get_page_size();
	sll_page_size_t ptr_sz=(s_sz+a_dt->vc*sizeof(sll_object_t*)+a_dt->st.l*sizeof(sll_object_t)+CALL_STACK_SIZE*sizeof(call_stack_frame_t)+sz-1)&(0xffffffffffffffffull-(sz-1));
	uint64_t ptr=(uint64_t)sll_platform_allocate_page(ptr_sz);
	sll_object_t** v=(sll_object_t**)ptr;
	sll_static_int[0]->rc+=a_dt->vc;
	for (sll_variable_index_t i=0;i<a_dt->vc;i++){
		*(v+i)=sll_static_int[0];
	}
	ptr+=a_dt->vc*sizeof(sll_object_t*);
	sll_object_t* cs=(sll_object_t*)ptr;
	for (sll_string_index_t i=0;i<a_dt->st.l;i++){
		SLL_GC_ZERO_DEBUG_DATA(cs+i);
		(cs+i)->rc=1;
		(cs+i)->t=SLL_OBJECT_TYPE_STRING;
		(cs+i)->dt.s=*(a_dt->st.dt+i);
	}
	ptr+=a_dt->st.l*sizeof(sll_object_t);
	call_stack_t c_st={
		(call_stack_frame_t*)ptr,
		0
	};
	ptr+=CALL_STACK_SIZE*sizeof(call_stack_frame_t);
	sll_handle_type_t hl_l=r_dt->hl->dtl;
	sll_return_code_t rc=0;
	sll_object_t** s=(sll_object_t**)ptr;
	stack_offset_t si=0;
	sll_instruction_index_t ii=0;
	sll_bool_t io=!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)||sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO);
	while (1){
		if (ii>=a_dt->ic){
			e->t=SLL_ERROR_INVALID_INSTRUCTION_INDEX;
			goto _end;
		}
		sll_current_instruction_count++;
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
					sll_object_t* tos=*(s+si-1);
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
					sll_object_t* tos=SLL_CREATE();
					*(s+si)=tos;
					tos->t=SLL_OBJECT_TYPE_INT;
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
					sll_object_t* tos=SLL_CREATE();
					*(s+si)=tos;
					tos->t=SLL_OBJECT_TYPE_FLOAT;
					tos->dt.f=ai->dt.f;
					si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
				*(s+si)=SLL_ACQUIRE_STATIC_CHAR(ai->dt.c);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_NULL:
				*(s+si)=sll_memory_get_null_pointer();
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL:
				*(s+si)=SLL_FROM_INT((SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?ii+ai->dt.rj:ai->dt.j));
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
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_ARRAY;
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
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_ARRAY;
					sll_array_create(1,&(tos->dt.a));
					tos->dt.a.v[0]=*(s+si-1);
					*(s+si-1)=tos;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP:
				{
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_MAP;
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
				ai=_get_instruction_at_offset(a_dt,ii);
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
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JSE:
				{
					if (sll_operator_strict_equal(*(s+si-2),*(s+si-1))){
						si-=2;
						SLL_RELEASE(*(s+si));
						SLL_RELEASE(*(s+si+1));
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JSNE:
				{
					if (!sll_operator_strict_equal(*(s+si-2),*(s+si-1))){
						si-=2;
						SLL_RELEASE(*(s+si));
						SLL_RELEASE(*(s+si+1));
						goto _jump;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JT:
				{
					si--;
					stack_offset_t e_si=si;
					si-=ai->dt.al<<1;
					stack_offset_t i=si-1;
					sll_object_t* cnd=*(s+si-1);
					sll_integer_t cnd_v;
					if (SLL_OBJECT_GET_TYPE(cnd)==SLL_OBJECT_TYPE_INT){
						cnd_v=cnd->dt.i;
					}
					else if (SLL_OBJECT_GET_TYPE(cnd)==SLL_OBJECT_TYPE_CHAR){
						cnd_v=cnd->dt.c;
					}
					else{
						SLL_RELEASE(cnd);
						while (si<e_si){
							SLL_RELEASE(*(s+si));
							si++;
						}
						goto _jump_to_default;
					}
					SLL_RELEASE(cnd);
					while (si<e_si){
						sll_object_t* k=*(s+si);
						if (SLL_OBJECT_GET_TYPE(k)==SLL_OBJECT_TYPE_INT){
							if (k->dt.i==cnd_v){
								goto _cleanup_jump_table;
							}
						}
						else if (SLL_OBJECT_GET_TYPE(k)==SLL_OBJECT_TYPE_CHAR){
							if (k->dt.c==cnd_v){
								goto _cleanup_jump_table;
							}
						}
						SLL_RELEASE(k);
						SLL_RELEASE(*(s+si+1));
						si+=2;
					}
_jump_to_default:
					SLL_ASSERT(si==e_si);
					ii=(sll_instruction_index_t)((*(s+si))->dt.i);
					ai=_get_instruction_at_offset(a_dt,ii);
					SLL_RELEASE(*(s+si));
					si=i;
					continue;
_cleanup_jump_table:;
					ii=(sll_instruction_index_t)((*(s+si+1))->dt.i);
					ai=_get_instruction_at_offset(a_dt,ii);
					while (si<e_si){
						SLL_RELEASE(*(s+si));
						si++;
					}
					SLL_RELEASE(*(s+si));
					si=i;
					continue;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s+si-1);
					sll_object_t* n=sll_static_int[!sll_operator_bool(*tos)];
					SLL_ACQUIRE(n);
					SLL_RELEASE(*tos);
					*tos=n;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_BOOL:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s+si-1);
					sll_object_t* n=sll_static_int[sll_operator_bool(*tos)];
					SLL_ACQUIRE(n);
					SLL_RELEASE(*tos);
					*tos=n;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_INC:
				OPERATOR_INSTRUCTION_UNARY(sll_operator_inc);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DEC:
				OPERATOR_INSTRUCTION_UNARY(sll_operator_dec);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_add);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_SUB:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_sub);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MULT:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_mult);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DIV:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_div);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_FDIV:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_floor_div);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MOD:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_mod);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_AND:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_and);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_OR:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_or);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_XOR:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_xor);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_INV:
				OPERATOR_INSTRUCTION_UNARY(sll_operator_inv);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_SHR:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_shr);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_SHL:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_shl);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LENGTH:
				OPERATOR_INSTRUCTION_UNARY(sll_operator_len);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_COPY:
				OPERATOR_INSTRUCTION_UNARY(sll_operator_dup);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_access);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_TWO:
				OPERATOR_INSTRUCTION_TERNARY(sll_operator_access_range);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_THREE:
				OPERATOR_INSTRUCTION_QUATERNARY(sll_operator_access_range_step);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST:
				OPERATOR_INSTRUCTION_BINARY(sll_operator_cast);
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s+si-1);
					sll_object_t* t=SLL_CREATE();
					t->t=SLL_OBJECT_TYPE_INT;
					t->dt.i=ai->dt.t;
					sll_object_t* n=sll_operator_cast(*tos,t);
					SLL_RELEASE(*tos);
					SLL_RELEASE(t);
					*tos=n;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_TYPEOF:
				{
					sll_object_t** tos=(SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?v+ai->dt.v:s+si-1);
					sll_object_t* n=sll_static_int[SLL_OBJECT_GET_TYPE(*tos)];
					SLL_ACQUIRE(n);
					SLL_RELEASE(*tos);
					*tos=n;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT:
				{
					si--;
_print_from_stack:;
					sll_object_t* tos=*(s+si);
					if (io){
						if (tos->t==SLL_OBJECT_TYPE_STRING){
							sll_file_write(r_dt->out,tos->dt.s.v,tos->dt.s.l*sizeof(sll_char_t));
						}
						else{
							sll_string_t str;
							sll_api_string_convert((const sll_object_t*const*)&tos,1,&str);
							sll_file_write(r_dt->out,str.v,str.l*sizeof(sll_char_t));
							sll_free_string(&str);
						}
					}
					SLL_RELEASE(tos);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
				if (io){
					sll_file_write_char(r_dt->out,ai->dt.c);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
				if (io){
					sll_file_write(r_dt->out,(a_dt->st.dt+ai->dt.s)->v,(a_dt->st.dt+ai->dt.s)->l*sizeof(sll_char_t));
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR:
				*(s+si)=*(v+ai->dt.v);
				SLL_ACQUIRE(*(s+si));
				goto _print_from_stack;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP:
				{
					si--;
					sll_object_t* tos=*(s+si);
					if (SLL_OBJECT_GET_TYPE(tos)==SLL_OBJECT_TYPE_INT){
						sll_integer_t i=tos->dt.i;
						SLL_RELEASE(tos);
						if (i<0){
							sll_function_index_t j=(sll_function_index_t)(~i);
							if (j<r_dt->ift->l){
								si-=ai->dt.ac;
								sll_object_t* n=(*(r_dt->ift->dt+j))->p((const sll_object_t*const*)(s+si),ai->dt.ac);
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
							ai=_get_instruction_at_offset(a_dt,ii);
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
					if (i<r_dt->ift->l){
						*(s+si)=(*(r_dt->ift->dt+i))->p(NULL,0);
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
					ai=_get_instruction_at_offset(a_dt,ii);
					continue;
				}
				*(s+si)=SLL_ACQUIRE_STATIC_INT(0);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:
				if (ai->dt.i<0){
					sll_function_index_t i=(sll_function_index_t)(~ai->dt.i);
					if (i<r_dt->ift->l){
						sll_object_t* n=(*(r_dt->ift->dt+i))->p((const sll_object_t*const*)(s+si-1),1);
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
					ai=_get_instruction_at_offset(a_dt,ii);
					continue;
				}
				*(s+si-1)=SLL_ACQUIRE_STATIC_INT(0);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_REF:
				{
					sll_object_t* n=sll_memory_from_object(*(s+si-1));
					SLL_RELEASE(*(s+si-1));
					*(s+si-1)=n;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET:
_return:;
				sll_object_t* tmp=*(s+si-1);
				SLL_ASSERT(c_st.l);
				c_st.l--;
				ii=(c_st.dt+c_st.l)->ii;
				ai=_get_instruction_at_offset(a_dt,ii);
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
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_INT;
					tos->dt.i=ai->dt.i;
					*(s+si)=tos;
					si++;
					goto _return;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				{
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_FLOAT;
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
					sll_object_t* tos=*(s+si);
					switch (SLL_OBJECT_GET_TYPE(tos)){
						case SLL_OBJECT_TYPE_INT:
							rc=(tos->dt.i<INT32_MIN?INT32_MIN:(tos->dt.i>INT32_MAX?INT32_MAX:(sll_return_code_t)tos->dt.i));
							goto _end;
						case SLL_OBJECT_TYPE_FLOAT:
							rc=(tos->dt.f<INT32_MIN?INT32_MIN:(tos->dt.f>INT32_MAX?INT32_MAX:(sll_return_code_t)tos->dt.f));
							goto _end;
						case SLL_OBJECT_TYPE_CHAR:
							rc=tos->dt.c;
							goto _end;
						case SLL_OBJECT_TYPE_STRING:
							rc=(tos->dt.s.l>INT32_MAX?INT32_MAX:tos->dt.s.l);
							goto _end;
						case SLL_OBJECT_TYPE_ARRAY:
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
	sll_platform_free_page((void*)(ptr-a_dt->vc*sizeof(sll_object_t*)-a_dt->st.l*sizeof(sll_object_t)-CALL_STACK_SIZE*sizeof(call_stack_frame_t)),ptr_sz);
	sll_cleanup_handles(r_dt->hl,hl_l);
	sll_current_runtime_data=NULL;
	return rc;
}
