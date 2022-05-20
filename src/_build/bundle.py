import platform
import sys
import zipfile



EXECUTABLE_EXTENSION={"darwin":"","linux":"","windows":".exe"}
LIBRARY_EXTENSION={"darwin":".so","linux":".so","windows":".dll"}



system=platform.system().lower()
with zipfile.ZipFile(system+".zip","w",compression=zipfile.ZIP_DEFLATED) as zf:
	for k in ["build/sll"+EXECUTABLE_EXTENSION[system],"build/sll-"+sys.argv[1]+LIBRARY_EXTENSION[system],"build/lib/stdlib.slb","build/lib_debug/stdlib.slb"]:
		zf.write(k,arcname=k[6:])
	if (system=="windows"):
		zf.write("build/sllw.exe",arcname="sllw.exe")
	zf.write("build/sll.h",arcname="include/sll.h")
