import build
import docs
import header
import os
import subprocess
import sys
import util
import zipfile



ALPHABET="abcdefghijklmnopqrstuvwxyz"
API_CODE_FILE_PATH="src/sll/api/_generated.c"
API_HEADER_FILE_PATH="src/include/sll/api/_generated.h"
COMPILATION_DEFINES=([b"_MSC_VER",b"_WINDOWS",b"WINDLL",b"USERDLL",b"_UNICODE",b"UNICODE"]+([b"NDEBUG"] if "--release" in sys.argv else [b"_DEBUG",b"DEBUG_BUILD"]) if os.name=="nt" else ([] if "--release" in sys.argv else [b"DEBUG_BUILD"]))
PLATFORM_SOURCE_CODE={"posix":"src/sll/platform/posix.c","nt":"src/sll/platform/windows.c"}
RETURN_CODE_BASE_TYPE={"0":"I","1":"I","h":"H","Z":"S","f":"F","I":"I","F":"F","C":"C","S":"S","a":"A","A":"A","H":"H","M":"M","O":"O"}
RETURN_CODE_TYPE_MAP={"0":"return SLL_ACQUIRE_STATIC_INT(0)","1":"return SLL_ACQUIRE_STATIC_INT(1)","h":"return SLL_ACQUIRE_STATIC(handle_zero)","Z":"return SLL_ACQUIRE_STATIC(str_zero)","f":"return SLL_ACQUIRE_STATIC(float_zero)","a":"return SLL_ACQUIRE_STATIC(array_zero)"}
TYPE_ACCESS_MAP={"I":"$->dt.i","F":"$->dt.f","C":"$->dt.c","S":"&($->dt.s)","A":"&($->dt.a)","H":"&($->dt.h)","M":"&($->dt.m)","O":"$"}
TYPE_ACCESS_OPT_MAP={"I":"($?$->dt.i:0)","F":"($?$->dt.f:0)","C":"($?$->dt.c:0)","S":"($?&($->dt.s):NULL)","A":"($?&($->dt.a):NULL)","H":"($?&($->dt.h):NULL)","M":"($?&($->dt.m):NULL)","O":"$"}
TYPE_CHECK_MAP={"I":"SLL_RUNTIME_OBJECT_GET_TYPE($)==SLL_RUNTIME_OBJECT_TYPE_INT","F":"SLL_RUNTIME_OBJECT_GET_TYPE($)==SLL_RUNTIME_OBJECT_TYPE_FLOAT","C":"SLL_RUNTIME_OBJECT_GET_TYPE($)==SLL_RUNTIME_OBJECT_TYPE_CHAR","S":"SLL_RUNTIME_OBJECT_GET_TYPE($)==SLL_RUNTIME_OBJECT_TYPE_STRING","A":"SLL_RUNTIME_OBJECT_GET_TYPE($)==SLL_RUNTIME_OBJECT_TYPE_ARRAY","H":"(SLL_RUNTIME_OBJECT_GET_TYPE($)==SLL_RUNTIME_OBJECT_TYPE_HANDLE&&$->dt.h.t!=SLL_HANDLE_UNKNOWN_TYPE)","M":"SLL_RUNTIME_OBJECT_GET_TYPE($)==SLL_RUNTIME_OBJECT_TYPE_MAP"}
TYPE_MAP={"I":"sll_integer_t","F":"sll_float_t","C":"sll_char_t","S":"sll_string_t*","A":"sll_array_t*","H":"sll_handle_data_t*","M":"sll_map_t*","O":"sll_runtime_object_t*"}
TYPE_RETURN_MAP={"I":"return SLL_FROM_INT(out)","F":"return SLL_FROM_FLOAT(out)","S":"sll_runtime_object_t* out_o=SLL_CREATE();out_o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;out_o->dt.s=out;return out_o","A":"sll_runtime_object_t* out_o=SLL_CREATE();out_o->t=SLL_RUNTIME_OBJECT_TYPE_ARRAY;out_o->dt.a=out;return out_o","H":"sll_runtime_object_t* out_o=SLL_CREATE();out_o->t=SLL_RUNTIME_OBJECT_TYPE_HANDLE;out_o->dt.h=out;return out_o","M":"sll_runtime_object_t* out_o=SLL_CREATE();out_o->t=SLL_RUNTIME_OBJECT_TYPE_MAP;out_o->dt.m=out;return out_o"}



vb=("--verbose" in sys.argv)
if (vb):
	print("Generating Build Directory...")
if (os.path.exists("build")):
	dl=[]
	for r,ndl,fl in os.walk("build"):
		r=r.replace("\\","/").rstrip("/")+"/"
		for d in ndl:
			dl.insert(0,r+d)
		for f in fl:
			os.remove(r+f)
	for k in dl:
		if (k!="build/lib" and k!="build/objects"):
			os.rmdir(k)
else:
	os.mkdir("build")
if (not os.path.exists("build/lib")):
	os.mkdir("build/lib")
if (not os.path.exists("build/objects")):
	os.mkdir("build/objects")
if (vb):
	print("Copying Modules...")
for f in os.listdir("src/sll/lib"):
	if (vb):
		print(f"  Copying Module 'src/sll/lib/{f}'...")
	with open(f"src/sll/lib/{f}","rb") as rf,open(f"build/lib/{f}","wb") as wf:
		wf.write(rf.read())
ver=header.read_version("src/include/sll/version.h")
if (vb):
	print("Collecting Documentation Files...")
d_fl=[]
for r,_,fl in os.walk("src/include/sll"):
	for k in fl:
		if (k[0]!="_"):
			d_fl.append(os.path.join(r,k))
if (vb):
	print(f"  Found {len(d_fl)} Files")
	print("Generating Documentation...")
d_dt=docs.create_docs(d_fl)
if ("--generate-api" in sys.argv):
	if (vb):
		print(f"Generating Code & Signatures for {len(d_dt)} API functions...")
	with open(API_HEADER_FILE_PATH,"w") as hf,open(API_CODE_FILE_PATH,"w") as cf:
		hf.write("// WARNING: This is an auto-generated file. Any changes made to this file might be lost at any moment. Do Not Edit!\n#ifndef __SLL_API__GENERATED__\n#define __SLL_API__GENERATED__\n#include <sll/common.h>\n#include <sll/types.h>\n\n\n")
		cf.write("// WARNING: This is an auto-generated file. Any changes made to this file might be lost at any moment. Do Not Edit!\n#include <sll/_sll_internal.h>\n#include <sll/api.h>\n#include <sll/api/_generated.h>\n#include <sll/common.h>\n#include <sll/handle.h>\n#include <sll/ift.h>\n#include <sll/memory.h>\n#include <sll/runtime_object.h>\n#include <sll/static_object.h>\n#include <sll/types.h>\n")
		for k in d_dt:
			if ("api" in k["flag"]):
				rt=RETURN_CODE_BASE_TYPE[k["ret"][0]["type"]]
				for i in range(1,len(k["ret"])):
					if (RETURN_CODE_BASE_TYPE[k["ret"][i]["type"]]!=rt):
						rt="O"
						break
				cf.write(f"\n\n\n__SLL_API_TYPE_{k['name']} {k['name']}(__SLL_API_ARGS_{k['name']});\n__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* {k['name']}_raw(const sll_runtime_object_t*const* al,sll_arg_count_t all){{\n")
				a=""
				pc=""
				end=""
				if (len(k["args"])>0):
					err_c=RETURN_CODE_TYPE_MAP[k["ret"][k["err_ret"]]["type"]]
					i=len(k["args"])
					while (i>0 and k["args"][i-1]["opt"]):
						i-=1
					if (i!=0):
						cf.write(f"\tif (all<{i}){{\n\t\t"+err_c.replace(";",";\n\t\t")+";\n\t}\n")
					for i,e in enumerate(k["args"]):
						if (i):
							a+=","
							pc+=","
						if (i==len(k["args"])-1 and "var_arg" in k["flag"]):
							a+=f"const {TYPE_MAP[e['type']]+('const' if TYPE_MAP[e['type']][-1]=='*' else '')}* {ALPHABET[i]},sll_arg_count_t {ALPHABET[i]}c"
							if (e["type"]=="O"):
								cf.write(f"\tconst sll_runtime_object_t*const* {ALPHABET[i]}=al+{i};\n\tsll_arg_count_t {ALPHABET[i]}c=all-{i};\n")
							else:
								cf.write(f"\tsll_arg_count_t {ALPHABET[i]}c=all-{i};\n\tconst {TYPE_MAP[e['type']]}* {ALPHABET[i]}=sll_allocate({ALPHABET[i]}c*sizeof({TYPE_MAP[e['type']]}));\n\tfor (sll_arg_count_t idx=0;idx<{ALPHABET[i]}c;idx++){{\n\t\tconst sll_runtime_object_t* tmp=*(al+idx+{i});\n\t\tif (!({TYPE_CHECK_MAP[e['type']].replace('$','tmp')})){{\n\t\t\tsll_deallocate((void*){ALPHABET[i]});\n\t\t\t"+err_c.replace(";",";\n\t\t\t")+f";\n\t\t}}\n\t\t*({ALPHABET[i]}+idx)={TYPE_ACCESS_MAP[e['type']].replace('$','tmp')};\n\t}}\n")
								end=f"\tsll_deallocate((void*){ALPHABET[i]});\n"
							pc+=ALPHABET[i]+","+ALPHABET[i]+"c"
						else:
							a+=f"const {(TYPE_MAP[e['type']] if len(e['type'])==1 else 'sll_runtime_object_t*')} {ALPHABET[i]}"
							e_tb=""
							if (e["opt"]):
								e_tb="\t"
								cf.write(f"\tconst sll_runtime_object_t* {ALPHABET[i]}=NULL;\n\tif (all>{i}){{\n\t\t{ALPHABET[i]}=*(al+{i});\n")
							else:
								cf.write(f"\tconst sll_runtime_object_t* {ALPHABET[i]}=*(al+{i});\n")
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
					hf.write("__SLL_CHECK_OUTPUT sll_runtime_object_t*")
					cf.write(f"\tsll_runtime_object_t* out={k['name']}({pc});\n{end}\treturn out;\n")
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
						hf.write("__SLL_CHECK_OUTPUT "+TYPE_MAP[rt])
						cf.write("\t"+TYPE_MAP[rt]+f" out={k['name']}({pc});\n{end}\t"+TYPE_RETURN_MAP[rt].replace(";",";\n\t")+";\n")
				if (len(a)==0):
					a="void"
				hf.write(f"\n#define __SLL_API_ARGS_{k['name']} {a}\n")
				cf.write(f"}}\nINTERNAL_FUNCTION(\"{k['name'][8:]}\",{k['name']}_raw,{('0' if 'optimizable' in k['flag'] else 'SLL_INTERNAL_FUNCTION_FLAG_REQUIRED')}|{('SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL' if 'compilation_call' in k['flag'] else '0')});\n")
		hf.write("\n\n\n#endif\n")
header.generate_help("rsrc/help.txt","build/help_text.h",vb)
h_dt=header.parse_header("src/include/sll",vb)
if (vb):
	print("Generating Library Header File...")
with open("build/sll.h","wb") as wf:
	wf.write(b"#ifndef __SLL_H__\n#define __SLL_H__ 1"+header.generate_header(h_dt,COMPILATION_DEFINES)+b"\n#endif\n")
if ("--standalone" in sys.argv or "--test" in sys.argv):
	if (vb):
		print("Generating Standalone Library Header File...")
	with open("build/sll_standalone.h","wb") as wf:
		wf.write(b"#ifndef __SLL_STANDALONE_H__\n#define __SLL_STANDALONE_H__ 1"+header.generate_header(h_dt,COMPILATION_DEFINES+[b"__SLL_STATIC__"])+b"\n#endif\n")
if (vb):
	print("Fixing Env Variables...")
if (os.name=="nt"):
	if ("__ghactions" in sys.argv):
		for k in str(subprocess.run([str(subprocess.run([os.environ["ProgramFiles(x86)"]+"/Microsoft Visual Studio/Installer/vswhere.exe","-nologo","-latest","-products","*","-requires","Microsoft.VisualStudio.Component.VC.Tools.x86.x64","-property","installationPath"],stdout=subprocess.PIPE).stdout.strip(),"utf-8")+"/VC/Auxiliary/Build/vcvarsall.bat","x64","&&","cls","&&","set"],shell=True,stdout=subprocess.PIPE).stdout.split(b"\x0c")[1],"utf-8").split("\r\n"):
			k=[e.strip() for e in k.split("=")]
			if (k[0].lower() in ["path","include","lib","libpath"]):
				os.environ[k[0].upper()]=k[1]
else:
	ld=os.getenv("LD_LIBRARY_PATH","")
	if ("build:" not in ld):
		os.environ["LD_LIBRARY_PATH"]="build:"+ld
if (vb):
	print("Listing Source Code Files...")
i_fl=[PLATFORM_SOURCE_CODE[os.name]]
for r,_,fl in os.walk("src/sll"):
	r=r.replace("\\","/").rstrip("/")+"/"
	if ("/platform/" in r.lower()):
		continue
	for f in fl:
		if (f[-2:]==".c"):
			i_fl.append(r+f)
if (vb):
	print("Generating Executable...")
build.build_sll(i_fl,ver,vb,("--release" in sys.argv))
if (vb):
	print("Compiling Modules...")
fl=list(os.listdir("build/lib"))
if (util.wrap_output(["build/sll","-C","-c",("-O3" if "--release" in sys.argv else "-O1"),"-e","-R"]+["build/lib/"+e for e in fl]+(["-v"] if vb else []),pfx=b"  ").returncode!=0):
	sys.exit(1)
if (vb):
	print("Removing Module Source Files...")
for k in fl:
	if (vb):
		print(f"  Removing 'build/lib/{k}'...")
	os.remove(f"build/lib/{k}")
if ("--standalone" in sys.argv):
	if (vb):
		print(f"Converting Modules to Header File ({len(fl)} Modules)...")
	with open("build/compiled_modules.h","wb") as f:
		f.write(bytes(f"#ifndef __COMPILED_MODULES_H__\n#define __COMPILED_MODULES_H__\n#include <stdint.h>\n#define COMPILED_MODULE_COUNT {len(fl)}\ntypedef struct __MODULE{{const char* nm;uint32_t nml;uint8_t c;uint32_t sz;const uint8_t* dt;}} module_t;\n","utf-8"))
		m_dt=[]
		for i,e in enumerate(fl):
			f.write(bytes(f"const uint8_t _m{i}[]={{","utf-8"))
			with open(f"build/lib/{e}.slc","rb") as rf:
				dt=rf.read()
				if (vb):
					print(f"  Converting Module 'build/lib/{e}.slc' ({len(dt)} bytes)")
				c=0
				for k in bytes(e,"utf-8"):
					c^=k
				m_dt.append(bytes(f"{{\"{e.replace(chr(34),chr(92)+chr(34))}\",{len(e)},{c},{len(dt)},_m{i}}}","utf-8"))
				st=False
				for c in dt:
					f.write((b"," if st else b"")+bytearray([48,120,(48 if (c>>4)<10 else 87)+(c>>4),(48 if (c&0xf)<10 else 87)+(c&0xf)]))
					st=True
			f.write(b"};\n")
		f.write(b"const module_t m_dt[]={"+b",".join(m_dt)+b"};\n#endif")
	if (vb):
		print("Generating Standalone Executable...")
	build.build_sll_standalone(vb,("--release" in sys.argv))
if ("--bundle" in sys.argv):
	with zipfile.ZipFile("build/sll.zip","w",compression=zipfile.ZIP_DEFLATED) as zf:
		for k in (["build/sll.exe",f"build/sll-{ver[0]}.{ver[1]}.{ver[2]}.dll"] if os.name=="nt" else ["build/sll",f"build/sll-{ver[0]}.{ver[1]}.{ver[2]}.so"]):
			zf.write(k,arcname=k[6:])
		for k in os.listdir("build/lib"):
			zf.write("build/lib/"+k,arcname="lib/"+k)
if ("--test" in sys.argv):
	if (vb):
		print("Generating Test Executable...")
	build.build_sll_test(vb,("--release" in sys.argv))
	if ("--do-not-run-test" not in sys.argv):
		if (vb):
			print("  Running Tests...")
		if (util.wrap_output(["build/run_tests"]).returncode!=0):
			sys.exit(1)
if ("--run" in sys.argv):
	if (vb):
		print("Running 'examples/_internal_test/test.sll'...")
	e_nm=("build/sll_standalone" if "--standalone " in sys.argv else "build/sll")
	subprocess.run([e_nm,"-h","-C"])
	if (subprocess.run([e_nm,"examples/_internal_test/test.sll","-C","-v","-O3","-c","-o","build/test","-e","-I","examples/_internal_test","-R"]).returncode!=0 or subprocess.run([e_nm,"build/test.slc","-C","-v","-O0","-p","-P","-e","-a","-c","-o","build/test2","-R"]).returncode!=0 or subprocess.run([e_nm,"build/test2.sla","-C","-v","-P"]).returncode!=0):
		sys.exit(1)
