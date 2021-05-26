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



void _write_hex(lll_output_data_stream_t* os,int64_t v){
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
	uint8_t i=(FIND_LAST_SET_BIT64(v)+4)&0x7c;
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
}



void _write_identifier_data(lll_output_data_stream_t* os,identifier_data_t* dt){
	switch (GET_BASE_REGISTER(dt->r)){
		case REGISTER_CONST:
			{
				_write_hex(os,dt->e.v);
				return;
			}
		case REGISTER_A:
			if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'a');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'l');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'a');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'x');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_32BIT){
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
			if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'l');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'x');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_32BIT){
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
			if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'c');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'l');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'c');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'x');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_32BIT){
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
			if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'d');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'l');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'d');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'x');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_32BIT){
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
			if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'s');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'i');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'l');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'s');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'i');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_32BIT){
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
			if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'d');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'i');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'l');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'d');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'i');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_32BIT){
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
			if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'8');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'8');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'w');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_32BIT){
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
			if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'9');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'9');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'w');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_32BIT){
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
			if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'0');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'0');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'w');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_32BIT){
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
			if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'w');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_32BIT){
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
			if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'2');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'2');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'w');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_32BIT){
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
			if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'3');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'3');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'w');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_32BIT){
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
			if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'4');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'4');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'w');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_32BIT){
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
			if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_8BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'5');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'b');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_16BIT){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'r');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'1');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'5');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'w');
			}
			else if ((dt->r&REGISTER_SIZE_MASK)==REGISTER_32BIT){
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



void FORCE_INLINE _write_save_context(lll_output_data_stream_t* os,uint16_t ctx){
	identifier_data_t tmp;
	while (ctx){
		tmp.r=REGISTER_FROM_BIT_INDER(FIND_FIRST_SET_BIT(ctx));
		ctx&=~REGISTER_TO_MASK(tmp.r);
		tmp.r|=REGISTER_64BIT;
		LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tpush ");
		_write_identifier_data(os,&tmp);
		LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
	}
}



void FORCE_INLINE _write_restore_context(lll_output_data_stream_t* os,uint16_t ctx){
	identifier_data_t tmp;
	while (ctx){
		tmp.r=REGISTER_FROM_BIT_INDER(FIND_LAST_SET_BIT(ctx));
		ctx&=~REGISTER_TO_MASK(tmp.r);
		tmp.r|=REGISTER_64BIT;
		LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tpop ");
		_write_identifier_data(os,&tmp);
		LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
	}
}



void FORCE_INLINE _write_label(lll_output_data_stream_t* os,label_t id){
	LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'.');
	LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'l');
	if (!id){
		LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'0');
		return;
	}
	uint8_t i=(FIND_LAST_SET_BIT(id)+4)&0x3c;
	while (i){
		i-=4;
		uint8_t c=(id>>i)&0xf;
		if (c>9){
			LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,87+c);
		}
		else{
			LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,48+c);
		}
	}
}



void FORCE_INLINE _write_label_define(lll_output_data_stream_t* os,label_t id){
	_write_label(os,id);
	LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,':');
	LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
}



void FORCE_INLINE _release_identifier(identifier_data_t* dt,identifier_map_t* im){
	if (dt->r==REGISTER_CONST){
		return;
	}
	if (dt->r==REGISTER_STACK){
		ASSERT_EXIT(!"Unimplemented");
	}
	im->rm|=REGISTER_TO_MASK(GET_BASE_REGISTER(dt->r));
}



void FORCE_INLINE _write_ensure_register(lll_output_data_stream_t* os,identifier_data_t* dt,cpu_register_t r,identifier_map_t* im){
	if (GET_BASE_REGISTER(dt->r)!=r){
		if (!(im->rm&REGISTER_TO_MASK(r))){
			ASSERT_EXIT(!"Unimplemented");
		}
		identifier_data_t tmp={
			r|(dt->r&REGISTER_SIZE_MASK)
		};
		LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tmov ");
		_write_identifier_data(os,&tmp);
		LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,',');
		_write_identifier_data(os,dt);
		LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
		dt->r=r;
	}
}



uint64_t _get_string_id(assembly_generator_data_t* agd,identifier_data_extra_string_t* s){
	string_table_t* st=&(agd->st);
	for (uint32_t i=0;i<st->l;i++){
		string_table_entry_t* e=*(st->dt+i);
		if (e->sz!=s->l){
			continue;
		}
		else{
			for (uint32_t j=0;j<e->sz;j++){
				if (e->v[j]!=*(s->ptr+j)){
					goto _nxt;
				}
			}
			return i|(((uint64_t)e->nb)<<32);
		}
_nxt:;
	}
	st->l++;
	st->dt=realloc(st->dt,st->l*sizeof(string_table_entry_t*));
	string_table_entry_t* n=malloc(sizeof(string_table_entry_t)+s->l);
	n->sz=s->l;
	n->nb=0;
	for (uint32_t i=0;i<s->l;i++){
		if (!(*(s->ptr+i))){
			n->nb=1;
			break;
		}
	}
	REPEAT_BYTE_COPY((unsigned char*)(n->v),(unsigned char*)(s->ptr),s->l);
	*(st->dt+st->l-1)=n;
	return (st->l-1)|(((uint64_t)n->nb)<<32);
}



uint8_t _get_object_as_identifier(lll_output_data_stream_t* os,lll_object_t* o,identifier_data_t* va,assembly_generator_data_t* agd,lll_error_t* e){
	identifier_map_t* im=&(agd->im);
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_CHAR:
			va->r=REGISTER_CONST|REGISTER_32BIT;
			va->t=IDENTIFIER_DATA_TYPE_CHAR;
			va->e.v=LLL_GET_OBJECT_AS_CHAR(o);
			return 1;
		case LLL_OBJECT_TYPE_INT:
			va->r=REGISTER_CONST;
			if (LLL_IS_OBJECT_INT8(o)){
				va->r|=REGISTER_32BIT;
				va->t=IDENTIFIER_DATA_TYPE_INT32;
				va->e.v=LLL_GET_OBJECT_AS_INT8(o);
			}
			else if (LLL_IS_OBJECT_INT16(o)){
				va->r|=REGISTER_32BIT;
				va->t=IDENTIFIER_DATA_TYPE_INT32;
				va->e.v=LLL_GET_OBJECT_AS_INT16(o);
			}
			else if (LLL_IS_OBJECT_INT32(o)){
				va->r|=REGISTER_32BIT;
				va->t=IDENTIFIER_DATA_TYPE_INT32;
				va->e.v=LLL_GET_OBJECT_AS_INT32(o);
			}
			else{
				va->r|=REGISTER_64BIT;
				va->t=IDENTIFIER_DATA_TYPE_INT64;
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
			va->r=REGISTER_CONST;
			va->t=IDENTIFIER_DATA_TYPE_STRING;
			va->e.str.l=LLL_GET_OBJECT_STRING_LENGTH(o);
			va->e.str.ptr=LLL_GET_OBJECT_AS_STRING(o);
			return 1;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			if (va->r==REGISTER_COPY_IDENTIFIER){
				if (!(im->rm)){
					ASSERT(!"Move other Var to Stack",e,0);
				}
				identifier_data_t* id=im->dt+IDENTIFIER_INDEX_TO_MAP_OFFSET(LLL_GET_OBJECT_AS_IDENTIFIER(o),im);
				va->r=REGISTER_FROM_BIT_INDER(FIND_FIRST_SET_BIT(im->rm));
				va->t=id->t;
				im->rm&=~REGISTER_TO_MASK(va->r);
				va->r|=(id->r&(REGISTER_SIZE_MASK|REGISTER_KNOWN_VALUE))|REGISTER_TEMPORARY;
				if (id->r&REGISTER_KNOWN_VALUE){
					va->e.v=id->e.v;
				}
				LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tmov ");
				_write_identifier_data(os,va);
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,',');
				_write_identifier_data(os,id);
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
				return 1;
			}
			*va=*(im->dt+IDENTIFIER_INDEX_TO_MAP_OFFSET(LLL_GET_OBJECT_AS_IDENTIFIER(o),im));
			return 1;
		case LLL_OBJECT_TYPE_FUNC_PRINT:
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
			{
				lll_arg_count_t ac=*LLL_GET_OBJECT_ARGUMENT_COUNT(o);
				if (!ac){
					ASSERT(!"NIL Unimplemented",e,0);
				}
				va->r=REGISTER_COPY_IDENTIFIER;
				uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
				while (ac){
					ac--;
					lll_object_t* a=LLL_GET_OBJECT_ARGUMENT(o,off);
					off+=_get_object_size(a);
					if (va->r==REGISTER_COPY_IDENTIFIER){
						if (!_get_object_as_identifier(os,a,va,agd,e)){
							return 0;
						}
					}
					else{
						identifier_data_t tmp={
							REGISTER_NONE
						};
						if (!_get_object_as_identifier(os,a,&tmp,agd,e)){
							return 0;
						}
						if ((va->r&REGISTER_SIZE_MASK)<(tmp.r&REGISTER_SIZE_MASK)){
							va->r=(va->r&(~REGISTER_SIZE_MASK))|(tmp.r&REGISTER_SIZE_MASK);
						}
						if (va->t!=tmp.t){
							ASSERT(!"Unimplemented",e,0);
						}
						if (GET_BASE_REGISTER(va->r)==REGISTER_CONST){
							ASSERT(!"Unimplemented",e,0);
						}
						else if (GET_BASE_REGISTER(va->r)==REGISTER_STACK){
							ASSERT(!"Unimplemented",e,0);
						}
						else{
							if (GET_BASE_REGISTER(tmp.r)==REGISTER_CONST){
								if (!tmp.e.v){
									continue;
								}
								else if (tmp.e.v<0){
									LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tsub ");
									_write_identifier_data(os,va);
									LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,',');
									_write_hex(os,-tmp.e.v);
									LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
								}
								else{
									LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tadd ");
									_write_identifier_data(os,va);
									LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,',');
									_write_hex(os,tmp.e.v);
									LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
								}
								continue;
							}
							else if (GET_BASE_REGISTER(tmp.r)==REGISTER_STACK){
								ASSERT(!"Unimplemented",e,0);
							}
							else{
								ASSERT(!"Unimplemented",e,0);
							}
						}
						if (tmp.r&REGISTER_TEMPORARY){
							_release_identifier(&tmp,im);
						}
					}
				}
				return 1;
			}
		case LLL_OBJECT_TYPE_SUB:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_MULT:
			{
				lll_arg_count_t ac=*LLL_GET_OBJECT_ARGUMENT_COUNT(o);
				if (!ac){
					ASSERT(!"NIL Unimplemented",e,0);
				}
				va->r=REGISTER_COPY_IDENTIFIER;
				uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
				while (ac){
					ac--;
					lll_object_t* a=LLL_GET_OBJECT_ARGUMENT(o,off);
					off+=_get_object_size(a);
					if (va->r==REGISTER_COPY_IDENTIFIER){
						if (!_get_object_as_identifier(os,a,va,agd,e)){
							return 0;
						}
					}
					else{
						identifier_data_t tmp={
							REGISTER_NONE
						};
						if (!_get_object_as_identifier(os,a,&tmp,agd,e)){
							return 0;
						}
						if ((va->r&REGISTER_SIZE_MASK)<(tmp.r&REGISTER_SIZE_MASK)){
							va->r=(va->r&(~REGISTER_SIZE_MASK))|(tmp.r&REGISTER_SIZE_MASK);
						}
						if (va->t!=tmp.t){
							ASSERT(!"Unimplemented",e,0);
						}
						if (GET_BASE_REGISTER(va->r)==REGISTER_CONST){
							ASSERT(!"Unimplemented",e,0);
						}
						else if (GET_BASE_REGISTER(va->r)==REGISTER_STACK){
							ASSERT(!"Unimplemented",e,0);
						}
						else{
							if (GET_BASE_REGISTER(tmp.r)==REGISTER_CONST){
								if (!tmp.e.v){
									if (va->r&REGISTER_TEMPORARY){
										_release_identifier(va,im);
									}
									va->r=REGISTER_CONST;
									va->e.v=0;
									return 1;
								}
								else if (tmp.e.v==-1){
									LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tneg ");
									_write_identifier_data(os,va);
									LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
								}
								else if (tmp.e.v!=1){
									ASSERT(!"Unimplemented",e,0);
								}
								continue;
							}
							else if (GET_BASE_REGISTER(tmp.r)==REGISTER_STACK){
								ASSERT(!"Unimplemented",e,0);
							}
							else{
								LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\timul ");
								_write_identifier_data(os,va);
								LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,',');
								_write_identifier_data(os,&tmp);
								LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
							}
						}
						if (tmp.r&REGISTER_TEMPORARY){
							_release_identifier(&tmp,im);
						}
					}
				}
				return 1;
			}
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



uint8_t FORCE_INLINE _write_object_compare(lll_output_data_stream_t* os,lll_object_t* a,lll_object_t* b,assembly_generator_data_t* agd,lll_error_t* e){
	identifier_data_t va={
		REGISTER_NONE
	};
	identifier_data_t vb={
		REGISTER_NONE
	};
	if (!_get_object_as_identifier(os,a,&va,agd,e)||!_get_object_as_identifier(os,b,&vb,agd,e)){
		return 0;
	}
	LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tcmp ");
	_write_identifier_data(os,&va);
	LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,',');
	_write_identifier_data(os,&vb);
	LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
	return 1;
}



uint8_t _get_cond_type(lll_object_t* o,identifier_map_t* im){
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_CHAR:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_INT:
			if (LLL_IS_OBJECT_INT8(o)){
				if (!LLL_GET_OBJECT_AS_INT8(o)){
					return COMPARE_ALWAYS_FALSE;
				}
			}
			else if (LLL_IS_OBJECT_INT16(o)){
				if (!LLL_GET_OBJECT_AS_INT16(o)){
					return COMPARE_ALWAYS_FALSE;
				}
			}
			else if (LLL_IS_OBJECT_INT32(o)){
				if (!LLL_GET_OBJECT_AS_INT32(o)){
					return COMPARE_ALWAYS_FALSE;
				}
			}
			else{
				if (!LLL_GET_OBJECT_AS_INT64(o)){
					return COMPARE_ALWAYS_FALSE;
				}
			}
			return COMPARE_ALWAYS_TRUE;
		case LLL_OBJECT_TYPE_FLOAT:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_NIL:
		case LLL_OBJECT_TYPE_FALSE:
		case LLL_OBJECT_TYPE_FUNC_PRINT:
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			return COMPARE_ALWAYS_FALSE;
		case LLL_OBJECT_TYPE_TRUE:
			return COMPARE_ALWAYS_TRUE;
		case LLL_OBJECT_TYPE_STRING:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_IDENTIFIER:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_FUNC_TYPEOF:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_AND:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_OR:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_NOT:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_SET:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_FUNC:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_IF:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_FOR:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_ADD:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_SUB:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_MULT:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_DIV:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_FLOOR_DIV:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_MOD:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_BIT_AND:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_BIT_OR:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_BIT_XOR:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_BIT_NOT:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_DIV_MOD:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_POW:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_ROOT:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_FLOOR_ROOT:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_LOG:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_FLOOR_LOG:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_LESS:
			{
				lll_arg_count_t ac=*LLL_GET_OBJECT_ARGUMENT_COUNT(o);
				if (ac<2){
					return COMPARE_ALWAYS_TRUE;
				}
				// uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
				// lll_object_t* a=LLL_GET_OBJECT_ARGUMENT(o,off);
				// off+=_get_object_size(a);
				// REMOVE_PADDING_DEBUG(a,off);
				// ac--;
				// while (ac){
				// 	lll_object_t* b=LLL_GET_OBJECT_ARGUMENT(o,off);
				// 	off+=_get_object_size(b);
				// 	REMOVE_PADDING_DEBUG(b,off);
				// 	if (!_write_object_compare(os,a,b,agd,e)){
				// 		return 0;
				// 	}
				// 	LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tjb ");
				// 	_write_label(os,jl);
				// 	LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
				// 	a=b;
				// 	ac--;
				// }
				return COMPARE_UNKNOWN;
			}
		case LLL_OBJECT_TYPE_LESS_EQUAL:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_EQUAL:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_NOT_EQUAL:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_MORE:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_MORE_EQUAL:
			ASSERT_EXIT(!"Unimplemented");
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_DEBUG_DATA:
		default:
			ASSERT_EXIT(!"Unexpected lll_object_type_t Value");
	}
	return COMPARE_UNKNOWN;
}



uint8_t _write_jump_if_true(lll_output_data_stream_t* os,lll_object_t* o,assembly_generator_data_t* agd,label_t jl,lll_error_t* e){
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_CHAR:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_INT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FLOAT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_NIL:
		case LLL_OBJECT_TYPE_FALSE:
		case LLL_OBJECT_TYPE_TRUE:
			ASSERT(!"Shouldn't happen",e,0);
		case LLL_OBJECT_TYPE_STRING:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_IDENTIFIER:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FUNC_PRINT:
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
					ASSERT(!"Should'n happen",e,0);
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
					if (!_write_object_compare(os,a,b,agd,e)){
						return 0;
					}
					LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tjb ");
					_write_label(os,jl);
					LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
					a=b;
					ac--;
				}
				return COMPARE_UNKNOWN;
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



uint8_t _write_jump_if_false(lll_output_data_stream_t* os,lll_object_t* o,assembly_generator_data_t* agd,label_t jl,lll_error_t* e){
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_CHAR:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_INT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FLOAT:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_NIL:
		case LLL_OBJECT_TYPE_FALSE:
		case LLL_OBJECT_TYPE_TRUE:
			ASSERT(!"Shouldn't happen",e,0);
		case LLL_OBJECT_TYPE_STRING:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_IDENTIFIER:
			ASSERT(!"Unimplemented",e,0);
		case LLL_OBJECT_TYPE_FUNC_PRINT:
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
					ASSERT(!"Should'n happen",e,0);
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
					if (!_write_object_compare(os,a,b,agd,e)){
						return 0;
					}
					LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tjae ");
					_write_label(os,jl);
					LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
					a=b;
					ac--;
				}
				return COMPARE_UNKNOWN;
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



uint32_t _write_object_as_assembly(lll_output_data_stream_t* os,lll_object_t* o,assembly_generator_data_t* agd,lll_error_t* e){
	uint32_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	identifier_map_t* im=&(agd->im);
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
		case LLL_OBJECT_TYPE_FUNC_PRINT:
			{
				lll_arg_count_t ac=*LLL_GET_OBJECT_ARGUMENT_COUNT(o);
				uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
				while (ac){
					ac--;
					lll_object_t* a=LLL_GET_OBJECT_ARGUMENT(o,off);
					REMOVE_PADDING_DEBUG(a,off);
					off+=_get_object_size(a);
					identifier_data_t tmp={
						REGISTER_NONE
					};
					if (!_get_object_as_identifier(os,a,&tmp,agd,e)){
						return UINT32_MAX;
					}
					if (!IS_IDENTIFIER_DATA_TYPE_SINGLE(tmp.t)){
						ASSERT(!"Unimplemented",e,UINT32_MAX);
					}
					if (tmp.r&REGISTER_TEMPORARY){
						_release_identifier(&tmp,im);
					}
					switch (tmp.t){
						case IDENTIFIER_DATA_TYPE_CHAR:
							{
								tmp.r=(tmp.r&(~REGISTER_SIZE_MASK))|REGISTER_32BIT;
								_write_ensure_register(os,&tmp,*FUNCTION_CALL_REGISTERS,im);
								uint16_t f_r=(~im->rm)&FUNCTION_NON_VOLATILE_REGISTERS;
								_write_save_context(os,f_r);
								LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tcall putchar\n");
								_write_restore_context(os,f_r);
								break;
							}
						case IDENTIFIER_DATA_TYPE_INT8:
						case IDENTIFIER_DATA_TYPE_INT16:
						case IDENTIFIER_DATA_TYPE_INT32:
							{
								agd->f|=ASSEMBLY_GENERATOR_DATA_FLAG_PRINT_INT32;
								tmp.r=(tmp.r&(~REGISTER_SIZE_MASK))|REGISTER_32BIT;
								_write_ensure_register(os,&tmp,*FUNCTION_CALL_REGISTERS,im);
								uint16_t f_r=(~im->rm)&FUNCTION_NON_VOLATILE_REGISTERS;
								_write_save_context(os,f_r);
								LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tcall .__print_int32\n");
								_write_restore_context(os,f_r);
								break;
							}
						case IDENTIFIER_DATA_TYPE_INT64:
							ASSERT(!"Unimplemented",e,UINT32_MAX);
						case IDENTIFIER_DATA_TYPE_UINT8:
							ASSERT(!"Unimplemented",e,UINT32_MAX);
						case IDENTIFIER_DATA_TYPE_UINT16:
							ASSERT(!"Unimplemented",e,UINT32_MAX);
						case IDENTIFIER_DATA_TYPE_UINT32:
							ASSERT(!"Unimplemented",e,UINT32_MAX);
						case IDENTIFIER_DATA_TYPE_UINT64:
							ASSERT(!"Unimplemented",e,UINT32_MAX);
						case IDENTIFIER_DATA_TYPE_FLOAT32:
							ASSERT(!"Unimplemented",e,UINT32_MAX);
						case IDENTIFIER_DATA_TYPE_FLOAT64:
							ASSERT(!"Unimplemented",e,UINT32_MAX);
						case IDENTIFIER_DATA_TYPE_STRING:
							{
								if (GET_BASE_REGISTER(tmp.r)!=REGISTER_CONST){
									ASSERT(!"Unimplemented",e,UINT32_MAX);
									break;
								}
								identifier_data_extra_string_t s=tmp.e.str;
								if (!s.l){
									break;
								}
								uint16_t f_r=(~im->rm)&FUNCTION_NON_VOLATILE_REGISTERS;
								_write_save_context(os,f_r);
								if (s.l==1){
									char c=*(s.ptr);
									if (!(im->rm&REGISTER_TO_MASK(*FUNCTION_CALL_REGISTERS))){
										ASSERT(!"Move var to stack",e,UINT32_MAX);
									}
									LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tmov ");
									identifier_data_t tmp_r={
										(*FUNCTION_CALL_REGISTERS)|REGISTER_32BIT
									};
									_write_identifier_data(os,&tmp_r);
									LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,",0x");
									LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,((c>>4)<10?48:87)+(c>>4));
									LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,((c&0xf)<10?48:87)+(c&0xf));
									LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\n\tcall putchar\n");
								}
								else{
									agd->f|=ASSEMBLY_GENERATOR_DATA_FLAG_PRINT_STRING;
									uint64_t id=_get_string_id(agd,&s);
									if (id>>32){
										id&=0xffffffff;
										ASSERT(!"Print String with Null Bytes",e,UINT32_MAX);
									}
									LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tlea ");
									identifier_data_t tmp_r={
										(*FUNCTION_CALL_REGISTERS)|REGISTER_64BIT
									};
									_write_identifier_data(os,&tmp_r);
									LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,",[_str_");
									uint8_t i=(FIND_LAST_SET_BIT64(id)+4)&0x7c;
									while (i){
										i-=4;
										uint8_t c=(id>>i)&0xf;
										if (c>9){
											LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,87+c);
										}
										else{
											LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,48+c);
										}
									}
									LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"]\n\tcall .__print_str\n");
								}
								_write_restore_context(os,f_r);
								break;
							}
						case IDENTIFIER_DATA_TYPE_NIL:
							ASSERT(!"Unimplemented",e,UINT32_MAX);
						default:
							UNREACHABLE();
					}
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_FUNC_TYPEOF:
			ASSERT(!"'typeof' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
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
				identifier_data_t* i_dt=im->dt+IDENTIFIER_INDEX_TO_MAP_OFFSET(LLL_GET_OBJECT_AS_IDENTIFIER(va),im);
				lll_object_t* dta=LLL_GET_OBJECT_ARGUMENT(o,off);
				REMOVE_PADDING_DEBUG(dta,off);
				if (i_dt->r==REGISTER_NONE){
					if (!agd->im.rm){
						ASSERT(!"Move Other Variable to Stack",e,UINT32_MAX);
					}
					i_dt->r=REGISTER_FROM_BIT_INDER(FIND_FIRST_SET_BIT(agd->im.rm));
					agd->im.rm&=~REGISTER_TO_MASK(i_dt->r);
				}
				identifier_data_t v={
					REGISTER_NONE
				};
				if (!_get_object_as_identifier(os,dta,&v,agd,e)){
					return UINT32_MAX;
				}
				i_dt->r=GET_BASE_REGISTER(i_dt->r)|(v.r&REGISTER_SIZE_MASK);
				i_dt->t=v.t;
				if (GET_BASE_REGISTER(v.r)==REGISTER_CONST){
					i_dt->r|=REGISTER_KNOWN_VALUE;
					i_dt->e.v=v.e.v;
					if (!v.e.v){
						LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\txor ");
						_write_identifier_data(os,i_dt);
						LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,',');
						_write_identifier_data(os,i_dt);
						LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
					}
					else{
						LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tmov ");
						_write_identifier_data(os,i_dt);
						LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,',');
						_write_identifier_data(os,&v);
						LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
					}
				}
				else if (v.r==REGISTER_STACK){
					ASSERT(!"Unimplemented",e,UINT32_MAX);
				}
				else{
					if (v.r&REGISTER_KNOWN_VALUE){
						i_dt->r|=REGISTER_KNOWN_VALUE;
						i_dt->e.v=v.e.v;
					}
					LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tmov ");
					_write_identifier_data(os,i_dt);
					LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,',');
					_write_identifier_data(os,&v);
					LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
				}
				if (v.r&REGISTER_TEMPORARY){
					_release_identifier(&v,im);
				}
				return off+eoff+_get_object_size(dta);
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
				off+=_get_object_size(init);
				lll_object_t* cnd=LLL_GET_OBJECT_ARGUMENT(o,off);
				REMOVE_PADDING_DEBUG(cnd,off);
				off+=_get_object_size(cnd);
				uint8_t cnd_t=_get_cond_type(cnd,im);
				if (cnd_t==COMPARE_UNKNOWN){
					if (_write_object_as_assembly(os,init,agd,e)==UINT32_MAX){
						return UINT32_MAX;
					}
					label_t sl=im->nl;
					label_t el=im->nl+1;
					im->nl+=2;
					if (!_write_jump_if_false(os,cnd,agd,el,e)){
						return UINT32_MAX;
					}
					_write_label_define(os,sl);
					while (ac){
						ac--;
						lll_object_t* a=LLL_GET_OBJECT_ARGUMENT(o,off);
						REMOVE_PADDING_DEBUG(a,off);
						uint32_t aoff=_write_object_as_assembly(os,a,agd,e);
						if (aoff==UINT32_MAX){
							return UINT32_MAX;
						}
						off+=aoff;
					}
					if (!_write_jump_if_true(os,cnd,agd,sl,e)){
						return UINT32_MAX;
					}
					_write_label_define(os,el);
				}
				else if (cnd_t==COMPARE_ALWAYS_TRUE){
					if (_write_object_as_assembly(os,init,agd,e)==UINT32_MAX){
						return UINT32_MAX;
					}
					label_t sl=im->nl;
					im->nl++;
					_write_label_define(os,sl);
					while (ac){
						ac--;
						lll_object_t* a=LLL_GET_OBJECT_ARGUMENT(o,off);
						REMOVE_PADDING_DEBUG(a,off);
						uint32_t aoff=_write_object_as_assembly(os,a,agd,e);
						if (aoff==UINT32_MAX){
							return UINT32_MAX;
						}
						off+=aoff;
					}
					LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\tjmp ");
					_write_label(os,sl);
					LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
				}
				else{
					while (ac){
						ac--;
						off+=_get_object_size(LLL_GET_OBJECT_ARGUMENT(o,off));
					}
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_AND:
		case LLL_OBJECT_TYPE_OR:
		case LLL_OBJECT_TYPE_NOT:
		case LLL_OBJECT_TYPE_ADD:
		case LLL_OBJECT_TYPE_SUB:
		case LLL_OBJECT_TYPE_MULT:
		case LLL_OBJECT_TYPE_DIV:
		case LLL_OBJECT_TYPE_FLOOR_DIV:
		case LLL_OBJECT_TYPE_MOD:
		case LLL_OBJECT_TYPE_BIT_AND:
		case LLL_OBJECT_TYPE_BIT_OR:
		case LLL_OBJECT_TYPE_BIT_XOR:
		case LLL_OBJECT_TYPE_BIT_NOT:
		case LLL_OBJECT_TYPE_DIV_MOD:
		case LLL_OBJECT_TYPE_POW:
		case LLL_OBJECT_TYPE_ROOT:
		case LLL_OBJECT_TYPE_FLOOR_ROOT:
		case LLL_OBJECT_TYPE_LOG:
		case LLL_OBJECT_TYPE_FLOOR_LOG:
			{
				lll_arg_count_t ac=*LLL_GET_OBJECT_ARGUMENT_COUNT(o);
				uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
				while (ac){
					ac--;
					uint32_t aoff=_write_object_as_assembly(os,LLL_GET_OBJECT_ARGUMENT(o,off),agd,e);
					if (aoff==UINT32_MAX){
						return UINT32_MAX;
					}
					off+=aoff;
				}
				return off+eoff;
			}
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
					sc--;
					lll_object_t* s=LLL_GET_OBJECT_STATEMENT(o,off);
					uint32_t aoff=_write_object_as_assembly(os,s,agd,e);
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
				uint32_t off=_write_object_as_assembly(os,LLL_GET_DEBUG_OBJECT_CHILD(dbg,sz),agd,e);
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
		LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,ASSEMBLY_INIT_CODE);
		assembly_generator_data_t agd={
			{
				.rm=ALL_REGISTER_AVAIBLE_MASK,
				.nl=0
			},
			{
				NULL,
				0
			}
		};
		uint32_t off=0;
		for (uint32_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
			agd.im.off[i]=off;
			off+=c_dt->i_dt.s[i].l;
		}
		agd.im.off[LLL_MAX_SHORT_IDENTIFIER_LENGTH]=off;
		off+=c_dt->i_dt.ill;
		agd.im.dt=malloc(off*sizeof(identifier_data_t));
		REPEAT_BYTE_SET((unsigned char*)agd.im.dt,REGISTER_NONE,off*sizeof(identifier_data_t)/sizeof(uint8_t));
		if (_write_object_as_assembly(os,c_dt->h,&agd,e)==UINT32_MAX){
			free(agd.im.dt);
			for (uint32_t i=0;i<agd.st.l;i++){
				free(*(agd.st.dt+i));
			}
			if (agd.st.dt){
				free(agd.st.dt);
			}
			return LLL_RETURN_ERROR;
		}
		free(agd.im.dt);
		LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,ASSEMBLY_EXIT_CODE);
		if (agd.f&ASSEMBLY_GENERATOR_DATA_FLAG_PRINT_STRING){
			LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,".__print_str:\n\tpush rsi\n\tmov rsi,rcx\n.__print_str_loop:\n\tlodsb\n\ttest al,al\n\tje .__print_str_end\n\tmov ecx,eax\n\tcall putchar\n\tjmp .__print_str_loop\n.__print_str_end:\n\tpop rsi\n\tret\n");
		}
		if (agd.f&ASSEMBLY_GENERATOR_DATA_FLAG_PRINT_INT32){
			LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,".__print_int32:\n\tpush rsi\n\tpush rbx\n\tsub rsp,0x38\n\tmov ebx,ecx\n\ttest ecx,ecx\n\tjs .__print_int32_neg\n\tje .__print_int32_zero\n.__print_int32_loop_setup:\n\txor edx,edx\n\tmov r9d,0xcccccccd\n.__print_int32_loop:\n\tmov eax,ebx\n\tmovzx r8d,dl\n\tadd edx,0x1\n\timul rax,r9\n\tshr rax,0x23\n\tlea ecx,[rax+rax*4]\n\tadd ecx,ecx\n\tsub ebx,ecx\n\tadd ebx,0x30\n\tmov BYTE [rsp+r8*1+0x26],bl\n\tmov ebx,eax\n\ttest eax,eax\n\tjne .__print_int32_loop\n\tlea rsi,[rsp+0x26]\n\tmov rbx,r8\n\tadd rbx,rsi\n.__print_int32_loop2:\n\tmovsx ecx,BYTE [rbx]\n\tcall putchar\n\tmov rax,rbx\n\tsub rbx,0x1\n\tcmp rsi,rax\n\tjne .__print_int32_loop2\n\tadd rsp,0x38\n\tpop rbx\n\tpop rsi\n\tret\n.__print_int32_neg:\n\tmov ecx,0x2d\n\tneg ebx\n\tcall putchar\n\tjmp .__print_int32_loop_setup\n.__print_int32_zero:\n\tmov ecx,0x30\n\tadd rsp,0x38\n\tpop rbx\n\tpop rsi\n\tjmp putchar\n");
		}
		if (agd.st.l){
			LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"section .data\n");
		}
		for (uint32_t i=0;i<agd.st.l;i++){
			LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\t_str_");
			uint8_t j=(FIND_LAST_SET_BIT(i)+4)&0x3c;
			while (j){
				j-=4;
				uint8_t c=(i>>j)&0xf;
				if (c>9){
					LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,87+c);
				}
				else{
					LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,48+c);
				}
			}
			LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,":db ");
			char* s=(*(agd.st.dt+i))->v;
			for (uint32_t j=0;j<(*(agd.st.dt+i))->sz;j++){
				uint8_t c=(uint8_t)(*s);
				s++;
				if (j){
					LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,',');
				}
				LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"0x");
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,((c>>4)<10?48:87)+(c>>4));
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,((c&0xf)<10?48:87)+(c&0xf));
			}
			if (!(*(agd.st.dt+i))->nb){
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,',');
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'0');
			}
			LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,'\n');
			free(*(agd.st.dt+i));
		}
		if (agd.st.dt){
			free(agd.st.dt);
		}
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
