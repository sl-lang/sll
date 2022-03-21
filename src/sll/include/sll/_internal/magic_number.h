#ifndef __SLL__INTERNAL_MAGIC_NUMBER_H__
#define __SLL__INTERNAL_MAGIC_NUMBER_H__ 1
#include <sll/_size_types.h>



#define _CREATE_MAGIC_NUMBER(a,b,c,d) ((((magic_number_t)(d))<<24)|(((magic_number_t)(c))<<16)|(((magic_number_t)(b))<<8)|(a))

#define ASSEMBLY_FILE_MAGIC_NUMBER _CREATE_MAGIC_NUMBER('S','L','A',0)
#define BUNDLE_FILE_MAGIC_NUMBER _CREATE_MAGIC_NUMBER('S','L','B',0)
#define COMPLIED_OBJECT_FILE_MAGIC_NUMBER _CREATE_MAGIC_NUMBER('S','L','C',0)



typedef __SLL_U32 magic_number_t;



#endif
