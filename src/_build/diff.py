import json
import os
import requests
import sys



GH_HEADERS={"Authorization":f"token {sys.argv[1]}","Accept":"application/vnd.github.v3+json","User-Agent":"Sl-Lang Commit Diff"}



def _get_tree(sha):
	t=requests.get(url=f"https://api.github.com/repos/sl-lang/sll/git/trees/{sha}?recursive=true",headers=GH_HEADERS).json()
	if (t["truncated"]):
		raise RuntimeError
	return [(k["path"],k["size"],k["sha"]) for k in t["tree"] if k["type"]=="blob"]



def _diff(sha):
	a=_get_tree(sha)
	b=_get_tree(requests.get("https://api.github.com/repos/sl-lang/sll/git/commits/"+sha,headers=GH_HEADERS).json()["parents"][0]["sha"])
	l=[]
	for k in a:
		n=True
		for e in b:
			if (k[0]==e[0]):
				b.remove(e)
				if (k[1]!=e[1] or k[2]!=e[2]):
					l.append(k[0])
				n=False
				break
		if (n):
			l.append(k[0])
	l.extend(b)
	o=set()
	for k in l:
		k=k.split("/")
		o.add(k[0])
		e=k[0]
		for i in range(1,len(k)):
			e+="/"+k[i]
			o.add(e)
	return o



t=_diff(os.environ["GITHUB_SHA"])
o=""
for k in sys.argv[2:]:
	k=k.split("=")
	v="0"
	for e in k[1].split(","):
		if (e in t):
			v="1"
			break
	if (v=="0"):
		print(f"No changes found for '{k[0]}'")
	else:
		print(f"Changes found for '{k[0]}'")
	if (len(o)!=0):
		o+=","
	o+=f"\"{k[0]}\":{v}"
sys.stdout.write(f"::set-output name=table::{{{o}}}\n")
