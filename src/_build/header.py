import os
import re
import sys
import util



COMMENT_REGEX=re.compile(r"\/\*.*?\*\/|\/\/.*?$",re.DOTALL|re.MULTILINE)
DEFINE_LINE_CONTINUE_REGEX=re.compile(r"\s*\\\n[ \t\r]*")
DEFINE_REMOVE_REGEX=re.compile(r"^[ \t\r]*(#define [a-zA-Z0-9_]+\([^\)]*\))[ \t\r]*(\\\n(?:[ \t\r]*.*\\\n)+[ \t\r]*.*\n?)",re.MULTILINE)
INCLUDE_REGEX=re.compile(r"""^\s*#\s*include\s*(<[^>]*>|\"[^\"]*\")\s*$""",re.MULTILINE)
INTERNAL_SLL_HEADERS=["_dispatcher.h"]
MULTIPLE_NEWLINE_REGEX=re.compile(r"\n+")
STACK_ADD=0
STACK_SKIP=1
STACK_SKIP_INNER=2
STACK_UNKNOWN=3



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



def _add_source_file(b_fp,fp,file_list,include_list,cycle):
	if (fp in cycle):
		raise RuntimeError("Cycle: "+" > ".join(cycle[cycle.index(fp):]+[fp]))
	if (fp in file_list or not os.path.exists(b_fp+fp)):
		return ""
	o=""
	with open(b_fp+fp,"r") as rf:
		dt=rf.read()+"\n"
		n_incl=[]
		for k in INCLUDE_REGEX.findall(dt):
			n_incl.append(k[1:-1])
			o+=_add_source_file(b_fp,k[1:-1],file_list,include_list,cycle+[fp])
		o+=INCLUDE_REGEX.sub(lambda m:("" if os.path.exists(b_fp+m[1][1:-1]) or m[1][1:-1] in include_list else m[0]),dt)
		include_list.extend(n_incl)
	file_list.append(fp)
	return o



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



def generate_error_header(i_fp,o_fp,nm):
	util.log(f"Convering '{i_fp}' to '{o_fp}' ...")
	with open(i_fp,"rb") as rf,open(o_fp,"w") as wf:
		dt=rf.read().replace(b"\r\n",b"\n").split(b"$$$")
		if (len(dt)==1):
			wf.write(f"#ifndef __SLL_GENERATED_{nm}_H__\n#define __SLL_GENERATED_{nm}_H__ 1\n#include <sll/types.h>\n\n\n\n#define {nm}_SIZE {len(dt[0])}\n\n\n\nstatic const sll_char_t {nm}[]={{\n\t")
			_write_byte_array(wf,dt[0])
		else:
			wf.write(f"#ifndef __SLL_GENERATED_{nm}_H__\n#define __SLL_GENERATED_{nm}_H__ 1\n#include <sll/types.h>\n\n\n\n#define {nm}_START_SIZE {len(dt[0])}\n#define {nm}_END_SIZE {len(dt[1])}\n\n\n\nstatic const sll_char_t {nm}_START[]={{\n\t")
			_write_byte_array(wf,dt[0])
			wf.write(f"\n}};\n\n\n\nstatic const sll_char_t {nm}_END[]={{\n\t")
			_write_byte_array(wf,dt[1])
		wf.write("\n};\n\n\n\n#endif\n")



def parse_headers(fp):
	util.log("Combining Library Header Files...")
	o=""
	file_list=[]
	include_list=[]
	fp=fp.replace("\\","/").rstrip("/")+"/"
	for r,_,fl in os.walk(fp):
		if ("internal" in r or "generated" in r):
			continue
		for f in fl:
			if (f[-2:]==".h" and f not in INTERNAL_SLL_HEADERS):
				include_name=os.path.join(r,f)[len(fp):].replace("\\","/")
				o+=_add_source_file(fp,include_name,file_list,include_list,[])
	util.log(f"  Combined {len(file_list)} Files\nPreprocessing Combined Library Header File...")
	return DEFINE_REMOVE_REGEX.sub(lambda g:g.group(1)+" "+DEFINE_LINE_CONTINUE_REGEX.sub(r"",g.group(2)),MULTIPLE_NEWLINE_REGEX.sub(r"\n",COMMENT_REGEX.sub(r"",o).strip().replace("\r\n","\n"))).split("\n")



def generate_header(h_dt):
	defines={"__SLL_TIME_RAW__":str(util.BUILD_TIME),f"__SLL_BUILD_{util.system.upper()}":"1"}
	if (os.getenv("GITHUB_SHA") is not None):
		defines["__SLL_SHA__"]="\""+os.getenv("GITHUB_SHA")[:7]+"\""
		defines["__SLL_FULL_SHA__"]="\""+os.getenv("GITHUB_SHA")+"\""
	stack=[STACK_ADD]
	o=[]
	for e in h_dt:
		if (e[0]=="#"):
			macro=e[1:].split(" ")
			if (stack[-1] in [STACK_SKIP,STACK_SKIP_INNER]):
				if (macro[0] in ["ifdef","ifndef"]):
					stack.append(STACK_SKIP_INNER)
				elif (macro[0]=="else"):
					if (stack[-1]==STACK_SKIP):
						stack[-1]=STACK_ADD
						continue
				elif (macro[0]=="endif"):
					stack.pop()
					continue
			elif (macro[0]=="ifdef"):
				if (macro[1] in defines):
					stack.append(STACK_ADD)
					continue
				if (macro[1][:2]=="__"):
					stack.append(STACK_SKIP)
					continue
				stack.append(STACK_UNKNOWN)
			elif (macro[0]=="ifndef"):
				if (macro[1] in defines):
					stack.append(STACK_SKIP)
					continue
				if (macro[1][:2]=="__"):
					stack.append(STACK_ADD)
					continue
				stack.append(STACK_UNKNOWN)
			elif (macro[0]=="else"):
				if (stack[-1]!=STACK_UNKNOWN):
					stack[-1]=STACK_SKIP
					continue
			elif (macro[0]=="endif"):
				if (stack.pop()!=STACK_UNKNOWN):
					continue
			elif (macro[0]=="define" and stack[-1]==STACK_ADD and macro[1][:2]=="__" and "(" not in macro[1]):
				defines[macro[1]]=" ".join(macro[2:])
				continue
		if (stack[-1] not in [STACK_SKIP,STACK_SKIP_INNER]):
			o.append(e)
	regex_defines=[]
	for k,v in defines.items():
		i=0
		while (i<len(v)):
			for sk,sv in defines.items():
				if (sk==k):
					continue
				if (v.startswith(sk,i)):
					v=v[:i]+sv+v[i+len(sk):]
					i-=1
					break
			i+=1
		regex_defines.append((re.compile(fr"\b{k}\b"),v))
	for i,k in enumerate(o):
		for nm,v in regex_defines:
			k=nm.sub(v,k)
		o[i]=k
	return "\n".join(o)
