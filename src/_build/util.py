import subprocess
import sys



def wrap_output(a,pfx=b"    "):
	sys.stdout.flush()
	p=subprocess.Popen(a,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
	st=False
	for c in iter(lambda:p.stdout.read(1),b""):
		if (not st):
			st=True
			sys.stdout.buffer.write(pfx)
		sys.stdout.buffer.write(c)
		if (c==b"\n"):
			st=False
			sys.stdout.buffer.flush()
	p.wait()
	return p
