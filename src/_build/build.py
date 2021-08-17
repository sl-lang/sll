import datetime
import os
import re
import sys
import util



DEFINE_REGEX=re.compile(br"^[ \t]*#[ \t]*define[ \t]+([a-zA-Z0-9_]+)",re.M)



def generate_source_file(fl,o_fp,vb):
	dt=b""
	i=0
	for f_nm in fl:
		with open(f_nm,"rb") as f:
			f_dt=f.read().replace(b"\r\n",b"\n")
			dt+=bytes(f"\n#define __FILE_ID__ {i}\n#line 1 \"{f_nm}\"\n","utf-8")+f_dt+b"\n#undef __FILE_ID__"
			for k in DEFINE_REGEX.findall(f_dt):
				dt+=b"\n#undef "+k
		i+=1
	dt=dt.split(b"\n")
	inc=[]
	for i,k in enumerate(dt):
		k=k.strip()
		if (k[:1]==b"#"):
			e=k[1:].strip().replace(b"\t",b" ").split(b" ")
			if (e[0]==b"include"):
				e=b" ".join(e[1:])
				if (e not in inc):
					inc.append(e)
				dt[i]=b""
			elif (e[0]==b"define" and e[1]==b"WIN32_LEAN_AND_MEAN"):
				dt[i]=b""
	dt=[b"#include "+k for k in sorted(inc) if k!=b"<windows.h>"]+dt
	if (b"<windows.h>" in inc):
		dt=[b"#define WIN32_LEAN_AND_MEAN 1",b"#include <windows.h>"]+dt
	with open(o_fp,"wb") as f:
		f.write(b"\n".join(dt))



def build_sll(fp,o_fp,v,vb,r):
	fp=os.path.abspath(fp)
	o_fp=os.path.abspath(o_fp)
	nm=f"sll-{v[0]}.{v[1]}.{v[2]}"
	cd=os.getcwd()
	os.chdir("build")
	if (os.name=="nt"):
		if (r):
			if (vb):
				print("  Compiling Library Files (Release Mode)...")
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","WINDLL","/D","USERDLL","/D","_UNICODE","/D","UNICODE","/D","__SLL_COMPILATION__","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/MD","/I","../src/include","/Fo"+o_fp,fp]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Linking Library Files (Release Mode)...")
			if (util.wrap_output(["link",o_fp,f"/OUT:{nm}.dll","/DLL","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:NOREF","/INCREMENTAL:NO","/RELEASE"]).returncode!=0):
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
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","WINDLL","/D","USERDLL","/D","_UNICODE","/D","UNICODE","/D","__SLL_COMPILATION__","/D","DEBUG_BUILD","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/Zi","/Od","/RTC1","/MDd","/I","../src/include","/Fo"+o_fp,fp]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Linking Library Files...")
			if (util.wrap_output(["link",o_fp,f"/OUT:{nm}.dll","/DLL","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL:NO","/RELEASE"]).returncode!=0):
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
		if (r):
			if (vb):
				print("  Compiling Library Files (Release Mode)...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-fPIC","-c","-fvisibility=hidden","-D","__SLL_COMPILATION__","-Wall","-O3","-Werror","-o",o_fp,"-I","../src/include",fp,"-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Linking Library Files (Release Mode)...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-shared","-fPIC","-fvisibility=hidden","-Wall","-O3","-Werror","-o",nm+".so",o_fp,"-lm"]).returncode!=0):
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
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-fPIC","-c","-fvisibility=hidden","-D","__SLL_COMPILATION__","-D","DEBUG_BUILD","-Wall","-O0","-Werror","-o",o_fp,"-I","../src/include",fp,"-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Linking Library Files...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-shared","-fPIC","-fvisibility=hidden","-Wall","-O0","-Werror","-o",nm+".so",o_fp,"-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Compiling & Linking Files...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-D","DEBUG_BUILD","-Wall","-lm","-Werror","-O0","../src/main.c",nm+".so","-o","sll","-I","."]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	os.chdir(cd)



def build_sll_standalone(fp,vb,r):
	fp=os.path.abspath(fp)
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
			if (util.wrap_output(["link","main.obj",fp,"/OUT:sll_standalone.exe","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO","/RELEASE"]).returncode!=0):
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
			if (util.wrap_output(["link","main.obj",fp,"/OUT:sll_standalone.exe","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL:NO","/RELEASE"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	else:
		if (r):
			if (vb):
				print("  Compiling & Linking Files (Release Mode)...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-D","__SLL_STATIC__","-D","STANDALONE_BUILD","-Wall","-lm","-Werror","-O3","../src/main.c","-o","sll_standalone","-I",".","-I","../src/include",fp,"-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			if (vb):
				print("  Compiling & Linking Files...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-D","__SLL_STATIC__","-D","STANDALONE_BUILD","-D","DEBUG_BUILD","-Wall","-lm","-Werror","-O0","../src/main.c","-o","sll_standalone","-I",".","-I","../src/include",fp,"-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	os.chdir(cd)



def build_sll_test(fp,t_fp,vb,r):
	fp=os.path.abspath(fp)
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
			if (util.wrap_output(["link","run_tests.obj",fp,"/OUT:run_tests.exe","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO","/RELEASE"]).returncode!=0):
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
			if (util.wrap_output(["link","run_tests.obj",fp,"/OUT:run_tests.exe","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL:NO","/RELEASE"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	else:
		if (r):
			if (vb):
				print("  Compiling & Linking Files (Release Mode)...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-D","__TEST_ROOT_DIR__=\""+t_fp+"\"","-Wall","-Werror","-O3","../tests/run_tests.c",fp,"-o","run_tests","-I",".","-I","../tests","-I","../src/include","-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			if (vb):
				print("  Compiling & Linking Files...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-D","__TEST_ROOT_DIR__=\""+t_fp+"\"","-Wall","-Werror","-O0","../tests/run_tests.c",fp,"-o","run_tests","-I",".","-I","../tests","-I","../src/include","-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	os.chdir(cd)
