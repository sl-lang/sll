import os
import re
import sys
import util



COMMENT_REGEX=re.compile(r"\/\*.*?\*\/|\/\/.*?$",re.DOTALL|re.MULTILINE)
DEFINE_LINE_CONTINUE_REGEX=re.compile(r"\s*\\\n[ \t\r]*")
DEFINE_REMOVE_REGEX=re.compile(r"^[ \t\r]*(#define [a-zA-Z0-9_]+\([^\)]*\))[ \t\r]*(\\\n(?:[ \t\r]*.*\\\n)+[ \t\r]*.*\n?)",re.MULTILINE)
HEX_NUMBER_REGEX=re.compile(r"\b(0x[0-9a-f]+)(ul{0,2})?\b")
IDENTIFIER_CHARACTERS="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"
IDENTIFIER_REGEX=re.compile(r"\b[a-zA-Z_][a-zA-Z0-9_]*\b")
INCLUDE_REGEX=re.compile(r"""^\s*#\s*include\s*(<[^>]*>|\"[^\"]*\")\s*$""",re.MULTILINE)
INTERNAL_SLL_HEADERS=["assembly_optimizer.h","help_text.h","_dispatcher.h","_sll_internal.h","operator_parser.h","critical_error.h","memory_fail.h"]
LETTERS="abcdefghijklmnopqrstuvwxyz"
MULTIPLE_NEWLINE_REGEX=re.compile(r"\n+")
NUMBERS="0123456789"
SPACE_CHARACTERS=" \t\n\v\f\r"
SPACE_CHARACTERS_REGEX=re.compile(" \t\n\v\f\r")



def _expand_macros(k,dm,dfm,j_exp):
	i=0
	while (i<len(k)):
		if ((i==0 or k[i-1] not in IDENTIFIER_CHARACTERS) and k[i] not in NUMBERS):
			j=i
			while (i<len(k) and k[i] in IDENTIFIER_CHARACTERS):
				i+=1
			if (k[j]!="_" or k[j+1]=="_"):
				for e,v in dm.items():
					if (e==k[j:i]):
						k=k[:j]+v+k[i:]
						i=j-1
						break
				if (i!=j-1):
					for e,v in dfm.items():
						if (e==k[j:i]):
							if (k[i]!="("):
								continue
							i+=1
							b=1
							a=[""]
							ai=0
							while (True):
								si=i
								if (k[i]=="\'"):
									i+=1
									while (k[i]!="\'"):
										if (k[i]=="\\"):
											i+=1
										i+=1
								elif (k[i]=="\""):
									i+=1
									while (k[i]!="\""):
										if (k[i]=="\\"):
											i+=1
										i+=1
								elif (k[i]=="("):
									b+=1
								elif (k[i]==")"):
									b-=1
									if (b==0):
										break
								elif (b==1 and k[i]==","):
									a.append("")
									ai+=1
									i+=1
									continue
								i+=1
								a[ai]+=k[si:i]
							if (ai==0 and len(a[0])==0):
								a=[]
							va=""
							if (len(a)!=len(v[0])):
								if (len(a)>len(v[0]) and not v[1]):
									raise RuntimeError("Invalid Macro Argument Count")
								va=",".join(a[len(v[0]):])
							tmp=k[i+1:]
							k=k[:j]
							for m,n in enumerate(v[2]):
								if (m&1):
									k+=(va if n==-1 else a[n])
								else:
									k+=n
							k+=tmp
							i=j-1
							break
		i+=1
	if (j_exp and "##" in k):
		return _expand_macros(k.replace("##",""),dm,dfm,j_exp)
	return k



def _merge_strings(k):
	i=0
	while (i<len(k)):
		if (k[i]=="\'"):
			i+=1
			while (k[i]!="\'"):
				if (k[i]=="\\"):
					i+=1
				i+=1
		elif (k[i]=="\""):
			i+=1
			while (True):
				while (i<len(k) and k[i]!="\""):
					if (k[i]=="\\"):
						i+=1
					i+=1
				j=i
				i+=1
				while (i<len(k) and k[i] in SPACE_CHARACTERS):
					i+=1
				if (i==len(k) or k[i]!="\""):
					break
				k=k[:j]+k[i+1:]
		i+=1
	return k



def _write_byte_array(wf,dt):
	st=True
	i=0
	for c in dt:
		if (st is False):
			wf.write(",")
			if (i==8):
				wf.write("\n\t")
				i=0
		wf.write(f"0x{c:02x}")
		i+=1
		st=False



def read_version(fp):
	o=[0,0,0]
	with open(fp,"r") as f:
		for k in f.read().replace("\r\n","\n").split("\n"):
			k=k.strip()
			if (k[:1]=="#"):
				k=k[1:].lstrip().split(" ")
				if (k[0]=="define"):
					if (k[1]=="SLL_VERSION_MAJOR"):
						o[0]=int(k[2])
					elif (k[1]=="SLL_VERSION_MINOR"):
						o[1]=int(k[2])
					elif (k[1]=="SLL_VERSION_PATCH"):
						o[2]=int(k[2])
	return tuple(o)



def generate_help(i_fp,o_fp):
	util.log(f"Convering '{i_fp}' to '{o_fp}' ...")
	with open(i_fp,"rb") as rf,open(o_fp,"w") as wf:
		dt=rf.read().replace(b"\r\n",b"\n")
		wf.write(f"#ifndef __SLL_GENERATED_HELP_TEXT_H__\n#define __SLL_GENERATED_HELP_TEXT_H__ 1\n#include <sll/types.h>\n\n\n\n#define HELP_TEXT_SIZE {len(dt)}\n\n\n\nstatic const sll_char_t HELP_TEXT[]={{\n\t")
		_write_byte_array(wf,dt)
		wf.write("\n};\n\n\n\n#endif\n")



def generate_error_header(i_fp,o_fp,nm):
	util.log(f"Convering '{i_fp}' to '{o_fp}' ...")
	with open(i_fp,"rb") as rf,open(o_fp,"w") as wf:
		dt=rf.read().replace(b"\r\n",b"\n").split(b"$$$")
		wf.write(f"#ifndef __SLL_GENERATED_{nm}_H__\n#define __SLL_GENERATED_{nm}_H__ 1\n#include <sll/types.h>\n\n\n\n#define {nm}_START_SIZE {len(dt[0])}\n#define {nm}_END_SIZE {len(dt[1])}\n\n\n\nstatic const sll_char_t {nm}_START[]={{\n\t")
		_write_byte_array(wf,dt[0])
		wf.write(f"\n}};\n\n\n\nstatic const sll_char_t {nm}_END[]={{\n\t")
		_write_byte_array(wf,dt[1])
		wf.write("\n};\n\n\n\n#endif\n")



def parse_headers(fp):
	util.log("Combining Library Header Files...")
	o=""
	il=[]
	fp=fp.replace("\\","/").rstrip("/")+"/"
	for r,_,fl in os.walk(fp):
		if ("internal" in r):
			continue
		for f in fl:
			if (f[-2:]==".h" and f not in INTERNAL_SLL_HEADERS):
				with open(os.path.join(r,f),"r") as rf:
					il.append(os.path.join(r,f)[len(fp):].replace("\\","/"))
					o+=rf.read()+"\n"
	util.log(f"  Combined {len(il)} Files\nPreprocessing Combined Library Header File...")
	o=INCLUDE_REGEX.sub(lambda m:("" if m.group(1)[1:-1] in il else "#include <"+(il.append(m.group(1)[1:-1]),m.group(1)[1:-1])[1]+">"),DEFINE_REMOVE_REGEX.sub(lambda g:g.group(1)+" "+DEFINE_LINE_CONTINUE_REGEX.sub(r"",g.group(2)),MULTIPLE_NEWLINE_REGEX.sub(r"\n",COMMENT_REGEX.sub(r"",o).strip().replace("\r\n","\n")))).split("\n")
	return o



def generate_header(h_dt):
	l=[]
	st=[True]
	dm={"__SLL_TIME_RAW__":str(util.BUILD_TIME),f"__SLL_BUILD_{util.system.upper()}":"1"}
	if (os.getenv("GITHUB_SHA") is not None):
		dm["__SLL_SHA__"]="\""+os.getenv("GITHUB_SHA")[:7]+"\""
		dm["__SLL_FULL_SHA__"]="\""+os.getenv("GITHUB_SHA")+"\""
	if ("--release" not in sys.argv):
		dm["DEBUG_BUILD"]="1"
	dfm={}
	d_v=[]
	d_f=[]
	il=""
	for e in h_dt:
		e=e.strip()
		if (e[:1]=="#"):
			f=e[1:].split(" ")
			if (f[0]=="ifdef"):
				if (f[1] in dm):
					st.append(True)
				else:
					st.append(False)
				continue
			if (f[0]=="ifndef"):
				if (f[1] in dm):
					st.append(False)
				else:
					st.append(True)
				continue
			if (f[0]=="else"):
				st[-1]=not st[-1]
				continue
			if (f[0]=="endif"):
				st.pop()
				continue
			if (f[0]=="include" and False not in st):
				f_nm="src/include/"+" ".join(f[1:]).strip("\"").strip("<").strip(">")
				if (not os.path.exists(f_nm)):
					il+="\n#include "+" ".join(f[1:])
				continue
			if (f[0]=="define" and False not in st):
				if ("(" in f[1]):
					al=f[1].split("(")[1].split(")")[0].split(",")
					if (al[-1]==""):
						al.pop(len(al)-1)
					va=False
					if (len(al)>0 and al[-1]=="..."):
						va=True
						al.pop()
					if (len(al)>0 and len(al[0])==0):
						al=tuple()
					else:
						al=tuple(al)
					k=" ".join(f[2:])
					if (f[1][:4]=="SLL_" or f[1][:5]=="_SLL_"):
						d_f.append((f[1],k))
					sl=[""]
					sli=0
					i=0
					while (i<len(k)):
						if ((i==0 or k[i-1:i] not in IDENTIFIER_CHARACTERS) and k[i] in IDENTIFIER_CHARACTERS):
							if (k.startswith("__VA_ARGS__",i) and (i+11==len(k) or k[i+11] not in IDENTIFIER_CHARACTERS)):
								sl.append(-1)
								sl.append("")
								sli+=2
								i+=11
								continue
							nxt=False
							for j,se in enumerate(al):
								if (k.startswith(se,i) and (i+len(se)==len(k) or k[i+len(se)] not in IDENTIFIER_CHARACTERS)):
									sl.append(j)
									sl.append("")
									sli+=2
									i+=len(se)
									nxt=True
									break
							if (nxt):
								continue
						sl[sli]+=k[i]
						i+=1
					for i in range(0,len(sl),2):
						k=sl[i]
						if (k and k[-1]=="#" and (len(k)==1 or k[-2]!="#")):
							sl[i]=k[:-1]+"\""
							sl[i+2]="\""+sl[i+2]
					dfm[f[1].split("(")[0]]=(al,va,tuple(sl))
				else:
					dm[f[1]]=" ".join(f[2:])
					if (f[1][:4]=="SLL_" or f[1][:5]=="_SLL_"):
						d_v.append((f[1]," ".join(f[2:])))
				continue
		if (False not in st):
			l.append(e)
	d_s=""
	o=""
	for i,(k,v) in enumerate(sorted(d_v,key=lambda e:e[0])):
		v=HEX_NUMBER_REGEX.sub(lambda m:str(int(m.group(1),16))+(m.group(2) or ""),_expand_macros(v,dm,dfm,True))
		d_v[i]=(k,v)
		d_s+="\n#define "+k+" "+_merge_strings(v).strip()
	for i,(k,v) in enumerate(sorted(d_f,key=lambda e:e[0])):
		k=SPACE_CHARACTERS_REGEX.sub("",k)
		a={}
		j=[0]
		nk=k.split("(")[0]+"("
		args=k[k.index("(")+1:-1].strip()
		if (len(args)>0):
			for e in args.split(","):
				if (len(j)>1 or j[0]!=0):
					nk+=","
				if (e=="..."):
					nk+="..."
				else:
					a[e]="".join([LETTERS[m] for m in j])
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
		nk+=")"
		v=IDENTIFIER_REGEX.sub(lambda m:(a[m.group(0)] if m.group(0) in a else m.group(0)),HEX_NUMBER_REGEX.sub(lambda m:str(int(m.group(1),16))+(m.group(2) or ""),_expand_macros(v,dm,dfm,False))).strip()
		d_f[i]=(nk,v)
		d_s+="\n#define "+nk+" "+_merge_strings(v).strip()
	fl=[]
	e_v=[]
	for k in l:
		k=_expand_macros(k,dm,dfm,True).strip()
		if (len(k)==0):
			continue
		if ("extern" in k):
			e_v.append((k.replace("\t"," ").split(" ")[-1].split(";")[0],k))
			continue
		if ("(" in k and "(*" not in SPACE_CHARACTERS_REGEX.sub("",k) and k.count("(")==k.count(")") and k.count("{")==k.count("}") and "inline" not in k):
			k=k.split(";")
			fl.append((k[0][:-len(k[0].split("(")[-1])-1].split(" ")[-1],k[0].strip()))
			for e in k[1:]:
				e=e.strip()
				if (len(e)>0):
					l.append(e+";")
			continue
		o+="\n"+k.strip()
	i=0
	while (i<len(o)):
		if (i<len(o)-8 and o[i] not in IDENTIFIER_CHARACTERS and ((o[i+1:i+6]=="union" and o[i+6] not in IDENTIFIER_CHARACTERS) or (o[i+1:i+7]=="struct" and o[i+7] not in IDENTIFIER_CHARACTERS))):
			i+=(6 if o[i+1]=="u" else 7)
			while (o[i] not in ";{"):
				i+=1
			if (o[i]=="{"):
				b=1
				i+=1
				while (b!=0):
					if (o[i]=="{"):
						b+=1
					elif (o[i]=="}"):
						b-=1
					else:
						j=i
						while (i<len(o) and o[i] in SPACE_CHARACTERS):
							i+=1
						if (i!=j):
							o=o[:j]+("" if o[j-1] not in IDENTIFIER_CHARACTERS else " ")+o[i:]
							i-=i-j+(0 if o[j-1] not in IDENTIFIER_CHARACTERS else -1)
							continue
					i+=1
		i+=1
	for _,k in sorted(e_v,key=lambda e:e[0]):
		if (len(o)>0):
			o+="\n"
		o+=k
	for _,k in sorted(fl,key=lambda e:e[0]):
		if (len(o)>0):
			o+="\n"
		ai=len(k.split("(")[-1])
		o+=k[:-ai]
		st=True
		void=True
		for a in k[-ai:].split(")")[0].split(","):
			if (st is False):
				o+=","
			st=False
			if (a=="..."):
				o+="..."
				void=False
				break
			o+=a[:-len(a.split(" ")[-1])].strip()
			if (len(a[:-len(a.split(" ")[-1])].strip())>0):
				void=False
		if (void):
			o+="void"
		o+=");"
	o=d_s+"\n"+o.strip()
	return il+o
