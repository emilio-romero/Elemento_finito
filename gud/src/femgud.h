#ifndef FEMGUD_H
#define FEMGUD_H 
#include <stdio.h>
#include <stdlib.h>
#include "algebralineal.h"
#include "funcinterpolacion.h"
typedef struct{
  int dim; //Dimension del problema 
  int npe; //Numero de nodos por elemento
  int npi1; //ni puta idea ? xD //Numeros de puntos de integracion direccion 1 1D 2D y 3D
  int npi2; //Numeros de puntos de integracion direccion 2 2D y 3D
  int npi3; //Numeros de puntos de integracion direccion 3 3D 
  double **pi; //Puntos de integracion   
  double *wi; //Pesos de integracion
  void (*Ni)(double*, double**); 
  void (*dNi)(double*, double**); 
} elemento;

void fabricarElemento(elemento *me);
//elementos
void E1D2N(elemento *me);
void E1D3N(elemento *me);
void E2D3N(elemento *me);

//Matrices de rigidez y esfuerzos
double **matrizRigidez(elemento *me, int nnodos, int nelem, int **MC, double **MN);
void invJacobian(double **Jacobian, elemento *me,double *detJ ,double **invJ);

#endif
