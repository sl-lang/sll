import json
import re



DOCS_COMMENT_REGEX=re.compile(r"\/\*\s*~~DOCS~~\s*(.*?)\*\/",re.S)
FLAG_API=1
FLAG_VAR_ARG=2



def create_docs(fl):
	o=[]
	for nm in fl:
		with open(nm,"r") as f:
			for k in DOCS_COMMENT_REGEX.findall(f.read().replace("\r\n","\n")):
				dt={"flag":0,"name":None,"desc":None,"args":[],"ret":[]}
				for e in k.split("\n"):
					t=e.split(" ")[0]
					if (t=="TYPE"):
						for se in e.split(" ")[1:]:
							if (se=="api"):
								dt["flag"]|=FLAG_API
							elif (se=="var_arg"):
								dt["flag"]|=FLAG_VAR_ARG
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
						dt["args"].append((se[1]," ".join(se[2:])))
					elif (t=="RET"):
						se=e.split(" ")
						dt["ret"].append((se[1]," ".join(se[2:])))
					else:
						raise RuntimeError(f"Unknown tag '{t}'")
				o.append(dt)
	with open("build/docs.json","w") as f:
		f.write(json.dumps(o,indent="\t"))
