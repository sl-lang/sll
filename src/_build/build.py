import datetime
import header
import os
import re
import sys
import util



CHAR_BACKSLASH=92
CHAR_COMMA=44
CHAR_DOUBLE_QUOTE=34
CHAR_LEFT_PARENTHESIS=40
CHAR_RIGHT_PARENTHESIS=41
CHAR_SINGLE_QUOTE=39
DEFINE_REGEX=re.compile(br"^[ \t]*#[ \t]*define[ \t]+([a-zA-Z0-9_]+)",re.M)
IDENTIFIER_CHARACTERS=b"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"
MONTHS=["Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"]
SPACE_CHARACTERS_REGEX=re.compile(b" \t\n\v\f\r")
START_IDENTIFIER_CHARACTERS=b"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"



def _find_line(dt,i):
	while (dt[i:i+1] in b" \t"):
		i+=1
	j=i
	while (dt[j:j+1]!=b"\n"):
		j+=1
	return dt[i:j]



def _preprocess_fragment(dt,ls,le,dm,dfm):
	c_m=[{} for _ in range(0,256)]
	for k,v in dm.items():
		c=0
		for e in k:
			c^=e
		c_m[c][k]=(False,v)
	for k,v in dfm.items():
		c=0
		for e in k:
			c^=e
		c_m[c][k]=(True,*v)
	li=ls
	while (li<le):
		k=dt[li]
		i=0
		while (i<len(k)):
			if (k[i]==CHAR_SINGLE_QUOTE):
				i+=1
				while (k[i]!=CHAR_SINGLE_QUOTE):
					if (k[i]==CHAR_BACKSLASH):
						i+=1
					i+=1
			elif (k[i]==CHAR_DOUBLE_QUOTE):
				i+=1
				while (k[i]!=CHAR_DOUBLE_QUOTE):
					if (k[i]==CHAR_BACKSLASH):
						i+=1
					i+=1
			elif (k[i] in START_IDENTIFIER_CHARACTERS):
				j=i
				c=0
				while (i<len(k) and k[i] in IDENTIFIER_CHARACTERS):
					c^=k[i]
					i+=1
				nm=k[j:i]
				if (nm==b"__LINE__"):
					k=k[:j]+bytes(str(li-ls+1),"utf-8")+k[i:]
					i=j-1
				else:
					for e,v in c_m[c].items():
						if (e==nm):
							if (v[0]):
								i+=1
								l=i
								b=1
								a=[]
								si=i
								while (True):
									if (k[i]==CHAR_SINGLE_QUOTE):
										i+=1
										while (k[i]!=CHAR_SINGLE_QUOTE):
											if (k[i]==CHAR_BACKSLASH):
												i+=1
											i+=1
									elif (k[i]==CHAR_DOUBLE_QUOTE):
										i+=1
										while (k[i]!=CHAR_DOUBLE_QUOTE):
											if (k[i]==CHAR_BACKSLASH):
												i+=1
											i+=1
									elif (k[i]==CHAR_LEFT_PARENTHESIS):
										b+=1
									elif (k[i]==CHAR_RIGHT_PARENTHESIS):
										b-=1
										if (b==0):
											break
									elif (b==1 and k[i]==CHAR_COMMA):
										a.append(k[si:i])
										i+=1
										si=i
										continue
									i+=1
								a.append(k[si:i])
								if (len(a)==1 and len(a[0])==0):
									a=tuple()
								else:
									a=tuple(a)
								va=b""
								if (len(a)!=len(v[1])):
									if (len(a)>len(v[1]) and not v[2]):
										raise RuntimeError("Invalid Macro Argument Count")
									va=b",".join(a[len(v[1]):])
								tmp=k[i+1:]
								k=k[:j]
								for m,n in enumerate(v[3]):
									if (m&1):
										if (n==0):
											k+=va
										elif (n>0):
											k+=a[n-1]
										else:
											k+=a[~n].replace(b"\\",b"\\\\").replace(b"\"",b"\\\"")
									else:
										k+=n
								k+=tmp
							else:
								k=k[:j]+v[1]+k[i:]
							i=j-1
							break
			i+=1
		if (b"##" in k):
			dt[li]=k.replace(b"##",b"")
			continue
		dt[li]=k
		li+=1



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



def convert_to_coverage(i_fp,o_fp,inc_fp,cm,vb):
	with open(i_fp,"rb") as f:
		dt=[b"#include <sll/__coverage.h>"]+f.read().replace(b"\r\n",b"\n").split(b"\n")
	o_dt=b""
	inc=[]
	st=[1]
	tm=datetime.datetime.now()
	dm={b"__TIME__":bytes(tm.strftime("\"%H:%M:%S\""),"utf-8"),b"__DATE__":bytes(tm.strftime(f"\"{MONTHS[tm.month-1]} %d %Y\""),"utf-8"),b"UINT8_MAX":b"255",b"UINT16_MAX":b"65535",b"UINT32_MAX":b"4294967295u",b"UINT64_MAX":b"18446744073709551615ull",b"__FILE_ID__":b"-1"}
	for k in cm:
		dm[k]=b""
	dfm={}
	i=0
	li=0
	while (i<len(dt)):
		k=dt[i].strip()
		if (k[:1]==b"#"):
			j=1
			while (k[j:j+1] in b" \t"):
				j+=1
			if (k[j:j+4]==b"else"):
				st[-1]=(1-st[-1] if st[-1]<2 else 2)
				dt[i]=b""
			elif (k[j:j+5]==b"endif"):
				st.pop()
				dt[i]=b""
			elif (st[-1]==1):
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
						if (e==b"windows.h"):
							o_dt=b"#define WIN32_LEAN_AND_MEAN 1\n#include <windows.h>\n"
						elif (e[:4]==b"sll/"):
							with open(inc_fp+str(e,"utf-8"),"rb") as f:
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
									sl.append(0)
									sl.append(b"")
									sli+=2
									l+=11
									continue
								nxt=False
								for si,se in enumerate(al):
									if (m_dt.startswith(se,l) and (l+len(se)==len(m_dt) or m_dt[l+len(se):l+len(se)+1] not in IDENTIFIER_CHARACTERS)):
										sl.append(si+1)
										sl.append(b"")
										sli+=2
										l+=len(se)
										nxt=True
										break
								if (nxt):
									continue
							sl[sli]+=m_dt[l:l+1]
							l+=1
						for j,se in enumerate(sl):
							if ((j&1)==0 and se[-1:]==b"#" and se[-2:-1]!=b"#"):
								sl[j]=se[:-1]+b"\""
								sl[j+1]=-sl[j+1]
								sl[j+2]=b"\""+sl[j+2]
						dfm[e]=(al,va,tuple(sl))
					else:
						dm[e]=k[j:].lstrip()
						while (dm[e][-1:]==b"\\"):
							dt[i]=b""
							i+=1
							dm[e]=dm[e][:-1].rstrip()+b" "+dt[i].strip()
					dt[i]=b""
				elif (k[j:j+5]==b"undef"):
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
						st.append((1 if t==b"ifdef" else 0))
					else:
						st.append((0 if t==b"ifdef" else 1))
					dt[i]=b""
				elif (k[j:j+4]==b"line"):
					_preprocess_fragment(dt,li,i,dm,dfm)
					li=i+1
			else:
				if (k[j:j+5]==b"ifdef" or k[j:j+6]==b"ifndef"):
					st.append(2)
				dt[i]=b""
		elif (st[-1]!=1):
			dt[i]=b""
		i+=1
	_preprocess_fragment(dt,li,i,dm,dfm)
	dt=b"\n".join(dt)+b"\n"
	cov_dt=[]
	cov_s_dt=[bytes(o_fp,"utf-8")]
	i=0
	ln=1
	c_fp_id=0
	f_id=-1
	ln_off=0
	while (i<len(dt)):
		while (dt[i:i+1] in b" \t\r\n\f\v"):
			if (dt[i:i+1]==b"\n"):
				ln+=1
				ln_off=i+1
			i+=1
		while (dt[i:i+1]==b"#"):
			i+=1
			while (dt[i:i+1] in b" \t\r\n\f\v"):
				if (dt[i:i+1]==b"\n"):
					ln+=1
					ln_off=i+1
				i+=1
			j=i
			while (i!=len(dt) and dt[i:i+1] not in b" \t\r\n\f\v"):
				i+=1
			if (dt[j:i]==b"line"):
				while (dt[i:i+1] in b" \t\r\n\f\v"):
					if (dt[i:i+1]==b"\n"):
						ln+=1
						ln_off=i+1
					i+=1
				j=i
				while (i!=len(dt) and dt[i:i+1] not in b" \t\r\n\f\v"):
					i+=1
				ln=int(dt[j:i])-1
				while (dt[i:i+1] in b" \t\r\n\f\v"):
					if (dt[i:i+1]==b"\n"):
						ln+=1
						ln_off=i+1
					i+=1
				j=i
				while (i!=len(dt) and dt[i:i+1] not in b" \t\r\n\f\v"):
					i+=1
				cov_s_dt.append(dt[j:i][1:-1])
				c_fp_id=len(cov_s_dt)-1
				dm[b"__FILE_ID__"]=bytes(str(f_id),"utf-8")
				f_id+=1
			while (dt[i:i+1]!=b"\n"):
				i+=1
			while (i!=len(dt) and dt[i:i+1] in b" \t\r\n\f\v"):
				if (dt[i:i+1]==b"\n"):
					ln+=1
					ln_off=i+1
				i+=1
		if (i==len(dt)):
			break
		b=0
		j=i
		nm_id=-1
		st=[None]
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
			elif (dt[i:i+1]==b"\n"):
				ln+=1
				ln_off=i+1
			i+=1
		else:
			i+=1
			while (dt[i:i+1] in b" \t\r\n\f\v"):
				if (dt[i:i+1]==b"\n"):
					ln+=1
					ln_off=i+1
				i+=1
			b=0
			if (dt[i:i+1]==b"{"):
				b=1
				i+=1
				e=SPACE_CHARACTERS_REGEX.sub(b" ",dt[j:i+1])
				if (b"static" not in e):
					cov_s_dt.append(e.split(b"(")[0].split(b" ")[-1])
					nm_id=len(cov_s_dt)-1
					cov_dt.append([c_fp_id,nm_id,ln,-1])
					st[0]=cov_dt[-1]
					s=bytes(f"__coverage_map[{(len(cov_dt)-1)>>6}]|={1<<((len(cov_dt)-1)&63)}ull;","utf-8")
					dt=dt[:i]+s+dt[i:]
					i+=len(s)
		if (i==len(dt)):
			break
		rt=False
		sw=False
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
			elif (dt[i:i+1]==b"("):
				b+=1
			elif (dt[i:i+1]==b"{"):
				if (nm_id!=-1):
					if (st[-1] is None or sw is True):
						st.append(None)
					else:
						j=i-1
						while (dt[j:j+1] in b" \t\r\n\f\v"):
							j-=1
						if (dt[j:j+1]!=b"="):
							cov_dt.append([c_fp_id,nm_id,ln,-1])
							st.append(cov_dt[-1])
							s=bytes(f"__coverage_map[{(len(cov_dt)-1)>>6}]|={1<<((len(cov_dt)-1)&63)}ull;","utf-8")
							dt=dt[:i+1]+s+dt[i+1:]
							i+=len(s)
						else:
							st.append(None)
				sw=False
				b+=1
			elif (dt[i:i+1]==b")"):
				b-=1
			elif (dt[i:i+1]==b"}"):
				if (nm_id!=-1):
					e=st.pop()
					if (e is not None):
						e[3]=ln
				b-=1
			elif (dt[i:i+1]==b";"):
				sw=False
			elif (dt[i:i+1]==b"\n"):
				ln+=1
				ln_off=i+1
			elif (nm_id!=-1):
				if (dt[i-1:i] not in IDENTIFIER_CHARACTERS):
					if (dt[i:i+6]==b"switch" and dt[i+6:i+7] not in IDENTIFIER_CHARACTERS):
						sw=True
			i+=1
		while (dt[i:i+1] in b" \t\r\n\f\v"):
			if (dt[i:i+1]==b"\n"):
				ln+=1
				ln_off=i+1
			i+=1
			if (i==len(dt)):
				break
		if (i==len(dt)):
			break
		if (dt[i:i+1]!=b"#"):
			i+=1
	with open(o_fp,"wb") as f:
		f.write(o_dt)
		f.write(dt)
		f.write(bytes(f"uint32_t __coverage_count={len(cov_dt)};\nuint32_t __coverage_map_length={(len(cov_dt)>>6)+1};\nuint64_t __coverage_map[{(len(cov_dt)>>6)+1}]={{0{',0'*(len(cov_dt)>>6)}}};\n__coverage_data_t __coverage_data[{len(cov_dt)}]={{","utf-8"))
		st=False
		for k in cov_dt:
			if (st):
				f.write(b",")
			st=True
			f.write(bytes(f"{{{k[0]},{k[1]},{k[2]},{k[3]}}}","utf-8"))
		f.write(bytes(f"}};\nconst char* __coverage_strings[{len(cov_s_dt)}]={{","utf-8"))
		st=False
		for k in cov_s_dt:
			if (st):
				f.write(b",")
			st=True
			f.write(b"\""+k.replace(b"\\",b"\\\\").replace(b"\"",b"\\\"")+b"\"")
		f.write(b"};\n")



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
	src_fp=os.path.abspath("build/sll.c").replace("\\","/")
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
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","NDEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/D","__SLL_STATIC__","/D","__TEST_ROOT_DIR__=\""+t_fp+"\"","/D","__SOURCE_FILE_PATH__=\""+src_fp+"\"","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/GL","/Gy","/Zi","/O2","/MD","/I",".","/I","../tests","/I","../src/include","/Forun_tests.obj","../tests/run_tests.c"]).returncode!=0):
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
			if (util.wrap_output(["cl","/c","/permissive-","/Zc:preprocessor","/std:c11","/Wv:18","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/D","DEBUG_BUILD","/D","__SLL_STATIC__","/D","__TEST_ROOT_DIR__=\""+t_fp+"\"","/D","__SOURCE_FILE_PATH__=\""+src_fp+"\"","/D","_CRT_SECURE_NO_WARNINGS","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/EHsc","/nologo","/diagnostics:column","/Zi","/Od","/RTC1","/MDd","/I",".","/I","../tests","/I","../src/include","/Forun_tests.obj","../tests/run_tests.c"]).returncode!=0):
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
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-D","__TEST_ROOT_DIR__=\""+t_fp+"\"","-D","__SOURCE_FILE_PATH__=\""+src_fp+"\"","-Wall","-Werror","-O3","../tests/run_tests.c","sll_coverage.o","-o","run_tests","-I",".","-I","../tests","-I","../src/include","-lm"]).returncode!=0):
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
			if (util.wrap_output(["gcc","-fdiagnostics-color=always","-D","__TEST_ROOT_DIR__=\""+t_fp+"\"","-D","__SOURCE_FILE_PATH__=\""+src_fp+"\"","-Wall","-Werror","-O0","../tests/run_tests.c","sll_coverage.o","-o","run_tests","-I",".","-I","../tests","-I","../src/include","-lm"]).returncode!=0):
				os.chdir(cd)
				sys.exit(1)
	os.chdir(cd)
