def generate(d_dt,o_fp):
	with open(o_fp,"w") as cf:
		api_fn=sorted([(k["name"],k["api_fmt"]) for k in d_dt["data"] if k["api_fmt"] is not None])
		cf.write("#include <sll/common.h>\n#include <sll/ift.h>\n#include <sll/string.h>\n#include <sll/types.h>\n\n\n\n"+"".join([f"extern void {k[0]}(void);\n" for k in api_fn])+"\n\n\nstatic const sll_internal_function_descriptor_t _ifunc_data_ptr[]={\n\t"+",\n\t".join([f"{{\n\t\tSLL_CHAR(\"sll:{k[0][8:]}\"),\n\t\t{k[0]},\n\t\tSLL_CHAR(\"{k[1]}\")\n\t}}" for k in api_fn])+f"\n}};\n\n\n\n__SLL_EXTERNAL const sll_internal_function_descriptor_t* sll_builtin_internal_function_data=(const sll_internal_function_descriptor_t*)(&_ifunc_data_ptr);\n__SLL_EXTERNAL const sll_function_index_t sll_builtin_internal_function_count={len(api_fn)};\n")
