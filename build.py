import subprocess
import sys
import os



if (os.path.exists("build")):
	dl=[]
	for r,ndl,fl in os.walk("build"):
		r=r.replace("\\","/").strip("/")+"/"
		for d in ndl:
			dl.insert(0,r+d)
		for f in fl:
			os.remove(r+f)
	for k in dl:
		os.rmdir(k)
else:
	os.mkdir("build")
if (os.name=="nt"):
	cd=os.getcwd()
	os.chdir("build")
	if ("--release" in sys.argv):
		if (subprocess.run(["cl","/c","/permissive-","/Zc:preprocessor","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/FC","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/Oi","/MD","/I","../src/include","../src/main.c","../src/language/*.c"]).returncode!=0 or subprocess.run(["link","*.obj","/OUT:lisp_like_language.exe","/DYNAMICBASE","kernel32.lib","user32.lib","gdi32.lib","winspool.lib","comdlg32.lib","advapi32.lib","shell32.lib","ole32.lib","oleaut32.lib","uuid.lib","odbc32.lib","odbccp32.lib","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO","/OPT:ICF"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
	else:
		if (subprocess.run(["cl","/c","/permissive-","/Zc:preprocessor","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/FC","/EHsc","/nologo","/diagnostics:column","/ZI","/Od","/RTC1","/MDd","/I","../src/include","../src/main.c","../src/language/*.c"]).returncode!=0 or subprocess.run(["link","*.obj","/OUT:lisp_like_language.exe","/DYNAMICBASE","kernel32.lib","user32.lib","gdi32.lib","winspool.lib","comdlg32.lib","advapi32.lib","shell32.lib","ole32.lib","oleaut32.lib","uuid.lib","odbc32.lib","odbccp32.lib","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
	os.chdir(cd)
	if ("--run" in sys.argv):
		subprocess.run(["build/lisp_like_language.exe","test.lll"])
else:
	if ("--release" in sys.argv):
		fl=[]
		for r,_,cfl in os.walk("src"):
			r=r.replace("\\","/").strip("/")+"/"
			for f in cfl:
				if (f[-2:]==".c"):
					fl.append(f"build/{(r+f).replace('/','$')}.o")
					if (subprocess.run(["gcc","-Wall","-lm","-Werror","-O3","-c",r+f,"-o",f"build/{(r+f).replace('/','$')}.o","-Isrc/include"]).returncode!=0):
						sys.exit(1)
		if (subprocess.run(["gcc","-o","build/lisp_like_language"]+fl).returncode!=0):
			sys.exit(1)
	else:
		fl=[]
		for r,_,cfl in os.walk("src"):
			r=r.replace("\\","/").strip("/")+"/"
			for f in cfl:
				if (f[-2:]==".c"):
					fl.append(f"build/{(r+f).replace('/','$')}.o")
					if (subprocess.run(["gcc","-Wall","-lm","-Werror","-O0","-c",r+f,"-o",f"build/{(r+f).replace('/','$')}.o","-Isrc/include"]).returncode!=0):
						sys.exit(1)
		if (subprocess.run(["gcc","-o","build/lisp_like_language"]+fl).returncode!=0):
			sys.exit(1)
	if ("--run" in sys.argv):
		subprocess.run(["build/lisp_like_language","test.lll"])
