
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <omp.h>

/*Declaracion y definicion de parametros*/
struct param{

int NumAng;
int N;
int NumPix;
int *resultados; /*No especificar el tamaño e inicializarlo sin más */
double beta;
int contador;
int dix;
int diy;
double deltaPaso;
double deltaP;
double deltaX;
double deltaY;
int factordim;
double tam_main_lens; /*Tamaño de la lente principal*/
int div_main_lens; /*Num. de divisiones de la lente principal*/
int num_len; /*Num de lentillas*/
double dist_malla_mainlens; /*distancia malla - mainlens*/
double dist_mainlens_lens; 
double dist_lens_detector;
double anchura_malla; 
double angmax;
double **obj;
double delta_source;
double tam_lenslet;
double d; /*PRopagacion source - mesh */
};

#define PATH "/home/pablo/Resultados_C/LFT/"

/* Inputc.txt y inputdouc.txt, los dos comentados, sin NumAng, específicamente escritos para esto  */
#define PI 3.141592653
#define INPUT_TXT /*De momento no hay otra opcion*/
#define DEB_ANG
#define SQUARE
#define DOSPI
#define F_MLENS
#define F_LENSLET

/* Declaración de funciones  */
void propagacion(struct param targ);
void saltar_com(FILE *fin);
void guarda_resul(FILE *fin, double **resul, int filas, int columnas, double *dist);
FILE *apertura_archivo(void);
int main (int argc, char **argv)
{

int cont1, cont2, cont3, NumAng, dimtotal, a, b, limx, limy;
//~ int array_inp[10];
//~ double array_inp2[20];
double delta_ang, deltax, deltay, radio;
FILE *finp;
char filen[100];

#ifdef INPUT_ARG

#endif


#ifdef INPUT_TXT
//~ sprintf(filen, "%cinput/inputintc.txt", PATH);
finp=fopen("/home/pablo/Resultados_C/LFT/input/inputintc.txt", "r");
if(finp==NULL)
{
	printf("Error al abrir el archivo, compadre\n");
	return 1;
	
}
	saltar_com(finp);
fscanf(finp, "%d", &NumAng);
printf("NumAng = %d\n", NumAng);
struct param par[NumAng];
fclose(finp);

for(cont1=0; cont1<NumAng; cont1++) /* Carga de los archivos de input */
{
	finp=fopen("/home/pablo/Resultados_C/LFT/input/inputintc.txt", "r");
					saltar_com(finp);
				fscanf(finp, "%d", &par[cont1].NumAng); /*Este primer fscanf es sólo para que el puntero se desplace una posicion downward*/
					saltar_com(finp);
					saltar_com(finp);
				fscanf(finp, "%d", &par[cont1].N);
				printf("%d\n", par[cont1].N);
					saltar_com(finp);
					saltar_com(finp);
				fscanf(finp, "%d", &par[cont1].NumPix);
				printf("%d\n", par[cont1].NumPix);
					saltar_com(finp);
					saltar_com(finp);
				fscanf(finp, "%d", &par[cont1].factordim);
					saltar_com(finp);
					saltar_com(finp);
				fscanf(finp, "%d", &par[cont1].div_main_lens);
					saltar_com(finp);
					saltar_com(finp);
				fscanf(finp, "%d", &par[cont1].num_len);
				
	fclose(finp);
	
	finp=fopen("/home/pablo/Resultados_C/LFT/input/inputdouc.txt", "r");
					saltar_com(finp);
				fscanf(finp, "%lf", &par[cont1].anchura_malla);
					saltar_com(finp);
					saltar_com(finp);
				fscanf(finp, "%lf", &par[cont1].deltaX);
					saltar_com(finp);
					saltar_com(finp);
				fscanf(finp, "%lf", &par[cont1].deltaY);
					saltar_com(finp);
					saltar_com(finp);
				fscanf(finp, "%lf", &par[cont1].delta_source);
					saltar_com(finp);
					saltar_com(finp);
				fscanf(finp, "%lf", &par[cont1].d); /*Sin usar ¿?*/
					saltar_com(finp);
					saltar_com(finp);
				fscanf(finp, "%lf", &par[cont1].deltaPaso);
					saltar_com(finp);
					saltar_com(finp);
				fscanf(finp, "%lf", &par[cont1].tam_main_lens);
					saltar_com(finp);
					saltar_com(finp);
				fscanf(finp, "%lf", &par[cont1].tam_lenslet);
					saltar_com(finp);
					saltar_com(finp);
				fscanf(finp, "%lf", &par[cont1].dist_malla_mainlens);
					saltar_com(finp);
					saltar_com(finp);
				fscanf(finp, "%lf", &par[cont1].dist_mainlens_lens);
					saltar_com(finp);
					saltar_com(finp);
				fscanf(finp, "%lf", &par[cont1].dist_lens_detector);
	//~ fscanf(fin, "%lf", &par[cont1].d);
	fclose(finp);

	
	par[cont1].angmax=par[cont1].tam_main_lens/(2*(par[cont1].d + par[cont1].anchura_malla+par[cont1].dist_malla_mainlens));
	//~ par[cont1].resultados=crea_array_ent(par[cont1].NumPix); /*Necesario ¿?¿?*/
	par[cont1].dix=(int) par[cont1].anchura_malla/par[cont1].deltaX;
	par[cont1].diy=(int) par[cont1].anchura_malla/par[cont1].deltaY;
	par[cont1].deltaP=par[cont1].tam_lenslet/par[cont1].NumPix; /* Aquí suponemos que el tamaño de las lentillas es similar al tamaño del array de detectores */
}


#endif

delta_ang=2*PI/NumAng; /* Intervalo angular entre cada rayo*/

dimtotal=par[0].factordim*par[0].factordim*par[0].dix*par[0].diy;
limy=par[0].diy*par[0].factordim;
limx=par[0].dix*par[0].factordim;


double **objeto; /* Inicializar y reservar memoria para el objeto a escanear, en cada struct de los parámetros */
objeto=malloc(limx*sizeof(*objeto));
for(cont1=0; cont1<NumAng; cont1++)
{
	par[cont1].obj=malloc(limy*sizeof(*par[cont1].obj));
	
}

for(cont1=0; cont1<NumAng; cont1++)
{
	for(cont2=0; cont2<limy; cont2++)
	{
		par[cont1].obj[cont2]=malloc(limx*sizeof(*par[cont1].obj[cont2]));
		
	}
	
}

for(cont1=0; cont1<limy; cont1++)
{
	objeto[cont1]=malloc(limx*sizeof(*objeto[cont1]));
	
}

/* Parametros de los objetos regulares y simétricos a escanear */
deltax=par[0].factordim*par[0].dix/15;
deltay=par[0].factordim*par[0].diy/15;
radio=deltay/2;

				#ifdef SQUARE /* Definición de objeto cuadrado*/
			finp=fopen("/home/pablo/Resultados_C/LFT/res/matriz_objeto.txt", "w");
		
					for(a=0; a<limx; a++)
					{
								for(b=0; b<limy; b++)
								{		
										objeto[b][a]=2;
													if(a < par[0].factordim*par[0].dix/2 + deltax && a > par[0].factordim*par[0].dix/2 -deltax && 
													b < par[0].factordim*par[0].diy/2 +deltay && b > par[0].factordim*par[0].dix/2 - deltay) /*Aqui parentesis en las condiciones ¿? */
															objeto[b][a]=500;
													
										fprintf(finp, "%lf  ", objeto[b][a]);
							    }
					    fprintf(finp, "\n");
					}
			fclose(finp);
	
			#endif
			
			
			
			#ifdef SQUARE_IN_CIRCLE /* Definición de objeto circular macizo dentro de un cuadrado de menor densidad*/
			
		finp=fopen("/home/pablo/Resultados_C/LFT/res/matriz_objeto.txt", "w");
							for(a=0; a<(par[0].factordim*par[0].dix); a++)
								{
										for(b=0; b<limy; b++)
											{		
												objeto[b][a]=0.02;
												
													if(a < limx/2 + deltax && a > limx/2 -deltax && b < limy/2 +deltay && b > limy/2 - deltay) /*El objeto aquí está mal definido */
													objeto[b][a]=1;
													
													if(((a-par[0].factordim*par[0].dix/2)* (a-par[0].factordim*par[0].dix/2) + (b - par[0].factordim*par[0].diy/2)*(b - par[0].factordim*par[0].diy/2)) < (radio*radio))
													objeto[b][a]=5;
													
													fprintf(finp, "%lf  ", objeto[b][a]);
										    }
										    fprintf(finp, "\n");
								}
								
							fclose(finp);
						
			
			#endif
			
			#ifdef CIRCLE /*Definición de objeto circular*/
				finp=fopen("/home/pablo/Resultados_C/LFT/res/matriz_objeto.txt", "w");
				
				for(a=0; a<limx; a++)
				{
					for(b=0; b<limy; b++)
					{
						objeto[b][a]=0.02;
						if( ((a*a+b*b)< (radio*radio)))
						objeto[b][a]=1;
					}
					
					
				}
				fclose(finp);
			#endif


for(cont1=0; cont1<NumAng; cont1++)/* Copia el objeto en el objeto de cada struct---Esta asignación está mal seguro*/
{

	//~ memcpy(par[cont1].obj, objeto, sizeof(par[cont1].obj));
		for(a=0; a<limx; a++)
				{
					for(b=0; b<limy; b++)
					{
						par[cont1].obj[a][b]=objeto[a][b];
						//~ printf("objeto %f, objeto en struct %f\n", objeto[a][b], par[cont1].obj[a][b]);
						
					}
					
					
				}

}

//~ getchar();

		for(cont1=0; cont1<NumAng; cont1++)
		{
						par[cont1].contador=cont1;
						
						#ifdef DOSPI
						par[cont1].beta=cont1*delta_ang;
						#endif
						
						#ifdef ABANICO
						par[cont1].beta=angulos[cont1];
						#endif
						
						propagacion(par[cont1]); /* Propagación de todos los rayos para un mismo ángulo (dispuesto de esta forma para paralelizar la función propagación)*/
						
		}
	

return 0;

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

FILE *apertura_archivo(void)
{
	char namefile[60]="/home/pablo/Resultados_C/LFT/res/Result_LFT.txt";
	
	return fopen(namefile, "w");
	
	
}
void guarda_resul(FILE *fin, double **resul, int filas, int columnas, double *dist) /*  Repensar esta función*/
{
	int cont1, cont2;
	double pos;
	for(cont1=0; cont1<filas; cont1++)
	{
	
	for(cont2=0; cont2<columnas; cont2++)
	{
		pos=dist[cont2];
	if(resul[cont1][cont2] != -1)	
	fprintf(fin, "%lf %lf\n", pos, resul[cont1][cont2]);
		
		
	}	
		
	}
	
}
void propagacion(struct param targ) /* Pasarlo como referencia ¿?, en ese caso tendría que definir variables con ->*/
{
	
	FILE *fui, *fout;
	int i, j, pixu, pami, ind, Nhit, pi1, pi2, Niter, pixx, pixy, numout; 
	double cero;
	char nombre_archivo[100]="algo.txt";
	double num_rand, sinang, cosang, mu, betarand;
	int *pix = malloc(2*sizeof(*pix));
	int *pix2 = malloc(2*sizeof(*pix2));
	//~ num_det=(int)
	printf("Tamaño main lens=%lf\n", targ.tam_main_lens);
	printf("N en el bucle =%d\n", targ.N);
	double deltaP=targ.tam_lenslet/targ.NumPix;
	double *pos0 = malloc(2*sizeof(*pos0));
	double *pos1 = malloc(2*sizeof(*pos1));
	double distancia[2];
	int **resul;
		
		cosang=cos(targ.beta);
		sinang=sin(targ.beta);
		
		numout=0;
		pami=0;
		cero=0;	
		
		distancia[0]=targ.d+targ.dist_malla_mainlens+targ.anchura_malla; 
		distancia[1]=distancia[0]+targ.dist_mainlens_lens+targ.dist_lens_detector;	
		
		resul=malloc(targ.N*sizeof(resul)); /*Declaración, reserva de memoria e inicialización de la matriz resul tamaño  Nx2 (Num. de Rayos / angulo)  Primera col sería el punto de donde corta en la main lens, la segunda el píxel en el detector al que llega  */
	srand(time(NULL));	

	 for(i=0; i<targ.N; i++)
	 {
		 
		 resul[i]=malloc(2*sizeof(*resul[i]));
	 }
	
	
	for(i=0; i<targ.N; i++)
	{
		for(j=0; j<2; j++)
		{
			resul[i][j]=-1;
			
		}
		
	}

	Niter=(int)(targ.dix*targ.deltaX/targ.deltaPaso); /*Numero de pasos de propagación del rayo a través del objeto */
		fout=fopen("/home/pablo/Resultados_C/LFT/res/resultadosFPredux.txt", "w");
	for(i=0; i<targ.N; i++)
	{
	

	#ifdef RAND_ANG /* Cone-beam CT */
	num_rand=rand()/((double)RAND_MAX + 1);
	betarand=-targ.angmax+2*num_rand*targ.angmax; /* angmax está definido fuera de la función y es la limitación de la pupila de entrada (la main lens en este caso) */
	num_rand=rand()/((double)RAND_MAX + 1);
	*pos0=-targ.delta_source+targ.anchura_malla/2*targ.factordim+2*targ.delta_source*num_rand; /*Definicion de la posicion inicial para un fuente pseudopuntual (tiene una anchura de targ.delta_source (quizás guardar esto también en resul[][]) */
	//~ *pos0=0;
	*(pos0+1)=betarand;
	#endif
	
	
	#ifdef DEB_ANG /*  Parallel-beam CT */ /* Revisar  esto */
	num_rand=2*targ.angmax*i/targ.N;
	betarand=-targ.angmax+2*num_rand*targ.angmax;
	num_rand=0.5;
	*pos0=-targ.delta_source+targ.anchura_malla/2*targ.factordim+2*targ.delta_source*num_rand; /*  */
	*(pos0+1)=betarand;
	#endif
	
	*pos0+=*(pos0+1)*targ.d; /*Prop. desde la fuente al objeto */
	
	pixx=-1;
	pixy=-1;

			for(j=0; j<Niter; j++) /* Propagacion a lo largo de la malla */
			{
				*pos0+=*(pos0+1)*targ.deltaPaso; /* Propagacion deltapaso(=anchura_malla/Niter) */
				*pix=rint(targ.dix/2-j*targ.deltaPaso/targ.deltaX-1); /*Calculo del pixel en x al que llega el rayo si angulo de incidencia sobre el objeto es 0*/
				*(pix+1)=rint((*pos0-targ.factordim*targ.anchura_malla/2)/targ.deltaY);  /*Calculo del pixel en y al que llega */
				*pix2=*pix*cosang+*(pix+1)*sinang;    /* Rotación angulo beta */
				*(pix2+1)=*(pix+1)*cosang-*(pix)*sinang;
				*pix=*pix2+targ.factordim/2*targ.dix;
				*(pix+1)=*(pix2+1)+targ.factordim/2*targ.diy;
				
				pi1=*pix;
				pi2=*(pix +1);
				
				mu=targ.obj[pi2][pi1]; /*Coef. de atenuación lineal del píxel al que llega el rayo*/
				
						if(pixx!=*(pix) || pixy !=*(pix+1))
						{
							pixx=*pix;
							pixy=*(pix +1);
							num_rand=rand()/((double) RAND_MAX + 1);
							//~ printf("mu = %f, num_rand=%f\n", mu, num_rand);
									if(exp(-mu*targ.deltaPaso) < num_rand)
									{printf("Rayo no llego");
										pami+=1;
										break;
										
									}
							
						}
						
				}
				/* Fin de la propagación a través del objeto*/
				
				//~ getchar();
				
						if(j== (Niter )) /* Si se ha salido del break de exp(-mu*deltaPaso) no se entrará en este condicional*/
					{	//~  	sprintf(nombre_archivo, "/home/pablo/Resultados_C/LFT/res/FP_%d.txt", numout);
							//~ numout+=1;
							//~ fout=fopen(nombre_archivo, "w+");
							
						
							numout+=1;
							*pos0+=*(pos0+1)*targ.dist_malla_mainlens;  										/* Propagación hasta la main lens*/
							printf("Angulo antes = %lf\n", *(pos0+1) );							
							*(pos0+1)=-(*pos0-targ.tam_main_lens/2)/(distancia[0])+*(pos0+1); 	/* Cambio de ángulo debido al paso por lente fina ---- Tocar*/
						
							printf("Posicion =%lf, posiciones con cero en mitad de lente =%lf, Angulo despues = %lf, factor mult = %lf\n\n", *(pos0),(*pos0-1.5),*(pos0+1), targ.anchura_malla*targ.factordim+targ.tam_main_lens);
							
							pixu=rint((*pos0+targ.anchura_malla*targ.factordim/2-targ.tam_main_lens/2)*targ.div_main_lens/targ.tam_main_lens); 							/* División de la lente a la que llega  !!!!!! mover *pos0 ¿?*/ 
							printf("La posicion = %lf, pixel = %d de %d pixeles\n\n", *pos0, pixu, targ.div_main_lens);
							//~ getchar();
							#ifdef F_MLENS
							*pos0=pixu*targ.tam_main_lens/targ.div_main_lens;
							#endif
							//~ resul[i][0]=pixu*targ.tam_main_lens/targ.div_main_lens; 						/* Se guarda el punto de la main lens donde llega */
							resul[i][0]=pixu;
							fprintf(fout, "%lf %lf\n", cero, targ.anchura_malla*targ.factordim/2); 
							fprintf(fout, "%lf %lf\n", distancia[0], *pos0); 									/*  Echar un ojo a esto */
							
							printf("%lf %lf\n", cero, targ.anchura_malla*targ.factordim/2);
							printf("%lf %lf\n", distancia[0], *pos0);
							*pos0+=*(pos0+1)*targ.dist_mainlens_lens; 											/*Propagación hasta el sistema de lentes*/
						
											/* Aquí el angulo cambia o no? (si se cumple el ifdef, es como si incidiera con angulo 0)*/
											
							pixu=rint((*pos0+targ.anchura_malla*targ.factordim/2-targ.tam_lenslet/2)/targ.tam_lenslet*targ.num_len);									/*Calcular aqui la lente a la que llega (de las lenslet). !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  mover pos0¿?*/
							#ifdef F_LENSLET
							*pos0=pixu*targ.tam_main_lens/targ.num_len; 										/* pos0 se fuerza para que pase por el centro de la lentilla*/
							#endif
							*pos0+=*(pos0+1)*targ.dist_lens_detector;											/*  Propagacion hasta el detector*/
							pixu=rint((*pos0)/deltaP); 															/*Calculo del detector al que llega*/
							resul[i][1]=pixu; 																	/* Guarda el pixel al que llega el rayo en resul*/
							fprintf(fout, "%lf %lf\n\n", distancia[1], *pos0);
							printf("%lf %lf %d\n\n", distancia[1], *pos0, pixu);
											
		
							ind+=1;
							Nhit+=1;
						}
		
		}
	fclose(fout);

				//~ fui=apertura_archivo();
				//~ guarda_resul(fui, resul, targ.N, 2, distancia);
				//~ fclose(fui);
				printf("Rayos que han llegado = %d\n", (numout - 1));
				/* Free allocated memory */
				FILE *pipeplot;
				pipeplot=popen("gnuplot -persist", "w");
				
				fprintf(pipeplot, "call '/home/pablo/Resultados_C/LFT/gp/LFTT.gp' %d\n", (numout-1));
				pclose(pipeplot);
				
				fui=fopen("/home/pablo/Resultados_C/LFT/res/matresul.txt", "w");
				for(i=0; i<numout; i++)
				fprintf(fui, "%d %d\n", resul[i][0], resul[i][1] );
	fclose(fui);
}
