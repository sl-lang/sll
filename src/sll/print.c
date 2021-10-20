#include <sll/_sll_internal.h>
#include <sll/assembly.h>
#include <sll/cast.h>
#include <sll/common.h>
#include <sll/object.h>
#include <sll/stream.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdio.h>



#define PRINT_STATIC_STRING(s,os) SLL_WRITE_TO_OUTPUT_DATA_STREAM((os),SLL_CHAR((s)),sizeof(s)/sizeof(char)-1)
#define PRINT_INT_SIGN(v,os) \
	do{ \
		int64_t __v=(v); \
		if (__v>=0){ \
			SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM((os),'+'); \
		} \
		_print_int(__v,(os)); \
	} while(0)



static void _print_int(int64_t v,sll_output_data_stream_t* os){
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



static void _print_float(double v,sll_output_data_stream_t* os){
	char bf[128];
	int sz=snprintf(bf,128,"%.16lg",v);
	SLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)bf,sz*sizeof(char));
}



static void _print_identifier(sll_identifier_index_t ii,const sll_compilation_data_t* c_dt,sll_output_data_stream_t* os){
	sll_identifier_list_length_t j=SLL_IDENTIFIER_GET_ARRAY_ID(ii);
	if (j==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
		sll_string_t* s=c_dt->st.dt+(c_dt->idt.il+SLL_IDENTIFIER_GET_ARRAY_INDEX(ii))->i;
		SLL_WRITE_TO_OUTPUT_DATA_STREAM(os,s->v,s->l);
		PRINT_STATIC_STRING("|#",os);
		_print_int((c_dt->idt.il+SLL_IDENTIFIER_GET_ARRAY_INDEX(ii))->sc,os);
		PRINT_STATIC_STRING("#|",os);
	}
	else{
		sll_char_t* s=(c_dt->st.dt+(c_dt->idt.s[j].dt+SLL_IDENTIFIER_GET_ARRAY_INDEX(ii))->i)->v;
		SLL_WRITE_TO_OUTPUT_DATA_STREAM(os,s,j+1);
		PRINT_STATIC_STRING("|#",os);
		_print_int((c_dt->idt.s[j].dt+SLL_IDENTIFIER_GET_ARRAY_INDEX(ii))->sc,os);
		PRINT_STATIC_STRING("#|",os);
	}
}



static const sll_object_t* _print_object_internal(const sll_compilation_data_t* c_dt,const sll_internal_function_table_t* i_ft,const sll_object_t* o,sll_output_data_stream_t* os){
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
		o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	if (SLL_IS_OBJECT_TYPE_NOT_TYPE(o)&&o->t!=SLL_OBJECT_TYPE_OPERATION_LIST&&o->t!=SLL_OBJECT_TYPE_DEBUG_DATA){
		SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'(');
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
			PRINT_STATIC_STRING("(unknown)",os);
			return o+1;
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
				return o+1;
			}
		case SLL_OBJECT_TYPE_INT:
			_print_int(o->dt.i,os);
			return o+1;
		case SLL_OBJECT_TYPE_FLOAT:
			_print_float(o->dt.f,os);
			return o+1;
		case SLL_OBJECT_TYPE_STRING:
			{
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'"');
				sll_string_t* s=c_dt->st.dt+o->dt.s;
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
				return o+1;
			}
		case SLL_OBJECT_TYPE_ARRAY:
			{
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'[');
				sll_array_length_t al=o->dt.al;
				o++;
				for (sll_array_length_t i=0;i<al;i++){
					if (i){
						SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,' ');
					}
					o=_print_object_internal(c_dt,i_ft,o,os);
				}
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,']');
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'<');
				sll_map_length_t ml=o->dt.ml;
				o++;
				for (sll_map_length_t i=0;i<ml;i++){
					if (i){
						SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,' ');
					}
					o=_print_object_internal(c_dt,i_ft,o,os);
				}
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'>');
				return o;
			}
		case SLL_OBJECT_TYPE_IDENTIFIER:
			_print_identifier(o->dt.id,c_dt,os);
			return o+1;
		case SLL_OBJECT_TYPE_FUNCTION_ID:
			_print_int(o->dt.fn_id,os);
			return o+1;
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
			{
				PRINT_STATIC_STRING(",,,",os);
				const sll_function_t* f=*(c_dt->ft.dt+o->dt.fn.id);
				for (sll_arg_count_t i=0;i<f->al;i++){
					SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,' ');
					_print_identifier(f->a[i],c_dt,os);
				}
			}
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				if (o->t==SLL_OBJECT_TYPE_INTERNAL_FUNC){
					if (i_ft&&o->dt.fn.id<i_ft->l){
						const sll_internal_function_t* f=*(i_ft->dt+o->dt.fn.id);
						PRINT_STATIC_STRING("... \"",os);
						SLL_WRITE_TO_OUTPUT_DATA_STREAM(os,f->nm,f->nml);
						SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\"');
					}
					else{
						PRINT_STATIC_STRING("... #",os);
						_print_int(o->dt.fn.id,os);
					}
				}
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,' ');
					o=_print_object_internal(c_dt,i_ft,o,os);
				}
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,')');
				return o;
			}
		case SLL_OBJECT_TYPE_INLINE_FUNC:
			PRINT_STATIC_STRING("***",os);
			break;
		case SLL_OBJECT_TYPE_CALL:
			{
				PRINT_STATIC_STRING("<-",os);
				sll_arg_count_t ac=o->dt.ac;
				o++;
				if (ac){
					while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
						o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
					}
					sll_arg_count_t i=0;
					if (i_ft&&o->t==SLL_OBJECT_TYPE_INT&&o->dt.i<0){
						sll_function_index_t j=(sll_function_index_t)(~(o->dt.i));
						if (j<i_ft->l){
							const sll_internal_function_t* f=*(i_ft->dt+j);
							PRINT_STATIC_STRING(" (... \"",os);
							SLL_WRITE_TO_OUTPUT_DATA_STREAM(os,f->nm,f->nml);
							PRINT_STATIC_STRING("\")",os);
							i++;
							o++;
						}
					}
					for (;i<ac;i++){
						SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,' ');
						o=_print_object_internal(c_dt,i_ft,o,os);
					}
				}
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,')');
				return o;
			}
		case SLL_OBJECT_TYPE_IF:
			SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'?');
			break;
		case SLL_OBJECT_TYPE_INLINE_IF:
			PRINT_STATIC_STRING("?:",os);
			break;
		case SLL_OBJECT_TYPE_SWITCH:
			PRINT_STATIC_STRING("??",os);
			break;
		case SLL_OBJECT_TYPE_FOR:
			{
				PRINT_STATIC_STRING("->",os);
				sll_arg_count_t ac=o->dt.l.ac;
				o++;
				for (sll_arg_count_t i=0;i<ac;i++){
					SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,' ');
					o=_print_object_internal(c_dt,i_ft,o,os);
				}
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,')');
				return o;
			}
		case SLL_OBJECT_TYPE_WHILE:
			{
				PRINT_STATIC_STRING(">-",os);
				sll_arg_count_t ac=o->dt.l.ac;
				o++;
				for (sll_arg_count_t i=0;i<ac;i++){
					SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,' ');
					o=_print_object_internal(c_dt,i_ft,o,os);
				}
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,')');
				return o;
			}
		case SLL_OBJECT_TYPE_LOOP:
			{
				PRINT_STATIC_STRING("><",os);
				sll_arg_count_t ac=o->dt.l.ac;
				o++;
				for (sll_arg_count_t i=0;i<ac;i++){
					SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,' ');
					o=_print_object_internal(c_dt,i_ft,o,os);
				}
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,')');
				return o;
			}
		case SLL_OBJECT_TYPE_ADD:
			SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'+');
			break;
		case SLL_OBJECT_TYPE_SUB:
			SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'-');
			break;
		case SLL_OBJECT_TYPE_MULT:
			SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'*');
			break;
		case SLL_OBJECT_TYPE_DIV:
			SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'/');
			break;
		case SLL_OBJECT_TYPE_FLOOR_DIV:
			PRINT_STATIC_STRING("//",os);
			break;
		case SLL_OBJECT_TYPE_MOD:
			SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'%');
			break;
		case SLL_OBJECT_TYPE_BIT_AND:
			SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'&');
			break;
		case SLL_OBJECT_TYPE_BIT_OR:
			SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'|');
			break;
		case SLL_OBJECT_TYPE_BIT_XOR:
			SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'^');
			break;
		case SLL_OBJECT_TYPE_BIT_NOT:
			SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'~');
			break;
		case SLL_OBJECT_TYPE_BIT_RSHIFT:
			PRINT_STATIC_STRING(">>",os);
			break;
		case SLL_OBJECT_TYPE_BIT_LSHIFT:
			PRINT_STATIC_STRING("<<",os);
			break;
		case SLL_OBJECT_TYPE_LESS:
			SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'<');
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
			SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'>');
			break;
		case SLL_OBJECT_TYPE_MORE_EQUAL:
			PRINT_STATIC_STRING(">=",os);
			break;
		case SLL_OBJECT_TYPE_LENGTH:
			SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'$');
			break;
		case SLL_OBJECT_TYPE_ACCESS:
			SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,':');
			break;
		case SLL_OBJECT_TYPE_CAST:
			PRINT_STATIC_STRING("::",os);
			break;
		case SLL_OBJECT_TYPE_BREAK:
			SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'@');
			break;
		case SLL_OBJECT_TYPE_CONTINUE:
			PRINT_STATIC_STRING("<<<",os);
			break;
		case SLL_OBJECT_TYPE_REF:
			PRINT_STATIC_STRING("%%",os);
			break;
		case SLL_OBJECT_TYPE_RETURN:
			PRINT_STATIC_STRING("@@",os);
			break;
		case SLL_OBJECT_TYPE_EXIT:
			PRINT_STATIC_STRING("@@@",os);
			break;
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'{');
				sll_arg_count_t ac=o->dt.ac;
				o++;
				for (sll_arg_count_t i=0;i<ac;i++){
					if (i){
						SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,' ');
					}
					o=_print_object_internal(c_dt,i_ft,o,os);
				}
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'}');
				return o;
			}
		case SLL_OBJECT_TYPE_COMMA:
			{
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,',');
				sll_arg_count_t ac=o->dt.ac;
				o++;
				for (sll_arg_count_t i=0;i<ac;i++){
					SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,' ');
					o=_print_object_internal(c_dt,i_ft,o,os);
				}
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,')');
				return o;
			}
		case SLL_OBJECT_TYPE_DEBUG_DATA:
			{
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'|');
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'#');
				sll_string_t* fp=c_dt->st.dt+o->dt.dbg.fpi;
				for (sll_string_length_t i=0;i<fp->l;i++){
					SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,fp->v[i]);
				}
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,':');
				_print_int(o->dt.dbg.ln+1,os);
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,':');
				_print_int(o->dt.dbg.cn+1,os);
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'#');
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'|');
				return _print_object_internal(c_dt,i_ft,o+1,os);
			}
		default:
			SLL_UNREACHABLE();
	}
	sll_arg_count_t ac=o->dt.ac;
	o++;
	for (sll_arg_count_t i=0;i<ac;i++){
		SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,' ');
		o=_print_object_internal(c_dt,i_ft,o,os);
	}
	SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,')');
	return o;
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
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_NULL:
				PRINT_STATIC_STRING("PUSH null_ref",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL:
				PRINT_STATIC_STRING("PUSH .",os);
				if (SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,os);
				}
				else{
					_print_int(ai->dt.j,os);
				}
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
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP:
				PRINT_STATIC_STRING("MAP ",os);
				_print_int(ai->dt.ml,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP_ZERO:
				PRINT_STATIC_STRING("MAP 0",os);
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
				if (SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,os);
				}
				else{
					_print_int(ai->dt.j,os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JB:
				PRINT_STATIC_STRING("JB .",os);
				if (SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,os);
				}
				else{
					_print_int(ai->dt.j,os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:
				PRINT_STATIC_STRING("JBE .",os);
				if (SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,os);
				}
				else{
					_print_int(ai->dt.j,os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JA:
				PRINT_STATIC_STRING("JA .",os);
				if (SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,os);
				}
				else{
					_print_int(ai->dt.j,os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:
				PRINT_STATIC_STRING("JAE .",os);
				if (SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,os);
				}
				else{
					_print_int(ai->dt.j,os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JE:
				PRINT_STATIC_STRING("JE .",os);
				if (SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,os);
				}
				else{
					_print_int(ai->dt.j,os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:
				PRINT_STATIC_STRING("JNE .",os);
				if (SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,os);
				}
				else{
					_print_int(ai->dt.j,os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:
				PRINT_STATIC_STRING("JZ .",os);
				if (SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,os);
				}
				else{
					_print_int(ai->dt.j,os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ:
				PRINT_STATIC_STRING("JNZ .",os);
				if (SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->dt.rj,os);
				}
				else{
					_print_int(ai->dt.j,os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JT:
				PRINT_STATIC_STRING("JT ",os);
				_print_int(ai->dt.i,os);
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
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_SHR:
				if (SLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("SHR $",os);
					_print_int(ai->dt.v,os);
				}
				else{
					PRINT_STATIC_STRING("SHR",os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_SHL:
				if (SLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					PRINT_STATIC_STRING("SHL $",os);
					_print_int(ai->dt.v,os);
				}
				else{
					PRINT_STATIC_STRING("SHL",os);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LENGTH:
				PRINT_STATIC_STRING("LENGTH",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_COPY:
				PRINT_STATIC_STRING("COPY",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS:
				PRINT_STATIC_STRING("ACCESS",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_TWO:
				PRINT_STATIC_STRING("ACCESS_RANGE",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_THREE:
				PRINT_STATIC_STRING("ACCESS_RANGE_STEP",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST:
				PRINT_STATIC_STRING("CAST",os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE:
				PRINT_STATIC_STRING("CAST ",os);
				switch (ai->dt.t){
					case SLL_CONSTANT_TYPE_INT:
						PRINT_STATIC_STRING("int_type",os);
						break;
					case SLL_CONSTANT_TYPE_FLOAT:
						PRINT_STATIC_STRING("float_type",os);
						break;
					case SLL_CONSTANT_TYPE_CHAR:
						PRINT_STATIC_STRING("char_type",os);
						break;
					case SLL_CONSTANT_TYPE_STRING:
						PRINT_STATIC_STRING("string_type",os);
						break;
					case SLL_CONSTANT_TYPE_ARRAY:
						PRINT_STATIC_STRING("array_type",os);
						break;
					case SLL_CONSTANT_TYPE_HANDLE:
						PRINT_STATIC_STRING("handle_type",os);
						break;
					case SLL_CONSTANT_TYPE_MAP:
						PRINT_STATIC_STRING("map_type",os);
						break;
					case SLL_CONSTANT_TYPE_MAP_KEY:
						PRINT_STATIC_STRING("map_key_type",os);
						break;
					case SLL_CONSTANT_TYPE_MAP_VALUE:
						PRINT_STATIC_STRING("map_value_type",os);
						break;
					default:
						SLL_UNREACHABLE();
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
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_REF:
				PRINT_STATIC_STRING("REF",os);
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
				PRINT_STATIC_STRING("DEL $",os);
				_print_int(ai->dt.v,os);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL:
				PRINT_STATIC_STRING("LOAD $",os);
				_print_int(ai->dt.v,os);
				PRINT_STATIC_STRING(" & DEL",os);
				break;
			default:
				SLL_UNREACHABLE();
		}
		ai++;
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			ai=ai->dt._p;
		}
	}
}



__SLL_FUNC void sll_print_object(const sll_compilation_data_t* c_dt,const sll_internal_function_table_t* i_ft,const sll_object_t* o,sll_output_data_stream_t* os){
	_print_object_internal(c_dt,i_ft,o,os);
}
