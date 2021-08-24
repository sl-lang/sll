#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/constants.h>
#include <sll/gc.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdio.h>
#include <string.h>



sll_string_length_t _object_to_string(const sll_runtime_object_t* a,sll_string_t* o,sll_string_index_t i){
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
			memcpy(o->v+i,a->dt.s.v,a->dt.s.l);
			o->c^=a->dt.s.c;
			return i+a->dt.s.l;
		case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
			o->v[i]='[';
			i++;
			for (sll_array_length_t k=0;k<a->dt.a.l;k++){
				if (k){
					o->v[i]=' ';
					i++;
				}
				i=_object_to_string(*(a->dt.a.v+k),o,i);
			}
			o->v[i]=']';
			o->c^='['^']';
			if (a->dt.a.l&1){
				o->c^=' ';
			}
			return i+1;
		default:
			SLL_UNREACHABLE();
	}
	return 0;
}



__SLL_FUNC void sll_object_to_string(const sll_runtime_object_t** a,sll_array_length_t al,sll_string_t* o){
	if (!al){
		SLL_ZERO_STRING(o);
		return;
	}
	sll_string_create(sll_object_to_string_length(a,al),o);
	sll_string_length_t i=0;
	for (sll_array_length_t j=0;j<al;j++){
		i=_object_to_string(*(a+j),o,i);
	}
	SLL_ASSERT(i==o->l);
}



__SLL_FUNC sll_string_length_t sll_object_to_string_length(const sll_runtime_object_t** al,sll_array_length_t all){
	sll_string_length_t o=0;
	for (sll_array_length_t i=0;i<all;i++){
		const sll_runtime_object_t* a=*(al+i);
		switch (a->t){
			case SLL_RUNTIME_OBJECT_TYPE_INT:
				{
					sll_integer_t v=a->dt.i;
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
				o+=snprintf(NULL,0,"%.16lf",a->dt.f);
				break;
			case SLL_RUNTIME_OBJECT_TYPE_CHAR:
				o++;
				break;
			case SLL_RUNTIME_OBJECT_TYPE_STRING:
				o+=a->dt.s.l;
				break;
			case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
				o+=sll_object_to_string_length((const sll_runtime_object_t**)(a->dt.a.v),a->dt.a.l)+a->dt.a.l+1;
				break;
			default:
				SLL_UNREACHABLE();
		}
	}
	return o;
}



__API_FUNC(string_convert){
	if (!ac){
		SLL_RETURN_ZERO_STRING;
	}
	sll_runtime_object_t* o=SLL_CREATE();
	o->rc=1;
	o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
	sll_object_to_string(a,ac,&(o->dt.s));
	return o;
}



__API_FUNC(string_length){
	return sll_int_to_object(sll_object_to_string_length(a,ac));
}
