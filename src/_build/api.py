ALPHABET="abcdefghijklmnopqrstuvwxyz"
API_CODE_FILE_PATH="src/sll/api/_generated_raw.c"
API_HEADER_FILE_PATH="src/sll/include/sll/generated/api.h"
FORMAT_MAP={"B":"b","I":"i","F":"f","IF":"x","C":"c","D":"d","S":"s","CS":"y","A":"a","M":"m","O":"o"}
TYPE_MAP={"B":"sll_bool_t","I":"sll_integer_t","F":"sll_float_t","IF":"sll_int_float_t","C":"sll_char_t","D":"sll_complex_t","S":"sll_string_t","CS":"sll_char_string_t","A":"sll_array_t","M":"sll_map_t","O":"sll_object_t*"}
TYPE_PTR=["IF","D","S","CS","A","M"]
TYPE_CHECK_OUTPUT=["B","I","F","C","O"]
TYPE_PTR_NO_FMT=["IF","CS"]
TYPE_RETURN_MAP={"I":"return sll_int_to_object(out)","B":"SLL_ACQUIRE(sll_static_int[out]);return sll_static_int[out]","F":"return sll_float_to_object(out)","C":"return SLL_FROM_CHAR(out)","D":"return sll_complex_to_object(out)","S":"return STRING_TO_OBJECT_NOCOPY(&out)","A":"return sll_array_to_object_nocopy(&out)","M":"return sll_map_to_object_nocopy(&out)"}
RET_FORMAP_MAP={"B":"b","I":"i","F":"f","C":"c","D":"d","S":"s","A":"a","M":"m","O":"o","V":"v"}



def generate_c_api(d_dt,api_dt):
	with open(API_HEADER_FILE_PATH,"w") as hf,open(API_CODE_FILE_PATH,"w") as cf:
		hf.write("// WARNING: This is an auto-generated file. Any changes made to this file might be lost at any moment. Do Not Edit!\n#ifndef __SLL_GENERATED_API__\n#define __SLL_GENERATED_API__\n#include <sll/common.h>\n#include <sll/types.h>\n\n\n")
		cf.write("// WARNING: This is an auto-generated file. Any changes made to this file might be lost at any moment. Do Not Edit!\n#include <sll/_internal/api.h>\n#include <sll/common.h>\n#include <sll/generated/api.h>\n#include <sll/string.h>\n#include <sll/types.h>\n\n\n\n")
		fn_l=[]
		d_gl=[]
		for k in api_dt:
			cf.write(f"__SLL_API_CALL __SLL_API_TYPE_{k['name']} {k['name']}(__SLL_API_ARGS_{k['name']});\n")
			args=""
			docs=""
			fmt=""
			if (len(k["args"])>0):
				for i,e in enumerate(k["args"]):
					if (i):
						args+=","
					e_type=e["type"]
					arr=False
					if ("+" in e_type):
						e_type=e_type.replace("+","")
						arr=True
					at=TYPE_MAP[e_type]
					ptr=("*" if e_type in TYPE_PTR else "")
					fmt+=FORMAT_MAP[e_type]
					if ((i==len(k["args"])-1 and "var_arg" in k["flag"]) or arr):
						t=f"{at+('*const' if ptr else '')}* {ALPHABET[i]}"
						args+=f"{t},sll_arg_count_t {ALPHABET[i]}c"
						docs+=f"\n * \\arg {t} -> {e['desc']}\n * \\arg sll_arg_count_t {ALPHABET[i]}c"
						fmt+=("+" if arr else "!")
					elif (e_type in TYPE_PTR_NO_FMT):
						t=f"{at+ptr} {ALPHABET[i]}"
						args+=t
						docs+=f"\n * \\arg {t} -> {e['desc']}"
					else:
						t=f"{at+ptr} {ALPHABET[i]}"
						args+=t
						docs+=f"\n * \\arg {t} -> {e['desc']}"
			hf.write(f"\n#define __SLL_API_TYPE_{k['name']} ")
			if (k["ret"]["type"]=="O"):
				docs+=f"\n * \\ret sll_object_t*"
				hf.write("__SLL_CHECK_OUTPUT sll_object_t*")
			elif (k["ret"]["type"]=="V"):
				hf.write("void")
			else:
				if (k["ret"]["type"] in TYPE_PTR):
					if (len(args)>0):
						args+=","
					args+=TYPE_MAP[k["ret"]["type"]]+"* out"
					hf.write("void")
				else:
					docs+=f"\n * \\ret {TYPE_MAP[k['ret']['type']]}"
					hf.write("__SLL_CHECK_OUTPUT "+TYPE_MAP[k["ret"]["type"]])
			if (len(args)==0):
				args="void"
			sg=("" if k["subgroup"] is None else f"\n\\subgroup {k['subgroup']}")
			hf.write(f"\n#define __SLL_API_ARGS_{k['name']} {args}\n/**\n * \\flags {('check_output ' if k['ret']['type'] in TYPE_CHECK_OUTPUT else '')}func\n * \\name {k['name']}\n * \\group {k['group']}{sg}\n * \\desc {k['desc']}{docs}\n */\n/**\n * \\flags check_output func\n * \\name {k['name']}_raw\n * \\group raw-api\n * \\subgroup raw-api-{k['group']}\n * \\desc Wrapper function for :{k['name']}:\n * \\arg sll_object_t*const* al -> Arguments\n * \\arg sll_arg_count_t all -> Argument count\n * \\ret sll_object_t* -> The return value of the function\n */")
			if (k["group"] not in d_gl):
				d_gl.append(k["group"])
				hf.write(f"\n/**\n * \\flags subgroup\n * \\name {d_dt['groups'][k['group']]['name'][:-3].strip()}\n * \\group raw-api\n * \\subgroup raw-api-{k['group']}\n * \\desc Docs!\n */")
			hf.write("\n\n\n")
			fn_l.append(f"{{\n\t\tSLL_CHAR(\"sll:{k['name'][8:]}\"),\n\t\t{k['name']},\n\t\tSLL_CHAR(\"{fmt}|{RET_FORMAP_MAP[k['ret']['type']]}\")\n\t}}")
		hf.write("\n#endif\n")
		cf.write(f"\n\n\nstatic const internal_function_t _ifunc_data_ptr[]={{\n\t"+",\n\t".join(fn_l)+f"\n}};\n\n\n\nconst sll_function_index_t _ifunc_size={len(fn_l)};\nconst internal_function_t* _ifunc_data=(const internal_function_t*)(&_ifunc_data_ptr);\n")
