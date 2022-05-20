import sys
import util



if (util.execute([util.sll_runtime,"src/_build/main.sll","-I","src/sll/lib","-I","src/_build","-A"]+sys.argv)):
	sys.exit(0)
