import os
import subprocess
import sys
import util



def build_sll(fl,v,r):
	nm=f"sll-{v[0]}.{v[1]}.{v[2]}"
	def_l=["__SLL_COMPILATION__=1",f"__TIME_RAW__={util.BUILD_TIME}"]
	if (len(os.getenv("GITHUB_SHA",""))>0):
		def_l.append(f"__SHA__=\"{os.getenv('GITHUB_SHA')[:7]}\"")
		def_l.append(f"__FULL_SHA__=\"{os.getenv('GITHUB_SHA')}\"")
	if (len(os.getenv("USE_STACK_ALLOCATOR",""))>0):
		def_l.append("USE_STACK_ALLOCATOR=1")
	if (not r):
		def_l.append("DEBUG_BUILD")
	cd=os.getcwd()
	os.chdir("build")
	if (os.name=="nt"):
		def_l.extend(["_WINDOWS","WINDLL","USERDLL","WIN32_LEAN_AND_MEAN","_CRT_SECURE_NO_WARNINGS"])
		win_def=[]
		for k in def_l:
			win_def.append("/D")
			win_def.append(k)
		if (r):
			util.log("  Compiling Files (Release Mode)...")
			if (subprocess.run(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/MD","/I","../src/sll/include","/Foobjects\\"]+win_def+["../"+e for e in fl]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			util.log("  Linking Files (Release Mode)...")
			if (subprocess.run(["link",f"/OUT:{nm}.dll","/DLL","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:WINDOWS","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:NOREF","/INCREMENTAL:NO","/RELEASE","bcrypt.lib"]+["objects/"+e for e in os.listdir("objects")]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			util.log("  Compiling Files...")
			if (subprocess.run(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/Zi","/Od","/RTC1","/MDd","/I","../src/sll/include","/Foobjects\\"]+win_def+["../"+e for e in fl]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			util.log("  Linking Files...")
			if (subprocess.run(["link",f"/OUT:{nm}.dll","/DLL","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:WINDOWS","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL:NO","/RELEASE","bcrypt.lib"]+["objects/"+e for e in os.listdir("objects")]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	else:
		def_l.append("_GNU_SOURCE")
		linux_def=[]
		for k in def_l:
			linux_def.append("-D")
			linux_def.append(k)
		if (r):
			util.log("  Compiling Files (Release Mode)...")
			os.chdir("objects")
			if (subprocess.run(["gcc","-fno-exceptions","-fdiagnostics-color=always","-fPIC","-c","-fvisibility=hidden","-Wall","-O3","-Werror","-I","../../src/sll/include"]+linux_def+["../../"+e for e in fl]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			os.chdir("..")
			util.log("  Linking Files (Release Mode)...")
			if (subprocess.run(["gcc","-fno-exceptions","-fdiagnostics-color=always","-shared","-fPIC","-fvisibility=hidden","-Wall","-O3","-Werror","-o",nm+".so"]+["objects/"+e for e in os.listdir("objects")]+["-lm","-ldl"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			util.log("  Stripping Executable...")
			if (subprocess.run(["strip",nm+".so","-s","-R",".note.*","-R",".comment"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			util.log("  Compiling Files...")
			os.chdir("objects")
			if (subprocess.run(["gcc","-fno-exceptions","-fdiagnostics-color=always","-fPIC","-c","-fvisibility=hidden","-Wall","-g","-O0","-Werror","-I","../../src/sll/include"]+linux_def+["../../"+e for e in fl]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			os.chdir("..")
			util.log("  Linking Files...")
			if (subprocess.run(["gcc","-fno-exceptions","-fdiagnostics-color=always","-shared","-fPIC","-fvisibility=hidden","-Wall","-Werror","-g","-O0","-o",nm+".so"]+["objects/"+e for e in os.listdir("objects")]+["-lm","-ldl"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	os.chdir(cd)



def build_sll_cli():
	cd=os.getcwd()
	os.chdir("build")
	if (os.name=="nt"):
		util.log("  Compiling Files (Release Mode)...")
		if (subprocess.run(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","_CRT_SECURE_NO_WARNINGS","/D","WIN32_LEAN_AND_MEAN","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/MD","/I",".","/Fomain.obj","../src/cli/main_windows.c"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
		util.log("  Linking Files (Release Mode)...")
		if (subprocess.run(["link","main.obj","/OUT:sll.exe","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:WINDOWS","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO","/RELEASE"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
	else:
		util.log("  Compiling & Linking Files...")
		if (subprocess.run(["gcc","-fdiagnostics-color=always","-Wall","-lm","-Werror","-O3","../src/cli/main_linux.c","-o","sll","-I",".","-ldl"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
		util.log("  Stripping Executable...")
		if (subprocess.run(["strip","sll","-s","-R",".note.*","-R",".comment"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
	os.chdir(cd)



def build_sll_extension(fl,v,r):
	b_nm=f"sll-{v[0]}.{v[1]}.{v[2]}"
	nm=f"sll-ext-debug-{v[0]}.{v[1]}.{v[2]}"
	cd=os.getcwd()
	os.chdir("build")
	if (os.name=="nt"):
		if (r):
			util.log("  Compiling Library Files (Release Mode)...")
			if (subprocess.run(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","WINDLL","/D","USERDLL","/D","WIN32_LEAN_AND_MEAN","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/MD","/I","../src/ext/debug/include","/I",".","/Foobjects_ext\\"]+["../"+e for e in fl]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			util.log("  Linking Library Files (Release Mode)...")
			if (subprocess.run(["link",f"/OUT:{nm}.dll","/DLL","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:WINDOWS","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:NOREF","/INCREMENTAL:NO","/RELEASE",b_nm+".lib"]+["objects_ext/"+e for e in os.listdir("objects_ext")]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			util.log("  Compiling Library Files...")
			if (subprocess.run(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","WINDLL","/D","USERDLL","/D","WIN32_LEAN_AND_MEAN","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/Zi","/Od","/RTC1","/MDd","/I","../src/ext/debug/include","/I",".","/Foobjects_ext\\"]+["../"+e for e in fl]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			util.log("  Linking Library Files...")
			if (subprocess.run(["link",f"/OUT:{nm}.dll","/DLL","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:WINDOWS","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL:NO","/RELEASE",b_nm+".lib"]+["objects_ext/"+e for e in os.listdir("objects_ext")]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	else:
		if (r):
			util.log("  Compiling Library Files (Release Mode)...")
			os.chdir("objects_ext")
			if (subprocess.run(["gcc","-fdiagnostics-color=always","-fPIC","-c","-fvisibility=hidden","-Wall","-O3","-Werror","-I","../../src/ext/debug/include","-I","../"]+["../../"+e for e in fl]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			os.chdir("..")
			util.log("  Linking Library Files (Release Mode)...")
			if (subprocess.run(["gcc","-fdiagnostics-color=always","-shared","-fPIC","-fvisibility=hidden","-Wall","-O3","-Werror","-o",nm+".so"]+["objects_ext/"+e for e in os.listdir("objects_ext")]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			util.log("  Stripping Executable...")
			if (subprocess.run(["strip",nm+".so","-s","-R",".note.*","-R",".comment"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			util.log("  Compiling Library Files...")
			os.chdir("objects_ext")
			if (subprocess.run(["gcc","-fdiagnostics-color=always","-fPIC","-c","-fvisibility=hidden","-Wall","-g","-O0","-Werror","-I","../../src/ext/debug/include","-I","../"]+["../../"+e for e in fl]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			os.chdir("..")
			util.log("  Linking Library Files...")
			if (subprocess.run(["gcc","-fdiagnostics-color=always","-shared","-fPIC","-fvisibility=hidden","-Wall","-g","-O0","-o",nm+".so"]+["objects_ext/"+e for e in os.listdir("objects_ext")]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	os.chdir(cd)
