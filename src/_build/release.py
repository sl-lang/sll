import datetime
import header
import json
import os
import re
import requests
import subprocess
import sys
import util



FILE_PATH_MAIN_REGEX=re.compile(r"\[(`[^`]+?`)\]\[main((?:\/[a-zA-Z0-9_\-\.]+)+?)\]")
FILE_PATH_REGEX=re.compile(r"\[`[^`]+?`\]\[([0-9]+\.[0-9]+\.[0-9]+)((?:\/[a-zA-Z0-9_\-\.]+)+?)\]")
HEADING_REGEX=re.compile(r"\n#(#+) ")
ISSUE_REGEX=re.compile(r"\[#([0-9]+)\]")



if (__name__=="__main__"):
	util.create_output_dir()
	v=header.read_version("src/include/sll/version.h")
	util.log("Generating Release Changelog...")
	with open("CHANGELOG.md","r") as f:
		dt=f.read().replace("\r\n","\n")
		s=dt.index("\n",dt.index("\n## ")+1)+1
		dt=FILE_PATH_MAIN_REGEX.sub(fr"[\1][{v[0]}.{v[1]}.{v[2]}\2]",dt[s:dt.index("\n## ",s+1)])
		now=datetime.datetime.now()
		desc=f"# Sll [{v[0]}.{v[1]}.{v[2]}] - {now.year}-{now.month:02}-{now.day:02}\n\n"+HEADING_REGEX.sub(r"\n\1 ",dt).strip()+f"\n\n[{v[0]}.{v[1]}.{v[2]}]: https://github.com/sl-lang/sll/compare/v{v[0]}.{v[1]}.{v[2]-1}...v{v[0]}.{v[1]}.{v[2]}\n"
		util.log("  Generating Links...")
		for e in sorted(list(dict.fromkeys(map(int,ISSUE_REGEX.findall(dt)))),reverse=True):
			desc+=f"[#{e}]: https://github.com/sl-lang/sll/issues/{e}\n"
		l={}
		for t,p in FILE_PATH_REGEX.findall(dt):
			t=tuple(map(int,t.split(".")))
			if (t not in l):
				l[t]=[p]
			else:
				l[t].append(p)
		for t,pl in sorted(l.items(),key=lambda e:e[0],reverse=True):
			ts=".".join(map(str,t))
			for p in sorted(pl):
				desc+=f"[{ts}{p}]: https://github.com/sl-lang/sll/blob/v{ts}{p}\n"
	util.log("Creating Git Tags...")
	cwd=os.getcwd()
	os.chdir("build")
	subprocess.run(["git","clone","https://github.com/sl-lang/sll.git"])
	os.chdir("sll")
	subprocess.run(["git","remote","set-url","origin",f"https://krzem5:{sys.argv[-1]}@github.com/sl-lang/sll.git"])
	subprocess.run(["git","checkout","-b",f"v{v[0]}.{v[1]}.{v[2]}","main"])
	subprocess.run(["git","tag",f"sll-v{v[0]}.{v[1]}.{v[2]}"])
	subprocess.run(["git","push","origin",f"v{v[0]}.{v[1]}.{v[2]}"])
	subprocess.run(["git","push","origin","--tags"])
	os.chdir(cwd)
	util.log("Creating Release...")
	r_id=requests.post("https://api.github.com/repos/sl-lang/sll/releases",headers={"accept":"application/vnd.github.v3+json","Authorization":"token "+sys.argv[-1]},data=json.dumps({"tag_name":f"sll-v{v[0]}.{v[1]}.{v[2]}","target_commitish":f"v{v[0]}.{v[1]}.{v[2]}","prerelease":True,"body":desc,"name":f"sll-v{v[0]}.{v[1]}.{v[2]}"})).json()["id"]
	util.log("Uploading Assets...")
	for r,_,fl in os.walk("assets"):
		for fp in fl:
			if (fp[-3:]==".dt"):
				continue
			util.log(f"  Uploading '{os.path.join(r,fp)}'...")
			with open(os.path.join(r,fp),"rb") as f:
				requests.post(f"https://uploads.github.com/repos/sl-lang/sll/releases/{r_id}/assets?name={fp}",headers={"Accept":"application/vnd.github.v3+json","Authorization":"token "+sys.argv[-1],"Content-Type":"application/octet-stream"},data=f.read())
