#include <lll/_lll_internal.h>
#include <lll/common.h>
#include <lll/core.h>
#include <lll/types.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>



void _print_int64(int64_t v,FILE* f){
	if (!v){
		fputc('0',f);
		return;
	}
	if (v<0){
		v=-v;
		fputc('-',f);
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
		fputc(bf[i]+48,f);
	}
}



lll_object_offset_t _print_object_internal(const lll_compilation_data_t* c_dt,const lll_object_t* o,FILE* f){
	lll_object_offset_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff++;
		o++;
	}
	if (LLL_IS_OBJECT_TYPE_NOT_TYPE(o)&&o->t!=LLL_OBJECT_TYPE_OPERATION_LIST&&o->t!=LLL_OBJECT_TYPE_DEBUG_DATA){
		fputc('(',f);
	}
	switch (o->t){
		case LLL_OBJECT_TYPE_UNKNOWN:
			fprintf(f,"(unknown)");
			return eoff+1;
		case LLL_OBJECT_TYPE_CHAR:
			{
				fputc('\'',f);
				char c=o->dt.c;
				if (c=='\''||c=='"'||c=='\\'){
					fputc('\\',f);
				}
				else if (c=='\t'){
					fputc('\\',f);
					c='t';
				}
				else if (c=='\n'){
					fputc('\\',f);
					c='n';
				}
				else if (c=='\v'){
					fputc('\\',f);
					c='v';
				}
				else if (c=='\f'){
					fputc('\\',f);
					c='f';
				}
				else if (c=='\r'){
					fputc('\\',f);
					c='r';
				}
				else if (c<32||c>126){
					fputc('\\',f);
					fputc('x',f);
					fputc((((uint8_t)c)>>4)+(((uint8_t)c)>159?87:48),f);
					c=(c&0xf)+((c&0xf)>9?87:48);
				}
				fputc(c,f);
				fputc('\'',f);
				return eoff+1;
			}
		case LLL_OBJECT_TYPE_INT:
			{
				_print_int64(o->dt.i,f);
				return eoff+1;
			}
		case LLL_OBJECT_TYPE_FLOAT:
			fprintf(f,"%lf",o->dt.f);
			return eoff+1;
		case LLL_OBJECT_TYPE_STRING:
			{
				fputc('"',f);
				lll_string_t* s=*(c_dt->st.dt+o->dt.s);
				for (lll_string_length_t i=0;i<s->l;i++){
					char c=s->v[i];
					if (c=='\''||c=='"'||c=='\\'){
						fputc('\\',f);
					}
					else if (c=='\t'){
						fputc('\\',f);
						c='t';
					}
					else if (c=='\n'){
						fputc('\\',f);
						c='n';
					}
					else if (c=='\v'){
						fputc('\\',f);
						c='v';
					}
					else if (c=='\f'){
						fputc('\\',f);
						c='f';
					}
					else if (c=='\r'){
						fputc('\\',f);
						c='r';
					}
					else if (c<32||c>126){
						fputc('\\',f);
						fputc('x',f);
						fputc((((uint8_t)c)>>4)+(((uint8_t)c)>159?87:48),f);
						c=(c&0xf)+((c&0xf)>9?87:48);
					}
					fputc(c,f);
				}
				fputc('"',f);
				return eoff+1;
			}
		case LLL_OBJECT_TYPE_IDENTIFIER:
			{
				lll_identifier_index_t i=o->dt.id;
				lll_identifier_list_length_t j=LLL_IDENTIFIER_GET_ARRAY_ID(i);
				if (j==LLL_MAX_SHORT_IDENTIFIER_LENGTH){
					lll_string_t* s=*(c_dt->st.dt+(c_dt->idt.il+LLL_IDENTIFIER_GET_ARRAY_INDEX(i))->i);
					for (lll_string_length_t k=0;k<s->l;k++){
						fputc(s->v[k],f);
					}
					fputc('$',f);
					_print_int64((c_dt->idt.il+LLL_IDENTIFIER_GET_ARRAY_INDEX(i))->sc,f);
				}
				else{
					lll_char_t* s=(*(c_dt->st.dt+(c_dt->idt.s[j].dt+LLL_IDENTIFIER_GET_ARRAY_INDEX(i))->i))->v;
					for (lll_string_length_t k=0;k<j+1;k++){
						fputc(*(s+k),f);
					}
					fputc('$',f);
					_print_int64((c_dt->idt.s[j].dt+LLL_IDENTIFIER_GET_ARRAY_INDEX(i))->sc,f);
				}
				return eoff+1;
			}
		case LLL_OBJECT_TYPE_PRINT:
			fprintf(f,":>");
			break;
		case LLL_OBJECT_TYPE_INPUT:
			fprintf(f,"<:");
			break;
		case LLL_OBJECT_TYPE_AND:
			fprintf(f,"&&");
			break;
		case LLL_OBJECT_TYPE_OR:
			fprintf(f,"||");
			break;
		case LLL_OBJECT_TYPE_NOT:
			fprintf(f,"!");
			break;
		case LLL_OBJECT_TYPE_ASSIGN:
			fprintf(f,"=");
			break;
		case LLL_OBJECT_TYPE_FUNC:
		case LLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				if (o->t==LLL_OBJECT_TYPE_FUNC){
					fprintf(f,",,,");
				}
				else{
					fprintf(f,"... #");
					_print_int64(o->dt.fn.id,f);
				}
				lll_stack_offset_t off=1;
				lll_arg_count_t l=o->dt.fn.ac;
				while (l){
					l--;
					fputc(' ',f);
					off+=_print_object_internal(c_dt,o+off,f);
				}
				fputc(')',f);
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_CALL:
			fprintf(f,"<-");
			break;
		case LLL_OBJECT_TYPE_IF:
			fprintf(f,"?");
			break;
		case LLL_OBJECT_TYPE_FOR:
			fprintf(f,"->");
			break;
		case LLL_OBJECT_TYPE_WHILE:
			fprintf(f,">-");
			break;
		case LLL_OBJECT_TYPE_LOOP:
			fprintf(f,"><");
			break;
		case LLL_OBJECT_TYPE_ADD:
			fprintf(f,"+");
			break;
		case LLL_OBJECT_TYPE_SUB:
			fprintf(f,"-");
			break;
		case LLL_OBJECT_TYPE_MULT:
			fprintf(f,"*");
			break;
		case LLL_OBJECT_TYPE_DIV:
			fprintf(f,"/");
			break;
		case LLL_OBJECT_TYPE_FLOOR_DIV:
			fprintf(f,"//");
			break;
		case LLL_OBJECT_TYPE_MOD:
			fprintf(f,"%%");
			break;
		case LLL_OBJECT_TYPE_BIT_AND:
			fprintf(f,"&");
			break;
		case LLL_OBJECT_TYPE_BIT_OR:
			fprintf(f,"|");
			break;
		case LLL_OBJECT_TYPE_BIT_XOR:
			fprintf(f,"^");
			break;
		case LLL_OBJECT_TYPE_BIT_NOT:
			fprintf(f,"~");
			break;
		case LLL_OBJECT_TYPE_LESS:
			fprintf(f,"<");
			break;
		case LLL_OBJECT_TYPE_LESS_EQUAL:
			fprintf(f,"<=");
			break;
		case LLL_OBJECT_TYPE_EQUAL:
			fprintf(f,"==");
			break;
		case LLL_OBJECT_TYPE_NOT_EQUAL:
			fprintf(f,"!=");
			break;
		case LLL_OBJECT_TYPE_MORE:
			fprintf(f,">");
			break;
		case LLL_OBJECT_TYPE_MORE_EQUAL:
			fprintf(f,">=");
			break;
		case LLL_OBJECT_TYPE_RETURN:
			fprintf(f,"@@");
			break;
		case LLL_OBJECT_TYPE_EXIT:
			fprintf(f,"@");
			break;
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				fputc('{',f);
				lll_stack_offset_t off=1;
				lll_statement_count_t sc=o->dt.sc;
				for (lll_statement_count_t i=0;i<sc;i++){
					if (i){
						fputc(' ',f);
					}
					off+=_print_object_internal(c_dt,o+off,f);
				}
				fputc('}',f);
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			fprintf(f,"[%s:%u:%u]",(*(c_dt->st.dt+o->dt.dbg.fpi))->v,o->dt.dbg.ln+1,o->dt.dbg.cn+1);
			return eoff+_print_object_internal(c_dt,o+1,f)+1;
		default:
			UNREACHABLE();
	}
	lll_stack_offset_t off=1;
	lll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		fputc(' ',f);
		off+=_print_object_internal(c_dt,o+off,f);
	}
	fputc(')',f);
	return off+eoff;
}



__LLL_FUNC void lll_print_assembly(const lll_assembly_data_t* a_dt,FILE* f){
	lll_assembly_instruction_t* ai=a_dt->h;
	for (lll_instruction_index_t i=0;i<a_dt->ic;i++){
		if (i){
			fputc(',',f);
		}
		switch (LLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)){
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP:
				fprintf(f,"NOP");
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_POP:
				fprintf(f,"POP");
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO:
				fprintf(f,"POP & POP");
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_ROT:
				fprintf(f,"ROT");
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP:
				fprintf(f,"ROT & POP");
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_DUP:
				fprintf(f,"DUP");
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
				fprintf(f,"PUSH %"PRId64"",ai->dt.i);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE:
				fprintf(f,"PUSH -1");
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO:
				fprintf(f,"PUSH 0");
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE:
				fprintf(f,"PUSH 1");
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO:
				fprintf(f,"PUSH 2");
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE:
				fprintf(f,"PUSH 3");
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR:
				fprintf(f,"PUSH 4");
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
				fprintf(f,"PUSH %lf",ai->dt.f);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
				fprintf(f,"PUSH %u (char)",ai->dt.c);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD:
				fprintf(f,"LOAD $%u",ai->dt.v);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
				fprintf(f,"LOADS #%u",ai->dt.s);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE:
				fprintf(f,"STORE $%u",ai->dt.v);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP:
				fprintf(f,"STORE $%u & POP",ai->dt.v);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE:
				fprintf(f,"PUSH -1 & STORE $%u",ai->dt.v);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ZERO:
				fprintf(f,"PUSH 0 & STORE $%u",ai->dt.v);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ONE:
				fprintf(f,"PUSH 1 & STORE $%u",ai->dt.v);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_TWO:
				fprintf(f,"PUSH 2 & STORE $%u",ai->dt.v);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_THREE:
				fprintf(f,"PUSH 3 & STORE $%u",ai->dt.v);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_FOUR:
				fprintf(f,"PUSH 4 & STORE $%u",ai->dt.v);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					fprintf(f,"JMP .%+d",ai->dt.rj);
				}
				else{
					fprintf(f,"JMP .%u",ai->dt.j);
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JB:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					fprintf(f,"JB .%+d",ai->dt.rj);
				}
				else{
					fprintf(f,"JB .%u",ai->dt.j);
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					fprintf(f,"JBE .%+d",ai->dt.rj);
				}
				else{
					fprintf(f,"JBE .%u",ai->dt.j);
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JA:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					fprintf(f,"JA .%+d",ai->dt.rj);
				}
				else{
					fprintf(f,"JA .%u",ai->dt.j);
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					fprintf(f,"JAE .%+d",ai->dt.rj);
				}
				else{
					fprintf(f,"JAE .%u",ai->dt.j);
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JE:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					fprintf(f,"JE .%+d",ai->dt.rj);
				}
				else{
					fprintf(f,"JE .%u",ai->dt.j);
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					fprintf(f,"JNE .%+d",ai->dt.rj);
				}
				else{
					fprintf(f,"JNE .%u",ai->dt.j);
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					fprintf(f,"JZ .%+d",ai->dt.rj);
				}
				else{
					fprintf(f,"JZ .%u",ai->dt.j);
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					fprintf(f,"JNZ .%+d",ai->dt.rj);
				}
				else{
					fprintf(f,"JNZ .%u",ai->dt.j);
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_NOT:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					fprintf(f,"NOT $%u",ai->dt.v);
				}
				else{
					fprintf(f,"NOT");
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_ADD:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					fprintf(f,"ADD $%u",ai->dt.v);
				}
				else{
					fprintf(f,"ADD");
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_INC:
				fprintf(f,"INC $%u",ai->dt.v);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_SUB:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					fprintf(f,"SUB $%u",ai->dt.v);
				}
				else{
					fprintf(f,"SUB");
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_DEC:
				fprintf(f,"DEC $%u",ai->dt.v);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_MULT:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					fprintf(f,"MULT $%u",ai->dt.v);
				}
				else{
					fprintf(f,"MULT");
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_DIV:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					fprintf(f,"DIV $%u",ai->dt.v);
				}
				else{
					fprintf(f,"DIV");
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_FDIV:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					fprintf(f,"FDIV $%u",ai->dt.v);
				}
				else{
					fprintf(f,"FDIV");
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_MOD:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					fprintf(f,"MOD $%u",ai->dt.v);
				}
				else{
					fprintf(f,"MOD");
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_AND:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					fprintf(f,"AND $%u",ai->dt.v);
				}
				else{
					fprintf(f,"AND");
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_OR:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					fprintf(f,"OR $%u",ai->dt.v);
				}
				else{
					fprintf(f,"OR");
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_XOR:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					fprintf(f,"XOR $%u",ai->dt.v);
				}
				else{
					fprintf(f,"XOR");
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_INV:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					fprintf(f,"INV $%u",ai->dt.v);
				}
				else{
					fprintf(f,"INV");
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT:
				fprintf(f,"PRINT");
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
				fprintf(f,"PRINT %u (char)",ai->dt.c);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
				fprintf(f,"PRINT #%u",ai->dt.s);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR:
				fprintf(f,"PRINT $%u",ai->dt.v);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
				fprintf(f,"CALL %u",ai->dt.ac);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP:
				fprintf(f,"CALL %u & POP",ai->dt.ac);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET:
				fprintf(f,"RET");
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				fprintf(f,"RET %"PRId64"",ai->dt.i);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO:
				fprintf(f,"RET 0");
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				fprintf(f,"RET %lf",ai->dt.f);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				fprintf(f,"RET %u (char)",ai->dt.c);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				fprintf(f,"RET #%u",ai->dt.s);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR:
				fprintf(f,"RET $%u",ai->dt.v);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_END:
				fprintf(f,"END");
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_END_ZERO:
				fprintf(f,"END 0");
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_END_ONE:
				fprintf(f,"END 1");
				break;
			default:
				UNREACHABLE();
		}
		ai++;
	}
}



__LLL_FUNC void lll_print_object(const lll_compilation_data_t* c_dt,const lll_object_t* o,FILE* f){
	_print_object_internal(c_dt,o,f);
}
