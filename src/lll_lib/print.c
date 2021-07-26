#include <lll_lib.h>
#include <_lll_lib_internal.h>
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



lll_stack_offset_t _print_object_internal(lll_compilation_data_t* c_dt,const lll_object_t* o,FILE* f){
	lll_stack_offset_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	if ((LLL_GET_OBJECT_TYPE(o)>LLL_OBJECT_TYPE_MAX_TYPE&&LLL_GET_OBJECT_TYPE(o)<LLL_OBJECT_TYPE_MIN_EXTRA)||LLL_GET_OBJECT_TYPE(o)==LLL_OBJECT_TYPE_IMPORT){
		fputc('(',f);
	}
	if (LLL_IS_OBJECT_CONST(o)){
		fprintf(f,"@const ");
	}
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
			fprintf(f,"(unknown)");
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_CHAR:
			{
				fputc('\'',f);
				char c=((lll_char_object_t*)o)->v;
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
				return sizeof(lll_char_object_t)+eoff;
			}
		case LLL_OBJECT_TYPE_INT:
			{
				_print_int64(((lll_integer_object_t*)o)->v,f);
				return sizeof(lll_integer_object_t)+eoff;
			}
		case LLL_OBJECT_TYPE_FLOAT:
			fprintf(f,"%lf",((lll_float_object_t*)o)->v);
			return sizeof(lll_float_object_t)+eoff;
		case LLL_OBJECT_TYPE_NIL:
			fprintf(f,"nil");
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_TRUE:
			fprintf(f,"true");
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_FALSE:
			fprintf(f,"false");
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_STRING:
			{
				fputc('"',f);
				lll_string_t* s=*(c_dt->st.dt+((lll_string_object_t*)o)->i);
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
				return sizeof(lll_string_object_t)+eoff;
			}
		case LLL_OBJECT_TYPE_IDENTIFIER:
			{
				lll_identifier_index_t i=((lll_identifier_object_t*)o)->idx;
				lll_identifier_list_length_t j=LLL_IDENTIFIER_GET_ARRAY_ID(i);
				if (j==LLL_MAX_SHORT_IDENTIFIER_LENGTH){
					lll_string_t* s=*(c_dt->st.dt+(c_dt->i_dt.il+LLL_IDENTIFIER_GET_ARRAY_INDEX(i))->i);
					for (lll_string_length_t k=0;k<s->l;k++){
						fputc(s->v[k],f);
					}
					fputc('$',f);
					_print_int64((c_dt->i_dt.il+LLL_IDENTIFIER_GET_ARRAY_INDEX(i))->sc,f);
				}
				else{
					char* s=(*(c_dt->st.dt+(c_dt->i_dt.s[j].dt+LLL_IDENTIFIER_GET_ARRAY_INDEX(i))->i))->v;
					for (lll_string_length_t k=0;k<j+1;k++){
						fputc(*(s+k),f);
					}
					fputc('$',f);
					_print_int64((c_dt->i_dt.s[j].dt+LLL_IDENTIFIER_GET_ARRAY_INDEX(i))->sc,f);
				}
				return sizeof(lll_identifier_object_t)+eoff;
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
		case LLL_OBJECT_TYPE_SET:
			fprintf(f,"=");
			break;
		case LLL_OBJECT_TYPE_FUNC:
			{
				fprintf(f,",,,");
				lll_stack_offset_t off=sizeof(lll_function_object_t);
				lll_arg_count_t l=((lll_function_object_t*)o)->ac;
				while (l){
					l--;
					fputc(' ',f);
					off+=_print_object_internal(c_dt,LLL_GET_OBJECT_ARGUMENT(o,off),f);
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
		case LLL_OBJECT_TYPE_IMPORT:
			{
				fputc('-',f);
				fputc('-',f);
				lll_import_object_t* io=(lll_import_object_t*)o;
				for (lll_arg_count_t i=0;i<io->ac;i++){
					fputc(' ',f);
					fputc('"',f);
					lll_string_t* dt=*(c_dt->st.dt+*(c_dt->im.dt+io->idx[i]));
					for (lll_string_length_t i=0;i<dt->l;i++){
						fputc(dt->v[i],f);
					}
					fputc('"',f);
				}
				fputc(')',f);
				return sizeof(lll_import_object_t)+sizeof(lll_import_index_t)*io->ac+eoff;
			}
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				fputc('{',f);
				lll_stack_offset_t off=sizeof(lll_operation_list_object_t);
				lll_statement_count_t sc=((lll_operation_list_object_t*)o)->sc;
				for (lll_statement_count_t i=0;i<sc;i++){
					if (i){
						fputc(' ',f);
					}
					off+=_print_object_internal(c_dt,LLL_GET_OBJECT_STATEMENT(o,off),f);
				}
				fputc('}',f);
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			{
				lll_debug_object_t* dbg=(lll_debug_object_t*)o;
				fprintf(f,"[%s:%u:%u]",(*(c_dt->st.dt+dbg->fpi))->v,dbg->ln,dbg->cn);
				return sizeof(lll_debug_object_t)+eoff+_print_object_internal(c_dt,LLL_GET_DEBUG_OBJECT_CHILD(dbg),f);
			}
		default:
			UNREACHABLE();
	}
	lll_stack_offset_t off=sizeof(lll_operator_object_t);
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	while (l){
		l--;
		fputc(' ',f);
		off+=_print_object_internal(c_dt,LLL_GET_OBJECT_ARGUMENT(o,off),f);
	}
	fputc(')',f);
	return off+eoff;
}



__LLL_IMPORT_EXPORT void lll_print_object(lll_compilation_data_t* c_dt,const lll_object_t* o,FILE* f){
	_print_object_internal(c_dt,o,f);
}
