#ifndef __SLL_API_JSON_H__
#define __SLL_API_JSON_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/string.h>
#include <sll/types.h>
/**
 * \flags group
 * \name JSON API
 * \group json-api
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Types
 * \group json-api
 * \subgroup json-api-type
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_MAX_JSON_ARRAY_LENGTH
 * \group json-api
 * \desc Docs!
 * \type sll_json_array_length_t
 */
#define SLL_MAX_JSON_ARRAY_LENGTH __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_JSON_MAP_LENGTH
 * \group json-api
 * \desc Docs!
 * \type sll_json_map_length_t
 */
#define SLL_MAX_JSON_MAP_LENGTH __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_JSON_OBJECT_TYPE_NULL
 * \group json-api
 * \subgroup json-api-type
 * \desc Docs!
 * \type sll_json_object_type_t
 */
#define SLL_JSON_OBJECT_TYPE_NULL 0



/**
 * \flags macro var
 * \name SLL_JSON_OBJECT_TYPE_FALSE
 * \group json-api
 * \subgroup json-api-type
 * \desc Docs!
 * \type sll_json_object_type_t
 */
#define SLL_JSON_OBJECT_TYPE_FALSE 1



/**
 * \flags macro var
 * \name SLL_JSON_OBJECT_TYPE_TRUE
 * \group json-api
 * \subgroup json-api-type
 * \desc Docs!
 * \type sll_json_object_type_t
 */
#define SLL_JSON_OBJECT_TYPE_TRUE 2



/**
 * \flags macro var
 * \name SLL_JSON_OBJECT_TYPE_INTEGER
 * \group json-api
 * \subgroup json-api-type
 * \desc Docs!
 * \type sll_json_object_type_t
 */
#define SLL_JSON_OBJECT_TYPE_INTEGER 3



/**
 * \flags macro var
 * \name SLL_JSON_OBJECT_TYPE_FLOAT
 * \group json-api
 * \subgroup json-api-type
 * \desc Docs!
 * \type sll_json_object_type_t
 */
#define SLL_JSON_OBJECT_TYPE_FLOAT 4



/**
 * \flags macro var
 * \name SLL_JSON_OBJECT_TYPE_STRING
 * \group json-api
 * \subgroup json-api-type
 * \desc Docs!
 * \type sll_json_object_type_t
 */
#define SLL_JSON_OBJECT_TYPE_STRING 5



/**
 * \flags macro var
 * \name SLL_JSON_OBJECT_TYPE_ARRAY
 * \group json-api
 * \subgroup json-api-type
 * \desc Docs!
 * \type sll_json_object_type_t
 */
#define SLL_JSON_OBJECT_TYPE_ARRAY 6



/**
 * \flags macro var
 * \name SLL_JSON_OBJECT_TYPE_MAP
 * \group json-api
 * \subgroup json-api-type
 * \desc Docs!
 * \type sll_json_object_type_t
 */
#define SLL_JSON_OBJECT_TYPE_MAP 7



struct _SLL_JSON_OBJECT;
struct _SLL_JSON_MAP_KEYPAIR;



/**
 * \flags type var
 * \name sll_json_object_type_t
 * \group json-api
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_json_object_type_t;



/**
 * \flags type var
 * \name sll_json_array_length_t
 * \group json-api
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_json_array_length_t;



/**
 * \flags type var
 * \name sll_json_map_length_t
 * \group json-api
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_json_map_length_t;



/**
 * \flags type var
 * \name sll_json_parser_state_t
 * \group json-api
 * \desc Docs!
 * \type const sll_char_t*
 */
typedef const sll_char_t* sll_json_parser_state_t;



/**
 * \flags type
 * \name sll_json_array_t
 * \group json-api
 * \desc Docs!
 * \arg struct _SLL_JSON_OBJECT* data
 * \arg sll_json_array_length_t length
 */
typedef struct _SLL_JSON_ARRAY{
	struct _SLL_JSON_OBJECT* data;
	sll_json_array_length_t length;
} sll_json_array_t;



/**
 * \flags type
 * \name sll_json_map_t
 * \group json-api
 * \desc Docs!
 * \arg struct _SLL_JSON_MAP_KEYPAIR* data
 * \arg sll_json_map_length_t length
 */
typedef struct _SLL_JSON_MAP{
	struct _SLL_JSON_MAP_KEYPAIR* data;
	sll_json_map_length_t length;
} sll_json_map_t;



/**
 * \flags type union
 * \name sll_json_object_data_t
 * \group json-api
 * \desc Docs!
 * \arg sll_integer_t int_
 * \arg sll_float_t float_
 * \arg sll_string_t string
 * \arg sll_json_array_t array
 * \arg sll_json_map_t map
 */
typedef union _SLL_JSON_OBJECT_DATA{
	sll_integer_t int_;
	sll_float_t float_;
	sll_string_t string;
	sll_json_array_t array;
	sll_json_map_t map;
} sll_json_object_data_t;



/**
 * \flags type
 * \name sll_json_object_t
 * \group json-api
 * \desc Docs!
 * \arg sll_json_object_type_t type
 * \arg sll_json_object_data_t data
 */
typedef struct _SLL_JSON_OBJECT{
	sll_json_object_type_t type;
	sll_json_object_data_t data;
} sll_json_object_t;



/**
 * \flags type
 * \name sll_json_map_keypair_t
 * \group json-api
 * \desc Docs!
 * \arg sll_string_t key
 * \arg sll_json_object_t value
 */
typedef struct _SLL_JSON_MAP_KEYPAIR{
	sll_string_t key;
	sll_json_object_t value;
} sll_json_map_keypair_t;



/**
 * \flags func
 * \name sll_api_json__init
 * \group json-api
 * \desc Docs!
 * \api ooo|
 * \arg sll_object_t* null_obj
 * \arg sll_object_t* true_obj
 * \arg sll_object_t* false_obj
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_json__init(sll_object_t* null_obj,sll_object_t* true_obj,sll_object_t* false_obj);



/**
 * \flags check_output func
 * \name sll_api_json_parse
 * \group json-api
 * \desc Docs!
 * \api #s|o
 * \arg const sll_string_t* string
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_json_parse(const sll_string_t* string);



/**
 * \flags func
 * \name sll_api_json_stringify
 * \group json-api
 * \desc Docs!
 * \api o|s
 * \arg sll_object_t* json
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_json_stringify(sll_object_t* json,sll_string_t* out);



/**
 * \flags func
 * \name sll_free_json_object
 * \group json-api
 * \desc Docs!
 * \arg sll_json_object_t* json
 */
__SLL_EXTERNAL void sll_free_json_object(sll_json_object_t* json);



/**
 * \flags check_output func
 * \name sll_json_get_by_key
 * \group json-api
 * \desc Docs!
 * \arg sll_json_object_t* json
 * \arg const sll_string_t* key
 * \ret sll_json_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_json_object_t* sll_json_get_by_key(sll_json_object_t* json,const sll_string_t* key);



/**
 * \flags check_output func
 * \name sll_json_parse
 * \group json-api
 * \desc Docs!
 * \arg sll_json_parser_state_t* p
 * \arg sll_json_object_t* out
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_json_parse(sll_json_parser_state_t* p,sll_json_object_t* out);



#endif
