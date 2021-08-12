#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/core.h>
#include <sll/operator.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdint.h>
#include <string.h>



#define COMBINE_TYPES(a,b) (((uint16_t)(a)<<8)|(b))
#define COMBINED_TYPE_II COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_INT,SLL_RUNTIME_OBJECT_TYPE_INT)
#define COMBINED_TYPE_IF COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_INT,SLL_RUNTIME_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_IC COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_INT,SLL_RUNTIME_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_IS COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_INT,SLL_RUNTIME_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_FI COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_FLOAT,SLL_RUNTIME_OBJECT_TYPE_INT)
#define COMBINED_TYPE_FF COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_FLOAT,SLL_RUNTIME_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_FC COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_FLOAT,SLL_RUNTIME_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_FS COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_FLOAT,SLL_RUNTIME_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_CI COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_CHAR,SLL_RUNTIME_OBJECT_TYPE_INT)
#define COMBINED_TYPE_CF COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_CHAR,SLL_RUNTIME_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_CC COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_CHAR,SLL_RUNTIME_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_CS COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_CHAR,SLL_RUNTIME_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_SI COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_RUNTIME_OBJECT_TYPE_INT)
#define COMBINED_TYPE_SF COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_RUNTIME_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_SC COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_RUNTIME_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_SS COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_RUNTIME_OBJECT_TYPE_STRING)



__SLL_OPERATOR_UNARY(inc){
	switch (SLL_RUNTIME_OBJECT_GET_TYPE(a)){
		case SLL_RUNTIME_OBJECT_TYPE_INT:
			o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
			o->dt.i=a->dt.i+1;
			return;
		case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
			SLL_ASSERT(!"Unimplemented");
		case SLL_RUNTIME_OBJECT_TYPE_CHAR:
			SLL_ASSERT(!"Unimplemented");
		case SLL_RUNTIME_OBJECT_TYPE_STRING:
			SLL_ASSERT(!"Unimplemented");
		default:
			UNREACHABLE();
	}
}



__SLL_OPERATOR_UNARY(dec){
	switch (SLL_RUNTIME_OBJECT_GET_TYPE(a)){
		case SLL_RUNTIME_OBJECT_TYPE_INT:
			o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
			o->dt.i=a->dt.i-1;
			return;
		case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
			SLL_ASSERT(!"Unimplemented");
		case SLL_RUNTIME_OBJECT_TYPE_CHAR:
			SLL_ASSERT(!"Unimplemented");
		case SLL_RUNTIME_OBJECT_TYPE_STRING:
			SLL_ASSERT(!"Unimplemented");
		default:
			UNREACHABLE();
	}
}



__SLL_OPERATOR(add){
	switch (COMBINE_TYPES(SLL_RUNTIME_OBJECT_GET_TYPE(a),SLL_RUNTIME_OBJECT_GET_TYPE(b))){
		case COMBINED_TYPE_II:
			o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
			o->dt.i=a->dt.i+b->dt.i;
			return;
		case COMBINED_TYPE_IF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_IC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_IS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SS:
			{
				sll_string_t* sa=a->dt.s;
				sll_string_t* sb=b->dt.s;
				sll_string_t* s=sll_string_create(sa->l+sb->l);
				s->rc=1;
				s->c=sa->c^sb->c;
				memcpy(s->v,sa->v,sa->l);
				memcpy(s->v+sa->l,sb->v,sb->l);
				o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
				o->dt.s=s;
				return;
			}
		default:
			UNREACHABLE();
	}
}



__SLL_OPERATOR(sub){
	switch (COMBINE_TYPES(SLL_RUNTIME_OBJECT_GET_TYPE(a),SLL_RUNTIME_OBJECT_GET_TYPE(b))){
		case COMBINED_TYPE_II:
			o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
			o->dt.i=a->dt.i-b->dt.i;
			return;
		case COMBINED_TYPE_IF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_IC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_IS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SS:
			SLL_ASSERT(!"Unimplemented");
		default:
			UNREACHABLE();
	}
}



__SLL_OPERATOR(mult){
	switch (COMBINE_TYPES(SLL_RUNTIME_OBJECT_GET_TYPE(a),SLL_RUNTIME_OBJECT_GET_TYPE(b))){
		case COMBINED_TYPE_II:
			o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
			o->dt.i=a->dt.i*b->dt.i;
			return;
		case COMBINED_TYPE_IF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_IC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_IS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SS:
			SLL_ASSERT(!"Unimplemented");
		default:
			UNREACHABLE();
	}
}



__SLL_OPERATOR(div){
	switch (COMBINE_TYPES(SLL_RUNTIME_OBJECT_GET_TYPE(a),SLL_RUNTIME_OBJECT_GET_TYPE(b))){
		case COMBINED_TYPE_II:
			o->t=SLL_RUNTIME_OBJECT_TYPE_FLOAT;
			o->dt.f=((sll_float_t)a->dt.i)/b->dt.i;
			return;
		case COMBINED_TYPE_IF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_IC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_IS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SS:
			SLL_ASSERT(!"Unimplemented");
		default:
			UNREACHABLE();
	}
}



__SLL_OPERATOR(floor_div){
	switch (COMBINE_TYPES(SLL_RUNTIME_OBJECT_GET_TYPE(a),SLL_RUNTIME_OBJECT_GET_TYPE(b))){
		case COMBINED_TYPE_II:
			o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
			o->dt.i=a->dt.i/b->dt.i;
			return;
		case COMBINED_TYPE_IF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_IC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_IS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SS:
			SLL_ASSERT(!"Unimplemented");
		default:
			UNREACHABLE();
	}
}



__SLL_OPERATOR(mod){
	switch (COMBINE_TYPES(SLL_RUNTIME_OBJECT_GET_TYPE(a),SLL_RUNTIME_OBJECT_GET_TYPE(b))){
		case COMBINED_TYPE_II:
			o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
			o->dt.i=a->dt.i%b->dt.i;
			return;
		case COMBINED_TYPE_IF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_IC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_IS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SS:
			SLL_ASSERT(!"Unimplemented");
		default:
			UNREACHABLE();
	}
}



__SLL_OPERATOR(and){
	switch (COMBINE_TYPES(SLL_RUNTIME_OBJECT_GET_TYPE(a),SLL_RUNTIME_OBJECT_GET_TYPE(b))){
		case COMBINED_TYPE_II:
			o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
			o->dt.i=a->dt.i&b->dt.i;
			return;
		case COMBINED_TYPE_IF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_IC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_IS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SS:
			SLL_ASSERT(!"Unimplemented");
		default:
			UNREACHABLE();
	}
}



__SLL_OPERATOR(or){
	switch (COMBINE_TYPES(SLL_RUNTIME_OBJECT_GET_TYPE(a),SLL_RUNTIME_OBJECT_GET_TYPE(b))){
		case COMBINED_TYPE_II:
			o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
			o->dt.i=a->dt.i|b->dt.i;
			return;
		case COMBINED_TYPE_IF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_IC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_IS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SS:
			SLL_ASSERT(!"Unimplemented");
		default:
			UNREACHABLE();
	}
}



__SLL_OPERATOR(xor){
	switch (COMBINE_TYPES(SLL_RUNTIME_OBJECT_GET_TYPE(a),SLL_RUNTIME_OBJECT_GET_TYPE(b))){
		case COMBINED_TYPE_II:
			o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
			o->dt.i=a->dt.i^b->dt.i;
			return;
		case COMBINED_TYPE_IF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_IC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_IS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_FS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_CS:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SI:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SF:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SC:
			SLL_ASSERT(!"Unimplemented");
		case COMBINED_TYPE_SS:
			SLL_ASSERT(!"Unimplemented");
		default:
			UNREACHABLE();
	}
}



__SLL_OPERATOR_UNARY(inv){
	switch (SLL_RUNTIME_OBJECT_GET_TYPE(a)){
		case SLL_RUNTIME_OBJECT_TYPE_INT:
			o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
			o->dt.i=~a->dt.i;
			return;
		case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
			SLL_ASSERT(!"Unimplemented");
		case SLL_RUNTIME_OBJECT_TYPE_CHAR:
			SLL_ASSERT(!"Unimplemented");
		case SLL_RUNTIME_OBJECT_TYPE_STRING:
			SLL_ASSERT(!"Unimplemented");
		default:
			UNREACHABLE();
	}
}
