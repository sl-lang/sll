def generate_c_api(d_dt,o_fp):
	with open(o_fp,"w") as cf:
		cf.write("// WARNING: This is an auto-generated file. Any changes made to this file might be lost at any moment. Do Not Edit!\n#include <sll/_internal/api.h>\n#include <sll/common.h>\n#include <sll/string.h>\n#include <sll/types.h>\n\n\n\n")
		fn_l=[]
		for k in d_dt["data"]:
			if (k["api_fmt"] is None):
				continue
			cf.write(f"void {k['name']}(void);\n")
			fn_l.append((k["name"],f"{{\n\t\tSLL_CHAR(\"sll:{k['name'][8:]}\"),\n\t\t{k['name']},\n\t\tSLL_CHAR(\"{k['api_fmt']}\")\n\t}}"))
		cf.write(f"\n\n\nstatic const internal_function_t _ifunc_data_ptr[]={{\n\t"+",\n\t".join([e[1] for e in sorted(fn_l,key=lambda e:e[0])])+f"\n}};\n\n\n\nconst sll_function_index_t _ifunc_size={len(fn_l)};\nconst internal_function_t* _ifunc_data=(const internal_function_t*)(&_ifunc_data_ptr);\n")
