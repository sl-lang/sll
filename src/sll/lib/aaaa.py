from math import *



X=1
delta=3
for i in range(0,1024):
	print(X,X+4,(X+4)**0.5)#(sqrt(19600+196*X)-140)/98)
	# 49mm+140m
	X+=delta
	delta+=2


m=106
X=64
