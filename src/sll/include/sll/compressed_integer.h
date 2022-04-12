#ifndef __SLL_COMPRESSED_INTEGER_H__
#define __SLL_COMPRESSED_INTEGER_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Integer Compression
 * \group compressed-integer
 * \desc Docs!
 */



/**
 * \flags check_output func
 * \name sll_compress_integer
 * \group compressed-integer
 * \desc Docs!
 * \arg sll_integer_t n
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_compress_integer(sll_integer_t n);



/**
 * \flags check_output func
 * \name sll_decompress_integer
 * \group compressed-integer
 * \desc Docs!
 * \arg sll_compressed_integer_t v
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_decompress_integer(sll_compressed_integer_t v);



#endif
