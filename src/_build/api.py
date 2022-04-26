def generate(d_dt,o_fp):
	with open(o_fp,"w") as cf:
		cf.write("#include <sll/common.h>\n#include <sll/string.h>\n#include <sll/types.h>\n\n\n\n")
		fn_l=[]
		for k in d_dt["data"]:
			if (k["api_fmt"] is None):
				continue
			cf.write(f"extern void {k['name']}(void);\n")
			fn_l.append((k["name"],f"{{\n\t\tSLL_CHAR(\"sll:{k['name'][8:]}\"),\n\t\t{k['name']},\n\t\tSLL_CHAR(\"{k['api_fmt']}\")\n\t}}"))
		cf.write(f"\n\n\nstatic const sll_internal_function_descriptor_t _ifunc_data_ptr[]={{\n\t"+",\n\t".join([e[1] for e in sorted(fn_l,key=lambda e:e[0])])+f"\n}};\n\n\n\n__SLL_EXTERNAL const sll_internal_function_descriptor_t* sll_builtin_internal_function_data=(const sll_internal_function_descriptor_t*)(&_ifunc_data_ptr);\n__SLL_EXTERNAL const sll_function_index_t sll_builtin_internal_function_count={len(fn_l)};\n")
