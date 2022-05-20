import os
import platform
import re
import sys



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



def generate_header(src_fp,out_fp,time):
	log=(print if "--verbose" in sys.argv else lambda _:None)
	log(f"Combining header files from '{src_fp}' to '{out_fp}'...\n  Listing files...")
	src=""
	file_list=[]
	include_list=[]
	src_fp=src_fp.replace("\\","/").rstrip("/")+"/"
	for r,_,fl in os.walk(src_fp):
		if ("internal" in r or "generated" in r):
			continue
		for f in fl:
			if (f[-2:]==".h" and f not in INTERNAL_SLL_HEADERS):
				include_name=os.path.join(r,f)[len(src_fp):].replace("\\","/")
				src+=_add_source_file(src_fp,include_name,file_list,include_list,[])
	log(f"    Found {len(file_list)} files\n  Preprocessing combined header files...")
	defines={"__SLL_TIME_RAW__":time,f"__SLL_BUILD_{platform.system().upper()}":"1","NULL":"((void*)0)"}
	if (os.getenv("GITHUB_SHA") is not None):
		defines["__SLL_SHA__"]="\""+os.getenv("GITHUB_SHA")[:7]+"\""
		defines["__SLL_FULL_SHA__"]="\""+os.getenv("GITHUB_SHA")+"\""
	stack=[STACK_ADD]
	o=[]
	for e in DEFINE_REMOVE_REGEX.sub(lambda g:g.group(1)+" "+DEFINE_LINE_CONTINUE_REGEX.sub(r"",g.group(2)),COMMENT_REGEX.sub(r"",src).strip().replace("\r\n","\n")).split("\n"):
		if (not e):
			continue
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
	log(f"  Writing header file to '{out_fp}...")
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
	with open(out_fp,"w") as wf:
		wf.write("#ifndef __SLL_H__\n#define __SLL_H__ 1")
		for k in o:
			for nm,v in regex_defines:
				k=nm.sub(v,k)
			wf.write("\n"+k)
		wf.write("\n#endif\n")



if (__name__=="__main__"):
	generate_header("src/sll/include","build/sll.h",sys.argv[1])
