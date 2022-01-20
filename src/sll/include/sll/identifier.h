#ifndef __SLL_IDENTIFIER_H__
#define __SLL_IDENTIFIER_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Identifier
 * \group identifier
 * \desc Docs!
 */



#define SLL_IDENTIFIER_GET_ARRAY_ID(i) ((i)&0xf)
#define SLL_IDENTIFIER_GET_ARRAY_INDEX(i) ((i)>>4)
#define SLL_IDENTIFIER_ADD_INDEX(i,j) ((i)+((j)<<4))
#define SLL_CREATE_IDENTIFIER(i,j) (((i)<<4)|(j))




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
 * \name sll_identifier_create_identifier
 * \group identifier
 * \desc Docs!
 * \arg sll_identifier_index_t i
 * \arg sll_identifier_index_t j
 * \ret sll_identifier_index_t
 */

__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_identifier_index_t sll_identifier_create_identifier(sll_identifier_index_t i,sll_identifier_index_t j);




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



#endif
