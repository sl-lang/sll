#ifndef __SLL__INTERNAL_INIT_H__
#define __SLL__INTERNAL_INIT_H__ 1
#include <sll/types.h>



#define MAX_CLEANUP_TABLE_SIZE 512



void _cleanup_vm_exit_tables(void);



__SLL_API_CALL sll_bool_t _check_cpuid_flags(void);



#endif
