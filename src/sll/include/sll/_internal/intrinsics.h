#ifndef __SLL__INTERNAL_INTRINSIC_H__
#define __SLL__INTERNAL_INTRINSIC_H__ 1
#ifdef __SLL_BUILD_WINDOWS
#include <intrin.h>
#endif
#include <sll/_internal/attributes.h>
#include <sll/_internal/common.h>
#include <sll/_size_types.h>



#ifdef __SLL_BUILD_WINDOWS
#pragma intrinsic(_BitScanForward64)
#pragma intrinsic(__popcnt64)
#pragma intrinsic(_rotl)
#pragma intrinsic(_rotl64)
static __SLL_FORCE_INLINE __SLL_U32 FIND_FIRST_SET_BIT(__SLL_U64 m){
	unsigned long o;
	_BitScanForward64(&o,m);
	return o;
}
static __SLL_FORCE_INLINE __SLL_U32 FIND_LAST_SET_BIT(__SLL_U64 m){
	unsigned long o;
	_BitScanReverse64(&o,m);
	return o;
}
#define POPULATION_COUNT(m) __popcnt64((m))
#define SWAP_BYTES(m) _byteswap_ulong((m))
#define SWAP_BYTES64(m) _byteswap_uint64((m))
#define ROTATE_BITS(a,b) _rotl((a),(b))
#define ROTATE_BITS_RIGHT(a,b) _rotr((a),(b))
#define ROTATE_BITS64(a,b) _rotl64((a),(b))
#define ROTATE_BITS_RIGHT64(a,b) _rotr64((a),(b))
#define _ASSUME_ALIGNED(p,n,x) \
	do{ \
		if ((ADDR(p)-(x))&((1ull<<(n))-1)){ \
			__assume(0); \
		} \
	} while (0)
#else
#define FIND_FIRST_SET_BIT(m) (__builtin_ffsll((m))-1)
#define FIND_LAST_SET_BIT(m) (63-__builtin_clzll((m)))
#define POPULATION_COUNT(m) __builtin_popcountll((m))
#define SWAP_BYTES(m) __builtin_bswap32((m))
#define SWAP_BYTES64(m) __builtin_bswap64((m))
static __SLL_FORCE_INLINE unsigned int ROTATE_BITS(unsigned int a,unsigned char b){
	__asm__("rol %1,%0":"+r"(a):"c"(b));
	return a;
}
static __SLL_FORCE_INLINE unsigned int ROTATE_BITS_RIGHT(unsigned int a,unsigned char b){
	__asm__("ror %1,%0":"+r"(a):"c"(b));
	return a;
}
static __SLL_FORCE_INLINE unsigned long long int ROTATE_BITS64(unsigned long long int a,unsigned char b){
	__asm__("rolq %1,%0":"+r"(a):"c"(b));
	return a;
}
static __SLL_FORCE_INLINE unsigned long long int ROTATE_BITS_RIGHT64(unsigned long long int a,unsigned char b){
	__asm__("rorq %1,%0":"+r"(a):"c"(b));
	return a;
}
#define _ASSUME_ALIGNED(p,n,x) \
	do{ \
		p=__builtin_assume_aligned((p),1<<(n),(x)); \
	} while (0)
#endif



#endif
