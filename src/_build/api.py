ALPHABET="abcdefghijklmnopqrstuvwxyz"
API_CODE_FILE_PATH="src/sll/api/_generated.c"
API_HEADER_FILE_PATH="src/include/sll/api/_generated.h"
RETURN_CODE_BASE_TYPE={"0":"I","1":"I","h":"H","Z":"S","f":"F","I":"I","B":"B","F":"F","C":"C","S":"S","E":"A","A":"A","H":"H","M":"M","O":"O"}
RETURN_CODE_TYPE_MAP={"0":"return SLL_ACQUIRE_STATIC_INT(0)","1":"return SLL_ACQUIRE_STATIC_INT(1)","h":"return SLL_ACQUIRE_STATIC(handle_zero)","Z":"return SLL_ACQUIRE_STATIC(str_zero)","f":"return SLL_ACQUIRE_STATIC(float_zero)","E":"return SLL_ACQUIRE_STATIC(array_zero)"}
TYPE_ACCESS_MAP={"I":"$->dt.i","F":"$->dt.f","C":"$->dt.c","S":"&($->dt.s)","A":"&($->dt.a)","H":"&($->dt.h)","M":"&($->dt.m)","O":"$"}
TYPE_ACCESS_OPT_MAP={"I":"($?$->dt.i:0)","F":"($?$->dt.f:0)","C":"($?$->dt.c:SLL_NO_CHAR)","S":"($?&($->dt.s):NULL)","A":"($?&($->dt.a):NULL)","H":"($?&($->dt.h):NULL)","M":"($?&($->dt.m):NULL)","O":"$"}
TYPE_CHECK_MAP={"I":"SLL_OBJECT_GET_TYPE($)==SLL_OBJECT_TYPE_INT","F":"SLL_OBJECT_GET_TYPE($)==SLL_OBJECT_TYPE_FLOAT","C":"SLL_OBJECT_GET_TYPE($)==SLL_OBJECT_TYPE_CHAR","S":"SLL_OBJECT_GET_TYPE($)==SLL_OBJECT_TYPE_STRING","A":"SLL_OBJECT_GET_TYPE($)==SLL_OBJECT_TYPE_ARRAY","H":"(SLL_OBJECT_GET_TYPE($)==SLL_OBJECT_TYPE_HANDLE&&$->dt.h.t!=SLL_HANDLE_UNKNOWN_TYPE)","M":"SLL_OBJECT_GET_TYPE($)==SLL_OBJECT_TYPE_MAP"}
TYPE_MAP={"I":"sll_integer_t","B":"sll_bool_t","F":"sll_float_t","C":"sll_char_t","S":"sll_string_t*","A":"sll_array_t*","H":"sll_handle_data_t*","M":"sll_map_t*","O":"sll_object_t*"}
TYPE_RETURN_MAP={"I":"return SLL_FROM_INT(out)","B":"return SLL_ACQUIRE_STATIC_INT(out)","F":"return SLL_FROM_FLOAT(out)","S":"sll_object_t* out_o=SLL_CREATE();out_o->t=SLL_OBJECT_TYPE_STRING;out_o->dt.s=out;return out_o","A":"sll_object_t* out_o=SLL_CREATE();out_o->t=SLL_OBJECT_TYPE_ARRAY;out_o->dt.a=out;return out_o","H":"sll_object_t* out_o=SLL_CREATE();out_o->t=SLL_OBJECT_TYPE_HANDLE;out_o->dt.h=out;return out_o","M":"sll_object_t* out_o=SLL_CREATE();out_o->t=SLL_OBJECT_TYPE_MAP;out_o->dt.m=out;return out_o"}



def generate_c_api(d_dt,api_dt):
	with open(API_HEADER_FILE_PATH,"w") as hf,open(API_CODE_FILE_PATH,"w") as cf:
		hf.write("// WARNING: This is an auto-generated file. Any changes made to this file might be lost at any moment. Do Not Edit!\n#ifndef __SLL_API__GENERATED__\n#define __SLL_API__GENERATED__\n#include <sll/common.h>\n#include <sll/types.h>\n\n\n")
		cf.write("// WARNING: This is an auto-generated file. Any changes made to this file might be lost at any moment. Do Not Edit!\n#include <sll/_sll_internal.h>\n#include <sll/api.h>\n#include <sll/api/_generated.h>\n#include <sll/common.h>\n#include <sll/handle.h>\n#include <sll/ift.h>\n#include <sll/memory.h>\n#include <sll/object.h>\n#include <sll/static_object.h>\n#include <sll/types.h>\n")
		fn_l=[]
		d_gl=[]
		for k in api_dt:
			rt=RETURN_CODE_BASE_TYPE[k["ret"][0]["type"]]
			for i in range(1,len(k["ret"])):
				m=RETURN_CODE_BASE_TYPE[k["ret"][i]["type"]]
				if ((m=="I" and rt=="B") or (m=="B" and rt=="I")):
					rt="I"
				elif (m!=rt):
					rt="O"
					break
			cf.write(f"\n\n\n__SLL_API_TYPE_{k['name']} {k['name']}(__SLL_API_ARGS_{k['name']});\n__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* {k['name']}_raw(const sll_object_t*const* al,sll_arg_count_t all){{\n")
			a=""
			d_str=""
			pc=""
			end=""
			if (len(k["args"])>0):
				err_c=RETURN_CODE_TYPE_MAP[k["ret"][0]["type"]]
				i=len(k["args"])
				while (i>0 and k["args"][i-1]["opt"]):
					i-=1
				if (i!=0):
					cf.write(f"\tif (all<{i}){{\n\t\t"+err_c.replace(";",";\n\t\t")+";\n\t}\n")
				for i,e in enumerate(k["args"]):
					if (i):
						a+=","
						pc+=","
					at=("sll_read_char_t" if e["type"]=="C" and e["opt"] else (TYPE_MAP[e["type"]] if len(e["type"])==1 else "sll_object_t*"))
					if (i==len(k["args"])-1 and "var_arg" in k["flag"]):
						t=f"const {at+('const' if at[-1]=='*' else '')}* {ALPHABET[i]}"
						a+=f"{t},sll_arg_count_t {ALPHABET[i]}c"
						d_str+=f"\n * \\arg {t} -> {e['desc']}\n * \\arg sll_arg_count_t {ALPHABET[i]}c"
						if (e["type"]=="O"):
							cf.write(f"\tconst sll_object_t*const* {ALPHABET[i]}=al+{i};\n\tsll_arg_count_t {ALPHABET[i]}c=all-{i};\n")
						else:
							cf.write(f"\tsll_arg_count_t {ALPHABET[i]}c=all-{i};\n\tconst {at}* {ALPHABET[i]}=sll_allocate({ALPHABET[i]}c*sizeof({at}));\n\tfor (sll_arg_count_t idx=0;idx<{ALPHABET[i]}c;idx++){{\n\t\tconst sll_object_t* tmp=*(al+idx+{i});\n\t\tif (!({TYPE_CHECK_MAP[e['type']].replace('$','tmp')})){{\n\t\t\tsll_deallocate((void*){ALPHABET[i]});\n\t\t\t"+err_c.replace(";",";\n\t\t\t")+f";\n\t\t}}\n\t\t*({ALPHABET[i]}+idx)={TYPE_ACCESS_MAP[e['type']].replace('$','tmp')};\n\t}}\n")
							end=f"\tsll_deallocate((void*){ALPHABET[i]});\n"
						pc+=ALPHABET[i]+","+ALPHABET[i]+"c"
					else:
						t=f"const {at} {ALPHABET[i]}"
						a+=t
						d_str+=f"\n * \\arg {t} -> {e['desc']}"
						e_tb=""
						if (e["opt"]):
							e_tb="\t"
							cf.write(f"\tconst sll_object_t* {ALPHABET[i]}=NULL;\n\tif (all>{i}){{\n\t\t{ALPHABET[i]}=*(al+{i});\n")
						else:
							cf.write(f"\tconst sll_object_t* {ALPHABET[i]}=*(al+{i});\n")
						if ("O" not in e["type"]):
							cf.write(e_tb+"\tif (!("+"||".join([TYPE_CHECK_MAP[se].replace('$',ALPHABET[i]) for se in e["type"]])+f")){{\n\t\t"+e_tb+err_c.replace(";",";\n\t\t"+e_tb)+f";\n{e_tb}\t}}\n")
						if (e["opt"]):
							cf.write("\t}\n")
						if (len(e["type"])==1):
							pc+=(TYPE_ACCESS_OPT_MAP[e["type"]] if e["opt"] else TYPE_ACCESS_MAP[e["type"]]).replace("$",ALPHABET[i])
						else:
							pc+=ALPHABET[i]
			hf.write(f"\n#define __SLL_API_TYPE_{k['name']} ")
			if (rt=="O"):
				d_str+=f"\n * \\ret sll_object_t*"
				hf.write("__SLL_CHECK_OUTPUT sll_object_t*")
				cf.write(f"\tsll_object_t* out={k['name']}({pc});\n{end}\treturn out;\n")
			else:
				if (TYPE_MAP[rt][-1]=="*"):
					if (len(pc)>0):
						a+=f",{TYPE_MAP[rt]} out"
						pc+=","
					else:
						a=TYPE_MAP[rt]+" out"
					hf.write("void")
					cf.write("\t"+TYPE_MAP[rt][:-1]+f" out;\n\t{k['name']}({pc}&out);\n{end}\t"+TYPE_RETURN_MAP[rt].replace(";",";\n\t")+";\n")
				else:
					d_str+=f"\n * \\ret {TYPE_MAP[rt]}"
					hf.write("__SLL_CHECK_OUTPUT "+TYPE_MAP[rt])
					cf.write("\t"+TYPE_MAP[rt]+f" out={k['name']}({pc});\n{end}\t"+TYPE_RETURN_MAP[rt].replace(";",";\n\t")+";\n")
			if (len(a)==0):
				a="void"
			hf.write(f"\n#define __SLL_API_ARGS_{k['name']} {a}\n/**\n * \\flags {('check_output ' if TYPE_MAP[rt][-1]!='*' else '')}func optimizable\n * \\name {k['name']}\n * \\group {k['group']}\n * \\desc {k['desc']}{d_str}\n */\n/**\n * \\flags check_output func optimizable\n * \\name {k['name']}_raw\n * \\group raw-api\n * \\subgroup raw-api-{k['group']}\n * \\desc Wrapper function for :{k['name']}:\n * \\arg const sll_object_t*const* al -> Arguments\n * \\arg sll_arg_count_t all -> Argument count\n * \\ret sll_object_t* -> The return value of the function\n */")
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
			fn_l.append(f"{{\"sll:{k['name'][8:]}\",{k['name']}_raw,{fl}}}")
		hf.write("\n#endif\n")
		cf.write(f"\n\n\nstatic const internal_function_t _ifunc_data_ptr[]={{\n\t"+",\n\t".join(fn_l)+f"\n}};\n\n\n\nconst sll_function_index_t _ifunc_size={len(fn_l)};\nconst internal_function_t* _ifunc_data=(const internal_function_t*)(&_ifunc_data_ptr);\n")
