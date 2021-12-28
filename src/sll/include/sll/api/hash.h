#ifndef __SLL_API_HASH_H__
#define __SLL_API_HASH_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Hash API
 * \group hash-api
 * \desc Docs!
 */



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_hash_md5
 * \group hash-api
 * \desc Docs!
 * \arg I
 * \arg I
 * \arg I
 * \arg I
 * \arg S
 * \ret A
 */
__API_FUNC_DECL(hash_md5);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_hash_sha1
 * \group hash-api
 * \desc Docs!
 * \arg I
 * \arg I
 * \arg I
 * \arg I
 * \arg I
 * \arg S
 * \ret A
 */
__API_FUNC_DECL(hash_sha1);



#endif
