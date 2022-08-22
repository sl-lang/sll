#ifndef __SLL_GENERATED_OPERATOR_PARSER_H__
#define __SLL_GENERATED_OPERATOR_PARSER_H__ 1
#include <sll/_internal/common.h>
#include <sll/_internal/parse.h>
#include <sll/file.h>
#include <sll/node.h>
#include <sll/string.h>
#include <sll/types.h>



static __SLL_FORCE_INLINE sll_read_char_t _operator_parser(sll_node_t o,sll_read_char_t c,unsigned int* fl,sll_source_file_t* source_file,sll_file_t* rf){
	sll_char_t str[4];
	sll_string_length_t sz=0;
	do{
		if (sz<4){
			str[sz]=(sll_char_t)c;
			sz++;
		}
		c=sll_file_read_char(rf,NULL);
	} while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!=SLL_END_OF_DATA));
	o->type=SLL_NODE_TYPE_OPERATION_LIST;
	if (sz==1){
		if (str[0]=='!'){
			o->type=SLL_NODE_TYPE_NOT;
		}
		else if (str[0]=='#'){
			o->type=SLL_NODE_TYPE_OPERATION_LIST;
			(*fl)|=EXTRA_COMPILATION_DATA_VARIABLE_DECLARATION;
		}
		else if (str[0]=='$'){
			o->type=SLL_NODE_TYPE_LENGTH;
		}
		else if (str[0]=='%'){
			o->type=SLL_NODE_TYPE_MOD;
		}
		else if (str[0]=='&'){
			o->type=SLL_NODE_TYPE_BIT_AND;
		}
		else if (str[0]=='*'){
			o->type=SLL_NODE_TYPE_MULT;
		}
		else if (str[0]=='+'){
			o->type=SLL_NODE_TYPE_ADD;
		}
		else if (str[0]==','){
			o->type=SLL_NODE_TYPE_COMMA;
		}
		else if (str[0]=='-'){
			o->type=SLL_NODE_TYPE_SUB;
		}
		else if (str[0]=='.'){
			o->type=SLL_NODE_TYPE_NEW;
		}
		else if (str[0]=='/'){
			o->type=SLL_NODE_TYPE_DIV;
		}
		else if (str[0]==':'){
			o->type=SLL_NODE_TYPE_ACCESS;
		}
		else if (str[0]=='<'){
			o->type=SLL_NODE_TYPE_LESS;
		}
		else if (str[0]=='='){
			o->type=SLL_NODE_TYPE_ASSIGN;
		}
		else if (str[0]=='>'){
			o->type=SLL_NODE_TYPE_MORE;
		}
		else if (str[0]=='?'){
			o->type=SLL_NODE_TYPE_IF;
		}
		else if (str[0]=='@'){
			o->type=SLL_NODE_TYPE_BREAK;
		}
		else if (str[0]=='^'){
			o->type=SLL_NODE_TYPE_BIT_XOR;
		}
		else if (str[0]=='|'){
			o->type=SLL_NODE_TYPE_BIT_OR;
		}
		else if (str[0]=='~'){
			o->type=SLL_NODE_TYPE_BIT_NOT;
		}
	}
	else if (sz==2){
		if (str[0]=='!'&&str[1]=='!'){
			o->type=SLL_NODE_TYPE_BOOL;
		}
		else if (str[0]=='!'&&str[1]=='.'){
			o->type=SLL_NODE_TYPE_THREAD_ID;
		}
		else if (str[0]=='!'&&str[1]=='='){
			o->type=SLL_NODE_TYPE_NOT_EQUAL;
		}
		else if (str[0]=='#'&&str[1]=='#'){
			o->type=SLL_NODE_TYPE_OPERATION_LIST;
			(*fl)|=EXTRA_COMPILATION_DATA_EXPORT;
		}
		else if (str[0]=='%'&&str[1]=='%'){
			o->type=SLL_NODE_TYPE_REF;
		}
		else if (str[0]=='&'&&str[1]=='&'){
			o->type=SLL_NODE_TYPE_AND;
		}
		else if (str[0]=='&'&&str[1]==':'){
			o->type=SLL_NODE_TYPE_DECL;
			o->data.declaration.description_string_index=SLL_MAX_STRING_INDEX;
		}
		else if (str[0]=='+'&&str[1]=='+'){
			o->type=SLL_NODE_TYPE_INC;
		}
		else if (str[0]=='-'&&str[1]=='-'){
			o->type=SLL_NODE_TYPE_DEC;
		}
		else if (str[0]=='-'&&str[1]=='>'){
			o->type=SLL_NODE_TYPE_FOR;
			o->data.loop.scope=source_file->_next_scope;
		}
		else if (str[0]=='.'&&str[1]=='?'){
			o->type=SLL_NODE_TYPE_NAMEOF;
		}
		else if (str[0]=='/'&&str[1]=='/'){
			o->type=SLL_NODE_TYPE_FLOOR_DIV;
		}
		else if (str[0]==':'&&str[1]=='!'){
			o->type=SLL_NODE_TYPE_DEEP_COPY;
		}
		else if (str[0]==':'&&str[1]==':'){
			o->type=SLL_NODE_TYPE_CAST;
		}
		else if (str[0]==':'&&str[1]=='>'){
			o->type=SLL_NODE_TYPE_PRINT;
		}
		else if (str[0]==':'&&str[1]=='?'){
			o->type=SLL_NODE_TYPE_TYPEOF;
		}
		else if (str[0]=='<'&&str[1]=='-'){
			o->type=SLL_NODE_TYPE_CALL;
		}
		else if (str[0]=='<'&&str[1]=='<'){
			o->type=SLL_NODE_TYPE_BIT_LEFT_SHIFT;
		}
		else if (str[0]=='<'&&str[1]=='='){
			o->type=SLL_NODE_TYPE_LESS_EQUAL;
		}
		else if (str[0]=='='&&str[1]=='='){
			o->type=SLL_NODE_TYPE_EQUAL;
		}
		else if (str[0]=='>'&&str[1]=='-'){
			o->type=SLL_NODE_TYPE_WHILE;
			o->data.loop.scope=source_file->_next_scope;
		}
		else if (str[0]=='>'&&str[1]=='<'){
			o->type=SLL_NODE_TYPE_LOOP;
		}
		else if (str[0]=='>'&&str[1]=='='){
			o->type=SLL_NODE_TYPE_MORE_EQUAL;
		}
		else if (str[0]=='>'&&str[1]=='>'){
			o->type=SLL_NODE_TYPE_BIT_RIGHT_SHIFT;
		}
		else if (str[0]=='?'&&str[1]==':'){
			o->type=SLL_NODE_TYPE_INLINE_IF;
		}
		else if (str[0]=='?'&&str[1]=='?'){
			o->type=SLL_NODE_TYPE_SWITCH;
		}
		else if (str[0]=='@'&&str[1]=='@'){
			o->type=SLL_NODE_TYPE_RETURN;
		}
		else if (str[0]=='['&&str[1]=='<'){
			o->type=SLL_NODE_TYPE_WHILE_ARRAY;
			o->data.loop.scope=source_file->_next_scope;
		}
		else if (str[0]=='['&&str[1]=='>'){
			o->type=SLL_NODE_TYPE_FOR_ARRAY;
			o->data.loop.scope=source_file->_next_scope;
		}
		else if (str[0]=='{'&&str[1]=='<'){
			o->type=SLL_NODE_TYPE_WHILE_MAP;
			o->data.loop.scope=source_file->_next_scope;
		}
		else if (str[0]=='{'&&str[1]=='>'){
			o->type=SLL_NODE_TYPE_FOR_MAP;
			o->data.loop.scope=source_file->_next_scope;
		}
		else if (str[0]=='|'&&str[1]==':'){
			o->type=SLL_NODE_TYPE_HAS;
		}
		else if (str[0]=='|'&&str[1]=='|'){
			o->type=SLL_NODE_TYPE_OR;
		}
	}
	else if (sz==3){
		if (str[0]=='!'&&str[1]=='<'&&str[2]=='*'){
			o->type=SLL_NODE_TYPE_THREAD_LOCK;
		}
		else if (str[0]=='!'&&str[1]=='<'&&str[2]=='+'){
			o->type=SLL_NODE_TYPE_THREAD_SEMAPHORE;
		}
		else if (str[0]=='!'&&str[1]=='<'&&str[2]=='-'){
			o->type=SLL_NODE_TYPE_READ_BLOCKING;
		}
		else if (str[0]=='!'&&str[1]=='<'&&str[2]=='.'){
			o->type=SLL_NODE_TYPE_READ_BLOCKING_CHAR;
		}
		else if (str[0]=='!'&&str[1]=='<'&&str[2]=='<'){
			o->type=SLL_NODE_TYPE_THREAD_WAIT;
		}
		else if (str[0]=='!'&&str[1]=='<'&&str[2]=='='){
			o->type=SLL_NODE_TYPE_THREAD_BARRIER_EQ;
		}
		else if (str[0]=='!'&&str[1]=='<'&&str[2]=='>'){
			o->type=SLL_NODE_TYPE_THREAD_BARRIER_GEQ;
		}
		else if (str[0]=='!'&&str[1]=='='&&str[2]=='='){
			o->type=SLL_NODE_TYPE_STRICT_NOT_EQUAL;
		}
		else if (str[0]=='!'&&str[1]=='@'&&str[2]=='@'){
			o->type=SLL_NODE_TYPE_THREAD_EXIT;
		}
		else if (str[0]=='&'&&str[1]==':'&&str[2]=='?'){
			o->type=SLL_NODE_TYPE_NAMEOF_TYPE;
		}
		else if (str[0]=='*'&&str[1]=='*'&&str[2]=='*'){
			o->type=SLL_NODE_TYPE_INLINE_FUNC;
			o->data.function.scope=source_file->_next_scope;
		}
		else if (str[0]==','&&str[1]==','&&str[2]==','){
			o->type=SLL_NODE_TYPE_FUNC;
			o->data.function.scope=source_file->_next_scope;
		}
		else if (str[0]=='-'&&str[1]=='-'&&str[2]=='-'){
			o->type=SLL_NODE_TYPE_OPERATION_LIST;
			(*fl)|=EXTRA_COMPILATION_DATA_IMPORT;
		}
		else if (str[0]=='.'&&str[1]=='.'&&str[2]=='.'){
			o->type=SLL_NODE_TYPE_INTERNAL_FUNC;
		}
		else if (str[0]=='<'&&str[1]=='-'&&str[2]=='*'){
			o->type=SLL_NODE_TYPE_CALL_ARRAY;
		}
		else if (str[0]=='<'&&str[1]=='<'&&str[2]=='<'){
			o->type=SLL_NODE_TYPE_CONTINUE;
		}
		else if (str[0]=='='&&str[1]=='='&&str[2]=='='){
			o->type=SLL_NODE_TYPE_STRICT_EQUAL;
		}
	}
	return c;
}



#endif
