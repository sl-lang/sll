ALPHABET="abcdefghijklmnopqrstuvwxyz"
API_CODE_FILE_PATH="src/sll/api/_generated.c"
API_HEADER_FILE_PATH="src/sll/include/sll/api/_generated.h"
TYPE_ACCESS_MAP={"I":"$->dt.i","B":"!!($->dt.i)","F":"$->dt.f","C":"$->dt.c","S":"&($->dt.s)","A":"&($->dt.a)","M":"&($->dt.m)","O":"$"}
TYPE_ACCESS_OPT_MAP={"I":"($?$->dt.i:0)","B":"($?!!($->dt.i):0)","F":"($?$->dt.f:0)","C":"($?$->dt.c:SLL_NO_CHAR)","S":"($?&($->dt.s):NULL)","A":"($?&($->dt.a):NULL)","M":"($?&($->dt.m):NULL)","O":"$"}
TYPE_CHECK_MAP={"I":"SLL_OBJECT_GET_TYPE($)==SLL_OBJECT_TYPE_INT","B":"SLL_OBJECT_GET_TYPE($)==SLL_OBJECT_TYPE_INT","F":"SLL_OBJECT_GET_TYPE($)==SLL_OBJECT_TYPE_FLOAT","C":"SLL_OBJECT_GET_TYPE($)==SLL_OBJECT_TYPE_CHAR","S":"SLL_OBJECT_GET_TYPE($)==SLL_OBJECT_TYPE_STRING","A":"SLL_OBJECT_GET_TYPE($)==SLL_OBJECT_TYPE_ARRAY","M":"SLL_OBJECT_GET_TYPE($)==SLL_OBJECT_TYPE_MAP"}
TYPE_COPY_MAP={"I":"#=$->dt.i","B":"#=$->dt.i","F":"#=$->dt.f","C":"#=$->dt.c","S":"sll_string_clone(&($->dt.s),#)","A":"sll_array_clone(&($->dt.a),#)","M":"sll_map_clone(&($->dt.m),#)"}
TYPE_FULL_NAME_MAP={"I":"INT","B":"INT","F":"FLOAT","C":"CHAR","S":"STRING","A":"ARRAY","M":"MAP"}
TYPE_MAP={"I":"sll_integer_t","B":"sll_bool_t","F":"sll_float_t","C":"sll_char_t","S":"sll_string_t*","A":"sll_array_t*","M":"sll_map_t*","O":"sll_object_t*","V":"void"}
TYPE_RETURN_MAP={"I":"return SLL_FROM_INT(out)","B":"SLL_ACQUIRE(sll_static_int[out]);return sll_static_int[out]","F":"return SLL_FROM_FLOAT(out)","S":"sll_object_t* out_o=SLL_CREATE();out_o->t=SLL_OBJECT_TYPE_STRING;out_o->dt.s=out;return out_o","A":"sll_object_t* out_o=SLL_CREATE();out_o->t=SLL_OBJECT_TYPE_ARRAY;out_o->dt.a=out;return out_o","M":"sll_object_t* out_o=SLL_CREATE();out_o->t=SLL_OBJECT_TYPE_MAP;out_o->dt.m=out;return out_o"}



def generate_c_api(d_dt,api_dt):
	with open(API_HEADER_FILE_PATH,"w") as hf,open(API_CODE_FILE_PATH,"w") as cf:
		hf.write("// WARNING: This is an auto-generated file. Any changes made to this file might be lost at any moment. Do Not Edit!\n#ifndef __SLL_API__GENERATED__\n#define __SLL_API__GENERATED__\n#include <sll/common.h>\n#include <sll/types.h>\n\n\n")
		cf.write("// WARNING: This is an auto-generated file. Any changes made to this file might be lost at any moment. Do Not Edit!\n#include <sll/_sll_internal.h>\n#include <sll/api.h>\n#include <sll/api/_generated.h>\n#include <sll/array.h>\n#include <sll/common.h>\n#include <sll/ift.h>\n#include <sll/map.h>\n#include <sll/memory.h>\n#include <sll/object.h>\n#include <sll/operator.h>\n#include <sll/static_object.h>\n#include <sll/types.h>\n")
		fn_l=[]
		d_gl=[]
		for k in api_dt:
			cf.write(f"\n\n\n__SLL_API_TYPE_{k['name']} {k['name']}(__SLL_API_ARGS_{k['name']});\n__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* {k['name']}_raw(sll_object_t*const* al,sll_arg_count_t all){{\n")
			a=""
			d_str=""
			pc=""
			end=""
			if (len(k["args"])>0):
				for i,e in enumerate(k["args"]):
					if (i):
						a+=","
						pc+=","
					at=(TYPE_MAP[e["type"]] if len(e["type"])==1 else "sll_object_t*")
					if (i==len(k["args"])-1 and "var_arg" in k["flag"]):
						t=f"{at+('const' if at[-1]=='*' else '')}* {ALPHABET[i]}"
						a+=f"{t},sll_arg_count_t {ALPHABET[i]}c"
						d_str+=f"\n * \\arg {t} -> {e['desc']}\n * \\arg sll_arg_count_t {ALPHABET[i]}c"
						sz=(f"(all>{i}?all-{i}:0)" if i else "all")
						if (e["type"]=="O"):
							cf.write(f"\tsll_object_t*const* {ALPHABET[i]}=al+{i};\n\tsll_arg_count_t {ALPHABET[i]}c={sz};\n")
						else:
							cf.write(f"\tsll_arg_count_t {ALPHABET[i]}c={sz};\n\t{at}* {ALPHABET[i]}=sll_allocate({ALPHABET[i]}c*sizeof({at}));\n\tfor (sll_arg_count_t idx=0;idx<{ALPHABET[i]}c;idx++){{\n\t\tsll_object_t* tmp=*(al+idx+{i});\n\t\tif (!({TYPE_CHECK_MAP[e['type']].replace('$','tmp')})){{\n\t\t\ttmp=sll_operator_cast(tmp,sll_static_int[SLL_OBJECT_TYPE_{TYPE_FULL_NAME_MAP[e['type']]}]);\n\t\t\t{TYPE_COPY_MAP[e['type']].replace('#','*('+ALPHABET[i]+'+idx)').replace('$','tmp')};\n\t\t\tSLL_RELEASE(tmp);\n\t\t}}\n\t\telse{{\n\t\t\t*({ALPHABET[i]}+idx)={TYPE_ACCESS_MAP[e['type']].replace('$','tmp')};\n\t\t}}\n\t}}\n")
							end=f"\tsll_deallocate((void*){ALPHABET[i]});\n"+end
						pc+=ALPHABET[i]+","+ALPHABET[i]+"c"
					else:
						t=f"{at} {ALPHABET[i]}"
						a+=t
						d_str+=f"\n * \\arg {t} -> {e['desc']}"
						end=f"\tSLL_RELEASE({ALPHABET[i]});\n"+end
						cf.write(f"\tsll_object_t* {ALPHABET[i]}=NULL;\n\tif (all>{i}){{\n\t\t{ALPHABET[i]}=*(al+{i});\n")
						if ("O" not in e["type"]):
							cf.write("\t\tif ("+"||".join([TYPE_CHECK_MAP[se].replace('$',ALPHABET[i]) for se in e["type"]])+f"){{\n\t\t\tSLL_ACQUIRE({ALPHABET[i]});\n\t\t}}\n\t\telse{{\n\t\t\t{ALPHABET[i]}=sll_operator_cast({ALPHABET[i]},sll_static_int[SLL_OBJECT_TYPE_{TYPE_FULL_NAME_MAP[e['type'][0]]}]);\n\t\t}}\n")
						else:
							cf.write(f"\t\tSLL_ACQUIRE({ALPHABET[i]});\n")
						cf.write("\t}\n\telse{\n\t\t")
						if (e["type"][0] in ["I","B","O"]):
							cf.write(ALPHABET[i]+"=SLL_ACQUIRE_STATIC_INT(0);")
						elif (e["type"][0]=="F"):
							cf.write(ALPHABET[i]+"=SLL_ACQUIRE_STATIC(float_zero);")
						elif (e["type"][0]=="C"):
							cf.write(ALPHABET[i]+"=SLL_ACQUIRE_STATIC_CHAR(0);")
						elif (e["type"][0]=="S"):
							cf.write(f"{ALPHABET[i]}=SLL_CREATE();\n\t\t{ALPHABET[i]}->t=SLL_OBJECT_TYPE_STRING;\n\t\tsll_string_create(0,&({ALPHABET[i]}->dt.s));")
						elif (e["type"][0]=="A"):
							cf.write(f"{ALPHABET[i]}=SLL_CREATE();\n\t\t{ALPHABET[i]}->t=SLL_OBJECT_TYPE_ARRAY;\n\t\tif (!sll_array_create(0,&({ALPHABET[i]}->dt.a))){{\n\t\t\tSLL_UNIMPLEMENTED();\n\t\t}}")
						elif (e["type"][0]=="H"):
							cf.write(f"{ALPHABET[i]}=SLL_CREATE();\n\t\t{ALPHABET[i]}->t=SLL_OBJECT_TYPE_HANDLE;\n\t\tSLL_INIT_HANDLE_DATA(&({ALPHABET[i]}->dt.h));")
						else:
							if (e["type"][0]!="M"):
								raise RuntimeError
							cf.write(f"{ALPHABET[i]}=SLL_CREATE();\n\t\t{ALPHABET[i]}->t=SLL_OBJECT_TYPE_MAP;\n\t\tsll_map_create(0,&({ALPHABET[i]}->dt.m));")
						cf.write("\n\t}\n")
						if (len(e["type"])==1):
							pc+=TYPE_ACCESS_MAP[e["type"]].replace("$",ALPHABET[i])
						else:
							pc+=ALPHABET[i]
			hf.write(f"\n#define __SLL_API_TYPE_{k['name']} ")
			if (k["ret"]["type"]=="O"):
				d_str+=f"\n * \\ret sll_object_t*"
				hf.write("__SLL_CHECK_OUTPUT sll_object_t*")
				cf.write(f"\tsll_object_t* out={k['name']}({pc});\n{end}\treturn out;\n")
			elif (k["ret"]["type"]=="V"):
				hf.write("void")
				cf.write(f"\t{k['name']}({pc});\n{end}\treturn SLL_ACQUIRE_STATIC_INT(0);\n")
			else:
				if (TYPE_MAP[k["ret"]["type"]][-1]=="*"):
					if (len(pc)>0):
						a+=f",{TYPE_MAP[k['ret']['type']]} out"
						pc+=","
					else:
						a=TYPE_MAP[k["ret"]["type"]]+" out"
					hf.write("void")
					cf.write("\t"+TYPE_MAP[k["ret"]["type"]][:-1]+f" out;\n\t{k['name']}({pc}&out);")
				else:
					d_str+=f"\n * \\ret {TYPE_MAP[k['ret']['type']]}"
					hf.write("__SLL_CHECK_OUTPUT "+TYPE_MAP[k["ret"]["type"]])
					cf.write("\t"+TYPE_MAP[k["ret"]["type"]]+f" out={k['name']}({pc});")
				cf.write(f"\n{end}\t"+TYPE_RETURN_MAP[k["ret"]["type"]].replace(";",";\n\t")+";\n")
			if (len(a)==0):
				a="void"
			sg=("" if k["subgroup"] is None else f"\n\\subgroup {k['subgroup']}")
			hf.write(f"\n#define __SLL_API_ARGS_{k['name']} {a}\n/**\n * \\flags {('check_output ' if TYPE_MAP[k['ret']['type']][-1]!='*' else '')}func optimizable\n * \\name {k['name']}\n * \\group {k['group']}{sg}\n * \\desc {k['desc']}{d_str}\n */\n/**\n * \\flags check_output func optimizable\n * \\name {k['name']}_raw\n * \\group raw-api\n * \\subgroup raw-api-{k['group']}\n * \\desc Wrapper function for :{k['name']}:\n * \\arg sll_object_t*const* al -> Arguments\n * \\arg sll_arg_count_t all -> Argument count\n * \\ret sll_object_t* -> The return value of the function\n */")
			if (k["group"] not in d_gl):
				d_gl.append(k["group"])
				hf.write(f"\n/**\n * \\flags subgroup\n * \\name {d_dt['groups'][k['group']]['name'][:-3].strip()}\n * \\group raw-api\n * \\subgroup raw-api-{k['group']}\n * \\desc Docs!\n */")
			hf.write("\n\n\n")
			cf.write("}\n")
			fl=""
			if ("optimizable" not in k["flag"]):
				fl="SLL_INTERNAL_FUNCTION_FLAG_REQUIRED"
			if ("compilation_call" in k["flag"]):
				if (len(fl)>0):
					fl+="|"
				fl+="SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL"
			if (len(fl)==0):
				fl="0"
			fn_l.append(f"{{\n\t\t\"sll:{k['name'][8:]}\",\n\t\t{k['name']}_raw,\n\t\t{fl}\n\t}}")
		hf.write("\n#endif\n")
		cf.write(f"\n\n\nstatic const internal_function_t _ifunc_data_ptr[]={{\n\t"+",\n\t".join(fn_l)+f"\n}};\n\n\n\nconst sll_function_index_t _ifunc_size={len(fn_l)};\nconst internal_function_t* _ifunc_data=(const internal_function_t*)(&_ifunc_data_ptr);\n")
