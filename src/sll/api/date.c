#include <sll/_internal/common.h>
#include <sll/api/date.h>
#include <sll/api/math.h>
#include <sll/api/time.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/new_object.h>
#include <sll/platform/time.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>



static const sll_time_zone_t _date_utc_time_zone={"GMT",0};
static const sll_day_t _date_days[12]={31,28,31,30,31,30,30,31,30,31,30,31};



__SLL_EXTERNAL const sll_time_zone_t* sll_utc_time_zone=&_date_utc_time_zone;



__SLL_EXTERNAL __SLL_API_CALL void sll_api_date_get_time_zone(sll_array_t* out){
	sll_new_object_array(SLL_CHAR("iS"),out,sll_platform_time_zone->offset,sll_platform_time_zone->name);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_date_merge(sll_year_t year,sll_month_t month,sll_day_t day,sll_hour_t hour,sll_minute_t minute,sll_second_t second){
	if (month>11){
		month=11;
	}
	sll_day_t n=_date_days[month];
	if (month==1&&!(year&3)&&(year%100||!(year%400))){
		n++;
	}
	sll_date_t dt={
		year,
		month,
		(day>n?n:day),
		0,
		(hour>23?23:hour),
		(minute>59?59:minute),
		(second<0?0:(second>=60?59.999999:second)),
		_date_utc_time_zone
	};
	return sll_date_to_time(&dt);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_date_split(sll_float_t time,sll_array_t* out){
	sll_date_t dt;
	sll_date_from_time(time,NULL,&dt);
	sll_new_object_array(SLL_CHAR("hhhhhhf"),out,dt.year,dt.month,dt.day,dt.week_day,dt.hour,dt.minute,dt.second);
}



__SLL_EXTERNAL void sll_date_from_time(sll_float_t time,const sll_time_zone_t* time_zone,sll_date_t* out){
	// Based on http://howardhinnant.github.io/date_algorithms.html#civil_from_days
	if (!time_zone){
		time_zone=sll_utc_time_zone;
	}
	out->time_zone=*time_zone;
	time+=out->time_zone.offset*60;
	sll_float_t hms=sll_math_mod(time,86400)+(time<0?86400:0);
	out->second=sll_math_mod(hms,60);
	sll_integer_t hms_i=((sll_integer_t)hms)/60;
	out->minute=hms_i%60;
	out->hour=(hms_i/60)%60;
	sll_integer_t v=((sll_integer_t)time)/86400;
	out->week_day=(v+4)%7;
	v+=719468-(time<0?1:0);
	sll_integer_t e=(v-(v<0?146096:0))/146097;
	sll_size_t d=v-e*146097;
	sll_size_t y=(d-d/1460+d/36524-d/146096)/365;
	e=y+e*400;
	if (e>65534){
		out->year=65535;
		out->month=11;
		out->day=30;
		out->week_day=6;
		out->hour=23;
		out->minute=59;
		out->second=59.999999;
		return;
	}
	out->year=(sll_year_t)e;
	d-=y*365+y/4-y/100;
	sll_size_t m=(d*5+2)/153;
	d=d-(m*153+2)/5;
	SLL_ASSERT(d<31);
	out->day=(sll_day_t)d;
	if (m<10){
		m+=2;
	}
	else{
		m-=10;
	}
	if (m<2){
		out->year++;
	}
	SLL_ASSERT(m<12);
	out->month=(sll_month_t)m;
}



__SLL_EXTERNAL void sll_date_from_time_ns(sll_time_t time,const sll_time_zone_t* time_zone,sll_date_t* out){
	sll_date_from_time(time*1e-9,time_zone,out);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_date_to_time(sll_date_t* date){
	// Based on http://howardhinnant.github.io/date_algorithms.html#days_from_civil
	sll_size_t y=date->year;
	if (date->month<2){
		y--;
	}
	sll_size_t e=y/400;
	y-=e*400;
	return (e*146097+y*365+y/4-y/100+(153*(date->month+1+(date->month+1>2?-3:9))+2)/5+date->day+1-1-719468)*86400+date->hour*3600+date->minute*60+date->second-date->time_zone.offset*60;
}
