#include "femgud.h"

void fabricarElemento(elemento *me){
  if(me->dim==1 && me->npe==2){
    me->npi1=1;//Asigna el numero de puntos de integracion
    me->pi1=crear_vector(1); //Crea el vector para los puntos 
    me->wi1=crear_vector(1); // Crea el vector para los pesos
    me->pi1[0]=0.0;  //Asigna puntos
    me->wi1[0]=2.0; //Asigna pesos 
    //Deberia asignar las funciones de interpolacion 
    me->Ni=N1Dl;
    me->dNi=dN1Dl;
  } 
  if(me->dim==1 && me->npe==3){
    me->npi1=2; 
    me->pi1=crear_vector(2);
    me->wi1=crear_vector(2);
    me->pi1[0]=0.5773502692;me->pi1[1]=-0.5773502692;
    me->wi1[0]=me->wi1[1]=1.0; 
  }
}

double **matrizRigidez(elemento *me, int nnodos, int nelem, int **MC, double **MN){
  double **K=crear_matriz(nnodos,nnodos);
  double *dn=crear_vector(me->npe);
  double **ke=crear_matriz(me->npe, me->npe);
  double **jacobiano=crear_matriz(me->dim, me->dim);
  double *xi=crear_vector(me->npe);
  for(int ele=0;ele<nelem;++ele){
    
    //
    for(int p=0;p<me->npi1;++p){
      me->Ni(me->pi1[p],dn);
      for(int j=0;j<me->dim;j++){
       //Rellenar xi con los valores correspondientes de nodos
       //Cambiar dn, en vez de vector una matriz, para obtener las derivadas
       //respecto a otras coordenadas naturales 
       //..... me la voy a pelar 
        for(int i=0;i<me->dim;i++){
          jacobiano[i][j]=punto(dn,xi,me->npe);
        }
      }
    }
    //

  }
free(xi);
liberar_matriz(ke,me->npe);
liberar_matriz(jacobiano,me->dim);
  return(K);
}

  
