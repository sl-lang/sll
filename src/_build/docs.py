import json
import re



DOCS_COMMENT_REGEX=re.compile(r"\/\*\s*~~DOCS~~\s*(.*?)\*\/",re.S)



def create_docs(fl):
	o=[]
	for nm in fl:
		with open(nm,"r") as f:
			for k in DOCS_COMMENT_REGEX.findall(f.read().replace("\r\n","\n")):
				dt={"flag":[],"name":None,"desc":None,"args":[],"ret":[],"err_ret":0}
				for e in k.split("\n"):
					t=e.split(" ")[0]
					if (t=="TYPE"):
						for se in e.lower().split(" ")[1:]:
							if (se not in dt["flag"]):
								dt["flag"].append(se)
					elif (t=="FUNC"):
						if (dt["name"] is not None):
							raise RuntimeError("Only one 'FUNC' tag can be present")
						dt["name"]=e.split(" ")[1]
					elif (t=="DESC"):
						if (dt["desc"] is not None):
							raise RuntimeError("Only one 'DESC' tag can be present")
						dt["desc"]=e[5:]
					elif (t=="ARG"):
						se=e.split(" ")
						dt["args"].append({"type":se[1].replace("?",""),"opt":("?" in se[1]),"desc":" ".join(se[2:])})
					elif (t=="RET"):
						se=e.split(" ")
						dt["ret"].append({"type":se[1],"desc":" ".join(se[2:])})
					elif (t=="ERR"):
						dt["err_ret"]=int(e.split(" ")[1])
					else:
						raise RuntimeError(f"Unknown tag '{t}'")
				o.append(dt)
	with open("build/docs.json","w") as f:
		f.write(json.dumps(o,indent="\t"))
	return o
