import build
import header
import os
import subprocess
import sys
import util
import zipfile



COMPILATION_DEFINES=([b"_MSC_VER",b"_WINDOWS",b"WINDLL",b"USERDLL",b"_UNICODE",b"UNICODE"]+([b"NDEBUG"] if "--release" in sys.argv else [b"_DEBUG",b"DEBUG_BUILD"]) if os.name=="nt" else ([] if "--release" in sys.argv else [b"DEBUG_BUILD"]))
PLATFORM_SOURCE_CODE={"posix":"src/sll/platform/posix.c","nt":"src/sll/platform/windows.c"}



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
		if (k!="build/lib"):
			os.rmdir(k)
else:
	os.mkdir("build")
if (not os.path.exists("build/lib")):
	os.mkdir("build/lib")
if (vb):
	print("Copying Modules...")
for f in os.listdir("src/sll/lib"):
	if (vb):
		print(f"  Copying Module 'src/sll/lib/{f}'...")
	with open(f"src/sll/lib/{f}","rb") as rf,open(f"build/lib/{f}","wb") as wf:
		wf.write(rf.read())
v=header.read_version("src/include/sll/version.h")
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
if (os.name!="nt"):
	if (vb):
		print("Fixing 'LD_LIBRARY_PATH'...")
	ld=os.getenv("LD_LIBRARY_PATH","")
	if ("build:" not in ld):
		os.environ["LD_LIBRARY_PATH"]="build:"+ld
if (vb):
	print("Generating Source Code File...")
i_fl=[PLATFORM_SOURCE_CODE[os.name]]
for r,_,fl in os.walk("src/sll"):
	r=r.replace("\\","/").rstrip("/")+"/"
	if ("/platform/" in r.lower()):
		continue
	for f in fl:
		if (f[-2:]==".c"):
			i_fl.append(r+f)
build.generate_source_file(i_fl,"build/sll.c",vb)
if ("--test" in sys.argv):
	if (vb):
		print("Generating Coverage Source Code File...")
	build.convert_to_coverage("build/sll.c","build/sll_coverage.c",vb)
build.build_sll("build/sll.c","build/sll.o",v,vb,("--release" in sys.argv))
if (vb):
	print("Compiling Modules...")
fl=list(os.listdir("build/lib"))
if (util.wrap_output(["build/sll","-c",("-O3" if "--release" in sys.argv else "-O1"),"-L","-e","-R"]+["build/lib/"+e for e in fl]+(["-v"] if vb else []),pfx=b"  ").returncode!=0):
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
			with open(f"build/lib/{e}.sllc","rb") as rf:
				dt=rf.read()
				if (vb):
					print(f"  Converting Module 'build/lib/{e}.sllc' ({len(dt)} bytes)")
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
	build.build_sll_standalone("build/sll.o",vb,("--release" in sys.argv))
if ("--bundle" in sys.argv):
	with zipfile.ZipFile("build/sll.zip","w") as zf:
		for k in (["build/sll.exe","build/sll.dll"] if os.name=="nt" else ["build/sll","build/sll.so"]):
			zf.write(k,arcname=k[6:])
		for k in os.listdir("build/lib"):
			zf.write("build/lib/"+k,arcname="lib/"+k)
if ("--test" in sys.argv):
	if (vb):
		print("Generating Test Executable...")
	build.build_sll_test("build/sll_coverage.c","tests/data",vb,("--release" in sys.argv))
	if ("--do-not-start-test" not in sys.argv):
		if (vb):
			print("  Running Tests...")
		if (util.wrap_output(["build/run_tests"]).returncode!=0):
			sys.exit(1)
if ("--run" in sys.argv):
	if (vb):
		print("Running 'example/test.sll'...")
	e_nm=("build/sll_standalone" if "--standalone " in sys.argv else "build/sll")
	subprocess.run([e_nm,"-h"])
	if (subprocess.run([e_nm,"example/test.sll","-v","-O3","-c","-o","build/test","-e","-I","example","-R"]).returncode!=0 or subprocess.run([e_nm,"build/test.sllc","-v","-O0","-p","-P","-e","-L","-a","-c","-o","build/test2","-R"]).returncode!=0 or subprocess.run([e_nm,"build/test2.slla","-v","-P","-L"]).returncode!=0):
		sys.exit(1)
