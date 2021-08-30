import os
import sys



BASE64_ALPHABET=b"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
MAX_SIZE=10485760



def upload(fpl,t):
	dt=b""
	for k in fpl:
		with open("build/"+k,"rb") as f:
			sz=os.stat("build/"+k).st_size
			dt+=bytearray([len(k),sz>>24,(sz>>16)&0xff,(sz>>8)&0xff,sz&0xff])+bytes(k,"utf-8")+f.read()
	if (len(dt)>MAX_SIZE):
		raise RuntimeError("Too Much Data")
	with open("token_data/cf-secret.dt","r") as f:
		id_,nm_id,tk=map(str.strip,f.read().split("\n")[:3])
	o=__import__("requests").put(f"https://api.cloudflare.com/client/v4/accounts/{id_}/storage/kv/namespaces/{nm_id}/values/data-"+t,headers={"Authorization":"Bearer "+tk,"Content-Type":"application/json"},data=dt)
	if (o.status_code!=200):
		print(o.text)
		sys.exit(1)
