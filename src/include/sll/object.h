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
#define SLL_OBJECT_TYPE_INPUT 10
#define SLL_OBJECT_TYPE_AND 11
#define SLL_OBJECT_TYPE_OR 12
#define SLL_OBJECT_TYPE_NOT 13
#define SLL_OBJECT_TYPE_ASSIGN 14
#define SLL_OBJECT_TYPE_FUNC 15
#define SLL_OBJECT_TYPE_INTERNAL_FUNC 16
#define SLL_OBJECT_TYPE_INLINE_FUNC 17
#define SLL_OBJECT_TYPE_CALL 18
#define SLL_OBJECT_TYPE_IF 19
#define SLL_OBJECT_TYPE_FOR 20
#define SLL_OBJECT_TYPE_WHILE 21
#define SLL_OBJECT_TYPE_LOOP 22
#define SLL_OBJECT_TYPE_ADD 23
#define SLL_OBJECT_TYPE_SUB 24
#define SLL_OBJECT_TYPE_MULT 25
#define SLL_OBJECT_TYPE_DIV 26
#define SLL_OBJECT_TYPE_FLOOR_DIV 27
#define SLL_OBJECT_TYPE_MOD 28
#define SLL_OBJECT_TYPE_BIT_AND 29
#define SLL_OBJECT_TYPE_BIT_OR 30
#define SLL_OBJECT_TYPE_BIT_XOR 31
#define SLL_OBJECT_TYPE_BIT_NOT 32
#define SLL_OBJECT_TYPE_BIT_RSHIFT 33
#define SLL_OBJECT_TYPE_BIT_LSHIFT 34
#define SLL_OBJECT_TYPE_LESS 35
#define SLL_OBJECT_TYPE_LESS_EQUAL 36
#define SLL_OBJECT_TYPE_EQUAL 37
#define SLL_OBJECT_TYPE_NOT_EQUAL 38
#define SLL_OBJECT_TYPE_MORE 39
#define SLL_OBJECT_TYPE_MORE_EQUAL 40
#define SLL_OBJECT_TYPE_LENGTH 41
#define SLL_OBJECT_TYPE_ACCESS 42
#define SLL_OBJECT_TYPE_CAST 43
#define SLL_OBJECT_TYPE_BREAK 44
#define SLL_OBJECT_TYPE_CONTINUE 45
#define SLL_OBJECT_TYPE_REF 46
#define SLL_OBJECT_TYPE_RETURN 47
#define SLL_OBJECT_TYPE_EXIT 48
#define SLL_OBJECT_TYPE_COMMA 49
#define SLL_OBJECT_TYPE_OPERATION_LIST 50
#define SLL_OBJECT_TYPE_DEBUG_DATA 51
#define SLL_OBJECT_TYPE_RESERVED0 254
#define SLL_OBJECT_TYPE_NOP 255

#define SLL_IS_OBJECT_TYPE_NOT_TYPE(o) ((o)->t>SLL_OBJECT_TYPE_IDENTIFIER)



__SLL_FUNC void sll_insert_debug_object(sll_compilation_data_t* c_dt,sll_input_data_stream_t* is);



__SLL_FUNC void sll_optimize_object(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft);



__SLL_FUNC void sll_optimize_metadata(sll_compilation_data_t* c_dt);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_return_t sll_parse_all_objects(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft,sll_import_loader_t il,sll_error_t* e);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_return_t sll_parse_object(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft,sll_import_loader_t il,sll_error_t* e,sll_object_t** o);



__SLL_FUNC void sll_print_object(const sll_compilation_data_t* c_dt,const sll_object_t* o,sll_output_data_stream_t* os);



__SLL_FUNC void sll_remove_object_debug_data(sll_object_t* o);



__SLL_FUNC void sll_remove_object_padding(sll_compilation_data_t* c_dt,sll_object_t* o);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_object_t* sll_skip_object(sll_object_t* o);



__SLL_FUNC __SLL_CHECK_OUTPUT const sll_object_t* sll_skip_object_const(const sll_object_t* o);



#endif
