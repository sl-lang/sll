import datetime
import os
import re



COMMENT_REGEX=re.compile(br"\/\*.*?\*\/|\/\/.*?$",re.DOTALL|re.MULTILINE)
DEFINE_LINE_CONTINUE_REGEX=re.compile(br"\\\n[ \t\r]*")
DEFINE_REMOVE_REGEX=re.compile(br"^[ \t\r]*(#define [a-zA-Z0-9_]+\([^\)]*\))[ \t\r]*(\\\n(?:[ \t\r]*.*\\\n)+[ \t\r]*.*\n?)",re.MULTILINE)
HEADER_INCLUDE_GUARD_REGEX=re.compile(br"^\s*#ifndef\s+(?P<h_nm>[a-zA-Z0-9_]+)\s+#define\s+(?P=h_nm)\s+(?:1\s+)?(.*)#endif\s*$",re.DOTALL)
HEX_NUMBER_REGEX=re.compile(br"\b0x[0-9a-f]+\b")
IDENTIFIER_CHARACTERS=b"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"
IDENTIFIER_REGEX=re.compile(br"\b[a-zA-Z0-9_]+\b")
INCLUDE_REGEX=re.compile(br"""^\s*#\s*include\s*(<[^>]*>|\"[^>]*\")\s*$""",re.MULTILINE)
LETTERS=b"abcdefghijklmnopqrstuvwxyz"
MONTHS=["Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"]
MULTIPLE_NEWLINE_REGEX=re.compile(br"\n+")
NUMBERS=b"0123456789"
SPACE_CHARACTERS=b" \t\n\v\f\r"
SPACE_CHARACTERS_REGEX=re.compile(b" \t\n\v\f\r")



def _expand_macros(k,dm,dfm):
	i=0
	while (i<len(k)):
		if ((i==0 or k[i-1:i] not in IDENTIFIER_CHARACTERS) and k[i] not in NUMBERS):
			j=i
			while (i<len(k) and k[i:i+1] in IDENTIFIER_CHARACTERS):
				i+=1
			for e,v in dm.items():
				if (e==k[j:i]):
					k=k[:j]+v+k[i:]
					i=j-1
					break
			if (i!=j):
				for e,v in dfm.items():
					if (e==k[j:i]):
						if (k[i:i+1]!=b"("):
							continue
						i+=1
						l=i
						while (k[i:i+1]!=b")"):
							i+=1
						a=k[l:i].split(b".")
						if (len(a)!=len(v[0])):
							raise RuntimeError("Invalid Macro Argument Count")
						tmp=k[i+1:]
						k=k[:j]
						for m,n in enumerate(v[1]):
							if (m&1):
								k+=a[n]
							else:
								k+=n
						k+=tmp
						i=j-1
						break
		i+=1
	return k



def generate_help(i_fp,o_fp,vb):
	if (vb):
		print(f"Convering '{i_fp}' to Header File...")
	with open(i_fp,"rb") as rf,open(o_fp,"wb") as wf:
		wf.write(b"#ifndef __HELP_TEXT_H__\n#define __HELP_TEXT_H__ 1\n#include <stdint.h>\nconst uint8_t HELP_TEXT[]={")
		st=True
		for c in rf.read().replace(b"\r\n",b"\n"):
			if (st is False):
				wf.write(b",")
			wf.write(bytearray([48,120,(48 if (c>>4)<10 else 87)+(c>>4),(48 if (c&0xf)<10 else 87)+(c&0xf)]))
			st=False
		wf.write(b",0x00};\n#endif")



def parse_header(fp,vb):
	if (vb):
		print("Combining Library Header Files...")
	o=b""
	il=[]
	for f in os.listdir(fp):
		if (f[0]!="_" and f[-2:]==".h"):
			with open(fp+"/"+f,"rb") as rf:
				il.append(bytes(f,"utf-8"))
				dt=rf.read()
				m=HEADER_INCLUDE_GUARD_REGEX.fullmatch(dt)
				if (m!=None):
					o+=m.group(2)+b"\n"
				else:
					o+=dt+b"\n"
	if (vb):
		print(f"  Combined {len(il)} Files\nPreprocessing Combined Library Header File...")
	o=INCLUDE_REGEX.sub(lambda m:(b"" if m.group(1)[1:-1] in il else b"#include <"+(il.append(m.group(1)[1:-1]),m.group(1)[1:-1])[1]+b">"),DEFINE_REMOVE_REGEX.sub(lambda g:g.group(1)+b" "+DEFINE_LINE_CONTINUE_REGEX.sub(br"",g.group(2)),MULTIPLE_NEWLINE_REGEX.sub(br"\n",COMMENT_REGEX.sub(br"",o).strip().replace(b"\r\n",b"\n")))).split(b"\n")
	return o



def generate_header(h_dt,c_m):
	l=[]
	st=[True]
	tm=datetime.datetime.now()
	dm={b"__TIME__":bytes(tm.strftime("\"%H:%M:%S\""),"utf-8"),b"__DATE__":bytes(tm.strftime(f"\"{MONTHS[tm.month-1]} %d %Y\""),"utf-8"),b"UINT16_MAX":b"65535",b"UINT32_MAX":b"4294967295u"}
	dfm={}
	d_v=[]
	d_f=[]
	il=b""
	for e in h_dt:
		e=e.strip()
		if (e[:1]==b"#"):
			f=e[1:].split(b" ")
			if (f[0]==b"ifdef"):
				if (f[1] in c_m or f[1] in dm):
					st.append(True)
				else:
					st.append(False)
				continue
			elif (f[0]==b"ifndef"):
				if (f[1] in c_m or f[1] in dm):
					st.append(False)
				else:
					st.append(True)
				continue
			elif (f[0]==b"else"):
				st[-1]=not st[-1]
				continue
			elif (f[0]==b"endif"):
				st.pop()
				continue
			elif (f[0]==b"include" and False not in st):
				f_nm="src/include/"+str(b" ".join(f[1:]).strip(b"\"").strip(b"<").strip(b">"),"utf-8")
				if (not os.path.exists(f_nm)):
					il+=b"\n#include "+b" ".join(f[1:])
				continue
			elif (f[0]==b"define" and False not in st):
				if (b"(" in f[1]):
					al=f[1].split(b"(")[1].split(b")")[0].split(b",")
					if (al[-1]==b""):
						al.pop(len(al)-1)
					al=tuple(al)
					k=b" ".join(f[2:])
					if (f[1][:2]!=b"__"):
						d_f.append((f[1],k))
					sl=[b""]
					sli=0
					i=0
					while (i<len(k)):
						if (i==0 or k[i-1:i] not in IDENTIFIER_CHARACTERS):
							nxt=False
							for j,se in enumerate(al):
								if (k.startswith(se,i) and k[i+len(se):i+len(se)+1] not in IDENTIFIER_CHARACTERS):
									sl.append(j)
									sl.append(b"")
									sli+=2
									i+=len(se)
									nxt=True
									break
							if (nxt):
								continue
						sl[sli]+=k[i:i+1]
						i+=1
					dfm[f[1].split(b"(")[0]]=(al,tuple([(sl_e if j&1 else sl_e.replace(b"##",b"")) for j,sl_e in enumerate(sl)]))
				else:
					dm[f[1]]=b" ".join(f[2:])
					if (f[1][:2]!=b"__"):
						d_v.append((f[1],b" ".join(f[2:])))
				continue
		if (False not in st):
			l.append(e)
	d_s=b""
	o=b""
	for i,(k,v) in enumerate(sorted(d_v,key=lambda e:e[0])):
		v=HEX_NUMBER_REGEX.sub(lambda m:bytes(str(int(m.group(0),16)),"utf-8"),_expand_macros(v,dm,dfm))
		d_v[i]=(k,v)
		d_s+=b"\n#define "+k+b" "+v.strip()
	for i,(k,v) in enumerate(sorted(d_f,key=lambda e:e[0])):
		k=SPACE_CHARACTERS_REGEX.sub(b"",k)
		a={}
		j=[0]
		nk=k.split(b"(")[0]+b"("
		for e in k[k.index(b"(")+1:-1].split(b","):
			if (len(j)>1 or j[0]!=0):
				nk+=b","
			a[e]=b"".join([LETTERS[m:m+1] for m in j])
			nk+=a[e]
			j[-1]+=1
			for m in range(len(j)-1,-1,-1):
				if (j[m]==len(LETTERS)):
					j[m]=0
					if (m==0):
						j.insert(0,0)
					else:
						j[m-1]+=1
				else:
					break
		nk+=b")"
		v=IDENTIFIER_REGEX.sub(lambda m:(a[m.group(0)] if m.group(0) in a else m.group(0)),HEX_NUMBER_REGEX.sub(lambda m:bytes(str(int(m.group(0),16)),"utf-8"),_expand_macros(v,dm,dfm)))
		d_f[i]=(nk,v)
		d_s+=b"\n#define "+nk+b" "+v.strip()
	fl=[]
	for k in l:
		k=_expand_macros(k,dm,dfm)
		if (len(k)==0):
			continue
		if (b"(" in k and b"(*" not in SPACE_CHARACTERS_REGEX.sub(b"",k) and k.count(b"(")==k.count(b")") and k.count(b"{")==k.count(b"}")):
			fl.append((k[:-len(k.split(b"(")[-1])-1].split(b" ")[-1],k.strip()))
			continue
		o+=b"\n"+k.strip()
	i=0
	while (i<len(o)):
		if (i<len(o)-7 and o[i:i+1] not in IDENTIFIER_CHARACTERS and o[i+1:i+6]==b"union" and o[i+6:i+7] not in IDENTIFIER_CHARACTERS):
			i+=6
			while (o[i] not in b";{"):
				i+=1
			if (o[i:i+1]==b"{"):
				b=1
				i+=1
				while (b!=0):
					if (o[i:i+1]==b"{"):
						b+=1
					elif (o[i:i+1]==b"}"):
						b-=1
					else:
						j=i
						while (i<len(o) and o[i] in SPACE_CHARACTERS):
							i+=1
						if (i!=j):
							o=o[:j]+(b"" if o[j-1] not in IDENTIFIER_CHARACTERS else b" ")+o[i:]
							i-=i-j+(0 if o[j-1] not in IDENTIFIER_CHARACTERS else -1)
							continue
					i+=1
		elif (i<len(o)-8 and o[i:i+1] not in IDENTIFIER_CHARACTERS and o[i+1:i+7]==b"struct" and o[i+7:i+8] not in IDENTIFIER_CHARACTERS):
			i+=7
			while (o[i] not in b";{"):
				i+=1
			if (o[i:i+1]==b"{"):
				b=1
				i+=1
				while (b!=0):
					if (o[i:i+1]==b"{"):
						b+=1
					elif (o[i:i+1]==b"}"):
						b-=1
					else:
						j=i
						while (i<len(o) and o[i] in SPACE_CHARACTERS):
							i+=1
						if (i!=j):
							o=o[:j]+(b"" if o[j-1] not in IDENTIFIER_CHARACTERS else b" ")+o[i:]
							i-=i-j+(0 if o[j-1] not in IDENTIFIER_CHARACTERS else -1)
							continue
					i+=1
		i+=1
	for k,v in sorted(fl,key=lambda e:e[0]):
		if (len(o)>0):
			o+=b"\n"
		ai=len(v.split(b"(")[-1])
		o+=v[:-ai]
		st=True
		void=True
		for a in v[-ai:].split(b")")[0].split(b","):
			if (st is False):
				o+=b","
			st=False
			o+=a[:-len(a.split(b" ")[-1])].strip()
			if (len(a[:-len(a.split(b" ")[-1])].strip())>0):
				void=False
		if (void):
			o+=b"void"
		o+=b");"
	return il+d_s+b"\n"+o.strip()
