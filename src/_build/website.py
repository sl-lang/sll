import docs
import json
import os
import sys
import util



def _generate_toc(dt):
	m={}
	for k in dt["groups"]:
		m[k]={"":[]}
	for k in dt["data"]:
		e=(k["name"],f"<li><a href=\"/{k['name']}\">{k['name']+('()' if 'func' in k['flag'] else '')}</a></li>")
		if (k["subgroup"] is None):
			m[k["group"]][""].append(e)
		elif (k["subgroup"] not in m[k["group"]]):
			m[k["group"]][k["subgroup"]]=[e]
		else:
			m[k["group"]][k["subgroup"]].append(e)
	o=""
	for k,v in sorted(m.items(),key=lambda e:dt["groups"][e[0]]["name"]):
		o+=f"<div class=\"group\" id=\"{k}\"><h2 class=\"title\">{dt['groups'][k]['name']}</h2><div class=\"group-box\">"
		if (v[""]):
			o+=f"<div class=\"subgroup\"><ul>{''.join([e[1] for e in sorted(v[''],key=lambda se:se[0])])}</ul></div>"
		del v[""]
		for sk,sv in sorted(v.items(),key=lambda e:dt["subgroups"][e[0]]["name"]):
			o+=f"<div class=\"subgroup\"><h3 class=\"sg-title\">{dt['subgroups'][sk]['name']}</h3><ul>{''.join([e[1] for e in sorted(sv,key=lambda se:se[0])])}</ul></div>"
		o+="</div></div>"
	return bytes(o,"utf-8")



def _add_data(nm,dt):
	nm=nm.replace("\\","/")[:255].encode("ascii","ignore")
	dt=dt[:16777215]
	return bytearray([len(nm),len(dt)&0xff,(len(dt)>>8)&0xff,len(dt)>>16])+nm+dt



def generate():
	o=bytearray()
	util.log("Reading CSS Files...")
	for k in os.listdir("src/web/client/css"):
		util.log(f"  Found file 'src/web/client/css/{k}'")
		with open("src/web/client/css/"+k,"rb") as rf:
			o+=_add_data("/css/"+k,rf.read())
	util.log("Collecting Documentation Files...")
	d_fl=util.get_docs_files()
	util.log(f"  Found {len(d_fl)} Files\nGenerating Documentation...")
	d_dt=docs.create_docs(d_fl)[0]
	util.log(f"Generating Table of Content for {len(d_dt['data'])} Definitions...")
	toc=_generate_toc(d_dt)
	util.log("Reading 'src/web/client/index.html'...")
	with open("src/web/client/index.html","rb") as rf:
		o+=_add_data("/index.html",rf.read().replace(b"{{DATA}}",toc))
	util.log("Reading 'src/web/client/404.html'...")
	with open("src/web/client/404.html","rb") as rf:
		o+=_add_data("/404.html",rf.read())
	util.log("Reading 'src/web/client/shell_install.sh'...")
	with open("src/web/client/shell_install.sh","rb") as rf:
		o+=_add_data("shell_install.sh",rf.read())
	if (os.getenv("GITHUB_ACTIONS",None) is not None):
		with open("web-bundle.dt","wb") as f:
			f.write(o)
	else:
		util.log("Generating Local Website...")
		i=0
		while (i<len(o)):
			l=o[i]
			sz=o[i+1]|(o[i+2]<<8)|(o[i+3]<<16)
			i+=4
			fp="build/web/"+o[i:i+l].decode("ascii","ignore")
			i+=l
			util.log(f"  Writing '{fp}' ({sz} bytes)...")
			with open(fp,"wb") as f:
				f.write(o[i:i+sz])
			i+=sz



if (__name__=="__main__"):
	import requests
	if ("--server" in sys.argv):
		with open("src/web/server/main.js","rb") as f:
			util.log("Uploading Server Code...")
			requests.put(f"https://api.cloudflare.com/client/v4/accounts/{sys.argv[-3]}/workers/scripts/{sys.argv[-2]}",data=f.read(),headers={"Authorization":"Bearer "+sys.argv[-1],"Content-Type":"application/javascript"})
	else:
		url=f"https://api.cloudflare.com/client/v4/accounts/{sys.argv[-3]}/storage/kv/namespaces/{sys.argv[-2]}/"
		h={"Authorization":"Bearer "+sys.argv[-1],"Content-Type":"application/json"}
		util.log("Listing Current KV Keys...")
		l=requests.get(url+"keys",headers=h).json()["result"]
		l=[k["name"] for k in l if k["name"][:5]!="/apt/"]
		util.log(f"  Found {len(l)} Keys\nClearing KV Storage...")
		requests.delete(url+"bulk",headers=h,data="["+",".join([f"\"{e}\"" for e in l])+"]")
		util.log("Generating Request...")
		with open("web-bundle.dt","rb") as f:
			dt=f.read()
		i=0
		o=[]
		while (i<len(dt)):
			l=dt[i]
			sz=dt[i+1]|(dt[i+2]<<8)|(dt[i+3]<<16)
			i+=4
			fp=dt[i:i+l].decode("ascii","ignore")
			i+=l
			util.log(f"  Encoding File '{fp}' ({sz} bytes)...")
			o.append({"key":fp,"value":util.encode(dt[i:i+sz]),"base64":True})
			i+=sz
		util.log("Uploading Data...")
		requests.put(url+"bulk",headers=h,data=json.dumps(o))
