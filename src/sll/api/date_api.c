#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/math.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_date_from_time(sll_float_t tm,sll_date_t* o){
	// Based on http://howardhinnant.github.io/date_algorithms.html#civil_from_days
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
	return;
}



__SLL_EXTERNAL void sll_date_from_time_ns(sll_size_t tm,sll_date_t* o){
	sll_date_from_time(tm*1e-9,o);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_date_to_time(sll_date_t* dt){
	// Based on http://howardhinnant.github.io/date_algorithms.html#days_from_civil
	sll_size_t y=dt->y;
	if (dt->m<2){
		y--;
	}
	sll_size_t e=y/400;
	y-=e*400;
	return (e*146097+y*365+y/4-y/100+(153*(dt->m+1+(dt->m+1>2?-3:9))+2)/5+dt->d+1-1-719468)*86400+dt->h*3600+dt->mn*60+dt->s;
}



__API_FUNC(date_merge){
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
		(f<0?0:(f>60-sll_float_compare_error?60-sll_float_compare_error:f))
	};
	return sll_date_to_time(&dt);
}



__API_FUNC(date_split){
	sll_date_t dt;
	sll_date_from_time(a,&dt);
	sll_array_create(7,out);
	out->v[0]=SLL_FROM_INT(dt.y);
	out->v[1]=SLL_ACQUIRE_STATIC_INT(dt.m);
	out->v[2]=SLL_ACQUIRE_STATIC_INT(dt.d);
	out->v[3]=SLL_ACQUIRE_STATIC_INT(dt.wd);
	out->v[4]=SLL_ACQUIRE_STATIC_INT(dt.h);
	out->v[5]=SLL_ACQUIRE_STATIC_INT(dt.mn);
	out->v[6]=SLL_FROM_FLOAT(dt.s);
}
