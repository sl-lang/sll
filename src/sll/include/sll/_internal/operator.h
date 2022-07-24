#ifndef __SLL__INTERNAL_OPERATOR_H__
#define __SLL__INTERNAL_OPERATOR_H__ 1
#include <sll/_size_types.h>
#include <sll/api/math.h>
#include <sll/data.h>
#include <sll/gc.h>
#include <sll/object.h>



#define COMBINE_TYPES(a,b) (((__SLL_U32)(a)<<8)|(b))
#define COMBINED_ARGS COMBINE_TYPES(a->type,b->type)
#define COMBINED_TYPE_II COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_INT)
#define COMBINED_TYPE_IF COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_IC COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_ID COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_COMPLEX)
#define COMBINED_TYPE_IS COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_IA COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_IM COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_FI COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_INT)
#define COMBINED_TYPE_FF COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_FC COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_FD COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_COMPLEX)
#define COMBINED_TYPE_FS COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_FA COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_FM COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_CI COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_INT)
#define COMBINED_TYPE_CF COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_CC COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_CD COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_COMPLEX)
#define COMBINED_TYPE_CS COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_CA COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_CM COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_DI COMBINE_TYPES(SLL_OBJECT_TYPE_COMPLEX,SLL_OBJECT_TYPE_INT)
#define COMBINED_TYPE_DF COMBINE_TYPES(SLL_OBJECT_TYPE_COMPLEX,SLL_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_DC COMBINE_TYPES(SLL_OBJECT_TYPE_COMPLEX,SLL_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_DD COMBINE_TYPES(SLL_OBJECT_TYPE_COMPLEX,SLL_OBJECT_TYPE_COMPLEX)
#define COMBINED_TYPE_DS COMBINE_TYPES(SLL_OBJECT_TYPE_COMPLEX,SLL_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_DA COMBINE_TYPES(SLL_OBJECT_TYPE_COMPLEX,SLL_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_DM COMBINE_TYPES(SLL_OBJECT_TYPE_COMPLEX,SLL_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_SI COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_INT)
#define COMBINED_TYPE_SF COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_SC COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_SD COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_COMPLEX)
#define COMBINED_TYPE_SS COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_SA COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_SM COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_AI COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_INT)
#define COMBINED_TYPE_AF COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_AC COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_AD COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_COMPLEX)
#define COMBINED_TYPE_AS COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_AA COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_AM COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_MI COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_INT)
#define COMBINED_TYPE_MF COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_MC COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_MD COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_COMPLEX)
#define COMBINED_TYPE_MS COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_MA COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_MM COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_MAP)

#define COMMUTATIVE_OPERATOR \
	sll_bool_t inv=0; \
	if (a->type>b->type){ \
		sll_object_t __tmp=a; \
		a=b; \
		b=__tmp; \
		inv=1; \
	} \

#define TO_FLOAT_BITS(n) ((n)->data.int_)



#endif
