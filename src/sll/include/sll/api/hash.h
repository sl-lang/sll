#ifndef __SLL_API_HASH_H__
#define __SLL_API_HASH_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/string.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Hash API
 * \group hash-api
 * \desc Docs!
 */



/**
 * \flags func macro
 * \name SLL_INIT_MD5
 * \group hash-api
 * \desc Docs!
 * \arg sll_md5_data_t* o
 */
#define SLL_INIT_MD5(o) \
	do{ \
		(o)->a=0x67452301; \
		(o)->b=0xefcdab89; \
		(o)->c=0x98badcfe; \
		(o)->d=0x10325476; \
	} while (0)



/**
 * \flags func macro
 * \name SLL_INIT_SHA1
 * \group hash-api
 * \desc Docs!
 * \arg sll_sha1_data_t* o
 */
#define SLL_INIT_SHA1(o) \
	do{ \
		(o)->a=0x67452301; \
		(o)->b=0xefcdab89; \
		(o)->c=0x98badcfe; \
		(o)->d=0x10325476; \
		(o)->e=0xc3d2e1f0; \
	} while (0)



/**
 * \flags func macro
 * \name SLL_INIT_SHA256
 * \group hash-api
 * \desc Docs!
 * \arg sll_sha256_data_t* o
 */
#define SLL_INIT_SHA256(o) \
	do{ \
		(o)->a=0x6a09e667; \
		(o)->b=0xbb67ae85; \
		(o)->c=0x3c6ef372; \
		(o)->d=0xa54ff53a; \
		(o)->e=0x510e527f; \
		(o)->f=0x9b05688c; \
		(o)->g=0x1f83d9ab; \
		(o)->h=0x5be0cd19; \
	} while (0)



/**
 * \flags func macro
 * \name SLL_INIT_SHA512
 * \group hash-api
 * \desc Docs!
 * \arg sll_sha512_data_t* o
 */
#define SLL_INIT_SHA512(o) \
	do{ \
		(o)->a=0x6a09e667f3bcc908ull; \
		(o)->b=0xbb67ae8584caa73bull; \
		(o)->c=0x3c6ef372fe94f82bull; \
		(o)->d=0xa54ff53a5f1d36f1ull; \
		(o)->e=0x510e527fade682d1ull; \
		(o)->f=0x9b05688c2b3e6c1full; \
		(o)->g=0x1f83d9abfb41bd6bull; \
		(o)->h=0x5be0cd19137e2179ull; \
	} while (0)



/**
 * \flags macro var
 * \name SLL_INIT_MD5_STRUCT
 * \group hash-api
 * \desc Docs!
 * \type sll_md5_data_t
 */
#define SLL_INIT_MD5_STRUCT {0x67452301,0xefcdab89,0x98badcfe,0x10325476}



/**
 * \flags macro var
 * \name SLL_INIT_SHA1_STRUCT
 * \group hash-api
 * \desc Docs!
 * \type sll_sha1_data_t
 */
#define SLL_INIT_SHA1_STRUCT {0x67452301,0xefcdab89,0x98badcfe,0x10325476,0xc3d2e1f0}



/**
 * \flags macro var
 * \name SLL_INIT_SHA256_STRUCT
 * \group hash-api
 * \desc Docs!
 * \type sll_sha256_data_t
 */
#define SLL_INIT_SHA256_STRUCT {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19}



/**
 * \flags macro var
 * \name SLL_INIT_SHA512_STRUCT
 * \group hash-api
 * \desc Docs!
 * \type sll_sha512_data_t
 */
#define SLL_INIT_SHA512_STRUCT {0x6a09e667f3bcc908ull,0xbb67ae8584caa73bull,0x3c6ef372fe94f82bull,0xa54ff53a5f1d36f1ull,0x510e527fade682d1ull,0x9b05688c2b3e6c1full,0x1f83d9abfb41bd6bull,0x5be0cd19137e2179ull}



/**
 * \flags type
 * \name sll_md5_data_t
 * \group hash-api
 * \desc Docs!
 * \arg __SLL_U32 a
 * \arg __SLL_U32 b
 * \arg __SLL_U32 c
 * \arg __SLL_U32 d
 */
typedef struct _SLL_MD5_DATA{
	__SLL_U32 a;
	__SLL_U32 b;
	__SLL_U32 c;
	__SLL_U32 d;
} sll_md5_data_t;



/**
 * \flags type
 * \name sll_sha1_data_t
 * \group hash-api
 * \desc Docs!
 * \arg __SLL_U32 a
 * \arg __SLL_U32 b
 * \arg __SLL_U32 c
 * \arg __SLL_U32 d
 * \arg __SLL_U32 e
 */
typedef struct _SLL_SHA1_DATA{
	__SLL_U32 a;
	__SLL_U32 b;
	__SLL_U32 c;
	__SLL_U32 d;
	__SLL_U32 e;
} sll_sha1_data_t;



/**
 * \flags type
 * \name sll_sha256_data_t
 * \group hash-api
 * \desc Docs!
 * \arg __SLL_U32 a
 * \arg __SLL_U32 b
 * \arg __SLL_U32 c
 * \arg __SLL_U32 d
 * \arg __SLL_U32 e
 * \arg __SLL_U32 f
 * \arg __SLL_U32 g
 * \arg __SLL_U32 h
 */
typedef struct _SLL_SHA256_DATA{
	__SLL_U32 a;
	__SLL_U32 b;
	__SLL_U32 c;
	__SLL_U32 d;
	__SLL_U32 e;
	__SLL_U32 f;
	__SLL_U32 g;
	__SLL_U32 h;
} sll_sha256_data_t;



/**
 * \flags type
 * \name sll_sha512_data_t
 * \group hash-api
 * \desc Docs!
 * \arg __SLL_U64 a
 * \arg __SLL_U64 b
 * \arg __SLL_U64 c
 * \arg __SLL_U64 d
 * \arg __SLL_U64 e
 * \arg __SLL_U64 f
 * \arg __SLL_U64 g
 * \arg __SLL_U64 h
 */
typedef struct _SLL_SHA512_DATA{
	__SLL_U64 a;
	__SLL_U64 b;
	__SLL_U64 c;
	__SLL_U64 d;
	__SLL_U64 e;
	__SLL_U64 f;
	__SLL_U64 g;
	__SLL_U64 h;
} sll_sha512_data_t;



/**
 * \flags func
 * \name sll_api_hash_md5
 * \group hash-api
 * \desc Docs!
 * \api DDDD#s|a
 * \arg __SLL_U32 a
 * \arg __SLL_U32 b
 * \arg __SLL_U32 c
 * \arg __SLL_U32 d
 * \arg const sll_string_t* str
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_hash_md5(__SLL_U32 a,__SLL_U32 b,__SLL_U32 c,__SLL_U32 d,const sll_string_t* str,sll_array_t* out);



/**
 * \flags func
 * \name sll_api_hash_sha1
 * \group hash-api
 * \desc Docs!
 * \api DDDDD#s|a
 * \arg __SLL_U32 a
 * \arg __SLL_U32 b
 * \arg __SLL_U32 c
 * \arg __SLL_U32 d
 * \arg __SLL_U32 e
 * \arg const sll_string_t* str
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_hash_sha1(__SLL_U32 a,__SLL_U32 b,__SLL_U32 c,__SLL_U32 d,__SLL_U32 e,const sll_string_t* str,sll_array_t* out);



/**
 * \flags func
 * \name sll_api_hash_sha256
 * \group hash-api
 * \desc Docs!
 * \api DDDDDDDD#s|a
 * \arg __SLL_U32 a
 * \arg __SLL_U32 b
 * \arg __SLL_U32 c
 * \arg __SLL_U32 d
 * \arg __SLL_U32 e
 * \arg __SLL_U32 f
 * \arg __SLL_U32 g
 * \arg __SLL_U32 h
 * \arg const sll_string_t* str
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_hash_sha256(__SLL_U32 a,__SLL_U32 b,__SLL_U32 c,__SLL_U32 d,__SLL_U32 e,__SLL_U32 f,__SLL_U32 g,__SLL_U32 h,const sll_string_t* str,sll_array_t* out);



/**
 * \flags func
 * \name sll_api_hash_sha512
 * \group hash-api
 * \desc Docs!
 * \api QQQQQQQQ#s|a
 * \arg __SLL_U64 a
 * \arg __SLL_U64 b
 * \arg __SLL_U64 c
 * \arg __SLL_U64 d
 * \arg __SLL_U64 e
 * \arg __SLL_U64 f
 * \arg __SLL_U64 g
 * \arg __SLL_U64 h
 * \arg const sll_string_t* str
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_hash_sha512(__SLL_U64 a,__SLL_U64 b,__SLL_U64 c,__SLL_U64 d,__SLL_U64 e,__SLL_U64 f,__SLL_U64 g,__SLL_U64 h,const sll_string_t* str,sll_array_t* out);



/**
 * \flags func
 * \name sll_hash_md5
 * \group hash-api
 * \desc Desc!
 * \arg sll_md5_data_t* dt
 * \arg void* buffer
 * \arg sll_size_t bfl
 */
__SLL_EXTERNAL void sll_hash_md5(sll_md5_data_t* dt,void* buffer,sll_size_t bfl);



/**
 * \flags func
 * \name sll_hash_sha1
 * \group hash-api
 * \desc Desc!
 * \arg sll_sha1_data_t* dt
 * \arg void* buffer
 * \arg sll_size_t bfl
 */
__SLL_EXTERNAL void sll_hash_sha1(sll_sha1_data_t* dt,void* buffer,sll_size_t bfl);



/**
 * \flags func
 * \name sll_hash_sha256
 * \group hash-api
 * \desc Desc!
 * \arg sll_sha256_data_t* dt
 * \arg void* buffer
 * \arg sll_size_t bfl
 */
__SLL_EXTERNAL void sll_hash_sha256(sll_sha256_data_t* dt,void* buffer,sll_size_t bfl);



/**
 * \flags func
 * \name sll_hash_sha512
 * \group hash-api
 * \desc Desc!
 * \arg sll_sha512_data_t* dt
 * \arg void* buffer
 * \arg sll_size_t bfl
 */
__SLL_EXTERNAL void sll_hash_sha512(sll_sha512_data_t* dt,void* buffer,sll_size_t bfl);



#endif
