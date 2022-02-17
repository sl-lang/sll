import os
import platform
import subprocess
import sys
import time
import zipfile



BASE64_ALPHABET="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
BUILD_PATHS=["build/lib","build/lib/debug","build/sys_lib","build/objects","build/objects_ext","build/web"]
BUILD_TIME=time.time_ns()
PLATFORM_SOURCE_CODE={"linux":"src/sll/platform/linux","windows":"src/sll/platform/windows"}



log=(print if "--verbose" in sys.argv else lambda *_:None)



def fix_env():
	if (os.name=="nt"):
		if (os.getenv("GITHUB_ACTIONS",None) is not None):
			for k in str(subprocess.run([str(subprocess.run([os.environ["ProgramFiles(x86)"]+"/Microsoft Visual Studio/Installer/vswhere.exe","-nologo","-latest","-products","*","-requires","Microsoft.VisualStudio.Component.VC.Tools.x86.x64","-property","installationPath"],stdout=subprocess.PIPE).stdout.strip(),"utf-8")+"/VC/Auxiliary/Build/vcvarsall.bat","x64","&&","cls","&&","set"],shell=True,stdout=subprocess.PIPE).stdout.split(b"\x0c")[1],"utf-8").split("\r\n"):
				k=[e.strip() for e in k.split("=")]
				if (k[0].lower() in ["path","include","lib","libpath"]):
					os.environ[k[0].upper()]=k[1]



def create_output_dir():
	if (os.path.exists("build")):
		dl=[]
		for r,ndl,fl in os.walk("build"):
			r=r.replace("\\","/").rstrip("/")+"/"
			for d in ndl:
				dl.insert(0,r+d)
			for f in fl:
				os.remove(r+f)
		for k in dl:
			if ("_sll_runtime" not in k and k not in BUILD_PATHS):
				os.rmdir(k)
	else:
		os.mkdir("build")
	for k in BUILD_PATHS:
		if (not os.path.exists(k)):
			os.mkdir(k)



def get_docs_files():
	o=[]
	for r,_,fl in os.walk("src/sll/include/sll"):
		for k in fl:
			o.append(os.path.join(r,k))
	return o



def get_sll_files():
	o=[]
	for r,_,fl in os.walk(PLATFORM_SOURCE_CODE[platform.system().lower()]):
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



def get_ext_files():
	o=[]
	for r,_,fl in os.walk("src/ext/debug"):
		r=r.replace("\\","/").rstrip("/")+"/"
		for f in fl:
			if (f[-2:]==".c"):
				o.append(r+f)
	return o



def bundle(v):
	with zipfile.ZipFile("build/sll.zip","w",compression=zipfile.ZIP_DEFLATED) as zf:
		for k in (["build/sll.exe","build/sllw.exe",f"build/sll-{v[0]}.{v[1]}.{v[2]}.dll"] if os.name=="nt" else ["build/sll",f"build/sll-{v[0]}.{v[1]}.{v[2]}.so"]):
			zf.write(k,arcname=k[6:])
		for k in os.listdir("build/lib"):
			zf.write("build/lib/"+k,arcname="lib/"+k)



def bundle_ext(v):
	with zipfile.ZipFile("build/sll_ext_debug.zip","w",compression=zipfile.ZIP_DEFLATED) as zf:
		fp=(f"build/sll-ext-debug-{v[0]}.{v[1]}.{v[2]}.dll" if os.name=="nt" else f"build/sll-ext-debug-{v[0]}.{v[1]}.{v[2]}.so")
		zf.write(fp,arcname=fp[6:])
		for k in os.listdir("build/lib/debug"):
			zf.write("build/lib/debug/"+k,arcname="lib/"+k)



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
