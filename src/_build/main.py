import build
import hashlist
import header
import os
import sys
import util



DEBUGGER={
	"darwin": ["lldb","--"],
	"linux": ["gdb","-ex","r","-iex","set pagination off","--args"],
	"windows": ["windbg"]
}[util.system]



if (util.execute([util.sll_runtime,"src/_build/main.sll","-I","src/sll/lib","-I","src/_build","-A"]+sys.argv)):
	sys.exit(0)
if ("--web" in sys.argv):
	sys.exit(0)
hashlist.load_hash_list("build/.files")
header.generate_header("src/sll/include","build/sll.h")
util.log("Compiling...")
build.build_sll()
util.log("Compiling CLI...")
build.build_sll_cli()
util.log("Listing modules...")
fl=list(os.listdir("src/sll/lib"))
util.log("Compiling modules...")
if (util.execute(["build/sll","-c","-R","-I","build/lib","-I","src/sll/lib","-o","build/lib/"]+["src/sll/lib/"+e for e in fl]+(["-v"] if "--verbose" in sys.argv else []))):
	sys.exit(1)
util.log("Generating bundle...")
if (util.execute(["build/sll","-b","-d","-D","-n","-N","/","-R","-O","build/lib/stdlib.slb"]+["build/lib/"+e+".slc" for e in fl if e[0]!="_"]+(["-v"] if "--verbose" in sys.argv else []))):
	sys.exit(1)
util.log("Generating debug bundle...")
if (util.execute(["build/sll","-b","-n","-N","/","-R","-O","build/lib_debug/stdlib.slb"]+["build/lib/"+e+".slc" for e in fl if e[0]!="_"]+(["-v"] if "--verbose" in sys.argv else []))):
	sys.exit(1)
util.log("Removing module compilation files...")
for k in fl:
	k="build/lib/"+k+".slc"
	util.log(f"  Removing '{k}'...")
	os.remove(k)
if ("--bundle" in sys.argv or "--upload" in sys.argv):
	util.log("Compressing executable files...")
	util.bundle()
if ("--test" in sys.argv):
	util.log("Running tests...")
	util.execute(["build/sll","tests/_runner.sll"])
if ("--upload" in sys.argv):
	os.rename("build/sll.zip",util.system+".zip")
if ("--run" in sys.argv):
	util.log(f"Running 'examples/_internal_test/test.sll'...")
	util.execute(["build/sll","-h"])
	if (util.execute(["build/sll","-v","-c","-o","build/test","-e","-R","examples/_internal_test/test.sll","-I","@internal|examples/_internal_test"]+(["-r"] if util.release else [])) or util.execute(["build/sll","build/test.slc","-v","-e","-a","-c","-o","build/test2","-R"]) or util.execute((DEBUGGER if len(os.getenv("SLL_DEBUGGER","")) else [])+["build/sll","build/test2.sla","-v","-P"])):
		sys.exit(1)
