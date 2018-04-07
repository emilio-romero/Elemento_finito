#include <stdio.h>
#include <stdlib.h>
#include "lecturamalla.h"
#include "algebralineal.h"
#include "femgud.h"
int main(int argc, char *argv[]){

char ain[50], aout[50]; 
getParams(argc,argv,ain,aout);
elemento e1; 
/*
* Datos basicos 
*/
char problem[50], solucionador[30]; 
double tol; 
int maxiter, dim, npe, nelem,nnodos; 
printf("...........\n Se leera el archivo: %s \n..............\n",ain);
datosMalla(ain,problem,solucionador,&tol,&maxiter,&dim,&npe,&nelem);

int **mc=matrizConectividad(ain,npe,nelem);
for(int i=0;i<5;++i){
  for(int j=0;j<npe+1;++j){
    printf("%d ",mc[i][j]);}
  printf("\n");}
double **mn=matrizNodos(ain,&nnodos,dim);

for(int i=0;i<5;i++){
  for(int j=0;j<dim;++j){
  printf("%lf ",mn[i][j]);}
  printf("\n");}
printf("continuan...\n");

e1.dim=dim; e1.npe=npe;
printf("...........\n Se escribira el archivo: %s \n..............\n",aout);
fabricarElemento(&e1);
printf("Se creo el elemento\n");
double **k=matrizRigidez(&e1,nnodos,nelem,mc,mn);
for(int i=0;i<nnodos;++i){
  for(int j=0;j<nnodos;++j){
    printf("%1.2lf ",k[i][j]);
  } printf("\n");
}
/*Liberacion de memoria*/
printf("Liberando memoria... \n");

for(int i=0;i<nelem;i++){
  free(mc[i]);
}
free(mc);

for(int i=0;i<nnodos;i++){
  free(mn[i]);
}
free(mn);

printf("Listo!\n");

printf("Gracias por usar gud!\nThanks for using gud!\n");
return(0);}
