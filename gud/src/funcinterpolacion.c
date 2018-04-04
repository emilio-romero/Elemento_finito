#include "funcinterpolacion.h"

void N1Dl(double xi, double *out){
  out[0]=(1.0-xi)*0.5; 
  out[1]=(1.0+xi)*0.5; 
}

void dN1Dl(double xi, double *out){
  out[0]=-0.5;
  out[1]=0.5;
}
