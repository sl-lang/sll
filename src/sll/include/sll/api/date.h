#ifndef __SLL_API_DATE_H__
#define __SLL_API_DATE_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Date API
 * \group date-api
 * \desc Docs!
 */



/**
 * \flags var
 * \name sll_utc_time_zone
 * \group date-api
 * \desc Docs!
 * \type const sll_time_zone_t*
 */
__SLL_EXTERNAL extern const sll_time_zone_t* sll_utc_time_zone;



/**
 * \flags func
 * \name sll_date_from_time
 * \group date-api
 * \desc Docs!
 * \arg sll_float_t tm
 * \arg const sll_time_zone_t* tz
 * \arg sll_date_t* o
 */
__SLL_EXTERNAL void sll_date_from_time(sll_float_t tm,const sll_time_zone_t* tz,sll_date_t* o);



/**
 * \flags func
 * \name sll_date_from_time_ns
 * \group date-api
 * \desc Docs!
 * \arg sll_size_t tm
 * \arg const sll_time_zone_t* tz
 * \arg sll_date_t* o
 */
__SLL_EXTERNAL void sll_date_from_time_ns(sll_size_t tm,const sll_time_zone_t* tz,sll_date_t* o);



/**
 * \flags check_output func
 * \name sll_date_to_time
 * \group date-api
 * \desc Docs!
 * \arg sll_date_t* dt
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_date_to_time(sll_date_t* dt);



/**
 * \flags api func
 * \name sll_api_date_merge
 * \group date-api
 * \desc Docs!
 * \arg I
 * \arg I
 * \arg I
 * \arg I
 * \arg I
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(date_merge);



/**
 * \flags api func
 * \name sll_api_date_split
 * \group date-api
 * \desc Docs!
 * \arg F
 * \ret A
 */
__API_FUNC_DECL(date_split);



#endif
