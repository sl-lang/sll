import platform
import subprocess
import sys
import time
import zipfile



EXECUTABLE_EXTENSION={"darwin":"","linux":"","windows":".exe"}
LIBRARY_EXTENSION={"darwin":".so","linux":".so","windows":".dll"}



def _read_version(fp):
	o=[None,None,None]
	with open(fp,"r") as f:
		for k in f.read().replace("\r\n","\n").split("\n"):
			k=k.strip()
			if (k[:1]=="#"):
				k=k[1:].lstrip().split(" ")
				if (k[0]=="define"):
					if (k[1]=="SLL_VERSION_MAJOR"):
						o[0]=k[2]
					elif (k[1]=="SLL_VERSION_MINOR"):
						o[1]=k[2]
					elif (k[1]=="SLL_VERSION_PATCH"):
						o[2]=k[2]
	return ".".join(o)



log=(print if "--verbose" in sys.argv else lambda _:None)
system=platform.system().lower()
sll_runtime=f"build/_sll_runtime_{system}/sll"+EXECUTABLE_EXTENSION[system]
verbose=("--verbose" in sys.argv)
version=_read_version("src/sll/include/sll/version.h")



def execute(args):
	sys.__stdout__.flush()
	return (subprocess.run(args).returncode!=0)



def bundle():
	with zipfile.ZipFile(system+".zip","w",compression=zipfile.ZIP_DEFLATED) as zf:
		for k in ["build/sll"+EXECUTABLE_EXTENSION[system],"build/sll-"+version+LIBRARY_EXTENSION[system],"build/lib/stdlib.slb","build/lib_debug/stdlib.slb"]:
			zf.write(k,arcname=k[6:])
		if (system=="windows"):
			zf.write("build/sllw.exe",arcname="sllw.exe")
		zf.write("build/sll.h",arcname="include/sll.h")
