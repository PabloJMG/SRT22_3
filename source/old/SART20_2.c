
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <sys/types.h>
#include <unistd.h>
#include <fenv.h>
#warning "Error de compilación"

#define PATH "/home/pablo/Resultados_C/LFT"
#define pi 3.141592
#define DOSPI
#define COEF_COM

void *carga_archivo(int NumAng, int NumPix, float **b);

FILE *apertura_archivo(int contador);
FILE *apertura_archivo2(int cont1, int cont2);
void saltar_com(FILE *fin);
struct nonzer{
	
	int nonnum;
	int **pixeles;
	float *as; /*Necesario esto --- se podria liberar toda la matriz a*/
	
};
#ifdef COEF_COM
struct coef{
	
	float **a;
	
};
#endif

#ifdef COEF_INT
struct coef{
	
int  **a;
	
};
#endif
/*Vas a tener que cambiar todo por el factordim  ---- DONE*/ 

/* Cambiar el modo de guardar los pixeles distintos de cero, o al menos revisarlo*/
int main(int argc, char **argv)
{

int NumPix, cont, cont2, cont3, cont4, cont5, cont6, dimx, dimy, pixx, pixy, NumAng, i, valu, dimtotal, NumRayos, nth, limx, limy, con2, con3, pox, poy;
float beta, cosbeta, factora, sinbeta, deltaang, lambda, deltax, deltay, deltaf, posx, posy, posx2, posy2, val, diferencia, cant, sumpix, factorpos, prue1, prue2;
double angulos[10];
char filename[100];
FILE *finp;
clock_t inicio, final;
feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
int factordim=2;
factora=0.02;
//~ dimx=dimy=64;
finp=fopen("input/dimSART.txt", "r");
fscanf(finp, "%d", &dimx);
fscanf(finp, "%d", &dimy);
fclose(finp);
int aint[3];
lambda=1; /*Parámetro de corrección*/
finp=fopen("input/inputint.txt", "r");
i=0;

for(cont=0; cont < 3; cont++)
	{
		saltar_com(finp);		
	fscanf(finp, "%d", &aint[cont]);
	printf("%d\n", aint[cont]);
saltar_com(finp);
	}
fclose(finp);

#ifdef DOSPI
NumAng=aint[0];
#endif


#ifdef ABANICO /*Lo voy a dejar en version naif*/

finp=fopen("NumAngAbanico.txt", "r");
fscanf(finp, "%d", &NumAng);
fclose(finp);
finp=fopen("AngulosBundle.txt", "r");
for(cont=0; cont<NumAng; cont++)
{
	fscanf(finp, "%lf", &angulos[cont]);
	//~ printf("%lf\n", angulos[cont]);
	
}
fclose(finp);
//~ getchar();
#endif

NumPix=aint[2];
dimtotal=factordim*factordim*dimx*dimy;
NumRayos=NumAng*NumPix;
limx=factordim*dimx;
limy=factordim*dimy;
//~ struct nonzer{
	
	//~ int nonnum;
	//~ int pixeles[2][NumRayos];
	//~ float as[NumRayos]; /*Necesario esto --- se podria liberar toda la matriz a*/
	
//~ };
struct coef coe[NumAng][NumPix]; /*Necesario guardar todas las matrices de coeficientes o con el struct de abajo basta ¿? */
struct nonzer nonum[NumAng][NumPix];


for(cont=0; cont<NumAng; cont++)
{
	
	for(cont2=0; cont2<NumPix; cont2++)
	{
		
		coe[cont][cont2].a=malloc(limx*sizeof(float));
		for(cont3=0; cont3<limx; cont3++)
		{
			
			coe[cont][cont2].a[cont3]=malloc(limy*sizeof(float));
			for(cont4=0; cont4<limy; cont4++)
			{
				coe[cont][cont2].a[cont3][cont4]=0;
				
			}
		}
		
	}
	
}
factorpos=(float)dimy/NumPix;  /* factordim incluido, necesario ¿?*/
printf("factorpos=%f\n", factorpos);
//~ getchar();
		 //~ float *a=calloc(factordim*factordim*dimx*dimy*NumPix*NumAng, sizeof(float));
		 //~ float *b = callocc(NumPix*NumAng, sizeof(float)); /*cambiado hoy */
		 
float **b, **betar, **x;


//Reserva de memoria de b
b=calloc(NumAng, sizeof(float));
for(cont=0; cont<NumAng; cont++){
	b[cont]=calloc(NumPix, sizeof(*b));	
    }
//~ carga_archivo(NumAng, NumPix, b);							
for(cont=0; cont<NumAng; cont++){
	for(cont2=0; cont2<NumPix; cont2++)
	printf("b[%d][%d]= %f", cont, cont2, b[cont][cont2]);
}
printf("b=%f\n", b[6][6]);
getchar();
carga_archivo(NumAng, NumPix, b);	

//Reserva de memoria de las betas
betar=calloc(NumAng, sizeof(float));

for(cont=0; cont<NumAng; cont++){
								
	betar[cont]=calloc(NumPix, sizeof(*betar));
	}
							
//Reserva de memoria de las xs							
x=calloc(limx, sizeof(x));

for(cont2=0; cont2<limx; cont2++) {
	
	x[cont2]=calloc(limy, sizeof(*x));
	
}

struct coef gamma[NumAng];

				for(cont=0; cont<NumAng; cont++){
									
									gamma[cont].a=calloc(limx, sizeof(*gamma));
									for(cont2=0; cont2<limx; cont2++) {										
										gamma[cont].a[cont2]=calloc(limy, sizeof(*gamma[cont].a));	/* Sumatorio de los coeficientes a_ij   */										
										}
				} 

  deltaang=2*pi/NumAng;


for(cont=0; cont<limx; cont++)
{
	for(cont2=0; cont2<limy; cont2++)
	{x[cont][cont2]=0.;
printf("x[%d][%d] = %f\n", cont, cont2, x[cont][cont2]);
}
	
}
getchar();
/* Alocacion de espacio para pixeles.nonum y as.nonum  */

		
		for(cont=0; cont<NumAng; cont++)
		{
			
			for(cont2=0; cont2<NumPix; cont2++)
			
			{
				 nonum[cont][cont2].as=malloc(dimtotal*sizeof(float));
				 nonum[cont][cont2].pixeles=malloc(2*sizeof(int *));
				 for(cont3=0; cont3<2; cont3++)
				 
				 nonum[cont][cont2].pixeles[cont3]=malloc(dimtotal*sizeof(int));
				
			}
		}
////~ #ifdef COEF_INT /*No tocada mucho, no es la que suelo usar */
////~ for(cont=0; cont<NumAng; cont++)
////~ {	
	 ////~ #ifdef DOSPI 
	 ////~ beta=cont*deltaang;
	 ////~ #endif
	 
	  ////~ #ifdef ABANICO
	 ////~ beta=angulos[cont];
	 ////~ #endif
		 ////~ cosbeta=cos(beta);
	 ////~ sinbeta=sin(beta);
	////~ for(cont2=0; cont2<NumPix; cont2++)
	////~ {
		////~ finp=apertura_archivo2(cont, cont2);
		////~ nth=0;
		////~ for(cont3=0; cont3< dimx; cont3++)
		////~ {
			  //posx=dimx-cont3;	
			  ////~ posx=dimx/2 -cont3;
			 ////~ posy=dimy/2-cont2*factorpos; /*Error aqui*/
			 //printf("posyprevia=%f, cont2=%d, dimy=%d, NumPix=%d\n",posy, cont2, dimy,NumPix);
			 //posx2=posx-factordim*dimx/2;
			 //posy2=posy-factordim*dimy/2;
			 ////~ posx=posx2*cosbeta + posy2*sinbeta; /*inventada a toda prisa*/
			 ////~ posy=posy2*cosbeta - posx2*sinbeta;
			 ////~ posx=posx+factordim*dimx/2;
			 ////~ posy=posy+factordim*dimy/2;
			 ////~ pixx=trunc(posx);
			 ////~ pixy=trunc(posy);
			
			////~ coe[cont][cont2].a[pixx][pixy]+=1;
			 ////~ for(cont4=0; cont4<dimy; cont4++)
			 ////~ {
			
							 ////~ if(coe[cont][cont2].a[cont3][cont4]!=0) /* Se tiene que cambiar */
							 ////~ {

								 	////~ nonum[cont][cont2].pixeles[0][nth]=pixx;
									////~ nonum[cont][cont2].pixeles[1][nth]=pixy;
									////~ nonum[cont][cont2].as[nth]=coe[cont][cont2].a[cont3][cont4];
									////~ nth+=1;
									////~ fprintf(finp, "%d %d %lf\n", cont3, cont4, coe[cont][cont2][cont3][cont4]);
							 ////~ }
		 ////~ }
		 //getchar();
		 		 //nonum[cont][cont2].nonnum= nth;
		 		 //printf("nth=%d\n", nth);
		 ////~ fprintf(finp,"\n");
		 //getchar();
		////~ }
		 ////~ nonum[cont][cont2].nonnum= nth;
		 		 //printf("nth=%d\n", nth);
		 		 //getchar();
		////~ fclose(finp);
	////~ }
	
	
////~ }

////~ #endif


#ifdef COEF_COM
 for(cont=0; cont<NumAng; cont++) /*inicializar la matriz de coeficientes a*/
 { 		
	 #ifdef DOSPI 
	 beta=cont*deltaang;
	 #endif
	 
	 #ifdef ABANICO
	 beta=angulos[cont];
	 #endif
	 cosbeta=cos(beta);
	 sinbeta=sin(beta);
	 for(cont2=0; cont2<NumPix; cont2++){
			finp=apertura_archivo2(cont, cont2);
			nth=0;
						 for(cont3=0; cont3<dimx; cont3++) 
						 {	
							 //~ for(cont4=0; cont4<(dimy-1); cont4++)
							 //~ {
								posx=dimx/2-cont3;	
								posy=dimy/2 -cont2*factorpos; /*Error aqui*/
								posx2=posx;
								posy2=posy;
							 //~ posx2=posx-factordim*dimx/2;
							 //~ posy2=posy-factordim*dimy/2;
								 posx=posx2*cosbeta - posy2*sinbeta; /*inventada a toda prisa*/
								 posy=posy2*cosbeta + posx2*sinbeta;
								 posx+=factordim*dimx/2;
								 posy+=factordim*dimy/2;
								 //~ posy=2.0;
								 pixx=trunc(posx);
								 pixy=trunc(posy);
								 deltax=posx-pixx;
								 deltay=posy-pixy;
													
								 coe[cont][cont2].a[pixx][pixy]+=(2-deltax-deltay)/factora;
								 coe[cont][cont2].a[pixx][pixy+1]+=(1-deltax+deltay)/factora;
								 coe[cont][cont2].a[pixx+1][pixy]+=(1-deltay+deltax)/factora;
								 coe[cont][cont2].a[pixx+1][pixy+1]+=(deltax+deltay)/factora;
							 		//~ printf("posx=%f, posy=%f, pixx = %d, pixy=%d", posx, posy, pixx, pixy);
							
						
							 for(cont4=0; cont4<dimy; cont4++)
							 {
							fprintf(finp,  "%d %d %lf\n", cont3, cont4, coe[cont][cont2].a[cont3][cont4]);
							posx=dimx/2-cont3;
							prue1=posx;
							posy=dimy/2-cont4;
							posx2=posx;
							posx=posx2*cosbeta-posy*sinbeta;
							posy=posy*cosbeta + posx2*sinbeta;
							prue2=posx;
							pox=trunc(posx+factordim*dimx/2);
							poy=trunc(posy+factordim*dimy/2);
							//~ fprintf(finp,  "%d %d %lf posx %d, posy %d\n", cont3, cont4, coe[cont][cont2].a[cont3][cont4], pox, poy);
							printf( "NumAng =%d dimx=%d dimy=%d, posx %d, posy %d, pox antes = %f, pox durante =%f, cos=%f, sin=%f\n",cont,  cont3, cont4, pox, poy, prue1, prue2, cosbeta, sinbeta);
							if(coe[cont][cont2].a[pox][poy]>0.00001) /* Esto está mal, no se cumple, pero bueno. Igual si que se cumple, fijate tu*/
							{
										nonum[cont][cont2].pixeles[0][nth]=pox;
										nonum[cont][cont2].pixeles[1][nth]=poy;
										nonum[cont][cont2].as[nth]=coe[cont][cont2].a[pox][poy];
										nth+=1;
							
							printf("NumAng =%d, numpix = %d, x =%d, pixx = %d, pox=%d, y=%d,  pixy =%d, poy= %d coe =%f \n", cont, cont2, cont3, pixx,  pox, cont4, pixy, poy, coe[cont][cont2].a[pox][poy]);
							//~ getchar();	
							}
							 //~ getchar();
						 }
						 fprintf(finp,"\n");
						 //~ getchar();
						 
					 }
					 nonum[cont][cont2].nonnum= nth;		
					 printf("nth=%d\n", nth);
	fclose(finp);
	
	 }
	 
	 
 }
 #endif
 //~ getchar();
 for(cont=0; cont<NumAng; cont++)
 {
	 for(cont2=0; cont2<NumPix; cont2++)
	 { finp=apertura_archivo2(cont, cont2);
		 for(cont3=0; cont3<limx; cont3++)
		 {
			 for(cont4=0; cont4 < limy; cont4++)
			 {
				 
				 fprintf(finp, "%d %d %f\n", cont3, cont4, coe[cont][cont2].a[cont3][cont4]);
				 
			 }
		//~ fprintf(finp, "\n");
		
		 }
		 fclose(finp);
	 }
 }
/*Carga los resultados de las proyecciones desde los archivos a un array*/
 //~ carga_archivo(NumAng, NumPix, b);
/* Se calculan las cantidades beta y gamma */
//~ printf("comienza iteración, presionar tecla\n");
//~ getchar();
inicio=clock();
printf("%d\n", sizeof(**betar)/sizeof(betar[0][0]));
getchar();
for(cont=0; cont<NumAng; cont++)
 {
		for(cont2=0; cont2<NumPix; cont2++)
		{
			for(cont3=(factordim-1)*dimx/2; cont3<(factordim+1)*dimx/2; cont3++)
			{
				for(cont4=(factordim-1)*dimy/2; cont4<(factordim+1)*dimy/2; cont4++)
				{
				
							betar[cont][cont2]+=(coe[cont][cont2].a[cont3][cont4]);
									
				}
			}
			printf("Beta = %f ind1 = %d ind2 = %d\n", betar[cont][cont2], cont, cont2);
			if(betar[cont][cont2]<0.001)
			printf("indices de beta que ha dado cero %d %d\n", cont, cont2);
		}	
	 
	 
 }
printf("final de las betas\n");
//~ getchar();
 for(cont=0; cont<NumAng; cont++)
	{
		for(cont2=(factordim-1)*dimx/2; cont2<(factordim+1)*dimx/2; cont2++)
		{
			for(cont3=(factordim-1)*dimy/2; cont3<(factordim+1)*dimy/2; cont3++)
			
			{
				for(cont4=0; cont4<NumPix; cont4++)
				{	
					gamma[cont].a[cont2][cont3]+=(coe[cont][cont4].a[cont2][cont3]); /*Esta bien esta inicializacion¿? */
					printf("%f\n", coe[cont][cont4].a[cont2][cont3]);
					
				}
				
				printf("\n\n");
			//~ printf("gamma=%lf, cont=%d, cont2=%d, cont3=%d\n", *(gamma+cont*dimtotal+cont2*dimy+cont3), cont, cont2, cont3 );
			printf("gamma = %f, ind1 = %d, ind2 = %d, ind3 = %d\n", gamma[cont].a[cont2][cont3], cont, cont2, cont3);
			if(gamma[cont].a[cont2][cont3]<0.0001)
			printf("indices de gamma que ha dado cero %d %d %d y gamma %f\n", cont, cont2, cont3, gamma[cont].a[cont2][cont3]);
			}
			
		}
		
	}
	printf("End of gammas, ahora hay un getchar()");
getchar();
i=0;
deltaf=1;

for(cont=0; cont<limx; cont++)
{
	for(cont2=0; cont2<limy; cont2++)
	printf("x[%d][%d] = %f\n", cont, cont2, x[cont][cont2]);
	x[cont][cont2]=0.f;
}

printf("comienza iteración\n");
getchar();
  while((deltaf>0.002) && (i < 10))
 {
 deltaf=0;
 sumpix=0;
	 for(cont=0; cont<NumAng; cont++)
	 {	
		 
		 //~ #pragma omp parallel
		 {
		 
			 //~ #pragma omp for private(cont2, cont3, cont4, cont5, cont6)
		for(cont2=(factordim-1)*dimx/2; cont2<(factordim+1)*dimx/2; cont2++) /*Cambiados limites aqui */
		{//~ printf("cont2=%d, numhilo=%d\n", cont2, omp_get_thread_num());
			for(cont3=(factordim-1)*dimy/2; cont3<(factordim+1)*dimy/2; cont3++)
			{		if(gamma[cont].a[cont2][cont3] != 0) /* Este if es conflictivo y creo q sobra según la definición de gamma */
				
				{
				for(cont4=0; cont4 < NumPix; cont4++)
				{
					cant=0;
				
				for(cont5=0; cont5<(nonum[cont][cont4].nonnum); cont5++)
				{
					
					//~ cant+=nonum[cont][cont4].as[cont5]*(*(x+(nonum[cont][cont4].pixeles[0][cont5])*dimy+nonum[cont][cont4].pixeles[1][cont5])); /*  sum_{i=0}^{N} a_{ij}*x_{i}  ¿?*/
						cant+=nonum[cont][cont4].as[cont5]*(x[nonum[cont][cont4].pixeles[0][cont5]][nonum[cont][cont4].pixeles[1][cont5]]);
						printf("cant = %f, as = %f, x = %f, pix =%d, pixy=%d iteracion nº %d\n", cant, nonum[cont][cont4].as[cont5], x[nonum[cont][cont4].pixeles[0][cont5]][nonum[cont][cont4].pixeles[1][cont5]], nonum[cont][cont4].pixeles[0][cont5], nonum[cont][cont4].pixeles[1][cont5], i);//~ printf("Suma a cantidad = %lf mult por %lf\n", nonum[cont][cont4].as[cont5], (x[nonum[cont][cont4].pixeles[0][cont5]][nonum[cont][cont4].pixeles[1][cont5]]) );	
				}
				
		
			
				diferencia+=(b[cont][cont4]-cant)*((coe[cont][cont4].a[cont3][cont2]))/(betar[cont][cont4]); /* Aquí mirar  ---- (b-a*x)*a_{j}/b_j */
				printf("cant = %f, ang=%d, numpix =%d, x, y = %d, %d, diferencia =%f, b=%d\n\n", cant, cont, cont4, cont2, cont3, diferencia, b[cont][cont4]);
			} /* Final del bucle que recorre todos los detectores  */
		
			printf("Update del pixel es=%f, diferencia = %f, gamma =%f, x, y =%d %d", lambda*diferencia/gamma[cont].a[cont2][cont3], diferencia, gamma[cont].a[cont2][cont3], cont2, cont3);
			x[cont2][cont3]+=lambda*diferencia/(gamma[cont].a[cont2][cont3]); /* update de un pixel */
			deltaf+=diferencia;
			sumpix+=x[cont2][cont3];
			getchar();
		}
			//~ diferencia=0;
			
		
		
		diferencia=0;
		}	/*Final bucle dimy*/
	 //~ getchar();	
	}
}/*Final de parte paralela - aqui he quitado una llave*/

	
 }/*Final de un angulo*/
  deltaf=deltaf/sumpix;
 deltaf=fabs(deltaf);
 /* arg_{min}   */

	 printf("Deltaf=%lf y n de iteraciones =%d\n", deltaf, i);
	 sprintf(filename, "res/Resultados_SART/Resultados_SART_%d.txt", i);
	 
	 finp=fopen(filename, "w+");

for(cont=0; cont<limy; cont++)
{
	
	for(cont2=0; cont2<limx; cont2++)
	{
		
		
		fprintf(finp, "%d %d %f\n",cont, cont2, x[cont][cont2]); 
	}
	
}

fclose(finp);
	 //~ getchar();
 i+=1;
}
//~ final=clock();
//~ printf("Tardo %lf segundos para %d angulos, %d pixeles y tamaño %d x %d\n", (double)(final-inicio)/CLOCKS_PER_SEC, NumAng, NumPix, dimx, dimy);
//~ finp=fopen("Resultados_SART.txt", "w+");

//~ for(cont=0; cont<dimy; cont++)
//~ {
	
	//~ for(cont2=0; cont2<dimx; cont2++)
	//~ {
		
		
		//~ fprintf(finp, "%f   ", *(x+dimx*cont+cont2)); 
	//~ }
	//~ fprintf(finp, "\n");
//~ }
//~ *(g+m)+=*(a+m*dimI+i)*(*(p+i)-(*(at+m*dimI+i)*(*(g+ m*dimI+i)))/((*(at+m*dimI+i))*(*(a+m*dimI+i)));
//~ fclose(finp);
//~ free(b);
//~ free(x);
/* Liberar memoria*/
FILE *pipeplot=popen("gnuplot -persist", "w");
/*fprintf(pipeplot, "set title /lo que sea/"   */
//~ fprintf(pipeplot, "set pm3d map\n");
//~ fprintf(pipeplot, "set size square 1, 1\n");
fprintf(pipeplot, "plot '/home/pablo/Resultados_C/LFT/res/Resultados_SART/Resultados_SART_%d.txt'w image \n",(i-1));

//~ fprintf(pipeplot, "set term png\n");
//~ fprintf(pipeplot, "set output 'BP_Imagen_Angulos%d_Rayos%d.png'\n", NumAng, ray);
//~ fprintf(pipeplot, "replot\n");
fflush(pipeplot);
fprintf(pipeplot, "quit\n");
pclose(pipeplot);

return 0;
}


void *carga_archivo(int NumAng, int NumPix, float **b)
{
	int cont, cont2, valu;
	
	FILE *finp;
	
 for(cont=0; cont<NumAng; cont++) /*  Cargar b (esto está mal)*/
 {
	 	
	 finp=apertura_archivo(cont);
 printf("Dentro de carga_archivo %f, %d", b[6][0], sizeof(**b));
	 for(cont2=0; cont2<NumPix; cont2++)
	 {
	printf("cont =%d, cont2=%d, b[]=%f,valu = %d\n",cont, cont2, b[cont][cont2], valu);
	fscanf(finp, "%d", &valu);
	
	b[cont][cont2]=log(1+valu); /*Esto aquí lo he cambiado*/
	
	printf("val=%d, log(val)=%f, cont=%d, cont2=%d\n", valu, b[cont][cont2], cont, cont2);
	
	 }
	 
	 fclose(finp);
 }
	
	printf("Archivo cargado, presione alguna tecla\n");
	//~ getchar();
}

FILE *apertura_archivo(int contador)
{

char nombrearchivo[25]="Resultados_pixeles.txt";

sprintf(nombrearchivo, "res/angulo_%d.txt", contador);

if(fopen(nombrearchivo, "r")==NULL)
{
	perror("El archivo no existe o no se puede abrir\n");
}

return fopen(nombrearchivo, "r");



}

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

FILE *apertura_archivo2(int cont1, int cont2)
{
	
	char file_name[40];
	sprintf(file_name, "res/Resultados_SART/Matriz_coef_SART_%d_%d.txt", cont1, cont2);
	
	
	
	return fopen(file_name, "w+");
}

//~ void guarda_matriz_coeficientes(int numang, int numpi, int lix, int liy, void *loq)
//~ {
	
	//~ FILE *fin;
	//~ loq=(struct) coef;
	//~ int cont, cont2, cont3, cont4;
	
	//~ for(cont=0; cont<numang; cont++)
	//~ {
		
		//~ for(cont2=0; cont2<numpi; cont2++)
		//~ {
			//~ fin=apertura_archivo2(cont, cont2);
			
			//~ for(cont3=0; cont3 <lix; cont3++)
			//~ {
				
				//~ for(cont4=0; cont4<limy; cont4++)
				
				//~ {
					
					//~ fprintf(fin, "%d %d %f\n", cont3, cont4, mfeÇ);
					
				//~ }
				
			//~ }
			
		//~ }
		
	//~ }
	
	
	
	
//~ }