#ifndef __SLL_COMPRESSION_H__
#define __SLL_COMPRESSION_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Compression
 * \group compression
 * \desc Docs!
 */



/**
 * \flags check_output func
 * \name sll_compress_integer
 * \group compression
 * \desc Docs!
 * \arg sll_integer_t n
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_compress_integer(sll_integer_t n);



/**
 * \flags check_output func
 * \name sll_decompress_integer
 * \group compression
 * \desc Docs!
 * \arg sll_compressed_integer_t n
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_decompress_integer(sll_compressed_integer_t n);



#endif
