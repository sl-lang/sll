#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdint.h>



__SLL_EXTERNAL void sll_print_error(sll_file_t* rf,const sll_error_t* e){
	if (e->t==SLL_ERROR_UNKNOWN||e->t>SLL_MAX_COMPILATION_ERROR){
		switch (e->t){
			default:
			case SLL_ERROR_UNKNOWN:
				sll_file_write_format(sll_stderr,SLL_CHAR("Unknown Error: %c%c\n"),(e->t>>4)+((e->t>>4)>9?87:48),(e->t&0xf)+((e->t&0xf)>9?87:48));
				return;
			case SLL_ERROR_NO_STACK:
				sll_file_write_format(sll_stderr,SLL_CHAR("No Internal Stack\n"));
				return;
			case SLL_ERROR_DIVISION_BY_ZERO:
				sll_file_write_format(sll_stderr,SLL_CHAR("Division By Zero\n"));
				return;
			case SLL_ERROR_INVALID_FILE_FORMAT:
				sll_file_write_format(sll_stderr,SLL_CHAR("Invalid File Format\n"));
				return;
			case SLL_ERROR_INVALID_INSTRUCTION:
				sll_file_write_format(sll_stderr,SLL_CHAR("Invalid Instruction: 0x%.2hhx\n"),e->dt.it);
				return;
			case SLL_ERROR_STACK_CORRUPTED:
				sll_file_write_format(sll_stderr,SLL_CHAR("Stack Corruption Detected\n"));
				return;
			case SLL_ERROR_INVALID_INSTRUCTION_INDEX:
				sll_file_write_format(sll_stderr,SLL_CHAR("Instruction Index Out of Bounds\n"));
				return;
			case SLL_ERROR_INVALID_STACK_INDEX:
				sll_file_write_format(sll_stderr,SLL_CHAR("Stack Index Out of Bounds\n"));
				return;
		}
	}
	sll_file_offset_t os=e->dt.r.off;
	sll_file_offset_t oe=os+e->dt.r.sz;
	sll_file_reset_line(rf,os);
	sll_file_offset_t off=SLL_FILE_GET_OFFSET(rf);
	sll_file_offset_t s_off=off;
	uint32_t os_tb=0;
	uint32_t oe_tb=0;
	sll_read_char_t c=sll_file_read_char(rf);
	sll_char_t t=0;
	sll_char_t* sym=NULL;
	sll_char_t* sp=NULL;
	if (e->t==SLL_ERROR_UNKNOWN_SYMBOL||e->t==SLL_ERROR_UNKNOWN_IDENTIFIER){
		sym=sll_allocate((oe-os+1)*sizeof(sll_char_t));
		sp=sym;
	}
	while (c!='\n'&&c!='\r'&&c!=SLL_END_OF_DATA){
		if (off==os){
			t=(sll_char_t)c;
		}
		if (off>=os&&off<oe&&sp){
			*sp=(sll_char_t)c;
			sp++;
		}
		if (c=='\t'){
			if (off<os){
				os_tb+=ERROR_DISPLAY_TAB_WIDTH-1;
			}
			if (off<oe){
				oe_tb+=ERROR_DISPLAY_TAB_WIDTH-1;
			}
			for (uint8_t i=0;i<ERROR_DISPLAY_TAB_WIDTH;i++){
				sll_file_write_char(sll_stderr,' ');
			}
		}
		else{
			sll_file_write_char(sll_stderr,(sll_char_t)c);
		}
		c=sll_file_read_char(rf);
		off++;
	}
	sll_file_write_char(sll_stderr,'\n');
	sll_file_offset_t i=s_off;
	for (;i<os+os_tb;i++){
		sll_file_write_char(sll_stderr,' ');
	}
	for (;i<oe+oe_tb;i++){
		sll_file_write_char(sll_stderr,'~');
	}
	sll_file_write_char(sll_stderr,'\n');
	switch (e->t){
		default:
			sll_file_write_format(sll_stderr,SLL_CHAR("Unknown Error: %c%c\n"),(e->t>>4)+((e->t>>4)>9?87:48),(e->t&0xf)+((e->t&0xf)>9?87:48));
			return;
		case SLL_ERROR_INTERNAL_STACK_OVERFLOW:
			sll_file_write_format(sll_stderr,SLL_CHAR("Internal Stack Overflow\n"));
			return;
		case SLL_ERROR_UNMATCHED_OPEN_PARENTHESES:
			sll_file_write_format(sll_stderr,SLL_CHAR("Unmatched Left Parentheses\n"));
			return;
		case SLL_ERROR_UNMATCHED_CLOSE_PARENTHESES:
			sll_file_write_format(sll_stderr,SLL_CHAR("Unmatched Right Parentheses\n"));
			return;
		case SLL_ERROR_UNMATCHED_OPEN_QUOTE:
			sll_file_write_format(sll_stderr,SLL_CHAR("Unmatched Block Quote\n"));
			return;
		case SLL_ERROR_UNMATCHED_CURLY_OPEN_BRACKETS:
			sll_file_write_format(sll_stderr,SLL_CHAR("Unmatched Left Curly Brackets\n"));
			return;
		case SLL_ERROR_UNMATCHED_CURLY_CLOSE_BRACKETS:
			sll_file_write_format(sll_stderr,SLL_CHAR("Unmatched Right Curly Brackets\n"));
			return;
		case SLL_ERROR_UNMATCHED_ARRAY_OPEN_BRACKETS:
			sll_file_write_format(sll_stderr,SLL_CHAR("Unmatched Left Array Brackets\n"));
			return;
		case SLL_ERROR_UNMATCHED_ARRAY_CLOSE_BRACKETS:
			sll_file_write_format(sll_stderr,SLL_CHAR("Unmatched Right Array Brackets\n"));
			return;
		case SLL_ERROR_UNMATCHED_MAP_OPEN_BRACKETS:
			sll_file_write_format(sll_stderr,SLL_CHAR("Unmatched Left Map Brackets\n"));
			return;
		case SLL_ERROR_UNMATCHED_MAP_CLOSE_BRACKETS:
			sll_file_write_format(sll_stderr,SLL_CHAR("Unmatched Right Map Brackets\n"));
			return;
		case SLL_ERROR_UNMATCHED_QUOTES:
			if (t=='\''){
				sll_file_write_format(sll_stderr,SLL_CHAR("Unmatched Single Quotes\n"));
			}
			else{
				sll_file_write_format(sll_stderr,SLL_CHAR("Unmatched Double Quotes\n"));
			}
			return;
		case SLL_ERROR_EMPTY_CHAR_STRING:
			sll_file_write_format(sll_stderr,SLL_CHAR("Empty Character String\n"));
			return;
		case SLL_ERROR_UNTERMINATED_CHAR_STRING:
			sll_file_write_format(sll_stderr,SLL_CHAR("Character String too Long\n"));
			return;
		case SLL_ERROR_UNTERMINATED_ESCAPE_SEQUENCE:
			sll_file_write_format(sll_stderr,SLL_CHAR("Unterminated Escape Sequence\n"));
			return;
		case SLL_ERROR_UNKNOWN_ESCAPE_CHARACTER:
			sll_file_write_format(sll_stderr,SLL_CHAR("Unknown Escape Character: '%c'\n"),t);
			return;
		case SLL_ERROR_UNTERMINATED_HEX_ESCAPE_SEQUENCE:
			sll_file_write_format(sll_stderr,SLL_CHAR("Unterminated Hexadecimal Escape Sequence\n"));
			return;
		case SLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER:
			sll_file_write_format(sll_stderr,SLL_CHAR("Unknown Hexadecimal Character: '%c'\n"),t);
			return;
		case SLL_ERROR_UNKNOWN_DECIMAL_CHARCTER:
			sll_file_write_format(sll_stderr,SLL_CHAR("Unknown Decimal Character: '%c'\n"),t);
			return;
		case SLL_ERROR_UNKNOWN_OCTAL_CHARCTER:
			sll_file_write_format(sll_stderr,SLL_CHAR("Unknown Octal Character: '%c'\n"),t);
			return;
		case SLL_ERROR_UNKNOWN_BINARY_CHARCTER:
			sll_file_write_format(sll_stderr,SLL_CHAR("Unknown Binary Character: '%c'\n"),t);
			return;
		case SLL_ERROR_UNKNOWN_SYMBOL:
			*sp=0;
			sll_file_write_format(sll_stderr,SLL_CHAR("Unknown Symbol: '%s'\n"),sym);
			sll_deallocate(sym);
			return;
		case SLL_ERROR_UNKNOWN_IDENTIFIER_CHARACTER:
			sll_file_write_format(sll_stderr,SLL_CHAR("Unknown Identifier Character: '%c'\n"),t);
			return;
		case SLL_ERROR_UNEXPECTED_CHARACTER:
			sll_file_write_format(sll_stderr,SLL_CHAR("Unexpected Character: '%c'\n"),t);
			return;
		case SLL_ERROR_SYMBOL_TOO_LONG:
			sll_file_write_format(sll_stderr,SLL_CHAR("Symbol Too Long\n"));
			return;
		case SLL_ERROR_NO_SYMBOL:
			sll_file_write_format(sll_stderr,SLL_CHAR("Expression Without a Symbol\n"));
			return;
		case SLL_ERROR_TOO_MANY_ARGUMENTS:
			sll_file_write_format(sll_stderr,SLL_CHAR("Too Many Arguments\n"));
			return;
		case SLL_ERROR_ARRAY_TOO_LONG:
			sll_file_write_format(sll_stderr,SLL_CHAR("Array Too Long\n"));
			return;
		case SLL_ERROR_MAP_TOO_LONG:
			sll_file_write_format(sll_stderr,SLL_CHAR("Map Too Long\n"));
			return;
		case SLL_ERROR_UNKNOWN_IDENTIFIER:
			*sp=0;
			sll_file_write_format(sll_stderr,SLL_CHAR("Unknown Identifier '%s'\n"),sym);
			sll_deallocate(sym);
			return;
		case SLL_ERROR_INTERNAL_FUNCTION_NAME_TOO_LONG:
			sll_file_write_format(sll_stderr,SLL_CHAR("Internal Function Name Too Long\n"));
			return;
		case SLL_ERROR_INTERNAL_FUNCTION_NAME_NOT_ASCII:
			sll_file_write_format(sll_stderr,SLL_CHAR("Internal Function Names Must be ASCII\n"));
			return;
	}
}
