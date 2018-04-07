#include "funcinterpolacion.h"
/*1D dos nodos*/
void N1Dl(double *xi, double **out){
  out[0][0]=(1.0-xi[0])*0.5; 
  out[0][1]=(1.0+xi[0])*0.5; 
}

void dN1Dl(double *xi, double **out){
  out[0][0]=-0.5;
  out[0][1]=0.5;
}
/*1D tres nodos*/
void N1Dq(double *xi,double **out){
  out[0][0]=0.5*(xi[0]*xi[0]-xi[0]);
  out[0][1]=1.0-xi[0]*xi[0];
  out[0][2]=0.5*(xi[0]*xi[0]+xi[0]);
}

void dN1Dq(double *xi,double **out){
  out[0][0]=xi[0]-0.5;
  out[0][1]=-2.0*xi[0];
  out[0][2]=xi[0]+0.5;
}
/*2D tres nodos*/
/*
 * cni son coordenadas naturales 
 * cni[0]-> xi, cni[1]->eta, cni[2]->zeta
 * */
void N2Dl(double *cni,double **out){
  out[0][0]=1.0-cni[0]-cni[1];  
  out[0][1]=cni[0];  
  out[0][2]=cni[1];  
}
void dN2Dl(double *cni,double **out){
  out[0][0]=-1.0;
  out[0][1]=1.0;
  out[0][2]=0.0;
  
  out[1][0]=-1.0;
  out[1][1]=0.0;
  out[1][2]=1.0;
}
