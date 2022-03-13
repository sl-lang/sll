#ifndef __SLL_COMMON_H__
#define __SLL_COMMON_H__ 1



#ifdef __SLL_BUILD_WINDOWS
#ifdef __SLL_COMPILATION__
#define __SLL_EXTERNAL __declspec(dllexport)
#else
#define __SLL_EXTERNAL __declspec(dllimport)
#endif
#define __SLL_CHECK_OUTPUT _Check_return_
#else
#ifdef __SLL_COMPILATION__
#define __SLL_EXTERNAL __attribute__((visibility("default")))
#else
#define __SLL_EXTERNAL
#endif
#define __SLL_CHECK_OUTPUT __attribute__((warn_unused_result))
#endif



#ifndef NULL
#define NULL ((void*)0)
#endif



/**
 * \flags macro var
 * \name SLL_MAX_SHORT_IDENTIFIER_LENGTH
 * \group identifier
 * \desc Docs!
 * \type sll_identifier_index_t
 */
#define SLL_MAX_SHORT_IDENTIFIER_LENGTH 15



#endif
