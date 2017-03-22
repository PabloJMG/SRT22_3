#!/usr/bin/gnuplot --persist

nombre_file(n)=sprintf("/home/pablo/Resultados_C/LFT/res/FP_%d.txt", n)
NumRay=$0

unset key
plot for[i=0:NumRay] nombre_file(i) u 1:2 w lines




