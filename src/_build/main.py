import api
import assembly
import build
import docs
import hashlist
import header
import operator_parser
import os
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
operator_parser.generate_operator_parser("src/sll/data/operator_parser.txt","src/sll/include/sll/generated/operator_parser.h")
header.generate_help("src/sll/data/help.txt","src/sll/include/sll/generated/help_text.h")
header.generate_error_header("src/sll/data/memory_fail.txt","src/sll/include/sll/generated/memory_fail.h","MEMORY_FAIL")
header.generate_error_header("src/sll/data/critical_error.txt","src/sll/include/sll/generated/critical_error.h","CRITICAL_ERROR")
if ("--web" in sys.argv):
	website.generate()
	sys.exit(0)
hashlist.load_hash_list("build/.files")
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
if (util.execute(["build/sll","-c","-R","-I","build/lib"]+fl+(["-v"] if "--verbose" in sys.argv else [])+(["-d","-D"] if "--release" in sys.argv else []))):
	sys.exit(1)
util.log("Generating Bundle...")
if (util.execute(["build/sll","-b","-n","-N","/","-R","-O","build/lib/stdlib.slb"]+[e+".slc" for e in fl if e.split("/")[-1][0]!="_"]+(["-v"] if "--verbose" in sys.argv else [])+(["-d","-D"] if "--release" in sys.argv else []))):
	sys.exit(1)
util.log("Removing Module Source Files...")
for k in fl:
	util.log(f"  Removing '{k}'...")
	os.remove(k)
	util.log(f"  Removing '{k}.slc'...")
	os.remove(k+".slc")
if ("--bundle" in sys.argv or "--upload" in sys.argv):
	util.log("Compressing executable files...")
	util.bundle(ver)
if ("--test" in sys.argv):
	util.log("Running tests...")
	util.execute(["build/sll","tests/_runner.sll"])
if ("--upload" in sys.argv):
	os.rename("build/sll.zip",util.system+".zip")
if ("--run" in sys.argv):
	util.log(f"Running 'examples/_internal_test/test.sll'...")
	util.execute(["build/sll","-h"])
	if (util.execute(["build/sll","-v","-c","-o","build/test","-e","-R","examples/_internal_test/test.sll","-I","examples/_internal_test"]) or util.execute(["build/sll","build/test.slc","-v","-p","-P","-e","-a","-c","-o","build/test2","-R"]) or util.execute((["gdb","-ex","r","-iex","set pagination off","--args"] if len(os.getenv("SLL_GDB","")) else [])+["build/sll","build/test2.sla","-v","-P"])):
		sys.exit(1)
