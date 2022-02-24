#ifndef __SLL_VERSION_H__
#define __SLL_VERSION_H__ 1
#include <sll/types.h>
/**
 * \flags group
 * \name Build Version
 * \group version
 * \desc Docs!
 */



#define __VERSION_STR_(x) #x
#define __VERSION_STR(x) __VERSION_STR_(x)



/**
 * \flags macro var
 * \name SLL_VERSION_MAJOR
 * \group version
 * \desc Docs!
 * \type sll_version_t
 */
#define SLL_VERSION_MAJOR 0



/**
 * \flags macro var
 * \name SLL_VERSION_MINOR
 * \group version
 * \desc Docs!
 * \type sll_version_t
 */
#define SLL_VERSION_MINOR 7



/**
 * \flags macro var
 * \name SLL_VERSION_PATCH
 * \group version
 * \desc Docs!
 * \type sll_version_t
 */
#define SLL_VERSION_PATCH 6



/**
 * \flags macro var
 * \name SLL_VERSION_SHA
 * \group version
 * \desc Docs!
 * \type const char*
 */
/**
 * \flags macro var
 * \name SLL_VERSION_FULL_SHA
 * \group version
 * \desc Docs!
 * \type const char*
 */
/**
 * \flags macro var
 * \name SLL_VERSION_TAG
 * \group version
 * \desc Docs!
 * \type const char*
 */
/**
 * \flags macro var
 * \name SLL_VERSION_HAS_SHA
 * \group version
 * \desc Docs!
 * \type sll_bool_t
 */
#ifdef __SLL_SHA__
#define SLL_VERSION_SHA __SLL_SHA__
#define SLL_VERSION_FULL_SHA __SLL_FULL_SHA__
#define SLL_VERSION_TAG "sll-v"SLL_VERSION_STRING
#define SLL_VERSION_HAS_SHA 1
#else
#define SLL_VERSION_SHA ""
#define SLL_VERSION_FULL_SHA ""
#define SLL_VERSION_TAG ""
#define SLL_VERSION_HAS_SHA 0
#endif



/**
 * \flags macro var
 * \name SLL_VERSION_BUILD_TIME
 * \group version
 * \desc Docs!
 * \type sll_time_t
 */
#define SLL_VERSION_BUILD_TIME __SLL_TIME_RAW__



/**
 * \flags macro var
 * \name SLL_VERSION_STRING
 * \group version
 * \desc Docs!
 * \type const char*
 */
#define SLL_VERSION_STRING __VERSION_STR(SLL_VERSION_MAJOR)"."__VERSION_STR(SLL_VERSION_MINOR)"."__VERSION_STR(SLL_VERSION_PATCH)



/**
 * \flags macro var
 * \name SLL_VERSION
 * \group version
 * \desc Docs!
 * \type sll_version_t
 */
#define SLL_VERSION ((sll_version_t)((SLL_VERSION_MAJOR<<24)|(SLL_VERSION_MINOR<<16)|SLL_VERSION_PATCH))



/**
 * \flags func macro
 * \name SLL_GET_MAJOR
 * \group version
 * \desc Docs!
 * \arg sll_version_t v
 * \ret sll_version_t
 */
#define SLL_GET_MAJOR(v) ((v)>>24)



/**
 * \flags func macro
 * \name SLL_GET_MINOR
 * \group version
 * \desc Docs!
 * \arg sll_version_t v
 * \ret sll_version_t
 */
#define SLL_GET_MINOR(v) (((v)>>16)&0xff)



/**
 * \flags func macro
 * \name SLL_GET_PATCH
 * \group version
 * \desc Docs!
 * \arg sll_version_t v
 * \ret sll_version_t
 */
#define SLL_GET_PATCH(v) ((v)&0xffff)



/**
 * \flags check_output func
 * \name sll_version
 * \group version
 * \desc Docs!
 * \ret sll_version_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_version_t sll_version(void);



#endif
