#ifndef __SLL___COVERAGE_H__
#define __SLL___COVERAGE_H__ 1
#include <stdint.h>



typedef struct ____COVERAGE_DATA{
	uint16_t fp;
	uint16_t nm;
	uint32_t s;
	uint32_t e;
} __coverage_data_t;



extern uint32_t __coverage_count;



extern uint32_t __coverage_map_length;



extern uint64_t __coverage_map[];



extern __coverage_data_t __coverage_data[];



extern const char* __coverage_strings[];



#endif
