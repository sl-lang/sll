import os
import subprocess
import sys



BUILD_PATHS=["build/lib","build/objects","build/web","build/web/css"]



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
