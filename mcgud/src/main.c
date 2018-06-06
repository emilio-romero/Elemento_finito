#include <stdio.h>
#include <stdlib.h>
#include "lecturamalla.h"
#include "algebralineal.h"
#include "mcgud.h"
int main(int argc, char *argv[]){

char ain[50], aout[50]; 
getParams(argc,argv,ain,aout);
elemento e1; 
/*
* Datos basicos 
*/
char problem[50], solucionador[30]; 
double tol; 
int maxiter, dim, npe, nelem,nnodos,ncond,nmat,tinterval;
double mtime; 
printf("...........\n Se leera el archivo: %s \n..............\n",ain);
datosMalla(ain,problem,solucionador,&tol,&maxiter,&mtime,&tinterval,&dim,&npe,&nelem);

int **mc=matrizConectividad(ain,npe,nelem);
  printf("Se leyo matriz de conectividad...\n");
double **mn=matrizNodos(ain,&nnodos,dim);
  printf("Se leyo matriz de nodos...\n");
double **mat=matrizMateriales(ain,&nmat); 
  printf("Se leyo matriz de materiales...\n");
double **cond=matrizCondiciones(ain,&ncond);
int ncondf; 
double **condf=matrizCondicionesF(ain,&ncondf);
  printf("Se leyeron condiciones de frontera...\n");
e1.dim=dim; e1.npe=npe;
int ncondm2,ncondm1; 
double **condm2=CondicionInicialm2(ain,&ncondm2);
double **condm1=CondicionInicialm1(ain,&ncondm1);
  printf("Se leyeron condiciones iniciales...\n");
fabricarElemento(&e1);
printf("Se creo el elemento...\n");

double *f=crear_vector(nnodos);
double **o=crear_matriz(nnodos,nnodos);
double **m=crear_matriz(nnodos,nnodos);
double **k=matrizRigidez(&e1,nnodos,nelem,mc,mn,mat,ncond,cond,ncondf,condf,f,m,o);
printf("Se crearon la matriz de rigidez y el vector de esfuerzos...\n");
double *phi=crear_vector(nnodos);


/*Solucion en el tiempo*/
printf("Se empezara la solucion del problema\n");
if(strcmp(solucionador,"Conjugate_gradient")==0){
  printf("Resolvere con CG\n");
  //GradienteConjugado(k,f,nnodos,nnodos,tol,phi);
}else{
  printf("Resolvere con Cholesky\n"); 
  //solLL(k,f,nnodos,nnodos,phi);
  MaxwellCatanneo(m,o,k,f,nnodos,mtime,tinterval,nelem,mc,mn,ncondm1,condm1,ncondm2,condm2,
      ncond,cond,&e1,aout);
}
//double **Flux=crear_matriz(nelem*e1.npi1,e1.dim);
//printf("Se calcularan los flujos\n");
//calcularFlujos(&e1,nnodos,nelem,mc,mn,mat,phi,Flux);
printf("...........\n Se escribira el archivo: %s \n..............\n",aout);
//resultados(aout,nnodos,phi,e1.npi1,nelem,e1.dim,e1.npe,Flux);
/*Liberacion de memoria*/
printf("Liberando memoria... \n");
for(int i=0;i<nelem;++i) free(mc[i]); 
free(mc);
liberar_matriz(mn,nnodos);
liberar_matriz(mat,nmat);
liberar_matriz(cond,ncond);
//liberar_matriz(Flux,nelem*e1.npi1);
free(phi);
liberar_matriz(k,nnodos);
liberar_matriz(m,nnodos);
liberar_matriz(o,nnodos);
printf("Listo!\n");

printf("Gracias por usar gud!\nThanks for using gud!\n");
return(0);}
