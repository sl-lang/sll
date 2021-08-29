import os
import requests
import sys



BASE64_ALPHABET=b"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
MAX_SIZE=10485760



def upload(fpl,t):
	t=bytes(t,"utf-8")
	dt=b""
	for k in fpl:
		with open("build/"+k,"rb") as f:
			sz=os.stat("build/"+k).st_size
			dt+=bytearray([len(k),sz>>24,(sz>>16)&0xff,(sz>>8)&0xff,sz&0xff])+bytes(k,"utf-8")+f.read()
	o=bytearray()
	i=0
	while (i<len(dt)-2):
		o.append(BASE64_ALPHABET[dt[i]>>2])
		o.append(BASE64_ALPHABET[((dt[i]<<4)&0x3f)|(dt[i+1]>>4)])
		o.append(BASE64_ALPHABET[((dt[i+1]<<2)&0x3f)|(dt[i+2]>>6)])
		o.append(BASE64_ALPHABET[dt[i+2]&0x3f])
		i+=3
	if (i==len(dt)-2):
		o.append(BASE64_ALPHABET[dt[i]>>2])
		o.append(BASE64_ALPHABET[((dt[i]<<4)&0x3f)|(dt[i+1]>>4)])
		o.append(BASE64_ALPHABET[(dt[i+1]<<2)&0x3f])
		o.append(0x3d)
	elif (i==len(dt)-1):
		o.append(BASE64_ALPHABET[dt[i]>>2])
		o.append(BASE64_ALPHABET[(dt[i]<<4)&0x3f])
		o.append(0x3d)
		o.append(0x3d)
	if (len(o)>MAX_SIZE):
		raise RuntimeError("Executable Too Big")
	with open("token_data/cf-secret.dt","r") as f:
		id_,nm_id,tk=map(str.strip,f.read().split("\n")[:3])
	o=requests.put(f"https://api.cloudflare.com/client/v4/accounts/{id_}/storage/kv/namespaces/{nm_id}/bulk",headers={"Authorization":"Bearer "+tk,"Content-Type":"application/json"},data=b"[{\"key\":\"chunk-count-"+t+b"\",\"value\":\"1\"},{\"key\":\"chunk-"+t+b"-0\",\"value\":\""+o+b"\",\"base64\":true}]")
	if (o.status_code!=200):
		print(o.text)
		sys.exit(1)
