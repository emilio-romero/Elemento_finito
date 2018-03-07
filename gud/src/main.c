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

matrizConectividad(ain,npe,nelem);

printf("...........\n Se se escribira el archivo: %s \n..............\n",aout);

printf("Gracias por usar gud!\nThanks for using gud!\n");
return(0);}
