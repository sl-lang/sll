#include <lll.h>
#include <_lll_internal.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



__LLL_IMPORT_EXPORT void lll_print_error(lll_input_data_stream_t* is,lll_error_t* e){
	if (e->t==LLL_ERROR_UNKNOWN||e->t>LLL_MAX_SYNTAX_ERROR){
		switch (e->t){
			default:
			case LLL_ERROR_UNKNOWN:
				printf("Unknown Error: %.2"PRIx8"\n",e->t);
				return;
			case LLL_ERROR_NO_STACK:
				printf("No Internal Stack\n");
				return;
			case LLL_ERROR_STACK_TOO_BIG:
				printf("Stack Size Too Big\n");
				return;
			case LLL_ERROR_FAILED_FILE_WRITE:
				printf("Failed to Write to File\n");
				return;
			case LLL_ERROR_DIVISION_BY_ZERO:
				printf("Division By Zero\n");
				return;
			case LLL_ERROR_ASSERTION:
				printf("%s\n",e->err);
				return;
		}
	}
	ENABLE_COLOR();
	uint32_t os=e->off;
	uint32_t oe=os+e->sz;
	LLL_INPUT_DATA_STREAM_RESTART_LINE(is,os);
	uint32_t off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is);
	uint32_t s_off=off;
	uint32_t os_tb=0;
	uint32_t oe_tb=0;
	int c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
	char t=0;
	char* sym=NULL;
	char* sp=NULL;
	if (e->t==LLL_ERROR_UNKNOWN_SYMBOL||e->t==LLL_ERROR_UNKNOWN_MODIFIER||e->t==LLL_ERROR_UNKNOWN_OUTPUT_MODIFIER){
		sym=malloc((oe-os+1)*sizeof(char));
		sp=sym;
	}
	while (c!='\n'&&c!='\r'&&c!=LLL_END_OF_DATA){
		if (off==os){
			t=c;
			printf(HIGHLIGHT_COLOR);
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
		c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
		off++;
		if (off==oe){
			printf(HIGHLIGHT_COLOR_RESET);
		}
	}
	putchar('\n');
	uint32_t i=s_off;
	for (;i<os+os_tb;i++){
		putchar(' ');
	}
	printf(HIGHLIGHT_COLOR);
	for (;i<oe+oe_tb;i++){
		putchar('~');
	}
	printf(HIGHLIGHT_COLOR_RESET"\n");
	DISABLE_COLOR();
	switch (e->t){
		default:
			printf("Unknown Error: %.2"PRIx8"\n",e->t);
			return;
		case LLL_ERROR_INTERNAL_STACK_OVERFLOW:
			printf("Internal Stack Overflow\n");
			return;
		case LLL_ERROR_UNMATCHED_OPEN_PARENTHESES:
			printf("Unmatched Left Parentheses\n");
			return;
		case LLL_ERROR_UNMATCHED_CLOSE_PARENTHESES:
			printf("Unmatched Right Parentheses\n");
			return;
		case LLL_ERROR_UNMATCHED_OPEN_QUOTE:
			printf("Unmatched Block Quote\n");
			return;
		case LLL_ERROR_UNMATCHED_CURLY_OPEN_BRACKETS:
			printf("Unmatched Left Curly Brackets\n");
			return;
		case LLL_ERROR_UNMATCHED_CURLY_CLOSE_BRACKETS:
			printf("Unmatched Right Curly Brackets\n");
			return;
		case LLL_ERROR_UNMATCHED_QUOTES:
			if (t=='\''){
				printf("Unmatched Single Quotes\n");
			}
			else{
				printf("Unmatched Double Quotes\n");
			}
			return;
		case LLL_ERROR_EMPTY_CHAR_STRING:
			printf("Empty Character String\n");
			return;
		case LLL_ERROR_UNTERMINATED_CHAR_STRING:
			printf("Character String too Long\n");
			return;
		case LLL_ERROR_UNTERMINATED_ESCAPE_SEQUENCE:
			printf("Unterminated Escape Sequence\n");
			return;
		case LLL_ERROR_UNKNOWN_ESCAPE_CHARACTER:
			printf("Unknown Escape Character: '%c'\n",t);
			return;
		case LLL_ERROR_UNTERMINATED_HEX_ESCAPE_SEQUENCE:
			printf("Unterminated Hexadecimal Escape Sequence\n");
			return;
		case LLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER:
			printf("Unknown Hexadecimal Character: '%c'\n",t);
			return;
		case LLL_ERROR_UNKNOWN_DECIMAL_CHARCTER:
			printf("Unknown Decimal Character: '%c'\n",t);
			return;
		case LLL_ERROR_UNKNOWN_OCTAL_CHARCTER:
			printf("Unknown Octal Character: '%c'\n",t);
			return;
		case LLL_ERROR_UNKNOWN_BINARY_CHARCTER:
			printf("Unknown Binary Character: '%c'\n",t);
			return;
		case LLL_ERROR_UNKNOWN_SYMBOL:
			*sp=0;
			printf("Unknown Symbol: '%s'\n",sym);
			free(sym);
			return;
		case LLL_ERROR_UNKNOWN_MODIFIER:
			*sp=0;
			printf("Unknown Modifier: '%s'\n",sym);
			free(sym);
			return;
		case LLL_ERROR_UNKNOWN_OUTPUT_MODIFIER:
			*sp=0;
			printf("Unknown Output Modifier: '%s'\n",sym);
			free(sym);
			return;
		case LLL_ERROR_UNKNOWN_IDENTIFIER_CHARACTER:
			printf("Unknown Identifier Character: '%c'\n",t);
			return;
		case LLL_ERROR_UNEXPECTED_CHARACTER:
			printf("Unexpected Character: '%c'\n",t);
			return;
		case LLL_ERROR_UNSUPPORTED_8BIT_FLOAT_SIZE:
			printf("8-bit Float not Supported\n");
			return;
		case LLL_ERROR_UNSUPPORTED_16BIT_FLOAT_SIZE:
			printf("16-bit Float not Supported\n");
			return;
		case LLL_ERROR_SYMBOL_TOO_LONG:
			printf("Symbol Too Long\n");
			return;
		case LLL_ERROR_MODIFIER_TOO_LONG:
			printf("Modifier Too Long\n");
			return;
		case LLL_ERROR_NO_SYMBOL:
			printf("Expression Without a Symbol\n");
			return;
		case LLL_ERROR_TOO_MANY_ARGUMENTS:
			printf("Too Many Arguments\n");
			return;
		case LLL_ERROR_TOO_MANY_STATEMENTS:
			printf("Too Many Statements\n");
			return;
		case LLL_ERROR_MATH_OP_NOT_ENOUGH_ARGUMENTS:
			printf("Math Expression Contains not Enough Symbols\n");
			return;
		case LLL_ERROR_MATH_OP_TOO_MANY_ARGUMENTS:
			printf("Math Expression Contains too Many Symbols\n");
			return;
		case LLL_ERROR_MULTIPLE_OUTPUT_TYPE_MODIFIERS:
			printf("Multiple Output Type Modifiers\n");
			return;
		case LLL_ERROR_MULTIPLE_SIZE_MODIFIERS:
			printf("Multiple Size Modifiers\n");
			return;
		case LLL_ERROR_UNUSED_MODIFIERS:
			printf("Unused Modifiers\n");
			return;
	}
}
