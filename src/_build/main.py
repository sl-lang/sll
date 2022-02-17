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



util.log("Generating Build Directory...")
util.create_output_dir()
ver=header.read_version("src/sll/include/sll/version.h")
util.log("Collecting Documentation Files...")
d_fl=util.get_docs_files()
util.log(f"  Found {len(d_fl)} Files\nGenerating Documentation...")
d_dt,api_dt=docs.create_docs(d_fl)
util.log(f"Generating Code & Signatures for {len(api_dt)} API functions...")
api.generate_c_api(d_dt,api_dt)
assembly.generate_assembly_optimizer("src/sll/data/assembly_optimizer.txt","src/sll/include/sll/generated/assembly_optimizer.h")
header.generate_help("src/sll/data/help.txt","src/sll/include/sll/generated/help_text.h")
header.generate_memory_fail("src/sll/data/memory_fail.txt","src/sll/include/sll/generated/memory_fail.h")
if ("--web" in sys.argv):
	website.generate()
	sys.exit(0)
h_dt=header.parse_headers("src/sll/include")
util.log("Generating Library Header File...")
with open("build/sll.h","wb") as wf:
	wf.write(b"#ifndef __SLL_H__\n#define __SLL_H__ 1"+header.generate_header(h_dt)+b"\n#endif\n")
util.log("Fixing Env Variables...")
util.fix_env()
util.log("Listing Source Code Files...")
fl=util.get_sll_files()
util.log("Compiling Sll...")
build.build_sll(fl,ver,("--release" in sys.argv))
util.log("Compiling Sll CLI...")
build.build_sll_cli()
util.log("Copying Modules...")
fl=[]
for f in os.listdir("src/sll/lib"):
	util.log(f"  Copying Module 'src/sll/lib/{f}'...")
	with open(f"src/sll/lib/{f}","rb") as rf,open(f"build/lib/{f}","wb") as wf:
		wf.write(rf.read())
	fl.append("build/lib/"+f)
util.log("Compiling Modules...")
if (subprocess.run(["build/sll","-c","-R","-I","build/lib"]+fl+(["-v"] if "--verbose" in sys.argv else [])+(["-D"] if "--release" in sys.argv else [])).returncode!=0):
	sys.exit(1)
util.log("Removing Module Source Files...")
for k in fl:
	util.log(f"  Removing '{k}'...")
	os.remove(k)
if ("--bundle" in sys.argv or "--upload" in sys.argv):
	util.log("Compressing executable files...")
	util.bundle(ver)
for nm in util.get_ext_list():
	util.log(f"Listing Source Code Files ({nm})...")
	fl=util.get_ext_files(nm)
	util.log(f"Compiling Sll Extension ({nm})...")
	build.build_sll_extension(nm,fl,ver,("--release" in sys.argv))
	util.log(f"Copying Extension Modules ({nm})...")
	fl=[]
	for f in os.listdir(f"src/ext/{nm}/lib"):
		util.log(f"  Copying Extension Module 'src/ext/{nm}/lib/{f}'...")
		with open(f"src/ext/{nm}/lib/{f}","rb") as rf,open(f"build/lib/{nm}/{f}","wb") as wf:
			wf.write(rf.read())
		fl.append(f"build/lib/{nm}/"+f)
	util.log("Compiling Extension Modules...")
	if (subprocess.run(["build/sll","-c","-R","-I","build/lib/"+nm]+fl+(["-v"] if "--verbose" in sys.argv else [])+(["-D"] if "--release" in sys.argv else [])).returncode!=0):
		sys.exit(1)
	util.log("Removing Extension Module Source Files...")
	for k in fl:
		util.log(f"  Removing '{k}'...")
		os.remove(k)
	if ("--bundle" in sys.argv or "--upload" in sys.argv):
		util.log("Compressing extension files...")
		util.bundle_ext(nm,ver)
		if ("--upload" in sys.argv):
			os.rename(f"build/sll_ext_{nm}.zip",("win" if os.name=="nt" else "linux")+f"_ext_{nm}.zip")
	util.log("Installing extension library...")
	with open(f"build/sll-ext-{nm}-{ver[0]}.{ver[1]}.{ver[2]}."+("dll" if os.name=="nt" else "so"),"rb") as rf,open(f"build/sys_lib/sll-ext-{nm}-{ver[0]}.{ver[1]}.{ver[2]}."+("dll" if os.name=="nt" else "so"),"wb") as wf:
		wf.write(rf.read())
if ("--test" in sys.argv):
	util.log("Running tests...")
	subprocess.run(["build/sll","tests/_runner.sll"])
if ("--upload" in sys.argv):
	os.rename("build/sll.zip",("win.zip" if os.name=="nt" else "linux.zip"))
if ("--run" in sys.argv):
	a=(["examples/_internal_test_ext_debug/test.sll","-I","build/lib_ext"] if "--extension" in sys.argv else ["examples/_internal_test/test.sll","-I","examples/_internal_test"])
	util.log(f"Running '{a[0]}...")
	subprocess.run(["build/sll","-h"])
	if (subprocess.run(["build/sll","-c","-v","-o","build/raw","-e","-R","-F"]+a).returncode!=0 or subprocess.run(["build/sll","-v","-c","-o","build/test","-e","-R","-F"]+a).returncode!=0 or subprocess.run(["build/sll","build/test.slc","-v","-p","-P","-e","-a","-c","-o","build/test2","-R"]).returncode!=0 or subprocess.run(["build/sll","build/test2.sla","-v","-P"]).returncode!=0):
		sys.exit(1)
