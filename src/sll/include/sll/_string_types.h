#ifndef __SLL__STRING_TYPES_H__
#define __SLL__STRING_TYPES_H__ 1
#include <sll/_size_types.h>
#include <sll/types.h>



/**
 * \flags type var
 * \name sll_string_checksum_t
 * \group string
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_string_checksum_t;



/**
 * \flags type var
 * \name sll_string_length_t
 * \group string
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_string_length_t;



/**
 * \flags type var
 * \name sll_wide_char_t
 * \group string
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_wide_char_t;



/**
 * \flags type
 * \name sll_string_t
 * \group string
 * \desc Docs!
 * \arg sll_string_length_t l
 * \arg sll_string_checksum_t c
 * \arg sll_char_t* v
 */
typedef struct _SLL_STRING{
    sll_string_length_t l;
    sll_string_checksum_t c;
    sll_char_t* v;
} sll_string_t;



#endif
