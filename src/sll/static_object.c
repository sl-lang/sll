#include <sll/_sll_internal.h>
#include <sll/array.h>
#include <sll/constants.h>
#include <sll/gc.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_STATIC_OBJECT(int_minus_one,SLL_RUNTIME_OBJECT_TYPE_INT,i,-1);



__SLL_STATIC_OBJECT(int_zero,SLL_RUNTIME_OBJECT_TYPE_INT,i,0);



__SLL_STATIC_OBJECT(int_one,SLL_RUNTIME_OBJECT_TYPE_INT,i,1);



__SLL_STATIC_OBJECT(int_two,SLL_RUNTIME_OBJECT_TYPE_INT,i,2);



__SLL_STATIC_OBJECT(int_three,SLL_RUNTIME_OBJECT_TYPE_INT,i,3);



__SLL_STATIC_OBJECT(int_four,SLL_RUNTIME_OBJECT_TYPE_INT,i,4);



__SLL_STATIC_OBJECT(float_zero,SLL_RUNTIME_OBJECT_TYPE_FLOAT,f,0);



__SLL_STATIC_OBJECT(float_one,SLL_RUNTIME_OBJECT_TYPE_FLOAT,f,1);



__SLL_STATIC_OBJECT(float_half,SLL_RUNTIME_OBJECT_TYPE_FLOAT,f,0.5);



__SLL_STATIC_OBJECT(float_third,SLL_RUNTIME_OBJECT_TYPE_FLOAT,f,0.3333333333333333);



__SLL_STATIC_OBJECT(str_zero,SLL_RUNTIME_OBJECT_TYPE_STRING,s,SLL_ZERO_STRING_STRUCT);



__SLL_STATIC_OBJECT(array_zero,SLL_RUNTIME_OBJECT_TYPE_ARRAY,a,SLL_ZERO_ARRAY_STRUCT);



static sll_runtime_object_t* _small_int[]={__SLL_STATIC_RAW(int_minus_one),__SLL_STATIC_RAW(int_zero),__SLL_STATIC_RAW(int_one),__SLL_STATIC_RAW(int_two),__SLL_STATIC_RAW(int_three),__SLL_STATIC_RAW(int_four)};



__SLL_FUNC sll_runtime_object_t* sll_int_to_object(sll_integer_t v){
	if (v>-2&&v<5){
		sll_runtime_object_t* o=_small_int[v+1];
		SLL_ACQUIRE(o);
		return o;
	}
	sll_runtime_object_t* o=sll_create_object();
	o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
	o->dt.i=v;
	return o;
}



__SLL_FUNC sll_runtime_object_t* sll_float_to_object(sll_float_t v){
	if (!v){
		return SLL_ACQUIRE_STATIC(float_zero);
	}
	if (v==1){
		return SLL_ACQUIRE_STATIC(float_one);
	}
	if (v==0.5){
		return SLL_ACQUIRE_STATIC(float_half);
	}
	if (v==0.3333333333333333){
		return SLL_ACQUIRE_STATIC(float_third);
	}
	sll_runtime_object_t* o=sll_create_object();
	o->t=SLL_RUNTIME_OBJECT_TYPE_FLOAT;
	o->dt.f=v;
	return o;
}
