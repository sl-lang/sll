#ifndef __SLL_IO_H__
#define __SLL_IO_H__ 1
#include <sll/assembly.h>
#include <sll/bundle.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/node.h>
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
 * \arg sll_file_t* file
 * \arg sll_assembly_data_t* out
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_load_assembly(sll_file_t* file,sll_assembly_data_t* out);



/**
 * \flags check_output func
 * \name sll_load_bundle
 * \group io
 * \desc Docs!
 * \arg sll_file_t* file
 * \arg sll_bundle_t* out
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_load_bundle(sll_file_t* file,sll_bundle_t* out);



/**
 * \flags check_output func
 * \name sll_load_compiled_node
 * \group io
 * \desc Docs!
 * \arg sll_file_t* file
 * \arg sll_compilation_data_t* out
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_load_compiled_node(sll_file_t* file,sll_compilation_data_t* out);



/**
 * \flags func
 * \name sll_write_assembly
 * \group io
 * \desc Docs!
 * \arg sll_file_t* wf
 * \arg const sll_assembly_data_t* assembly_data
 */
__SLL_EXTERNAL void sll_write_assembly(sll_file_t* wf,const sll_assembly_data_t* assembly_data);



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
 * \arg const sll_compilation_data_t* compilation_data
 */
__SLL_EXTERNAL void sll_write_compiled_node(sll_file_t* wf,const sll_compilation_data_t* compilation_data);



#endif
