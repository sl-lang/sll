#include <sll/_internal/common.h>
#include <sll/_internal/print.h>
#include <sll/api/math.h>
#include <sll/api/serial.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/compression.h>
#include <sll/file.h>
#include <sll/identifier.h>
#include <sll/ift.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>
#include <stdio.h>



#define PRINT_INT_SIGN(v,wf) \
	do{ \
		sll_integer_t __v=(v); \
		if (__v>=0){ \
			sll_file_write_char((wf),'+',NULL); \
		} \
		_print_int(__v,(wf)); \
	} while(0)



static void _print_char(sll_char_t c,sll_file_t* wf){
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
		sll_file_write_char(wf,c,NULL);
		return;
	}
	sll_file_write(wf,bf,bfl,NULL);
}



static void _print_float(sll_float_t v,sll_file_t* wf){
	char bf[128];
	int sz=snprintf(bf,128,"%.16lg",v);
	sll_file_write(wf,bf,sz*sizeof(char),NULL);
}



static void _print_complex(const sll_complex_t* c,sll_file_t* wf){
	if (c->real){
		_print_float(c->real,wf);
		if (c->imag){
			sll_file_write_char(wf,(c->imag<0?'-':'+'),NULL);
			_print_float(sll_math_abs(c->imag),wf);
			sll_file_write_char(wf,'i',NULL);
		}
	}
	else if (c->imag){
		_print_float(c->imag,wf);
		sll_file_write_char(wf,'i',NULL);
	}
	else{
		sll_file_write_char(wf,'0',NULL);
	}
}



static void _print_int(sll_integer_t v,sll_file_t* wf){
	if (!v){
		sll_file_write_char(wf,'0',NULL);
		return;
	}
	if (v<0){
		v=-v;
		sll_file_write_char(wf,'-',NULL);
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
		sll_file_write_char(wf,bf[i]+48,NULL);
	}
}



static void _print_identifier(sll_identifier_index_t ii,const sll_source_file_t* sf,sll_file_t* wf){
	sll_identifier_t* id=(SLL_IDENTIFIER_GET_ARRAY_ID(ii)==SLL_MAX_SHORT_IDENTIFIER_LENGTH?sf->identifier_table.long_data:sf->identifier_table.short_[SLL_IDENTIFIER_GET_ARRAY_ID(ii)].data)+SLL_IDENTIFIER_GET_ARRAY_INDEX(ii);
	sll_string_t* s=sf->string_table.data+SLL_IDENTIFIER_GET_STRING_INDEX(id);
	if (SLL_IDENTIFIER_IS_TLS(id)){
		sll_file_write_char(wf,'!',NULL);
	}
	sll_file_write(wf,s->data,s->length,NULL);
	PRINT_STATIC_STRING("|#",wf);
	_print_int(id->scope,wf);
	PRINT_STATIC_STRING("#|",wf);
}



static void _print_line(sll_string_index_t s,const sll_source_file_t* sf,sll_file_offset_t* ln,sll_file_t* wf){
	if (s!=SLL_MAX_STRING_INDEX){
		PRINT_STATIC_STRING("|# ",wf);
		sll_file_write(wf,(sf->string_table.data+s)->data,(sf->string_table.data+s)->length,NULL);
		PRINT_STATIC_STRING(" #|",wf);
		*ln=0;
	}
	(*ln)++;
	PRINT_STATIC_STRING("|# :",wf);
	_print_int(*ln,wf);
	PRINT_STATIC_STRING(" #|",wf);
}



static void _print_assembly_identifier(sll_variable_index_t v,sll_file_t* wf){
	if (SLL_ASSEMBLY_VARIABLE_IS_TLS(v)){
		sll_file_write_char(wf,'!',NULL);
	}
	_print_int(SLL_ASSEMBLY_VARIABLE_GET_INDEX(v),wf);
}



static const sll_node_t* _print_node_internal(const sll_source_file_t* sf,const sll_internal_function_table_t* i_ft,const sll_node_t* o,sll_file_t* wf,sll_file_offset_t* ln){
	while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
		if (o->type==SLL_NODE_TYPE_DBG){
			_print_line(o->data.string_index,sf,ln,wf);
		}
		o=(o->type==SLL_NODE_TYPE_CHANGE_STACK?o->data._next_node:o+1);
	}
	if (SLL_IS_OBJECT_TYPE_NOT_TYPE(o)&&o->type!=SLL_NODE_TYPE_VAR_ACCESS&&o->type!=SLL_NODE_TYPE_OPERATION_LIST&&o->type!=SLL_NODE_TYPE_DBG){
		sll_file_write_char(wf,'(',NULL);
	}
	switch (o->type){
		case SLL_NODE_TYPE_INT:
			_print_int(o->data.int_,wf);
			return o+1;
		case SLL_NODE_TYPE_FLOAT:
			_print_float(o->data.float_,wf);
			return o+1;
		case SLL_NODE_TYPE_CHAR:
			sll_file_write_char(wf,'\'',NULL);
			_print_char(o->data.char_,wf);
			sll_file_write_char(wf,'\'',NULL);
			return o+1;
		case SLL_NODE_TYPE_COMPLEX:
			_print_complex(&(o->data.complex_),wf);
			return o+1;
		case SLL_NODE_TYPE_STRING:
			{
				sll_file_write_char(wf,'"',NULL);
				sll_string_t* s=sf->string_table.data+o->data.string_index;
				for (sll_string_length_t i=0;i<s->length;i++){
					_print_char(s->data[i],wf);
				}
				sll_file_write_char(wf,'"',NULL);
				return o+1;
			}
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_file_write_char(wf,'[',NULL);
				sll_array_length_t al=o->data.array_length;
				o++;
				for (sll_array_length_t i=0;i<al;i++){
					if (i){
						sll_file_write_char(wf,' ',NULL);
					}
					o=_print_node_internal(sf,i_ft,o,wf,ln);
				}
				sll_file_write_char(wf,']',NULL);
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_file_write_char(wf,'<',NULL);
				sll_map_length_t ml=o->data.map_length;
				o++;
				for (sll_map_length_t i=0;i<ml;i++){
					if (i){
						sll_file_write_char(wf,' ',NULL);
					}
					o=_print_node_internal(sf,i_ft,o,wf,ln);
				}
				sll_file_write_char(wf,'>',NULL);
				return o;
			}
		case SLL_NODE_TYPE_IDENTIFIER:
			_print_identifier(o->data.identifier_index,sf,wf);
			return o+1;
		case SLL_NODE_TYPE_FIELD:
			sll_file_write(wf,(sf->string_table.data+o->data.string_index)->data,(sf->string_table.data+o->data.string_index)->length,NULL);
			return o+1;
		case SLL_NODE_TYPE_FUNCTION_ID:
			_print_int(o->data.function_index,wf);
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
			sll_file_write_char(wf,'!',NULL);
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
				const sll_function_t* f=*(sf->function_table.data+o->data.function.function_index);
				if (f->name_string_index!=SLL_MAX_STRING_INDEX){
					PRINT_STATIC_STRING("|#",wf);
					sll_file_write(wf,(sf->string_table.data+f->name_string_index)->data,(sf->string_table.data+f->name_string_index)->length,NULL);
					PRINT_STATIC_STRING("#|",wf);
				}
				for (sll_arg_count_t i=0;i<SLL_FUNCTION_GET_ARGUMENT_COUNT(f);i++){
					sll_file_write_char(wf,' ',NULL);
					_print_identifier(f->args[i],sf,wf);
				}
				if (f->description_string_index!=SLL_MAX_STRING_INDEX){
					SLL_UNIMPLEMENTED();
				}
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				if (o->type==SLL_NODE_TYPE_INTERNAL_FUNC){
					if (i_ft&&o->data.function.function_index<i_ft->length){
						PRINT_STATIC_STRING("... \"",wf);
						sll_file_write(wf,(i_ft->data+o->data.function.function_index)->name.data,(i_ft->data+o->data.function.function_index)->name.length,NULL);
						sll_file_write_char(wf,'\"',NULL);
					}
					else{
						PRINT_STATIC_STRING("... #",wf);
						_print_int(o->data.function.function_index,wf);
					}
				}
				sll_arg_count_t l=o->data.function.arg_count;
				o++;
				while (l){
					l--;
					sll_file_write_char(wf,' ',NULL);
					o=_print_node_internal(sf,i_ft,o,wf,ln);
				}
				sll_file_write_char(wf,')',NULL);
				return o;
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC_LOAD:
			PRINT_STATIC_STRING("...",wf);
			break;
		case SLL_NODE_TYPE_INLINE_FUNC:
			PRINT_STATIC_STRING("***",wf);
			break;
		case SLL_NODE_TYPE_CALL:
		case SLL_NODE_TYPE_CALL_ARRAY:
			{
				if (o->type==SLL_NODE_TYPE_CALL){
					PRINT_STATIC_STRING("<-",wf);
				}
				else{
					PRINT_STATIC_STRING("<-*",wf);
				}
				sll_arg_count_t ac=o->data.arg_count;
				o++;
				if (ac){
					while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
						if (o->type==SLL_NODE_TYPE_DBG){
							_print_line(o->data.string_index,sf,ln,wf);
						}
						o=(o->type==SLL_NODE_TYPE_CHANGE_STACK?o->data._next_node:o+1);
					}
					sll_arg_count_t i=0;
					if (i_ft&&o->type==SLL_NODE_TYPE_INT&&o->data.int_<0){
						sll_function_index_t j=(sll_function_index_t)(~(o->data.int_));
						if (j<i_ft->length){
							PRINT_STATIC_STRING(" (... \"",wf);
							sll_file_write(wf,(i_ft->data+j)->name.data,(i_ft->data+j)->name.length,NULL);
							PRINT_STATIC_STRING("\")",wf);
							i++;
							o++;
						}
					}
					for (;i<ac;i++){
						sll_file_write_char(wf,' ',NULL);
						o=_print_node_internal(sf,i_ft,o,wf,ln);
					}
				}
				sll_file_write_char(wf,')',NULL);
				return o;
			}
		case SLL_NODE_TYPE_IF:
			sll_file_write_char(wf,'?',NULL);
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
				sll_arg_count_t ac=o->data.loop.arg_count;
				o++;
				for (sll_arg_count_t i=0;i<ac;i++){
					sll_file_write_char(wf,' ',NULL);
					o=_print_node_internal(sf,i_ft,o,wf,ln);
				}
				sll_file_write_char(wf,')',NULL);
				return o;
			}
		case SLL_NODE_TYPE_WHILE:
			{
				PRINT_STATIC_STRING(">-",wf);
				sll_arg_count_t ac=o->data.loop.arg_count;
				o++;
				for (sll_arg_count_t i=0;i<ac;i++){
					sll_file_write_char(wf,' ',NULL);
					o=_print_node_internal(sf,i_ft,o,wf,ln);
				}
				sll_file_write_char(wf,')',NULL);
				return o;
			}
		case SLL_NODE_TYPE_LOOP:
			{
				PRINT_STATIC_STRING("><",wf);
				sll_arg_count_t ac=o->data.loop.arg_count;
				o++;
				for (sll_arg_count_t i=0;i<ac;i++){
					sll_file_write_char(wf,' ',NULL);
					o=_print_node_internal(sf,i_ft,o,wf,ln);
				}
				sll_file_write_char(wf,')',NULL);
				return o;
			}
		case SLL_NODE_TYPE_INC:
			PRINT_STATIC_STRING("++",wf);
			break;
		case SLL_NODE_TYPE_DEC:
			PRINT_STATIC_STRING("--",wf);
			break;
		case SLL_NODE_TYPE_ADD:
			sll_file_write_char(wf,'+',NULL);
			break;
		case SLL_NODE_TYPE_SUB:
			sll_file_write_char(wf,'-',NULL);
			break;
		case SLL_NODE_TYPE_MULT:
			sll_file_write_char(wf,'*',NULL);
			break;
		case SLL_NODE_TYPE_DIV:
			sll_file_write_char(wf,'/',NULL);
			break;
		case SLL_NODE_TYPE_FLOOR_DIV:
			PRINT_STATIC_STRING("//",wf);
			break;
		case SLL_NODE_TYPE_MOD:
			sll_file_write_char(wf,'%',NULL);
			break;
		case SLL_NODE_TYPE_BIT_AND:
			sll_file_write_char(wf,'&',NULL);
			break;
		case SLL_NODE_TYPE_BIT_OR:
			sll_file_write_char(wf,'|',NULL);
			break;
		case SLL_NODE_TYPE_BIT_XOR:
			sll_file_write_char(wf,'^',NULL);
			break;
		case SLL_NODE_TYPE_BIT_NOT:
			sll_file_write_char(wf,'~',NULL);
			break;
		case SLL_NODE_TYPE_BIT_RSHIFT:
			PRINT_STATIC_STRING(">>",wf);
			break;
		case SLL_NODE_TYPE_BIT_LSHIFT:
			PRINT_STATIC_STRING("<<",wf);
			break;
		case SLL_NODE_TYPE_LESS:
			sll_file_write_char(wf,'<',NULL);
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
			sll_file_write_char(wf,'>',NULL);
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
			sll_file_write_char(wf,'$',NULL);
			break;
		case SLL_NODE_TYPE_ACCESS:
			sll_file_write_char(wf,':',NULL);
			break;
		case SLL_NODE_TYPE_DEEP_COPY:
			PRINT_STATIC_STRING(":!",wf);
			break;
		case SLL_NODE_TYPE_VAR_ACCESS:
			{
				sll_arg_count_t l=o->data.arg_count;
				SLL_ASSERT(l>1);
				o++;
				while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
					if (o->type==SLL_NODE_TYPE_DBG){
						_print_line(o->data.string_index,sf,ln,wf);
					}
					o=(o->type==SLL_NODE_TYPE_CHANGE_STACK?o->data._next_node:o+1);
				}
				SLL_ASSERT(o->type==SLL_NODE_TYPE_IDENTIFIER);
				_print_identifier(o->data.identifier_index,sf,wf);
				o++;
				l--;
				do{
					l--;
					sll_file_write_char(wf,'$',NULL);
					while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
						if (o->type==SLL_NODE_TYPE_DBG){
							_print_line(o->data.string_index,sf,ln,wf);
						}
						o=(o->type==SLL_NODE_TYPE_CHANGE_STACK?o->data._next_node:o+1);
					}
					SLL_ASSERT(o->type==SLL_NODE_TYPE_FIELD);
					sll_string_t* s=sf->string_table.data+o->data.string_index;
					sll_file_write(wf,s->data,s->length,NULL);
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
		case SLL_NODE_TYPE_NAMEOF:
			PRINT_STATIC_STRING(".?",wf);
			break;
		case SLL_NODE_TYPE_NAMEOF_TYPE:
			PRINT_STATIC_STRING("&:?",wf);
			break;
		case SLL_NODE_TYPE_DECL:
			{
				PRINT_STATIC_STRING("&:",wf);
				if (o->data.declaration.name_string_index!=SLL_MAX_STRING_INDEX){
					PRINT_STATIC_STRING("|#",wf);
					sll_file_write(wf,(sf->string_table.data+o->data.declaration.name_string_index)->data,(sf->string_table.data+o->data.declaration.name_string_index)->length,NULL);
					PRINT_STATIC_STRING("#|",wf);
				}
				sll_arg_count_t ac=o->data.declaration.arg_count;
				o++;
				for (sll_arg_count_t i=0;i<ac;i++){
					sll_file_write_char(wf,' ',NULL);
					o=_print_node_internal(sf,i_ft,o,wf,ln);
				}
				sll_file_write_char(wf,')',NULL);
				return o;
			}
		case SLL_NODE_TYPE_NEW:
			sll_file_write_char(wf,'.',NULL);
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
			sll_file_write_char(wf,'@',NULL);
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
		case SLL_NODE_TYPE_COMMA:
			{
				sll_file_write_char(wf,',',NULL);
				sll_arg_count_t ac=o->data.arg_count;
				o++;
				for (sll_arg_count_t i=0;i<ac;i++){
					sll_file_write_char(wf,' ',NULL);
					o=_print_node_internal(sf,i_ft,o,wf,ln);
				}
				sll_file_write_char(wf,')',NULL);
				return o;
			}
		case SLL_NODE_TYPE_OPERATION_LIST:
			{
				sll_file_write_char(wf,'{',NULL);
				sll_arg_count_t ac=o->data.arg_count;
				o++;
				for (sll_arg_count_t i=0;i<ac;i++){
					if (i){
						sll_file_write_char(wf,' ',NULL);
					}
					o=_print_node_internal(sf,i_ft,o,wf,ln);
				}
				sll_file_write_char(wf,'}',NULL);
				return o;
			}
		case SLL_NODE_TYPE_THREAD_ID:
			PRINT_STATIC_STRING("!.",wf);
			break;
		case SLL_NODE_TYPE_THREAD_WAIT:
			PRINT_STATIC_STRING("!<<",wf);
			break;
		case SLL_NODE_TYPE_THREAD_LOCK:
			PRINT_STATIC_STRING("!<*",wf);
			break;
		case SLL_NODE_TYPE_THREAD_SEMAPHORE:
			PRINT_STATIC_STRING("!<+",wf);
			break;
		case SLL_NODE_TYPE_THREAD_BARRIER_EQ:
			PRINT_STATIC_STRING("!<=",wf);
			break;
		case SLL_NODE_TYPE_THREAD_EXIT:
			PRINT_STATIC_STRING("!@@",wf);
			break;
		case SLL_NODE_TYPE_THREAD_BARRIER_GEQ:
			PRINT_STATIC_STRING("!<>",wf);
			break;
		case SLL_NODE_TYPE_READ_BLOCKING:
			PRINT_STATIC_STRING("!<-",wf);
			break;
		case SLL_NODE_TYPE_READ_BLOCKING_CHAR:
			PRINT_STATIC_STRING("!<.",wf);
			break;
		default:
			SLL_UNREACHABLE();
	}
	sll_arg_count_t ac=o->data.arg_count;
	o++;
	for (sll_arg_count_t i=0;i<ac;i++){
		sll_file_write_char(wf,' ',NULL);
		o=_print_node_internal(sf,i_ft,o,wf,ln);
	}
	sll_file_write_char(wf,')',NULL);
	return o;
}



__SLL_EXTERNAL void sll_print_assembly(const sll_assembly_data_t* assembly_data,sll_file_t* out){
	sll_assembly_instruction_t* ai=assembly_data->first_instruction;
	for (sll_instruction_index_t i=0;i<assembly_data->instruction_count;i++){
		if (i){
			sll_file_write_char(out,',',NULL);
		}
		switch (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)){
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_POP:
				PRINT_STATIC_STRING("POP",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO:
				PRINT_STATIC_STRING("POP & POP",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT:
				PRINT_STATIC_STRING("ROT",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP:
				PRINT_STATIC_STRING("ROT & POP",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DUP:
				PRINT_STATIC_STRING("DUP",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
				PRINT_STATIC_STRING("PUSH ",out);
				_print_int(ai->data.int_,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT_COMPRESSED:
				{
					PRINT_STATIC_STRING("PUSH ",out);
					sll_size_t dec=sll_decompress_integer(ai->data.compressed_int);
					_print_int(SLL_DECODE_SIGNED_INTEGER(dec),out);
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE:
				PRINT_STATIC_STRING("PUSH -1",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO:
				PRINT_STATIC_STRING("PUSH 0",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE:
				PRINT_STATIC_STRING("PUSH 1",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO:
				PRINT_STATIC_STRING("PUSH 2",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE:
				PRINT_STATIC_STRING("PUSH 3",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR:
				PRINT_STATIC_STRING("PUSH 4",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
				PRINT_STATIC_STRING("PUSH ",out);
				_print_float(ai->data.float_,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_COMPLEX:
				PRINT_STATIC_STRING("PUSH ",out);
				_print_complex(&(ai->data.complex_),out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
				PRINT_STATIC_STRING("PUSH c",out);
				_print_int(ai->data.char_,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL:
				PRINT_STATIC_STRING("PUSH .",out);
_print_jump:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)){
					PRINT_INT_SIGN(ai->data.jump.target.rel,out);
				}
				else{
					_print_int(ai->data.jump.target.abs,out);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_STACK:
				PRINT_STATIC_STRING("PUSH 0 x ",out);
				_print_int(ai->data.stack_offset,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD:
				PRINT_STATIC_STRING("LOAD $",out);
				_print_assembly_identifier(ai->data.variable,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
				PRINT_STATIC_STRING("LOADS #",out);
				_print_int(ai->data.string_index,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK:
				PRINT_STATIC_STRING("PACK ",out);
				_print_int(ai->data.array_length,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO:
				PRINT_STATIC_STRING("PACK 0",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ONE:
				PRINT_STATIC_STRING("PACK 1",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP:
				PRINT_STATIC_STRING("MAP ",out);
				_print_int(ai->data.map_length,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP_ZERO:
				PRINT_STATIC_STRING("MAP 0",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE:
				PRINT_STATIC_STRING("STORE $",out);
				_print_assembly_identifier(ai->data.variable,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP:
				PRINT_STATIC_STRING("STORE $",out);
				_print_assembly_identifier(ai->data.variable,out);
				PRINT_STATIC_STRING(" & POP",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE:
				PRINT_STATIC_STRING("PUSH -1 & STORE $",out);
				_print_assembly_identifier(ai->data.variable,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ZERO:
				PRINT_STATIC_STRING("PUSH 0 & STORE $",out);
				_print_assembly_identifier(ai->data.variable,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ONE:
				PRINT_STATIC_STRING("PUSH 1 & STORE $",out);
				_print_assembly_identifier(ai->data.variable,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_TWO:
				PRINT_STATIC_STRING("PUSH 2 & STORE $",out);
				_print_assembly_identifier(ai->data.variable,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_THREE:
				PRINT_STATIC_STRING("PUSH 3 & STORE $",out);
				_print_assembly_identifier(ai->data.variable,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_FOUR:
				PRINT_STATIC_STRING("PUSH 4 & STORE $",out);
				_print_assembly_identifier(ai->data.variable,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP:
				PRINT_STATIC_STRING("LOOKUP",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR:
				PRINT_STATIC_STRING("LOOKUP #",out);
				_print_int(ai->data.string_index,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP:
				PRINT_STATIC_STRING("JMP .",out);
				goto _print_jump;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JB:
				PRINT_STATIC_STRING("JB .",out);
				goto _print_jump;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:
				PRINT_STATIC_STRING("JBE .",out);
				goto _print_jump;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JA:
				PRINT_STATIC_STRING("JA .",out);
				goto _print_jump;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:
				PRINT_STATIC_STRING("JAE .",out);
				goto _print_jump;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JE:
				PRINT_STATIC_STRING("JE .",out);
				goto _print_jump;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:
				PRINT_STATIC_STRING("JNE .",out);
				goto _print_jump;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:
				PRINT_STATIC_STRING("JZ .",out);
				goto _print_jump;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ:
				PRINT_STATIC_STRING("JNZ .",out);
				goto _print_jump;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JSE:
				PRINT_STATIC_STRING("JSE .",out);
				goto _print_jump;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JSNE:
				PRINT_STATIC_STRING("JSNE .",out);
				goto _print_jump;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JI:
				PRINT_STATIC_STRING("JI .",out);
				goto _print_jump;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNI:
				PRINT_STATIC_STRING("JNI .",out);
				goto _print_jump;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JT:
				PRINT_STATIC_STRING("JT ",out);
				_print_int(ai->data.int_,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT:
				PRINT_STATIC_STRING("NOT",out);
_print_inplace:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)){
					PRINT_STATIC_STRING(" $",out);
					_print_assembly_identifier(ai->data.variable,out);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_BOOL:
				PRINT_STATIC_STRING("BOOL",out);
				goto _print_inplace;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_INC:
				PRINT_STATIC_STRING("INC",out);
				goto _print_inplace;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DEC:
				PRINT_STATIC_STRING("DEC",out);
				goto _print_inplace;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD:
				PRINT_STATIC_STRING("ADD",out);
				goto _print_inplace;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_SUB:
				PRINT_STATIC_STRING("SUB",out);
				goto _print_inplace;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MULT:
				PRINT_STATIC_STRING("MULT",out);
				goto _print_inplace;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DIV:
				PRINT_STATIC_STRING("DIV",out);
				goto _print_inplace;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_FDIV:
				PRINT_STATIC_STRING("FDIV",out);
				goto _print_inplace;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MOD:
				PRINT_STATIC_STRING("MOD",out);
				goto _print_inplace;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_AND:
				PRINT_STATIC_STRING("AND",out);
				goto _print_inplace;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_OR:
				PRINT_STATIC_STRING("OR",out);
				goto _print_inplace;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_XOR:
				PRINT_STATIC_STRING("XOR",out);
				goto _print_inplace;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_INV:
				PRINT_STATIC_STRING("INV",out);
				goto _print_inplace;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_SHR:
				PRINT_STATIC_STRING("SHR",out);
				goto _print_inplace;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_SHL:
				PRINT_STATIC_STRING("SHL",out);
				goto _print_inplace;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LENGTH:
				PRINT_STATIC_STRING("LENGTH",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_COPY:
				PRINT_STATIC_STRING("COPY",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DEEP_COPY:
				PRINT_STATIC_STRING("DEEP_COPY",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS:
				PRINT_STATIC_STRING("ACCESS",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_TWO:
				PRINT_STATIC_STRING("ACCESS_RANGE",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_THREE:
				PRINT_STATIC_STRING("ACCESS_RANGE_STEP",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_VAR:
				PRINT_STATIC_STRING("PUSH ",out);
				_print_int(ai->data.variable_access.arg_count,out);
				PRINT_STATIC_STRING(" & ACCESS_VAR $",out);
				_print_assembly_identifier(ai->data.variable_access.variable,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN:
				PRINT_STATIC_STRING("ASSIGN",out);
				goto _print_inplace;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_TWO:
				PRINT_STATIC_STRING("ASSIGN_RANGE",out);
				goto _print_inplace;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_THREE:
				PRINT_STATIC_STRING("ASSIGN_RANGE_STEP",out);
				goto _print_inplace;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR_ACCESS:
				PRINT_STATIC_STRING("PUSH ",out);
				_print_int(ai->data.variable_access.arg_count,out);
				PRINT_STATIC_STRING(" & ASSIGN_VAR_ACCESS $",out);
				_print_assembly_identifier(ai->data.variable_access.variable,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST:
				PRINT_STATIC_STRING("CAST",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE:
				PRINT_STATIC_STRING("CAST ",out);
				_print_int(ai->data.type,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_TYPEOF:
				PRINT_STATIC_STRING("TYPEOF",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NAMEOF:
				PRINT_STATIC_STRING("NAMEOF",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NAMEOF_TYPE:
				PRINT_STATIC_STRING("NAMEOF_TYPE",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL:
				PRINT_STATIC_STRING("DECL ",out);
				_print_int(ai->data.arg_count,out);
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_ANONYMOUS(ai)){
					PRINT_STATIC_STRING(" (anonymous)",out);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL_ZERO:
				PRINT_STATIC_STRING("DECL 0",out);
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_ANONYMOUS(ai)){
					PRINT_STATIC_STRING(" (anonymous)",out);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW:
				PRINT_STATIC_STRING("NEW ",out);
				_print_int(ai->data.arg_count,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW_DECL:
				PRINT_STATIC_STRING("DECL 0 & NEW 0",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT:
				PRINT_STATIC_STRING("PRINT",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
				PRINT_STATIC_STRING("PRINT c",out);
				_print_int(ai->data.char_,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
				PRINT_STATIC_STRING("PRINT #",out);
				_print_int(ai->data.string_index,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR:
				PRINT_STATIC_STRING("PRINT $",out);
				_print_assembly_identifier(ai->data.variable,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
				PRINT_STATIC_STRING("CALL ",out);
				_print_int(ai->data.arg_count,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP:
				PRINT_STATIC_STRING("CALL ",out);
				_print_int(ai->data.arg_count,out);
				PRINT_STATIC_STRING(" & POP",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO:
				PRINT_STATIC_STRING("PUSH ",out);
				_print_int(ai->data.int_,out);
				PRINT_STATIC_STRING(" & CALL 0",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:
				PRINT_STATIC_STRING("PUSH ",out);
				_print_int(ai->data.int_,out);
				PRINT_STATIC_STRING(" & CALL 1",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ARRAY:
				PRINT_STATIC_STRING("CALL_ARRAY",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_REF:
				PRINT_STATIC_STRING("REF",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET:
				PRINT_STATIC_STRING("RET",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				PRINT_STATIC_STRING("RET ",out);
				_print_int(ai->data.int_,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO:
				PRINT_STATIC_STRING("RET 0",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				PRINT_STATIC_STRING("RET ",out);
				_print_float(ai->data.float_,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				PRINT_STATIC_STRING("RET c",out);
				_print_int(ai->data.char_,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				PRINT_STATIC_STRING("RET #",out);
				_print_int(ai->data.string_index,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR:
				PRINT_STATIC_STRING("RET $",out);
				_print_assembly_identifier(ai->data.variable,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL:
				PRINT_STATIC_STRING("DEL $",out);
				_print_assembly_identifier(ai->data.variable,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL:
				PRINT_STATIC_STRING("LOAD $",out);
				_print_assembly_identifier(ai->data.variable,out);
				PRINT_STATIC_STRING(" & DEL",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_WAIT:
				PRINT_STATIC_STRING("THREAD_WAIT",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_ID:
				PRINT_STATIC_STRING("PUSH tid",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_LOCK:
				PRINT_STATIC_STRING("THREAD_LOCK",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_SEMAPHORE:
				PRINT_STATIC_STRING("THREAD_SEMAPHORE",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_BARRIER_EQ:
				PRINT_STATIC_STRING("THREAD_BARRIER_EQ",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_BARRIER_GEQ:
				PRINT_STATIC_STRING("THREAD_BARRIER_GEQ",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_EXIT:
				PRINT_STATIC_STRING("THREAD_EXIT",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_READ_BLOCKING:
				PRINT_STATIC_STRING("READ_BLOCKING",out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_READ_BLOCKING_CHAR:
				PRINT_STATIC_STRING("READ_BLOCKING_CHAR",out);
				break;
			default:
				SLL_UNREACHABLE();
		}
		ai++;
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			ai=ai->data._next_instruction;
		}
	}
}



__SLL_EXTERNAL void sll_print_node(const sll_source_file_t* sf,const sll_internal_function_table_t* i_ft,const sll_node_t* o,sll_file_t* wf){
	sll_file_offset_t off=0;
	_print_node_internal(sf,i_ft,(o?o:sf->first_node),wf,&off);
}
