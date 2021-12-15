import docs
import os
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



util.create_output_dir()
for k in os.listdir("src/web/css"):
	with open("src/web/css/"+k,"rb") as rf,open("build/web/css/"+k,"wb") as wf:
		wf.write(rf.read())
d_dt=docs.create_docs(util.get_docs_files())[0]
with open("src/web/404.html","rb") as rf,open("build/web/404.html","wb") as wf:
	wf.write(rf.read())
with open("src/web/index.html","rb") as rf,open("build/web/index.html","wb") as wf:
	wf.write(rf.read().replace(b"{{DATA}}",_generate_toc(d_dt)))
