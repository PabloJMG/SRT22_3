#!/usr/bin/gnuplot --persist

nombre_file(n)=sprintf("/home/pablo/Resultados_C/LFT/res/Resultados_SART/Matriz_coef_SART_1_%d.txt", n)

unset key

plot for[i=0:50] nombre_file(i)  w image
