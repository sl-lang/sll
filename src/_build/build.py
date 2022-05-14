import hashlist
import os
import sys
import util



def build_sll(fl,v,r):
	lib_nm=f"sll-{v[0]}.{v[1]}.{v[2]}"
	def_l=["__SLL_COMPILATION__",f"__SLL_TIME_RAW__={util.BUILD_TIME}","__SLL_BUILD_"+util.system.upper()]
	if (len(os.getenv("GITHUB_SHA",""))>0):
		def_l.append(f"__SLL_SHA__=\"{os.getenv('GITHUB_SHA')[:7]}\"")
		def_l.append(f"__SLL_FULL_SHA__=\"{os.getenv('GITHUB_SHA')}\"")
	if (len(os.getenv("USE_STACK_ALLOCATOR",""))>0):
		def_l.append("USE_STACK_ALLOCATOR=1")
	if (not r):
		def_l.append("DEBUG_BUILD")
	if (util.system=="windows"):
		def_l.extend(["_WINDOWS","WINDLL","USERDLL","WIN32_LEAN_AND_MEAN","_CRT_SECURE_NO_WARNINGS","NOGDICAPMASKS","NOVIRTUALKEYCODES","NOWINMESSAGES","NOWINSTYLES","NOSYSMETRICS","NOMENUS","NOICONS","NOKEYSTATES","NOSYSCOMMANDS","NORASTEROPS","NOSHOWWINDOW","OEMRESOURCE","NOATOM","NOCLIPBOARD","NOCOLOR","NOCTLMGR","NODRAWTEXT","NOGDI","NOKERNEL","NOUSER","NOMB","NOMEMMGR","NOMETAFILE","NOMINMAX","NOMSG","NOOPENFILE","NOSCROLL","NOSERVICE","NOSOUND","NOTEXTMETRIC","NOWH","NOWINOFFSETS","NOCOMM","NOKANJI","NOHELP","NOPROFILER","NODEFERWINDOWPOS","NOMCX"])
		win_def=[]
		for k in def_l:
			win_def.append("/D")
			win_def.append(k)
		if (r):
			util.log("  Compiling files (Release mode)...")
			out_fl=[]
			err=False
			for k in fl:
				out_fp=util.output_file_path(k)
				out_fl.append(out_fp)
				if (hashlist.update(k,"src/sll/include")):
					util.log("    "+k)
					if (util.execute((["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/O2","/MD","/I","src/sll/include","/Fo"+out_fp,k]+win_def if k[-2:]==".c" else ["nasm","-I","src/sll/include","-D__SLL_BUILD_"+util.system.upper(),"-o",out_fp,"-Ox","-Wall","-Werror","-f","win64",k]))):
						hashlist.fail(k)
						err=True
			if (err):
				sys.exit(1)
			util.log("  Linking files (Release mode)...")
			if (util.execute(["link",f"/OUT:build/{lib_nm}.dll","/DLL","/DYNAMICBASE","/MACHINE:X64","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:NOREF","/INCREMENTAL:NO","/RELEASE","advapi32.lib","bcrypt.lib"]+out_fl)):
				sys.exit(1)
		else:
			util.log("  Compiling files...")
			out_fl=[]
			err=False
			for k in fl:
				out_fp=util.output_file_path(k)
				out_fl.append(out_fp)
				if (hashlist.update(k,"src/sll/include")):
					util.log("    "+k)
					if (util.execute((["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/Zi","/Od","/RTC1","/MDd","/I","src/sll/include","/Fdbuild/sll.pdb","/Fo"+out_fp,k]+win_def if k[-2:]==".c" else ["nasm","-I","src/sll/include","-D__SLL_BUILD_"+util.system.upper(),"-DDEBUG_BUILD","-o",out_fp,"-O0","-Wall","-Werror","-gcv8","-f","win64",k]))):
						hashlist.fail(k)
						err=True
			if (err):
				sys.exit(1)
			util.log("  Linking files...")
			if (util.execute(["link",f"/OUT:build/{lib_nm}.dll","/DLL","/DYNAMICBASE","/MACHINE:X64","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL:NO","/RELEASE","/PDB:build/sll.pdb","advapi32.lib","bcrypt.lib"]+out_fl)):
				sys.exit(1)
	else:
		def_l.append("_GNU_SOURCE")
		linux_opt=[]
		for k in def_l:
			linux_opt.append("-D")
			linux_opt.append(k)
		if (util.system!="darwin"):
			linux_opt.append("-mavx")
			linux_opt.append("-mavx2")
		nasm_fmt=("macho64" if util.system=="darwin" else "elf64")
		if (r):
			if ("--debug" in sys.argv):
				linux_opt.append("-g")
			util.log("  Compiling files (Release mode)...")
			out_fl=[]
			err=False
			for k in fl:
				out_fp=util.output_file_path(k)
				out_fl.append(out_fp)
				if (hashlist.update(k,"src/sll/include")):
					util.log("    "+k)
					if (util.execute((["gcc","-fno-exceptions","-fno-stack-protector","-fdiagnostics-color=always","-fPIC","-c","-fvisibility=hidden","-Wall","-O3","-Werror","-I","src/sll/include","-o",out_fp,k]+linux_opt if k[-2:]==".c" else ["nasm","-I","src/sll/include","-D__SLL_BUILD_"+util.system.upper(),"-o",out_fp,"-O3","-Wall","-Werror","-f",nasm_fmt,k]))):
						hashlist.fail(k)
						err=True
			if (err):
				sys.exit(1)
			util.log("  Linking files (Release mode)...")
			if (util.execute(["gcc","-fno-exceptions","-fno-stack-protector","-fdiagnostics-color=always","-shared","-fPIC","-fvisibility=hidden","-Wall","-O3","-Werror","-o",f"build/{lib_nm}.so"]+out_fl+["-lm","-ldl","-pthread"]+linux_opt)):
				sys.exit(1)
			if (util.system!="darwin" and "--debug" not in sys.argv):
				util.log("  Stripping executable...")
				if (util.execute(["strip",f"build/{lib_nm}.so","-s","-R",".note.*","-R",".comment"])):
					sys.exit(1)
		else:
			link_opt=[]
			if (len(os.getenv("GENERATE_COVERAGE",""))>0):
				linux_opt.append("--coverage")
				link_opt.append("--coverage")
			util.log("  Compiling files...")
			out_fl=[]
			err=False
			for k in fl:
				out_fp=util.output_file_path(k)
				out_fl.append(out_fp)
				if (hashlist.update(k,"src/sll/include")):
					util.log("    "+k)
					if (util.execute((["gcc","-fno-exceptions","-fno-stack-protector","-fdiagnostics-color=always","-fPIC","-c","-fvisibility=hidden","-Wall","-g","-O0","-Werror","-I","src/sll/include","-o",out_fp,k]+linux_opt if k[-2:]==".c" else ["nasm","-I","src/sll/include","-D__SLL_BUILD_"+util.system.upper(),"-DDEBUG_BUILD","-o",out_fp,"-O0","-Wall","-Werror","-gdwarf","-f",nasm_fmt,k]))):
						hashlist.fail(k)
						err=True
			if (err):
				sys.exit(1)
			util.log("  Linking files...")
			if (util.execute(["gcc","-fno-exceptions","-fno-stack-protector","-fdiagnostics-color=always","-shared","-fPIC","-fvisibility=hidden","-Wall","-Werror","-g","-O0","-o",f"build/{lib_nm}.so"]+out_fl+["-lm","-ldl","-pthread"]+linux_opt+link_opt)):
				sys.exit(1)



def build_sll_cli():
	if (util.system=="windows"):
		util.log("  Compiling resource files...")
		if (util.execute(["rc","/r","/fo","build/app.res","/nologo","src/cli/rsrc/app.rc"])):
			sys.exit(1)
		util.log("  Compiling files (Release mode)...")
		out_fp=util.output_file_path("src/cli/windows.c")
		if (util.execute(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","_CRT_SECURE_NO_WARNINGS","/D","WIN32_LEAN_AND_MEAN","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/O2","/MD","/I","build","/Fo"+out_fp,"src/cli/windows.c"])):
			hashlist.fail("src/cli/windows.c")
			sys.exit(1)
		util.log("  Linking files (Release console mode)...")
		if (util.execute(["link",out_fp,"build/app.res","/OUT:build/sll.exe","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO","/RELEASE"])):
			sys.exit(1)
		util.log("  Linking files (Release mode)...")
		if (util.execute(["link",out_fp,"build/app.res","/OUT:build/sllw.exe","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:WINDOWS","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO","/RELEASE"])):
			sys.exit(1)
	else:
		util.log("  Compiling files...")
		out_fp=util.output_file_path("src/cli/linux.c")
		if (util.execute(["gcc","-fno-exceptions","-fno-stack-protector","-fdiagnostics-color=always","-c","-fvisibility=hidden","-Wall","-O3","-Werror","-D","__SLL_BUILD_"+util.system.upper(),"-I","build","-o",out_fp,"src/cli/linux.c"])):
			hashlist.fail("src/cli/linux.c")
			sys.exit(1)
		util.log("  Linking files...")
		if (util.execute(["gcc","-fno-exceptions","-fno-stack-protector","-fdiagnostics-color=always","-fvisibility=hidden","-Wall","-O3","-Werror","-o","build/sll",out_fp,"-ldl"])):
			sys.exit(1)
		if (util.system!="darwin"):
			util.log("  Stripping executable...")
			if (util.execute(["strip","build/sll","-s","-R",".note.*","-R",".comment"])):
				sys.exit(1)
