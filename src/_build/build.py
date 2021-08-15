import datetime
import header
import os
import re
import sys
import util



DEFINE_REGEX=re.compile(br"^[ \t]*#[ \t]*define[ \t]+([a-zA-Z0-9_]+)",re.M)
IDENTIFIER_CHARACTERS=b"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"
MONTHS=["Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"]
SPACE_CHARACTERS_REGEX=re.compile(b" \t\n\v\f\r")



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



def convert_to_coverage(i_fp,o_fp,b_fp,cm,vb):
	with open(i_fp,"rb") as f:
		dt=f.read().replace(b"\r\n",b"\n")
	wnd=(b"#include <windows.h>" in dt)
	dt=dt.split(b"\n")
	st=[True]
	tm=datetime.datetime.now()
	dm={b"__TIME__":bytes(tm.strftime("\"%H:%M:%S\""),"utf-8"),b"__DATE__":bytes(tm.strftime(f"\"{MONTHS[tm.month-1]} %d %Y\""),"utf-8"),b"UINT8_MAX":b"255",b"UINT16_MAX":b"65535",b"UINT32_MAX":b"4294967295u",b"UINT64_MAX":b"18446744073709551615ull",b"__LINE__":b"0",b"__FILE_ID__":b"-1"}
	for k in cm:
		dm[k]=b""
	dfm={}
	inc=[b"windows.h"]
	ln=0
	i=0
	while (i<len(dt)):
		k=dt[i].strip()
		if (k[:1]==b"#"):
			j=1
			while (k[j:j+1] in b" \t"):
				j+=1
			if (k[j:j+4]==b"else"):
				st[-1]=not st[-1]
				dt[i]=b""
			elif (k[j:j+5]==b"endif"):
				st.pop()
				dt[i]=b""
			elif (st[-1]):
				if (k[j:j+7]==b"include"):
					j+=7
					while (k[j:j+1] in b" \t"):
						j+=1
					j+=1
					l=j
					while (k[j:j+1]!=b">"):
						j+=1
					e=k[l:j]
					if (e not in inc):
						inc.append(e)
						if (e[:4]==b"sll/"):
							with open(b_fp+str(e,"utf-8"),"rb") as f:
								dt=dt[:i]+f.read().replace(b"\r\n",b"\n").split(b"\n")+dt[i+1:]
							i-=1
					else:
						dt[i]=b""
				elif (k[j:j+6]==b"define"):
					j+=6
					while (k[j:j+1] in b" \t"):
						j+=1
					if (j==len(k)):
						dm
					l=j
					while (k[j:j+1] in IDENTIFIER_CHARACTERS and j<len(k)):
						j+=1
					e=k[l:j]
					if (k[j:j+1]==b"("):
						j+=1
						l=j
						while (k[j:j+1]!=b")"):
							j+=1
						al=[e.strip() for e in k[l:j].split(b",")]
						va=False
						if (al[-1]==b"..."):
							va=True
							al.pop()
						if (len(al)>0 and len(al[0])==0):
							al=tuple()
						else:
							al=tuple(al)
						j+=1
						m_dt=k[j:].lstrip()
						while (m_dt[-1:]==b"\\"):
							dt[i]=b""
							i+=1
							m_dt=m_dt[:-1].rstrip()+b" "+dt[i].strip()
						sl=[b""]
						sli=0
						l=0
						while (l<len(m_dt)):
							if (l==0 or m_dt[l-1:l] not in IDENTIFIER_CHARACTERS):
								if (m_dt.startswith(b"__VA_ARGS__",l) and (l+11==len(m_dt) or m_dt[l+11:l+12] not in IDENTIFIER_CHARACTERS)):
									sl.append(-1)
									sl.append(b"")
									sli+=2
									l+=11
									continue
								nxt=False
								for si,se in enumerate(al):
									if (m_dt.startswith(se,l) and (l+len(se)==len(m_dt) or m_dt[l+len(se):l+len(se)+1] not in IDENTIFIER_CHARACTERS)):
										sl.append(si)
										sl.append(b"")
										sli+=2
										l+=len(se)
										nxt=True
										break
								if (nxt):
									continue
							sl[sli]+=m_dt[l:l+1]
							l+=1
						sl.append(b"")
						for j,se in enumerate(sl):
							if ((j&1)==0 and se[-1:]==b"#" and se[-2:-1]!=b"#"):
								sl[j]=se[:-1]+b"\""
								sl[j+2]=b"\""+sl[j+2]
						if (len(sl[-1])==0):
							sl.pop()
						dfm[e]=(al,va,tuple(sl))
					else:
						dm[e]=k[j:].lstrip()
						while (dm[e][-1:]==b"\\"):
							dt[i]=b""
							i+=1
							dm[e]=dm[e][:-1].rstrip()+b" "+dt[i].strip()
					dt[i]=b""
				elif (k[j:j+5]==b"undef"):
					j+=5
					while (k[j:j+1] in b" \t"):
						j+=1
					l=j
					while (k[j:j+1] not in b" \t"):
						j+=1
					if (k[l:j] in dm):
						del dm[k[l:j]]
					else:
						del dfm[k[l:j]]
					dt[i]=b""
				elif (k[j:j+5]==b"ifdef" or k[j:j+6]==b"ifndef"):
					t=k[j:j+5]
					j+=6
					while (k[j:j+1] in b" \t"):
						j+=1
					l=j
					while (k[j:j+1] not in b" \t"):
						j+=1
					if (k[l:j] in dm or k[l:j] in dfm):
						st.append(t==b"ifdef")
					else:
						st.append(t!=b"ifdef")
					dt[i]=b""
				elif (k[j:j+4]==b"line"):
					j+=4
					while (k[j:j+1] in b" \t"):
						j+=1
					l=j
					while (k[j:j+1] not in b" \t"):
						j+=1
					ln=int(k[l:j])-1
			else:
				if (k[j:j+5]==b"ifdef" or k[j:j+6]==b"ifndef"):
					st.append(False)
				dt[i]=b""
		elif (st[-1]):
			dm[b"__LINE__"]=bytes(str(ln),"utf-8")
			dt[i]=header._expand_macros(dt[i],dm,dfm)
		else:
			dt[i]=b""
		i+=1
		ln+=1
	dt=b"\n".join(dt)+b"\n"
	f_nm={}
	i=0
	while (i<len(dt)):
		while (dt[i:i+1] in b" \t\r\n\f\v"):
			i+=1
		while (dt[i:i+1]==b"#"):
			while (dt[i:i+1]!=b"\n"):
				i+=1
			while (i!=len(dt) and dt[i:i+1] in b" \t\r\n\f\v"):
				i+=1
		if (i==len(dt)):
			break
		b=0
		j=i
		nm=None
		while (dt[i:i+1]!=b")" or b!=1):
			if (i==len(dt)):
				b=0
				break
			if (dt[i:i+1]==b"("):
				b+=1
			elif (dt[i:i+1]==b")"):
				b-=1
			elif (dt[i:i+1]==b";"):
				break
			elif (dt[i:i+1]==b"{"):
				b=1
				i+=1
				break
			i+=1
		else:
			i+=1
			while (dt[i:i+1] in b" \t\r\n\f\v"):
				i+=1
			b=0
			if (dt[i:i+1]==b"{"):
				b=1
				i+=1
				e=SPACE_CHARACTERS_REGEX.sub(b" ",dt[j:i+1])
				if (b"static" not in e):
					nm=e.split(b"(")[0].split(b" ")[-1]
					f_nm[nm]=([],[])
					s=b"__coverage_call("+bytes(str(len(f_nm)-1),"utf-8")+b");"
					dt=dt[:i]+s+dt[i:]
					i+=len(s)
		if (i==len(dt)):
			break
		while (b!=0):
			if (dt[i:i+1]==b"\'"):
				i+=1
				while (dt[i:i+1]!=b"\'"):
					if (dt[i:i+1]==b"\\"):
						i+=1
					i+=1
			elif (dt[i:i+1]==b"\""):
				i+=1
				while (dt[i:i+1]!=b"\""):
					if (dt[i:i+1]==b"\\"):
						i+=1
					i+=1
			elif (dt[i:i+1] in b"({"):
				b+=1
			elif (dt[i:i+1] in b")}"):
				b-=1
			elif (nm is not None):
				if (dt[i-1:i] not in IDENTIFIER_CHARACTERS):
					if (dt[i:i+5]==b"break" and dt[i+5:i+6] not in IDENTIFIER_CHARACTERS):
						f_nm[nm][0].append(None)
						s=b"__coverage_break("+bytes(str(len(f_nm[nm][0])-1),"utf-8")+b");"
						dt=dt[:i]+s+dt[i:]
						i+=len(s)
			i+=1
		while (dt[i:i+1] in b" \t\r\n\f\v"):
			i+=1
			if (i==len(dt)):
				break
		if (i==len(dt)):
			break
		i+=1
	with open(o_fp,"wb") as f:
		if (wnd):
			f.write(b"#define WIN32_LEAN_AND_MEAN 1\n#include <windows.h>\n")
		f.write(b"void __coverage_break(unsigned int);\nvoid __coverage_call(unsigned int);\nvoid __coverage_continue(unsigned int);\nvoid __coverage_goto(unsigned int);\n")
		f.write(dt)
	quit()



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
				print("  Compiling Library Files (Release Mode)...")
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","WINDLL","/D","USERDLL","/D","_UNICODE","/D","UNICODE","/D","__SLL_COMPILATION__","/D","__SLL_STATIC__","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/MD","/I","../src/include","/Fosll_coverage.obj",fp]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Compiling Files (Release Mode)...")
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/D","__SLL_STATIC__","/D","__TEST_ROOT_DIR__=\""+t_fp+"\"","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/MD","/I",".","/I","../tests","/Forun_tests.obj","../tests/run_tests.c"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Linking Files (Release Mode)...")
			if (util.wrap_output(["link","run_tests.obj","sll_coverage.obj","/OUT:run_tests.exe","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO","/RELEASE"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			if (vb):
				print("  Compiling Library Files...")
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","WINDLL","/D","USERDLL","/D","_UNICODE","/D","UNICODE","/D","__SLL_COMPILATION__","/D","__SLL_STATIC__","/D","DEBUG_BUILD","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/Zi","/Od","/RTC1","/MDd","/I","../src/include","/Fosll_coverage.obj",fp]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Compiling Files...")
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/D","DEBUG_BUILD","/D","__SLL_STATIC__","/D","__TEST_ROOT_DIR__=\""+t_fp+"\"","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/Zi","/Od","/RTC1","/MDd","/I",".","/I","../tests","/Forun_tests.obj","../tests/run_tests.c"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Linking Files...")
			if (util.wrap_output(["link","run_tests.obj","sll_coverage.obj","/OUT:run_tests.exe","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL:NO","/RELEASE"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	else:
		if (r):
			if (vb):
				print("  Compiling Library Files (Release Mode)...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-fPIC","-c","-fvisibility=hidden","-D","__SLL_COMPILATION__","-D","__SLL_STATIC__","-Wall","-O3","-Werror","-o","sll_coverage.o","-I","../src/include",fp,"-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Compiling & Linking Files (Release Mode)...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-D","__TEST_ROOT_DIR__=\""+t_fp+"\"","-Wall","-Werror","-O3","../tests/run_tests.c","sll_coverage.o","-o","run_tests","-I",".","-I","../tests","-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			if (vb):
				print("  Compiling Library Files...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-fPIC","-c","-fvisibility=hidden","-D","__SLL_COMPILATION__","-D","__SLL_STATIC__","-Wall","-O0","-Werror","-o","sll_coverage.o","-I","../src/include",fp,"-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Compiling & Linking Files...")
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-D","__TEST_ROOT_DIR__=\""+t_fp+"\"","-Wall","-Werror","-O0","../tests/run_tests.c","sll_coverage.o","-o","run_tests","-I",".","-I","../tests","-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	os.chdir(cd)
