
#include <stdio.h>
#include <stdlib.h>




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


int main(int argc, char **argv[])
{

FILE *finp;
int numero;
finp=fopen("input/inputintc.txt", "r");

saltar_com(finp);
fscanf(finp, "%d", &numero);
printf("%d\n", numero);
getchar();
saltar_com(finp);
saltar_com(finp);
fscanf(finp, "%d", &numero);
printf("%d\n", numero);
getchar();
saltar_com(finp);
saltar_com(finp);
fscanf(finp, "%d", &numero);
printf("%d\n", numero);


return 0;
}
