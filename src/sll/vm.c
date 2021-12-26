#include <sll/_sll_internal.h>
#include <sll/api/string.h>
#include <sll/array.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/ift.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/platform.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <sll/util.h>
#include <sll/vm.h>



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



__SLL_EXTERNAL sll_instruction_index_t sll_current_instruction_count=0;
__SLL_EXTERNAL sll_instruction_index_t sll_current_instruction_index=0;
__SLL_EXTERNAL const sll_runtime_data_t* sll_current_runtime_data=NULL;
__SLL_EXTERNAL const sll_vm_config_t* sll_current_vm_config=NULL;



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_code_t sll_execute_assembly(const sll_assembly_data_t* a_dt,const sll_vm_config_t* cfg,sll_error_t* e){
	e->t=SLL_ERROR_UNKNOWN;
	sll_current_instruction_count=0;
	sll_current_vm_config=cfg;
	const sll_assembly_instruction_t* ai=a_dt->h;
	sll_size_t ptr_sz=SLL_ROUND_LARGE_PAGE(cfg->s_sz+a_dt->vc*sizeof(sll_object_t*)+a_dt->st.l*sizeof(sll_object_t)+SLL_CALL_STACK_SIZE*sizeof(sll_call_stack_frame_t));
	uint64_t ptr=(uint64_t)sll_platform_allocate_page(ptr_sz,1);
	sll_object_t** v=(sll_object_t**)ptr;
	sll_static_int[0]->rc+=a_dt->vc;
	for (sll_variable_index_t i=0;i<a_dt->vc;i++){
		*(v+i)=sll_static_int[0];
	}
	ptr+=a_dt->vc*sizeof(sll_object_t*);
	sll_object_t* cs=(sll_object_t*)ptr;
	for (sll_string_index_t i=0;i<a_dt->st.l;i++){
		(cs+i)->rc=1;
		(cs+i)->t=SLL_OBJECT_TYPE_STRING;
		(cs+i)->_dbg=NULL;
		(cs+i)->dt.s=*(a_dt->st.dt+i);
	}
	ptr+=a_dt->st.l*sizeof(sll_object_t);
	sll_call_stack_t c_st={
		(sll_call_stack_frame_t*)ptr,
		0
	};
	ptr+=SLL_CALL_STACK_SIZE*sizeof(sll_call_stack_frame_t);
	sll_internal_function_table_t ift;
	sll_create_internal_function_table(&ift);
	sll_register_builtin_internal_functions(&ift);
	sll_object_type_table_t tt=SLL_INIT_OBJECT_TYPE_TABLE_STRUCT;
	sll_runtime_data_t r_dt={
		a_dt,
		&ift,
		&tt,
		&c_st
	};
	sll_current_runtime_data=&r_dt;// lgtm [cpp/stack-address-escape]
	sll_return_code_t rc=0;
	sll_object_t** s=(sll_object_t**)ptr;
	sll_stack_offset_t si=0;
	sll_instruction_index_t ii=0;
	sll_bool_t io=!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)||sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO);
	while (1){
		if (ii>=a_dt->ic){
			e->t=SLL_ERROR_INVALID_INSTRUCTION_INDEX;
			goto _end;
		}
		sll_current_instruction_count++;
		sll_current_instruction_index=ii;
		switch (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)){
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
					if (!sll_array_create(ai->dt.al,a)){
						SLL_UNIMPLEMENTED();
					}
					si-=ai->dt.al;
					for (sll_array_length_t i=0;i<ai->dt.al;i++){
						a->v[i]=*(s+si+i);
					}
					*(s+si)=tos;
					si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO:
				{
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_ARRAY;
					if (!sll_array_create(0,&(tos->dt.a))){
						SLL_UNIMPLEMENTED();
					}
					*(s+si)=tos;
					si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ONE:
				{
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_ARRAY;
					if (!sll_array_create(1,&(tos->dt.a))){
						SLL_UNIMPLEMENTED();
					}
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
				{
					sll_object_t* tos=SLL_CREATE();
					tos->t=SLL_OBJECT_TYPE_MAP;
					sll_map_create(0,&(tos->dt.m));
					*(s+si)=tos;
					si++;
					break;
				}
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
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP:
				{
					sll_object_t* n=sll_operator_cast(*(s+si-1),sll_static_int[SLL_OBJECT_TYPE_STRING]);
					SLL_RELEASE(*(s+si-1));
					if (n->dt.s.l>SLL_INTERNAL_FUNCTION_MAX_LENGTH){
						SLL_RELEASE(n);
						*(s+si-1)=SLL_ACQUIRE_STATIC_INT(0);
						break;
					}
					sll_function_index_t i=sll_lookup_internal_function(sll_current_runtime_data->ift,n->dt.s.v);
					SLL_RELEASE(n);
					*(s+si-1)=(i==SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX?SLL_ACQUIRE_STATIC_INT(0):SLL_FROM_INT(~((sll_integer_t)i)));
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR:
			{
				si++;
				if ((a_dt->st.dt+ai->dt.s)->l>SLL_INTERNAL_FUNCTION_MAX_LENGTH){
					*(s+si-1)=SLL_ACQUIRE_STATIC_INT(0);
					break;
				}
				sll_function_index_t i=sll_lookup_internal_function(sll_current_runtime_data->ift,(a_dt->st.dt+ai->dt.s)->v);
				*(s+si-1)=(i==SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX?SLL_ACQUIRE_STATIC_INT(0):SLL_FROM_INT(~((sll_integer_t)i)));
				break;
			}
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
					sll_stack_offset_t e_si=si;
					si-=ai->dt.al<<1;
					sll_stack_offset_t i=si-1;
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
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_VAR:
				{
					si-=ai->dt.va.l-1;
					sll_object_t* d=*(v+ai->dt.va.v);
					SLL_ACQUIRE(d);
					sll_arg_count_t l=ai->dt.va.l;
					sll_object_t*const* p=s+si-1;
					do{
						SLL_ASSERT((*p)->t==SLL_OBJECT_TYPE_STRING);
						sll_object_t* nd=sll_operator_access(d,*p);
						SLL_RELEASE(d);
						SLL_RELEASE(*p);
						d=nd;
						p++;
						l--;
					} while (l);
					*(s+si-1)=d;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR:
				si-=2;
				sll_operator_assign(*(v+ai->dt.v),*(s+si),*(s+si+1));
				SLL_RELEASE(*(s+si));
				SLL_RELEASE(*(s+si+1));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_TWO_VAR:
				SLL_UNIMPLEMENTED();
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_THREE_VAR:
				SLL_UNIMPLEMENTED();
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR_ACCESS:
				{
					sll_object_t* nv=*(s+si-1);
					si-=ai->dt.va.l+1;
					sll_object_t* d=*(v+ai->dt.va.v);
					SLL_ACQUIRE(d);
					sll_arg_count_t l=ai->dt.va.l-1;
					sll_object_t*const* p=s+si;
					while (l){
						SLL_ASSERT((*p)->t==SLL_OBJECT_TYPE_STRING);
						sll_object_t* nd=sll_operator_access(d,*p);
						SLL_RELEASE(d);
						SLL_RELEASE(*p);
						d=nd;
						p++;
						l--;
					}
					SLL_ASSERT((*p)->t==SLL_OBJECT_TYPE_STRING);
					sll_operator_assign(d,*p,nv);
					SLL_RELEASE(d);
					SLL_RELEASE(*p);
					SLL_RELEASE(nv);
					break;
				}
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
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL:
				{
					si-=ai->dt.ac<<1;
					sll_object_type_t nt;
					if (SLL_ASSEMBLY_INSTRUCTION_IS_ANONYMOUS(ai)){
						nt=sll_add_type(sll_current_runtime_data->tt,(const sll_object_t*const*)(s+si),ai->dt.ac,NULL);
						for (sll_arg_count_t i=0;i<(ai->dt.ac<<1);i++){
							SLL_RELEASE(*(s+si+i));
						}
						si++;
					}
					else{
						SLL_ASSERT(SLL_OBJECT_GET_TYPE(*(s+si-1))==SLL_OBJECT_TYPE_STRING);
						nt=sll_add_type(sll_current_runtime_data->tt,(const sll_object_t*const*)(s+si),ai->dt.ac,&((*(s+si-1))->dt.s));
						SLL_RELEASE(*(s+si-1));
						for (sll_arg_count_t i=0;i<(ai->dt.ac<<1);i++){
							SLL_RELEASE(*(s+si+i));
						}
					}
					*(s+si-1)=SLL_FROM_INT(nt);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL_ZERO:
				{
					if (SLL_ASSEMBLY_INSTRUCTION_IS_ANONYMOUS(ai)){
						*(s+si)=SLL_FROM_INT(sll_add_type(sll_current_runtime_data->tt,NULL,0,NULL));
						si++;
					}
					else{
						SLL_ASSERT(SLL_OBJECT_GET_TYPE(*(s+si-1))==SLL_OBJECT_TYPE_STRING);
						sll_object_type_t nt=sll_add_type(sll_current_runtime_data->tt,NULL,0,&((*(s+si-1))->dt.s));
						SLL_RELEASE(*(s+si-1));
						*(s+si-1)=SLL_FROM_INT(nt);
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL_COPY:
				{
					if (SLL_ASSEMBLY_INSTRUCTION_IS_ANONYMOUS(ai)){
						SLL_ASSERT(SLL_OBJECT_GET_TYPE(*(s+si-1))==SLL_OBJECT_TYPE_STRING);
						sll_object_type_t t=sll_type_from_initializer(sll_current_runtime_data->tt,&(a_dt->st),*(a_dt->ot_it.dt+ai->dt.t),&((*(s+si-1))->dt.s));
						SLL_RELEASE(*(s+si-1));
						*(s+si-1)=SLL_FROM_INT(t);
						break;
					}
					*(s+si)=SLL_FROM_INT(sll_type_from_initializer(sll_current_runtime_data->tt,&(a_dt->st),*(a_dt->ot_it.dt+ai->dt.t),NULL));
					si++;
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW:
				{
					si-=ai->dt.ac;
					sll_object_t* t=sll_operator_cast(*(s+si-1),sll_static_int[SLL_OBJECT_TYPE_INT]);
					SLL_RELEASE(*(s+si-1));
					sll_object_type_t ot=(t->dt.i<0||t->dt.i>sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE?SLL_OBJECT_TYPE_INT:(sll_object_type_t)(t->dt.i));
					SLL_RELEASE(t);
					*(s+si-1)=sll_create_object_type(sll_current_runtime_data->tt,ot,s+si,ai->dt.ac);
					for (sll_arg_count_t i=0;i<ai->dt.ac;i++){
						SLL_RELEASE(*(s+si+i));
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW_DECL:
				*(s+si)=sll_create_new_object_type(sll_current_runtime_data->tt);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT:
				{
					si--;
_print_from_stack:;
					sll_object_t* tos=*(s+si);
					if (io){
						if (tos->t==SLL_OBJECT_TYPE_STRING){
							sll_file_write(cfg->out,tos->dt.s.v,tos->dt.s.l*sizeof(sll_char_t));
						}
						else{
							sll_string_t str;
							sll_api_string_convert(&tos,1,&str);
							sll_file_write(cfg->out,str.v,str.l*sizeof(sll_char_t));
							sll_free_string(&str);
						}
					}
					SLL_RELEASE(tos);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
				if (io){
					sll_file_write_char(cfg->out,ai->dt.c);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
				if (io){
					sll_file_write(cfg->out,(a_dt->st.dt+ai->dt.s)->v,(a_dt->st.dt+ai->dt.s)->l*sizeof(sll_char_t));
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
							if (j<ift.l){
								si-=ai->dt.ac;
								sll_object_t* n=(*(ift.dt+j))->p((s+si),ai->dt.ac);
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
							sll_assembly_function_t* af=a_dt->ft.dt+i-1;
							if (ai->dt.ac!=af->ac){
								if (af->ac>ai->dt.ac){
									sll_static_int[0]->rc+=af->ac-ai->dt.ac;
									for (sll_arg_count_t j=0;j<af->ac-ai->dt.ac;j++){
										*(s+si+j)=sll_static_int[0];
									}
									si+=af->ac-ai->dt.ac;
								}
								else{
									for (sll_arg_count_t j=ai->dt.ac-af->ac;j;j--){
										si--;
										SLL_RELEASE(*(s+si));
									}
								}
							}
							SLL_ASSERT(c_st.l<=SLL_CALL_STACK_SIZE);
							SLL_ASSERT(si>=af->ac);
							(c_st.dt+c_st.l)->nm=af->nm;
							(c_st.dt+c_st.l)->_ii=ii;
							(c_st.dt+c_st.l)->_s=si-af->ac;
							c_st.l++;
							ii=af->i;
							ai=_get_instruction_at_offset(a_dt,ii);
							continue;
						}
					}
					else{
						SLL_RELEASE(tos);
					}
					si-=ai->dt.ac;
					for (sll_arg_count_t i=0;i<ai->dt.ac;i++){
						SLL_RELEASE(*(s+si+i));
					}
					if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL){
						*(s+si)=SLL_ACQUIRE_STATIC_INT(0);
						si++;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO:
				if (ai->dt.i<0){
					sll_function_index_t i=(sll_function_index_t)(~ai->dt.i);
					if (i<ift.l){
						*(s+si)=(*(ift.dt+i))->p(NULL,0);
						si++;
						break;
					}
				}
				else if (ai->dt.i&&ai->dt.i<=a_dt->ft.l){
					sll_assembly_function_t* af=a_dt->ft.dt+ai->dt.i-1;
					if (ai->dt.ac!=af->ac){
						if (af->ac){
							sll_static_int[0]->rc+=af->ac;
							for (sll_arg_count_t i=0;i<af->ac;i++){
								*(s+si+i)=sll_static_int[0];
							}
							si+=af->ac;
						}
					}
					SLL_ASSERT(c_st.l<=SLL_CALL_STACK_SIZE);
					SLL_ASSERT(si>=af->ac);
					(c_st.dt+c_st.l)->nm=af->nm;
					(c_st.dt+c_st.l)->_ii=ii;
					(c_st.dt+c_st.l)->_s=si;
					c_st.l++;
					ii=af->i;
					ai=_get_instruction_at_offset(a_dt,ii);
					continue;
				}
				*(s+si)=SLL_ACQUIRE_STATIC_INT(0);
				si++;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:
				if (ai->dt.i<0){
					sll_function_index_t i=(sll_function_index_t)(~ai->dt.i);
					if (i<ift.l){
						sll_object_t* n=(*(ift.dt+i))->p(s+si-1,1);
						SLL_RELEASE(*(s+si-1));
						*(s+si-1)=n;
						break;
					}
				}
				else if (ai->dt.i&&ai->dt.i<=a_dt->ft.l){
					sll_assembly_function_t* af=a_dt->ft.dt+ai->dt.i-1;
					if (af->ac!=1){
						if (!af->ac){
							si--;
							SLL_RELEASE(*(s+si));
						}
						else{
							sll_static_int[0]->rc+=af->ac-1;
							for (sll_arg_count_t i=0;i<af->ac-1;i++){
								*(s+si+i)=sll_static_int[0];
							}
							si+=af->ac-1;
						}
					}
					SLL_ASSERT(c_st.l<=SLL_CALL_STACK_SIZE);
					(c_st.dt+c_st.l)->nm=af->nm;
					(c_st.dt+c_st.l)->_ii=ii;
					(c_st.dt+c_st.l)->_s=si-1;
					c_st.l++;
					ii=af->i;
					ai=_get_instruction_at_offset(a_dt,ii);
					continue;
				}
				SLL_RELEASE(*(s+si-1));
				*(s+si-1)=SLL_ACQUIRE_STATIC_INT(0);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_REF:
				{
					sll_integer_t a=(sll_integer_t)(sll_size_t)(void*)(*(s+si-1));
					SLL_RELEASE(*(s+si-1));
					*(s+si-1)=SLL_FROM_INT(a);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET:
_return:;
				sll_object_t* tmp=*(s+si-1);
				SLL_ASSERT(c_st.l);
				c_st.l--;
				ii=(c_st.dt+c_st.l)->_ii;
				ai=_get_instruction_at_offset(a_dt,ii);
				si=(c_st.dt+c_st.l)->_s;
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
		if (si>=cfg->s_sz){
			e->t=SLL_ERROR_INVALID_STACK_INDEX;
		}
	}
_end:
	for (sll_variable_index_t i=0;i<a_dt->vc;i++){
		SLL_RELEASE(*(v+i));
	}
	sll_platform_free_page((void*)(ptr-a_dt->vc*sizeof(sll_object_t*)-a_dt->st.l*sizeof(sll_object_t)-SLL_CALL_STACK_SIZE*sizeof(sll_call_stack_frame_t)),ptr_sz);
	sll_free_internal_function_table(&ift);
	sll_free_object_type_list(&tt);
	sll_current_runtime_data=NULL;
	return rc;
}
