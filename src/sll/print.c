#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/core.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdio.h>



#define PRINT_STATIC_STRING(s,os) SLL_WRITE_TO_OUTPUT_DATA_STREAM((os),(sll_buffer_t)(s),sizeof(s)-1)



void _print_int(int64_t v,sll_output_data_stream_t* os){
	if (!v){
		SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'0');
		return;
	}
	if (v<0){
		v=-v;
		SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'-');
	}
	char bf[20];
	uint8_t i=0;
	while (v){
		bf[i]=v%10;
		v/=10;
		i++;
	}
	while (i){
		i--;
		SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,bf[i]+48);
	}
}



void _print_float(double v,sll_output_data_stream_t* os){
	char bf[128];
	int sz=snprintf(bf,128,"%.16lg",v);
	SLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)bf,sz*sizeof(char));
}



sll_object_offset_t _print_object_internal(const sll_compilation_data_t* c_dt,const sll_object_t* o,sll_output_data_stream_t* os){
	sll_object_offset_t eoff=0;
	while (o->t==SLL_OBJECT_TYPE_NOP){
		eoff++;
		o++;
	}
	if (SLL_IS_OBJECT_TYPE_NOT_TYPE(o)&&o->t!=SLL_OBJECT_TYPE_OPERATION_LIST&&o->t!=SLL_OBJECT_TYPE_DEBUG_DATA){
		SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'(');
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
			PRINT_STATIC_STRING("(unknown)",os);
			return eoff+1;
		case SLL_OBJECT_TYPE_CHAR:
			{
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\'');
				char c=o->dt.c;
				if (c=='\''||c=='"'||c=='\\'){
					SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\\');
				}
				else if (c=='\t'){
					SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\\');
					c='t';
				}
				else if (c=='\n'){
					SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\\');
					c='n';
				}
				else if (c=='\v'){
					SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\\');
					c='v';
				}
				else if (c=='\f'){
					SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\\');
					c='f';
				}
				else if (c=='\r'){
					SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\\');
					c='r';
				}
				else if (c<32||c>126){
					SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\\');
					SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'x');
					SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,(((uint8_t)c)>>4)+(((uint8_t)c)>159?87:48));
					c=(c&0xf)+((c&0xf)>9?87:48);
				}
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,c);
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\'');
				return eoff+1;
			}
		case SLL_OBJECT_TYPE_INT:
			_print_int(o->dt.i,os);
			return eoff+1;
		case SLL_OBJECT_TYPE_FLOAT:
			_print_float(o->dt.f,os);
			return eoff+1;
		case SLL_OBJECT_TYPE_STRING:
			{
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'"');
				sll_string_t* s=*(c_dt->st.dt+o->dt.s);
				for (sll_string_length_t i=0;i<s->l;i++){
					char c=s->v[i];
					if (c=='\''||c=='"'||c=='\\'){
						SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\\');
					}
					else if (c=='\t'){
						SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\\');
						c='t';
					}
					else if (c=='\n'){
						SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\\');
						c='n';
					}
					else if (c=='\v'){
						SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\\');
						c='v';
					}
					else if (c=='\f'){
						SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\\');
						c='f';
					}
					else if (c=='\r'){
						SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\\');
						c='r';
					}
					else if (c<32||c>126){
						SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\\');
						SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'x');
						SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,(((uint8_t)c)>>4)+(((uint8_t)c)>159?87:48));
						c=(c&0xf)+((c&0xf)>9?87:48);
					}
					SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,c);
				}
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'"');
				return eoff+1;
			}
		case SLL_OBJECT_TYPE_ARRAY:
			{
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'<');
				sll_stack_offset_t off=1;
				sll_array_length_t al=o->dt.al;
				for (sll_array_length_t i=0;i<al;i++){
					if (i){
						SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,' ');
					}
					off+=_print_object_internal(c_dt,o+off,os);
				}
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'>');
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_IDENTIFIER:
			{
				sll_identifier_index_t i=o->dt.id;
				sll_identifier_list_length_t j=SLL_IDENTIFIER_GET_ARRAY_ID(i);
				if (j==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
					sll_string_t* s=*(c_dt->st.dt+(c_dt->idt.il+SLL_IDENTIFIER_GET_ARRAY_INDEX(i))->i);
					for (sll_string_length_t k=0;k<s->l;k++){
						SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,s->v[k]);
					}
					SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'$');
					_print_int((c_dt->idt.il+SLL_IDENTIFIER_GET_ARRAY_INDEX(i))->sc,os);
				}
				else{
					sll_char_t* s=(*(c_dt->st.dt+(c_dt->idt.s[j].dt+SLL_IDENTIFIER_GET_ARRAY_INDEX(i))->i))->v;
					for (sll_string_length_t k=0;k<j+1;k++){
						SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,*(s+k));
					}
					SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'$');
					_print_int((c_dt->idt.s[j].dt+SLL_IDENTIFIER_GET_ARRAY_INDEX(i))->sc,os);
				}
				return eoff+1;
			}
		case SLL_OBJECT_TYPE_PRINT:
			PRINT_STATIC_STRING(":>",os);
			break;
		case SLL_OBJECT_TYPE_INPUT:
			PRINT_STATIC_STRING("<:",os);
			break;
		case SLL_OBJECT_TYPE_AND:
			PRINT_STATIC_STRING("&&",os);
			break;
		case SLL_OBJECT_TYPE_OR:
			PRINT_STATIC_STRING("||",os);
			break;
		case SLL_OBJECT_TYPE_NOT:
			PRINT_STATIC_STRING("!",os);
			break;
		case SLL_OBJECT_TYPE_ASSIGN:
			PRINT_STATIC_STRING("=",os);
			break;
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				if (o->t==SLL_OBJECT_TYPE_FUNC){
					PRINT_STATIC_STRING(",,,",os);
				}
				else{
					PRINT_STATIC_STRING("... #",os);
					_print_int(o->dt.fn.id,os);
				}
				sll_stack_offset_t off=1;
				sll_arg_count_t l=o->dt.fn.ac;
				while (l){
					l--;
					SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,' ');
					off+=_print_object_internal(c_dt,o+off,os);
				}
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,')');
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_CALL:
			PRINT_STATIC_STRING("<-",os);
			break;
		case SLL_OBJECT_TYPE_IF:
			PRINT_STATIC_STRING("?",os);
			break;
		case SLL_OBJECT_TYPE_FOR:
			PRINT_STATIC_STRING("->",os);
			break;
		case SLL_OBJECT_TYPE_WHILE:
			PRINT_STATIC_STRING(">-",os);
			break;
		case SLL_OBJECT_TYPE_LOOP:
			PRINT_STATIC_STRING("><",os);
			break;
		case SLL_OBJECT_TYPE_ADD:
			PRINT_STATIC_STRING("+",os);
			break;
		case SLL_OBJECT_TYPE_SUB:
			PRINT_STATIC_STRING("-",os);
			break;
		case SLL_OBJECT_TYPE_MULT:
			PRINT_STATIC_STRING("*",os);
			break;
		case SLL_OBJECT_TYPE_DIV:
			PRINT_STATIC_STRING("/",os);
			break;
		case SLL_OBJECT_TYPE_FLOOR_DIV:
			PRINT_STATIC_STRING("//",os);
			break;
		case SLL_OBJECT_TYPE_MOD:
			PRINT_STATIC_STRING("%",os);
			break;
		case SLL_OBJECT_TYPE_BIT_AND:
			PRINT_STATIC_STRING("&",os);
			break;
		case SLL_OBJECT_TYPE_BIT_OR:
			PRINT_STATIC_STRING("|",os);
			break;
		case SLL_OBJECT_TYPE_BIT_XOR:
			PRINT_STATIC_STRING("^",os);
			break;
		case SLL_OBJECT_TYPE_BIT_NOT:
			PRINT_STATIC_STRING("~",os);
			break;
		case SLL_OBJECT_TYPE_LESS:
			PRINT_STATIC_STRING("<",os);
			break;
		case SLL_OBJECT_TYPE_LESS_EQUAL:
			PRINT_STATIC_STRING("<=",os);
			break;
		case SLL_OBJECT_TYPE_EQUAL:
			PRINT_STATIC_STRING("==",os);
			break;
		case SLL_OBJECT_TYPE_NOT_EQUAL:
			PRINT_STATIC_STRING("!=",os);
			break;
		case SLL_OBJECT_TYPE_MORE:
			PRINT_STATIC_STRING(">",os);
			break;
		case SLL_OBJECT_TYPE_MORE_EQUAL:
			PRINT_STATIC_STRING(">=",os);
			break;
		case SLL_OBJECT_TYPE_RETURN:
			PRINT_STATIC_STRING("@@",os);
			break;
		case SLL_OBJECT_TYPE_EXIT:
			PRINT_STATIC_STRING("@",os);
			break;
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'{');
				sll_stack_offset_t off=1;
				sll_statement_count_t sc=o->dt.sc;
				for (sll_statement_count_t i=0;i<sc;i++){
					if (i){
						SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,' ');
					}
					off+=_print_object_internal(c_dt,o+off,os);
				}
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'}');
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_DEBUG_DATA:
			{
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'[');
				sll_string_t* fp=*(c_dt->st.dt+o->dt.dbg.fpi);
				for (sll_string_length_t i=0;i<fp->l;i++){
					SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,fp->v[i]);
				}
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,':');
				_print_int(o->dt.dbg.ln+1,os);
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,':');
				_print_int(o->dt.dbg.cn+1,os);
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,']');
				return eoff+_print_object_internal(c_dt,o+1,os)+1;
			}
		default:
			UNREACHABLE();
	}
	sll_stack_offset_t off=1;
	sll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,' ');
		off+=_print_object_internal(c_dt,o+off,os);
	}
	SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,')');
	return off+eoff;
}



__SLL_FUNC void sll_print_assembly(const sll_assembly_data_t* a_dt,sll_output_data_stream_t* os){
	sll_assembly_instruction_t* ai=a_dt->h;
	for (sll_instruction_index_t i=0;i<a_dt->ic;i++){
		if (i){
			SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,',');
		}
		switch (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)){
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP:
				PRINT_STATIC_STRING("NOP",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_POP:
				PRINT_STATIC_STRING("POP",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO:
				PRINT_STATIC_STRING("POP & POP",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT:
				PRINT_STATIC_STRING("ROT",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP:
				PRINT_STATIC_STRING("ROT & POP",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DUP:
				PRINT_STATIC_STRING("DUP",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
				PRINT_STATIC_STRING("PUSH ",os);
				_print_int(ai->dt.i,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE:
				PRINT_STATIC_STRING("PUSH -1",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO:
				PRINT_STATIC_STRING("PUSH 0",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE:
				PRINT_STATIC_STRING("PUSH 1",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO:
				PRINT_STATIC_STRING("PUSH 2",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE:
				PRINT_STATIC_STRING("PUSH 3",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR:
				PRINT_STATIC_STRING("PUSH 4",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
				PRINT_STATIC_STRING("PUSH ",os);
				_print_float(ai->dt.f,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
				PRINT_STATIC_STRING("PUSH c",os);
				_print_int(ai->dt.c,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD:
				PRINT_STATIC_STRING("LOAD $",os);
				_print_int(ai->dt.v,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
				PRINT_STATIC_STRING("LOADS #",os);
				_print_int(ai->dt.s,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK:
				PRINT_STATIC_STRING("PACK ",os);
				_print_int(ai->dt.al,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO:
				PRINT_STATIC_STRING("PACK 0",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ONE:
				PRINT_STATIC_STRING("PACK 1",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE:
				PRINT_STATIC_STRING("STORE $",os);
				_print_int(ai->dt.v,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP:
				PRINT_STATIC_STRING("STORE $",os);
				_print_int(ai->dt.v,os);
				PRINT_STATIC_STRING(" & POP",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE:
				PRINT_STATIC_STRING("PUSH -1 & STORE $",os);
				_print_int(ai->dt.v,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ZERO:
				PRINT_STATIC_STRING("PUSH 0 & STORE $",os);
				_print_int(ai->dt.v,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ONE:
				PRINT_STATIC_STRING("PUSH 1 & STORE $",os);
				_print_int(ai->dt.v,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_TWO:
				PRINT_STATIC_STRING("PUSH 2 & STORE $",os);
				_print_int(ai->dt.v,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_THREE:
				PRINT_STATIC_STRING("PUSH 3 & STORE $",os);
				_print_int(ai->dt.v,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_FOUR:
				PRINT_STATIC_STRING("PUSH 4 & STORE $",os);
				_print_int(ai->dt.v,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP:
				PRINT_STATIC_STRING("JMP .",os);
				_print_int((SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?ai->dt.rj:ai->dt.j),os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JB:
				PRINT_STATIC_STRING("JB .",os);
				_print_int((SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?ai->dt.rj:ai->dt.j),os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:
				PRINT_STATIC_STRING("JBE .",os);
				_print_int((SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?ai->dt.rj:ai->dt.j),os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JA:
				PRINT_STATIC_STRING("JA .",os);
				_print_int((SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?ai->dt.rj:ai->dt.j),os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:
				PRINT_STATIC_STRING("JAE .",os);
				_print_int((SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?ai->dt.rj:ai->dt.j),os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JE:
				PRINT_STATIC_STRING("JE .",os);
				_print_int((SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?ai->dt.rj:ai->dt.j),os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:
				PRINT_STATIC_STRING("JNE .",os);
				_print_int((SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?ai->dt.rj:ai->dt.j),os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:
				PRINT_STATIC_STRING("JZ .",os);
				_print_int((SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?ai->dt.rj:ai->dt.j),os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ:
				PRINT_STATIC_STRING("JNZ .",os);
				_print_int((SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)?ai->dt.rj:ai->dt.j),os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT:
				if (SLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("NOT $",os);
					_print_int(ai->dt.v,os);
				}
				else{
					PRINT_STATIC_STRING("NOT",os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_INC:
				if (SLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("INC $",os);
					_print_int(ai->dt.v,os);
				}
				else{
					PRINT_STATIC_STRING("INC",os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DEC:
				if (SLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("DEC $",os);
					_print_int(ai->dt.v,os);
				}
				else{
					PRINT_STATIC_STRING("DEC",os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD:
				if (SLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("ADD $",os);
					_print_int(ai->dt.v,os);
				}
				else{
					PRINT_STATIC_STRING("ADD",os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_SUB:
				if (SLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("SUB $",os);
					_print_int(ai->dt.v,os);
				}
				else{
					PRINT_STATIC_STRING("SUB",os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MULT:
				if (SLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("MULT $",os);
					_print_int(ai->dt.v,os);
				}
				else{
					PRINT_STATIC_STRING("MULT",os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DIV:
				if (SLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("DIV $",os);
					_print_int(ai->dt.v,os);
				}
				else{
					PRINT_STATIC_STRING("DIV",os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_FDIV:
				if (SLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("FDIV $",os);
					_print_int(ai->dt.v,os);
				}
				else{
					PRINT_STATIC_STRING("FDIV",os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MOD:
				if (SLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("MOD $",os);
					_print_int(ai->dt.v,os);
				}
				else{
					PRINT_STATIC_STRING("MOD",os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_AND:
				if (SLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("AND $",os);
					_print_int(ai->dt.v,os);
				}
				else{
					PRINT_STATIC_STRING("AND",os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_OR:
				if (SLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("OR $",os);
					_print_int(ai->dt.v,os);
				}
				else{
					PRINT_STATIC_STRING("OR",os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_XOR:
				if (SLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("XOR $",os);
					_print_int(ai->dt.v,os);
				}
				else{
					PRINT_STATIC_STRING("XOR",os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_INV:
				if (SLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("INV $",os);
					_print_int(ai->dt.v,os);
				}
				else{
					PRINT_STATIC_STRING("INV",os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT:
				PRINT_STATIC_STRING("PRINT",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
				PRINT_STATIC_STRING("PRINT c",os);
				_print_int(ai->dt.c,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
				PRINT_STATIC_STRING("PRINT #",os);
				_print_int(ai->dt.s,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR:
				PRINT_STATIC_STRING("PRINT $",os);
				_print_int(ai->dt.v,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
				PRINT_STATIC_STRING("CALL ",os);
				_print_int(ai->dt.ac,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP:
				PRINT_STATIC_STRING("CALL ",os);
				_print_int(ai->dt.ac,os);
				PRINT_STATIC_STRING(" & POP",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO:
				PRINT_STATIC_STRING("PUSH ",os);
				_print_int(ai->dt.i,os);
				PRINT_STATIC_STRING(" & CALL 0",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:
				PRINT_STATIC_STRING("PUSH ",os);
				_print_int(ai->dt.i,os);
				PRINT_STATIC_STRING(" & CALL 1",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET:
				PRINT_STATIC_STRING("RET",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				PRINT_STATIC_STRING("RET ",os);
				_print_int(ai->dt.i,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO:
				PRINT_STATIC_STRING("RET 0",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				PRINT_STATIC_STRING("RET ",os);
				_print_float(ai->dt.f,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				PRINT_STATIC_STRING("RET c",os);
				_print_int(ai->dt.c,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				PRINT_STATIC_STRING("RET #",os);
				_print_int(ai->dt.s,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR:
				PRINT_STATIC_STRING("RET $",os);
				_print_int(ai->dt.v,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_END:
				PRINT_STATIC_STRING("END",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_END_ZERO:
				PRINT_STATIC_STRING("END 0",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_END_ONE:
				PRINT_STATIC_STRING("END 1",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL:
				PRINT_STATIC_STRING("LOAD $",os);
				_print_int(ai->dt.v,os);
				PRINT_STATIC_STRING(" & DEL",os);
				break;
			default:
				UNREACHABLE();
		}
		ai++;
	}
}



__SLL_FUNC void sll_print_object(const sll_compilation_data_t* c_dt,const sll_object_t* o,sll_output_data_stream_t* os){
	_print_object_internal(c_dt,o,os);
}
