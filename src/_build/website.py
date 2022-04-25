import docs
import hashlib
import json
import os
import sys
import util



def _add_data(nm,dt):
	nm=nm.replace("\\","/")[:255].encode("ascii","ignore")
	dt=dt[:16777215]
	if (nm[-5:]==b".html"):
		dt=dt.replace(b"{{ROOT}}",(b"" if os.getenv("DOMAIN_ROOT",None) is not None else b"."))
	return bytearray([len(nm),len(dt)&0xff,(len(dt)>>8)&0xff,len(dt)>>16])+nm+dt



def _generate_data(dt,pg_src):
	m={}
	for k in dt["groups"]:
		m[k]={"":[]}
	for k in dt["data"]:
		if (k["subgroup"] is None):
			m[k["group"]][""].append(k)
		elif (k["subgroup"] not in m[k["group"]]):
			m[k["group"]][k["subgroup"]]=[k]
		else:
			m[k["group"]][k["subgroup"]].append(k)
	toc=""
	pg_dt=b""
	for k,v in sorted(m.items(),key=lambda e:dt["groups"][e[0]]["name"]):
		toc+=f"<div class=\"group\" id=\"{k}\"><a href=\"{{{{ROOT}}}}/{k}.html\"><h2 class=\"title\">{dt['groups'][k]['name']}</h2></a><div class=\"group-box\">"
		pg=f"<h1>{dt['groups'][k]['name']}</h1><h3>{dt['groups'][k]['desc']}</h3>"
		for sk,sv in sorted(v.items(),key=lambda e:("" if e[0]=="" else dt["subgroups"][e[0]]["name"])):
			if (len(sv)==0):
				continue
			toc+="<div class=\"subgroup\">"
			if (len(sk)!=0):
				toc+=f"<a href=\"{{{{ROOT}}}}/{k}.html#{sk}\"><h3 class=\"sg-title\">{dt['subgroups'][sk]['name']}</h3></a>"
				pg+=f"<a id=\"{sk}\" href=\"#{sk}\" style=\"text-decoration: none;color: #3010ff\"><h2>{dt['subgroups'][sk]['name']}</h2></a><h4>{dt['subgroups'][sk]['desc']}</h4>"
			toc+="<ul>"
			for e in sorted(sv,key=lambda se:se["name"]):
				toc+=f"<li><a href=\"{{{{ROOT}}}}/{e['group']}.html#{e['name']}\">{e['name']+('()' if 'func' in e['flag'] else '')}</a></li>"
				pg+=f"<div><a id=\"{e['name']}\" href=\"#{e['name']}\" style=\"text-decoration: none;color: #ff0000\"><pre>"
				if ("func" in e["flag"]):
					if ("macro" in e["flag"]):
						pg+="#define "+e["name"]+"("
						st=True
						for a in e["args"]:
							if (st):
								st=False
							else:
								pg+=","
							pg+="<span style=\"color: #ceb187\">"+a["type"]+"</span> "+a["name"]
						if ("var_arg" in e["flag"]):
							if (not st):
								pg+=","
							pg+="..."
						pg+=")"
						if (e["ret"] is not None):
							pg+=" -> "+e["ret"]["type"]
					else:
						if ("check_output" in e["flag"]):
							pg+="<span style=\"color: #cf89a2\">(check_output)</span> "
						if (e["ret"] is not None):
							pg+=e["ret"]["type"]
						else:
							pg+="void"
						pg+=" "+e["name"]+"("
						if (len(e["args"])==0):
							pg+="void"
						else:
							st=True
							for a in e["args"]:
								if (st):
									st=False
								else:
									pg+=","
								pg+=a["type"]+" "+a["name"]
						if ("var_arg" in e["flag"]):
							pg+=",..."
						pg+=");"
				else:
					if ("macro" in e["flag"]):
						pg+="#define "+e["name"]+" -> "+e["type"]["type"]
					else:
						pg+=e["type"]["type"]+" "+e["name"]+";"
				pg+=f"</pre></a><pre>Description: {e['desc']}"
				if (len(e["args"])!=0):
					pg+="\nArguments:"
					for a in e["args"]:
						pg+=f"\n  {a['name']} -> {a['desc']}"
				if (e["ret"] is not None):
					pg+=f"\nReturn Value: {e['ret']['desc']}"
				pg+="</pre></div>"
			toc+="</ul></div>"
		toc+="</div></div>"
		util.log(f"  Generating '/{k}.html'...")
		pg_dt+=_add_data(f"/{k}.html",pg_src.replace(b"{{DATA}}",bytes(pg,"utf-8")).replace(b"{{NAME}}",bytes(dt["groups"][k]["name"],"utf-8")))
	return bytes(toc,"utf-8"),pg_dt



def generate():
	o=bytearray()
	util.log("Reading CSS Files...")
	for k in os.listdir("src/web/css"):
		util.log(f"  Found file 'src/web/css/{k}'")
		with open("src/web/css/"+k,"rb") as rf:
			o+=_add_data("/css/"+k,rf.read())
	util.log("Reading JS Files...")
	for k in os.listdir("src/web/js"):
		util.log(f"  Found file 'src/web/js/{k}'")
		with open("src/web/js/"+k,"rb") as rf:
			o+=_add_data("/js/"+k,rf.read())
	util.log("Collecting Documentation Files...")
	d_fl=util.get_docs_files()
	util.log(f"  Found {len(d_fl)} Files\nGenerating Documentation...")
	d_dt=docs.create_docs(d_fl)
	util.log("Reading 'src/web/page.html'...")
	with open("src/web/page.html","rb") as rf:
		pg_src=rf.read()
	util.log(f"Generating Table of Content & Pages for {len(d_dt['data'])} Symbols...")
	toc,pg_dt=_generate_data(d_dt,pg_src)
	o+=pg_dt
	util.log("Reading 'src/web/index.html'...")
	with open("src/web/index.html","rb") as rf:
		o+=_add_data("/index.html",rf.read().replace(b"{{DATA}}",toc))
	util.log("Reading 'src/web/404.html'...")
	with open("src/web/404.html","rb") as rf:
		o+=_add_data("404.html",rf.read())
	util.log("Reading 'src/web/apt.sh'...")
	with open("src/web/apt.sh","rb") as rf:
		o+=_add_data("/apt",rf.read())
	if (os.getenv("GITHUB_ACTIONS",None) is not None):
		with open("web-bundle.dt","wb") as f:
			f.write(o)
	else:
		util.log("Generating Local Website...")
		if (not os.path.exists("build/web")):
			os.mkdir("build/web")
		i=0
		while (i<len(o)):
			l=o[i]
			sz=o[i+1]|(o[i+2]<<8)|(o[i+3]<<16)
			i+=4
			fp="build/web/"+o[i:i+l].decode("ascii","ignore")
			os.makedirs(fp[:fp.rindex("/")],exist_ok=True)
			i+=l
			util.log(f"  Writing '{fp}' ({sz} bytes)...")
			with open(fp,"wb") as f:
				f.write(o[i:i+sz])
			i+=sz



def upload():
	import requests
	url=f"https://api.cloudflare.com/client/v4/accounts/{sys.argv[-3]}/storage/kv/namespaces/{sys.argv[-2]}/"
	h={"Authorization":"Bearer "+sys.argv[-1],"Content-Type":"application/json"}
	util.log("Listing Current KV Keys...")
	tb_r=requests.get(url+"values/__links",headers=h,stream=True).raw
	tb_r.decode_content=True
	rm_l=[]
	n_tb=[]
	for k in tb_r.read().split(b"\n"):
		if (k[:5]==b"/apt/" or k[:5]==b"/bin/"):
			n_tb.append(k)
		else:
			util.log(f"  Found Key '{k.decode('ascii','ignore')}' ")
			rm_l.append(hashlib.sha256(k).hexdigest())
	util.log("Generating Request...")
	with open("web-bundle.dt","rb") as f:
		dt=f.read()
	i=0
	o=[]
	while (i<len(dt)):
		l=dt[i]
		sz=dt[i+1]|(dt[i+2]<<8)|(dt[i+3]<<16)
		i+=4
		fp=dt[i:i+l]
		i+=l
		fp_h=hashlib.sha256(fp).hexdigest()
		if (fp_h in rm_l):
			rm_l.remove(fp_h)
		util.log(f"  Encoding File '{fp.decode('ascii','ignore')}' ({sz} bytes) -> '{fp_h}'...")
		n_tb.append(fp)
		o.append({"key":fp_h,"value":util.encode(dt[i:i+sz]),"base64":True})
		i+=sz
	o.append({"key":"__links","value":util.encode(b"\n".join(n_tb)),"base64":True})
	if (len(rm_l)>0):
		util.log("Clearing KV Storage...")
		requests.delete(url+"bulk",headers=h,data="["+",".join([f"\"{e}\"" for e in rm_l])+"]")
	util.log("Uploading Data...")
	requests.put(url+"bulk",headers=h,data=json.dumps(o))



if (__name__=="__main__"):
	upload()
