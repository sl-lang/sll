import hashlib
import os
import platform
import subprocess
import sys
import time
import zipfile



BASE64_ALPHABET="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
BUILD_PATHS=["build/lib","build/sys_lib","build/objects"]
BUILD_TIME=time.time_ns()
CLEAR_PATHS=["build/lib","build/sys_lib"]
EXECUTABLE_EXTENSION={"darwin":"","linux":"","windows":".exe"}
LIBRARY_EXTENSION={"darwin":".so","linux":".so","windows":".dll"}
PLATFORM_SOURCE_CODE={"darwin":"src/sll/platform/linux","linux":"src/sll/platform/linux","windows":"src/sll/platform/windows"}



log=(print if "--verbose" in sys.argv else lambda _:None)
system=platform.system().lower()



def create_output_dir():
	for base in CLEAR_PATHS:
		if (not os.path.exists(base)):
			os.mkdir(base)
			continue
		for f in os.listdir(base):
			os.remove(os.path.join(base,f))
	for k in BUILD_PATHS:
		if (not os.path.exists(k)):
			os.mkdir(k)



def execute(args):
	sys.__stdout__.flush()
	return (subprocess.run(args).returncode!=0)



def unique_file_path(fp):
	return system+":"+("release" if "--release" in sys.argv else "debug")+":"+fp.replace("\\","/")



def output_file_path(fp):
	return "build/objects/"+system+"_"+("release" if "--release" in sys.argv else "debug")+"_"+fp.replace("\\","/").replace("/","$")+".o"



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
			if (f[-2:]==".c"):
				o.append(r+f)
	for r,_,fl in os.walk("src/sll"):
		r=r.replace("\\","/").rstrip("/")+"/"
		if ("/platform/" in r.lower()):
			continue
		for f in fl:
			if (f[-2:]==".c"):
				o.append(r+f)
	return o



def bundle(v):
	with zipfile.ZipFile("build/sll.zip","w",compression=zipfile.ZIP_DEFLATED) as zf:
		for k in ["build/sll"+EXECUTABLE_EXTENSION[system],f"build/sll-{v[0]}.{v[1]}.{v[2]}"+LIBRARY_EXTENSION[system],"build/lib/stdlib.slb"]:
			zf.write(k,arcname=k[6:])
		if (system=="windows"):
			zf.write("build/sllw.exe",arcname="sllw.exe")
		zf.write("build/sll.h",arcname="include/sll.h")



def encode(dt):
	if (len(dt)==0):
		return ""
	o=""
	i=0
	while (i<len(dt)-2):
		o+=BASE64_ALPHABET[dt[i]>>2]+BASE64_ALPHABET[((dt[i]<<4)&0x3f)|(dt[i+1]>>4)]+BASE64_ALPHABET[((dt[i+1]<<2)&0x3f)|(dt[i+2]>>6)]+BASE64_ALPHABET[dt[i+2]&0x3f]
		i+=3
	if (i==len(dt)-2):
		return o+BASE64_ALPHABET[dt[i]>>2]+BASE64_ALPHABET[((dt[i]<<4)&0x3f)|(dt[i+1]>>4)]+BASE64_ALPHABET[(dt[i+1]<<2)&0x3f]+"="
	if (i==len(dt)-1):
		return o+BASE64_ALPHABET[dt[i]>>2]+BASE64_ALPHABET[(dt[i]<<4)&0x3f]+"=="
	return o



def hash_file(fp):
	sz=0
	o=hashlib.sha256()
	with open(fp,"rb") as rf:
		while (True):
			c=rf.read(4096)
			if (len(c)==0):
				break
			sz+=len(c)
			o.update(c)
	return (sz,o.hexdigest())
