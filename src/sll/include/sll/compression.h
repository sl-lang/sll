#ifndef __SLL_COMPRESSION_H__
#define __SLL_COMPRESSION_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Compression
 * \group compression
 * \desc Docs!
 */



/**
 * \flags type var
 * \name sll_compressed_integer_t
 * \group compression
 * \desc Docs!
 * \type __SLL_U64
 */
typedef __SLL_U64 sll_compressed_integer_t;



/**
 * \flags check_output func
 * \name sll_compress_integer
 * \group compression
 * \desc Docs!
 * \arg sll_size_t int_
 * \ret sll_compressed_integer_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compressed_integer_t sll_compress_integer(sll_size_t int_);



/**
 * \flags check_output func
 * \name sll_decompress_integer
 * \group compression
 * \desc Docs!
 * \arg sll_compressed_integer_t compressed_int
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_decompress_integer(sll_compressed_integer_t compressed_int);



#endif
