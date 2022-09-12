#include <sll.h>
#include <windows.h>



__DS4_EXTERNAL sll_bool_t SLL_ABI_INIT(sll_version_t version){
	return (version==SLL_VERSION);
}
