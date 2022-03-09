#ifndef __SLL_BUNDLE_H__
#define __SLL_BUNDLE_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Bundles
 * \group bundle
 * \desc Docs!
 */



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
 * \flags func
 * \name sll_free_bundle
 * \group bundle
 * \desc Docs!
 * \arg sll_bundle_t* b
 */
__SLL_EXTERNAL void sll_free_bundle(sll_bundle_t* b);



#endif
