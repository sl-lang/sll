#ifndef __LANGUAGE_H__
#define __LANGUAGE_H__ 1
#include <stdint.h>
#include <stdio.h>



#define OBJECT_TYPE_PRINT 0
#define OBJECT_TYPE_CHAR 1
#define OBJECT_TYPE_STRING 2
#define OBJECT_TYPE_INT 3
#define OBJECT_TYPE_FLOAT 4
#define OBJECT_TYPE_ADD 5
#define OBJECT_TYPE_SUB 6
#define OBJECT_TYPE_MULT 7
#define OBJECT_TYPE_DIV 8
#define OBJECT_TYPE_FLOOR_DIV 9
#define OBJECT_TYPE_MOD 10
#define OBJECT_TYPE_UNKNOWN 0xff
#define INTERNAL_STACK_SIZE 65536
#define GET_OBJECT_ARGUMENT(o,i) ((object_t*)((uint8_t*)(o)+(i)))
#define GET_OBJECT_AS_CHAR(o) ((char)(*((char*)(((uint8_t*)(o))+sizeof(object_t)))))
#define GET_OBJECT_AS_INT(o) ((int64_t)(*((int64_t*)(((uint8_t*)(o))+sizeof(object_t)))))
#define GET_OBJECT_AS_FLOAT(o) ((double)(*((double*)(((uint8_t*)(o))+sizeof(object_t)))))



typedef struct __OBJECT{
	uint8_t t;
	uint8_t l;
} object_t;



uint32_t print_object(object_t* o,FILE* f);



object_t* read_object(FILE* f);



#endif
