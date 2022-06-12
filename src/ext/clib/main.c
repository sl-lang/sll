#include <clib/common.h>
#include <sll.h>



__CLIB_EXTERNAL sll_bool_t SLL_ABI_INIT(sll_version_t version){
	return (version==SLL_VERSION);
}
