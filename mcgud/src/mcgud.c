#include "mcgud.h"

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
  if(me->dim==2 && me->npe==4){
    E2D4N(me); 
  }
  if(me->dim==3 && me->npe==4){
    E3D4N(me); 
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
/*Elemento 2D de cuatro nodos*/
void E2D4N(elemento *me){
  me->npi1=4;
  me->pi=crear_matriz(4,me->dim);
  me->wi=crear_vector(4);
  me->pi[0][0]=-0.5773502692; me->pi[0][1]=-0.5773502692;
  me->pi[1][0]=0.5773502692; me->pi[1][1]=-0.5773502692;
  me->pi[2][0]=0.5773502692; me->pi[2][1]=0.5773502692;
  me->pi[3][0]=-0.5773502692; me->pi[3][1]=0.5773502692;
  me->wi[0]=me->wi[1]=me->wi[2]=me->wi[3]=1.0; 
  me->Ni=N2Dq; 
  me->dNi=dN2Dq;
}
void E3D4N(elemento *me){
  me->npi1=1; 
  me->pi=crear_matriz(1,me->dim);
  me->wi=crear_vector(1);
  me->pi[0][0]=0.25;me->pi[0][1]=0.25;me->pi[0][2]=0.25;
  me->wi[0]=1.0; 
  me->Ni=N3Dl;
  me->dNi=dN3Dl;
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
double **matrizRigidez(elemento *me, int nnodos, int nelem, int **MC, double **MN,double **Mat,
    int ncond, double **cond,int ncondf,double **condf, double *f,
    double **M, double **O){
  double **K=crear_matriz(nnodos,nnodos);
  double **Nt=crear_matriz(me->npe,1);// Derivadas naturales
  double **N=crear_matriz(1,me->npe);// Derivadas naturales
  double **ke=crear_matriz(me->npe, me->npe); //Matriz elemental
  double **mme=crear_matriz(me->npe, me->npe); //Matriz elemental
  double **oe=crear_matriz(me->npe, me->npe); //Matriz elemental
  double **aux1=crear_matriz(me->dim, me->npe); //Matriz elemental
  double **aux2=crear_matriz(me->npe, me->npe); //Matriz elemental
  double detj;//determinante del jacobiano 
  double **cci=crear_matriz(me->dim,me->npe);//Coordenadas cartesianas del elemento
  int nodo_actual,e1,e2,tmat; 
  double **B=crear_matriz(me->dim,me->npe);
  double **Bt=crear_matriz(me->npe,me->dim);
  double **D=crear_matriz(me->dim,me->dim);
  double Q; 
  for(int ele=0;ele<nelem;++ele){
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
    tmat=MC[ele][0]-1; 
    for(int i=0;i<me->dim;++i){
      D[i][i]=Mat[tmat][i]; //Cambiar por la propiedad correspondiente al elemento
    }
    Q=Mat[tmat][3]; 
    for(int p=0;p<me->npi1;++p){
      me->Ni(me->pi[p],N);
      //Calculo de B 
      calcularB(me,p,cci,&detj,B);
      matriz_transponer(B,me->npe,me->dim,Bt);// 
      //Calculo de la matriz de rigidez elemental
      matriz_mul(D,B,me->dim,me->dim,me->npe,aux1);
      matriz_mul(Bt,aux1,me->npe,me->dim,me->npe,aux2);
      matriz_escalar(me->wi[p]*detj,aux2,me->npe,me->npe,aux2);
      matriz_suma(aux2,ke,me->npe,me->npe,ke);
      matriz_transponer(N,me->npe,1,Nt);// 
      matriz_mul(Nt,N,me->npe,1,me->npe,oe);
      matriz_copiar(oe,me->npe,me->npe,mme);
      //Calculo de la matriz de segundo sonido elemental 
      matriz_escalar(0.1*detj*me->wi[p],mme,me->npe,me->npe,mme);
      //Calculo de la matriz de primera derivada elemental
      matriz_escalar(1.0*detj*me->wi[p],oe,me->npe,me->npe,oe);
      /*Considerar el hecho de si el segundo sonido es matricial o no*/      
      //Calculo de f
      matriz_escalar(Q*detj*me->wi[p],N,1,me->npe,N);
    }
    //
    //Ensamblaje de la matriz global 
    for(int i=0;i<me->npe;++i){
      e1=MC[ele][i+1];
      for(int j=0;j<me->npe;++j){
        e2=MC[ele][j+1]; 
        K[e1-1][e2-1]+=ke[i][j];
        O[e1-1][e2-1]+=oe[i][j];
        M[e1-1][e2-1]+=mme[i][j];
      }
      f[e1-1]+=N[0][i];
    }
    matriz_ceros(me->npe,me->npe,ke);
    matriz_ceros(me->npe,me->npe,oe);
    matriz_ceros(me->npe,me->npe,mme);
  }
liberar_matriz(cci,me->dim);
liberar_matriz(ke,me->npe);
liberar_matriz(mme,me->npe);
liberar_matriz(oe,me->npe);
liberar_matriz(B,me->dim);
liberar_matriz(Bt,me->npe);
liberar_matriz(D,me->dim);
liberar_matriz(aux1,me->dim);
liberar_matriz(aux2,me->npe);
liberar_matriz(N,1);
liberar_matriz(Nt,me->npe);
  return(K);
}

int calcularB(elemento *me,int pi_actual,double **cci,double *detj,double **B){
  double **dn=crear_matriz(me->dim,me->npe);// Derivadas naturales
  double **jacobiano=crear_matriz(me->dim, me->dim);//Jacobiano
  double **invj=crear_matriz(me->dim,me->dim); //Inversa del jacobiano
  me->dNi(me->pi[pi_actual],dn);//Corregir los puntos de integracion que recibe
  //Calculo del Jacobiano
  for(int j=0;j<me->dim;j++){
    for(int i=0;i<me->dim;i++){
      jacobiano[i][j]=punto(dn[i],cci[j],me->npe);
    }
  }
  //Fin del calculo del jacobiano
  //Calculo de la inversa y determinante del jacobiano
  invJacobian(jacobiano,me,detj,invj);
  matriz_mul(invj,dn,me->dim,me->dim,me->npe,B); //Calculo de B
  liberar_matriz(jacobiano,me->dim);
  liberar_matriz(invj,me->dim);
  liberar_matriz(dn,me->dim);
return(1);}

void condicionesFrontera(double **K, double *f,int ncond ,double **cond,int ncondf, 
    double **condf,int nnodos, elemento *me){
  int nodo_actual; 
  int n1,n2; 
  for(int e=0;e<ncond;++e){
    nodo_actual=(int)cond[e][0]-1;
    for(int i=0;i<nnodos;++i){
      if(fabs(K[i][nodo_actual])>0 && i!=nodo_actual){
        f[i]=f[i]-K[i][nodo_actual]*cond[e][1];
        K[i][nodo_actual]=K[nodo_actual][i]=0.0;
      } 
    }
    K[nodo_actual][nodo_actual]=1.0; 
    f[nodo_actual]=cond[e][1]; 
  } 
  for(int c=0;c<ncondf;++c){
    n1=(int)condf[c][1]-1; n2=(int)condf[c][2]-1; 
    f[n1]+=condf[c][0];
    f[n2]+=condf[c][0];
  }
}


void invJacobian(double **Jacobian,elemento *me,double *detJ ,double **invJ){
  if(me->dim==1){
    *detJ=Jacobian[0][0]; 
    invJ[0][0]=1/Jacobian[0][0]; 
  }
  else if(me->dim==2){
    *detJ=Jacobian[0][0] * Jacobian[1][1] - Jacobian[0][1] * Jacobian[1][0]; 
    invJ[0][0]=Jacobian[1][1]/(*detJ); invJ[0][1]=-Jacobian[0][1]/(*detJ);
    invJ[1][0]=-Jacobian[1][0]/(*detJ); invJ[1][1]=Jacobian[0][0]/(*detJ);
  }
  else if(me->dim==3){
    *detJ=Jacobian[0][0] * Jacobian[1][1]*Jacobian[2][2] - Jacobian[0][2] * Jacobian[1][1]*Jacobian[2][0];
    *detJ=*detJ+Jacobian[0][1]*Jacobian[1][2]*Jacobian[2][0];
    *detJ=*detJ+Jacobian[0][2]*Jacobian[1][0]*Jacobian[2][1];
    *detJ=*detJ-Jacobian[0][0]*Jacobian[1][2]*Jacobian[2][1];
    *detJ=*detJ-Jacobian[0][1]*Jacobian[1][0]*Jacobian[2][2];

    invJ[0][0]=(Jacobian[2][2]*Jacobian[1][1]-Jacobian[2][1]*Jacobian[1][2])/(*detJ);
    invJ[0][1]=-(Jacobian[2][2]*Jacobian[0][1]-Jacobian[2][1]*Jacobian[0][2])/(*detJ);
    invJ[0][2]=(Jacobian[1][2]*Jacobian[0][1]-Jacobian[1][1]*Jacobian[0][2])/(*detJ);

    invJ[1][0]=-(Jacobian[2][2]*Jacobian[1][0]-Jacobian[2][0]*Jacobian[1][2])/(*detJ);
    invJ[1][1]=(Jacobian[2][2]*Jacobian[0][0]-Jacobian[2][0]*Jacobian[0][2])/(*detJ);
    invJ[1][2]=-(Jacobian[1][2]*Jacobian[0][0]-Jacobian[1][0]*Jacobian[0][2])/(*detJ);
    
    invJ[2][0]=(Jacobian[2][1]*Jacobian[1][0]-Jacobian[2][0]*Jacobian[1][1])/(*detJ);
    invJ[2][1]=-(Jacobian[2][1]*Jacobian[0][0]-Jacobian[2][0]*Jacobian[0][1])/(*detJ);
    invJ[2][2]=(Jacobian[1][1]*Jacobian[0][0]-Jacobian[1][0]*Jacobian[0][1])/(*detJ);
  }

}
 

int calcularFlujos(elemento *me,int nnodos, int nelem,int **MC, double **MN,double **Mat,
    double *Temperatura,double **Flujos){
int nodo_actual,tmat; 
double **cci=crear_matriz(me->dim,me->npe);
double **D=crear_matriz(me->dim,me->dim);
double **B=crear_matriz(me->dim,me->npe);
double *temp_elemento=crear_vector(me->npe);
double **DB=crear_matriz(me->dim,me->npe);
double *flujo_elemento=crear_vector(me->dim);
double detJ; 
  for(int ele=0;ele<nelem;++ele){
  //Coordenadas cartesianas y temperaturas de los nodos del elemento
    for(int i=0;i<me->npe;++i){
      nodo_actual=MC[ele][i+1]-1;
      for(int j=0;j<me->dim;++j){
        cci[j][i]=MN[nodo_actual][j];
      }
      temp_elemento[i]=Temperatura[nodo_actual];
    }
   // Propiedades del material
    tmat=MC[ele][0]-1; 
    for(int i=0;i<me->dim;++i){
      D[i][i]=Mat[tmat][i]; //
    }
    
    for(int p=0;p<me->npi1;++p){
      calcularB(me,p,cci,&detJ,B);
      matriz_mul(D,B,me->dim,me->dim,me->npe,DB);
      matriz_vector_mul(DB,temp_elemento,me->dim,me->npe,flujo_elemento);
      vector_escalar(-1.0,flujo_elemento,me->dim,flujo_elemento);
      vector_copiar(flujo_elemento,me->dim,Flujos[ele*me->npi1+p]);/*Agregar a la matriz de 
      los flujos (nelem*pi X dim)*/
    }
  }
liberar_matriz(cci,me->dim);
liberar_matriz(D,me->dim);
liberar_matriz(B,me->dim);
liberar_matriz(DB,me->dim);
return(1);}

int MaxwellCatanneo(double **M, double **O, double **K, double *f,int nnodos, double mtime,
    int tinterval,int nelem ,int **MC,double **MN,int ncndm1, double **mcondm1,
    int ncndm2, double **mcondm2,
    int ncond, double **cond,
    elemento *me, char *problema){
  double dt=mtime/((double)(tinterval));
double **Matt=crear_matriz(nnodos,nnodos);
double **Mattm1=crear_matriz(nnodos,nnodos);
double **Mattm2=crear_matriz(nnodos,nnodos);
double *rhs=crear_vector(nnodos);
double *vm1=crear_vector(nnodos);
double *vm2=crear_vector(nnodos);
double *auxm1=crear_vector(nnodos);
double *auxm2=crear_vector(nnodos);
double *sol=crear_vector(nnodos);
/*Preparacion de las matrices*/
matriz_escalar(1.0/(dt*dt),M,nnodos,nnodos,M);
matriz_escalar(1.0/(dt),O,nnodos,nnodos,O);
matriz_suma(Matt,M,nnodos,nnodos,Matt);
matriz_suma(Matt,O,nnodos,nnodos,Matt);
matriz_suma(Matt,K,nnodos,nnodos,Matt);
matriz_escalar(2.0,M,nnodos,nnodos,M);
matriz_suma(Mattm1,M,nnodos,nnodos,Mattm1);
matriz_suma(Mattm1,O,nnodos,nnodos,Mattm1);
matriz_escalar(0.5,M,nnodos,nnodos,M);
matriz_copiar(M,nnodos,nnodos,Mattm2);
/*Fin de preparacion de las matrices*/
/*Aplicacion de condiciones de frontera y condiciones iniciales*/
  //condicionesFrontera1(Matt,f,ncond,cond,nnodos,me);
  //condicionesFrontera1(Mattm1,f,ncond,cond,nnodos,me);
  //condicionesFrontera1(Mattm2,f,ncond,cond,nnodos,me);

/*Fin de condiciones de frontera*/
  FILE *f1=fopen(problema,"w");
  fprintf(f1,"GiD Post Result File 1.0\n \n");
/*Aplicacion de condiciones iniciales*/
  int nodo_actual; 
  for(int i=0;i<ncndm1;++i){
    nodo_actual=(int)mcondm1[i][0];
    vm1[nodo_actual]=mcondm1[i][1];
    //printf("%lf ",vm1[nodo_actual]);
  }
  for(int i=0;i<ncndm2;++i){
    nodo_actual=(int)mcondm2[i][0];
    vm2[nodo_actual]=mcondm2[i][1];
  }
  fprintf(f1,"Result \"Temperature\" \"MC\" %d Scalar OnNodes\n",0);
    fprintf(f1,"Values\n");
    for(int i=0;i<nnodos;++i){
      fprintf(f1,"%d %E\n",i+1,vm2[i]);
    }
    fprintf(f1,"End Values\n");
  fprintf(f1,"Result \"Temperature\" \"MC\" %d Scalar OnNodes\n",1);
    fprintf(f1,"Values\n");
    for(int i=0;i<nnodos;++i){
      fprintf(f1,"%d %E\n",i+1,vm1[i]);
      //printf("%E ",vm1[i]);
    }
    fprintf(f1,"End Values\n");
/*Fin de condiciones iniciales*/

/*METODO*/
  for(int t=0;t<tinterval;++t){
    matriz_vector_mul(Mattm1,vm1,nnodos,nnodos,auxm1);
    matriz_vector_mul(Mattm2,vm2,nnodos,nnodos,auxm2);
    vector_suma(rhs,f,nnodos,rhs);
    vector_suma(rhs,auxm1,nnodos,rhs);
    vector_resta(rhs,auxm2,nnodos,rhs);
  condicionesFrontera1(Matt,rhs,ncond,cond,nnodos,me);
    solLL(Matt,rhs,nnodos,nnodos,sol);
    fprintf(f1,"Result \"Temperature\" \"MC\" %d Scalar OnNodes\n",t+2);
    fprintf(f1,"Values\n");
    for(int i=0;i<nnodos;++i){
      fprintf(f1,"%d %E\n",i+1,sol[i]);
    }
    fprintf(f1,"End Values\n");
    vector_copiar(vm1,nnodos,vm2);
    vector_copiar(sol,nnodos,vm1);
  }
  fclose(f1);
/*FIN METODO*/


liberar_matriz(Matt,nnodos);
liberar_matriz(Mattm1,nnodos);
liberar_matriz(Mattm2,nnodos);
free(rhs);free(vm1); free(vm2);
return(1);}

void condicionesFrontera1(double **K,double *f,int ncond ,double **cond,int nnodos, elemento *me){
  int nodo_actual; 
  int n1,n2; 
  for(int e=0;e<ncond;++e){
    nodo_actual=(int)cond[e][0]-1;
    for(int i=0;i<nnodos;++i){
      if(fabs(K[i][nodo_actual])>0 && i!=nodo_actual){
        f[i]=f[i]-K[i][nodo_actual]*cond[e][1];
        K[i][nodo_actual]=K[nodo_actual][i]=0.0;
      } 
    }
    K[nodo_actual][nodo_actual]=1.0; 
    f[nodo_actual]=cond[e][1]; 
  } 
}


