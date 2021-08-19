import os
import sys
import util



def build_sll(fl,v,vb,r):
	nm=f"sll-{v[0]}.{v[1]}.{v[2]}"
	cd=os.getcwd()
	os.chdir("build")
	if (os.name=="nt"):
		sha_fl=(["/D",f"__SHA__=\"{os.getenv('GITHUB_SHA')[:7]}\"","/D",f"__FULL_SHA__=\"{os.getenv('GITHUB_SHA')}\""] if os.getenv("GITHUB_SHA") else [])
		if (r):
			if (vb):
				print("  Compiling Library Files (Release Mode)...")
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","WINDLL","/D","USERDLL","/D","_UNICODE","/D","UNICODE","/D","__SLL_COMPILATION__","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/MD","/I","../src/include","/Foobjects\\"]+sha_fl+["../"+e for e in fl]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Linking Library Files (Release Mode)...")
			if (util.wrap_output(["link",f"/OUT:{nm}.dll","/DLL","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:NOREF","/INCREMENTAL:NO","/RELEASE"]+["objects/"+e for e in os.listdir("objects")]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Compiling Files (Release Mode)...")
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/MD","/I",".","/Fomain.obj","../src/main.c"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Linking Files (Release Mode)...")
			if (util.wrap_output(["link","main.obj","/OUT:sll.exe","/DYNAMICBASE",nm+".lib","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO","/RELEASE"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			if (vb):
				print("  Compiling Library Files...")
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","WINDLL","/D","USERDLL","/D","_UNICODE","/D","UNICODE","/D","__SLL_COMPILATION__","/D","DEBUG_BUILD","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/Zi","/Od","/RTC1","/MDd","/I","../src/include","/Foobjects\\"]+sha_fl+["../"+e for e in fl]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Linking Library Files...")
			if (util.wrap_output(["link",f"/OUT:{nm}.dll","/DLL","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL:NO","/RELEASE"]+["objects/"+e for e in os.listdir("objects")]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Compiling Files...")
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/D","DEBUG_BUILD","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/Zi","/Od","/RTC1","/MDd","/I",".","/Fomain.obj","../src/main.c"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Linking Files...")
			if (util.wrap_output(["link","main.obj","/OUT:sll.exe","/DYNAMICBASE",nm+".lib","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL:NO","/RELEASE"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	else:
		sha_fl=(["-D",f"__SHA__=\"{os.getenv('GITHUB_SHA')[:7]}\"","-D",f"__FULL_SHA__=\"{os.getenv('GITHUB_SHA')}\""] if os.getenv("GITHUB_SHA") else [])
		if (r):
			if (vb):
				print("  Compiling Library Files (Release Mode)...")
			os.chdir("objects")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-fPIC","-c","-fvisibility=hidden","-D","__SLL_COMPILATION__","-Wall","-O3","-Werror","-I","../../src/include"]+sha_fl+["../../"+e for e in fl]+["-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			os.chdir("..")
			if (vb):
				print("  Linking Library Files (Release Mode)...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-shared","-fPIC","-fvisibility=hidden","-Wall","-O3","-Werror","-o",nm+".so"]+["objects/"+e for e in os.listdir("objects")]+["-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Compiling & Linking Files (Release Mode)...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-Wall","-lm","-Werror","-O3","../src/main.c",nm+".so","-o","sll","-I","."]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			if (vb):
				print("  Compiling Library Files...")
			os.chdir("objects")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-fPIC","-c","-fvisibility=hidden","-D","__SLL_COMPILATION__","-D","DEBUG_BUILD","-Wall","-O0","-Werror","-I","../../src/include"]+sha_fl+["../../"+e for e in fl]+["-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			os.chdir("..")
			if (vb):
				print("  Linking Library Files...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-shared","-fPIC","-fvisibility=hidden","-Wall","-O0","-o",nm+".so"]+["objects/"+e for e in os.listdir("objects")]+["-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Compiling & Linking Files...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-D","DEBUG_BUILD","-Wall","-lm","-Werror","-O0","../src/main.c",nm+".so","-o","sll","-I","."]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	os.chdir(cd)



def build_sll_standalone(vb,r):
	cd=os.getcwd()
	os.chdir("build")
	if (os.name=="nt"):
		if (r):
			if (vb):
				print("  Compiling Files (Release Mode)...")
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/D","_CRT_SECURE_NO_WARNINGS","/D","__SLL_STATIC__","/D","STANDALONE_BUILD","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/MD","/I",".","../src/main.c"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Linking Files (Release Mode)...")
			if (util.wrap_output(["link","main.obj","/OUT:sll_standalone.exe","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO","/RELEASE"]+["objects/"+e for e in os.listdir("objects")]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			if (vb):
				print("  Compiling Files...")
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/D","DEBUG_BUILD","/D","_CRT_SECURE_NO_WARNINGS","/D","__SLL_STATIC__","/D","STANDALONE_BUILD","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/Zi","/Od","/RTC1","/MDd","/I",".","../src/main.c"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Linking Files...")
			if (util.wrap_output(["link","main.obj","/OUT:sll_standalone.exe","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL:NO","/RELEASE"]+["objects/"+e for e in os.listdir("objects")]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	else:
		if (r):
			if (vb):
				print("  Compiling & Linking Files (Release Mode)...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-D","__SLL_STATIC__","-D","STANDALONE_BUILD","-Wall","-lm","-Werror","-O3","../src/main.c","-o","sll_standalone","-I",".","-I","../src/include"]+["objects/"+e for e in os.listdir("objects")]+["-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			if (vb):
				print("  Compiling & Linking Files...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-D","__SLL_STATIC__","-D","STANDALONE_BUILD","-D","DEBUG_BUILD","-Wall","-lm","-Werror","-O0","../src/main.c","-o","sll_standalone","-I",".","-I","../src/include"]+["objects/"+e for e in os.listdir("objects")]+["-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	os.chdir(cd)



def build_sll_test(t_fp,vb,r):
	t_fp=os.path.abspath(t_fp).replace("\\","/").rstrip("/")+"/"
	cd=os.getcwd()
	os.chdir("build")
	if (os.name=="nt"):
		if (r):
			if (vb):
				print("  Compiling Files (Release Mode)...")
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/D","__SLL_STATIC__","/D","__TEST_ROOT_DIR__=\""+t_fp+"\"","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/MD","/I",".","/I","../tests","/I","../src/include","/Forun_tests.obj","../tests/run_tests.c"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Linking Files (Release Mode)...")
			if (util.wrap_output(["link","run_tests.obj","/OUT:run_tests.exe","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO","/RELEASE"]+["objects/"+e for e in os.listdir("objects")]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			if (vb):
				print("  Compiling Files...")
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/D","DEBUG_BUILD","/D","__SLL_STATIC__","/D","__TEST_ROOT_DIR__=\""+t_fp+"\"","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/Zi","/Od","/RTC1","/MDd","/I",".","/I","../tests","/I","../src/include","/Forun_tests.obj","../tests/run_tests.c"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Linking Files...")
			if (util.wrap_output(["link","run_tests.obj","/OUT:run_tests.exe","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL:NO","/RELEASE"]+["objects/"+e for e in os.listdir("objects")]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	else:
		if (r):
			if (vb):
				print("  Compiling & Linking Files (Release Mode)...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-D","__TEST_ROOT_DIR__=\""+t_fp+"\"","-Wall","-Werror","-O3","../tests/run_tests.c","-o","run_tests","-I",".","-I","../tests","-I","../src/include"]+["objects/"+e for e in os.listdir("objects")]+["-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			if (vb):
				print("  Compiling & Linking Files...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-D","__TEST_ROOT_DIR__=\""+t_fp+"\"","-Wall","-Werror","-O0","../tests/run_tests.c","-o","run_tests","-I",".","-I","../tests","-I","../src/include"]+["objects/"+e for e in os.listdir("objects")]+["-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	os.chdir(cd)
