import os
import requests
import sys



GITHUB_HEADERS={"Authorization":f"token {sys.argv[-1]}","Accept":"application/vnd.github.v3+json","User-Agent":"Sl-Lang Commit Diff"}
GITHUB_OWNER_NAME="krzem5"



def _force_all():
	if (os.getenv("GITHUB_EVENT_NAME","")=="workflow_dispatch"):
		return True
	dt=requests.get("https://api.github.com/repos/sl-lang/sll/actions/workflows/all.yml/runs?per_page=2",headers=GITHUB_HEADERS).json()["workflow_runs"]
	return (dt[0]["run_attempt"]>1 or dt[1]["conclusion"]!="success")



def _get_tree(sha):
	t=requests.get(url=f"https://api.github.com/repos/sl-lang/sll/git/trees/{sha}?recursive=true",headers=GITHUB_HEADERS).json()
	if (t["truncated"]):
		raise RuntimeError
	return [(k["path"],k["size"],k["sha"]) for k in t["tree"] if k["type"]=="blob"]



def _diff(sha):
	a=_get_tree(sha)
	b=_get_tree(requests.get("https://api.github.com/repos/sl-lang/sll/git/commits/"+sha,headers=GITHUB_HEADERS).json()["parents"][0]["sha"])
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
	for k in b:
		l.append(k[0])
	o=set()
	for k in l:
		k=k.split("/")
		i=len(k)-1
		while (i and k[i-1]!="platform"):
			i-=1
		e="/".join(k[:i])
		while (i<len(k)):
			if (len(e)>0):
				e+="/"
			e+=k[i]
			o.add(e)
			i+=1
	return o



if (os.getenv("GITHUB_EVENT_NAME","")=="workflow_dispatch" and os.getenv("GITHUB_ACTOR","").lower()!=GITHUB_OWNER_NAME):
	sys.exit(1)
o=""
if (_force_all()):
	for k in sys.argv[1:-1]:
		k=k.split("=")[0]
		print(f"Changes found for '{k}'")
		if (len(o)!=0):
			o+=","
		o+=f"\"{k}\":1"
else:
	t=_diff(os.environ["GITHUB_SHA"])
	o=""
	for k in sys.argv[1:-1]:
		k=k.split("=")
		if (".github/workflows/all.yml" in t or "src/_build" in t):
			v="1"
		else:
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
sys.stdout.write(f"::set-output name=table::{{{o}}}")
