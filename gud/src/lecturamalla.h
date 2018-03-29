#ifndef LECTURA_MALLA
#define LECTURA_MALLA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getParams(int argc, char *argv[],char *chivoin, char *chivoout); 
int datosMalla(char *chivoin,char *probname, char* soltype, double *tol, int*maxs, int *ndime,\
               int *npe, int *nelem);
int **matrizConectividad(char *chivoin, int npe, int nelem);
double **matrizNodos(char *chivoin, int *nnodos, int dim);
#endif 
