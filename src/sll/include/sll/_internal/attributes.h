#ifndef __SLL__INTERNAL_ATTRIBUTES_H__
#define __SLL__INTERNAL_ATTRIBUTES_H__ 1



#ifdef __SLL_BUILD_WINDOWS
#define __SLL_FORCE_INLINE __inline __forceinline
#define __SLL_NO_RETURN __declspec(noreturn)
#define __SLL_TLS __declspec(thread)
#else
#define __SLL_FORCE_INLINE inline __attribute__((always_inline))
#define __SLL_NO_RETURN __attribute__((noreturn))
#define __SLL_TLS __thread
#endif



#endif
