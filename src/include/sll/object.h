#ifndef __SLL_OBJECT_H__
#define __SLL_OBJECT_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_OBJECT_TYPE_UNKNOWN 0
#define SLL_OBJECT_TYPE_CHAR 1
#define SLL_OBJECT_TYPE_INT 2
#define SLL_OBJECT_TYPE_FLOAT 3
#define SLL_OBJECT_TYPE_STRING 4
#define SLL_OBJECT_TYPE_ARRAY 5
#define SLL_OBJECT_TYPE_MAP 6
#define SLL_OBJECT_TYPE_IDENTIFIER 7
#define SLL_OBJECT_TYPE_FUNCTION_ID 8
#define SLL_OBJECT_TYPE_PRINT 9
#define SLL_OBJECT_TYPE_AND 10
#define SLL_OBJECT_TYPE_OR 11
#define SLL_OBJECT_TYPE_NOT 12
#define SLL_OBJECT_TYPE_ASSIGN 13
#define SLL_OBJECT_TYPE_FUNC 14
#define SLL_OBJECT_TYPE_INTERNAL_FUNC 15
#define SLL_OBJECT_TYPE_INLINE_FUNC 16
#define SLL_OBJECT_TYPE_CALL 17
#define SLL_OBJECT_TYPE_IF 18
#define SLL_OBJECT_TYPE_INLINE_IF 19
#define SLL_OBJECT_TYPE_SWITCH 20
#define SLL_OBJECT_TYPE_FOR 21
#define SLL_OBJECT_TYPE_WHILE 22
#define SLL_OBJECT_TYPE_LOOP 23
#define SLL_OBJECT_TYPE_ADD 24
#define SLL_OBJECT_TYPE_SUB 25
#define SLL_OBJECT_TYPE_MULT 26
#define SLL_OBJECT_TYPE_DIV 27
#define SLL_OBJECT_TYPE_FLOOR_DIV 28
#define SLL_OBJECT_TYPE_MOD 29
#define SLL_OBJECT_TYPE_BIT_AND 30
#define SLL_OBJECT_TYPE_BIT_OR 31
#define SLL_OBJECT_TYPE_BIT_XOR 32
#define SLL_OBJECT_TYPE_BIT_NOT 33
#define SLL_OBJECT_TYPE_BIT_RSHIFT 34
#define SLL_OBJECT_TYPE_BIT_LSHIFT 35
#define SLL_OBJECT_TYPE_LESS 36
#define SLL_OBJECT_TYPE_LESS_EQUAL 37
#define SLL_OBJECT_TYPE_EQUAL 38
#define SLL_OBJECT_TYPE_NOT_EQUAL 39
#define SLL_OBJECT_TYPE_MORE 40
#define SLL_OBJECT_TYPE_MORE_EQUAL 41
#define SLL_OBJECT_TYPE_STRICT_EQUAL 42
#define SLL_OBJECT_TYPE_STRICT_NOT_EQUAL 43
#define SLL_OBJECT_TYPE_LENGTH 44
#define SLL_OBJECT_TYPE_ACCESS 45
#define SLL_OBJECT_TYPE_CAST 46
#define SLL_OBJECT_TYPE_BREAK 47
#define SLL_OBJECT_TYPE_CONTINUE 48
#define SLL_OBJECT_TYPE_REF 49
#define SLL_OBJECT_TYPE_RETURN 50
#define SLL_OBJECT_TYPE_EXIT 51
#define SLL_OBJECT_TYPE_COMMA 52
#define SLL_OBJECT_TYPE_OPERATION_LIST 53
#define SLL_OBJECT_TYPE_DEBUG_DATA 54
#define SLL_OBJECT_TYPE_RESERVED0 254
#define SLL_OBJECT_TYPE_NOP 255

#define SLL_IS_OBJECT_TYPE_NOT_TYPE(o) ((o)->t>SLL_OBJECT_TYPE_FUNCTION_ID)
#define SLL_IS_OBJECT_TYPE_IF(o) ((o)->t>=SLL_OBJECT_TYPE_IF&&(o)->t<=SLL_OBJECT_TYPE_SWITCH)



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_offset_t sll_get_object_size(const sll_object_t* o);



__SLL_EXTERNAL void sll_insert_debug_object(sll_compilation_data_t* c_dt,sll_file_t* rf);



__SLL_EXTERNAL void sll_optimize_object(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft);



__SLL_EXTERNAL void sll_optimize_metadata(sll_compilation_data_t* c_dt);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_parse_all_objects(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft,sll_import_loader_t il,sll_error_t* e);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_parse_object(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft,sll_import_loader_t il,sll_error_t* e,sll_object_t** o);



__SLL_EXTERNAL void sll_print_object(const sll_compilation_data_t* c_dt,const sll_internal_function_table_t* i_ft,const sll_object_t* o,sll_file_t* wf);



__SLL_EXTERNAL void sll_remove_object_debug_data(sll_object_t* o);



__SLL_EXTERNAL void sll_remove_object_padding(sll_compilation_data_t* c_dt,sll_object_t* o);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_skip_object(sll_object_t* o);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT const sll_object_t* sll_skip_object_const(const sll_object_t* o);



#endif
