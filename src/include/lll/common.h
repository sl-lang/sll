#ifndef __LLL_COMMON_H__
#define __LLL_COMMON_H__ 1



#ifdef _MSC_VER
#ifdef __LLL_LIB_COMPILATION__
#define __LLL_FUNC __declspec(dllexport)
#else
#define __LLL_FUNC __declspec(dllimport)
#endif
#define __LLL_CHECK_OUTPUT _Check_return_
#else
#ifdef __LLL_LIB_COMPILATION__
#define __LLL_FUNC __attribute__((visibility("default")))
#else
#define __LLL_FUNC
#endif
#define __LLL_CHECK_OUTPUT __attribute__((warn_unused_result))
#endif
#define __LLL_RETURN __LLL_CHECK_OUTPUT lll_return_t
#define __LLL_RETURN_CODE __LLL_CHECK_OUTPUT lll_return_code_t
#define __LLL_RETURN_COMPARE __LLL_CHECK_OUTPUT lll_compare_result_t
#define __LLL_RETURN_FUNCTION_INDEX __LLL_CHECK_OUTPUT lll_function_index_t
#define __LLL_RETURN_SIZE __LLL_CHECK_OUTPUT lll_object_offset_t
#define __LLL_RETURN_STRING_INDEX __LLL_CHECK_OUTPUT lll_string_index_t



#endif
