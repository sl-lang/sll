import os
import re
import subprocess
import sys



COMMENT_REGEX=re.compile(br"\/\*.*?\*\/|\/\/.*?$",re.DOTALL|re.MULTILINE)
COMPILATION_DEFINES=([b"_MSC_VER",b"_WINDOWS",b"WINDLL",b"USERDLL",b"DLL1_EXPORTS",b"_UNICODE",b"UNICODE"]+([b"NDEBUG"] if "--release" in sys.argv else [b"_DEBUG"]) if os.name=="nt" else [])
DEFAULT_ARGS=["test.lll","-v","-O3","-c","-o","build/test.lllc","-fp"]
DEFINE_LINE_CONTINUE_REGEX=re.compile(br"\\\n[ \t\r]*")
DEFINE_REMOVE_REGEX=re.compile(br"^[ \t\r]*(#define [a-zA-Z0-9_]+\([^\)]*\))[ \t\r]*(\\\n(?:[ \t\r]*.*\\\n)+[ \t\r]*.*\n?)",re.MULTILINE)
HEADER_SINGLE_INCLUDE_REGEX=re.compile(br"^\s*#ifndef\s+(?P<h_nm>[a-zA-Z0-9_]+)\s+#define\s+(?P=h_nm)\s+(?:1\s+)?(.*)#endif\s*$",re.DOTALL)
IDENTIFIER_CHARACTERS=b"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"
MULTIPLE_NEWLINE_REGEX=re.compile(br"\n+")
SPACE_CHARACTERS=b" \t\n\v\f\r"



def _expand_macros(k,dm,dfm):
	for e,v in dm.items():
		k=re.sub(br"\b"+e+br"\b",v,k)
	for e,v in dm.items():
		i=0
		while (i<len(k)):
			if ((i==0 or k[i-1:i] not in IDENTIFIER_CHARACTERS) and k.startswith(e,i)):
				j=i
				i+=len(e)
				while (k[i:i+1] in SPACE_CHARACTERS):
					i+=1
				if (k[i:i+1]!=b"("):
					i=j+len(e)
					k=k[:j]+v+k[i:]
					i+=len(v)-(i-j)
			i+=1
	for e,v in dfm.items():
		i=0
		while (i<len(k)):
			if ((i==0 or k[i-1:i] not in IDENTIFIER_CHARACTERS) and k.startswith(e,i)):
				j=i
				i+=len(e)
				while (k[i:i+1] in SPACE_CHARACTERS):
					i+=1
				if (k[i:i+1]==b"("):
					b=0
					al=[b""]
					ali=0
					while (True):
						if (k[i:i+1]==b"("):
							b+=1
							if (b==1):
								i+=1
								continue
						elif (k[i:i+1]==b")"):
							b-=1
						if (b==0):
							break
						if (k[i:i+1]==b","):
							al.append(b"")
							ali+=1
						else:
							al[ali]+=k[i:i+1]
						i+=1
					if (len(al)!=len(v[0])):
						raise RuntimeError("Invalid Macro Invocation (Argument Count Mismatch)")
					s=b""
					for st in v[1]:
						if (type(st)==int):
							s+=al[st]
						else:
							s+=st
					k=k[:j]+s+k[i+1:]
					i+=len(s)-(i-j)
			i+=1
	return k



if (os.path.exists("build")):
	dl=[]
	for r,ndl,fl in os.walk("build"):
		r=r.replace("\\","/").rstrip("/")+"/"
		for d in ndl:
			dl.insert(0,r+d)
		for f in fl:
			os.remove(r+f)
	for k in dl:
		os.rmdir(k)
else:
	os.mkdir("build")
h_dt=b""
inc_r=br""
for r,_,fl in os.walk("src/include"):
	r=r.replace("\\","/").rstrip("/")+"/"
	for f in fl:
		if (f[0]!="_" and f[-2:]==".h"):
			with open(r+f,"rb") as rf:
				if (len(inc_r)==0):
					inc_r+=br"(?:^[ \t\f]*#include\s*(?:"
				else:
					inc_r+=br"|"
				inc_r+=bytes(f"<{f.replace('.',chr(92)+'.')}>|\\\"{f.replace('.',chr(92)+'.')}\\\"","utf-8")
				dt=rf.read()
				m=HEADER_SINGLE_INCLUDE_REGEX.fullmatch(dt)
				if (m!=None):
					h_dt+=m.group(2)+b"\n\n\n"
				else:
					h_dt+=dt+b"\n\n\n"
inc_r+=br")|//[^\n]*)$"
h_dt=DEFINE_REMOVE_REGEX.sub(lambda g:g.group(1)+b" "+DEFINE_LINE_CONTINUE_REGEX.sub(br"",g.group(2)),MULTIPLE_NEWLINE_REGEX.sub(br"\n",re.sub(inc_r,br"",COMMENT_REGEX.sub(br"",h_dt)).strip().replace(b"\r\n",b"\n"))).split(b"\n")
l=[]
st=[True]
dm={}
dfm={}
d_v=[]
d_f=[]
il=b""
for e in h_dt:
	e=e.strip()
	if (e[:1]==b"#"):
		f=e[1:].split(b" ")
		if (f[0]==b"ifdef"):
			if (f[1] in COMPILATION_DEFINES or f[1] in dm):
				st.append(True)
			else:
				st.append(False)
			continue
		elif (f[0]==b"ifndef"):
			if (f[1] in COMPILATION_DEFINES or f[1] in dm):
				st.append(False)
			else:
				st.append(True)
			continue
		elif (f[0]==b"else"):
			st[-1]=not st[-1]
			continue
		elif (f[0]==b"endif"):
			st=st[:-1]
			continue
		elif (f[0]==b"include"and st[-1] is True):
			il+=b"\n#include "+b" ".join(f[1:])
			continue
		elif (f[0]==b"define"and st[-1] is True):
			if (f[1][:2]==b"__" ):
				dm[f[1]]=b" ".join(f[2:])
			else:
				if (b"(" in f[1]):
					al=f[1].split(b"(")[1].split(b")")[0].split(b",")
					if (al[-1]==b""):
						al.pop(len(al)-1)
					al=tuple(al)
					k=b" ".join(f[2:])
					d_f.append((f[1],k))
					sl=[b""]
					sli=0
					i=0
					while (i<len(k)):
						if (i==0 or k[i-1:i] not in IDENTIFIER_CHARACTERS):
							nxt=False
							for j,e in enumerate(al):
								if (k.startswith(e,i) and k[i+len(e):i+len(e)+1] not in IDENTIFIER_CHARACTERS):
									sl.append(j)
									sl.append(b"")
									sli+=2
									i+=len(e)
									nxt=True
									break
							if (nxt):
								continue
						sl[sli]+=k[i:i+1]
						i+=1
					dfm[f[1].split(b"(")[0]]=(al,tuple(sl))
				else:
					dm[f[1]]=b" ".join(f[2:])
					d_v.append((f[1],b" ".join(f[2:])))
			continue
	if (st[-1] is True):
		l.append(e)
d_s=b""
o=b""
for k,v in d_v:
	while (True):
		nv=_expand_macros(v,dm,dfm)
		if (nv==v):
			break
		v=nv
	d_s+=b"\n#define "+k+b" "+v
for k,v in d_f:
	while (True):
		nv=_expand_macros(v,dm,dfm)
		if (nv==v):
			break
		v=nv
	d_s+=b"\n#define "+k+b" "+v
for k in l:
	while (True):
		nk=_expand_macros(k,dm,dfm)
		if (nk==k):
			break
		k=nk
	o+=b"\n"+k
i=0
sd=b""
while (i<len(o)):
	if (i<len(o)-7 and o[i:i+1] not in IDENTIFIER_CHARACTERS and o[i+1:i+6]==b"union" and o[i+6:i+7] not in IDENTIFIER_CHARACTERS):
		j=i
		i+=6
		while (o[i] in SPACE_CHARACTERS):
			i+=1
		k=i
		while (o[i] in IDENTIFIER_CHARACTERS):
			i+=1
		nm=o[k:i]
		while (o[i] in SPACE_CHARACTERS):
			i+=1
		if (o[i:i+1]==b";"):
			sd+=b"\nunion "+nm+b";"
			o=o[:j]+o[i+1:]
			i-=i-j
			continue
		if (o[i:i+1]==b"{"):
			b=1
			i+=1
			while (b!=0):
				if (o[i:i+1]==b"{"):
					b+=1
				elif (o[i:i+1]==b"}"):
					b-=1
				else:
					j=i
					while (i<len(o) and o[i] in SPACE_CHARACTERS):
						i+=1
					if (i!=j):
						o=o[:j]+(b"" if o[j-1] not in IDENTIFIER_CHARACTERS else b" ")+o[i:]
						i-=i-j+(0 if o[j-1] not in IDENTIFIER_CHARACTERS else -1)
						continue
				i+=1
	elif (i<len(o)-8 and o[i:i+1] not in IDENTIFIER_CHARACTERS and o[i+1:i+7]==b"struct" and o[i+7:i+8] not in IDENTIFIER_CHARACTERS):
		j=i
		i+=7
		while (o[i] in SPACE_CHARACTERS):
			i+=1
		k=i
		while (o[i] in IDENTIFIER_CHARACTERS):
			i+=1
		nm=o[k:i]
		while (o[i] in SPACE_CHARACTERS):
			i+=1
		if (o[i:i+1]==b";"):
			sd+=b"\nstruct "+nm+b";"
			o=o[:j]+o[i+1:]
			i-=i-j
			continue
		if (o[i:i+1]==b"{"):
			b=1
			i+=1
			while (b!=0):
				if (o[i:i+1]==b"{"):
					b+=1
				elif (o[i:i+1]==b"}"):
					b-=1
				else:
					j=i
					while (i<len(o) and o[i] in SPACE_CHARACTERS):
						i+=1
					if (i!=j):
						o=o[:j]+(b"" if o[j-1] not in IDENTIFIER_CHARACTERS else b" ")+o[i:]
						i-=i-j+(0 if o[j-1] not in IDENTIFIER_CHARACTERS else -1)
						continue
				i+=1
	i+=1
with open("build/lll_lib.h","wb") as wf:
	wf.write(b"#ifndef __LLL_H__\n#define __LLL_H__ 1"+il+d_s+sd+b"\n"+o.strip()+b"\n#endif\n")
if (os.name=="nt"):
	cd=os.getcwd()
	os.chdir("build")
	if ("--release" in sys.argv):
		if (subprocess.run(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","WINDLL","/D","USERDLL","/D","DLL1_EXPORTS","/D","_UNICODE","/D","UNICODE","/D","__LLL_LIB_COMPILATION__","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/FC","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/Oi","/MD","/I","../src/include","../src/lll_lib/*.c"]).returncode!=0 or subprocess.run(["link","*.obj","/OUT:lll_lib.dll","/DLL","/DYNAMICBASE","kernel32.lib","user32.lib","gdi32.lib","winspool.lib","comdlg32.lib","advapi32.lib","shell32.lib","ole32.lib","oleaut32.lib","uuid.lib","odbc32.lib","odbccp32.lib","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO","/OPT:ICF"]).returncode!=0 or subprocess.run(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/FC","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/Oi","/MD","/I",".","../src/main.c"]).returncode!=0 or subprocess.run(["link","main.obj","/OUT:lll.exe","/DYNAMICBASE","kernel32.lib","user32.lib","gdi32.lib","winspool.lib","comdlg32.lib","advapi32.lib","shell32.lib","ole32.lib","oleaut32.lib","uuid.lib","odbc32.lib","odbccp32.lib","lll_lib.lib","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO","/OPT:ICF"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
	else:
		if (subprocess.run(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","WINDLL","/D","USERDLL","/D","DLL1_EXPORTS","/D","_UNICODE","/D","UNICODE","/D","__LLL_LIB_COMPILATION__","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/FC","/EHsc","/nologo","/diagnostics:column","/ZI","/Od","/RTC1","/MDd","/I","../src/include","../src/lll_lib/*.c"]).returncode!=0 or subprocess.run(["link","*.obj","/OUT:lll_lib.dll","/DLL","/DYNAMICBASE","kernel32.lib","user32.lib","gdi32.lib","winspool.lib","comdlg32.lib","advapi32.lib","shell32.lib","ole32.lib","oleaut32.lib","uuid.lib","odbc32.lib","odbccp32.lib","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL"]).returncode!=0 or subprocess.run(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/FC","/EHsc","/nologo","/diagnostics:column","/ZI","/Od","/RTC1","/MDd","/I",".","../src/main.c"]).returncode!=0 or subprocess.run(["link","main.obj","/OUT:lll.exe","/DYNAMICBASE","kernel32.lib","user32.lib","gdi32.lib","winspool.lib","comdlg32.lib","advapi32.lib","shell32.lib","ole32.lib","oleaut32.lib","uuid.lib","odbc32.lib","odbccp32.lib","lll_lib.lib","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
	os.chdir(cd)
	if ("--run" in sys.argv):
		subprocess.run(["build/lll.exe"]+DEFAULT_ARGS)
else:
	if ("--release" in sys.argv):
		fl=[]
		for f in os.listdir("src/lll_lib"):
			if (f[-2:]==".c"):
				fl.append(f"build/{f}.o")
				if (subprocess.run(["gcc","-fPIC","-D","__LLL_LIB_COMPILATION__","-Wall","-lm","-Werror","-O3","-c","src/lll_lib/"+f,"-o",f"build/{f}.o","-Isrc/include"]).returncode!=0):
					sys.exit(1)
		if (subprocess.run(["gcc","-shared","-D","__LLL_LIB_COMPILATION__","-Wall","-O3","-Werror","-o","build/lll_lib.so"]+fl+["-lm"]).returncode!=0 or subprocess.run(["gcc","-Wall","-lm","-Werror","-O3","-c","src/main.c","-o","build/main.o","-Ibuild"]).returncode!=0 or subprocess.run(["gcc","-o","build/lll","-O3","build/main.o","build/lll_lib.so","-lm"]).returncode!=0):
			sys.exit(1)
	else:
		fl=[]
		for f in os.listdir("src/lll_lib"):
			if (f[-2:]==".c"):
				fl.append(f"build/{f}.o")
				if (subprocess.run(["gcc","-fPIC","-D","__LLL_LIB_COMPILATION__","-Wall","-lm","-Werror","-O0","-c","src/lll_lib/"+f,"-o",f"build/{f}.o","-Isrc/include"]).returncode!=0):
					sys.exit(1)
		if (subprocess.run(["gcc","-shared","-D","__LLL_LIB_COMPILATION__","-Wall","-O0","-Werror","-o","build/lll_lib.so"]+fl+["-lm"]).returncode!=0 or subprocess.run(["gcc","-Wall","-lm","-Werror","-O0","-c","src/main.c","-o","build/main.o","-Ibuild"]).returncode!=0 or subprocess.run(["gcc","-o","build/lll","-O0","build/main.o","build/lll_lib.so","-lm"]).returncode!=0):
			sys.exit(1)
	if ("--run" in sys.argv):
		subprocess.run(["build/lll"]+DEFAULT_ARGS)
