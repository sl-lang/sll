#ifndef __SLL_API_JSON_H__
#define __SLL_API_JSON_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>



#define SLL_JSON_OBJECT_TYPE_ARRAY 0
#define SLL_JSON_OBJECT_TYPE_FALSE 1
#define SLL_JSON_OBJECT_TYPE_FLOAT 2
#define SLL_JSON_OBJECT_TYPE_INTEGER 3
#define SLL_JSON_OBJECT_TYPE_MAP 4
#define SLL_JSON_OBJECT_TYPE_NULL 5
#define SLL_JSON_OBJECT_TYPE_STRING 6
#define SLL_JSON_OBJECT_TYPE_TRUE 7



__SLL_EXTERNAL void sll_free_json_object(sll_json_object_t* json);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_json_object_t* sll_json_get_by_key(sll_json_object_t* json,const sll_string_t* k);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_json_parse(sll_json_parser_state_t* p,sll_json_object_t* o);



/*~~DOCS~~
TYPE api optimizable
FUNC sll_api_json_parse
DESC Converts a [JSON][https://www.json.org/json-en.html] string into SLL types
ARG S The string to parse
RET h Failure
RET O The parsed object*/
__API_FUNC_DECL(json_parse);



/*~~DOCS~~
TYPE api compilation_call optimizable
FUNC sll_api_json_stringify
DESC Docs!
ARG O Docs!
RET Z Failure
RET S Success*/
__API_FUNC_DECL(json_stringify);



/*~~DOCS~~
TYPE api optimizable
FUNC sll_api_json_type
DESC Docs!
ARG I Docs!
RET h Failure
RET H Success*/
__API_FUNC_DECL(json_type);



#endif
