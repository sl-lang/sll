#ifndef __SLL_BUNDLE_H__
#define __SLL_BUNDLE_H__ 1
#include <sll/api/time.h>
#include <sll/common.h>
#include <sll/node.h>
#include <sll/string.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Bundles
 * \group bundle
 * \desc Docs!
 */



/**
 * \flags type
 * \name sll_bundle_source_file_t
 * \group bundle
 * \desc Docs!
 * \arg sll_string_t name
 * \arg sll_source_file_t data
 */
typedef struct _SLL_BUNDLE_SOURCE_FILE{
	sll_string_t name;
	sll_source_file_t data;
} sll_bundle_source_file_t;



/**
 * \flags type
 * \name sll_bundle_t
 * \group bundle
 * \desc Docs!
 * \arg sll_time_t time
 * \arg sll_string_t names
 * \arg sll_bundle_source_file_t** data
 * \arg sll_source_file_index_t length
 */
typedef struct _SLL_BUNDLE{
	sll_time_t time;
	sll_string_t name;
	sll_bundle_source_file_t** data;
	sll_source_file_index_t length;
} sll_bundle_t;



/**
 * \flags func
 * \name sll_bundle_add_file
 * \group bundle
 * \desc Docs!
 * \arg const sll_char_t* name
 * \arg sll_compilation_data_t* compilation_data
 * \arg sll_bundle_t* out
 */
__SLL_EXTERNAL void sll_bundle_add_file(const sll_char_t* name,sll_compilation_data_t* compilation_data,sll_bundle_t* out);



/**
 * \flags func
 * \name sll_bundle_create
 * \group bundle
 * \desc Docs!
 * \arg const sll_char_t* name
 * \arg sll_bundle_t* out
 */
__SLL_EXTERNAL void sll_bundle_create(const sll_char_t* name,sll_bundle_t* out);



/**
 * \flags check_output func
 * \name sll_bundle_fetch
 * \group bundle
 * \desc Docs!
 * \arg const sll_bundle_t* bundle
 * \arg const sll_string_t* name
 * \arg sll_compilation_data_t* out
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_bundle_fetch(const sll_bundle_t* bundle,const sll_string_t* name,sll_compilation_data_t* out);



/**
 * \flags func
 * \name sll_free_bundle
 * \group bundle
 * \desc Docs!
 * \arg sll_bundle_t* bundle
 */
__SLL_EXTERNAL void sll_free_bundle(sll_bundle_t* bundle);



#endif
