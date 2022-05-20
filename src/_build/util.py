import platform
import subprocess
import sys
import time
import zipfile



EXECUTABLE_EXTENSION={"darwin":"","linux":"","windows":".exe"}
LIBRARY_EXTENSION={"darwin":".so","linux":".so","windows":".dll"}



log=(print if "--verbose" in sys.argv else lambda _:None)
system=platform.system().lower()
sll_runtime=f"build/_sll_runtime_{system}/sll"+EXECUTABLE_EXTENSION[system]
verbose=("--verbose" in sys.argv)



def execute(args):
	sys.__stdout__.flush()
	return (subprocess.run(args).returncode!=0)



if (__name__=="__main__"):
	with zipfile.ZipFile(system+".zip","w",compression=zipfile.ZIP_DEFLATED) as zf:
		for k in ["build/sll"+EXECUTABLE_EXTENSION[system],"build/sll-"+sys.argv[1]+LIBRARY_EXTENSION[system],"build/lib/stdlib.slb","build/lib_debug/stdlib.slb"]:
			zf.write(k,arcname=k[6:])
		if (system=="windows"):
			zf.write("build/sllw.exe",arcname="sllw.exe")
		zf.write("build/sll.h",arcname="include/sll.h")
