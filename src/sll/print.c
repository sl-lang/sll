#include <sll/_sll_internal.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/identifier.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdio.h>



#define PRINT_INT_SIGN(v,wf) \
	do{ \
		sll_integer_t __v=(v); \
		if (__v>=0){ \
			sll_file_write_char((wf),'+'); \
		} \
		_print_int(__v,(wf)); \
	} while(0)



static void _print_identifier(sll_identifier_index_t ii,const sll_source_file_t* sf,sll_file_t* wf){
	sll_identifier_list_length_t j=SLL_IDENTIFIER_GET_ARRAY_ID(ii);
	if (j==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
		sll_string_t* s=sf->st.dt+(sf->idt.il+SLL_IDENTIFIER_GET_ARRAY_INDEX(ii))->i;
		sll_file_write(wf,s->v,s->l);
		PRINT_STATIC_STRING("|#",wf);
		_print_int((sf->idt.il+SLL_IDENTIFIER_GET_ARRAY_INDEX(ii))->sc,wf);
		PRINT_STATIC_STRING("#|",wf);
	}
	else{
		sll_char_t* s=(sf->st.dt+(sf->idt.s[j].dt+SLL_IDENTIFIER_GET_ARRAY_INDEX(ii))->i)->v;
		sll_file_write(wf,s,j+1);
		PRINT_STATIC_STRING("|#",wf);
		_print_int((sf->idt.s[j].dt+SLL_IDENTIFIER_GET_ARRAY_INDEX(ii))->sc,wf);
		PRINT_STATIC_STRING("#|",wf);
	}
}



static void _print_line(sll_string_index_t s,const sll_source_file_t* sf,sll_file_offset_t* ln,sll_file_t* wf){
	if (s==SLL_MAX_STRING_INDEX){
		(*ln)++;
		PRINT_STATIC_STRING("|# :",wf);
		_print_int(*ln,wf);
		PRINT_STATIC_STRING(" #|",wf);
	}
	else{
		sll_string_t* fp=sf->st.dt+s;
		*ln=0;
		PRINT_STATIC_STRING("|# ",wf);
		sll_file_write(wf,fp->v,fp->l);
		PRINT_STATIC_STRING(" #||# :1 #|",wf);
	}
}



static const sll_node_t* _print_node_internal(const sll_source_file_t* sf,const sll_internal_function_table_t* i_ft,const sll_node_t* o,sll_file_t* wf,sll_file_offset_t* ln){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
		if (o->t==SLL_NODE_TYPE_DBG){
			_print_line(o->dt.s,sf,ln,wf);
		}
		o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	if (SLL_IS_OBJECT_TYPE_NOT_TYPE(o)&&o->t!=SLL_NODE_TYPE_VAR_ACCESS&&o->t!=SLL_NODE_TYPE_OPERATION_LIST&&o->t!=SLL_NODE_TYPE_DBG){
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
				sll_string_t* s=sf->st.dt+o->dt.s;
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
				for (sll_array_length_t i=0;i<al;i++){
					if (i){
						sll_file_write_char(wf,' ');
					}
					o=_print_node_internal(sf,i_ft,o,wf,ln);
				}
				sll_file_write_char(wf,']');
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_file_write_char(wf,'<');
				sll_map_length_t ml=o->dt.ml;
				o++;
				for (sll_map_length_t i=0;i<ml;i++){
					if (i){
						sll_file_write_char(wf,' ');
					}
					o=_print_node_internal(sf,i_ft,o,wf,ln);
				}
				sll_file_write_char(wf,'>');
				return o;
			}
		case SLL_NODE_TYPE_IDENTIFIER:
			_print_identifier(o->dt.id,sf,wf);
			return o+1;
		case SLL_NODE_TYPE_FIELD:
			{
				sll_string_t* s=sf->st.dt+o->dt.s;
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
				const sll_function_t* f=*(sf->ft.dt+o->dt.fn.id);
				if (f->nm!=SLL_MAX_STRING_INDEX){
					PRINT_STATIC_STRING("|#",wf);
					sll_file_write(wf,(sf->st.dt+f->nm)->v,(sf->st.dt+f->nm)->l);
					PRINT_STATIC_STRING("#|",wf);
				}
				for (sll_arg_count_t i=0;i<SLL_FUNCTION_GET_ARGUMENT_COUNT(f);i++){
					sll_file_write_char(wf,' ');
					_print_identifier(f->a[i],sf,wf);
				}
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				if (o->t==SLL_NODE_TYPE_INTERNAL_FUNC){
					if (i_ft&&o->dt.fn.id<i_ft->l){
						const sll_internal_function_t* f=*(i_ft->dt+o->dt.fn.id);
						PRINT_STATIC_STRING("... \"",wf);
						sll_file_write(wf,f->nm.v,f->nm.l);
						sll_file_write_char(wf,'\"');
					}
					else{
						PRINT_STATIC_STRING("... #",wf);
						_print_int(o->dt.fn.id,wf);
					}
				}
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					sll_file_write_char(wf,' ');
					o=_print_node_internal(sf,i_ft,o,wf,ln);
				}
				sll_file_write_char(wf,')');
				return o;
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC_LOAD:
			PRINT_STATIC_STRING("...",wf);
			break;
		case SLL_NODE_TYPE_INLINE_FUNC:
			PRINT_STATIC_STRING("***",wf);
			break;
		case SLL_NODE_TYPE_CALL:
			{
				PRINT_STATIC_STRING("<-",wf);
				sll_arg_count_t ac=o->dt.ac;
				o++;
				if (ac){
					while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
						if (o->t==SLL_NODE_TYPE_DBG){
							_print_line(o->dt.s,sf,ln,wf);
						}
						o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
					}
					sll_arg_count_t i=0;
					if (i_ft&&o->t==SLL_NODE_TYPE_INT&&o->dt.i<0){
						sll_function_index_t j=(sll_function_index_t)(~(o->dt.i));
						if (j<i_ft->l){
							const sll_internal_function_t* f=*(i_ft->dt+j);
							PRINT_STATIC_STRING(" (... \"",wf);
							sll_file_write(wf,f->nm.v,f->nm.l);
							PRINT_STATIC_STRING("\")",wf);
							i++;
							o++;
						}
					}
					for (;i<ac;i++){
						sll_file_write_char(wf,' ');
						o=_print_node_internal(sf,i_ft,o,wf,ln);
					}
				}
				sll_file_write_char(wf,')');
				return o;
			}
		case SLL_NODE_TYPE_IF:
			sll_file_write_char(wf,'?');
			break;
		case SLL_NODE_TYPE_INLINE_IF:
			PRINT_STATIC_STRING("?:",wf);
			break;
		case SLL_NODE_TYPE_SWITCH:
			PRINT_STATIC_STRING("??",wf);
			break;
		case SLL_NODE_TYPE_FOR:
			{
				PRINT_STATIC_STRING("->",wf);
				sll_arg_count_t ac=o->dt.l.ac;
				o++;
				for (sll_arg_count_t i=0;i<ac;i++){
					sll_file_write_char(wf,' ');
					o=_print_node_internal(sf,i_ft,o,wf,ln);
				}
				sll_file_write_char(wf,')');
				return o;
			}
		case SLL_NODE_TYPE_WHILE:
			{
				PRINT_STATIC_STRING(">-",wf);
				sll_arg_count_t ac=o->dt.l.ac;
				o++;
				for (sll_arg_count_t i=0;i<ac;i++){
					sll_file_write_char(wf,' ');
					o=_print_node_internal(sf,i_ft,o,wf,ln);
				}
				sll_file_write_char(wf,')');
				return o;
			}
		case SLL_NODE_TYPE_LOOP:
			{
				PRINT_STATIC_STRING("><",wf);
				sll_arg_count_t ac=o->dt.l.ac;
				o++;
				for (sll_arg_count_t i=0;i<ac;i++){
					sll_file_write_char(wf,' ');
					o=_print_node_internal(sf,i_ft,o,wf,ln);
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
					if (o->t==SLL_NODE_TYPE_DBG){
						_print_line(o->dt.s,sf,ln,wf);
					}
					o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				SLL_ASSERT(o->t==SLL_NODE_TYPE_IDENTIFIER);
				_print_identifier(o->dt.id,sf,wf);
				o++;
				l--;
				do{
					l--;
					sll_file_write_char(wf,'$');
					while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
						if (o->t==SLL_NODE_TYPE_DBG){
							_print_line(o->dt.s,sf,ln,wf);
						}
						o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
					}
					SLL_ASSERT(o->t==SLL_NODE_TYPE_FIELD);
					sll_string_t* s=sf->st.dt+o->dt.s;
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
					PRINT_STATIC_STRING("|#",wf);
					sll_file_write(wf,(sf->st.dt+o->dt.d.nm)->v,(sf->st.dt+o->dt.d.nm)->l);
					PRINT_STATIC_STRING("#|",wf);
				}
				sll_arg_count_t ac=o->dt.d.ac;
				o++;
				for (sll_arg_count_t i=0;i<ac;i++){
					sll_file_write_char(wf,' ');
					o=_print_node_internal(sf,i_ft,o,wf,ln);
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
				for (sll_arg_count_t i=0;i<ac;i++){
					if (i){
						sll_file_write_char(wf,' ');
					}
					o=_print_node_internal(sf,i_ft,o,wf,ln);
				}
				sll_file_write_char(wf,'}');
				return o;
			}
		case SLL_NODE_TYPE_COMMA:
			{
				sll_file_write_char(wf,',');
				sll_arg_count_t ac=o->dt.ac;
				o++;
				for (sll_arg_count_t i=0;i<ac;i++){
					sll_file_write_char(wf,' ');
					o=_print_node_internal(sf,i_ft,o,wf,ln);
				}
				sll_file_write_char(wf,')');
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	sll_arg_count_t ac=o->dt.ac;
	o++;
	for (sll_arg_count_t i=0;i<ac;i++){
		sll_file_write_char(wf,' ');
		o=_print_node_internal(sf,i_ft,o,wf,ln);
	}
	sll_file_write_char(wf,')');
	return o;
}



void _print_char(sll_char_t c,sll_file_t* wf){
	sll_char_t bf[4]={'\\'};
	sll_string_length_t bfl=2;
	if (c=='\''||c=='"'||c=='\\'){
		bf[1]=c;
	}
	else if (c=='\t'){
		bf[1]='t';
	}
	else if (c=='\n'){
		bf[1]='n';
	}
	else if (c=='\v'){
		bf[1]='v';
	}
	else if (c=='\f'){
		bf[1]='f';
	}
	else if (c=='\r'){
		bf[1]='r';
	}
	else if (c<32||c>126){
		bf[1]='x';
		bf[2]=(c>>4)+(c>159?87:48);
		bf[3]=(c&0xf)+((c&0xf)>9?87:48);
		bfl=4;
	}
	else{
		sll_file_write_char(wf,c);
		return;
	}
	sll_file_write(wf,bf,bfl);
}



void _print_float(sll_float_t v,sll_file_t* wf){
	char bf[128];
	int sz=snprintf(bf,128,"%.16lg",v);
	sll_file_write(wf,bf,sz*sizeof(char));
}



void _print_int(sll_integer_t v,sll_file_t* wf){
	if (!v){
		sll_file_write_char(wf,'0');
		return;
	}
	if (v<0){
		v=-v;
		sll_file_write_char(wf,'-');
	}
	sll_char_t bf[20];
	sll_string_length_t i=0;
	while (v){
		bf[i]=v%10;
		v/=10;
		i++;
	}
	while (i){
		i--;
		sll_file_write_char(wf,bf[i]+48);
	}
}



__SLL_EXTERNAL void sll_print_assembly(const sll_assembly_data_t* a_dt,sll_file_t* wf){
	sll_assembly_instruction_t* ai=a_dt->h;
	for (sll_instruction_index_t i=0;i<a_dt->ic;i++){
		if (i){
			sll_file_write_char(wf,',');
		}
		switch (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)){
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_POP:
				PRINT_STATIC_STRING("POP",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO:
				PRINT_STATIC_STRING("POP & POP",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT:
				PRINT_STATIC_STRING("ROT",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP:
				PRINT_STATIC_STRING("ROT & POP",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DUP:
				PRINT_STATIC_STRING("DUP",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
				PRINT_STATIC_STRING("PUSH ",wf);
				_print_int(ai->dt.i,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE:
				PRINT_STATIC_STRING("PUSH -1",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO:
				PRINT_STATIC_STRING("PUSH 0",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE:
				PRINT_STATIC_STRING("PUSH 1",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO:
				PRINT_STATIC_STRING("PUSH 2",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE:
				PRINT_STATIC_STRING("PUSH 3",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR:
				PRINT_STATIC_STRING("PUSH 4",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
				PRINT_STATIC_STRING("PUSH ",wf);
				_print_float(ai->dt.f,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
				PRINT_STATIC_STRING("PUSH c",wf);
				_print_int(ai->dt.c,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL:
				PRINT_STATIC_STRING("PUSH .",wf);
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,wf);
				}
				else{
					_print_int(ai->dt.j,wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD:
				PRINT_STATIC_STRING("LOAD $",wf);
				_print_int(ai->dt.v,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
				PRINT_STATIC_STRING("LOADS #",wf);
				_print_int(ai->dt.s,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK:
				PRINT_STATIC_STRING("PACK ",wf);
				_print_int(ai->dt.al,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO:
				PRINT_STATIC_STRING("PACK 0",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ONE:
				PRINT_STATIC_STRING("PACK 1",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP:
				PRINT_STATIC_STRING("MAP ",wf);
				_print_int(ai->dt.ml,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP_ZERO:
				PRINT_STATIC_STRING("MAP 0",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE:
				PRINT_STATIC_STRING("STORE $",wf);
				_print_int(ai->dt.v,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP:
				PRINT_STATIC_STRING("STORE $",wf);
				_print_int(ai->dt.v,wf);
				PRINT_STATIC_STRING(" & POP",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE:
				PRINT_STATIC_STRING("PUSH -1 & STORE $",wf);
				_print_int(ai->dt.v,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ZERO:
				PRINT_STATIC_STRING("PUSH 0 & STORE $",wf);
				_print_int(ai->dt.v,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ONE:
				PRINT_STATIC_STRING("PUSH 1 & STORE $",wf);
				_print_int(ai->dt.v,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_TWO:
				PRINT_STATIC_STRING("PUSH 2 & STORE $",wf);
				_print_int(ai->dt.v,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_THREE:
				PRINT_STATIC_STRING("PUSH 3 & STORE $",wf);
				_print_int(ai->dt.v,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_FOUR:
				PRINT_STATIC_STRING("PUSH 4 & STORE $",wf);
				_print_int(ai->dt.v,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP:
				PRINT_STATIC_STRING("LOOKUP",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR:
				PRINT_STATIC_STRING("LOOKUP #",wf);
				_print_int(ai->dt.s,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP:
				PRINT_STATIC_STRING("JMP .",wf);
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,wf);
				}
				else{
					_print_int(ai->dt.j,wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JB:
				PRINT_STATIC_STRING("JB .",wf);
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,wf);
				}
				else{
					_print_int(ai->dt.j,wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:
				PRINT_STATIC_STRING("JBE .",wf);
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,wf);
				}
				else{
					_print_int(ai->dt.j,wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JA:
				PRINT_STATIC_STRING("JA .",wf);
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,wf);
				}
				else{
					_print_int(ai->dt.j,wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:
				PRINT_STATIC_STRING("JAE .",wf);
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,wf);
				}
				else{
					_print_int(ai->dt.j,wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JE:
				PRINT_STATIC_STRING("JE .",wf);
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,wf);
				}
				else{
					_print_int(ai->dt.j,wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:
				PRINT_STATIC_STRING("JNE .",wf);
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,wf);
				}
				else{
					_print_int(ai->dt.j,wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:
				PRINT_STATIC_STRING("JZ .",wf);
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,wf);
				}
				else{
					_print_int(ai->dt.j,wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ:
				PRINT_STATIC_STRING("JNZ .",wf);
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,wf);
				}
				else{
					_print_int(ai->dt.j,wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JSE:
				PRINT_STATIC_STRING("JSE .",wf);
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,wf);
				}
				else{
					_print_int(ai->dt.j,wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JSNE:
				PRINT_STATIC_STRING("JSNE .",wf);
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,wf);
				}
				else{
					_print_int(ai->dt.j,wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JI:
				PRINT_STATIC_STRING("JI .",wf);
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,wf);
				}
				else{
					_print_int(ai->dt.j,wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNI:
				PRINT_STATIC_STRING("JNI .",wf);
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,wf);
				}
				else{
					_print_int(ai->dt.j,wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JT:
				PRINT_STATIC_STRING("JT ",wf);
				_print_int(ai->dt.i,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("NOT $",wf);
					_print_int(ai->dt.v,wf);
				}
				else{
					PRINT_STATIC_STRING("NOT",wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_BOOL:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("BOOL $",wf);
					_print_int(ai->dt.v,wf);
				}
				else{
					PRINT_STATIC_STRING("BOOL",wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_INC:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("INC $",wf);
					_print_int(ai->dt.v,wf);
				}
				else{
					PRINT_STATIC_STRING("INC",wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DEC:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("DEC $",wf);
					_print_int(ai->dt.v,wf);
				}
				else{
					PRINT_STATIC_STRING("DEC",wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("ADD $",wf);
					_print_int(ai->dt.v,wf);
				}
				else{
					PRINT_STATIC_STRING("ADD",wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_SUB:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("SUB $",wf);
					_print_int(ai->dt.v,wf);
				}
				else{
					PRINT_STATIC_STRING("SUB",wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MULT:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("MULT $",wf);
					_print_int(ai->dt.v,wf);
				}
				else{
					PRINT_STATIC_STRING("MULT",wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DIV:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("DIV $",wf);
					_print_int(ai->dt.v,wf);
				}
				else{
					PRINT_STATIC_STRING("DIV",wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_FDIV:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("FDIV $",wf);
					_print_int(ai->dt.v,wf);
				}
				else{
					PRINT_STATIC_STRING("FDIV",wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MOD:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("MOD $",wf);
					_print_int(ai->dt.v,wf);
				}
				else{
					PRINT_STATIC_STRING("MOD",wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_AND:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("AND $",wf);
					_print_int(ai->dt.v,wf);
				}
				else{
					PRINT_STATIC_STRING("AND",wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_OR:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("OR $",wf);
					_print_int(ai->dt.v,wf);
				}
				else{
					PRINT_STATIC_STRING("OR",wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_XOR:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("XOR $",wf);
					_print_int(ai->dt.v,wf);
				}
				else{
					PRINT_STATIC_STRING("XOR",wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_INV:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("INV $",wf);
					_print_int(ai->dt.v,wf);
				}
				else{
					PRINT_STATIC_STRING("INV",wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_SHR:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("SHR $",wf);
					_print_int(ai->dt.v,wf);
				}
				else{
					PRINT_STATIC_STRING("SHR",wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_SHL:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("SHL $",wf);
					_print_int(ai->dt.v,wf);
				}
				else{
					PRINT_STATIC_STRING("SHL",wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LENGTH:
				PRINT_STATIC_STRING("LENGTH",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_COPY:
				PRINT_STATIC_STRING("COPY",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS:
				PRINT_STATIC_STRING("ACCESS",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_TWO:
				PRINT_STATIC_STRING("ACCESS_RANGE",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_THREE:
				PRINT_STATIC_STRING("ACCESS_RANGE_STEP",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_VAR:
				PRINT_STATIC_STRING("PUSH ",wf);
				_print_int(ai->dt.va.l,wf);
				PRINT_STATIC_STRING(" & ACCESS_VAR $",wf);
				_print_int(ai->dt.va.v,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("ASSIGN $",wf);
					_print_int(ai->dt.v,wf);
				}
				else{
					PRINT_STATIC_STRING("ASSIGN",wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_TWO:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("ASSIGN_RANGE $",wf);
					_print_int(ai->dt.v,wf);
				}
				else{
					PRINT_STATIC_STRING("ASSIGN_RANGE",wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_THREE:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("ASSIGN_RANGE_STEP $",wf);
					_print_int(ai->dt.v,wf);
				}
				else{
					PRINT_STATIC_STRING("ASSIGN_RANGE_STEP",wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR_ACCESS:
				PRINT_STATIC_STRING("PUSH ",wf);
				_print_int(ai->dt.va.l,wf);
				PRINT_STATIC_STRING(" & ASSIGN_VAR_ACCESS $",wf);
				_print_int(ai->dt.va.v,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST:
				PRINT_STATIC_STRING("CAST",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE:
				PRINT_STATIC_STRING("CAST ",wf);
				_print_int(ai->dt.t,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_TYPEOF:
				PRINT_STATIC_STRING("TYPEOF",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL:
				PRINT_STATIC_STRING("DECL ",wf);
				_print_int(ai->dt.ac,wf);
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_ANONYMOUS(ai)){
					PRINT_STATIC_STRING(" (anonymous)",wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL_ZERO:
				PRINT_STATIC_STRING("DECL 0",wf);
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_ANONYMOUS(ai)){
					PRINT_STATIC_STRING(" (anonymous)",wf);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW:
				PRINT_STATIC_STRING("NEW ",wf);
				_print_int(ai->dt.ac,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW_DECL:
				PRINT_STATIC_STRING("DECL 0 & NEW 0",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT:
				PRINT_STATIC_STRING("PRINT",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
				PRINT_STATIC_STRING("PRINT c",wf);
				_print_int(ai->dt.c,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
				PRINT_STATIC_STRING("PRINT #",wf);
				_print_int(ai->dt.s,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR:
				PRINT_STATIC_STRING("PRINT $",wf);
				_print_int(ai->dt.v,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
				PRINT_STATIC_STRING("CALL ",wf);
				_print_int(ai->dt.ac,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP:
				PRINT_STATIC_STRING("CALL ",wf);
				_print_int(ai->dt.ac,wf);
				PRINT_STATIC_STRING(" & POP",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO:
				PRINT_STATIC_STRING("PUSH ",wf);
				_print_int(ai->dt.i,wf);
				PRINT_STATIC_STRING(" & CALL 0",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:
				PRINT_STATIC_STRING("PUSH ",wf);
				_print_int(ai->dt.i,wf);
				PRINT_STATIC_STRING(" & CALL 1",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_REF:
				PRINT_STATIC_STRING("REF",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET:
				PRINT_STATIC_STRING("RET",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				PRINT_STATIC_STRING("RET ",wf);
				_print_int(ai->dt.i,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO:
				PRINT_STATIC_STRING("RET 0",wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				PRINT_STATIC_STRING("RET ",wf);
				_print_float(ai->dt.f,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				PRINT_STATIC_STRING("RET c",wf);
				_print_int(ai->dt.c,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				PRINT_STATIC_STRING("RET #",wf);
				_print_int(ai->dt.s,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR:
				PRINT_STATIC_STRING("RET $",wf);
				_print_int(ai->dt.v,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL:
				PRINT_STATIC_STRING("DEL $",wf);
				_print_int(ai->dt.v,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL:
				PRINT_STATIC_STRING("LOAD $",wf);
				_print_int(ai->dt.v,wf);
				PRINT_STATIC_STRING(" & DEL",wf);
				break;
			default:
				SLL_UNREACHABLE();
		}
		ai++;
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			ai=ai->dt._p;
		}
	}
}



__SLL_EXTERNAL void sll_print_node(const sll_source_file_t* sf,const sll_internal_function_table_t* i_ft,const sll_node_t* o,sll_file_t* wf){
	sll_file_offset_t off=0;
	_print_node_internal(sf,i_ft,(o?o:sf->dt),wf,&off);
}
