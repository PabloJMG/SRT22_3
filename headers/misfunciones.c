void saltar_com(FILE *fin) /* En archivo de texto salta a la siguiente línea tras \n*/
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



void guarda_resultados(int *arrayn, int npi,  FILE* finp)
{
int cont;
int res;



for(cont=0; cont<npi; cont++)
{
res=*(arrayn + cont);
fprintf(finp, "%d\n",  res);


}

}
