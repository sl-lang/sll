#ifndef __SLL_STATIC_OBJECT_H__
#define __SLL_STATIC_OBJECT_H__ 1
#include <sll/gc.h>
#include <sll/string.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Static Object
 * \group static-object
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Creation
 * \group static-object
 * \subgroup static-object-create
 * \desc Docs!
 */



/**
 * \flags func macro
 * \name SLL_ACQUIRE_STATIC
 * \group static-object
 * \desc Docs!
 * \arg __identifier__ name
 * \ret sll_object_t
 */
#define SLL_ACQUIRE_STATIC(name) (SLL_ACQUIRE(sll_static_##name),sll_static_##name)



/**
 * \flags func macro
 * \name SLL_ACQUIRE_STATIC_INT
 * \group static-object
 * \desc Docs!
 * \arg sll_integer_t value
 * \ret sll_object_t
 */
#define SLL_ACQUIRE_STATIC_INT(value) (SLL_ACQUIRE(sll_static_int[(value)]),sll_static_int[(value)])



/**
 * \flags func macro
 * \name SLL_ACQUIRE_STATIC_NEG_INT
 * \group static-object
 * \desc Docs!
 * \arg sll_integer_t value
 * \ret sll_object_t
 */
#define SLL_ACQUIRE_STATIC_NEG_INT(value) (SLL_ACQUIRE(sll_static_negative_int[(value)-1]),sll_static_negative_int[(value)-1])



/**
 * \flags func macro
 * \name SLL_FROM_CHAR
 * \group static-object
 * \desc Docs!
 * \arg sll_char_t char_
 * \ret sll_object_t
 */
#define SLL_FROM_CHAR(char_) (SLL_ACQUIRE(sll_static_char[(sll_char_t)(char_)]),sll_static_char[(sll_char_t)(char_)])



/**
 * \flags var
 * \name sll_static_int
 * \group static-object
 * \desc Docs!
 * \type sll_object_t[256]
 */
__SLL_EXTERNAL extern sll_object_t sll_static_int[256];



/**
 * \flags var
 * \name sll_static_negative_int
 * \group static-object
 * \desc Docs!
 * \type sll_object_t[16]
 */
__SLL_EXTERNAL extern sll_object_t sll_static_negative_int[16];



/**
 * \flags var
 * \name sll_static_float_zero
 * \group static-object
 * \desc Docs!
 * \type sll_object_t
 */
__SLL_EXTERNAL extern sll_object_t sll_static_float_zero;



/**
 * \flags var
 * \name sll_static_float_one
 * \group static-object
 * \desc Docs!
 * \type sll_object_t
 */
__SLL_EXTERNAL extern sll_object_t sll_static_float_one;



/**
 * \flags var
 * \name sll_static_float_half
 * \group static-object
 * \desc Docs!
 * \type sll_object_t
 */
__SLL_EXTERNAL extern sll_object_t sll_static_float_half;



/**
 * \flags var
 * \name sll_static_char
 * \group static-object
 * \desc Docs!
 * \type sll_object_t[256]
 */
__SLL_EXTERNAL extern sll_object_t sll_static_char[256];



/**
 * \flags var
 * \name sll_static_complex_zero
 * \group static-object
 * \desc Docs!
 * \type sll_object_t
 */
__SLL_EXTERNAL extern sll_object_t sll_static_complex_zero;



/**
 * \flags check_output func
 * \name sll_array_length_to_object
 * \group static-object
 * \subgroup static-object-create
 * \desc Docs!
 * \arg sll_array_length_t length
 * \ret sll_object_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_array_length_to_object(sll_array_length_t length);



/**
 * \flags check_output func
 * \name sll_array_to_object
 * \group static-object
 * \subgroup static-object-create
 * \desc Docs!
 * \arg const sll_array_t* array
 * \ret sll_object_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_array_to_object(const sll_array_t* array);



/**
 * \flags check_output func
 * \name sll_array_to_object_nocopy
 * \group static-object
 * \subgroup static-object-create
 * \desc Docs!
 * \arg sll_array_t* array
 * \ret sll_object_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_array_to_object_nocopy(sll_array_t* array);



/**
 * \flags check_output func
 * \name sll_char_to_object
 * \group static-object
 * \subgroup static-object-create
 * \desc Docs!
 * \arg sll_char_t char_
 * \ret sll_object_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_char_to_object(sll_char_t char_);



/**
 * \flags check_output func
 * \name sll_char_to_string_object
 * \group static-object
 * \subgroup static-object-create
 * \desc Docs!
 * \arg sll_char_t char_
 * \ret sll_object_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_char_to_string_object(sll_char_t char_);



/**
 * \flags check_output func
 * \name sll_complex_to_object
 * \group static-object
 * \subgroup static-object-create
 * \desc Docs!
 * \arg const sll_complex_t* complex_
 * \ret sll_object_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_complex_to_object(const sll_complex_t* complex_);



/**
 * \flags check_output func
 * \name sll_float_to_object
 * \group static-object
 * \subgroup static-object-create
 * \desc Docs!
 * \arg sll_float_t float_
 * \ret sll_object_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_float_to_object(sll_float_t float_);



/**
 * \flags check_output func
 * \name sll_int_to_object
 * \group static-object
 * \subgroup static-object-create
 * \desc Docs!
 * \arg sll_integer_t int_
 * \ret sll_object_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_int_to_object(sll_integer_t int_);



/**
 * \flags check_output func
 * \name sll_map_length_to_object
 * \group static-object
 * \subgroup static-object-create
 * \desc Docs!
 * \arg sll_map_length_t length
 * \ret sll_object_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_map_length_to_object(sll_map_length_t length);



/**
 * \flags check_output func
 * \name sll_map_to_object
 * \group static-object
 * \subgroup static-object-create
 * \desc Docs!
 * \arg const sll_map_t* map
 * \ret sll_object_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_map_to_object(const sll_map_t* map);



/**
 * \flags check_output func
 * \name sll_map_to_object_nocopy
 * \group static-object
 * \subgroup static-object-create
 * \desc Docs!
 * \arg sll_map_t* map
 * \ret sll_object_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_map_to_object_nocopy(sll_map_t* map);



/**
 * \flags check_output func
 * \name sll_string_pointer_to_object
 * \group static-object
 * \subgroup static-object-create
 * \desc Docs!
 * \arg const sll_char_t* pointer
 * \ret sll_object_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_string_pointer_to_object(const sll_char_t* pointer);



/**
 * \flags check_output func
 * \name sll_string_pointer_length_to_object
 * \group static-object
 * \subgroup static-object-create
 * \desc Docs!
 * \arg const sll_char_t* pointer
 * \arg sll_string_length_t length
 * \ret sll_object_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_string_pointer_length_to_object(const sll_char_t* pointer,sll_string_length_t length);



/**
 * \flags check_output func
 * \name sll_string_to_object
 * \group static-object
 * \subgroup static-object-create
 * \desc Docs!
 * \arg const sll_string_t* string
 * \ret sll_object_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_string_to_object(const sll_string_t* string);



/**
 * \flags check_output func
 * \name sll_string_to_object_nocopy
 * \group static-object
 * \subgroup static-object-create
 * \desc Docs!
 * \arg sll_string_t* string
 * \ret sll_object_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_string_to_object_nocopy(sll_string_t* string);



#endif
