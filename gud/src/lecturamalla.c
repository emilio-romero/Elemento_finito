#include "lecturamalla.h"

void getParams(int argc, char *argv[], char *chivoin, char *chivoout){
  if(argc>1) strcpy(chivoin,argv[1]);
  if(argc>2) strcpy(chivoout,argv[2]);
}

int datosMalla(char *chivoin, char* probname, char* soltype,double *tol,int *maxs, int *ndime,\
int *npe, int *nelem){
FILE *f1=fopen(chivoin,"r");
char buff[255]; 
for(int i=0;i<5;i++){
  fgets(buff,255,f1);
}
 fscanf(f1,"%s",probname);
 printf("Nombre del problema: %s\n",probname);
 fgets(buff,255,f1);fgets(buff,255,f1);
 fscanf(f1,"%s",soltype);
 printf("Solucionador usado: %s\n",soltype);
 fgets(buff,255,f1);fgets(buff,255,f1);
 fscanf(f1,"%lf",tol);
 fgets(buff,255,f1);fgets(buff,255,f1);
 fscanf(f1,"%d",maxs);
 fgets(buff,255,f1);fgets(buff,255,f1); fgets(buff,255,f1);
 fscanf(f1,"%d",ndime);
 fgets(buff,255,f1);fgets(buff,255,f1);
 fscanf(f1,"%d",npe); 
 fgets(buff,255,f1);fgets(buff,255,f1);
 fscanf(f1,"%d",nelem); 
 /**/
fclose(f1);
if(*ndime==2 && *npe==2) *ndime=1; 

printf("Tolerancia en los calculos: %lf\n",*tol);
printf("Iteraciones maximas: %d\n",*maxs);
printf("Dimension del problema: %d\n",*ndime);
printf("Nodos por elemento: %d\n",*npe);
printf("Numero de elementos: %d\n",*nelem);


return(1);}

int **matrizConectividad(char *chivoin, int npe, int nelem){
  int **aux=(int**)malloc(nelem*sizeof(int*));
  for(int i=0;i<nelem;i++) aux[i]=(int*)malloc((1+npe)*sizeof(int));
char buff[255];

FILE *f1=fopen(chivoin,"r");
fgets(buff,255,f1);
while(strcmp(buff,"#Elemento Material node1 node2 ...\n")!=0){
  fgets(buff,255,f1);
}
for(int i=0;i<nelem;i++){
  for(int j=0;j<(1+npe);j++){
     fscanf(f1,"%d",&aux[i][j]);
     }
}
fclose(f1);


return(aux);}

