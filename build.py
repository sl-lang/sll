import datetime
import os
import re
import subprocess
import sys



COMMENT_REGEX=re.compile(br"\/\*.*?\*\/|\/\/.*?$",re.DOTALL|re.MULTILINE)
COMPILATION_DEFINES=([b"_MSC_VER",b"_WINDOWS",b"WINDLL",b"USERDLL",b"_UNICODE",b"UNICODE"]+([b"NDEBUG"] if "--release" in sys.argv else [b"_DEBUG"]) if os.name=="nt" else [])
DEFINE_LINE_CONTINUE_REGEX=re.compile(br"\\\n[ \t\r]*")
DEFINE_REMOVE_REGEX=re.compile(br"^[ \t\r]*(#define [a-zA-Z0-9_]+\([^\)]*\))[ \t\r]*(\\\n(?:[ \t\r]*.*\\\n)+[ \t\r]*.*\n?)",re.MULTILINE)
ESCAPE_CHARACTER_REGEX=re.compile(br"[^\x20-~]")
HEADER_SINGLE_INCLUDE_REGEX=re.compile(br"^\s*#ifndef\s+(?P<h_nm>[a-zA-Z0-9_]+)\s+#define\s+(?P=h_nm)\s+(?:1\s+)?(.*)#endif\s*$",re.DOTALL)
HELP_FILE_PATH="rsrc/help.txt"
HEX_NUMBER_REGEX=re.compile(br"\b0x[0-9a-f]+\b")
IDENTIFIER_CHARACTERS=b"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"
LETTERS=b"abcdefghijklmnopqrstuvwxyz"
MONTHS=["Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"]
MULTIPLE_NEWLINE_REGEX=re.compile(br"\n+")
SPACE_CHARACTERS=b" \t\n\v\f\r"
SPACE_CHARACTERS_REGEX=re.compile(b" \t\n\v\f\r")
IDENTIFIER_REGEX=re.compile(br"\b[a-zA-Z0-9_]+\b")



def _expand_macros(k,dm,dfm):
	for e,v in dm.items():
		k=re.sub(br"\b"+e+br"\b",v,k)
	while (True):
		u=False
		for e,v in dm.items():
			i=0
			while (i<len(k)):
				if ((i==0 or k[i-1:i] not in IDENTIFIER_CHARACTERS) and k.startswith(e,i)):
					j=i
					i+=len(e)
					while (i<len(k) and k[i:i+1] in SPACE_CHARACTERS):
						i+=1
					if (k[i:i+1] not in b"("+IDENTIFIER_CHARACTERS):
						i=j+len(e)
						k=k[:j]+v+k[i:]
						i+=len(v)-(i-j)
						u=True
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
						u=True
				i+=1
		if (u is False):
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
with open(HELP_FILE_PATH,"rb") as rf,open("build/generated.h","wb") as wf:
	wf.write(b"#ifndef __GENERATED_H__\n#define __GENERATED_H__ 1\n\n\n\nconst char HELP_TEXT[]={")
	st=True
	for c in rf.read().replace(b"\r\n",b"\n"):
		if (st is False):
			wf.write(b",")
		wf.write(bytearray([48,120,(48 if (c>>4)<10 else 87)+(c>>4),(48 if (c&0xf)<10 else 87)+(c&0xf)]))
		st=False
	wf.write(b",0x00};\n\n\n\n#endif")
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
tm=datetime.datetime.now()
dm={b"__TIME__":bytes(tm.strftime("\"%H:%M:%S\""),"utf-8"),b"__DATE__":bytes(tm.strftime(f"\"{MONTHS[tm.month-1]} %d %Y\""),"utf-8")}
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
			st.pop()
			continue
		elif (f[0]==b"include" and False not in st):
			f_nm="src/include/"+str(b" ".join(f[1:]).strip(b"\"").strip(b"<").strip(b">"),"utf-8")
			if (not os.path.exists(f_nm)):
				il+=b"\n#include "+b" ".join(f[1:])
			continue
		elif (f[0]==b"define"and False not in st):
			if (f[1][:2]==b"__"):
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
							for j,se in enumerate(al):
								if (k.startswith(se,i) and k[i+len(se):i+len(se)+1] not in IDENTIFIER_CHARACTERS):
									sl.append(j)
									sl.append(b"")
									sli+=2
									i+=len(se)
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
	if (False not in st):
		l.append(e)
d_s=b""
o=b""
for i,(k,v) in enumerate(sorted(d_v,key=lambda e:e[0])):
	while (True):
		nv=_expand_macros(v,dm,dfm)
		if (nv==v):
			break
		v=nv
	v=HEX_NUMBER_REGEX.sub(lambda m:bytes(str(int(m.group(0),16)),"utf-8"),v)
	d_v[i]=(k,v)
	d_s+=b"\n#define "+k+b" "+v.strip()
for i,(k,v) in enumerate(sorted(d_f,key=lambda e:e[0])):
	k=SPACE_CHARACTERS_REGEX.sub(b"",k)
	a={}
	j=[0]
	nk=k.split(b"(")[0]+b"("
	for e in k[k.index(b"(")+1:-1].split(b","):
		if (len(j)>1 or j[0]!=0):
			nk+=b","
		a[e]=b"".join([LETTERS[m:m+1] for m in j])
		nk+=a[e]
		j[-1]+=1
		for m in range(len(j)-1,-1,-1):
			if (j[m]==len(LETTERS)):
				j[m]=0
				if (m==0):
					j.insert(0,0)
				else:
					j[m-1]+=1
			else:
				break
	nk+=b")"
	while (True):
		nv=_expand_macros(v,dm,dfm)
		if (nv==v):
			break
		v=nv
	v=IDENTIFIER_REGEX.sub(lambda m:(a[m.group(0)] if m.group(0) in a else m.group(0)),HEX_NUMBER_REGEX.sub(lambda m:bytes(str(int(m.group(0),16)),"utf-8"),v))
	d_f[i]=(nk,v)
	d_s+=b"\n#define "+nk+b" "+v.strip()
fl=[]
for k in l:
	while (True):
		nk=_expand_macros(k,dm,dfm)
		if (nk==k):
			break
		k=nk
	if (b"(" in k and b"(*" not in SPACE_CHARACTERS_REGEX.sub(b"",k) and k.count(b"(")==k.count(b")") and k.count(b"{")==k.count(b"}")):
		fl.append((k[:-len(k.split(b"(")[-1])-1].split(b" ")[-1],k))
		continue
	o+=b"\n"+k.strip()
i=0
while (i<len(o)):
	if (i<len(o)-7 and o[i:i+1] not in IDENTIFIER_CHARACTERS and o[i+1:i+6]==b"union" and o[i+6:i+7] not in IDENTIFIER_CHARACTERS):
		i+=6
		while (o[i] not in b";{"):
			i+=1
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
		i+=7
		while (o[i] not in b";{"):
			i+=1
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
for k,v in sorted(fl,key=lambda e:e[0]):
	if (len(o)>0):
		o+=b"\n"
	ai=len(v.split(b"(")[-1])
	o+=v[:-ai]
	st=True
	for a in v[-ai:].split(b")")[0].split(b","):
		if (st is False):
			o+=b","
		st=False
		o+=a[:-len(a.split(b" ")[-1])].strip()
	o+=b");"
with open("build/lll_lib.h","wb") as wf:
	wf.write(b"#ifndef __LLL_H__\n#define __LLL_H__ 1"+il+d_s+b"\n"+o.strip()+b"\n#endif\n")
if (os.name=="nt"):
	cd=os.getcwd()
	os.chdir("build")
	if ("--release" in sys.argv):
		if (subprocess.run(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","WINDLL","/D","USERDLL","/D","_UNICODE","/D","UNICODE","/D","__LLL_LIB_COMPILATION__","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/FC","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/Oi","/MD","/I","../src/include","../src/lll_lib/*.c"]).returncode!=0 or subprocess.run(["link","*.obj","/OUT:lll_lib.dll","/DLL","/DYNAMICBASE","kernel32.lib","user32.lib","gdi32.lib","winspool.lib","comdlg32.lib","advapi32.lib","shell32.lib","ole32.lib","oleaut32.lib","uuid.lib","odbc32.lib","odbccp32.lib","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO","/OPT:ICF"]).returncode!=0 or subprocess.run(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/FC","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/Oi","/MD","/I",".","../src/main.c"]).returncode!=0 or subprocess.run(["link","main.obj","/OUT:lll.exe","/DYNAMICBASE","kernel32.lib","user32.lib","gdi32.lib","winspool.lib","comdlg32.lib","advapi32.lib","shell32.lib","ole32.lib","oleaut32.lib","uuid.lib","odbc32.lib","odbccp32.lib","lll_lib.lib","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO","/OPT:ICF"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
	else:
		if (subprocess.run(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","WINDLL","/D","USERDLL","/D","_UNICODE","/D","UNICODE","/D","__LLL_LIB_COMPILATION__","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/FC","/EHsc","/nologo","/diagnostics:column","/ZI","/Od","/RTC1","/MDd","/I","../src/include","../src/lll_lib/*.c"]).returncode!=0 or subprocess.run(["link","*.obj","/OUT:lll_lib.dll","/DLL","/DYNAMICBASE","kernel32.lib","user32.lib","gdi32.lib","winspool.lib","comdlg32.lib","advapi32.lib","shell32.lib","ole32.lib","oleaut32.lib","uuid.lib","odbc32.lib","odbccp32.lib","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL"]).returncode!=0 or subprocess.run(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/FC","/EHsc","/nologo","/diagnostics:column","/ZI","/Od","/RTC1","/MDd","/I",".","../src/main.c"]).returncode!=0 or subprocess.run(["link","main.obj","/OUT:lll.exe","/DYNAMICBASE","kernel32.lib","user32.lib","gdi32.lib","winspool.lib","comdlg32.lib","advapi32.lib","shell32.lib","ole32.lib","oleaut32.lib","uuid.lib","odbc32.lib","odbccp32.lib","lll_lib.lib","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
	os.chdir(cd)
	if ("--run" in sys.argv):
		os.chdir("build")
		subprocess.run(["lll.exe","-h"])
		if (("--generate-code" in sys.argv and (subprocess.run(["lll.exe","../example/test.lll","-v","-O0","-c","-R","-o","test.lllc","-p","-fp","-I","../example","-m"]).returncode!=0 or subprocess.run(["lll.exe","test.lllc","-v","-O3","-p","-fp","-L","-R"]).returncode!=0 or subprocess.run(["nasm","-O3","-f","win64","-o","test.obj","test.asm"]).returncode!=0 or subprocess.run(["link","test.obj","lll_lib.lib","msvcrt.lib","/DYNAMICBASE","/ENTRY:main","/OUT:test.exe","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL"]).returncode!=0 or subprocess.run("test.exe").returncode!=0)) or ("--generate-code" not in sys.argv and (subprocess.run(["lll.exe","../example/test.lll","-v","-O0","-c","-o","test.lllc","-p","-fp","-I","../example","-m","-R"]).returncode!=0 or subprocess.run(["lll.exe","test.lllc","-v","-O3","-p","-fp","-L","-G"]).returncode!=0))):
			os.chdir(cd)
			sys.exit(1)
		os.chdir(cd)
else:
	if ("--release" in sys.argv):
		fl=[]
		for f in os.listdir("src/lll_lib"):
			if (f[-2:]==".c"):
				fl.append(f"build/{f}.o")
				if (subprocess.run(["gcc","-fPIC","-shared","-fvisibility=hidden","-D","__LLL_LIB_COMPILATION__","-Wall","-lm","-Werror","-O3","-c","src/lll_lib/"+f,"-o",f"build/{f}.o","-Isrc/include"]).returncode!=0):
					sys.exit(1)
		if (subprocess.run(["gcc","-shared","-fvisibility=hidden","-D","__LLL_LIB_COMPILATION__","-Wall","-O3","-Werror","-o","build/lll_lib.so"]+fl+["-lm"]).returncode!=0 or subprocess.run(["gcc","-Wall","-lm","-Werror","-O3","-c","src/main.c","-o","build/main.o","-Ibuild"]).returncode!=0 or subprocess.run(["gcc","-o","build/lll","-O3","build/main.o","build/lll_lib.so","-lm"]).returncode!=0):
			sys.exit(1)
	else:
		fl=[]
		for f in os.listdir("src/lll_lib"):
			if (f[-2:]==".c"):
				fl.append(f"build/{f}.o")
				if (subprocess.run(["gcc","-fPIC","-shared","-fvisibility=hidden","-D","__LLL_LIB_COMPILATION__","-Wall","-lm","-Werror","-O0","-c","src/lll_lib/"+f,"-o",f"build/{f}.o","-Isrc/include"]).returncode!=0):
					sys.exit(1)
		if (subprocess.run(["gcc","-shared","-fvisibility=hidden","-D","__LLL_LIB_COMPILATION__","-Wall","-O0","-Werror","-o","build/lll_lib.so"]+fl+["-lm"]).returncode!=0 or subprocess.run(["gcc","-Wall","-lm","-Werror","-O0","-c","src/main.c","-o","build/main.o","-Ibuild"]).returncode!=0 or subprocess.run(["gcc","-o","build/lll","-O0","build/main.o","build/lll_lib.so","-lm"]).returncode!=0):
			sys.exit(1)
	if ("--run" in sys.argv):
		subprocess.run(["build/lll","-h"])
		if (("--generate-code" in sys.argv and (subprocess.run(["build/lll","example/test.lll","-v","-O0","-c","-R","-o","build/test.lllc","-p","-fp","-I","example","-m"]).returncode!=0 or subprocess.run(["build/lll","build/test.lllc","-v","-O3","-p","-fp","-L","-R"]).returncode!=0 or subprocess.run(["nasm","-f","elf64","-o","build/test.o","build/test.asm"]).returncode!=0 or subprocess.run(["gcc","build/test.o","build/lll_lib.so","-o","build/test","-O3"]).returncode!=0 or subprocess.run("build/test").returncode!=0)) or ("--generate-code" not in sys.argv and (subprocess.run(["build/lll","example/test.lll","-v","-O0","-c","-o","build/test.lllc","-p","-fp","-I","example","-m","-R"]).returncode!=0 or subprocess.run(["build/lll","build/test.lllc","-v","-O3","-p","-fp","-L","-G"]).returncode!=0))):
			sys.exit(1)
