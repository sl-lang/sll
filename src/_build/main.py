import datetime
import os
import re
import subprocess
import sys
import zipfile



COMMENT_REGEX=re.compile(br"\/\*.*?\*\/|\/\/.*?$",re.DOTALL|re.MULTILINE)
COMPILATION_DEFINES=([b"_MSC_VER",b"_WINDOWS",b"WINDLL",b"USERDLL",b"_UNICODE",b"UNICODE"]+([b"NDEBUG"] if "--release" in sys.argv else [b"_DEBUG",b"DEBUG_BUILD"]) if os.name=="nt" else ([] if "--release" in sys.argv else [b"DEBUG_BUILD"]))
DEFINE_LINE_CONTINUE_REGEX=re.compile(br"\\\n[ \t\r]*")
DEFINE_REMOVE_REGEX=re.compile(br"^[ \t\r]*(#define [a-zA-Z0-9_]+\([^\)]*\))[ \t\r]*(\\\n(?:[ \t\r]*.*\\\n)+[ \t\r]*.*\n?)",re.MULTILINE)
ESCAPE_CHARACTER_REGEX=re.compile(br"[^\x20-~]")
HEADER_INCLUDE_GUARD_REGEX=re.compile(br"^\s*#ifndef\s+(?P<h_nm>[a-zA-Z0-9_]+)\s+#define\s+(?P=h_nm)\s+(?:1\s+)?(.*)#endif\s*$",re.DOTALL)
HELP_FILE_PATH="rsrc/help.txt"
HEX_NUMBER_REGEX=re.compile(br"\b0x[0-9a-f]+\b")
IDENTIFIER_CHARACTERS=b"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"
IDENTIFIER_REGEX=re.compile(br"\b[a-zA-Z0-9_]+\b")
INCLUDE_REGEX=re.compile(br"""^\s*#\s*include\s*(<[^>]*>|\"[^>]*\")\s*$""",re.MULTILINE)
LETTERS=b"abcdefghijklmnopqrstuvwxyz"
MONTHS=["Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"]
MULTIPLE_NEWLINE_REGEX=re.compile(br"\n+")
NUMBERS=b"0123456789"
SPACE_CHARACTERS=b" \t\n\v\f\r"
SPACE_CHARACTERS_REGEX=re.compile(b" \t\n\v\f\r")



def _wrap_output(a,pfx=b"    "):
	p=subprocess.Popen(a,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
	st=False
	for c in iter(lambda:p.stdout.read(1),b""):
		if (not st):
			st=True
			sys.stdout.buffer.write(pfx)
		sys.stdout.buffer.write(c)
		if (c==b"\n"):
			st=False
			sys.stdout.buffer.flush()
	p.wait()
	return p



def _expand_macros(k,dm,dfm):
	i=0
	while (i<len(k)):
		if ((i==0 or k[i-1:i] not in IDENTIFIER_CHARACTERS) and k[i] not in NUMBERS):
			j=i
			while (i<len(k) and k[i:i+1] in IDENTIFIER_CHARACTERS):
				i+=1
			for e,v in dm.items():
				if (e==k[j:i]):
					k=k[:j]+v+k[i:]
					i=j-1
					break
			if (i!=j):
				for e,v in dfm.items():
					if (e==k[j:i]):
						if (k[i:i+1]!=b"("):
							continue
						i+=1
						l=i
						while (k[i:i+1]!=b")"):
							i+=1
						a=k[l:i].split(b".")
						if (len(a)!=len(v[0])):
							raise RuntimeError("Invalid Macro Argument Count")
						tmp=k[i+1:]
						k=k[:j]
						for m,n in enumerate(v[1]):
							if (m&1):
								k+=a[n]
							else:
								k+=n
						k+=tmp
						i=j-1
						break
		i+=1
	return k



def _generate_header(h_dt,c_m):
	l=[]
	st=[True]
	tm=datetime.datetime.now()
	dm={b"__TIME__":bytes(tm.strftime("\"%H:%M:%S\""),"utf-8"),b"__DATE__":bytes(tm.strftime(f"\"{MONTHS[tm.month-1]} %d %Y\""),"utf-8"),b"UINT16_MAX":b"65535",b"UINT32_MAX":b"4294967295u"}
	dfm={}
	d_v=[]
	d_f=[]
	il=b""
	for e in h_dt:
		e=e.strip()
		if (e[:1]==b"#"):
			f=e[1:].split(b" ")
			if (f[0]==b"ifdef"):
				if (f[1] in c_m or f[1] in dm):
					st.append(True)
				else:
					st.append(False)
				continue
			elif (f[0]==b"ifndef"):
				if (f[1] in c_m or f[1] in dm):
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
			elif (f[0]==b"define" and False not in st):
				if (b"(" in f[1]):
					al=f[1].split(b"(")[1].split(b")")[0].split(b",")
					if (al[-1]==b""):
						al.pop(len(al)-1)
					al=tuple(al)
					k=b" ".join(f[2:])
					if (f[1][:2]!=b"__"):
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
					dfm[f[1].split(b"(")[0]]=(al,tuple([(sl_e if j&1 else sl_e.replace(b"##",b"")) for j,sl_e in enumerate(sl)]))
				else:
					dm[f[1]]=b" ".join(f[2:])
					if (f[1][:2]!=b"__"):
						d_v.append((f[1],b" ".join(f[2:])))
				continue
		if (False not in st):
			l.append(e)
	d_s=b""
	o=b""
	for i,(k,v) in enumerate(sorted(d_v,key=lambda e:e[0])):
		v=HEX_NUMBER_REGEX.sub(lambda m:bytes(str(int(m.group(0),16)),"utf-8"),_expand_macros(v,dm,dfm))
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
		v=IDENTIFIER_REGEX.sub(lambda m:(a[m.group(0)] if m.group(0) in a else m.group(0)),HEX_NUMBER_REGEX.sub(lambda m:bytes(str(int(m.group(0),16)),"utf-8"),_expand_macros(v,dm,dfm)))
		d_f[i]=(nk,v)
		d_s+=b"\n#define "+nk+b" "+v.strip()
	fl=[]
	for k in l:
		k=_expand_macros(k,dm,dfm)
		if (len(k)==0):
			continue
		if (b"(" in k and b"(*" not in SPACE_CHARACTERS_REGEX.sub(b"",k) and k.count(b"(")==k.count(b")") and k.count(b"{")==k.count(b"}")):
			fl.append((k[:-len(k.split(b"(")[-1])-1].split(b" ")[-1],k.strip()))
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
		void=True
		for a in v[-ai:].split(b")")[0].split(b","):
			if (st is False):
				o+=b","
			st=False
			o+=a[:-len(a.split(b" ")[-1])].strip()
			if (len(a[:-len(a.split(b" ")[-1])].strip())>0):
				void=False
		if (void):
			o+=b"void"
		o+=b");"
	return il+d_s+b"\n"+o.strip()



vb=("--verbose" in sys.argv)
if (vb):
	print("Generating Build Directory...")
if (os.path.exists("build")):
	dl=[]
	for r,ndl,fl in os.walk("build"):
		r=r.replace("\\","/").rstrip("/")+"/"
		for d in ndl:
			dl.insert(0,r+d)
		for f in fl:
			os.remove(r+f)
	for k in dl:
		if (k!="build/lib"):
			os.rmdir(k)
else:
	os.mkdir("build")
if (not os.path.exists("build/lib")):
	os.mkdir("build/lib")
if (vb):
	print("Copying Modules...")
for f in os.listdir("src/sll/lib"):
	if (vb):
		print(f"  Copying Module 'src/sll/lib/{f}'...")
	with open(f"src/sll/lib/{f}","rb") as rf,open(f"build/lib/{f}","wb") as wf:
		wf.write(rf.read())
if (vb):
	print(f"Convering '{HELP_FILE_PATH}' to Header File...")
with open(HELP_FILE_PATH,"rb") as rf,open("build/help_text.h","wb") as wf:
	wf.write(b"#ifndef __HELP_TEXT_H__\n#define __HELP_TEXT_H__ 1\n#include <stdint.h>\nconst uint8_t HELP_TEXT[]={")
	st=True
	for c in rf.read().replace(b"\r\n",b"\n"):
		if (st is False):
			wf.write(b",")
		wf.write(bytearray([48,120,(48 if (c>>4)<10 else 87)+(c>>4),(48 if (c&0xf)<10 else 87)+(c&0xf)]))
		st=False
	wf.write(b",0x00};\n#endif")
if (vb):
	print("Combining Library Header Files...")
h_dt=b""
il=[]
for f in os.listdir("src/include/sll"):
	if (f[0]!="_" and f[-2:]==".h"):
		with open(f"src/include/sll/{f}","rb") as rf:
			il.append(bytes(f,"utf-8"))
			dt=rf.read()
			m=HEADER_INCLUDE_GUARD_REGEX.fullmatch(dt)
			if (m!=None):
				h_dt+=m.group(2)+b"\n"
			else:
				h_dt+=dt+b"\n"
if (vb):
	print(f"  Combined {len(il)} Files\nPreprocessing Combined Library Header File...")
h_dt=INCLUDE_REGEX.sub(lambda m:(b"" if m.group(1)[1:-1] in il else b"#include <"+(il.append(m.group(1)[1:-1]),m.group(1)[1:-1])[1]+b">"),DEFINE_REMOVE_REGEX.sub(lambda g:g.group(1)+b" "+DEFINE_LINE_CONTINUE_REGEX.sub(br"",g.group(2)),MULTIPLE_NEWLINE_REGEX.sub(br"\n",COMMENT_REGEX.sub(br"",h_dt).strip().replace(b"\r\n",b"\n")))).split(b"\n")
if (vb):
	print("Generating Library Header File...")
with open("build/sll_lib.h","wb") as wf:
	wf.write(b"#ifndef __SLL_LIB_H__\n#define __SLL_LIB_H__ 1"+_generate_header(h_dt,COMPILATION_DEFINES)+b"\n#endif\n")
i_fl=[]
o_fl=[]
cd=os.getcwd()
if (vb):
	print("Generating Executable and Library...")
if (os.name=="nt"):
	if (vb):
		print("  Listing Files...")
	for r,_,fl in os.walk("src/sll"):
		r=r.replace("\\","/").rstrip("/")+"/"
		for f in fl:
			if (f[-2:]==".c" and (r!="src/sll/platform/" or f=="windows.c")):
				i_fl.append("../"+r+f)
				o_fl.append(f[:-2]+".obj")
	if (vb):
		print(f"    Found {len(i_fl)} Files")
	os.chdir("build")
	if ("--release" in sys.argv):
		if (vb):
			print("  Compiling Library Files (Release Mode)...")
		if (_wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","WINDLL","/D","USERDLL","/D","_UNICODE","/D","UNICODE","/D","__SLL_LIB_COMPILATION__","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/FC","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/MD","/I","../src/include"]+i_fl).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
		if (vb):
			print("  Linking Library Files (Release Mode)...")
		if (_wrap_output(["link","/OUT:sll_lib.dll","/DLL","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:NOREF","/INCREMENTAL:NO"]+o_fl).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
		if (vb):
			print("  Compiling Files (Release Mode)...")
		if (_wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/FC","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/MD","/I",".","../src/main.c"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
		if (vb):
			print("  Linking Files (Release Mode)...")
		if (_wrap_output(["link","main.obj","/OUT:sll.exe","/DYNAMICBASE","sll_lib.lib","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
	else:
		if (vb):
			print("  Compiling Library Files...")
		if (_wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","WINDLL","/D","USERDLL","/D","_UNICODE","/D","UNICODE","/D","__SLL_LIB_COMPILATION__","/D","DEBUG_BUILD","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/FC","/EHsc","/nologo","/diagnostics:column","/ZI","/Od","/RTC1","/MDd","/I","../src/include"]+i_fl).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
		if (vb):
			print("  Linking Library Files...")
		if (_wrap_output(["link","/OUT:sll_lib.dll","/DLL","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL"]+o_fl).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
		if (vb):
			print("  Compiling Files...")
		if (_wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/D","DEBUG_BUILD","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/FC","/EHsc","/nologo","/diagnostics:column","/ZI","/Od","/RTC1","/MDd","/I",".","../src/main.c"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
		if (vb):
			print("  Linking Files...")
		if (_wrap_output(["link","main.obj","/OUT:sll.exe","/DYNAMICBASE","sll_lib.lib","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
	os.chdir(cd)
else:
	if (vb):
		print("  Listing Files...")
	i_fl=[]
	o_fl=[]
	for r,_,fl in os.walk("src/sll"):
		r=r.rstrip("/")+"/"
		for f in fl:
			if (f[-2:]==".c" and (r!="src/sll/platform/" or f=="posix.c")):
				i_fl.append("../"+r+f)
				o_fl.append(f[:-2]+".o")
	if (vb):
		print(f"    Found {len(i_fl)} Files")
	os.chdir("build")
	if ("--release" in sys.argv):
		if (vb):
			print("  Compiling Library Files (Release Mode)...")
		if (_wrap_output(["gcc","-fPIC","-c","-fvisibility=hidden","-D","__SLL_LIB_COMPILATION__","-Wall","-O3","-Werror","-I","../src/include"]+i_fl+["-lm"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
		if (vb):
			print("  Linking Library Files (Release Mode)...")
		if (_wrap_output(["gcc","-shared","-fPIC","-fvisibility=hidden","-Wall","-O3","-Werror","-o","sll_lib.so"]+o_fl+["-lm"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
		if (vb):
			print("  Compiling & Linking Files (Release Mode)...")
		if (_wrap_output(["gcc","-Wall","-lm","-Werror","-O3","../src/main.c","sll_lib.so","-o","sll","-I","."]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
	else:
		if (vb):
			print("  Compiling Library Files...")
		if (subprocess.run(["gcc","-fPIC","-c","-fvisibility=hidden","-D","__SLL_LIB_COMPILATION__","-Wall","-O0","-Werror","-I","../src/include"]+i_fl+["-lm"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
		if (vb):
			print("  Linking Library Files...")
		if (subprocess.run(["gcc","-shared","-fPIC","-fvisibility=hidden","-Wall","-O0","-Werror","-o","sll_lib.so"]+o_fl+["-lm"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
		if (vb):
			print("  Compiling & Linking Files...")
		if (subprocess.run(["gcc","-Wall","-lm","-Werror","-O0","../src/main.c","sll_lib.so","-o","sll","-I","."]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
	os.chdir(cd)
if (os.name!="nt"):
	if (vb):
		print("Fixing 'LD_LIBRARY_PATH'...")
	ld=os.getenv("LD_LIBRARY_PATH","")
	if ("build:" not in ld):
		os.environ["LD_LIBRARY_PATH"]="build:"+ld
if (vb):
	print("Compiling Modules...")
fl=list(os.listdir("build/lib"))
if (_wrap_output(["build/sll","-c",("-O3" if "--release" in sys.argv else "-O1"),"-L","-e","-R"]+["build/lib/"+e for e in fl]+(["-v"] if vb else []),pfx=b"  ").returncode!=0):
	sys.exit(1)
if (vb):
	print("Removing Module Source Files...")
for k in fl:
	if (vb):
		print(f"  Removing 'build/lib/{k}'...")
	os.remove(f"build/lib/{k}")
e_nm="build/sll"
if ("--standalone" in sys.argv):
	if (vb):
		print("Generating Standalone Executable...")
	e_nm="build/sll_standalone"
	if (vb):
		print("  Generating Standalone Library Header File...")
	with open("build/sll_lib_standalone.h","wb") as wf:
		wf.write(b"#ifndef __SLL_LIB_STANDALONE_BUILD_H__\n#define __SLL_LIB_STANDALONE_BUILD_H__ 1"+_generate_header(h_dt,COMPILATION_DEFINES+[b"__SLL_LIB_STATIC__"])+b"\n#endif\n")
	if (vb):
		print(f"  Converting Modules to Header File ({len(fl)} Modules)...")
	with open("build/compiled_modules.h","wb") as f:
		f.write(bytes(f"#ifndef __COMPILED_MODULES_H__\n#define __COMPILED_MODULES_H__\n#include <stdint.h>\n#define COMPILED_MODULE_COUNT {len(fl)}\ntypedef struct __MODULE{{const char* nm;uint32_t nml;uint8_t c;uint32_t sz;const uint8_t* dt;}} module_t;\n","utf-8"))
		m_dt=[]
		for i,e in enumerate(fl):
			f.write(bytes(f"const uint8_t _m{i}[]={{","utf-8"))
			with open(f"build/lib/{e}.sllc","rb") as rf:
				dt=rf.read()
				if (vb):
					print(f"    Converting Module 'build/lib/{e}.sllc' ({len(dt)} bytes)")
				c=0
				for k in bytes(e,"utf-8"):
					c^=k
				m_dt.append(bytes(f"{{\"{e.replace(chr(34),chr(92)+chr(34))}\",{len(e)},{c},{len(dt)},_m{i}}}","utf-8"))
				st=False
				for c in dt:
					f.write((b"," if st else b"")+bytearray([48,120,(48 if (c>>4)<10 else 87)+(c>>4),(48 if (c&0xf)<10 else 87)+(c&0xf)]))
					st=True
			f.write(b"};\n")
		f.write(b"const module_t m_dt[]={"+b",".join(m_dt)+b"};\n#endif")
	os.chdir("build")
	if (os.name=="nt"):
		if ("--release" in sys.argv):
			if (vb):
				print("  Compiling Files (Release Mode)...")
			if (_wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/D","_CRT_SECURE_NO_WARNINGS","/D","__SLL_LIB_STATIC__","/D","STANDALONE_BUILD","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/FC","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/MD","/I",".","../src/main.c"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Linking Files (Release Mode)...")
			if (_wrap_output(["link","main.obj","/OUT:sll_standalone.exe","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO"]+o_fl).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			if (vb):
				print("  Compiling Files...")
			if (_wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/D","DEBUG_BUILD","/D","_CRT_SECURE_NO_WARNINGS","/D","__SLL_LIB_STATIC__","/D","STANDALONE_BUILD","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/FC","/EHsc","/nologo","/diagnostics:column","/ZI","/Od","/RTC1","/MDd","/I",".","../src/main.c"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
			if (vb):
				print("  Linking Files...")
			if (_wrap_output(["link","main.obj","/OUT:sll_standalone.exe","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL"]+o_fl).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	else:
		if ("--release" in sys.argv):
			if (vb):
				print("  Compiling & Linking Files (Release Mode)...")
			if (_wrap_output(["gcc","-D","__SLL_LIB_STATIC__","-D","STANDALONE_BUILD","-Wall","-lm","-Werror","-O3","../src/main.c","-o","sll_standalone","-I",".","-I","../src/include"]+i_fl+["-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
		else:
			if (vb):
				print("  Compiling & Linking Files...")
			if (_wrap_output(["gcc","-D","__SLL_LIB_STATIC__","-D","STANDALONE_BUILD","-D","DEBUG_BUILD","-Wall","-lm","-Werror","-O0","../src/main.c","-o","sll_standalone","-I",".","-I","../src/include"]+i_fl+["-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	os.chdir(cd)
if ("--bundle" in sys.argv):
	with zipfile.ZipFile("build/sll.zip","w") as zf:
		for k in (["build/sll.exe","build/sll_lib.dll"] if os.name=="nt" else ["build/sll","build/sll_lib.so"]):
			zf.write(k,arcname=k[6:])
		for k in os.listdir("build/lib"):
			zf.write("build/lib/"+k,arcname="lib/"+k)
if ("--test" in sys.argv):
	os.chdir("build")
	td=os.path.abspath("../tests/").replace("\\","/").rstrip("/")+"/"
	if (os.name=="nt"):
		if (vb):
			print("  Compiling Test Program...")
		if (_wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/D","_CRT_SECURE_NO_WARNINGS","/D","__TEST_ROOT_DIR__=\""+td+"\"","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/FC","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/MD","/I",".","../tests/run_tests.c"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
		if (vb):
			print("  Linking Test Program...")
		if (_wrap_output(["link","run_tests.obj","/OUT:run_tests.exe","/DYNAMICBASE","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/LTCG","/OPT:REF","/INCREMENTAL:NO"]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
	else:
		if (vb):
			print("  Compiling & Linking Files...")
		if (_wrap_output(["gcc","-D","__TEST_ROOT_DIR__=\""+td+"\"","-Wall","-lm","-Werror","-O3","../tests/run_tests.c","-o","run_tests","-I","."]).returncode!=0):
			os.chdir(cd)
			sys.exit(1)
	os.chdir(cd)
if ("--run" in sys.argv):
	if (vb):
		print("Running 'example/test.sll'...")
	subprocess.run([e_nm,"-h"])
	if (subprocess.run([e_nm,"example/test.sll","-v","-O3","-c","-o","build/test","-e","-I","example","-R"]).returncode!=0 or subprocess.run([e_nm,"build/test.sllc","-v","-O0","-p","-P","-e","-L","-a","-c","-o","build/test2","-R"]).returncode!=0 or subprocess.run([e_nm,"build/test2.slla","-v","-P","-L"]).returncode!=0):
		sys.exit(1)
