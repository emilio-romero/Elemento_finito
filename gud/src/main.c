#include <stdio.h>
#include "lecturamalla.h"

int main(int argc, char *argv[]){
char ain[50], aout[50]; 
getParams(argc,argv,ain,aout);

/*
* Datos basicos 
*/
char problem[50], solucionador[30]; 
double tol; 
int maxiter, dim, npe, nelem; 
printf("...........\n Se leera el archivo: %s \n..............\n",ain);
datosMalla(ain,problem,solucionador,&tol,&maxiter,&dim,&npe,&nelem);

int **mc=matrizConectividad(ain,npe,nelem);
for(int i=0;i<5;i++)
  printf("%d %d %d\n",mc[i][0],mc[i][1],mc[i][2]);

printf("...........\n Se se escribira el archivo: %s \n..............\n",aout);

/*Liberacion de memoria*/
for(int i=0;i<nelem;i++){
  free(mc[i]);
}
free(mc);

printf("Gracias por usar gud!\nThanks for using gud!\n");
return(0);}
