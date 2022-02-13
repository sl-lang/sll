#include <sll/_sll_internal.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/identifier.h>
#include <sll/node.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/types.h>



static void _generate_identifier(sll_identifier_index_t ii,const code_generation_data_t* cg_dt,sll_file_t* wf){
	sll_identifier_list_length_t j=SLL_IDENTIFIER_GET_ARRAY_ID(ii);
	if (j==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
		sll_string_t* s=cg_dt->sf->st.dt+(cg_dt->sf->idt.il+SLL_IDENTIFIER_GET_ARRAY_INDEX(ii))->i;
		sll_file_write(wf,s->v,s->l);
	}
	else{
		sll_file_write(wf,(cg_dt->sf->st.dt+(cg_dt->sf->idt.s[j].dt+SLL_IDENTIFIER_GET_ARRAY_INDEX(ii))->i)->v,j+1);
	}
}



static const sll_node_t* _generate_code_internal(const code_generation_data_t* cg_dt,const sll_node_t* o,sll_string_length_t lvl,sll_file_t* wf){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
		if (!cg_dt->p&&o->t==SLL_NODE_TYPE_DBG){
			sll_file_write_char(wf,'\n');
		}
		o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	if (SLL_IS_OBJECT_TYPE_NOT_TYPE(o)&&o->t!=SLL_NODE_TYPE_VAR_ACCESS&&o->t!=SLL_NODE_TYPE_OPERATION_LIST){
		sll_file_write_char(wf,'(');
	}
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
			{
				sll_file_write_char(wf,'\'');
				_print_char(o->dt.c,wf);
				sll_file_write_char(wf,'\'');
				return o+1;
			}
		case SLL_NODE_TYPE_INT:
			_print_int(o->dt.i,wf);
			return o+1;
		case SLL_NODE_TYPE_FLOAT:
			_print_float(o->dt.f,wf);
			return o+1;
		case SLL_NODE_TYPE_STRING:
			{
				sll_file_write_char(wf,'"');
				sll_string_t* s=cg_dt->sf->st.dt+o->dt.s;
				for (sll_string_length_t i=0;i<s->l;i++){
					_print_char(s->v[i],wf);
				}
				sll_file_write_char(wf,'"');
				return o+1;
			}
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_file_write_char(wf,'[');
				sll_array_length_t al=o->dt.al;
				o++;
				if (!al){
					sll_file_write_char(wf,']');
					return o;
				}
				if (cg_dt->p){
					sll_file_write_char(wf,'\n');
					for (sll_arg_count_t i=0;i<al;i++){
						sll_file_write_char_count(wf,'\t',lvl+1);
						o=_generate_code_internal(cg_dt,o,lvl+1,wf);
						sll_file_write_char(wf,'\n');
					}
					sll_file_write_char_count(wf,'\t',lvl);
				}
				else{
					for (sll_arg_count_t i=0;i<al;i++){
						if (i){
							sll_file_write_char(wf,' ');
						}
						o=_generate_code_internal(cg_dt,o,lvl,wf);
					}
				}
				sll_file_write_char(wf,']');
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_file_write_char(wf,'<');
				sll_map_length_t ml=o->dt.ml;
				o++;
				if (!ml){
					sll_file_write_char(wf,'>');
					return o;
				}
				if (cg_dt->p){
					sll_file_write_char(wf,'\n');
					sll_bool_t e=ml&1;
					ml>>=1;
					for (sll_arg_count_t i=0;i<ml;i++){
						sll_file_write_char_count(wf,'\t',lvl+1);
						o=_generate_code_internal(cg_dt,o,lvl+1,wf);
						sll_file_write_char(wf,' ');
						o=_generate_code_internal(cg_dt,o,lvl+1,wf);
						sll_file_write_char(wf,'\n');
					}
					if (e){
						o=_generate_code_internal(cg_dt,o,lvl+1,wf);
					}
					sll_file_write_char_count(wf,'\t',lvl);
				}
				else{
					for (sll_arg_count_t i=0;i<ml;i++){
						if (i){
							sll_file_write_char(wf,' ');
						}
						o=_generate_code_internal(cg_dt,o,lvl,wf);
					}
				}
				sll_file_write_char(wf,'>');
				return o;
			}
		case SLL_NODE_TYPE_IDENTIFIER:
			_generate_identifier(o->dt.id,cg_dt,wf);
			return o+1;
		case SLL_NODE_TYPE_FIELD:
			{
				sll_string_t* s=cg_dt->sf->st.dt+o->dt.s;
				sll_file_write(wf,s->v,s->l);
				return o+1;
			}
		case SLL_NODE_TYPE_FUNCTION_ID:
			_print_int(o->dt.fn_id,wf);
			return o+1;
		case SLL_NODE_TYPE_PRINT:
			PRINT_STATIC_STRING(":>",wf);
			break;
		case SLL_NODE_TYPE_AND:
			PRINT_STATIC_STRING("&&",wf);
			break;
		case SLL_NODE_TYPE_OR:
			PRINT_STATIC_STRING("||",wf);
			break;
		case SLL_NODE_TYPE_NOT:
			sll_file_write_char(wf,'!');
			break;
		case SLL_NODE_TYPE_BOOL:
			PRINT_STATIC_STRING("!!",wf);
			break;
		case SLL_NODE_TYPE_ASSIGN:
			PRINT_STATIC_STRING("=",wf);
			break;
		case SLL_NODE_TYPE_FUNC:
			{
				PRINT_STATIC_STRING(",,,",wf);
				const sll_function_t* f=*(cg_dt->sf->ft.dt+o->dt.fn.id);
				if (f->nm!=SLL_MAX_STRING_INDEX){
					PRINT_STATIC_STRING(" |#",wf);
					sll_file_write(wf,(cg_dt->sf->st.dt+f->nm)->v,(cg_dt->sf->st.dt+f->nm)->l);
					PRINT_STATIC_STRING("#|",wf);
				}
				for (sll_arg_count_t i=0;i<SLL_FUNCTION_GET_ARGUMENT_COUNT(f);i++){
					sll_file_write_char(wf,' ');
					_generate_identifier(f->a[i],cg_dt,wf);
				}
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				if (cg_dt->p){
					sll_file_write_char(wf,'\n');
					while (l){
						l--;
						sll_file_write_char_count(wf,'\t',lvl+1);
						o=_generate_code_internal(cg_dt,o,lvl+1,wf);
						sll_file_write_char(wf,'\n');
					}
					sll_file_write_char_count(wf,'\t',lvl);
				}
				else{
					for (sll_arg_count_t i=0;i<l;i++){
						sll_file_write_char(wf,' ');
						o=_generate_code_internal(cg_dt,o,lvl,wf);
					}
				}
				sll_file_write_char(wf,')');
				return o;
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				if (cg_dt->i_ft&&o->dt.fn.id<cg_dt->i_ft->l){
					const sll_internal_function_t* f=*(cg_dt->i_ft->dt+o->dt.fn.id);
					PRINT_STATIC_STRING("... \"",wf);
					sll_file_write(wf,f->nm.v,f->nm.l);
					sll_file_write_char(wf,'\"');
				}
				else{
					PRINT_STATIC_STRING(",",wf);
				}
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					sll_file_write_char(wf,' ');
					o=_generate_code_internal(cg_dt,o,lvl,wf);
				}
				if (!cg_dt->i_ft||o->dt.fn.id>=cg_dt->i_ft->l){
					sll_file_write_char(wf,' ');
					_print_int(o->dt.fn.id,wf);
				}
				sll_file_write_char(wf,')');
				return o;
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC_LOAD:
			PRINT_STATIC_STRING("...",wf);
			break;
		case SLL_NODE_TYPE_INLINE_FUNC:
			PRINT_STATIC_STRING("***",wf);
		case SLL_NODE_TYPE_COMMA:
			{
				if (o->t==SLL_NODE_TYPE_COMMA){
					sll_file_write_char(wf,',');
				}
				sll_arg_count_t ac=o->dt.ac;
				o++;
				if (!ac){
					sll_file_write_char(wf,')');
					return o;
				}
				if (cg_dt->p){
					sll_file_write_char(wf,'\n');
					for (sll_arg_count_t i=0;i<ac;i++){
						sll_file_write_char_count(wf,'\t',lvl+1);
						o=_generate_code_internal(cg_dt,o,lvl+1,wf);
						sll_file_write_char(wf,'\n');
					}
					sll_file_write_char_count(wf,'\t',lvl);
				}
				else{
					for (sll_arg_count_t i=0;i<ac;i++){
						sll_file_write_char(wf,' ');
						o=_generate_code_internal(cg_dt,o,lvl,wf);
					}
				}
				sll_file_write_char(wf,')');
				return o;
			}
		case SLL_NODE_TYPE_CALL:
			{
				PRINT_STATIC_STRING("<-",wf);
				sll_arg_count_t ac=o->dt.ac;
				o++;
				if (ac){
					while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
						if (!cg_dt->p&&o->t==SLL_NODE_TYPE_DBG){
							sll_file_write_char(wf,'\n');
						}
						o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
					}
					sll_arg_count_t i=0;
					if (cg_dt->i_ft&&o->t==SLL_NODE_TYPE_INT&&o->dt.i<0){
						sll_function_index_t j=(sll_function_index_t)(~(o->dt.i));
						if (j<cg_dt->i_ft->l){
							const sll_internal_function_t* f=*(cg_dt->i_ft->dt+j);
							PRINT_STATIC_STRING(" (... \"",wf);
							sll_file_write(wf,f->nm.v,f->nm.l);
							PRINT_STATIC_STRING("\")",wf);
							i++;
							o++;
						}
					}
					for (;i<ac;i++){
						sll_file_write_char(wf,' ');
						o=_generate_code_internal(cg_dt,o,lvl,wf);
					}
				}
				sll_file_write_char(wf,')');
				return o;
			}
		case SLL_NODE_TYPE_IF:
			{
				sll_file_write_char(wf,'?');
				sll_arg_count_t ac=o->dt.ac;
				o++;
				if (!ac){
					sll_file_write_char(wf,')');
					return o;
				}
				if (ac==1){
					sll_file_write_char(wf,' ');
					o=_generate_code_internal(cg_dt,o,lvl,wf);
					sll_file_write_char(wf,')');
					return o;
				}
				if (!cg_dt->p){
					for (sll_arg_count_t i=0;i<ac;i++){
						sll_file_write_char(wf,' ');
						o=_generate_code_internal(cg_dt,o,lvl,wf);
					}
					return o;
				}
				if (ac==2){
					sll_file_write_char(wf,' ');
					o=_generate_code_internal(cg_dt,o,lvl,wf);
					while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
						if (!cg_dt->p&&o->t==SLL_NODE_TYPE_DBG){
							sll_file_write_char(wf,'\n');
						}
						o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
					}
					if (o->t==SLL_NODE_TYPE_OPERATION_LIST){
						sll_file_write_char(wf,' ');
						o=_generate_code_internal(cg_dt,o,lvl,wf);
					}
					else{
						sll_file_write_char(wf,'\n');
						sll_file_write_char_count(wf,'\t',lvl+1);
						o=_generate_code_internal(cg_dt,o,lvl+1,wf);
						sll_file_write_char(wf,'\n');
						sll_file_write_char_count(wf,'\t',lvl);
					}
					sll_file_write_char(wf,')');
					return o;
				}
				if (ac==3){
					sll_file_write_char(wf,' ');
					o=_generate_code_internal(cg_dt,o,lvl,wf);
					while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
						if (!cg_dt->p&&o->t==SLL_NODE_TYPE_DBG){
							sll_file_write_char(wf,'\n');
						}
						o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
					}
					if (o->t==SLL_NODE_TYPE_OPERATION_LIST){
						sll_file_write_char(wf,' ');
						o=_generate_code_internal(cg_dt,o,lvl,wf);
						sll_file_write_char(wf,' ');
						o=_generate_code_internal(cg_dt,o,lvl,wf);
					}
					else{
						sll_file_write_char(wf,'\n');
						sll_file_write_char_count(wf,'\t',lvl+1);
						o=_generate_code_internal(cg_dt,o,lvl+1,wf);
						sll_file_write_char(wf,'\n');
						sll_file_write_char_count(wf,'\t',lvl+1);
						o=_generate_code_internal(cg_dt,o,lvl+1,wf);
						sll_file_write_char(wf,'\n');
						sll_file_write_char_count(wf,'\t',lvl);
					}
					sll_file_write_char(wf,')');
					return o;
				}
					sll_bool_t e=ac&1;
				ac>>=1;
				sll_file_write_char(wf,'\n');
				while (ac){
					ac--;
					sll_file_write_char_count(wf,'\t',lvl+1);
					o=_generate_code_internal(cg_dt,o,lvl+1,wf);
					sll_file_write_char(wf,' ');
					o=_generate_code_internal(cg_dt,o,lvl+1,wf);
					sll_file_write_char(wf,'\n');
				}
				if (e){
					sll_file_write_char_count(wf,'\t',lvl+1);
					o=_generate_code_internal(cg_dt,o,lvl+1,wf);
					sll_file_write_char(wf,'\n');
				}
				sll_file_write_char_count(wf,'\t',lvl);
				sll_file_write_char(wf,')');
				return o;
			}
		case SLL_NODE_TYPE_INLINE_IF:
			PRINT_STATIC_STRING("?:",wf);
			break;
		case SLL_NODE_TYPE_SWITCH:
			{
				PRINT_STATIC_STRING("??",wf);
				sll_arg_count_t ac=o->dt.ac;
				o++;
				if (!ac){
					sll_file_write_char(wf,')');
					return o;
				}
				ac--;
				sll_file_write_char(wf,' ');
				o=_generate_code_internal(cg_dt,o,lvl,wf);
				if (!ac){
					sll_file_write_char(wf,')');
					return o;
				}
				if (cg_dt->p){
					sll_bool_t d=ac&1;
					ac>>=1;
					sll_file_write_char(wf,'\n');
					while (ac){
						ac--;
						sll_file_write_char_count(wf,'\t',lvl+1);
						o=_generate_code_internal(cg_dt,o,lvl+1,wf);
						sll_file_write_char(wf,' ');
						o=_generate_code_internal(cg_dt,o,lvl+1,wf);
						sll_file_write_char(wf,'\n');
					}
					if (d){
						sll_file_write_char_count(wf,'\t',lvl+1);
						o=_generate_code_internal(cg_dt,o,lvl+1,wf);
						sll_file_write_char(wf,'\n');
					}
					sll_file_write_char_count(wf,'\t',lvl);
					sll_file_write_char(wf,')');
				}
				else{
					for (sll_arg_count_t i=0;i<ac;i++){
						sll_file_write_char(wf,' ');
						o=_generate_code_internal(cg_dt,o,lvl,wf);
					}
				}
				return o;
			}
		case SLL_NODE_TYPE_FOR:
			PRINT_STATIC_STRING("->",wf);
		case SLL_NODE_TYPE_WHILE:
			{
				if (o->t==SLL_NODE_TYPE_WHILE){
					PRINT_STATIC_STRING(">-",wf);
				}
				sll_arg_count_t ac=o->dt.l.ac;
				o++;
				if (ac<3){
					for (sll_arg_count_t i=0;i<ac;i++){
						sll_file_write_char(wf,' ');
						o=_generate_code_internal(cg_dt,o,lvl,wf);
					}
					sll_file_write_char(wf,')');
					return o;
				}
				for (sll_arg_count_t i=0;i<2;i++){
					sll_file_write_char(wf,' ');
					o=_generate_code_internal(cg_dt,o,lvl,wf);
				}
				if (cg_dt->p){
					while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
						if (!cg_dt->p&&o->t==SLL_NODE_TYPE_DBG){
							sll_file_write_char(wf,'\n');
						}
						o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
					}
					sll_bool_t op_l=0;
					if (ac==3&&o->t==SLL_NODE_TYPE_OPERATION_LIST){
						op_l=1;
						ac=o->dt.ac;
						o++;
						PRINT_STATIC_STRING(" {",wf);
					}
					else{
						ac-=2;
					}
					sll_file_write_char(wf,'\n');
					for (sll_arg_count_t i=0;i<ac;i++){
						sll_file_write_char_count(wf,'\t',lvl+1);
						o=_generate_code_internal(cg_dt,o,lvl+1,wf);
						sll_file_write_char(wf,'\n');
					}
					sll_file_write_char_count(wf,'\t',lvl);
					if (op_l){
						sll_file_write_char(wf,'}');
					}
				}
				else{
					for (sll_arg_count_t i=2;i<ac;i++){
						sll_file_write_char(wf,' ');
						o=_generate_code_internal(cg_dt,o,lvl,wf);
					}
				}
				sll_file_write_char(wf,')');
				return o;
			}
		case SLL_NODE_TYPE_LOOP:
			{
				PRINT_STATIC_STRING("><",wf);
				sll_arg_count_t ac=o->dt.l.ac;
				o++;
				if (!ac){
					sll_file_write_char(wf,')');
					return o;
				}
				for (sll_arg_count_t i=0;i<ac;i++){
					sll_file_write_char(wf,' ');
					o=_generate_code_internal(cg_dt,o,lvl,wf);
				}
				sll_file_write_char(wf,')');
				return o;
			}
		case SLL_NODE_TYPE_INC:
			PRINT_STATIC_STRING("++",wf);
			break;
		case SLL_NODE_TYPE_DEC:
			PRINT_STATIC_STRING("--",wf);
			break;
		case SLL_NODE_TYPE_ADD:
			sll_file_write_char(wf,'+');
			break;
		case SLL_NODE_TYPE_SUB:
			sll_file_write_char(wf,'-');
			break;
		case SLL_NODE_TYPE_MULT:
			sll_file_write_char(wf,'*');
			break;
		case SLL_NODE_TYPE_DIV:
			sll_file_write_char(wf,'/');
			break;
		case SLL_NODE_TYPE_FLOOR_DIV:
			PRINT_STATIC_STRING("//",wf);
			break;
		case SLL_NODE_TYPE_MOD:
			sll_file_write_char(wf,'%');
			break;
		case SLL_NODE_TYPE_BIT_AND:
			sll_file_write_char(wf,'&');
			break;
		case SLL_NODE_TYPE_BIT_OR:
			sll_file_write_char(wf,'|');
			break;
		case SLL_NODE_TYPE_BIT_XOR:
			sll_file_write_char(wf,'^');
			break;
		case SLL_NODE_TYPE_BIT_NOT:
			sll_file_write_char(wf,'~');
			break;
		case SLL_NODE_TYPE_BIT_RSHIFT:
			PRINT_STATIC_STRING(">>",wf);
			break;
		case SLL_NODE_TYPE_BIT_LSHIFT:
			PRINT_STATIC_STRING("<<",wf);
			break;
		case SLL_NODE_TYPE_LESS:
			sll_file_write_char(wf,'<');
			break;
		case SLL_NODE_TYPE_LESS_EQUAL:
			PRINT_STATIC_STRING("<=",wf);
			break;
		case SLL_NODE_TYPE_EQUAL:
			PRINT_STATIC_STRING("==",wf);
			break;
		case SLL_NODE_TYPE_NOT_EQUAL:
			PRINT_STATIC_STRING("!=",wf);
			break;
		case SLL_NODE_TYPE_MORE:
			sll_file_write_char(wf,'>');
			break;
		case SLL_NODE_TYPE_MORE_EQUAL:
			PRINT_STATIC_STRING(">=",wf);
			break;
		case SLL_NODE_TYPE_STRICT_EQUAL:
			PRINT_STATIC_STRING("===",wf);
			break;
		case SLL_NODE_TYPE_STRICT_NOT_EQUAL:
			PRINT_STATIC_STRING("!==",wf);
			break;
		case SLL_NODE_TYPE_LENGTH:
			sll_file_write_char(wf,'$');
			break;
		case SLL_NODE_TYPE_ACCESS:
			sll_file_write_char(wf,':');
			break;
		case SLL_NODE_TYPE_VAR_ACCESS:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l>1);
				o++;
				while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
					if (!cg_dt->p&&o->t==SLL_NODE_TYPE_DBG){
						sll_file_write_char(wf,'\n');
					}
					o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				SLL_ASSERT(o->t==SLL_NODE_TYPE_IDENTIFIER);
				_generate_identifier(o->dt.id,cg_dt,wf);
				o++;
				l--;
				do{
					l--;
					sll_file_write_char(wf,'$');
					while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
						if (!cg_dt->p&&o->t==SLL_NODE_TYPE_DBG){
							sll_file_write_char(wf,'\n');
						}
						o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
					}
					SLL_ASSERT(o->t==SLL_NODE_TYPE_FIELD);
					sll_string_t* s=cg_dt->sf->st.dt+o->dt.s;
					sll_file_write(wf,s->v,s->l);
					o++;
				} while (l);
				return o;
			}
		case SLL_NODE_TYPE_HAS:
			PRINT_STATIC_STRING("|:",wf);
			break;
		case SLL_NODE_TYPE_CAST:
			PRINT_STATIC_STRING("::",wf);
			break;
		case SLL_NODE_TYPE_TYPEOF:
			PRINT_STATIC_STRING(":?",wf);
			break;
		case SLL_NODE_TYPE_DECL:
			{
				PRINT_STATIC_STRING("&:",wf);
				if (o->dt.d.nm!=SLL_MAX_STRING_INDEX){
					PRINT_STATIC_STRING(" |#",wf);
					sll_file_write(wf,(cg_dt->sf->st.dt+o->dt.d.nm)->v,(cg_dt->sf->st.dt+o->dt.d.nm)->l);
					PRINT_STATIC_STRING("#|",wf);
				}
				sll_arg_count_t l=o->dt.d.ac;
				o++;
				if (!l){
					sll_file_write_char(wf,')');
					return o;
				}
				if (cg_dt->p){
					sll_file_write_char(wf,'\n');
					sll_bool_t e=l&1;
					l>>=1;
					while (l){
						l--;
						sll_file_write_char_count(wf,'\t',lvl+1);
						o=_generate_code_internal(cg_dt,o,lvl+1,wf);
						sll_file_write_char(wf,' ');
						o=_generate_code_internal(cg_dt,o,lvl+1,wf);
						sll_file_write_char(wf,'\n');
					}
					if (e){
						sll_file_write_char_count(wf,'\t',lvl+1);
						o=_generate_code_internal(cg_dt,o,lvl+1,wf);
						sll_file_write_char(wf,'\n');
					}
					sll_file_write_char_count(wf,'\t',lvl);
				}
				else{
					for (sll_arg_count_t i=0;i<l;i++){
						sll_file_write_char(wf,' ');
						o=_generate_code_internal(cg_dt,o,lvl,wf);
					}
				}
				sll_file_write_char(wf,')');
				return o;
			}
		case SLL_NODE_TYPE_NEW:
			sll_file_write_char(wf,'.');
			break;
		case SLL_NODE_TYPE_FOR_ARRAY:
			PRINT_STATIC_STRING("[>",wf);
			break;
		case SLL_NODE_TYPE_WHILE_ARRAY:
			PRINT_STATIC_STRING("[<",wf);
			break;
		case SLL_NODE_TYPE_FOR_MAP:
			PRINT_STATIC_STRING("{>",wf);
			break;
		case SLL_NODE_TYPE_WHILE_MAP:
			PRINT_STATIC_STRING("{<",wf);
			break;
		case SLL_NODE_TYPE_BREAK:
			sll_file_write_char(wf,'@');
			break;
		case SLL_NODE_TYPE_CONTINUE:
			PRINT_STATIC_STRING("<<<",wf);
			break;
		case SLL_NODE_TYPE_REF:
			PRINT_STATIC_STRING("%%",wf);
			break;
		case SLL_NODE_TYPE_RETURN:
			PRINT_STATIC_STRING("@@",wf);
			break;
		case SLL_NODE_TYPE_OPERATION_LIST:
			{
				sll_file_write_char(wf,'{');
				sll_arg_count_t ac=o->dt.ac;
				o++;
				if (!ac){
					sll_file_write_char(wf,'}');
					return o;
				}
				if (cg_dt->p){
					sll_file_write_char(wf,'\n');
					for (sll_arg_count_t i=0;i<ac;i++){
						sll_file_write_char_count(wf,'\t',lvl+1);
						o=_generate_code_internal(cg_dt,o,lvl+1,wf);
						sll_file_write_char(wf,'\n');
					}
					sll_file_write_char_count(wf,'\t',lvl);
				}
				else{
					for (sll_arg_count_t i=0;i<ac;i++){
						if (i){
							sll_file_write_char(wf,' ');
						}
						o=_generate_code_internal(cg_dt,o,lvl,wf);
					}
				}
				sll_file_write_char(wf,'}');
				return o;
			}
		case SLL_NODE_TYPE_THREAD_WAIT:
			PRINT_STATIC_STRING("!<<",wf);
			break;
		case SLL_NODE_TYPE_THREAD_ID:
			PRINT_STATIC_STRING("!.",wf);
			break;
		case SLL_NODE_TYPE_THREAD_LOCK:
			PRINT_STATIC_STRING("!<*",wf);
			break;
		default:
			SLL_UNREACHABLE();
	}
	sll_arg_count_t ac=o->dt.ac;
	o++;
	for (sll_arg_count_t i=0;i<ac;i++){
		sll_file_write_char(wf,' ');
		o=_generate_code_internal(cg_dt,o,lvl,wf);
	}
	sll_file_write_char(wf,')');
	return o;
}



__SLL_EXTERNAL void sll_write_sll_code(const sll_source_file_t* sf,const sll_internal_function_table_t* i_ft,sll_bool_t p,sll_file_t* wf){
	code_generation_data_t cg_dt={
		sf,
		i_ft,
		p
	};
	if (p&&sf->dt->t==SLL_NODE_TYPE_OPERATION_LIST){
		sll_arg_count_t ac=sf->dt->dt.ac;
		const sll_node_t* o=sf->dt+1;
		while (ac){
			o=_generate_code_internal(&cg_dt,o,0,wf);
			sll_file_write_char(wf,'\n');
			ac--;
		}
	}
	else{
		_generate_code_internal(&cg_dt,sf->dt,0,wf);
		if (p){
			sll_file_write_char(wf,'\n');
		}
	}
}
