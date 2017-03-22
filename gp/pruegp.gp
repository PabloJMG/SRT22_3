#!/usr/bin/gnuplot --persist

NumRay=$0
nombre_file="/home/pablo/Resultados_C/LFT/res/loquesea.txt"
unset key
plot for[i=0:NumRay] "<(sed -n 'i*3, (i+1)*3p' nombre_file)" u 1:2 w lines




