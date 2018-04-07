#include "femgud.h"

void fabricarElemento(elemento *me){
  if(me->dim==1 && me->npe==2){
    E1D2N(me);   
  } 
  if(me->dim==1 && me->npe==3){
    E1D3N(me);
  }
  if(me->dim==2 && me->npe==3){
    E2D3N(me); 
  }
}
/*Elemento 1D con dos nodos */
void E1D2N(elemento *me){
  me->npi1=1;//Asigna el numero de puntos de integracion
  me->pi=crear_matriz(1,1); //Crea el vector para los puntos 
  me->wi=crear_vector(1); // Crea el vector para los pesos
  me->pi[0][0]=0.0;  //Asigna puntos
  me->wi[0]=2.0; //Asigna pesos 
 //Deberia asignar las funciones de interpolacion 
  me->Ni=N1Dl;
  me->dNi=dN1Dl;
}
/*Elemento 1D con tres nodos*/
void E1D3N(elemento *me){
  me->npi1=2; 
  me->pi=crear_matriz(2,me->dim);
  me->wi=crear_vector(2);
  me->pi[0][0]=0.5773502692;me->pi[1][0]=-0.5773502692;
  me->wi[0]=me->wi[1]=1.0; 
  me->Ni=N1Dq;
  me->dNi=dN1Dq;
}
/*Elemento 2D con tres nodos*/
void E2D3N(elemento *me){
  me->npi1=1;
  me->pi=crear_matriz(1,me->dim);
  me->wi=crear_vector(1);
  me->pi[0][0]=1.0/3.0; me->pi[0][1]=1.0/3.0; 
  me->wi[0]=1.0; 
  me->Ni=N2Dl;
  me->dNi=dN2Dl;
}

/*Calculo de la matriz de rigidez*/
/* Ciclo sobre elementos
 * 1.- Obtener el elemento y las coordenadas del elemento //listo 
 * Ciclo sobre puntos de integracion 
 * 2.- Calcular las derivadas naturales 
 * 3.- Calcular el Jacobiano y el determinante 
 * 4.- Calcular la matriz constitutiva (propiedad del material D)
 * 5.- Calcular la matriz B 
 * 6.- Calcular BDB 
 * 7.- Sumar a la matriz elemental multiplicada por det|J| y W
 * Cierra ciclo sobre puntos de integracion
 * 8.- Sumar la matriz elemental a la matriz global 
 * Cierra ciclo sobre elementos
 *
 */
double **matrizRigidez(elemento *me, int nnodos, int nelem, int **MC, double **MN){
  double **K=crear_matriz(nnodos,nnodos);
  double **dn=crear_matriz(me->dim,me->npe);// Derivadas naturales
  double **ke=crear_matriz(me->npe, me->npe); //Matriz elemental
  double **aux1=crear_matriz(me->dim, me->npe); //Matriz elemental
  double **aux2=crear_matriz(me->npe, me->npe); //Matriz elemental
  double **jacobiano=crear_matriz(me->dim, me->dim);//Jacobiano
  double detj;//determinante del jacobiano 
  double **invj=crear_matriz(me->dim,me->dim); //Inversa del jacobiano
  double **cci=crear_matriz(me->dim,me->npe);//Coordenadas cartesianas del elemento
  int nodo_actual,e1,e2; 
  double **B=crear_matriz(me->dim,me->npe);
  double **Bt=crear_matriz(me->npe,me->dim);
  double **D=crear_matriz(me->dim,me->dim);
  for(int ele=0;ele<nelem;++ele){
  //printf("llegas aca?\n");
  //Coordenadas cartesianas del elemento en matriz de dimxnpe
    for(int i=0;i<me->npe;++i){
      for(int j=0;j<me->dim;++j){
        nodo_actual=MC[ele][i+1];
        cci[j][i]=MN[nodo_actual-1][j];
        //printf("%lf ",cci[j][i]);
      }
    }
   // printf("\n");//junto con cci[j][i]
   // Propiedades del material 
    for(int i=0;i<me->dim;++i){
      D[i][i]=1000.0; //Cambiar por la propiedad correspondiente al elemento
    }

    for(int p=0;p<me->npi1;++p){
      me->dNi(me->pi[p],dn);//Corregir los puntos de integracion que recibe
      //Calculo del Jacobiano
      for(int j=0;j<me->dim;j++){
       //..... me la voy a pelar 
        for(int i=0;i<me->dim;i++){
          jacobiano[i][j]=punto(dn[i],cci[j],me->npe);
        }
      }
      //Fin calculo del Jacobiano
      //Calculo del determinante y la inversa del determinante
      invJacobian(jacobiano,me,&detj,invj);
      //Fin del calculo del determinante
      //Calculo de B 
      matriz_mul(invj,dn,me->dim,me->dim,me->npe,B);
      matriz_transponer(B,me->npe,me->dim,Bt);
      matriz_mul(D,B,me->dim,me->dim,me->npe,aux1);
      matriz_mul(Bt,aux1,me->npe,me->dim,me->npe,aux2);
      matriz_escalar(me->wi[p]*detj,aux2,me->npe,me->npe,aux2);
      matriz_suma(aux2,ke,me->npe,me->npe,ke);
    }
    //
    //Ensamblaje de la matriz global 
    for(int i=0;i<me->npe;++i){
      for(int j=0;j<me->npe;++j){
        e1=MC[ele][i+1]; e2=MC[ele][j+1]; 
        K[e1-1][e2-1]+=ke[i][j];
      }
    }
    matriz_ceros(me->npe,me->npe,ke);
  }
liberar_matriz(dn,me->dim);
liberar_matriz(cci,me->dim);
liberar_matriz(ke,me->npe);
liberar_matriz(jacobiano,me->dim);
liberar_matriz(invj,me->dim);
liberar_matriz(B,me->dim);
liberar_matriz(Bt,me->npe);
liberar_matriz(D,me->dim);
liberar_matriz(aux1,me->dim);
liberar_matriz(aux2,me->npe);
  return(K);
}

void invJacobian(double **Jacobian,elemento *me,double *detJ ,double **invJ){
  if(me->dim==1){
    *detJ=Jacobian[0][0]; 
    invJ[0][0]=Jacobian[0][0]; 
  }
  else if(me->dim==2){
    *detJ=Jacobian[0][0] * Jacobian[1][1] - Jacobian[0][1] * Jacobian[1][0]; 
    invJ[0][0]=Jacobian[1][1]/(*detJ); invJ[0][1]=-Jacobian[0][1]/(*detJ);
    invJ[1][0]=-Jacobian[1][0]/(*detJ); invJ[1][1]=Jacobian[0][0]/(*detJ);
  }
  else if(me->dim==3){
  
  }

}
  
