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
 * \arg sll_string_t nm
 * \arg sll_source_file_t dt
 */
typedef struct _SLL_BUNDLE_SOURCE_FILE{
    sll_string_t nm;
    sll_source_file_t dt;
} sll_bundle_source_file_t;



/**
 * \flags type
 * \name sll_bundle_t
 * \group bundle
 * \desc Docs!
 * \arg sll_time_t tm
 * \arg sll_string_t nm
 * \arg sll_bundle_source_file_t** dt
 * \arg sll_source_file_index_t l
 */
typedef struct _SLL_BUNDLE{
    sll_time_t tm;
    sll_string_t nm;
    sll_bundle_source_file_t** dt;
    sll_source_file_index_t l;
} sll_bundle_t;



/**
 * \flags func
 * \name sll_bundle_add_file
 * \group bundle
 * \desc Docs!
 * \arg const sll_char_t* nm
 * \arg sll_compilation_data_t* c_dt
 * \arg sll_bundle_t* o
 */
__SLL_EXTERNAL void sll_bundle_add_file(const sll_char_t* nm,sll_compilation_data_t* c_dt,sll_bundle_t* o);



/**
 * \flags func
 * \name sll_bundle_create
 * \group bundle
 * \desc Docs!
 * \arg const sll_char_t* nm
 * \arg sll_bundle_t* o
 */
__SLL_EXTERNAL void sll_bundle_create(const sll_char_t* nm,sll_bundle_t* o);



/**
 * \flags check_output func
 * \name sll_bundle_fetch
 * \group bundle
 * \desc Docs!
 * \arg const sll_bundle_t* b
 * \arg const sll_string_t* nm
 * \arg sll_compilation_data_t* c_dt
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_bundle_fetch(const sll_bundle_t* b,const sll_string_t* nm,sll_compilation_data_t* c_dt);



/**
 * \flags func
 * \name sll_free_bundle
 * \group bundle
 * \desc Docs!
 * \arg sll_bundle_t* b
 */
__SLL_EXTERNAL void sll_free_bundle(sll_bundle_t* b);



#endif
