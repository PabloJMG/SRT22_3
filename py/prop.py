import numpy as np
import math as ma
import random as rnd

#Idea, en lugar de un if por cada paso hacer los ifs y comprobar luego
def propa(**param, cont, objeto )
	arpi=np.zeros(param["NumPix"])
	dc2=param["dx"]
	sinang=ma.sin(cont*param["dang"])
	cosang=ma.cos(cont*param["dang"])	
	for cont1 in range(param["NRay"]):
		amax=param["amax"]
		ang=-amax+2*amax*rnd.randint(0,1)
		pos=param["meshsize"]*param["fdim"]/2-param["srcsize"]+2*param["srcsize"]*rnd.randint(0,1)
		pos+=ang*param["d_src_mesh"]
		
		for cont2 in range(param["Niter"]): #Ni idea de si existe Niter ahora
			pos+=ang*dc2
			pix=rint(param["dimx"]/2-cont2*dc2-1)
			piy=rint(((pos-param["meshsize"]*param["fdim"]/2))/param["dy"])	
			pix2 = pix*cosang+piy*sinang
			piy2 = piy*cosang-pix*sinang
			pix=pix2+param["dimx"]*param["fdim"]/2
			piy=piy2+param["dimy"]*param["fdim"]/2
			mu=objeto[pix, piy]
			mutot+=mu/param["Niter"]
			#~ if(randint(0,1)>ma.exp(-mu*data["dx"])):
				#~ break
		if(randint(0,1)>ma.exp(-mu*param["dx"])
			npix=rint((pos-param["dimx"]*param["fdim"]/2+param["detsize"]/2)/param["deltaP"]) #deltaP no existe
			if(npix>0 and npix<param["NumPix"])
				arpi[npix]+=1
				
	return arpi
	
