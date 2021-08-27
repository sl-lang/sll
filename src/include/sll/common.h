#ifndef __SLL_COMMON_H__
#define __SLL_COMMON_H__ 1



#ifdef _MSC_VER
#ifdef __SLL_STATIC__
#define __SLL_FUNC
#else
#ifdef __SLL_COMPILATION__
#define __SLL_FUNC __declspec(dllexport)
#else
#define __SLL_FUNC __declspec(dllimport)
#endif
#endif
#define __SLL_CHECK_OUTPUT _Check_return_
#else
#ifdef __SLL_STATIC__
#define __SLL_FUNC
#else
#ifdef __SLL_COMPILATION__
#define __SLL_FUNC __attribute__((visibility("default")))
#else
#define __SLL_FUNC
#endif
#endif
#define __SLL_CHECK_OUTPUT __attribute__((warn_unused_result))
#endif
#define __SLL_RETURN __SLL_CHECK_OUTPUT sll_return_t
#define __SLL_RETURN_CODE __SLL_CHECK_OUTPUT sll_return_code_t
#define __SLL_RETURN_COMPARE __SLL_CHECK_OUTPUT sll_compare_result_t
#define __SLL_RETURN_FUNCTION_INDEX __SLL_CHECK_OUTPUT sll_function_index_t
#define __SLL_RETURN_SIZE __SLL_CHECK_OUTPUT sll_object_offset_t
#define __SLL_RETURN_STRING_INDEX __SLL_CHECK_OUTPUT sll_string_index_t
#define __SLL_RETURN_HANDLE_TYPE __SLL_CHECK_OUTPUT sll_handle_type_t



#endif
