#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/memory.h>
#include <sll/stream.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdio.h>



__SLL_FUNC void sll_print_error(sll_input_data_stream_t* is,const sll_error_t* e){
	if (e->t==SLL_ERROR_UNKNOWN||e->t>SLL_MAX_COMPILATION_ERROR){
		switch (e->t){
			default:
			case SLL_ERROR_UNKNOWN:
				printf("Unknown Error: %c%c\n",(e->t>>4)+((e->t>>4)>9?87:48),(e->t&0xf)+((e->t&0xf)>9?87:48));
				return;
			case SLL_ERROR_UNKNOWN_INTERNAL_FUNCTION:
				printf("Unknown Internal Function '%s'\n",e->dt.str);
				return;
			case SLL_ERROR_NO_STACK:
				printf("No Internal Stack\n");
				return;
			case SLL_ERROR_DIVISION_BY_ZERO:
				printf("Division By Zero\n");
				return;
			case SLL_ERROR_INVALID_FILE_FORMAT:
				printf("Invalid File Format\n");
				return;
			case SLL_ERROR_INVALID_INSTRUCTION:
				printf("Invalid Instruction: 0x%.2hhx\n",e->dt.it);
				return;
			case SLL_ERROR_STACK_CORRUPTED:
				printf("Stack Corruption Detected\n");
				return;
			case SLL_ERROR_INVALID_INSTRUCTION_INDEX:
				printf("Instruction Index Out of Bounds\n");
				return;
			case SLL_ERROR_INVALID_STACK_INDEX:
				printf("Stack Index Out of Bounds\n");
				return;
			case SLL_ERROR_UNRELEASED_OBJECTS:
				printf("Unreleased Runtime Objects\n");
				return;
			case SLL_ERROR_ASSERTION:
				printf("%s\n",e->dt.str);
				return;
		}
	}
	sll_file_offset_t os=e->dt.r.off;
	sll_file_offset_t oe=os+e->dt.r.sz;
	SLL_INPUT_DATA_STREAM_RESTART_LINE(is,os);
	sll_file_offset_t off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is);
	sll_file_offset_t s_off=off;
	uint32_t os_tb=0;
	uint32_t oe_tb=0;
	int c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
	char t=0;
	char* sym=NULL;
	char* sp=NULL;
	if (e->t==SLL_ERROR_UNKNOWN_SYMBOL||e->t==SLL_ERROR_UNKNOWN_IDENTIFIER){
		sym=sll_allocate((oe-os+1)*sizeof(char));
		sp=sym;
	}
	while (c!='\n'&&c!='\r'&&c!=SLL_END_OF_DATA){
		if (off==os){
			t=c;
		}
		if (off>=os&&off<oe&&sp){
			*sp=c;
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
				putchar(' ');
			}
		}
		else{
			putchar(c);
		}
		c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
		off++;
	}
	putchar('\n');
	sll_file_offset_t i=s_off;
	for (;i<os+os_tb;i++){
		putchar(' ');
	}
	for (;i<oe+oe_tb;i++){
		putchar('~');
	}
	putchar('\n');
	switch (e->t){
		default:
			printf("Unknown Error: %c%c\n",(e->t>>4)+((e->t>>4)>9?87:48),(e->t&0xf)+((e->t&0xf)>9?87:48));
			return;
		case SLL_ERROR_INTERNAL_STACK_OVERFLOW:
			printf("Internal Stack Overflow\n");
			return;
		case SLL_ERROR_UNMATCHED_OPEN_PARENTHESES:
			printf("Unmatched Left Parentheses\n");
			return;
		case SLL_ERROR_UNMATCHED_CLOSE_PARENTHESES:
			printf("Unmatched Right Parentheses\n");
			return;
		case SLL_ERROR_UNMATCHED_OPEN_QUOTE:
			printf("Unmatched Block Quote\n");
			return;
		case SLL_ERROR_UNMATCHED_CURLY_OPEN_BRACKETS:
			printf("Unmatched Left Curly Brackets\n");
			return;
		case SLL_ERROR_UNMATCHED_CURLY_CLOSE_BRACKETS:
			printf("Unmatched Right Curly Brackets\n");
			return;
		case SLL_ERROR_UNMATCHED_ARRAY_OPEN_BRACKETS:
			printf("Unmatched Left Array Brackets\n");
			return;
		case SLL_ERROR_UNMATCHED_ARRAY_CLOSE_BRACKETS:
			printf("Unmatched Right Array Brackets\n");
			return;
		case SLL_ERROR_UNMATCHED_MAP_OPEN_BRACKETS:
			printf("Unmatched Left Map Brackets\n");
			return;
		case SLL_ERROR_UNMATCHED_MAP_CLOSE_BRACKETS:
			printf("Unmatched Right Map Brackets\n");
			return;
		case SLL_ERROR_UNMATCHED_QUOTES:
			if (t=='\''){
				printf("Unmatched Single Quotes\n");
			}
			else{
				printf("Unmatched Double Quotes\n");
			}
			return;
		case SLL_ERROR_EMPTY_CHAR_STRING:
			printf("Empty Character String\n");
			return;
		case SLL_ERROR_UNTERMINATED_CHAR_STRING:
			printf("Character String too Long\n");
			return;
		case SLL_ERROR_UNTERMINATED_ESCAPE_SEQUENCE:
			printf("Unterminated Escape Sequence\n");
			return;
		case SLL_ERROR_UNKNOWN_ESCAPE_CHARACTER:
			printf("Unknown Escape Character: '%c'\n",t);
			return;
		case SLL_ERROR_UNTERMINATED_HEX_ESCAPE_SEQUENCE:
			printf("Unterminated Hexadecimal Escape Sequence\n");
			return;
		case SLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER:
			printf("Unknown Hexadecimal Character: '%c'\n",t);
			return;
		case SLL_ERROR_UNKNOWN_DECIMAL_CHARCTER:
			printf("Unknown Decimal Character: '%c'\n",t);
			return;
		case SLL_ERROR_UNKNOWN_OCTAL_CHARCTER:
			printf("Unknown Octal Character: '%c'\n",t);
			return;
		case SLL_ERROR_UNKNOWN_BINARY_CHARCTER:
			printf("Unknown Binary Character: '%c'\n",t);
			return;
		case SLL_ERROR_UNKNOWN_SYMBOL:
			*sp=0;
			printf("Unknown Symbol: '%s'\n",sym);
			sll_deallocate(sym);
			return;
		case SLL_ERROR_UNKNOWN_IDENTIFIER_CHARACTER:
			printf("Unknown Identifier Character: '%c'\n",t);
			return;
		case SLL_ERROR_UNEXPECTED_CHARACTER:
			printf("Unexpected Character: '%c'\n",t);
			return;
		case SLL_ERROR_SYMBOL_TOO_LONG:
			printf("Symbol Too Long\n");
			return;
		case SLL_ERROR_NO_SYMBOL:
			printf("Expression Without a Symbol\n");
			return;
		case SLL_ERROR_TOO_MANY_ARGUMENTS:
			printf("Too Many Arguments\n");
			return;
		case SLL_ERROR_ARRAY_TOO_LONG:
			printf("Array Too Long\n");
			return;
		case SLL_ERROR_MAP_TOO_LONG:
			printf("Map Too Long\n");
			return;
		case SLL_ERROR_UNKNOWN_IDENTIFIER:
			*sp=0;
			printf("Unknown Identifier '%s'\n",sym);
			sll_deallocate(sym);
			return;
		case SLL_ERROR_INTERNAL_FUNCTION_NAME_TOO_LONG:
			printf("Internal Function Name Too Long\n");
			return;
		case SLL_ERROR_INTERNAL_FUNCTION_NAME_NOT_ASCII:
			printf("Internal Function Names Must be ASCII\n");
			return;
	}
}
