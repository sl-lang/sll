#include <sll/_sll_internal.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/node.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdio.h>



#define GENERATE_STATIC_STRING(s,wf) sll_file_write((wf),SLL_CHAR((s)),sizeof(s)/sizeof(char)-1)
#define GENERATE_INT_SIGN(v,wf) \
	do{ \
		int64_t __v=(v); \
		if (__v>=0){ \
			sll_file_write_char((wf),'+'); \
		} \
		_generate_int(__v,(wf)); \
	} while(0)



static void _generate_int(int64_t v,sll_file_t* wf){
	if (!v){
		sll_file_write_char(wf,'0');
		return;
	}
	if (v<0){
		v=-v;
		sll_file_write_char(wf,'-');
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
		sll_file_write_char(wf,bf[i]+48);
	}
}



static void _generate_float(double v,sll_file_t* wf){
	char bf[128];
	int sz=snprintf(bf,128,"%.16lg",v);
	sll_file_write(wf,(uint8_t*)bf,sz*sizeof(char));
}



static void _generate_identifier(sll_identifier_index_t ii,const code_generation_data_t* cg_dt,sll_file_t* wf){
	sll_identifier_list_length_t j=SLL_IDENTIFIER_GET_ARRAY_ID(ii);
	if (j==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
		sll_string_t* s=cg_dt->c_dt->st.dt+(cg_dt->c_dt->idt.il+SLL_IDENTIFIER_GET_ARRAY_INDEX(ii))->i;
		sll_file_write(wf,s->v,s->l);
	}
	else{
		sll_file_write(wf,(cg_dt->c_dt->st.dt+(cg_dt->c_dt->idt.s[j].dt+SLL_IDENTIFIER_GET_ARRAY_INDEX(ii))->i)->v,j+1);
	}
}



static const sll_node_t* _generate_code_internal(const code_generation_data_t* cg_dt,const sll_node_t* o,sll_string_length_t lvl,sll_file_t* wf){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DEBUG_DATA||o->t==NODE_TYPE_CHANGE_STACK){
		o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	if (SLL_IS_OBJECT_TYPE_NOT_TYPE(o)&&o->t!=SLL_NODE_TYPE_VAR_ACCESS&&o->t!=SLL_NODE_TYPE_OPERATION_LIST&&o->t!=SLL_NODE_TYPE_DEBUG_DATA){
		sll_file_write_char(wf,'(');
	}
	switch (o->t){
		case SLL_NODE_TYPE_UNKNOWN:
			return o+1;
		case SLL_NODE_TYPE_CHAR:
			{
				sll_file_write_char(wf,'\'');
				sll_char_t c=o->dt.c;
				if (c=='\''||c=='"'||c=='\\'){
					sll_file_write_char(wf,'\\');
				}
				else if (c=='\t'){
					sll_file_write_char(wf,'\\');
					c='t';
				}
				else if (c=='\n'){
					sll_file_write_char(wf,'\\');
					c='n';
				}
				else if (c=='\v'){
					sll_file_write_char(wf,'\\');
					c='v';
				}
				else if (c=='\f'){
					sll_file_write_char(wf,'\\');
					c='f';
				}
				else if (c=='\r'){
					sll_file_write_char(wf,'\\');
					c='r';
				}
				else if (c<32||c>126){
					sll_file_write_char(wf,'\\');
					sll_file_write_char(wf,'x');
					sll_file_write_char(wf,(((uint8_t)c)>>4)+(((uint8_t)c)>159?87:48));
					c=(c&0xf)+((c&0xf)>9?87:48);
				}
				sll_file_write_char(wf,c);
				sll_file_write_char(wf,'\'');
				return o+1;
			}
		case SLL_NODE_TYPE_INT:
			_generate_int(o->dt.i,wf);
			return o+1;
		case SLL_NODE_TYPE_FLOAT:
			_generate_float(o->dt.f,wf);
			return o+1;
		case SLL_NODE_TYPE_STRING:
			{
				sll_file_write_char(wf,'"');
				sll_string_t* s=cg_dt->c_dt->st.dt+o->dt.s;
				for (sll_string_length_t i=0;i<s->l;i++){
					sll_char_t c=s->v[i];
					if (c=='\''||c=='"'||c=='\\'){
						sll_file_write_char(wf,'\\');
					}
					else if (c=='\t'){
						sll_file_write_char(wf,'\\');
						c='t';
					}
					else if (c=='\n'){
						sll_file_write_char(wf,'\\');
						c='n';
					}
					else if (c=='\v'){
						sll_file_write_char(wf,'\\');
						c='v';
					}
					else if (c=='\f'){
						sll_file_write_char(wf,'\\');
						c='f';
					}
					else if (c=='\r'){
						sll_file_write_char(wf,'\\');
						c='r';
					}
					else if (c<32||c>126){
						sll_file_write_char(wf,'\\');
						sll_file_write_char(wf,'x');
						sll_file_write_char(wf,(((uint8_t)c)>>4)+(((uint8_t)c)>159?87:48));
						c=(c&0xf)+((c&0xf)>9?87:48);
					}
					sll_file_write_char(wf,c);
				}
				sll_file_write_char(wf,'"');
				return o+1;
			}
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_file_write_char(wf,'[');
				sll_array_length_t al=o->dt.al;
				o++;
				for (sll_array_length_t i=0;i<al;i++){
					if (i){
						sll_file_write_char(wf,' ');
					}
					o=_generate_code_internal(cg_dt,o,lvl,wf);
				}
				sll_file_write_char(wf,']');
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_file_write_char(wf,'<');
				sll_map_length_t ml=o->dt.ml;
				o++;
				for (sll_map_length_t i=0;i<ml;i++){
					if (i){
						sll_file_write_char(wf,' ');
					}
					o=_generate_code_internal(cg_dt,o,lvl,wf);
				}
				sll_file_write_char(wf,'>');
				return o;
			}
		case SLL_NODE_TYPE_IDENTIFIER:
			_generate_identifier(o->dt.id,cg_dt,wf);
			return o+1;
		case SLL_NODE_TYPE_FIELD:
			{
				sll_string_t* s=cg_dt->c_dt->st.dt+o->dt.s;
				sll_file_write(wf,s->v,s->l);
				return o+1;
			}
		case SLL_NODE_TYPE_FUNCTION_ID:
			_generate_int(o->dt.fn_id,wf);
			return o+1;
		case SLL_NODE_TYPE_PRINT:
			GENERATE_STATIC_STRING(":>",wf);
			break;
		case SLL_NODE_TYPE_AND:
			GENERATE_STATIC_STRING("&&",wf);
			break;
		case SLL_NODE_TYPE_OR:
			GENERATE_STATIC_STRING("||",wf);
			break;
		case SLL_NODE_TYPE_NOT:
			sll_file_write_char(wf,'!');
			break;
		case SLL_NODE_TYPE_BOOL:
			GENERATE_STATIC_STRING("!!",wf);
			break;
		case SLL_NODE_TYPE_ASSIGN:
			GENERATE_STATIC_STRING("=",wf);
			break;
		case SLL_NODE_TYPE_FUNC:
			{
				GENERATE_STATIC_STRING(",,,",wf);
				const sll_function_t* f=*(cg_dt->c_dt->ft.dt+o->dt.fn.id);
				if (f->nm!=SLL_MAX_STRING_INDEX){
					GENERATE_STATIC_STRING("|#",wf);
					sll_file_write(wf,(cg_dt->c_dt->st.dt+f->nm)->v,(cg_dt->c_dt->st.dt+f->nm)->l);
					GENERATE_STATIC_STRING("#|",wf);
				}
				for (sll_arg_count_t i=0;i<f->al;i++){
					sll_file_write_char(wf,' ');
					_generate_identifier(f->a[i],cg_dt,wf);
				}
				sll_file_write_char(wf,'\n');
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					sll_file_write_char_count(wf,'\t',lvl+1);
					o=_generate_code_internal(cg_dt,o,lvl+1,wf);
					sll_file_write_char(wf,'\n');
				}
				sll_file_write_char_count(wf,'\t',lvl);
				sll_file_write_char(wf,')');
				return o;
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				if (cg_dt->i_ft&&o->dt.fn.id<cg_dt->i_ft->l){
					const sll_internal_function_t* f=*(cg_dt->i_ft->dt+o->dt.fn.id);
					GENERATE_STATIC_STRING("... \"",wf);
					sll_file_write(wf,f->nm.v,f->nm.l);
					sll_file_write_char(wf,'\"');
				}
				else{
					GENERATE_STATIC_STRING(",",wf);
				}
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					sll_file_write_char(wf,' ');
					o=_generate_code_internal(cg_dt,o,lvl,wf);
				}
				if (!cg_dt->i_ft||o->dt.fn.id>=cg_dt->i_ft->l){
					sll_file_write_char(wf,' ');
					_generate_int(o->dt.fn.id,wf);
				}
				sll_file_write_char(wf,')');
				return o;
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC_LOAD:
			GENERATE_STATIC_STRING("...",wf);
			break;
		case SLL_NODE_TYPE_INLINE_FUNC:
			GENERATE_STATIC_STRING("***",wf);
		case SLL_NODE_TYPE_COMMA:
			{
				if (o->t==SLL_NODE_TYPE_COMMA){
					sll_file_write_char(wf,',');
				}
				sll_arg_count_t ac=o->dt.ac;
				o++;
				if (!ac){
					sll_file_write_char(wf,')');
					return o;
				}
				sll_file_write_char(wf,'\n');
				for (sll_arg_count_t i=0;i<ac;i++){
					sll_file_write_char_count(wf,'\t',lvl+1);
					o=_generate_code_internal(cg_dt,o,lvl+1,wf);
					sll_file_write_char(wf,'\n');
				}
				sll_file_write_char_count(wf,'\t',lvl);
				sll_file_write_char(wf,')');
				return o;
			}
		case SLL_NODE_TYPE_CALL:
			{
				GENERATE_STATIC_STRING("<-",wf);
				sll_arg_count_t ac=o->dt.ac;
				o++;
				if (ac){
					while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DEBUG_DATA||o->t==NODE_TYPE_CHANGE_STACK){
						o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
					}
					sll_arg_count_t i=0;
					if (cg_dt->i_ft&&o->t==SLL_NODE_TYPE_INT&&o->dt.i<0){
						sll_function_index_t j=(sll_function_index_t)(~(o->dt.i));
						if (j<cg_dt->i_ft->l){
							const sll_internal_function_t* f=*(cg_dt->i_ft->dt+j);
							GENERATE_STATIC_STRING(" (... \"",wf);
							sll_file_write(wf,f->nm.v,f->nm.l);
							GENERATE_STATIC_STRING("\")",wf);
							i++;
							o++;
						}
					}
					for (;i<ac;i++){
						sll_file_write_char(wf,' ');
						o=_generate_code_internal(cg_dt,o,lvl,wf);
					}
				}
				sll_file_write_char(wf,')');
				return o;
			}
		case SLL_NODE_TYPE_IF:
			{
				sll_file_write_char(wf,'?');
				sll_arg_count_t ac=o->dt.ac;
				o++;
				if (!ac){
					sll_file_write_char(wf,')');
					return o;
				}
				if (ac==1){
					sll_file_write_char(wf,' ');
					o=_generate_code_internal(cg_dt,o,lvl,wf);
					sll_file_write_char(wf,')');
					return o;
				}
				if (ac==2){
					sll_file_write_char(wf,' ');
					o=_generate_code_internal(cg_dt,o,lvl,wf);
					while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DEBUG_DATA||o->t==NODE_TYPE_CHANGE_STACK){
						o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
					}
					if (o->t==SLL_NODE_TYPE_OPERATION_LIST){
						sll_file_write_char(wf,' ');
						o=_generate_code_internal(cg_dt,o,lvl,wf);
					}
					else{
						sll_file_write_char(wf,'\n');
						sll_file_write_char_count(wf,'\t',lvl+1);
						o=_generate_code_internal(cg_dt,o,lvl+1,wf);
						sll_file_write_char(wf,'\n');
						sll_file_write_char_count(wf,'\t',lvl);
					}
					sll_file_write_char(wf,')');
					return o;
				}
				if (ac==3){
					sll_file_write_char(wf,' ');
					o=_generate_code_internal(cg_dt,o,lvl,wf);
					while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DEBUG_DATA||o->t==NODE_TYPE_CHANGE_STACK){
						o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
					}
					if (o->t==SLL_NODE_TYPE_OPERATION_LIST){
						sll_file_write_char(wf,' ');
						o=_generate_code_internal(cg_dt,o,lvl,wf);
						sll_file_write_char(wf,' ');
						o=_generate_code_internal(cg_dt,o,lvl,wf);
					}
					else{
						sll_file_write_char(wf,'\n');
						sll_file_write_char_count(wf,'\t',lvl+1);
						o=_generate_code_internal(cg_dt,o,lvl+1,wf);
						sll_file_write_char(wf,'\n');
						sll_file_write_char_count(wf,'\t',lvl+1);
						o=_generate_code_internal(cg_dt,o,lvl+1,wf);
						sll_file_write_char(wf,'\n');
						sll_file_write_char_count(wf,'\t',lvl);
					}
					sll_file_write_char(wf,')');
					return o;
				}
				sll_bool_t e=ac&1;
				ac>>=1;
				sll_file_write_char(wf,'\n');
				while (ac){
					ac--;
					sll_file_write_char_count(wf,'\t',lvl+1);
					o=_generate_code_internal(cg_dt,o,lvl+1,wf);
					sll_file_write_char(wf,' ');
					o=_generate_code_internal(cg_dt,o,lvl+1,wf);
					sll_file_write_char(wf,'\n');
				}
				if (e){
					sll_file_write_char_count(wf,'\t',lvl+1);
					o=_generate_code_internal(cg_dt,o,lvl+1,wf);
					sll_file_write_char(wf,'\n');
				}
				sll_file_write_char_count(wf,'\t',lvl);
				sll_file_write_char(wf,')');
				return o;
			}
		case SLL_NODE_TYPE_INLINE_IF:
			GENERATE_STATIC_STRING("?:",wf);
			break;
		case SLL_NODE_TYPE_SWITCH:
			{
				GENERATE_STATIC_STRING("??",wf);
				sll_arg_count_t ac=o->dt.ac;
				o++;
				if (!ac){
					sll_file_write_char(wf,')');
					return o;
				}
				ac--;
				sll_file_write_char(wf,' ');
				o=_generate_code_internal(cg_dt,o,lvl,wf);
				if (!ac){
					sll_file_write_char(wf,')');
					return o;
				}
				sll_bool_t d=ac&1;
				ac>>=1;
				sll_file_write_char(wf,'\n');
				while (ac){
					ac--;
					sll_file_write_char_count(wf,'\t',lvl+1);
					o=_generate_code_internal(cg_dt,o,lvl+1,wf);
					sll_file_write_char(wf,' ');
					o=_generate_code_internal(cg_dt,o,lvl+1,wf);
					sll_file_write_char(wf,'\n');
				}
				if (d){
					sll_file_write_char_count(wf,'\t',lvl+1);
					o=_generate_code_internal(cg_dt,o,lvl+1,wf);
					sll_file_write_char(wf,'\n');
				}
				sll_file_write_char_count(wf,'\t',lvl);
				sll_file_write_char(wf,')');
				return o;
			}
		case SLL_NODE_TYPE_FOR:
			GENERATE_STATIC_STRING("->",wf);
		case SLL_NODE_TYPE_WHILE:
			{
				if (o->t==SLL_NODE_TYPE_WHILE){
					GENERATE_STATIC_STRING(">-",wf);
				}
				sll_arg_count_t ac=o->dt.l.ac;
				o++;
				if (ac<3){
					for (sll_arg_count_t i=0;i<ac;i++){
						sll_file_write_char(wf,' ');
						o=_generate_code_internal(cg_dt,o,lvl,wf);
					}
					sll_file_write_char(wf,')');
					return o;
				}
				for (sll_arg_count_t i=0;i<2;i++){
					sll_file_write_char(wf,' ');
					o=_generate_code_internal(cg_dt,o,lvl,wf);
				}
				while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DEBUG_DATA||o->t==NODE_TYPE_CHANGE_STACK){
					o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				sll_bool_t op_l=0;
				if (ac==3&&o->t==SLL_NODE_TYPE_OPERATION_LIST){
					op_l=1;
					ac=o->dt.ac;
					o++;
					GENERATE_STATIC_STRING(" {",wf);
				}
				else{
					ac-=2;
				}
				sll_file_write_char(wf,'\n');
				for (sll_arg_count_t i=0;i<ac;i++){
					sll_file_write_char_count(wf,'\t',lvl+1);
					o=_generate_code_internal(cg_dt,o,lvl+1,wf);
					sll_file_write_char(wf,'\n');
				}
				sll_file_write_char_count(wf,'\t',lvl);
				if (op_l){
					sll_file_write_char(wf,'}');
				}
				sll_file_write_char(wf,')');
				return o;
			}
		case SLL_NODE_TYPE_LOOP:
			{
				GENERATE_STATIC_STRING("><",wf);
				sll_arg_count_t ac=o->dt.l.ac;
				o++;
				if (!ac){
					sll_file_write_char(wf,')');
					return o;
				}
				for (sll_arg_count_t i=0;i<ac;i++){
					sll_file_write_char(wf,' ');
					o=_generate_code_internal(cg_dt,o,lvl,wf);
				}
				sll_file_write_char(wf,')');
				return o;
			}
		case SLL_NODE_TYPE_INC:
			GENERATE_STATIC_STRING("++",wf);
			break;
		case SLL_NODE_TYPE_DEC:
			GENERATE_STATIC_STRING("--",wf);
			break;
		case SLL_NODE_TYPE_ADD:
			sll_file_write_char(wf,'+');
			break;
		case SLL_NODE_TYPE_SUB:
			sll_file_write_char(wf,'-');
			break;
		case SLL_NODE_TYPE_MULT:
			sll_file_write_char(wf,'*');
			break;
		case SLL_NODE_TYPE_DIV:
			sll_file_write_char(wf,'/');
			break;
		case SLL_NODE_TYPE_FLOOR_DIV:
			GENERATE_STATIC_STRING("//",wf);
			break;
		case SLL_NODE_TYPE_MOD:
			sll_file_write_char(wf,'%');
			break;
		case SLL_NODE_TYPE_BIT_AND:
			sll_file_write_char(wf,'&');
			break;
		case SLL_NODE_TYPE_BIT_OR:
			sll_file_write_char(wf,'|');
			break;
		case SLL_NODE_TYPE_BIT_XOR:
			sll_file_write_char(wf,'^');
			break;
		case SLL_NODE_TYPE_BIT_NOT:
			sll_file_write_char(wf,'~');
			break;
		case SLL_NODE_TYPE_BIT_RSHIFT:
			GENERATE_STATIC_STRING(">>",wf);
			break;
		case SLL_NODE_TYPE_BIT_LSHIFT:
			GENERATE_STATIC_STRING("<<",wf);
			break;
		case SLL_NODE_TYPE_LESS:
			sll_file_write_char(wf,'<');
			break;
		case SLL_NODE_TYPE_LESS_EQUAL:
			GENERATE_STATIC_STRING("<=",wf);
			break;
		case SLL_NODE_TYPE_EQUAL:
			GENERATE_STATIC_STRING("==",wf);
			break;
		case SLL_NODE_TYPE_NOT_EQUAL:
			GENERATE_STATIC_STRING("!=",wf);
			break;
		case SLL_NODE_TYPE_MORE:
			sll_file_write_char(wf,'>');
			break;
		case SLL_NODE_TYPE_MORE_EQUAL:
			GENERATE_STATIC_STRING(">=",wf);
			break;
		case SLL_NODE_TYPE_STRICT_EQUAL:
			GENERATE_STATIC_STRING("===",wf);
			break;
		case SLL_NODE_TYPE_STRICT_NOT_EQUAL:
			GENERATE_STATIC_STRING("!==",wf);
			break;
		case SLL_NODE_TYPE_LENGTH:
			sll_file_write_char(wf,'$');
			break;
		case SLL_NODE_TYPE_ACCESS:
			sll_file_write_char(wf,':');
			break;
		case SLL_NODE_TYPE_VAR_ACCESS:
			{
				sll_arg_count_t l=o->dt.ac;
				SLL_ASSERT(l>1);
				o++;
				while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DEBUG_DATA||o->t==NODE_TYPE_CHANGE_STACK){
					o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
				}
				SLL_ASSERT(o->t==SLL_NODE_TYPE_IDENTIFIER);
				_generate_identifier(o->dt.id,cg_dt,wf);
				o++;
				l--;
				do{
					l--;
					sll_file_write_char(wf,'$');
					while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DEBUG_DATA||o->t==NODE_TYPE_CHANGE_STACK){
						o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
					}
					SLL_ASSERT(o->t==SLL_NODE_TYPE_FIELD);
					sll_string_t* s=cg_dt->c_dt->st.dt+o->dt.s;
					sll_file_write(wf,s->v,s->l);
					o++;
				} while (l);
				return o;
			}
		case SLL_NODE_TYPE_CAST:
			GENERATE_STATIC_STRING("::",wf);
			break;
		case SLL_NODE_TYPE_TYPEOF:
			GENERATE_STATIC_STRING(":?",wf);
			break;
		case SLL_NODE_TYPE_DECL:
			{
				GENERATE_STATIC_STRING("&:",wf);
				sll_arg_count_t l=o->dt.ac;
				o++;
				if (!l){
					sll_file_write_char(wf,')');
					return o;
				}
				sll_file_write_char(wf,'\n');
				sll_bool_t e=l&1;
				l>>=1;
				while (l){
					l--;
					sll_file_write_char_count(wf,'\t',lvl+1);
					o=_generate_code_internal(cg_dt,o,lvl+1,wf);
					sll_file_write_char(wf,' ');
					o=_generate_code_internal(cg_dt,o,lvl+1,wf);
					sll_file_write_char(wf,'\n');
				}
				if (e){
					sll_file_write_char_count(wf,'\t',lvl+1);
					o=_generate_code_internal(cg_dt,o,lvl+1,wf);
					sll_file_write_char(wf,'\n');
				}
				sll_file_write_char_count(wf,'\t',lvl);
				sll_file_write_char(wf,')');
				return o;
			}
		case SLL_NODE_TYPE_DECL_COPY:
			{
				GENERATE_STATIC_STRING("&:",wf);
				sll_object_type_initializer_t* oi=*(cg_dt->c_dt->ot_it.dt+o->dt.ot);
				SLL_ASSERT(oi->l);
				sll_file_write_char(wf,'\n');
				for (sll_arg_count_t i=0;i<oi->l;i++){
					sll_file_write_char_count(wf,'\t',lvl+1);
					if (oi->dt[i].t&SLL_OBJECT_FLAG_CONSTANT){
						sll_file_write_char(wf,'-');
						_generate_int(SLL_OBJECT_GET_TYPE_MASK(oi->dt[i].t)+1,wf);
					}
					else{
						_generate_int(oi->dt[i].t,wf);
					}
					sll_file_write_char(wf,' ');
					sll_string_t* s=cg_dt->c_dt->st.dt+oi->dt[i].f;
					sll_file_write(wf,s->v,s->l);
					sll_file_write_char(wf,'\n');
				}
				sll_file_write_char_count(wf,'\t',lvl);
				sll_file_write_char(wf,')');
				return o+1;
			}
		case SLL_NODE_TYPE_NEW:
			sll_file_write_char(wf,'.');
			break;
		case SLL_NODE_TYPE_BREAK:
			sll_file_write_char(wf,'@');
			break;
		case SLL_NODE_TYPE_CONTINUE:
			GENERATE_STATIC_STRING("<<<",wf);
			break;
		case SLL_NODE_TYPE_REF:
			GENERATE_STATIC_STRING("%%",wf);
			break;
		case SLL_NODE_TYPE_RETURN:
			GENERATE_STATIC_STRING("@@",wf);
			break;
		case SLL_NODE_TYPE_EXIT:
			GENERATE_STATIC_STRING("@@@",wf);
			break;
		case SLL_NODE_TYPE_OPERATION_LIST:
			{
				sll_file_write_char(wf,'{');
				sll_arg_count_t ac=o->dt.ac;
				o++;
				if (!ac){
					sll_file_write_char(wf,'}');
					return o;
				}
				sll_file_write_char(wf,'\n');
				for (sll_arg_count_t i=0;i<ac;i++){
					sll_file_write_char_count(wf,'\t',lvl+1);
					o=_generate_code_internal(cg_dt,o,lvl+1,wf);
					sll_file_write_char(wf,'\n');
				}
				sll_file_write_char_count(wf,'\t',lvl);
				sll_file_write_char(wf,'}');
				return o;
			}
		case SLL_NODE_TYPE_DEBUG_DATA:
			return _generate_code_internal(cg_dt,o+1,lvl,wf);
		default:
			SLL_UNREACHABLE();
	}
	sll_arg_count_t ac=o->dt.ac;
	o++;
	for (sll_arg_count_t i=0;i<ac;i++){
		sll_file_write_char(wf,' ');
		o=_generate_code_internal(cg_dt,o,lvl,wf);
	}
	sll_file_write_char(wf,')');
	return o;
}



__SLL_EXTERNAL void sll_write_sll_code(const sll_compilation_data_t* c_dt,const sll_internal_function_table_t* i_ft,sll_file_t* wf){
	code_generation_data_t cg_dt={
		c_dt,
		i_ft
	};
	if (c_dt->h->t==SLL_NODE_TYPE_OPERATION_LIST){
		sll_arg_count_t ac=c_dt->h->dt.ac;
		const sll_node_t* o=c_dt->h+1;
		while (ac){
			o=_generate_code_internal(&cg_dt,o,0,wf);
			sll_file_write_char(wf,'\n');
			ac--;
		}
	}
	else{
		_generate_code_internal(&cg_dt,c_dt->h,0,wf);
		sll_file_write_char(wf,'\n');
	}
}
