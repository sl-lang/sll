#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/constants.h>
#include <sll/gc.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdio.h>
#include <string.h>



sll_string_length_t _object_to_string(sll_runtime_object_t* a,sll_string_t* o,sll_string_index_t i){
	switch (a->t){
		case SLL_RUNTIME_OBJECT_TYPE_INT:
			{
				sll_integer_t v=a->dt.i;
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
				return i;
			}
		case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
			{
				sll_string_length_t k=i+snprintf((char*)(o->v+i),4096,"%.16lf",a->dt.f);
				while (i<k){
					o->c^=o->v[i];
					i++;
				}
				return i;
			}
		case SLL_RUNTIME_OBJECT_TYPE_CHAR:
			o->v[i]=a->dt.c;
			o->c^=a->dt.c;
			return i+1;
		case SLL_RUNTIME_OBJECT_TYPE_STRING:
			memcpy(o->v+i,(a->dt.s)->v,(a->dt.s)->l);
			o->c^=a->dt.s->c;
			return i+a->dt.s->l;
		case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
			o->v[i]='[';
			i++;
			for (sll_array_length_t k=0;k<a->dt.a->l;k++){
				if (k){
					o->v[i]=' ';
					i++;
				}
				i=_object_to_string(a->dt.a->v+k,o,i);
			}
			o->v[i]=']';
			o->c^='['^']';
			if (a->dt.a->l&1){
				o->c^=' ';
			}
			return i+1;
		default:
			SLL_UNREACHABLE();
	}
	return 0;
}



__SLL_FUNC sll_string_t* sll_object_to_string(sll_runtime_object_t* a,sll_array_length_t al){
	if (!al){
		SLL_ACQUIRE(&_zero_string);
		return &_zero_string;
	}
	if (al==1&&a->t==SLL_RUNTIME_OBJECT_TYPE_STRING){
		SLL_ACQUIRE(a->dt.s);
		return a->dt.s;
	}
	sll_string_t* o=sll_string_create(sll_object_to_string_length(a,al));
	o->c=0;
	sll_string_length_t i=0;
	for (sll_array_length_t j=0;j<al;j++){
		i=_object_to_string(a+j,o,i);
	}
	SLL_ASSERT(i==o->l);
	return o;
}



__SLL_FUNC sll_string_length_t sll_object_to_string_length(sll_runtime_object_t* a,sll_array_length_t al){
	sll_string_length_t o=0;
	for (sll_array_length_t i=0;i<al;i++){
		switch ((a+i)->t){
			case SLL_RUNTIME_OBJECT_TYPE_INT:
				{
					sll_integer_t v=(a+i)->dt.i;
					if (v<0){
						o++;
						v=-v;
					}
					do{
						o++;
						v/=10;
					} while (v);
					break;
				}
			case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
				o+=snprintf(NULL,0,"%.16lf",(a+i)->dt.f);
				break;
			case SLL_RUNTIME_OBJECT_TYPE_CHAR:
				o++;
				break;
			case SLL_RUNTIME_OBJECT_TYPE_STRING:
				o+=(a+i)->dt.s->l;
				break;
			case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
				o+=sll_object_to_string_length((a+i)->dt.a->v,(a+i)->dt.a->l)+(a+i)->dt.a->l+1;
				break;
			default:
				SLL_UNREACHABLE();
		}
	}
	return o;
}



__API_FUNC(string_convert){
	o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
	if (!ac){
		o->dt.s=&_zero_string;
		SLL_ACQUIRE(o->dt.s);
		return;
	}
	o->dt.s=sll_object_to_string(a,ac);
}



__API_FUNC(string_length){
	o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
	o->dt.i=sll_object_to_string_length(a,ac);
}
