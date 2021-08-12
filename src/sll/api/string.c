#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/constants.h>
#include <sll/core.h>
#include <sll/string.h>
#include <sll/types.h>
#include <string.h>



__SLL_FUNC sll_string_t* sll_object_to_string(sll_runtime_object_t* a,sll_arg_count_t ac){
	if (!ac){
		_zero_string.rc++;
		return &_zero_string;
	}
	if (ac==1){
		switch (a->t){
			case SLL_RUNTIME_OBJECT_TYPE_INT:
				SLL_ASSERT(!"Unimplemented");
				return NULL;
			case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
				SLL_ASSERT(!"Unimplemented");
				return NULL;
			case SLL_RUNTIME_OBJECT_TYPE_CHAR:
				SLL_ASSERT(!"Unimplemented");
				return NULL;
			case SLL_RUNTIME_OBJECT_TYPE_STRING:
				a->dt.s->rc++;
				return a->dt.s;
			default:
				UNREACHABLE();
		}
	}
	sll_string_length_t i=0;
	for (sll_arg_count_t j=0;j<ac;j++){
		switch ((a+j)->t){
			case SLL_RUNTIME_OBJECT_TYPE_INT:
				{
					sll_integer_t v=(a+j)->dt.i;
					if (v<0){
						i++;
						v=-v;
					}
					do{
						i++;
						v/=10;
					} while (v);
					break;
				}
			case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
				SLL_ASSERT(!"Unimplemented");
				break;
			case SLL_RUNTIME_OBJECT_TYPE_CHAR:
				i++;
				break;
			case SLL_RUNTIME_OBJECT_TYPE_STRING:
				i+=(a+j)->dt.s->l;
				break;
			default:
				UNREACHABLE();
		}
	}
	sll_string_t* o=sll_string_create(i);
	o->rc=1;
	o->c=0;
	i=0;
	for (sll_arg_count_t j=0;j<ac;j++){
		switch ((a+j)->t){
			case SLL_RUNTIME_OBJECT_TYPE_INT:
				{
					sll_integer_t v=(a+j)->dt.i;
					if (v<0){
						o->v[i]='-';
						o->c^='-';
						v=-v;
						i++;
					}
					uint8_t k=0;
					char bf[20];
					do{
						bf[k]=v%10;
						k++;
						v/=10;
					} while (v);
					while (k){
						k--;
						o->v[i]=bf[k]+48;
						o->c^=o->v[i];
						i++;
					}
					break;
				}
			case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
				SLL_ASSERT(!"Unimplemented");
				break;
			case SLL_RUNTIME_OBJECT_TYPE_CHAR:
				o->v[i]=(a+j)->dt.c;
				o->c^=(a+j)->dt.c;
				i++;
				break;
			case SLL_RUNTIME_OBJECT_TYPE_STRING:
				{
					sll_string_t* s=(a+j)->dt.s;
					memcpy(o->v+i,s->v,s->l);
					o->c^=(a+j)->dt.s->c;
					i+=(a+j)->dt.s->l;
					break;
				}
			default:
				UNREACHABLE();
		}
	}
	SLL_ASSERT(i==o->l);
	return o;
}



__API_FUNC(string_convert){
	o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
	o->dt.s=NULL;
	if (!ac){
		return;
	}
	o->dt.s=sll_object_to_string(a,ac);
}



__API_FUNC(string_ref_count){
	o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
	o->dt.i=0;
	if (!ac||a->t!=SLL_RUNTIME_OBJECT_TYPE_STRING){
		return;
	}
	o->dt.i=a->dt.s->rc;
}
