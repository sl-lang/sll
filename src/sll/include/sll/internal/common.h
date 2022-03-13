#ifndef __SLL_INTERNAL_COMMON_H__
#define __SLL_INTERNAL_COMMON_H__ 1
#include <sll/_size_types.h>
#include <sll/string.h>



#ifdef __SLL_BUILD_WINDOWS
#ifndef DEBUG_BUILD
#define SLL_UNREACHABLE() __assume(0)
#endif
#define __SLL_FORCE_INLINE __inline __forceinline
#define __SLL_NO_RETURN __declspec(noreturn)
#else
#ifndef DEBUG_BUILD
#define SLL_UNREACHABLE() __builtin_unreachable()
#endif
#define __SLL_FORCE_INLINE inline __attribute__((always_inline))
#define __SLL_NO_RETURN __attribute__((noreturn))
#endif

#ifdef DEBUG_BUILD
#define SLL_UNREACHABLE() _force_exit(SLL_CHAR("File \""__FILE__"\", Line "_SLL_STRINGIFY(__LINE__)" ("),SLL_CHAR(__func__),SLL_CHAR("): Unreachable Code\n"));
#endif

#define _SLL_STRINGIFY_(x) #x
#define _SLL_STRINGIFY(x) _SLL_STRINGIFY_(x)

#define SLL_UNIMPLEMENTED() _force_exit(SLL_CHAR("File \""__FILE__"\", Line "_SLL_STRINGIFY(__LINE__)" ("),SLL_CHAR(__func__),SLL_CHAR("): Unimplemented\n"));
#ifdef DEBUG_BUILD
#define ASSUME_ALIGNED(p,n,x) SLL_ASSERT(!((ADDR(p)-(x))&((1ull<<(n))-1)))
#define SLL_ASSERT(x) \
	do{ \
		if (!(x)){ \
			_force_exit(SLL_CHAR("File \""__FILE__"\", Line "_SLL_STRINGIFY(__LINE__)" ("),SLL_CHAR(__func__),SLL_CHAR("): "_SLL_STRINGIFY(x)": Assertion Failed\n")); \
		} \
	} while (0)
#else
#define ASSUME_ALIGNED(p,n,x) _ASSUME_ALIGNED(p,(n),(x))
#define SLL_ASSERT(x) \
	do{ \
		if (!(x)){ \
			SLL_UNREACHABLE(); \
		} \
	} while (0)
#endif

#define ADDR(x) ((addr_t)(x))
#define PTR(x) ((void*)(addr_t)(x))



typedef __SLL_U64 addr_t;



typedef __SLL_U64 bitmap_t;



typedef __SLL_U64 wide_data_t;



__SLL_NO_RETURN void _force_exit(const sll_char_t* a,const sll_char_t* b,const sll_char_t* c);



#endif
