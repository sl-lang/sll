#ifndef __SLL_OBJECT_H__
#define __SLL_OBJECT_H__ 1
#include <sll/_object_func.h>
#include <sll/_object_types.h>
#include <sll/_string_types.h>
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Object
 * \group object
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Types
 * \group object
 * \subgroup object-type
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_OBJECT_TYPE_INT
 * \group object
 * \subgroup object-type
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_OBJECT_TYPE_INT 0



/**
 * \flags macro var
 * \name SLL_OBJECT_TYPE_FLOAT
 * \group object
 * \subgroup object-type
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_OBJECT_TYPE_FLOAT 1



/**
 * \flags macro var
 * \name SLL_OBJECT_TYPE_CHAR
 * \group object
 * \subgroup object-type
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_OBJECT_TYPE_CHAR 2



/**
 * \flags macro var
 * \name SLL_OBJECT_TYPE_COMPLEX
 * \group object
 * \subgroup object-type
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_OBJECT_TYPE_COMPLEX 3



/**
 * \flags macro var
 * \name SLL_OBJECT_TYPE_STRING
 * \group object
 * \subgroup object-type
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_OBJECT_TYPE_STRING 4



/**
 * \flags macro var
 * \name SLL_OBJECT_TYPE_ARRAY
 * \group object
 * \subgroup object-type
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_OBJECT_TYPE_ARRAY 5



/**
 * \flags macro var
 * \name SLL_OBJECT_TYPE_MAP
 * \group object
 * \subgroup object-type
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_OBJECT_TYPE_MAP 6



/**
 * \flags macro var
 * \name SLL_OBJECT_TYPE_MAP_KEYS
 * \group object
 * \subgroup object-type
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_OBJECT_TYPE_MAP_KEYS 7



/**
 * \flags macro var
 * \name SLL_OBJECT_TYPE_MAP_VALUES
 * \group object
 * \subgroup object-type
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_OBJECT_TYPE_MAP_VALUES 8



/**
 * \flags macro var
 * \name SLL_OBJECT_TYPE_OBJECT
 * \group object
 * \subgroup object-type
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_OBJECT_TYPE_OBJECT 0xffffffff



/**
 * \flags macro var
 * \name SLL_MAX_OBJECT_TYPE
 * \group object
 * \subgroup object-type
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_MAX_OBJECT_TYPE SLL_OBJECT_TYPE_MAP_VALUES



/**
 * \flags func macro
 * \name SLL_INIT_OBJECT_TYPE_TABLE
 * \group object
 * \desc Docs!
 * \arg sll_object_type_table_t* out
 */
#define SLL_INIT_OBJECT_TYPE_TABLE(out) \
	do{ \
		(out)->dt=NULL; \
		(out)->l=0; \
	} while (0)



/**
 * \flags macro var
 * \name SLL_INIT_OBJECT_TYPE_TABLE_STRUCT
 * \group object
 * \desc Docs!
 * \type sll_object_type_table_t
 */
#define SLL_INIT_OBJECT_TYPE_TABLE_STRUCT {NULL,0}



/**
 * \flags type var
 * \name sll_object_function_index_t
 * \group object
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_object_function_index_t;



/**
 * \flags type var
 * \name sll_object_type_table_length_t
 * \group object
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_object_type_table_length_t;



/**
 * \flags type
 * \name sll_object_type_data_field_t
 * \group object
 * \desc Docs!
 * \arg sll_object_type_t type
 * \arg sll_bool_t read_only
 * \arg sll_string_t name
 */
typedef struct _SLL_OBJECT_TYPE_DATA_FIELD{
	sll_object_type_t type;
	sll_bool_t read_only;
	sll_string_t name;
} sll_object_type_data_field_t;



/**
 * \flags type
 * \name sll_object_type_data_t
 * \group object
 * \desc Docs!
 * \arg const sll_string_t name
 * \arg sll_arg_count_t field_count
 * \arg __SLL_U32 _hash_table_bit_mask
 * \arg sll_arg_count_t* _hash_table
 * \arg __SLL_U64 _rng
 * \arg sll_integer_t[SLL_MAX_OBJECT_FUNC+1] functions
 * \arg sll_object_type_data_field_t[] fields
 */
typedef struct _SLL_OBJECT_TYPE_DATA{
	const sll_string_t name;
	sll_arg_count_t field_count;
	__SLL_U32 _hash_table_bit_mask;
	sll_arg_count_t* _hash_table;
	__SLL_U64 _rng;
	sll_integer_t functions[SLL_MAX_OBJECT_FUNC+1];
	sll_object_type_data_field_t fields[];
} sll_object_type_data_t;



/**
 * \flags type
 * \name sll_object_type_table_t
 * \group object
 * \desc Docs!
 * \arg const sll_object_type_data_t** data
 * \arg sll_object_type_table_length_t length
 */
typedef struct _SLL_OBJECT_TYPE_TABLE{
	const sll_object_type_data_t** data;
	sll_object_type_table_length_t length;
} sll_object_type_table_t;



/**
 * \flags check_output func
 * \name sll_add_type
 * \group object
 * \desc Docs!
 * \arg sll_object_type_table_t* type_table
 * \arg sll_object_t*const* object_data
 * \arg sll_arg_count_t field_count
 * \arg const sll_string_t* name
 * \ret sll_object_type_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_add_type(sll_object_type_table_t* type_table,sll_object_t*const* object_data,sll_arg_count_t field_count,const sll_string_t* name);



/**
 * \flags check_output func
 * \name sll_create_new_object_type
 * \group object
 * \desc Docs!
 * \arg sll_object_type_table_t* type_table
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_create_new_object_type(sll_object_type_table_t* type_table);



/**
 * \flags check_output func
 * \name sll_create_object_type
 * \group object
 * \desc Docs!
 * \arg const sll_object_type_table_t* type_table
 * \arg sll_object_type_t type
 * \arg sll_object_t*const* args
 * \arg sll_arg_count_t arg_count
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_create_object_type(const sll_object_type_table_t* type_table,sll_object_type_t type,sll_object_t*const* args,sll_arg_count_t arg_count);



/**
 * \flags func
 * \name sll_free_object_type_list
 * \group object
 * \desc Docs!
 * \arg sll_object_type_table_t* type_table
 */
__SLL_EXTERNAL void sll_free_object_type_list(sll_object_type_table_t* type_table);



/**
 * \flags check_output func
 * \name sll_get_offset
 * \group object
 * \desc Docs!
 * \arg const sll_object_type_table_t* type_table
 * \arg sll_object_type_t type
 * \arg const sll_string_t* field_name
 * \ret sll_arg_count_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_arg_count_t sll_get_offset(const sll_object_type_table_t* type_table,sll_object_type_t type,const sll_string_t* field_name);



/**
 * \flags func
 * \name sll_get_type_name
 * \group object
 * \desc Docs!
 * \arg const sll_object_type_table_t* type_table
 * \arg sll_object_type_t type
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_get_type_name(const sll_object_type_table_t* type_table,sll_object_type_t type,sll_string_t* out);



/**
 * \flags check_output func
 * \name sll_object_clone
 * \group object
 * \desc Docs!
 * \arg const sll_object_type_table_t* type_table
 * \arg sll_object_t* object
 * \arg sll_bool_t deep
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_object_clone(const sll_object_type_table_t* type_table,sll_object_t* object,sll_bool_t deep);



/**
 * \flags check_output func
 * \name sll_object_get_field
 * \group object
 * \desc Docs!
 * \arg const sll_object_type_table_t* type_table
 * \arg sll_object_t* object
 * \arg const sll_string_t* field_name
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_object_get_field(const sll_object_type_table_t* type_table,sll_object_t* object,const sll_string_t* field_name);



/**
 * \flags func
 * \name sll_object_set_field
 * \group object
 * \desc Docs!
 * \arg const sll_object_type_table_t* type_table
 * \arg sll_object_t* object
 * \arg const sll_string_t* field_name
 * \arg sll_object_t* value
 */
__SLL_EXTERNAL void sll_object_set_field(const sll_object_type_table_t* type_table,sll_object_t* object,const sll_string_t* field_name,sll_object_t* value);



/**
 * \flags func
 * \name sll_object_to_array
 * \group object
 * \desc Docs!
 * \arg const sll_object_type_table_t* type_table
 * \arg sll_object_t* object
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_object_to_array(const sll_object_type_table_t* type_table,sll_object_t* object,sll_array_t* out);



/**
 * \flags func
 * \name sll_object_to_map
 * \group object
 * \desc Docs!
 * \arg const sll_object_type_table_t* type_table
 * \arg sll_object_t* object
 * \arg sll_map_t* out
 */
__SLL_EXTERNAL void sll_object_to_map(const sll_object_type_table_t* type_table,sll_object_t* object,sll_map_t* out);



#endif
