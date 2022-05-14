import hashlib
import os
import platform
import subprocess
import sys
import time
import zipfile



BUILD_PATHS=["build/lib","build/lib_debug","build/objects"]
BUILD_TIME=time.time_ns()
CLEAR_PATHS=["build/lib","build/lib_debug","build/web"]
EXECUTABLE_EXTENSION={"darwin":"","linux":"","windows":".exe"}
LIBRARY_EXTENSION={"darwin":".so","linux":".so","windows":".dll"}
PLATFORM_SOURCE_CODE={"darwin":"src/sll/platform/linux","linux":"src/sll/platform/linux","windows":"src/sll/platform/windows"}



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
release=("--release" in sys.argv)
system=platform.system().lower()
verbose=("--verbose" in sys.argv)
version=_read_version("src/sll/include/sll/version.h")



_obj_file_pfx="build/objects/"+system
if (release):
	_obj_file_pfx+="_release"
if (not release or "--debug" in sys.argv):
	_obj_file_pfx+="_debug"
_obj_file_pfx+="_"



def create_output_dir():
	for base in CLEAR_PATHS:
		if (not os.path.exists(base)):
			continue
		del_dl=[]
		for r,dl,fl in os.walk(base):
			for d in dl:
				del_dl.append(os.path.join(r,d))
			for f in fl:
				os.remove(os.path.join(r,f))
		for k in del_dl[::-1]:
			os.rmdir(k)
	for k in BUILD_PATHS:
		if (not os.path.exists(k)):
			os.mkdir(k)



def execute(args):
	sys.__stdout__.flush()
	return (subprocess.run(args).returncode!=0)



def output_file_path(fp):
	return _obj_file_pfx+fp.replace("\\","/").replace("/","$")+".o"



def get_docs_files():
	o=[]
	for r,_,fl in os.walk("src/sll/include/sll"):
		for k in fl:
			o.append(os.path.join(r,k))
	return o



def get_sll_files():
	o=[]
	for r,_,fl in os.walk(PLATFORM_SOURCE_CODE[system]):
		r=r.replace("\\","/").rstrip("/")+"/"
		for f in fl:
			if (f[-2:]==".c" or f[-4:]==".asm"):
				o.append(r+f)
	for r,_,fl in os.walk("src/sll"):
		r=r.replace("\\","/").rstrip("/")+"/"
		if ("/platform/" in r.lower()):
			continue
		for f in fl:
			if (f[-2:]==".c" or f[-4:]==".asm"):
				o.append(r+f)
	return o



def bundle():
	with zipfile.ZipFile("build/sll.zip","w",compression=zipfile.ZIP_DEFLATED) as zf:
		for k in ["build/sll"+EXECUTABLE_EXTENSION[system],"build/sll-"+version+LIBRARY_EXTENSION[system],"build/lib/stdlib.slb","build/lib_debug/stdlib.slb"]:
			zf.write(k,arcname=k[6:])
		if (system=="windows"):
			zf.write("build/sllw.exe",arcname="sllw.exe")
		zf.write("build/sll.h",arcname="include/sll.h")



def hash_file(fp):
	sz=0
	o=hashlib.sha256()
	with open(fp,"rb") as rf:
		while (True):
			c=rf.read(16384)
			if (len(c)==0):
				break
			sz+=len(c)
			o.update(c)
	return (sz,o.hexdigest())
