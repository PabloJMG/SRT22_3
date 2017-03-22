#!/usr/bin/gnuplot --persist

NumRay=$0
nombre_file='/home/pablo/Resultados_C/LFT/hand/res/resultadosFPredux.txt'
stats nombre_file using 0 nooutput
unset key
plot for[cont=0:(STATS_blocks-1)] nombre_file every :3 using 1:2 index cont w lines





