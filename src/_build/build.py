import os
import sys
import util



def build_sll(fl,v,r):
	nm=f"sll-{v[0]}.{v[1]}.{v[2]}"
	cd=os.getcwd()
	os.chdir("build")
	if (os.name=="nt"):
		e_fl=(["/D",f"__SHA__=\"{os.getenv('GITHUB_SHA')[:7]}\"","/D",f"__FULL_SHA__=\"{os.getenv('GITHUB_SHA')}\""] if os.getenv("GITHUB_SHA") else [])
		if (r):
			util.log("  Compiling Files (Release Mode)...")
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","WINDLL","/D","USERDLL","/D","_UNICODE","/D","UNICODE","/D","__SLL_COMPILATION__","/D","WIN32_LEAN_AND_MEAN","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/MD","/I","../src/include","/Foobjects\\"]+e_fl+["../"+e for e in fl]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			util.log("  Linking Files (Release Mode)...")
			if (util.wrap_output(["link",f"/OUT:{nm}.dll","/DLL","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:NOREF","/INCREMENTAL:NO","/RELEASE"]+["objects/"+e for e in os.listdir("objects")]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			util.log("  Compiling Files...")
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","WINDLL","/D","USERDLL","/D","_UNICODE","/D","UNICODE","/D","__SLL_COMPILATION__","/D","WIN32_LEAN_AND_MEAN","/D","DEBUG_BUILD","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/Zi","/Od","/RTC1","/MDd","/I","../src/include","/Foobjects\\"]+e_fl+["../"+e for e in fl]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			util.log("  Linking Files...")
			if (util.wrap_output(["link",f"/OUT:{nm}.dll","/DLL","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL:NO","/RELEASE"]+["objects/"+e for e in os.listdir("objects")]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	else:
		e_fl=(["-D",f"__SHA__=\"{os.getenv('GITHUB_SHA')[:7]}\"","-D",f"__FULL_SHA__=\"{os.getenv('GITHUB_SHA')}\""] if os.getenv("GITHUB_SHA") else [])
		if (r):
			util.log("  Compiling Files (Release Mode)...")
			os.chdir("objects")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-fPIC","-c","-fvisibility=hidden","-D","__SLL_COMPILATION__","-D","_GNU_SOURCE","-Wall","-O3","-Werror","-I","../../src/include"]+e_fl+["../../"+e for e in fl]+["-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			os.chdir("..")
			util.log("  Linking Files (Release Mode)...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-shared","-fPIC","-fvisibility=hidden","-Wall","-O3","-Werror","-o",nm+".so"]+["objects/"+e for e in os.listdir("objects")]+["-lm","-ldl"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			util.log("  Compiling Files...")
			os.chdir("objects")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-fPIC","-c","-fvisibility=hidden","-D","__SLL_COMPILATION__","-D","DEBUG_BUILD","-D","_GNU_SOURCE","-Wall","-g","-O0","-Werror","-I","../../src/include"]+e_fl+["../../"+e for e in fl]+["-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			os.chdir("..")
			util.log("  Linking Files...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-shared","-fPIC","-fvisibility=hidden","-Wall","-Werror","-g","-O0","-o",nm+".so"]+["objects/"+e for e in os.listdir("objects")]+["-lm","-ldl"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	os.chdir(cd)



def build_sll_cli(v,r):
	nm=f"sll-{v[0]}.{v[1]}.{v[2]}"
	cd=os.getcwd()
	os.chdir("build")
	if (os.name=="nt"):
		if (r):
			util.log("  Compiling Files (Release Mode)...")
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/MD","/I",".","/Fomain.obj","../src/cli/main.c"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			util.log("  Linking Files (Release Mode)...")
			if (util.wrap_output(["link","main.obj","/OUT:sll.exe","/DYNAMICBASE",nm+".lib","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO","/RELEASE"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			util.log("  Compiling Files...")
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/D","DEBUG_BUILD","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/Zi","/Od","/RTC1","/MDd","/I",".","/Fomain.obj","../src/cli/main.c"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			util.log("  Linking Files...")
			if (util.wrap_output(["link","main.obj","/OUT:sll.exe","/DYNAMICBASE",nm+".lib","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL:NO","/RELEASE"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	else:
		if (r):
			util.log("  Compiling & Linking Files (Release Mode)...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-Wall","-lm","-Werror","-O3","../src/cli/main.c",nm+".so","-o","sll","-I","."]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			util.log("  Compiling & Linking Files...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-D","DEBUG_BUILD","-Wall","-lm","-Werror","-g","-O0","../src/cli/main.c",nm+".so","-o","sll","-I","."]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	os.chdir(cd)



def build_sll_test(r):
	cd=os.getcwd()
	os.chdir("build")
	if (os.name=="nt"):
		if (r):
			util.log("  Compiling Files (Release Mode)...")
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/D","__SLL_STATIC__","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/MD","/I",".","/I","../tests","/I","../src/include","/Forun_tests.obj","../tests/run_tests.c"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			util.log("  Linking Files (Release Mode)...")
			if (util.wrap_output(["link","run_tests.obj","/OUT:run_tests.exe","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO","/RELEASE"]+["objects/"+e for e in os.listdir("objects")]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			util.log("  Compiling Files...")
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/D","DEBUG_BUILD","/D","__SLL_STATIC__","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/Zi","/Od","/RTC1","/MDd","/I",".","/I","../tests","/I","../src/include","/Forun_tests.obj","../tests/run_tests.c"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			util.log("  Linking Files...")
			if (util.wrap_output(["link","run_tests.obj","/OUT:run_tests.exe","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL:NO","/RELEASE"]+["objects/"+e for e in os.listdir("objects")]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	else:
		if (r):
			util.log("  Compiling & Linking Files (Release Mode)...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-Wall","-Werror","-O3","../tests/run_tests.c","-o","run_tests","-I",".","-I","../tests","-I","../src/include"]+["objects/"+e for e in os.listdir("objects")]+["-lm","-ldl"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			util.log("  Compiling & Linking Files...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-Wall","-Werror","-g","-O0","../tests/run_tests.c","-o","run_tests","-I",".","-I","../tests","-I","../src/include"]+["objects/"+e for e in os.listdir("objects")]+["-lm","-ldl"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	os.chdir(cd)



def build_sll_extension(fl,v,r):
	b_nm=f"sll-{v[0]}.{v[1]}.{v[2]}"
	nm=f"sll-ext-{v[0]}.{v[1]}.{v[2]}"
	cd=os.getcwd()
	os.chdir("build")
	if (os.name=="nt"):
		if (r):
			util.log("  Compiling Library Files (Release Mode)...")
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","WINDLL","/D","USERDLL","/D","_UNICODE","/D","UNICODE","/D","__SLL_EXT_COMPILATION__","/D","WIN32_LEAN_AND_MEAN","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/MD","/I","../src/include","/I",".","/Foobjects_ext\\"]+["../"+e for e in fl]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			util.log("  Linking Library Files (Release Mode)...")
			if (util.wrap_output(["link",f"/OUT:{nm}.dll","/DLL","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:NOREF","/INCREMENTAL:NO","/RELEASE",b_nm+".lib"]+["objects_ext/"+e for e in os.listdir("objects_ext")]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			util.log("  Compiling Library Files...")
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","WINDLL","/D","USERDLL","/D","_UNICODE","/D","UNICODE","/D","__SLL_EXT_COMPILATION__","/D","WIN32_LEAN_AND_MEAN","/D","DEBUG_BUILD","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/Zi","/Od","/RTC1","/MDd","/I","../src/include","/I",".","/Foobjects_ext\\"]+["../"+e for e in fl]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			util.log("  Linking Library Files...")
			if (util.wrap_output(["link",f"/OUT:{nm}.dll","/DLL","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL:NO","/RELEASE",b_nm+".lib"]+["objects_ext/"+e for e in os.listdir("objects_ext")]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	else:
		if (r):
			util.log("  Compiling Library Files (Release Mode)...")
			os.chdir("objects_ext")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-fPIC","-c","-fvisibility=hidden","-D","__SLL_EXT_COMPILATION__","-Wall","-O3","-Werror","-I","../../src/include","-I","../"]+["../../"+e for e in fl]+["-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			os.chdir("..")
			util.log("  Linking Library Files (Release Mode)...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-shared","-fPIC","-fvisibility=hidden","-Wall","-O3","-Werror","-o",nm+".so"]+["objects_ext/"+e for e in os.listdir("objects_ext")]+["-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			util.log("  Compiling Library Files...")
			os.chdir("objects_ext")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-fPIC","-c","-fvisibility=hidden","-D","__SLL_EXT_COMPILATION__","-D","DEBUG_BUILD","-Wall","-g","-O0","-Werror","-I","../../src/include","-I","../"]+["../../"+e for e in fl]+["-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			os.chdir("..")
			util.log("  Linking Library Files...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-shared","-fPIC","-fvisibility=hidden","-Wall","-g","-O0","-o",nm+".so"]+["objects_ext/"+e for e in os.listdir("objects_ext")]+["-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	os.chdir(cd)
