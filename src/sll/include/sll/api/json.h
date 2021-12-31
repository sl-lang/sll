#ifndef __SLL_API_JSON_H__
#define __SLL_API_JSON_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name JSON API
 * \group json-api
 * \desc Docs!
 */



#define SLL_JSON_OBJECT_TYPE_NULL 0
#define SLL_JSON_OBJECT_TYPE_FALSE 1
#define SLL_JSON_OBJECT_TYPE_TRUE 2
#define SLL_JSON_OBJECT_TYPE_INTEGER 3
#define SLL_JSON_OBJECT_TYPE_FLOAT 4
#define SLL_JSON_OBJECT_TYPE_STRING 5
#define SLL_JSON_OBJECT_TYPE_ARRAY 6
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
 * \flags api func
 * \name sll_api_json__init
 * \group json-api
 * \desc Docs!
 * \arg O
 * \arg O
 * \arg O
 * \ret V
 */
__API_FUNC_DECL(json__init);



/**
 * \flags api func optimizable
 * \name sll_api_json_parse
 * \group json-api
 * \desc Docs!
 * \arg S
 * \ret O
 */
__API_FUNC_DECL(json_parse);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_json_stringify
 * \group json-api
 * \desc Docs!
 * \arg O
 * \ret S
 */
__API_FUNC_DECL(json_stringify);



#endif
