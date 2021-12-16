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



vb=("--verbose" in sys.argv)
if (vb):
	print("Generating Build Directory...")
util.create_output_dir()
if ("--web" in sys.argv):
	website.generate(vb)
	sys.exit(0)
ver=header.read_version("src/include/sll/version.h")
if ("--generate-api" in sys.argv):
	if (vb):
		print("Collecting Documentation Files...")
	d_fl=util.get_docs_files()
	if (vb):
		print(f"  Found {len(d_fl)} Files\nGenerating Documentation...")
	d_dt,api_dt=docs.create_docs(d_fl)
	if (vb):
		print(f"Generating Code & Signatures for {len(api_dt)} API functions...")
	api.generate_c_api(d_dt,api_dt)
if (vb):
	print("Copying Modules...")
for f in os.listdir("src/sll/lib"):
	if (vb):
		print(f"  Copying Module 'src/sll/lib/{f}'...")
	with open(f"src/sll/lib/{f}","rb") as rf,open(f"build/lib/{f}","wb") as wf:
		wf.write(rf.read())
header.generate_help("rsrc/help.txt","build/help_text.h",vb)
h_dt=header.parse_headers("src/include/sll",vb)
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
util.fix_env()
if (vb):
	print("Listing Source Code Files...")
fl=util.get_sll_files()
if (vb):
	print("Compiling Sll...")
build.build_sll(fl,ver,vb,("--release" in sys.argv))
if (vb):
	print("Listing Source Code Files...")
fl=util.get_sll_ext_files()
if (vb):
	print("Compiling Sll Extension...")
build.build_sll_extension(fl,ver,vb,("--release" in sys.argv))
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
	if (vb):
		print("Compressing executable files...")
	util.bundle(ver)
if ("--upload" in sys.argv):
	import release
	if (os.name=="nt"):
		if (vb):
			print("Uploading 'win.zip'...")
		release.upload_asset("build/sll.zip","win.zip",sys.argv[-2],sys.argv[-1])
		if (vb):
			print("Uploading 'win_standalone.exe'...")
		release.upload_asset("build/sll_standalone.exe","win_standalone.exe",sys.argv[-2],sys.argv[-1])
	else:
		if (vb):
			print("Uploading 'posix.zip'...")
		release.upload_asset("build/sll.zip","posix.zip",sys.argv[-2],sys.argv[-1])
		if (vb):
			print("Uploading 'posix_standalone'...")
		release.upload_asset("build/sll_standalone","posix_standalone",sys.argv[-2],sys.argv[-1])
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
	if (subprocess.run([e_nm,"examples/_internal_test/test.sll","-c","-C","-v","-O0","-o","build/raw","-e","-I","examples/_internal_test","-R","-F"]).returncode!=0 or subprocess.run([e_nm,"examples/_internal_test/test.sll","-C","-v","-O3","-c","-o","build/test","-e","-I","examples/_internal_test","-R","-F"]).returncode!=0 or subprocess.run([e_nm,"build/test.slc","-C","-v","-O0","-p","-P","-e","-a","-c","-o","build/test2","-R"]).returncode!=0 or subprocess.run([e_nm,"build/test2.sla","-C","-v","-P"]).returncode!=0):
		sys.exit(1)
