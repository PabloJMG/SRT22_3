import math as ma
import numpy as np
import random as rnd
#~ from scipy.sparse import sparse_cscmatrix
NumAng=5
NumPix=10
dim=64

def abrir_archivo(con):
	filename = 'respy/angulo_%d.txt' %(con)
	return open(filename, "r")
b=[]
for con1 in range(NumAng):
	finp=abrir_archivo(con1)
	
	c=finp.readlines()
	
	c=(x.strip() for x in c)
	b.append(c)
	
	#~ print b

for x in range(len(b)):
	b[x]=tuple(b[x])

#~ [i[0] for i in b]	

b=list(sum(b,()))
	
#~ b=tuple(b)
print 'B es =', b, b[11]
