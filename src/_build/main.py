import api
import assembly
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
ver=header.read_version("src/sll/include/sll/version.h")
util.log("Collecting Documentation Files...")
d_fl=util.get_docs_files()
util.log(f"  Found {len(d_fl)} Files\nGenerating Documentation...")
d_dt,api_dt=docs.create_docs(d_fl)
util.log(f"Generating Code & Signatures for {len(api_dt)} API functions...")
api.generate_c_api(d_dt,api_dt)
assembly.generate_assembly_optimizer()
h_dt=header.parse_headers("src/sll/include")
util.log("Generating Library Header File...")
with open("build/sll.h","wb") as wf:
	wf.write(b"#ifndef __SLL_H__\n#define __SLL_H__ 1"+header.generate_header(h_dt,COMPILATION_DEFINES)+b"\n#endif\n")
if ("--test" in sys.argv):
	util.log("Generating Standalone Library Header File...")
	with open("build/sll_standalone.h","wb") as wf:
		wf.write(b"#ifndef __SLL_STANDALONE_H__\n#define __SLL_STANDALONE_H__ 1"+header.generate_header(h_dt,COMPILATION_DEFINES+[b"__SLL_STATIC__"])+b"\n#endif\n")
util.log("Fixing Env Variables...")
util.fix_env()
util.log("Listing Source Code Files...")
fl=util.get_sll_files()
util.log("Compiling Sll...")
build.build_sll(fl,ver,("--release" in sys.argv))
util.log("Compiling Sll CLI...")
header.generate_help("src/cli/data/help.txt","src/cli/include/help_text.h")
build.build_sll_cli(ver,("--release" in sys.argv))
util.log("Copying Modules...")
for f in os.listdir("src/sll/lib"):
	util.log(f"  Copying Module 'src/sll/lib/{f}'...")
	with open(f"src/sll/lib/{f}","rb") as rf,open(f"build/lib/{f}","wb") as wf:
		wf.write(rf.read())
util.log("Compiling Modules...")
fl=list(os.listdir("build/lib"))
if (subprocess.run(["build/sll","-C","-c","-e","-R"]+["build/lib/"+e for e in fl]+(["-v"] if "--verbose" in sys.argv else [])+(["-O","-D"] if "--release" in sys.argv else [])).returncode!=0):
	sys.exit(1)
util.log("Removing Module Source Files...")
for k in fl:
	util.log(f"  Removing 'build/lib/{k}'...")
	os.remove("build/lib/"+k)
if ("--extension" in sys.argv):
	util.log("Listing Source Code Files (Extension)...")
	fl=util.get_ext_files()
	util.log("Compiling Sll Extension...")
	build.build_sll_extension(fl,ver,("--release" in sys.argv))
	util.log("Copying Extension Modules...")
	for f in os.listdir("src/ext/debug/lib"):
		util.log(f"  Copying Extension Module 'src/ext/debug/lib/{f}'...")
		with open(f"src/ext/debug/lib/{f}","rb") as rf,open(f"build/lib_ext/{f}","wb") as wf:
			wf.write(rf.read())
	util.log("Compiling Extension Modules...")
	fl=list(os.listdir("build/lib_ext"))
	if (subprocess.run(["build/sll","-C","-c","-e","-R","-I","build/lib_ext"]+["build/lib_ext/"+e for e in fl]+(["-v"] if "--verbose" in sys.argv else [])+(["-O","-D"] if "--release" in sys.argv else [])).returncode!=0):
		sys.exit(1)
	util.log("Removing Extension Module Source Files...")
	for k in fl:
		util.log(f"  Removing 'build/lib_ext/{k}'...")
		os.remove("build/lib_ext/"+k)
	if ("--extension-only" in sys.argv):
		if ("--upload" in sys.argv):
			os.rename(f"build/sll-ext-debug-{ver[0]}.{ver[1]}.{ver[2]}."+("dll" if os.name=="nt" else "so"),("win-ext-debug.dll" if os.name=="nt" else "posix-ext-debug.so"))
		sys.exit(0)
if ("--bundle" in sys.argv):
	util.log("Compressing executable files...")
	util.bundle(ver)
if ("--upload" in sys.argv):
	os.rename("build/sll.zip",("win.zip" if os.name=="nt" else "posix.zip"))
if ("--test" in sys.argv):
	util.log("Generating Test Executable...")
	build.build_sll_test(("--release" in sys.argv))
	if ("--do-not-run-test" not in sys.argv):
		util.log("  Running Tests...")
		if (subprocess.run(["build/run_tests"]).returncode!=0):
			sys.exit(1)
if ("--run" in sys.argv):
	if ("--extension" in sys.argv):
		util.log("Installing extension library...")
		with open(f"build/sll-ext-debug-{ver[0]}.{ver[1]}.{ver[2]}."+("dll" if os.name=="nt" else "so"),"rb") as rf,open(f"build/sys_lib/sll-ext-debug-{ver[0]}.{ver[1]}.{ver[2]}."+("dll" if os.name=="nt" else "so"),"wb") as wf:
			wf.write(rf.read())
	a=(["examples/_internal_test_ext_debug/test.sll","-I","build/lib_ext"] if "--extension" in sys.argv else ["examples/_internal_test/test.sll","-I","examples/_internal_test"])
	util.log(f"Running '{a[0]}...")
	subprocess.run(["build/sll","-h","-C"])
	if (subprocess.run(["build/sll","-c","-C","-v","-o","build/raw","-e","-R","-F"]+a).returncode!=0 or subprocess.run(["build/sll","-C","-v","-O","-c","-o","build/test","-e","-R","-F"]+a).returncode!=0 or subprocess.run(["build/sll","build/test.slc","-C","-v","-p","-P","-e","-a","-c","-o","build/test2","-R"]).returncode!=0 or subprocess.run(["build/sll","build/test2.sla","-C","-v","-P"]).returncode!=0):
		sys.exit(1)
