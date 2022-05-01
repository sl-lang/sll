#include <sll/_internal/common.h>
#include <sll/_internal/platform.h>
#include <sll/error.h>
#include <sll/generated/critical_error.h>
#include <sll/platform/file.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_NO_RETURN void _critical_failure(const sll_char_t* nm,sll_error_t err){
	sll_file_descriptor_t fd=sll_platform_get_default_stream_descriptor(SLL_PLATFORM_STREAM_ERROR);
	sll_platform_file_write(fd,CRITICAL_ERROR_START,CRITICAL_ERROR_START_SIZE,NULL);
	sll_platform_file_write(fd,nm,sll_string_length(nm),NULL);
	if (err!=SLL_NO_ERROR){
		sll_char_t bf[16];
		__SLL_U32 i=16;
		while (i){
			i--;
			bf[i]=(err&15)+48;
			if (bf[i]>57){
				bf[i]+=39;
			}
			err>>=4;
		}
		sll_platform_file_write(fd,bf,16,NULL);
	}
	sll_platform_file_write(fd,CRITICAL_ERROR_END,CRITICAL_ERROR_END_SIZE,NULL);
	_force_exit_platform();
}
