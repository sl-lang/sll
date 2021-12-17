import os
import subprocess
import sys
import zipfile



BUILD_PATHS=["build/lib","build/objects","build/objects_ext","build/web","build/web/css"]
PLATFORM_SOURCE_CODE={"posix":"src/sll/platform/posix","nt":"src/sll/platform/windows"}
EXT_PLATFORM_SOURCE_CODE={"posix":"src/sll_ext/platform/posix","nt":"src/sll_ext/platform/windows"}



log=(print if "--verbose" in sys.argv else lambda *_:None)



def fix_env():
	if (os.name=="nt"):
		if (os.getenv("GITHUB_ACTIONS",None) is not None):
			for k in str(subprocess.run([str(subprocess.run([os.environ["ProgramFiles(x86)"]+"/Microsoft Visual Studio/Installer/vswhere.exe","-nologo","-latest","-products","*","-requires","Microsoft.VisualStudio.Component.VC.Tools.x86.x64","-property","installationPath"],stdout=subprocess.PIPE).stdout.strip(),"utf-8")+"/VC/Auxiliary/Build/vcvarsall.bat","x64","&&","cls","&&","set"],shell=True,stdout=subprocess.PIPE).stdout.split(b"\x0c")[1],"utf-8").split("\r\n"):
				k=[e.strip() for e in k.split("=")]
				if (k[0].lower() in ["path","include","lib","libpath"]):
					os.environ[k[0].upper()]=k[1]
	else:
		os.environ["LD_LIBRARY_PATH"]="build:"+os.getenv("LD_LIBRARY_PATH","")



def wrap_output(a,pfx=b"    "):
	sys.stdout.flush()
	p=subprocess.Popen(a,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
	st=False
	for c in iter(lambda:p.stdout.read(1),b""):
		if (not st):
			st=True
			sys.stdout.buffer.write(pfx)
		sys.stdout.buffer.write(c)
		if (c==b"\n"):
			st=False
			sys.stdout.buffer.flush()
	p.wait()
	return p



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
			if (k not in BUILD_PATHS):
				os.rmdir(k)
	else:
		os.mkdir("build")
	for k in BUILD_PATHS:
		if (not os.path.exists(k)):
			os.mkdir(k)



def get_docs_files():
	o=[]
	for r,_,fl in os.walk("src/include/sll"):
		for k in fl:
			if (k!="_sll_internal.h"):
				o.append(os.path.join(r,k))
	return o



def get_sll_files():
	o=[]
	for r,_,fl in os.walk(PLATFORM_SOURCE_CODE[os.name]):
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



def get_sll_ext_files():
	o=[]
	for r,_,fl in os.walk(EXT_PLATFORM_SOURCE_CODE[os.name]):
		r=r.replace("\\","/").rstrip("/")+"/"
		for f in fl:
			if (f[-2:]==".c"):
				o.append(r+f)
	for r,_,fl in os.walk("src/sll_ext"):
		r=r.replace("\\","/").rstrip("/")+"/"
		if ("/platform/" in r.lower()):
			continue
		for f in fl:
			if (f[-2:]==".c"):
				o.append(r+f)
	return o



def bundle(v):
	with zipfile.ZipFile("build/sll.zip","w",compression=zipfile.ZIP_DEFLATED) as zf:
		for k in (["build/sll.exe",f"build/sll-{v[0]}.{v[1]}.{v[2]}.dll"] if os.name=="nt" else ["build/sll",f"build/sll-{v[0]}.{v[1]}.{v[2]}.so"]):
			zf.write(k,arcname=k[6:])
		for k in os.listdir("build/lib"):
			zf.write("build/lib/"+k,arcname="lib/"+k)
