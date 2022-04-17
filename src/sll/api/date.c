#include <sll/_internal/common.h>
#include <sll/api/float.h>
#include <sll/api/math.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/new_object.h>
#include <sll/platform/util.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>



static const sll_time_zone_t _date_utc_time_zone={"GMT",0};



__SLL_EXTERNAL const sll_time_zone_t* sll_utc_time_zone=&_date_utc_time_zone;



__SLL_EXTERNAL void sll_date_from_time(sll_float_t tm,const sll_time_zone_t* tz,sll_date_t* o){
	// Based on http://howardhinnant.github.io/date_algorithms.html#civil_from_days
	if (!tz){
		tz=sll_utc_time_zone;
	}
	o->tz=*tz;
	tm+=o->tz.off*60;
	sll_float_t hms=sll_math_mod(tm,86400)+(tm<0?86400:0);
	o->s=sll_math_mod(hms,60);
	sll_integer_t hms_i=((sll_integer_t)hms)/60;
	o->mn=hms_i%60;
	o->h=(hms_i/60)%60;
	sll_integer_t v=((sll_integer_t)tm)/86400;
	o->wd=(v+4)%7;
	v+=719468-(tm<0?1:0);
	sll_integer_t e=(v-(v<0?146096:0))/146097;
	sll_size_t d=v-e*146097;
	sll_size_t y=(d-d/1460+d/36524-d/146096)/365;
	e=y+e*400;
	if (e>65534){
		o->y=65535;
		o->m=11;
		o->d=30;
		o->wd=6;
		o->h=23;
		o->mn=59;
		o->s=59.999999;
		return;
	}
	o->y=(sll_year_t)e;
	d-=y*365+y/4-y/100;
	sll_size_t m=(d*5+2)/153;
	d=d-(m*153+2)/5;
	SLL_ASSERT(d<31);
	o->d=(sll_day_t)d;
	if (m<10){
		m+=2;
	}
	else{
		m-=10;
	}
	if (m<2){
		o->y++;
	}
	SLL_ASSERT(m<12);
	o->m=(sll_month_t)m;
}



__SLL_EXTERNAL void sll_date_from_time_ns(sll_size_t tm,const sll_time_zone_t* tz,sll_date_t* o){
	sll_date_from_time(tm*1e-9,tz,o);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_date_to_time(sll_date_t* dt){
	// Based on http://howardhinnant.github.io/date_algorithms.html#days_from_civil
	sll_size_t y=dt->y;
	if (dt->m<2){
		y--;
	}
	sll_size_t e=y/400;
	y-=e*400;
	return (e*146097+y*365+y/4-y/100+(153*(dt->m+1+(dt->m+1>2?-3:9))+2)/5+dt->d+1-1-719468)*86400+dt->h*3600+dt->mn*60+dt->s-dt->tz.off*60;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_date_get_time_zone(sll_array_t* out){
	sll_new_object_array(SLL_CHAR("iS"),out,sll_platform_time_zone->off,sll_platform_time_zone->nm);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_date_merge(sll_integer_t a,sll_integer_t b,sll_integer_t c,sll_integer_t d,sll_integer_t e,sll_float_t f){
	sll_day_t n[12]={31,28,31,30,31,30,30,31,30,31,30,31};
	a=(a<0?0:(a>65535?65535:a));
	b=(b<0?0:(b>11?11:b));
	if (b==1&&!(a&3)&&(a%100||!(a%400))){
		n[1]++;
	}
	sll_date_t dt={
		(sll_year_t)a,
		(sll_month_t)b,
		(sll_day_t)(c<0?0:(c>n[b]?n[b]:c)),
		0,
		(sll_hour_t)(d<0?0:(d>23?23:d)),
		(sll_minute_t)(e<0?0:(e>59?59:e)),
		(f<0?0:(f>60-sll_float_compare_error?60-sll_float_compare_error:f)),
		_date_utc_time_zone
	};
	return sll_date_to_time(&dt);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_date_split(sll_float_t a,sll_array_t* out){
	sll_date_t dt;
	sll_date_from_time(a,NULL,&dt);
	sll_new_object_array(SLL_CHAR("hhhhhhf"),out,dt.y,dt.m,dt.d,dt.wd,dt.h,dt.mn,dt.s);
}
