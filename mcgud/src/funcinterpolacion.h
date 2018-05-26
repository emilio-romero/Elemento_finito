#ifndef FUNCINTERPOLACION_H
#define FUNCINTERPOLACION_H 
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
void N1Dl(double *xi, double **out); 
void dN1Dl(double *xi, double **out); 
void N1Dq(double *xi, double **out); 
void dN1Dq(double *xi, double **out); 
void N2Dl(double *cni, double **out); 
void dN2Dl(double *cni, double **out); 
void N2Dq(double *cni, double **out); 
void dN2Dq(double *cni, double **out); 
void N3Dl(double *cni, double **out); 
void dN3Dl(double *cni, double **out); 
#endif 
