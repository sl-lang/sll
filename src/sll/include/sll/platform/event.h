#ifndef __SLL_PLATFORM_EVENT_H__
#define __SLL_PLATFORM_EVENT_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags subgroup
 * \name Events
 * \group platform
 * \subgroup platform-event
 * \desc Docs!
 */



/**
 * \flags check_output func
 * \name sll_platform_event_create
 * \group platform
 * \subgroup platform-event
 * \desc Docs!
 * \ret sll_event_handle_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_event_handle_t sll_platform_event_create(void);



/**
 * \flags check_output func
 * \name sll_platform_event_delete
 * \group platform
 * \subgroup platform-event
 * \desc Docs!
 * \arg sll_event_handle_t e
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_event_delete(sll_event_handle_t e);



/**
 * \flags check_output func
 * \name sll_platform_event_set
 * \group platform
 * \subgroup platform-event
 * \desc Docs!
 * \arg sll_event_handle_t e
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_event_set(sll_event_handle_t e);



/**
 * \flags check_output func
 * \name sll_platform_event_wait
 * \group platform
 * \subgroup platform-event
 * \desc Docs!
 * \arg sll_event_handle_t e
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_event_wait(sll_event_handle_t e);



#endif
