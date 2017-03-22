#include <stdio.h>
#include <stdlib.h>

#define PATH "/home/pablo/Resultados_C/LFT/"

/*Sin acabar*/

void saltar_com(FILE *fin);


int main(int argc, char **argv[])
{


FILE *fin;
FILE *pipeplot;
 char nombrearchivo[50];
int NumAng, cont;
sprintf(nombrearchivo, "%sinput/inputintc.txt", PATH);



fin=fopen(nombrearchivo, "r+");
saltar_com(fin);
fscanf(fin, "%d", &NumAng);
fclose(fin);



pipeplot=popen("gnuplot -persist", "w");

for(cont=0; cont<NumAng; cont++)
{

sprintf(nombrearchivo, "%sres/FP_%d.txt", PATH, cont);
fin=fopen(nombrearchivo, "r");
if(fin==NULL)
{
else{
//~ pipeplot=popen("gnuplot -persist", "w");
fprintf(pipeplot, "plot %s u 1:2 w lines", nombrearchivo);
}
}


}















return 0;
}



void saltar_com(FILE *fin)
{
	char col;
	while(fscanf(fin, "%c", &col))
	{
		if(col=='\n')
		{
			
			break;
		}
		
	}
	
	
}
