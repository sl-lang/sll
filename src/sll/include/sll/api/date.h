#ifndef __SLL_API_DATE_H__
#define __SLL_API_DATE_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Date API
 * \group date-api
 * \desc Docs!
 */



/**
 * \flags type var
 * \name sll_day_t
 * \group date-api
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_day_t;



/**
 * \flags type var
 * \name sll_hour_t
 * \group date-api
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_hour_t;



/**
 * \flags type var
 * \name sll_minute_t
 * \group date-api
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_minute_t;



/**
 * \flags type var
 * \name sll_month_t
 * \group date-api
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_month_t;



/**
 * \flags type var
 * \name sll_year_t
 * \group date-api
 * \desc Docs!
 * \type __SLL_U16
 */
typedef __SLL_U16 sll_year_t;



/**
 * \flags type
 * \name sll_time_zone_t
 * \group date-api
 * \desc Docs!
 * \arg sll_char_t* nm
 * \arg sll_time_t off
 */
typedef struct _SLL_TIME_ZONE{
    sll_char_t nm[32];
    sll_time_t off;
} sll_time_zone_t;



/**
 * \flags type
 * \name sll_date_t
 * \group date-api
 * \desc Docs!
 * \arg sll_year_t y
 * \arg sll_month_t m
 * \arg sll_day_t d
 * \arg sll_day_t wd
 * \arg sll_hour_t h
 * \arg sll_minute_t mn
 * \arg sll_second_t s
 * \arg sll_time_zone_t tz
 */
typedef struct _SLL_DATE{
    sll_year_t y;
    sll_month_t m;
    sll_day_t d;
    sll_day_t wd;
    sll_hour_t h;
    sll_minute_t mn;
    sll_second_t s;
    sll_time_zone_t tz;
} sll_date_t;



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
 * \name sll_api_date_get_time_zone
 * \group date-api
 * \desc Docs!
 * \api |a
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_date_get_time_zone(sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_api_date_merge
 * \group date-api
 * \desc Docs!
 * \api WBBBBf|f
 * \arg sll_year_t year
 * \arg sll_month_t month
 * \arg sll_day_t day
 * \arg sll_hour_t hour
 * \arg sll_minute_t minute
 * \arg sll_second_t second
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_date_merge(sll_year_t year,sll_month_t month,sll_day_t day,sll_hour_t hour,sll_minute_t minute,sll_second_t second);



/**
 * \flags func
 * \name sll_api_date_split
 * \group date-api
 * \desc Docs!
 * \api f|a
 * \arg sll_float_t time
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_date_split(sll_float_t time,sll_array_t* out);



/**
 * \flags func
 * \name sll_date_from_time
 * \group date-api
 * \desc Docs!
 * \arg sll_float_t time
 * \arg const sll_time_zone_t* tz
 * \arg sll_date_t* o
 */
__SLL_EXTERNAL void sll_date_from_time(sll_float_t time,const sll_time_zone_t* tz,sll_date_t* o);



/**
 * \flags func
 * \name sll_date_from_time_ns
 * \group date-api
 * \desc Docs!
 * \arg sll_time_t time
 * \arg const sll_time_zone_t* tz
 * \arg sll_date_t* o
 */
__SLL_EXTERNAL void sll_date_from_time_ns(sll_time_t time,const sll_time_zone_t* tz,sll_date_t* o);



/**
 * \flags check_output func
 * \name sll_date_to_time
 * \group date-api
 * \desc Docs!
 * \arg sll_date_t* date
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_date_to_time(sll_date_t* date);



#endif
