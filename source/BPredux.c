#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PATH "/home/pablo/Resultados_C/LFT/"

int main(int argc, char **argv[])
{

FILE *fin;
int i;

int *datadet=malloc(NumDet*sizeof(*int));
char nomfic[60];

sprintf(nomfic, "%cres/loquesea.txt", PATH);
/* Cargar archivo */
fin=fopen(nomfic, "r");
for(i=0; i<NumDet; i++)
{
	fscanf(fin, "%d", &datadet[i]);
	
	
	
	
}

fclose(fin);


/* ordenar los ray y definir ray-path */





return 0;

}
