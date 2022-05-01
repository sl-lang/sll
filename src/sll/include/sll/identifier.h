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
 * \name SLL_MAX_IDENTIFIER_INDEX
 * \group limits
 * \desc Docs!
 * \type sll_identifier_index_t
 */
#define SLL_MAX_IDENTIFIER_INDEX __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_IDENTIFIER_LIST_LENGTH
 * \group limits
 * \desc Docs!
 * \type sll_identifier_list_length_t
 */
#define SLL_MAX_IDENTIFIER_LIST_LENGTH __SLL_U32_MAX



/**
 * \flags func macro
 * \name SLL_IDENTIFIER_GET_ARRAY_ID
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_index_t i
 * \ret sll_identifier_index_t
 */
#define SLL_IDENTIFIER_GET_ARRAY_ID(i) ((i)&0xf)



/**
 * \flags func macro
 * \name SLL_IDENTIFIER_GET_ARRAY_INDEX
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_index_t i
 * \ret sll_identifier_index_t
 */
#define SLL_IDENTIFIER_GET_ARRAY_INDEX(i) ((i)>>4)



/**
 * \flags func macro
 * \name SLL_IDENTIFIER_ADD_INDEX
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_index_t i
 * \arg sll_identifier_index_t j
 * \ret sll_identifier_index_t
 */
#define SLL_IDENTIFIER_ADD_INDEX(i,j) ((i)+((j)<<4))



/**
 * \flags func macro
 * \name SLL_CREATE_IDENTIFIER
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_index_t i
 * \arg sll_identifier_index_t j
 * \ret sll_identifier_index_t
 */
#define SLL_CREATE_IDENTIFIER(i,j) (((i)<<4)|(j))



/**
 * \flags func macro
 * \name SLL_IDENTIFIER_GET_STRING_INDEX
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_t* v
 * \ret sll_string_index_t
 */
#define SLL_IDENTIFIER_GET_STRING_INDEX(v) ((v)->i>>1)



/**
 * \flags func macro
 * \name SLL_IDENTIFIER_IS_TLS
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_t* v
 * \ret sll_bool_t
 */
#define SLL_IDENTIFIER_IS_TLS(v) ((v)->i&1)



/**
 * \flags func macro
 * \name SLL_IDENTIFIER_UPDATE_STRING_INDEX
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_t* v
 * \arg sll_string_index_t si
 */
#define SLL_IDENTIFIER_UPDATE_STRING_INDEX(v,si) ((v)->i=((si)<<1)|((v)->i&1))



/**
 * \flags func macro
 * \name SLL_IDENTIFIER_SET_STRING_INDEX
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_t* v
 * \arg sll_string_index_t si
 * \arg sll_bool_t tls
 */
#define SLL_IDENTIFIER_SET_STRING_INDEX(v,si,tls) ((v)->i=((si)<<1)|(!!(tls)))



/**
 * \flags type
 * \name sll_identifier_t
 * \group identifier
 * \desc Docs!
 * \arg sll_scope_t sc
 * \arg sll_string_index_t i
 */
typedef struct _SLL_IDENTIFIER{
    sll_scope_t sc;
    sll_string_index_t i;
} sll_identifier_t;



/**
 * \flags func
 * \name sll_identifier_add_index
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_index_t i
 * \arg sll_identifier_index_t j
 * \ret sll_identifier_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_identifier_index_t sll_identifier_add_index(sll_identifier_index_t i,sll_identifier_index_t j);



/**
 * \flags func
 * \name sll_identifier_create
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_index_t i
 * \arg sll_identifier_index_t j
 * \ret sll_identifier_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_identifier_index_t sll_identifier_create(sll_identifier_index_t i,sll_identifier_index_t j);



/**
 * \flags func
 * \name sll_identifier_get_array_id
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_index_t i
 * \ret sll_identifier_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_identifier_index_t sll_identifier_get_array_id(sll_identifier_index_t i);



/**
 * \flags func
 * \name sll_identifier_get_array_index
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_index_t i
 * \ret sll_identifier_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_identifier_index_t sll_identifier_get_array_index(sll_identifier_index_t i);



/**
 * \flags check_output func
 * \name sll_identifier_get_string_index
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_t* v
 * \ret sll_string_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_identifier_get_string_index(sll_identifier_t* v);



/**
 * \flags check_output func
 * \name sll_identifier_is_tls
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_t* v
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_identifier_is_tls(sll_identifier_t* v);



/**
 * \flags func
 * \name sll_identifier_set_string_index
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_t* v
 * \arg sll_string_index_t si
 * \arg sll_bool_t tls
 */
__SLL_EXTERNAL void sll_identifier_set_string_index(sll_identifier_t* v,sll_string_index_t si,sll_bool_t tls);



/**
 * \flags func
 * \name sll_identifier_update_string_index
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_t* v
 * \arg sll_string_index_t si
 */
__SLL_EXTERNAL void sll_identifier_update_string_index(sll_identifier_t* v,sll_string_index_t si);



#endif
