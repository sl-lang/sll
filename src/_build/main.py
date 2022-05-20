import sys
import util



if (util.execute([util.sll_runtime,"src/_build/main.sll","-I","src/sll/lib","-I","src/_build","-A"]+sys.argv)):
	sys.exit(0)
if ("--web" in sys.argv):
	sys.exit(0)
if ("--bundle" in sys.argv):
	util.log("Compressing executable files...")
	util.bundle()
