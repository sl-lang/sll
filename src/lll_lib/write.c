#include <lll_lib.h>
#include <_lll_internal.h>
#include <stdint.h>
#include <stdlib.h>



#define REMOVE_PADDING_DEBUG(o,v) \
	do{ \
		while (1){ \
			while ((o)->t==LLL_OBJECT_TYPE_NOP){ \
				v+=sizeof(lll_object_type_t); \
				(o)=LLL_GET_OBJECT_AFTER_NOP((o)); \
			} \
			if (LLL_GET_OBJECT_TYPE((o))==LLL_OBJECT_TYPE_DEBUG_DATA){ \
				lll_debug_object_t* __dbg=(lll_debug_object_t*)(o); \
				uint32_t sz=sizeof(lll_debug_object_t)+LLL_GET_DEBUG_OBJECT_LINE_NUMBER_WIDTH(__dbg)+LLL_GET_DEBUG_OBJECT_COLUMN_NUMBER_WIDTH(__dbg)+LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(__dbg); \
				v+=sz; \
				(o)=LLL_GET_DEBUG_OBJECT_CHILD(__dbg,sz); \
				continue; \
			} \
			break; \
		} \
	} while (0)



uint32_t _get_object_size(lll_object_t* o){
	uint32_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_NIL:
		case LLL_OBJECT_TYPE_TRUE:
		case LLL_OBJECT_TYPE_FALSE:
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_CHAR:
			return sizeof(lll_object_t)+eoff+sizeof(char);
		case LLL_OBJECT_TYPE_STRING:
			return sizeof(lll_object_t)+eoff+sizeof(lll_string_length_t)+LLL_GET_OBJECT_STRING_LENGTH(o);
		case LLL_OBJECT_TYPE_IDENTIFIER:
			return sizeof(lll_object_t)+eoff+sizeof(lll_identifier_index_t);
		case LLL_OBJECT_TYPE_INT:
			return sizeof(lll_object_t)+eoff+LLL_GET_OBJECT_INTEGER_WIDTH(o);
		case LLL_OBJECT_TYPE_FLOAT:
			return sizeof(lll_object_t)+eoff+(LLL_IS_OBJECT_FLOAT64(o)?sizeof(double):sizeof(float));
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				uint32_t off=sizeof(lll_object_t)+sizeof(lll_statement_count_t);
				lll_statement_count_t l=*LLL_GET_OBJECT_STATEMENT_COUNT(o);
				while (l){
					l--;
					off+=_get_object_size(LLL_GET_OBJECT_STATEMENT(o,off));
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			{
				lll_debug_object_t* dbg=(lll_debug_object_t*)o;
				uint32_t sz=sizeof(lll_debug_object_t)+LLL_GET_DEBUG_OBJECT_LINE_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_COLUMN_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(dbg);
				return sz+eoff+_get_object_size(LLL_GET_DEBUG_OBJECT_CHILD(dbg,sz));
			}
	}
	uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
	lll_arg_count_t l=*LLL_GET_OBJECT_ARGUMENT_COUNT(o);
	while (l){
		l--;
		off+=_get_object_size(LLL_GET_OBJECT_ARGUMENT(o,off));
	}
	return off+eoff;
}



void _write_identifier_data(lll_output_data_stream_t* os,identifier_data_t* dt){
	switch (dt->r){
		case REGISTER_CONST:
			{
				int64_t v=dt->e.v;
				if (v<0){
					LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'-');
					v=-v;
				}
				if (!v){
					LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'0');
					return;
				}
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'0');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'x');
				uint64_t i=64;
				while (i){
					i-=4;
					uint8_t c=(v>>i)&0xf;
					if (c>9){
						LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,87+c);
					}
					else{
						LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,48+c);
					}
				}
				return;
			}
		case REGISTER_A:
			if ((dt->r&REGISER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'a');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'l');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'a');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'x');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_32BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'e');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'a');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'x');
			}
			else{
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'a');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'x');
			}
			return;
		case REGISTER_B:
			if ((dt->r&REGISER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'l');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'x');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_32BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'e');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'x');
			}
			else{
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'x');
			}
			return;
		case REGISTER_C:
			if ((dt->r&REGISER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'c');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'l');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'c');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'x');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_32BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'e');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'c');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'x');
			}
			else{
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'c');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'x');
			}
			return;
		case REGISTER_D:
			if ((dt->r&REGISER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'d');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'l');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'d');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'x');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_32BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'e');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'d');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'x');
			}
			else{
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'d');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'x');
			}
			return;
		case REGISTER_SI:
			if ((dt->r&REGISER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'s');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'i');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'l');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'s');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'i');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_32BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'e');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'s');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'i');
			}
			else{
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'s');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'i');
			}
			return;
		case REGISTER_DI:
			if ((dt->r&REGISER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'d');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'i');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'l');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'d');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'i');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_32BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'e');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'d');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'i');
			}
			else{
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'d');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'i');
			}
			return;
		case REGISTER_R8:
			if ((dt->r&REGISER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'8');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'8');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'w');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_32BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'8');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'d');
			}
			else{
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'8');
			}
			return;
		case REGISTER_R9:
			if ((dt->r&REGISER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'9');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'9');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'w');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_32BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'9');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'d');
			}
			else{
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'9');
			}
			return;
		case REGISTER_R10:
			if ((dt->r&REGISER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'0');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'0');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'w');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_32BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'0');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'d');
			}
			else{
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'0');
			}
			return;
		case REGISTER_R11:
			if ((dt->r&REGISER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'w');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_32BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'d');
			}
			else{
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
			}
			return;
		case REGISTER_R12:
			if ((dt->r&REGISER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'2');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'2');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'w');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_32BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'2');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'d');
			}
			else{
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'2');
			}
			return;
		case REGISTER_R13:
			if ((dt->r&REGISER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'3');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'3');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'w');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_32BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'3');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'d');
			}
			else{
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'3');
			}
			return;
		case REGISTER_R14:
			if ((dt->r&REGISER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'4');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'4');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'w');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_32BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'4');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'d');
			}
			else{
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'4');
			}
			return;
		case REGISTER_R15:
			if ((dt->r&REGISER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'5');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'5');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'w');
			}
			else if ((dt->r&REGISER_SIZE_MASK)==REGISTER_32BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'5');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'d');
			}
			else{
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'5');
			}
			return;
		case REGISTER_STACK:
			ASSERT_EXIT(!"Unimplemented");
	}
}



uint32_t _set_register_value(lll_output_data_stream_t* os,identifier_data_t* i_dt,identifier_map_t* im,lll_object_t* o,lll_error_t* e){
	if (i_dt->r==REGISTER_NONE){
		if (!im->rm){
			ASSERT(!"Move Other Variable to Stack",e,UINT32_MAX);
		}
		i_dt->r=REGISTER_FROM_BIT_INDER(FIND_FIRST_SET_BIT(im->rm));
		im->rm&=~REGISTER_TO_MASK(i_dt->r);
	}
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_CHAR:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_INT:
			{
				int64_t v;
				if (LLL_IS_OBJECT_INT8(o)){
					v=LLL_GET_OBJECT_AS_INT8(o);
				}
				else if (LLL_IS_OBJECT_INT16(o)){
					v=LLL_GET_OBJECT_AS_INT16(o);
				}
				else if (LLL_IS_OBJECT_INT32(o)){
					v=LLL_GET_OBJECT_AS_INT32(o);
				}
				else{
					v=LLL_GET_OBJECT_AS_INT64(o);
				}
				if (!v){
					LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\txor ");
					_write_identifier_data(os,i_dt);
					LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,',');
					_write_identifier_data(os,i_dt);
					LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
				}
				else{
					ASSERT(!"Unimplemented",e,UINT32_MAX);
				}
				return sizeof(lll_object_t)+LLL_GET_OBJECT_INTEGER_WIDTH(o);
			}
		case LLL_OBJECT_TYPE_FLOAT:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_NIL:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_TRUE:
			i_dt->r|=REGISTER_8BIT;
			LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tmov ");
			_write_identifier_data(os,i_dt);
			LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,",1\n");
			return sizeof(lll_object_t);
		case LLL_OBJECT_TYPE_FALSE:
			i_dt->r|=REGISTER_8BIT;
			LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\txor ");
			_write_identifier_data(os,i_dt);
			LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,',');
			_write_identifier_data(os,i_dt);
			LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
			return sizeof(lll_object_t);
		case LLL_OBJECT_TYPE_STRING:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_IDENTIFIER:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_CAST_CHAR:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_CAST_STRING:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_CAST_INT:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_CAST_INT64:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_CAST_FLOAT:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_CAST_FLOAT64:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_CAST_BOOL:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FUNC_PRINT:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FUNC_PTR:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FUNC_TYPEOF:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_AND:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_OR:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_NOT:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_SET:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FUNC:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_IF:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FOR:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_ADD:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_SUB:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_MULT:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_DIV:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FLOOR_DIV:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_MOD:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_BIT_AND:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_BIT_OR:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_BIT_XOR:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_BIT_NOT:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_DIV_MOD:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_POW:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_ROOT:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FLOOR_ROOT:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_LOG:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FLOOR_LOG:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_LESS:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_LESS_EQUAL:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_EQUAL:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_NOT_EQUAL:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_MORE:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_MORE_EQUAL:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			ASSERT(!"Unimplemented",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_DEBUG_DATA:
		default:
			ASSERT(!"Unexpected lll_object_type_t Value",e,UINT32_MAX);
	}
	return 0;
}



void _write_label(lll_output_data_stream_t* os,label_t id){
	LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'.');
	LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'l');
	for (uint8_t i=0;i<sizeof(label_t)*2;i++){
		if ((id&0xf)>9){
			LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,87+(id&0xf));
		}
		else{
			LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,48+(id&0xf));
		}
		id>>=4;
		if (!id){
			break;
		}
	}
}



void FORCE_INLINE _write_label_define(lll_output_data_stream_t* os,label_t id){
	_write_label(os,id);
	LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,':');
	LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
}



uint8_t _get_object_as_identifier(lll_output_data_stream_t* os,lll_object_t* o,identifier_data_t* va,identifier_map_t* im,lll_error_t* e){
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_CHAR:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_INT:
			va->r=REGISTER_CONST;
			if (LLL_IS_OBJECT_INT8(o)){
				va->e.v=LLL_GET_OBJECT_AS_INT8(o);
			}
			else if (LLL_IS_OBJECT_INT16(o)){
				va->e.v=LLL_GET_OBJECT_AS_INT16(o);
			}
			else if (LLL_IS_OBJECT_INT32(o)){
				va->e.v=LLL_GET_OBJECT_AS_INT32(o);
			}
			else{
				va->e.v=LLL_GET_OBJECT_AS_INT64(o);
			}
			return 1;
		case LLL_OBJECT_TYPE_FLOAT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_NIL:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_TRUE:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FALSE:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_STRING:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_IDENTIFIER:
			*va=*(im->dt+IDENTIFIER_INDEX_TO_MAP_OFFSET(LLL_GET_OBJECT_AS_IDENTIFIER(o),im));
			return 1;
		case LLL_OBJECT_TYPE_CAST_CHAR:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_CAST_STRING:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_CAST_INT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_CAST_INT64:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_CAST_FLOAT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_CAST_FLOAT64:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_CAST_BOOL:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FUNC_PRINT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FUNC_PTR:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FUNC_TYPEOF:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_AND:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_OR:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_NOT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_SET:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FUNC:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_IF:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FOR:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_ADD:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_SUB:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_MULT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_DIV:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FLOOR_DIV:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_MOD:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_BIT_AND:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_BIT_OR:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_BIT_XOR:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_BIT_NOT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_DIV_MOD:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_POW:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_ROOT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FLOOR_ROOT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_LOG:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FLOOR_LOG:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_LESS:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_LESS_EQUAL:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_EQUAL:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_NOT_EQUAL:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_MORE:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_MORE_EQUAL:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_DEBUG_DATA:
		default:
			ASSERT(!"Unexpected lll_object_type_t Value",e,0);
	}
	return 1;
}



uint8_t _write_object_compare(lll_output_data_stream_t* os,lll_object_t* a,lll_object_t* b,identifier_map_t* im,lll_error_t* e){
	identifier_data_t va;
	identifier_data_t vb;
	if (!_get_object_as_identifier(os,a,&va,im,e)||!_get_object_as_identifier(os,b,&vb,im,e)){
		return 0;
	}
	LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tcmp ");
	_write_identifier_data(os,&va);
	LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,' ');
	_write_identifier_data(os,&vb);
	LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
	return 1;
}



uint8_t _write_jump_if_false(lll_output_data_stream_t* os,lll_object_t* o,identifier_map_t* im,label_t jl,lll_error_t* e){
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_CHAR:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_INT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FLOAT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_NIL:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_TRUE:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FALSE:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_STRING:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_IDENTIFIER:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_CAST_CHAR:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_CAST_STRING:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_CAST_INT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_CAST_INT64:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_CAST_FLOAT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_CAST_FLOAT64:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_CAST_BOOL:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FUNC_PRINT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FUNC_PTR:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FUNC_TYPEOF:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_AND:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_OR:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_NOT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_SET:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FUNC:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_IF:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FOR:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_ADD:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_SUB:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_MULT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_DIV:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FLOOR_DIV:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_MOD:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_BIT_AND:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_BIT_OR:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_BIT_XOR:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_BIT_NOT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_DIV_MOD:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_POW:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_ROOT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FLOOR_ROOT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_LOG:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FLOOR_LOG:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_LESS:
			{
				lll_arg_count_t ac=*LLL_GET_OBJECT_ARGUMENT_COUNT(o);
				if (ac<2){
					return COMPARE_ALWAYS_FALSE;
				}
				uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
				lll_object_t* a=LLL_GET_OBJECT_ARGUMENT(o,off);
				off+=_get_object_size(a);
				REMOVE_PADDING_DEBUG(a,off);
				ac--;
				while (ac){
					lll_object_t* b=LLL_GET_OBJECT_ARGUMENT(o,off);
					off+=_get_object_size(b);
					REMOVE_PADDING_DEBUG(b,off);
					if (!_write_object_compare(os,a,b,im,e)){
						return 0;
					}
					LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tjge ");
					_write_label(os,jl);
					LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
					a=b;
					ac--;
				}
				return 1;
			}
		case LLL_OBJECT_TYPE_LESS_EQUAL:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_EQUAL:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_NOT_EQUAL:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_MORE:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_MORE_EQUAL:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_DEBUG_DATA:
		default:
			ASSERT(!"Unexpected lll_object_type_t Value",e,0);
	}
	return 1;
}



uint32_t _write_object_as_assembly(lll_output_data_stream_t* os,lll_object_t* o,identifier_map_t* im,lll_error_t* e){
	uint32_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_CHAR:
			ASSERT(!"Character Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_INT:
			ASSERT(!"Integer Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FLOAT:
			ASSERT(!"Float Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_NIL:
			ASSERT(!"NIL Value Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_TRUE:
			ASSERT(!"TRUE Value Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FALSE:
			ASSERT(!"FALSE Value Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_STRING:
			ASSERT(!"String Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_IDENTIFIER:
			ASSERT(!"Identifier Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_CAST_CHAR:
			ASSERT(!"'char' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_CAST_STRING:
			ASSERT(!"'str' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_CAST_INT:
			ASSERT(!"'int' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_CAST_INT64:
			ASSERT(!"'int64' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_CAST_FLOAT:
			ASSERT(!"'float' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_CAST_FLOAT64:
			ASSERT(!"'float64' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_CAST_BOOL:
			ASSERT(!"'bool' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FUNC_PRINT:
			{
				// lll_arg_count_t ac=*LLL_GET_OBJECT_STATEMENT_COUNT(o);
				// uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
				// while (ac){
				// 	lll_object_t* s=LLL_GET_OBJECT_STATEMENT(o,off);
				// 	uint32_t aoff=_write_object_as_assembly(os,s,im,e);
				// 	if (aoff==UINT32_MAX){
				// 		return UINT32_MAX;
				// 	}
				// 	off+=aoff;
				// }
				LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\t;print call");
				return _get_object_size(o)+eoff;
			}
		case LLL_OBJECT_TYPE_FUNC_PTR:
			ASSERT(!"'ptr' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FUNC_TYPEOF:
			ASSERT(!"'typeof' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_AND:
			ASSERT(!"'&&' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_OR:
			ASSERT(!"'||' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_NOT:
			ASSERT(!"'!' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_SET:
			{
				lll_arg_count_t ac=*LLL_GET_OBJECT_ARGUMENT_COUNT(o);
				ASSERT(ac>1,e,UINT32_MAX);
				ac-=2;
				ASSERT(!ac||"Assignment multiple arguments not implemented yet",e,UINT32_MAX);
				uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
				lll_object_t* va=LLL_GET_OBJECT_ARGUMENT(o,off);
				off+=sizeof(lll_object_t)+sizeof(lll_identifier_index_t);
				REMOVE_PADDING_DEBUG(va,off);
				ASSERT(va->t==LLL_OBJECT_TYPE_IDENTIFIER,e,UINT32_MAX);
				lll_identifier_index_t v=LLL_GET_OBJECT_AS_IDENTIFIER(va);
				lll_object_t* dta=LLL_GET_OBJECT_ARGUMENT(o,off);
				REMOVE_PADDING_DEBUG(dta,off);
				uint32_t aoff=_set_register_value(os,im->dt+IDENTIFIER_INDEX_TO_MAP_OFFSET(v,im),im,dta,e);
				if (aoff==UINT32_MAX){
					return UINT32_MAX;
				}
				return off+aoff+eoff;
			}
		case LLL_OBJECT_TYPE_FUNC:
			ASSERT(!"'func' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_IF:
			ASSERT(!"'if' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FOR:
			{
				lll_arg_count_t ac=*LLL_GET_OBJECT_ARGUMENT_COUNT(o);
				ASSERT(ac>1,e,UINT32_MAX);
				ac-=2;
				uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
				lll_object_t* init=LLL_GET_OBJECT_ARGUMENT(o,off);
				REMOVE_PADDING_DEBUG(init,off);
				uint32_t aoff=_write_object_as_assembly(os,init,im,e);
				if (aoff==UINT32_MAX){
					return UINT32_MAX;
				}
				off+=aoff;
				lll_object_t* cnd=LLL_GET_OBJECT_ARGUMENT(o,off);
				REMOVE_PADDING_DEBUG(cnd,off);
				off+=_get_object_size(cnd);
				label_t sl=im->nl;
				label_t el=im->nl+1;
				im->nl+=2;
				_write_label_define(os,sl);
				if (!_write_jump_if_false(os,cnd,im,el,e)){
					return UINT32_MAX;
				}
				while (ac){
					ac--;
					lll_object_t* a=LLL_GET_OBJECT_ARGUMENT(o,off);
					REMOVE_PADDING_DEBUG(a,off);
					aoff=_write_object_as_assembly(os,a,im,e);
					if (aoff==UINT32_MAX){
						return UINT32_MAX;
					}
					off+=aoff;
				}
				LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tjmp ");
				_write_label(os,sl);
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
				_write_label_define(os,el);
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_ADD:
			ASSERT(!"'+' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_SUB:
			ASSERT(!"'-' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_MULT:
			ASSERT(!"'*' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_DIV:
			ASSERT(!"'/' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FLOOR_DIV:
			ASSERT(!"'//' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_MOD:
			ASSERT(!"'%' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_BIT_AND:
			ASSERT(!"'&' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_BIT_OR:
			ASSERT(!"'|' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_BIT_XOR:
			ASSERT(!"'^' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_BIT_NOT:
			ASSERT(!"'~' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_DIV_MOD:
			ASSERT(!"'/%' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_POW:
			ASSERT(!"'**' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_ROOT:
			ASSERT(!"'*/' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FLOOR_ROOT:
			ASSERT(!"'*//' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_LOG:
			ASSERT(!"'_/' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FLOOR_LOG:
			ASSERT(!"'_//' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_LESS:
			ASSERT(!"'<' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_LESS_EQUAL:
			ASSERT(!"'<=' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_EQUAL:
			ASSERT(!"'==' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_NOT_EQUAL:
			ASSERT(!"'!=' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_MORE:
			ASSERT(!"'>' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_MORE_EQUAL:
			ASSERT(!"'>=' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_statement_count_t sc=*LLL_GET_OBJECT_STATEMENT_COUNT(o);
				uint32_t off=sizeof(lll_object_t)+sizeof(lll_statement_count_t);
				while (sc){
					lll_object_t* s=LLL_GET_OBJECT_STATEMENT(o,off);
					uint32_t aoff=_write_object_as_assembly(os,s,im,e);
					if (aoff==UINT32_MAX){
						return UINT32_MAX;
					}
					off+=aoff;
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			{
				lll_debug_object_t* dbg=(lll_debug_object_t*)o;
				uint32_t sz=sizeof(lll_debug_object_t)+LLL_GET_DEBUG_OBJECT_LINE_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_COLUMN_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(dbg);
				uint32_t off=_write_object_as_assembly(os,LLL_GET_DEBUG_OBJECT_CHILD(dbg,sz),im,e);
				if (off==UINT32_MAX){
					return off;
				}
				return sz+eoff+off;
			}
		default:
			UNREACHABLE();
	}
}



__LLL_IMPORT_EXPORT __LLL_CHECK_OUTPUT uint8_t lll_write_object(lll_output_data_stream_t* os,lll_object_t* o,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	if (LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)o,_get_object_size(o))){
		return LLL_RETURN_NO_ERROR;
	}
	e->t=LLL_ERROR_FAILED_FILE_WRITE;
	return LLL_RETURN_ERROR;
}



__LLL_IMPORT_EXPORT __LLL_CHECK_OUTPUT uint8_t lll_write_compiled_object(lll_output_data_stream_t* os,lll_compilation_data_t* c_dt,uint8_t f,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	if (f==LLL_WRITE_MODE_ASSEMBLY){
		LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"bits 64\ndefault rel\nsection .text\nglobal main\nextern _CRT_INIT\nextern ExitProcess\nmain:\n\tpush rbp\n\tmov rbp,rsp\n\tsub rsp,32\n\tcall _CRT_INIT\n");
		identifier_map_t im={
			.rm=ALL_REGISTER_AVAIBLE_MASK,
			.nl=0
		};
		uint32_t off=0;
		for (uint32_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
			im.off[i]=off;
			off+=c_dt->i_dt.s[i].l;
		}
		im.off[LLL_MAX_SHORT_IDENTIFIER_LENGTH]=off;
		off+=c_dt->i_dt.ill;
		im.dt=malloc(off*sizeof(identifier_data_t));
		REPEAT_BYTE_SET((unsigned char*)im.dt,REGISTER_NONE,off*sizeof(identifier_data_t)/sizeof(uint8_t));
		if (_write_object_as_assembly(os,c_dt->h,&im,e)==UINT32_MAX){
			free(im.dt);
			return LLL_RETURN_ERROR;
		}
		free(im.dt);
		LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\txor rax,rax\n\tjmp ExitProcess\n");
		return LLL_RETURN_NO_ERROR;
	}
	uint32_t sz=_get_object_size(c_dt->h);
	compiled_object_file_t dt={
		COMPLIED_OBJECT_FILE_MAGIC_NUMBER,
		sz,
		c_dt->tm,
		c_dt->fpl,
		.ill=c_dt->i_dt.ill
	};
	for (uint32_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		dt.sil[i]=c_dt->i_dt.s[i].l;
	}
	if (!LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)(&dt),sizeof(compiled_object_file_t))||!LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,c_dt->fp)){
		e->t=LLL_ERROR_FAILED_FILE_WRITE;
		return LLL_RETURN_ERROR;
	}
	for (uint32_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		lll_identifier_list_t* l=c_dt->i_dt.s+i;
		for (uint32_t j=0;j<l->l;j++){
			if (!LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)(&((l->dt+j)->sc)),sizeof(uint32_t))||!LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)((l->dt+j)->v),(i+1)*sizeof(char))){
				e->t=LLL_ERROR_FAILED_FILE_WRITE;
				return LLL_RETURN_ERROR;
			}
		}
	}
	for (uint32_t i=0;i<c_dt->i_dt.ill;i++){
		lll_identifier_t* k=*(c_dt->i_dt.il+i);
		if (!LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)(&(k->sz)),sizeof(uint32_t))||!LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)(&(k->sc)),sizeof(uint32_t))||!LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)(k->v),k->sz*sizeof(char))){
			e->t=LLL_ERROR_FAILED_FILE_WRITE;
			return LLL_RETURN_ERROR;
		}
	}
	if (!LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)(c_dt->h),sz)){
		e->t=LLL_ERROR_FAILED_FILE_WRITE;
		return LLL_RETURN_ERROR;
	}
	return LLL_RETURN_NO_ERROR;
}
