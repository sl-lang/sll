ALPHABET="abcdefghijklmnopqrstuvwxyz"
API_CODE_FILE_PATH="src/sll/api/_generated_raw.c"
API_HEADER_FILE_PATH="src/sll/include/sll/generated/api.h"
TYPE_ACCESS_MAP={"I":"$->dt.i","B":"!!($->dt.i)","F":"$->dt.f","C":"$->dt.c","S":"&($->dt.s)","A":"&($->dt.a)","M":"&($->dt.m)","O":"$"}
TYPE_ACCESS_OPT_MAP={"I":"($?$->dt.i:0)","B":"($?!!($->dt.i):0)","F":"($?$->dt.f:0)","C":"($?$->dt.c:SLL_NO_CHAR)","S":"($?&($->dt.s):NULL)","A":"($?&($->dt.a):NULL)","M":"($?&($->dt.m):NULL)","O":"$"}
TYPE_CHECK_MAP={"I":"SLL_OBJECT_GET_TYPE($)==SLL_OBJECT_TYPE_INT","B":"SLL_OBJECT_GET_TYPE($)==SLL_OBJECT_TYPE_INT","F":"SLL_OBJECT_GET_TYPE($)==SLL_OBJECT_TYPE_FLOAT","C":"SLL_OBJECT_GET_TYPE($)==SLL_OBJECT_TYPE_CHAR","S":"SLL_OBJECT_GET_TYPE($)==SLL_OBJECT_TYPE_STRING","A":"SLL_OBJECT_GET_TYPE($)==SLL_OBJECT_TYPE_ARRAY","M":"SLL_OBJECT_GET_TYPE($)==SLL_OBJECT_TYPE_MAP"}
TYPE_COPY_MAP={"I":"#=$->dt.i","B":"#=$->dt.i","F":"#=$->dt.f","C":"#=$->dt.c","S":"sll_string_clone(&($->dt.s),#)","A":"sll_array_clone(&($->dt.a),#)","M":"sll_map_clone(&($->dt.m),#)"}
TYPE_FULL_NAME_MAP={"I":"INT","B":"INT","F":"FLOAT","C":"CHAR","S":"STRING","A":"ARRAY","M":"MAP"}
TYPE_MAP={"I":"sll_integer_t","B":"sll_bool_t","F":"sll_float_t","C":"sll_char_t","S":"sll_string_t*","A":"sll_array_t*","M":"sll_map_t*","O":"sll_object_t*","V":"void"}
TYPE_RETURN_MAP={"I":"return sll_int_to_object(out)","B":"SLL_ACQUIRE(sll_static_int[out]);return sll_static_int[out]","F":"return sll_float_to_object(out)","S":"return sll_string_to_object_nocopy(&out)","A":"sll_object_t* out_o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);out_o->dt.a=out;return out_o","M":"sll_object_t* out_o=sll_create_object(SLL_OBJECT_TYPE_MAP);out_o->dt.m=out;return out_o"}



def generate_c_api(d_dt,api_dt):
	with open(API_HEADER_FILE_PATH,"w") as hf,open(API_CODE_FILE_PATH,"w") as cf:
		hf.write("// WARNING: This is an auto-generated file. Any changes made to this file might be lost at any moment. Do Not Edit!\n#ifndef __SLL_GENERATED_API__\n#define __SLL_GENERATED_API__\n#include <sll/common.h>\n#include <sll/types.h>\n\n\n")
		cf.write("// WARNING: This is an auto-generated file. Any changes made to this file might be lost at any moment. Do Not Edit!\n#include <sll/_internal/api.h>\n#include <sll/_internal/common.h>\n#include <sll/_internal/gc.h>\n#include <sll/api.h>\n#include <sll/array.h>\n#include <sll/common.h>\n#include <sll/generated/api.h>\n#include <sll/ift.h>\n#include <sll/map.h>\n#include <sll/memory.h>\n#include <sll/object.h>\n#include <sll/operator.h>\n#include <sll/static_object.h>\n#include <sll/string.h>\n#include <sll/types.h>\n")
		fn_l=[]
		d_gl=[]
		for k in api_dt:
			cf.write(f"\n\n\n__SLL_API_TYPE_{k['name']} {k['name']}(__SLL_API_ARGS_{k['name']});\n__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* {k['name']}_raw(sll_object_t*const* al,sll_arg_count_t all){{\n")
			a=""
			d_str=""
			pc=""
			end=""
			i=0
			if (len(k["args"])>0):
				for e in k["args"]:
					if (i):
						a+=","
						pc+=","
					at=(TYPE_MAP[e["type"]] if len(e["type"])==1 else "sll_object_t*")
					if (i==len(k["args"])-1 and "var_arg" in k["flag"]):
						t=f"{at+('const' if at[-1]=='*' else '')}* {ALPHABET[i]}"
						a+=f"{t},sll_arg_count_t {ALPHABET[i]}c"
						d_str+=f"\n * \\arg {t} -> {e['desc']}\n * \\arg sll_arg_count_t {ALPHABET[i]}c"
						cf.write(f"\tsll_arg_count_t {ALPHABET[i]}c={(f'(all>{i}?all-{i}:0)' if i else 'all')};\n")
						if (e["type"]=="O"):
							cf.write(f"\tsll_object_t*const* {ALPHABET[i]}=al+{i};\n")
						else:
							cf.write(f"\t{at}* {ALPHABET[i]}=sll_allocate_stack({ALPHABET[i]}c*sizeof({at}));\n\tfor (sll_arg_count_t idx=0;idx<{ALPHABET[i]}c;idx++){{\n\t\tsll_object_t* tmp=*(al+idx+{i});\n\t\tif (!({TYPE_CHECK_MAP[e['type']].replace('$','tmp')})){{\n\t\t\ttmp=sll_operator_cast(tmp,sll_static_int[SLL_OBJECT_TYPE_{TYPE_FULL_NAME_MAP[e['type']]}]);\n\t\t\t{TYPE_COPY_MAP[e['type']].replace('#','*('+ALPHABET[i]+'+idx)').replace('$','tmp')};\n\t\t\tsll_release_object(tmp);\n\t\t}}\n\t\telse{{\n\t\t\t*({ALPHABET[i]}+idx)={TYPE_ACCESS_MAP[e['type']].replace('$','tmp')};\n\t\t}}\n\t}}\n")
							end=f"\tsll_deallocate(PTR({ALPHABET[i]}));\n"+end
						pc+=ALPHABET[i]+","+ALPHABET[i]+"c"
						i=-1
					else:
						t=f"{at} {ALPHABET[i]}"
						a+=t
						d_str+=f"\n * \\arg {t} -> {e['desc']}"
						cf.write(f"\tsll_object_t* {ALPHABET[i]}=NULL;\n\tif (all>{i}){{\n\t\t{ALPHABET[i]}=*(al+{i});\n")
						end=f"\tsll_release_object({ALPHABET[i]});\n"+end
						if ("O" not in e["type"]):
							cf.write("\t\tif ("+"||".join([TYPE_CHECK_MAP[se].replace('$',ALPHABET[i]) for se in e["type"]])+f"){{\n\t\t\tSLL_ACQUIRE({ALPHABET[i]});\n\t\t}}\n\t\telse{{\n\t\t\t{ALPHABET[i]}=sll_operator_cast({ALPHABET[i]},sll_static_int[SLL_OBJECT_TYPE_{TYPE_FULL_NAME_MAP[e['type'][0]]}]);\n\t\t}}\n")
						else:
							cf.write(f"\t\tSLL_ACQUIRE({ALPHABET[i]});\n")
						cf.write(f"\t}}\n\telse{{\n\t\t{ALPHABET[i]}=")
						if (e["type"][0] in ["I","B","O"]):
							cf.write("SLL_ACQUIRE_STATIC_INT(0);")
						elif (e["type"][0]=="F"):
							cf.write("SLL_ACQUIRE_STATIC(float_zero);")
						elif (e["type"][0]=="C"):
							cf.write("SLL_FROM_CHAR(0);")
						elif (e["type"][0]=="S"):
							cf.write("sll_string_to_object(NULL);")
						elif (e["type"][0]=="A"):
							cf.write(f"sll_create_object(SLL_OBJECT_TYPE_ARRAY);\n\t\tsll_array_create(0,&({ALPHABET[i]}->dt.a));")
						else:
							if (e["type"][0]!="M"):
								raise RuntimeError
							cf.write(f"sll_create_object(SLL_OBJECT_TYPE_MAP);\n\t\tsll_map_create(0,&({ALPHABET[i]}->dt.m));")
						cf.write("\n\t}\n")
						pc+=(TYPE_ACCESS_MAP[e["type"]].replace("$",ALPHABET[i]) if len(e["type"])==1 else ALPHABET[i])
						i+=1
			hf.write(f"\n#define __SLL_API_TYPE_{k['name']} ")
			if (i):
				cf.write(f"\tsll_arg_count_t idx=0;\n\tfor (;idx<{('all' if i==-1 else f'(all<{i}?all:{i})')};idx++){{\n\t\tGC_LOCK(*(al+idx));\n\t}}\n")
				end="\twhile (idx){\n\t\tidx--;\n\t\tGC_UNLOCK(*(al+idx));\n\t}\n"+end
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
			hf.write(f"\n#define __SLL_API_ARGS_{k['name']} {a}\n/**\n * \\flags {('check_output ' if TYPE_MAP[k['ret']['type']][-1]!='*' else '')}func\n * \\name {k['name']}\n * \\group {k['group']}{sg}\n * \\desc {k['desc']}{d_str}\n */\n/**\n * \\flags check_output func\n * \\name {k['name']}_raw\n * \\group raw-api\n * \\subgroup raw-api-{k['group']}\n * \\desc Wrapper function for :{k['name']}:\n * \\arg sll_object_t*const* al -> Arguments\n * \\arg sll_arg_count_t all -> Argument count\n * \\ret sll_object_t* -> The return value of the function\n */")
			if (k["group"] not in d_gl):
				d_gl.append(k["group"])
				hf.write(f"\n/**\n * \\flags subgroup\n * \\name {d_dt['groups'][k['group']]['name'][:-3].strip()}\n * \\group raw-api\n * \\subgroup raw-api-{k['group']}\n * \\desc Docs!\n */")
			hf.write("\n\n\n")
			cf.write("}\n")
			fn_l.append(f"{{\n\t\tSLL_CHAR(\"sll:{k['name'][8:]}\"),\n\t\t{k['name']}_raw\n\t}}")
		hf.write("\n#endif\n")
		cf.write(f"\n\n\nstatic const internal_function_t _ifunc_data_ptr[]={{\n\t"+",\n\t".join(fn_l)+f"\n}};\n\n\n\nconst sll_function_index_t _ifunc_size={len(fn_l)};\nconst internal_function_t* _ifunc_data=(const internal_function_t*)(&_ifunc_data_ptr);\n")
