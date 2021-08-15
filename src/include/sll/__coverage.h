#ifndef __SLL___COVERAGE_H__
#define __SLL___COVERAGE_H__ 1
#include <stdint.h>



#define __COVERAGE_DATA_TYPE_BREAK 0
#define __COVERAGE_DATA_TYPE_CONTINUE 1
#define __COVERAGE_DATA_TYPE_FUNC 2
#define __COVERAGE_DATA_TYPE_FUNC_END 3
#define __COVERAGE_DATA_TYPE_GOTO 4
#define __COVERAGE_DATA_TYPE_RETURN 5



typedef struct ____COVERAGE_DATA{
	uint8_t t;
	uint32_t ln;
	uint16_t fp;
	uint16_t nm;
} __coverage_data_t;



extern uint32_t __coverage_count;



extern uint32_t __coverage_map_length;



extern uint64_t __coverage_map[];



extern __coverage_data_t __coverage_data[];



extern const char* __coverage_strings[];



#endif
