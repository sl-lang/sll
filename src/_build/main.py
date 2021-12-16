import api
import build
import docs
import header
import os
import subprocess
import sys
import util
import website



COMPILATION_DEFINES=([b"_MSC_VER",b"_WINDOWS",b"WINDLL",b"USERDLL",b"_UNICODE",b"UNICODE"]+([b"NDEBUG"] if "--release" in sys.argv else [b"_DEBUG",b"DEBUG_BUILD"]) if os.name=="nt" else ([] if "--release" in sys.argv else [b"DEBUG_BUILD"]))



util.log("Generating Build Directory...")
util.create_output_dir()
if ("--web" in sys.argv):
	website.generate()
	sys.exit(0)
ver=header.read_version("src/include/sll/version.h")
if ("--generate-api" in sys.argv):
	util.log("Collecting Documentation Files...")
	d_fl=util.get_docs_files()
	util.log(f"  Found {len(d_fl)} Files\nGenerating Documentation...")
	d_dt,api_dt=docs.create_docs(d_fl)
	util.log(f"Generating Code & Signatures for {len(api_dt)} API functions...")
	api.generate_c_api(d_dt,api_dt)
h_dt=header.parse_headers("src/include/sll")
util.log("Generating Library Header File...")
with open("build/sll.h","wb") as wf:
	wf.write(b"#ifndef __SLL_H__\n#define __SLL_H__ 1"+header.generate_header(h_dt,COMPILATION_DEFINES)+b"\n#endif\n")
if ("--standalone" in sys.argv or "--test" in sys.argv):
	util.log("Generating Standalone Library Header File...")
	with open("build/sll_standalone.h","wb") as wf:
		wf.write(b"#ifndef __SLL_STANDALONE_H__\n#define __SLL_STANDALONE_H__ 1"+header.generate_header(h_dt,COMPILATION_DEFINES+[b"__SLL_STATIC__"])+b"\n#endif\n")
util.log("Fixing Env Variables...")
util.fix_env()
if ("--extension" in sys.argv):
	util.log("Listing Source Code Files (Extension)...")
	fl=util.get_sll_ext_files()
	util.log("Compiling Sll Extension...")
	build.build_sll_extension(fl,ver,("--release" in sys.argv))
	if ("--extension-only" in sys.argv):
		if ("--upload" in sys.argv):
			import release
			nm=f"sll-ext-{ver[0]}.{ver[1]}.{ver[2]}."+("dll" if os.name=="nt" else "so")
			util.log(f"Uploading '{nm}'...")
			release.upload_asset(f"build/{nm}",nm,sys.argv[-2],sys.argv[-1])
		sys.exit(0)
util.log("Listing Source Code Files...")
fl=util.get_sll_files()
util.log("Compiling Sll...")
header.generate_help("rsrc/help.txt","build/help_text.h")
build.build_sll(fl,ver,("--release" in sys.argv))
util.log("Copying Modules...")
for f in os.listdir("src/sll/lib"):
	util.log(f"  Copying Module 'src/sll/lib/{f}'...")
	with open(f"src/sll/lib/{f}","rb") as rf,open(f"build/lib/{f}","wb") as wf:
		wf.write(rf.read())
util.log("Compiling Modules...")
fl=list(os.listdir("build/lib"))
if (util.wrap_output(["build/sll","-C","-c",("-O3" if "--release" in sys.argv else "-O1"),"-e","-R"]+["build/lib/"+e for e in fl]+(["-v"] if "--verbose" in sys.argv else []),pfx=b"  ").returncode!=0):
	sys.exit(1)
util.log("Removing Module Source Files...")
for k in fl:
	util.log(f"  Removing 'build/lib/{k}'...")
	os.remove(f"build/lib/{k}")
if ("--standalone" in sys.argv):
	util.log(f"Converting Modules to Header File ({len(fl)} Modules)...")
	with open("build/compiled_modules.h","wb") as f:
		f.write(bytes(f"#ifndef __COMPILED_MODULES_H__\n#define __COMPILED_MODULES_H__\n#include <stdint.h>\n#define COMPILED_MODULE_COUNT {len(fl)}\ntypedef struct __MODULE{{const char* nm;uint32_t nml;uint8_t c;uint32_t sz;const uint8_t* dt;}} module_t;\n","utf-8"))
		m_dt=[]
		for i,e in enumerate(fl):
			f.write(bytes(f"const uint8_t _m{i}[]={{","utf-8"))
			with open(f"build/lib/{e}.slc","rb") as rf:
				dt=rf.read()
				util.log(f"  Converting Module 'build/lib/{e}.slc' ({len(dt)} bytes)")
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
	util.log("Generating Standalone Executable...")
	build.build_sll_standalone(("--release" in sys.argv))
if ("--bundle" in sys.argv):
	util.log("Compressing executable files...")
	util.bundle(ver)
if ("--upload" in sys.argv):
	import release
	if (os.name=="nt"):
		util.log("Uploading 'win.zip'...")
		release.upload_asset("build/sll.zip","win.zip",sys.argv[-2],sys.argv[-1])
		util.log("Uploading 'win_standalone.exe'...")
		release.upload_asset("build/sll_standalone.exe","win_standalone.exe",sys.argv[-2],sys.argv[-1])
	else:
		util.log("Uploading 'posix.zip'...")
		release.upload_asset("build/sll.zip","posix.zip",sys.argv[-2],sys.argv[-1])
		util.log("Uploading 'posix_standalone'...")
		release.upload_asset("build/sll_standalone","posix_standalone",sys.argv[-2],sys.argv[-1])
if ("--test" in sys.argv):
	util.log("Generating Test Executable...")
	build.build_sll_test(("--release" in sys.argv))
	if ("--do-not-run-test" not in sys.argv):
		util.log("  Running Tests...")
		if (util.wrap_output(["build/run_tests"]).returncode!=0):
			sys.exit(1)
if ("--run" in sys.argv):
	util.log("Running 'examples/_internal_test/test.sll'...")
	e_nm=("build/sll_standalone" if "--standalone" in sys.argv else "build/sll")
	subprocess.run([e_nm,"-h","-C"])
	if (subprocess.run([e_nm,"examples/_internal_test/test.sll","-c","-C","-v","-O0","-o","build/raw","-e","-I","examples/_internal_test","-R","-F"]).returncode!=0 or subprocess.run([e_nm,"examples/_internal_test/test.sll","-C","-v","-O3","-c","-o","build/test","-e","-I","examples/_internal_test","-R","-F"]).returncode!=0 or subprocess.run([e_nm,"build/test.slc","-C","-v","-O0","-p","-P","-e","-a","-c","-o","build/test2","-R"]).returncode!=0 or subprocess.run([e_nm,"build/test2.sla","-C","-v","-P"]).returncode!=0):
		sys.exit(1)
