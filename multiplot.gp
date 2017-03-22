set term postscript
set output "img/ComparacionSART_Objeto.ps"
set multiplot layout 2, 1 title "Comparación objeto reconstrucción"
set size square
set title "SART"
unset key
set xrange[0:65]
set yrange[0:65]
set palette negative gray
plot "res/Resultados_SART/Resultados_SART_6.txt" w image

set title "Matriz_objeto"
unset key
set xrange[0:200]
set yrange[0:200]
set palette gray
plot "res/matriz_objeto.txt" w image

