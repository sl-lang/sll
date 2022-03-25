ALPHABET="abcdefghijklmnopqrstuvwxyz"
API_CODE_FILE_PATH="src/sll/api/_generated_raw.c"
API_HEADER_FILE_PATH="src/sll/include/sll/generated/api.h"
FORMAT_MAP={"I":"i","B":"b","F":"f","C":"c","S":"s","A":"a","M":"m","O":"o","SC":"y","FI":"x"}
TYPE_MAP={"I":"sll_integer_t","B":"sll_bool_t","F":"sll_float_t","C":"sll_char_t","S":"sll_string_t","A":"sll_array_t","M":"sll_map_t","O":"sll_object_t*","V":"void","SC":"sll_char_string_t","FI":"sll_int_float_t"}
TYPE_PTR=["S","A","M","SC","FI"]
TYPE_PTR_NO_FMT=["SC","FI"]
TYPE_RETURN_MAP={"I":"return sll_int_to_object(out)","B":"SLL_ACQUIRE(sll_static_int[out]);return sll_static_int[out]","F":"return sll_float_to_object(out)","S":"return sll_string_to_object_nocopy(&out)","A":"sll_object_t* out_o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);out_o->dt.a=out;return out_o","M":"sll_object_t* out_o=sll_create_object(SLL_OBJECT_TYPE_MAP);out_o->dt.m=out;return out_o"}



def generate_c_api(d_dt,api_dt):
	with open(API_HEADER_FILE_PATH,"w") as hf,open(API_CODE_FILE_PATH,"w") as cf:
		hf.write("// WARNING: This is an auto-generated file. Any changes made to this file might be lost at any moment. Do Not Edit!\n#ifndef __SLL_GENERATED_API__\n#define __SLL_GENERATED_API__\n#include <sll/common.h>\n#include <sll/types.h>\n\n\n")
		cf.write("// WARNING: This is an auto-generated file. Any changes made to this file might be lost at any moment. Do Not Edit!\n#include <sll/_internal/api.h>\n#include <sll/_internal/common.h>\n#include <sll/_internal/gc.h>\n#include <sll/api.h>\n#include <sll/array.h>\n#include <sll/common.h>\n#include <sll/generated/api.h>\n#include <sll/ift.h>\n#include <sll/map.h>\n#include <sll/memory.h>\n#include <sll/object.h>\n#include <sll/operator.h>\n#include <sll/parse_args.h>\n#include <sll/static_object.h>\n#include <sll/string.h>\n#include <sll/types.h>\n")
		fn_l=[]
		d_gl=[]
		for k in api_dt:
			cf.write(f"\n\n\n__SLL_API_TYPE_{k['name']} {k['name']}(__SLL_API_ARGS_{k['name']});\n__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* {k['name']}_raw(sll_object_t*const* al,sll_arg_count_t all){{\n")
			args=""
			docs=""
			call_args=""
			fmt=""
			fmt_args=""
			if (len(k["args"])>0):
				for i,e in enumerate(k["args"]):
					if (i):
						args+=","
						call_args+=","
					fmt_args+=","
					at=TYPE_MAP[e["type"]]
					ptr=("*" if e["type"] in TYPE_PTR else "")
					fmt+=FORMAT_MAP[e["type"]]
					fmt_args+="&"+ALPHABET[i]
					if (i==len(k["args"])-1 and "var_arg" in k["flag"]):
						t=f"{at+('*const' if ptr else '')}* {ALPHABET[i]}"
						args+=f"{t},sll_arg_count_t {ALPHABET[i]}c"
						docs+=f"\n * \\arg {t} -> {e['desc']}\n * \\arg sll_arg_count_t {ALPHABET[i]}c"
						call_args+=ALPHABET[i]+","+ALPHABET[i]+"c"
						fmt+="+"
						fmt_args+=f",&{ALPHABET[i]}c"
						cf.write(f"\t{t};\n\tsll_arg_count_t {ALPHABET[i]}c;\n")
					elif (e["type"] in TYPE_PTR_NO_FMT):
						t=f"{at+ptr} {ALPHABET[i]}"
						args+=t
						call_args+="&"+ALPHABET[i]
						docs+=f"\n * \\arg {t} -> {e['desc']}"
						cf.write(f"\t{at} {ALPHABET[i]};\n")
					else:
						t=f"{at+ptr} {ALPHABET[i]}"
						args+=t
						call_args+=ALPHABET[i]
						docs+=f"\n * \\arg {t} -> {e['desc']}"
						cf.write(f"\t{t};\n")
			hf.write(f"\n#define __SLL_API_TYPE_{k['name']} ")
			if (len(fmt)):
				cf.write(f"\tsll_parse_args(SLL_CHAR(\"{fmt}\"),al,all{fmt_args});\n")
			if (k["ret"]["type"]=="O"):
				docs+=f"\n * \\ret sll_object_t*"
				hf.write("__SLL_CHECK_OUTPUT sll_object_t*")
				cf.write(f"\tsll_object_t* out={k['name']}({call_args});\n\treturn out;\n")
			elif (k["ret"]["type"]=="V"):
				hf.write("void")
				cf.write(f"\t{k['name']}({call_args});\n\treturn SLL_ACQUIRE_STATIC_INT(0);\n")
			else:
				if (k["ret"]["type"] in TYPE_PTR):
					if (len(call_args)>0):
						args+=","
						call_args+=","
					args+=TYPE_MAP[k["ret"]["type"]]+"* out"
					hf.write("void")
					cf.write(f"\t{TYPE_MAP[k['ret']['type']]} out;\n\t{k['name']}({call_args}&out);")
				else:
					docs+=f"\n * \\ret {TYPE_MAP[k['ret']['type']]}"
					hf.write("__SLL_CHECK_OUTPUT "+TYPE_MAP[k["ret"]["type"]])
					cf.write("\t"+TYPE_MAP[k["ret"]["type"]]+f" out={k['name']}({call_args});")
				cf.write(f"\n\t"+TYPE_RETURN_MAP[k["ret"]["type"]].replace(";",";\n\t")+";\n")
			if (len(args)==0):
				a="void"
			sg=("" if k["subgroup"] is None else f"\n\\subgroup {k['subgroup']}")
			hf.write(f"\n#define __SLL_API_ARGS_{k['name']} {args}\n/**\n * \\flags {('check_output ' if TYPE_MAP[k['ret']['type']][-1]!='*' else '')}func\n * \\name {k['name']}\n * \\group {k['group']}{sg}\n * \\desc {k['desc']}{docs}\n */\n/**\n * \\flags check_output func\n * \\name {k['name']}_raw\n * \\group raw-api\n * \\subgroup raw-api-{k['group']}\n * \\desc Wrapper function for :{k['name']}:\n * \\arg sll_object_t*const* al -> Arguments\n * \\arg sll_arg_count_t all -> Argument count\n * \\ret sll_object_t* -> The return value of the function\n */")
			if (k["group"] not in d_gl):
				d_gl.append(k["group"])
				hf.write(f"\n/**\n * \\flags subgroup\n * \\name {d_dt['groups'][k['group']]['name'][:-3].strip()}\n * \\group raw-api\n * \\subgroup raw-api-{k['group']}\n * \\desc Docs!\n */")
			hf.write("\n\n\n")
			cf.write("}\n")
			fn_l.append(f"{{\n\t\tSLL_CHAR(\"sll:{k['name'][8:]}\"),\n\t\t{k['name']}_raw\n\t}}")
		hf.write("\n#endif\n")
		cf.write(f"\n\n\nstatic const internal_function_t _ifunc_data_ptr[]={{\n\t"+",\n\t".join(fn_l)+f"\n}};\n\n\n\nconst sll_function_index_t _ifunc_size={len(fn_l)};\nconst internal_function_t* _ifunc_data=(const internal_function_t*)(&_ifunc_data_ptr);\n")
