#ifndef __SLL_IO_H__
#define __SLL_IO_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name I/O
 * \group io
 * \desc Docs!
 */



/**
 * \flags check_output func
 * \name sll_load_assembly
 * \group io
 * \desc Docs!
 * \arg sll_file_t* rf
 * \arg sll_assembly_data_t* a_dt
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_load_assembly(sll_file_t* rf,sll_assembly_data_t* a_dt);



/**
 * \flags check_output func
 * \name sll_load_bundle
 * \group io
 * \desc Docs!
 * \arg sll_file_t* rf
 * \arg sll_bundle_t* b
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_load_bundle(sll_file_t* rf,sll_bundle_t* b);



/**
 * \flags check_output func
 * \name sll_load_compiled_node
 * \group io
 * \desc Docs!
 * \arg sll_file_t* rf
 * \arg sll_compilation_data_t* c_dt
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_load_compiled_node(sll_file_t* rf,sll_compilation_data_t* c_dt);



/**
 * \flags func
 * \name sll_write_assembly
 * \group io
 * \desc Docs!
 * \arg sll_file_t* wf
 * \arg const sll_assembly_data_t* a_dt
 */
__SLL_EXTERNAL void sll_write_assembly(sll_file_t* wf,const sll_assembly_data_t* a_dt);



/**
 * \flags func
 * \name sll_write_bundle
 * \group io
 * \desc Docs!
 * \arg sll_file_t* wf
 * \arg const sll_bundle_t* b
 */
__SLL_EXTERNAL void sll_write_bundle(sll_file_t* wf,const sll_bundle_t* b);



/**
 * \flags func
 * \name sll_write_compiled_node
 * \group io
 * \desc Docs!
 * \arg sll_file_t* wf
 * \arg const sll_compilation_data_t* c_dt
 */
__SLL_EXTERNAL void sll_write_compiled_node(sll_file_t* wf,const sll_compilation_data_t* c_dt);



#endif
