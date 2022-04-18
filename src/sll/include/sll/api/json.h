#ifndef __SLL_API_JSON_H__
#define __SLL_API_JSON_H__ 1
#include <sll/common.h>
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
 * \arg const sll_string_t* k
 * \ret sll_json_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_json_object_t* sll_json_get_by_key(sll_json_object_t* json,const sll_string_t* k);



/**
 * \flags check_output func
 * \name sll_json_parse
 * \group json-api
 * \desc Docs!
 * \arg sll_json_parser_state_t* p
 * \arg sll_json_object_t* o
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_json_parse(sll_json_parser_state_t* p,sll_json_object_t* o);



/**
 * \flags func
 * \name sll_api_json__init
 * \group json-api
 * \desc Docs!
 * \api ooo|v
 * \arg sll_object_t* null_obj
 * \arg sll_object_t* true_obj
 * \arg sll_object_t* false_obj
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_json__init(sll_object_t* null_obj,sll_object_t* true_obj,sll_object_t* false_obj);



/**
 * \flags func
 * \name sll_api_json_parse
 * \group json-api
 * \desc Docs!
 * \api #s|o
 * \arg const sll_string_t* str
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_json_parse(const sll_string_t* str);



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



#endif
