#ifndef __SLL__INTERNAL_API_MATH_H__
#define __SLL__INTERNAL_API_MATH_H__ 1



#define PUSH_FACTOR(v,c) \
	do{ \
		i++; \
		out=sll_reallocate(out,i*sizeof(sll_factor_t)); \
		(out+i-1)->number=v; \
		(out+i-1)->power=c; \
	} while (0)

#define WHEEL_STEP(x) \
	if (!(n%f)){ \
		j=0; \
		do{ \
			j++; \
			n/=f; \
		} while (!(n%f)); \
		PUSH_FACTOR(f,j); \
	} \
	f+=x; \
	if (f*f>n){ \
		break; \
	}

#define WHEEL_STEP_EULER(x) \
	if (!(n%f)){ \
		do{ \
			n/=f; \
		} while (!(n%f)); \
		out-=out/f; \
	} \
	f+=x; \
	if (f*f>n){ \
		break; \
	}



#endif
