import hashlist
import os
import sys
import util



def build_sll(fl,v,r):
	lib_nm=f"sll-{v[0]}.{v[1]}.{v[2]}"
	def_l=["__SLL_COMPILATION__",f"__SLL_TIME_RAW__={util.BUILD_TIME}"]
	if (len(os.getenv("GITHUB_SHA",""))>0):
		def_l.append(f"__SLL_SHA__=\"{os.getenv('GITHUB_SHA')[:7]}\"")
		def_l.append(f"__SLL_FULL_SHA__=\"{os.getenv('GITHUB_SHA')}\"")
	if (len(os.getenv("USE_STACK_ALLOCATOR",""))>0):
		def_l.append("USE_STACK_ALLOCATOR=1")
	if (not r):
		def_l.append("DEBUG_BUILD")
	if (util.system=="windows"):
		def_l.extend(["_WINDOWS","WINDLL","USERDLL","WIN32_LEAN_AND_MEAN","_CRT_SECURE_NO_WARNINGS","__SLL_BUILD_WINDOWS"])
		win_def=[]
		for k in def_l:
			win_def.append("/D")
			win_def.append(k)
		if (r):
			util.log("  Compiling Files (Release Mode)...")
			out_fl=[]
			err=False
			for k in fl:
				out_fp=util.output_file_path(k)
				out_fl.append(out_fp)
				if (hashlist.update(k,"src/sll/include") and util.execute(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/O2","/MD","/I","src/sll/include","/Fo"+out_fp,k]+win_def)):
					hashlist.fail(k)
					err=True
			if (err):
				sys.exit(1)
			util.log("  Linking Files (Release Mode)...")
			if (util.execute(["link",f"/OUT:build/{lib_nm}.dll","/DLL","/DYNAMICBASE","/MACHINE:X64","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:NOREF","/INCREMENTAL:NO","/RELEASE","bcrypt.lib","advapi32.lib"]+out_fl)):
				sys.exit(1)
		else:
			util.log("  Compiling Files...")
			out_fl=[]
			err=False
			for k in fl:
				out_fp=util.output_file_path(k)
				out_fl.append(out_fp)
				if (hashlist.update(k,"src/sll/include") and util.execute(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/Zi","/Od","/RTC1","/MDd","/I","src/sll/include","/Fdbuild/sll.pdb","/Fo"+out_fp,k]+win_def)):
					hashlist.fail(k)
					err=True
			if (err):
				sys.exit(1)
			util.log("  Linking Files...")
			if (util.execute(["link",f"/OUT:build/{lib_nm}.dll","/DLL","/DYNAMICBASE","/MACHINE:X64","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL:NO","/RELEASE","/PDB:build/sll.pdb","bcrypt.lib","advapi32.lib"]+out_fl)):
				sys.exit(1)
	else:
		def_l.extend(["_GNU_SOURCE","__SLL_BUILD_"+util.system.upper()])
		linux_def=[]
		for k in def_l:
			linux_def.append("-D")
			linux_def.append(k)
		if (r):
			util.log("  Compiling Files (Release Mode)...")
			out_fl=[]
			err=False
			for k in fl:
				out_fp=util.output_file_path(k)
				out_fl.append(out_fp)
				if (hashlist.update(k,"src/sll/include") and util.execute(["gcc","-fno-exceptions","-fdiagnostics-color=always","-fPIC","-c","-fvisibility=hidden","-Wall","-O3","-Werror","-I","src/sll/include","-o",out_fp,k]+linux_def)):
					hashlist.fail(k)
					err=True
			if (err):
				sys.exit(1)
			util.log("  Linking Files (Release Mode)...")
			if (util.execute(["gcc","-fno-exceptions","-fdiagnostics-color=always","-shared","-fPIC","-fvisibility=hidden","-Wall","-O3","-Werror","-o",f"build/{lib_nm}.so"]+out_fl+["-lm","-ldl","-pthread"])):
				sys.exit(1)
			if (util.system!="darwin"):
				util.log("  Stripping Executable...")
				if (util.execute(["strip",f"build/{lib_nm}.so","-s","-R",".note.*","-R",".comment"])):
					sys.exit(1)
		else:
			util.log("  Compiling Files...")
			out_fl=[]
			err=False
			for k in fl:
				out_fp=util.output_file_path(k)
				out_fl.append(out_fp)
				if (hashlist.update(k,"src/sll/include") and util.execute(["gcc","-fno-exceptions","-fdiagnostics-color=always","-fPIC","-c","-fvisibility=hidden","-Wall","-g","-O0","-Werror","-I","src/sll/include","-o",out_fp,k]+linux_def)):
					hashlist.fail(k)
					err=True
			if (err):
				sys.exit(1)
			util.log("  Linking Files...")
			if (util.execute(["gcc","-fno-exceptions","-fdiagnostics-color=always","-shared","-fPIC","-fvisibility=hidden","-Wall","-Werror","-g","-O0","-o",f"build/{lib_nm}.so"]+out_fl+["-lm","-ldl","-pthread"])):
				sys.exit(1)



def build_sll_cli():
	if (util.system=="windows"):
		util.log("  Compiling Resource Files...")
		if (util.execute(["rc","/r","/fo","build/app.res","/nologo","src/cli/resources/app.rc"])):
			sys.exit(1)
		util.log("  Compiling Files (Release Mode)...")
		out_fp=util.output_file_path("src/cli/windows.c")
		if (hashlist.update("src/cli/windows.c","build","src/cli/include") and util.execute(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","_CRT_SECURE_NO_WARNINGS","/D","WIN32_LEAN_AND_MEAN","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/O2","/MD","/I","build","/Fo"+out_fp,"src/cli/windows.c"])):
			hashlist.fail("src/cli/windows.c")
			sys.exit(1)
		util.log("  Linking Files (Release Console Mode)...")
		if (util.execute(["link",out_fp,"build/app.res","/OUT:build/sll.exe","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO","/RELEASE"])):
			sys.exit(1)
		util.log("  Linking Files (Release Mode)...")
		if (util.execute(["link",out_fp,"build/app.res","/OUT:build/sllw.exe","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:WINDOWS","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO","/RELEASE"])):
			sys.exit(1)
	else:
		util.log("  Compiling Files...")
		out_fp=util.output_file_path("src/cli/linux.c")
		if (hashlist.update("src/cli/linux.c","build") and util.execute(["gcc","-fno-exceptions","-fdiagnostics-color=always","-c","-fvisibility=hidden","-Wall","-O3","-Werror","-D","__SLL_BUILD_"+util.system.upper(),"-I","build","-o",out_fp,"src/cli/linux.c"])):
			hashlist.fail("src/cli/linux.c")
			sys.exit(1)
		util.log("  Linking Files...")
		if (util.execute(["gcc","-fno-exceptions","-fdiagnostics-color=always","-fvisibility=hidden","-Wall","-O3","-Werror","-o","build/sll",out_fp,"-ldl"])):
			sys.exit(1)
		if (util.system!="darwin"):
			util.log("  Stripping Executable...")
			if (util.execute(["strip","build/sll","-s","-R",".note.*","-R",".comment"])):
				sys.exit(1)
