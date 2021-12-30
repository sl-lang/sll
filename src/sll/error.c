#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdint.h>



__SLL_EXTERNAL void sll_print_error(sll_file_t* rf,const sll_error_t* e){
	sll_file_offset_t os=e->dt.r.off;
	sll_file_offset_t oe=os+e->dt.r.sz;
	sll_file_reset_line(rf,os);
	sll_file_offset_t off=SLL_FILE_GET_OFFSET(rf);
	sll_file_offset_t s_off=off;
	uint32_t os_tb=0;
	uint32_t oe_tb=0;
	sll_read_char_t c=sll_file_read_char(rf);
	sll_char_t t=0;
	while (c!='\n'&&c!='\r'&&c!=SLL_END_OF_DATA){
		if (off==os){
			t=(sll_char_t)c;
		}
		if (c=='\t'){
			if (off<os){
				os_tb+=ERROR_DISPLAY_TAB_WIDTH-1;
			}
			if (off<oe){
				oe_tb+=ERROR_DISPLAY_TAB_WIDTH-1;
			}
			for (unsigned int i=0;i<ERROR_DISPLAY_TAB_WIDTH;i++){
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
		case SLL_ERROR_UNMATCHED_OPEN_PARENTHESES:
			sll_file_write_format(sll_stderr,SLL_CHAR("Unmatched Left Parentheses\n"));
			return;
		case SLL_ERROR_UNMATCHED_CLOSE_PARENTHESES:
			sll_file_write_format(sll_stderr,SLL_CHAR("Unmatched Right Parentheses\n"));
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
	}
}
