#include <lll_lib.h>
#include <_lll_internal.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>



#ifdef _MSC_VER
#define UNREACHABLE() __assume(0)
#else
#define UNREACHABLE() __builtin_unreachable()
#endif



uint32_t _print_object_internal(lll_object_t* o,FILE* f){
	uint32_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	if (LLL_GET_OBJECT_TYPE(o)>LLL_OBJECT_TYPE_MAX_TYPE&&LLL_GET_OBJECT_TYPE(o)<LLL_OBJECT_TYPE_MIN_EXTRA){
		fputc('(',f);
	}
	if (LLL_IS_OBJECT_CONST(o)){
		fprintf(f,"@const ");
	}
	if (LLL_GET_OBJECT_TYPE(o)<LLL_OBJECT_TYPE_MIN_EXTRA){
		if (o->m&LLL_OBJECT_MODIFIER_FIXED){
			fprintf(f,"@fixed ");
		}
		if (o->m&LLL_OBJECT_MODIFIER_UNSIGNED){
			fprintf(f,"@unsigned ");
		}
		if (o->m&LLL_OBJECT_MODIFIER_SIZE){
			if ((o->m&LLL_OBJECT_MODIFIER_SIZE_MASK)==LLL_OBJECT_MODIFIER_8BIT){
				fprintf(f,"@8bit ");
			}
			else if ((o->m&LLL_OBJECT_MODIFIER_SIZE_MASK)==LLL_OBJECT_MODIFIER_16BIT){
				fprintf(f,"@16bit ");
			}
			else if ((o->m&LLL_OBJECT_MODIFIER_SIZE_MASK)==LLL_OBJECT_MODIFIER_32BIT){
				fprintf(f,"@32bit ");
			}
			else{
				fprintf(f,"@64bit ");
			}
		}
		if ((o->m&LLL_OBJECT_MODIFIER_OUTPUT_TYPE_MASK)==LLL_OBJECT_MODIFIER_ARRAY){
			fprintf(f,"@array ");
		}
		else if ((o->m&LLL_OBJECT_MODIFIER_OUTPUT_TYPE_MASK)==LLL_OBJECT_MODIFIER_LIST){
			fprintf(f,"@list ");
		}
		else if ((o->m&LLL_OBJECT_MODIFIER_OUTPUT_TYPE_MASK)==LLL_OBJECT_MODIFIER_LAST){
			fprintf(f,"@last ");
		}
	}
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
			fprintf(f,"(unknown)");
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_CHAR:
			fprintf(f,"'%c'",LLL_GET_OBJECT_AS_CHAR(o));
			return sizeof(lll_object_t)+eoff+sizeof(char);
		case LLL_OBJECT_TYPE_INT:
			if (LLL_IS_OBJECT_INT16(o)){
				fprintf(f,"%"PRId16,LLL_GET_OBJECT_AS_INT16(o));
				return sizeof(lll_object_t)+eoff+sizeof(int16_t);
			}
			if (LLL_IS_OBJECT_INT32(o)){
				fprintf(f,"%"PRId32,LLL_GET_OBJECT_AS_INT32(o));
				return sizeof(lll_object_t)+eoff+sizeof(int32_t);
			}
			if (LLL_IS_OBJECT_INT64(o)){
				fprintf(f,"%"PRId64,LLL_GET_OBJECT_AS_INT64(o));
				return sizeof(lll_object_t)+eoff+sizeof(int64_t);
			}
			fprintf(f,"%"PRId8,LLL_GET_OBJECT_AS_INT8(o));
			return sizeof(lll_object_t)+eoff+sizeof(int8_t);
		case LLL_OBJECT_TYPE_FLOAT:
			if (LLL_IS_OBJECT_FLOAT64(o)){
				fprintf(f,"%lf",LLL_GET_OBJECT_AS_FLOAT64(o));
				return sizeof(lll_object_t)+eoff+sizeof(double);
			}
			fprintf(f,"%f",LLL_GET_OBJECT_AS_FLOAT32(o));
			return sizeof(lll_object_t)+eoff+sizeof(float);
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
				lll_string_length_t l=LLL_GET_OBJECT_STRING_LENGTH(o);
				uint32_t off=sizeof(lll_object_t)+l+sizeof(lll_string_length_t);
				char* str=LLL_GET_OBJECT_AS_STRING(o);
				while (l){
					l--;
					char c=*str;
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
					str++;
				}
				fputc('"',f);
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_IDENTIFIER:
			{
				lll_identifier_index_t i=LLL_GET_OBJECT_AS_IDENTIFIER(o);
				if (LLL_IDENTIFIER_GET_ARRAY_ID(i)==LLL_MAX_SHORT_IDENTIFIER_LENGTH){
					fprintf(f,"$long:%u",LLL_IDENTIFIER_GET_ARRAY_INDEX(i));
				}
				else{
					fprintf(f,"$short-%u:%u",LLL_IDENTIFIER_GET_ARRAY_ID(i)+1,LLL_IDENTIFIER_GET_ARRAY_INDEX(i));
				}
				return sizeof(lll_object_t)+eoff+sizeof(lll_identifier_index_t);
			}
		case LLL_OBJECT_TYPE_CAST_CHAR:
			fprintf(f,"char");
			break;
		case LLL_OBJECT_TYPE_CAST_STRING:
			fprintf(f,"str");
			break;
		case LLL_OBJECT_TYPE_CAST_INT:
			fprintf(f,"int");
			break;
		case LLL_OBJECT_TYPE_CAST_INT64:
			fprintf(f,"int64");
			break;
		case LLL_OBJECT_TYPE_CAST_FLOAT:
			fprintf(f,"float");
			break;
		case LLL_OBJECT_TYPE_CAST_FLOAT64:
			fprintf(f,"float64");
			break;
		case LLL_OBJECT_TYPE_CAST_BOOL:
			fprintf(f,"bool");
			break;
		case LLL_OBJECT_TYPE_FUNC_PRINT:
			fprintf(f,"print");
			break;
		case LLL_OBJECT_TYPE_FUNC_PTR:
			fprintf(f,"ptr");
			break;
		case LLL_OBJECT_TYPE_FUNC_TYPEOF:
			fprintf(f,"typeof");
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
			fprintf(f,"func");
			break;
		case LLL_OBJECT_TYPE_IF:
			fprintf(f,"if");
			break;
		case LLL_OBJECT_TYPE_FOR:
			fprintf(f,"for");
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
		case LLL_OBJECT_TYPE_DIV_MOD:
			fprintf(f,"/%%");
			break;
		case LLL_OBJECT_TYPE_POW:
			fprintf(f,"**");
			break;
		case LLL_OBJECT_TYPE_ROOT:
			fprintf(f,"*/");
			break;
		case LLL_OBJECT_TYPE_FLOOR_ROOT:
			fprintf(f,"*//");
			break;
		case LLL_OBJECT_TYPE_LOG:
			fprintf(f,"_/");
			break;
		case LLL_OBJECT_TYPE_FLOOR_LOG:
			fprintf(f,"_//");
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
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				fputc('{',f);
				uint32_t off=sizeof(lll_object_t)+sizeof(lll_statement_count_t);
				lll_statement_count_t sc=*LLL_GET_OBJECT_STATEMENT_COUNT(o);
				for (lll_statement_count_t i=0;i<sc;i++){
					if (i){
						fputc(' ',f);
					}
					off+=_print_object_internal(LLL_GET_OBJECT_STATEMENT(o,off),f);
				}
				fputc('}',f);
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			{
				lll_debug_object_t* dbg=(lll_debug_object_t*)o;
				uint32_t i=sizeof(lll_debug_object_t);
				if (dbg->f&LLL_DEBUG_OBJECT_LINE_NUMBER_INT32){
					fprintf(f,"<%"PRIu32":",LLL_GET_DEBUG_OBJECT_DATA_UINT32(dbg,i)+1);
					i+=sizeof(uint32_t);
				}
				else if (dbg->f&LLL_DEBUG_OBJECT_LINE_NUMBER_INT16){
					fprintf(f,"<%"PRIu16":",LLL_GET_DEBUG_OBJECT_DATA_UINT16(dbg,i)+1);
					i+=sizeof(uint16_t);
				}
				else{
					fprintf(f,"<%"PRIu8":",LLL_GET_DEBUG_OBJECT_DATA_UINT8(dbg,i)+1);
					i+=sizeof(uint8_t);
				}
				if (dbg->f&LLL_DEBUG_OBJECT_COLUMN_NUMBER_INT32){
					fprintf(f,"%"PRIu32">",LLL_GET_DEBUG_OBJECT_DATA_UINT32(dbg,i)+1);
					i+=sizeof(uint32_t);
				}
				else if (dbg->f&LLL_DEBUG_OBJECT_COLUMN_NUMBER_INT16){
					fprintf(f,"%"PRIu16">",LLL_GET_DEBUG_OBJECT_DATA_UINT16(dbg,i)+1);
					i+=sizeof(uint16_t);
				}
				else{
					fprintf(f,"%"PRIu8">",LLL_GET_DEBUG_OBJECT_DATA_UINT8(dbg,i)+1);
					i+=sizeof(uint8_t);
				}
				i+=LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(dbg);
				return i+eoff+_print_object_internal(LLL_GET_DEBUG_OBJECT_CHILD(dbg,i),f);
			}
		default:
			UNREACHABLE();
	}
	uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
	lll_arg_count_t l=*LLL_GET_OBJECT_ARGUMENT_COUNT(o);
	while (l){
		l--;
		fputc(' ',f);
		off+=_print_object_internal(LLL_GET_OBJECT_ARGUMENT(o,off),f);
	}
	fputc(')',f);
	return off+eoff;
}



__LLL_IMPORT_EXPORT void lll_print_object(lll_object_t* o,FILE* f){
	_print_object_internal(o,f);
}
