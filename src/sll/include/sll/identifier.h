#ifndef __SLL_IDENTIFIER_H__
#define __SLL_IDENTIFIER_H__ 1
#include <sll/_identifier.h>
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/string_table.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Identifier
 * \group identifier
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_MAX_SCOPE
 * \group identifier
 * \desc Docs!
 * \type sll_scope_t
 */
#define SLL_MAX_SCOPE __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_IDENTIFIER_INDEX
 * \group identifier
 * \desc Docs!
 * \type sll_identifier_index_t
 */
#define SLL_MAX_IDENTIFIER_INDEX __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_IDENTIFIER_LIST_LENGTH
 * \group identifier
 * \desc Docs!
 * \type sll_identifier_list_length_t
 */
#define SLL_MAX_IDENTIFIER_LIST_LENGTH __SLL_U32_MAX



/**
 * \flags func macro
 * \name SLL_IDENTIFIER_GET_ARRAY_ID
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_index_t identifier_index
 * \ret sll_identifier_index_t
 */
#define SLL_IDENTIFIER_GET_ARRAY_ID(identifier_index) ((identifier_index)&0xf)



/**
 * \flags func macro
 * \name SLL_IDENTIFIER_GET_ARRAY_INDEX
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_index_t identifier_index
 * \ret sll_identifier_index_t
 */
#define SLL_IDENTIFIER_GET_ARRAY_INDEX(identifier_index) ((identifier_index)>>4)



/**
 * \flags func macro
 * \name SLL_IDENTIFIER_ADD_INDEX
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_index_t identifier_index
 * \arg sll_identifier_index_t delta_index
 * \ret sll_identifier_index_t
 */
#define SLL_IDENTIFIER_ADD_INDEX(identifier_index,delta_index) ((identifier_index)+((delta_index)<<4))



/**
 * \flags func macro
 * \name SLL_CREATE_IDENTIFIER
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_index_t index
 * \arg sll_identifier_index_t id
 * \ret sll_identifier_index_t
 */
#define SLL_CREATE_IDENTIFIER(index,id) (((index)<<4)|(id))



/**
 * \flags func macro
 * \name SLL_IDENTIFIER_GET_STRING_INDEX
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_t* identifier
 * \ret sll_string_index_t
 */
#define SLL_IDENTIFIER_GET_STRING_INDEX(identifier) ((identifier)->name_string_index>>1)



/**
 * \flags func macro
 * \name SLL_IDENTIFIER_IS_TLS
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_t* identifier
 * \ret sll_bool_t
 */
#define SLL_IDENTIFIER_IS_TLS(identifier) ((identifier)->name_string_index&1)



/**
 * \flags func macro
 * \name SLL_IDENTIFIER_UPDATE_STRING_INDEX
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_t* identifier
 * \arg sll_string_index_t name_string_index_
 */
#define SLL_IDENTIFIER_UPDATE_STRING_INDEX(identifier,name_string_index_) ((identifier)->name_string_index=((name_string_index_)<<1)|((identifier)->name_string_index&1))



/**
 * \flags func macro
 * \name SLL_IDENTIFIER_SET_STRING_INDEX
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_t* identifier
 * \arg sll_string_index_t name_string_index
 * \arg sll_bool_t tls
 */
#define SLL_IDENTIFIER_SET_STRING_INDEX(identifier,name_string_index_,tls) ((identifier)->name_string_index=((name_string_index_)<<1)|(!!(tls)))



/**
 * \flags type var
 * \name sll_identifier_index_t
 * \group identifier
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_identifier_index_t;



/**
 * \flags type var
 * \name sll_identifier_list_length_t
 * \group identifier
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_identifier_list_length_t;



/**
 * \flags type var
 * \name sll_scope_t
 * \group identifier
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_scope_t;



/**
 * \flags type
 * \name sll_identifier_t
 * \group identifier
 * \desc Docs!
 * \arg sll_scope_t scope
 * \arg sll_string_index_t name_string_index
 */
typedef struct _SLL_IDENTIFIER{
	sll_scope_t scope;
	sll_string_index_t name_string_index;
} sll_identifier_t;



/**
 * \flags type
 * \name sll_identifier_list_t
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_t* data
 * \arg sll_identifier_list_length_t length
 */
typedef struct _SLL_IDENTIFIER_LIST{
	sll_identifier_t* data;
	sll_identifier_list_length_t length;
} sll_identifier_list_t;



/**
 * \flags type
 * \name sll_identifier_table_t
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_list_t[SLL_MAX_SHORT_IDENTIFIER_LENGTH] short_
 * \arg sll_identifier_t* long_data
 * \arg sll_identifier_list_length_t long_data_length
 */
typedef struct _SLL_IDENTIFIER_TABLE{
	sll_identifier_list_t short_[SLL_MAX_SHORT_IDENTIFIER_LENGTH];
	sll_identifier_t* long_data;
	sll_identifier_list_length_t long_data_length;
} sll_identifier_table_t;



/**
 * \flags func
 * \name sll_identifier_add_index
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_index_t identifier_index
 * \arg sll_identifier_index_t delta_index
 * \ret sll_identifier_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_identifier_index_t sll_identifier_add_index(sll_identifier_index_t identifier_index,sll_identifier_index_t delta_index);



/**
 * \flags func
 * \name sll_identifier_create
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_index_t index
 * \arg sll_identifier_index_t id
 * \ret sll_identifier_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_identifier_index_t sll_identifier_create(sll_identifier_index_t index,sll_identifier_index_t id);



/**
 * \flags func
 * \name sll_identifier_get_array_id
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_index_t identifier_index
 * \ret sll_identifier_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_identifier_index_t sll_identifier_get_array_id(sll_identifier_index_t identifier_index);



/**
 * \flags func
 * \name sll_identifier_get_array_index
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_index_t identifier_index
 * \ret sll_identifier_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_identifier_index_t sll_identifier_get_array_index(sll_identifier_index_t identifier_index);



/**
 * \flags check_output func
 * \name sll_identifier_get_string_index
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_t* identifier
 * \ret sll_string_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_identifier_get_string_index(sll_identifier_t* identifier);



/**
 * \flags check_output func
 * \name sll_identifier_is_tls
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_t* identifier
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_identifier_is_tls(sll_identifier_t* identifier);



/**
 * \flags func
 * \name sll_identifier_set_string_index
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_t* identifier
 * \arg sll_string_index_t name_string_index
 * \arg sll_bool_t tls
 */
__SLL_EXTERNAL void sll_identifier_set_string_index(sll_identifier_t* identifier,sll_string_index_t name_string_index,sll_bool_t tls);



/**
 * \flags func
 * \name sll_identifier_update_string_index
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_t* identifier
 * \arg sll_string_index_t name_string_index
 */
__SLL_EXTERNAL void sll_identifier_update_string_index(sll_identifier_t* identifier,sll_string_index_t name_string_index);



#endif
