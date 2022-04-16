#include <sll/common.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/version.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_version_t sll_version(void){
	return SLL_VERSION;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT const sll_char_t* sll_version_sha(void){
	return SLL_CHAR(SLL_VERSION_SHA);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT const sll_char_t* sll_version_string(void){
	return SLL_CHAR(SLL_VERSION_STRING);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT const sll_char_t* sll_version_tag(void){
	return SLL_CHAR(SLL_VERSION_TAG);
}
