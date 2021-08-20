#ifndef __RUN_TESTS_H__
#define __RUN_TESTS_H__ 1
#include <stdint.h>



#define COMPILER_STACK_SIZE 65536

#define TEST_ID_PARSE 0

#define JSON_OBJECT_TYPE_ARRAY 0
#define JSON_OBJECT_TYPE_FALSE 1
#define JSON_OBJECT_TYPE_FLOAT 2
#define JSON_OBJECT_TYPE_INTEGER 3
#define JSON_OBJECT_TYPE_MAP 4
#define JSON_OBJECT_TYPE_NULL 5
#define JSON_OBJECT_TYPE_STRING 6
#define JSON_OBJECT_TYPE_TRUE 7
#define JSON_OBJECT_TYPE_UNKNOWN 255



struct __JSON_OBJECT;
struct __JSON_MAP_KEYPAIR;



typedef char* json_parser_state_t;



typedef struct __TEST_RESULT{
	uint32_t p;
	uint32_t f;
	uint32_t s;
} test_result_t;



typedef struct __STRING_32BIT{
	uint32_t l;
	char* v;
} string_32bit_t;



typedef struct __JSON_ARRAY{
	uint32_t l;
	struct __JSON_OBJECT* dt;
} json_array_t;



typedef struct __JSON_MAP{
	uint32_t l;
	struct __JSON_MAP_KEYPAIR* dt;
} json_map_t;



typedef union __JSON_OBJECT_DATA{
	int64_t i;
	double f;
	string_32bit_t s;
	json_array_t a;
	json_map_t m;
} json_object_data_t;



typedef struct __JSON_OBJECT{
	uint8_t t;
	json_object_data_t dt;
} json_object_t;



typedef struct __JSON_MAP_KEYPAIR{
	string_32bit_t k;
	json_object_t v;
} json_map_keypair_t;



typedef void (*file_callback_t)(const char* restrict fp,test_result_t* restrict o);



#endif
