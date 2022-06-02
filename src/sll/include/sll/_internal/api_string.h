#ifndef __SLL__INTERNAL_API_STRING_H__
#define __SLL__INTERNAL_API_STRING_H__ 1
#include <sll/_internal/common.h>



typedef struct _ADDRESS_LIST{
	addr_t addr;
	struct _ADDRESS_LIST* next;
} address_list_t;



#endif
