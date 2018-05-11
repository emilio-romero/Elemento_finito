#include "lecturamalla.h"

void getParams(int argc, char *argv[], char *chivoin, char *chivoout){
  if(argc>1) strcpy(chivoin,argv[1]);
  if(argc>2) strcpy(chivoout,argv[2]);
}

int datosMalla(char *chivoin, char* probname, char* soltype,double *tol,int *maxs, int *ndime,\
int *npe, int *nelem){
FILE *f1=fopen(chivoin,"r");
char buff[255]; 
for(int i=0;i<5;i++){
  fgets(buff,255,f1);
}
 fscanf(f1,"%s",probname);
 printf("Nombre del problema: %s\n",probname);
 fgets(buff,255,f1);fgets(buff,255,f1);
 fscanf(f1,"%s",soltype);
 printf("Solucionador usado: %s\n",soltype);
 fgets(buff,255,f1);fgets(buff,255,f1);
 fscanf(f1,"%lf",tol);
 fgets(buff,255,f1);fgets(buff,255,f1);
 fscanf(f1,"%d",maxs);
 fgets(buff,255,f1);fgets(buff,255,f1); fgets(buff,255,f1);
 fscanf(f1,"%d",ndime);
 fgets(buff,255,f1);fgets(buff,255,f1);
 fscanf(f1,"%d",npe); 
 fgets(buff,255,f1);fgets(buff,255,f1);
 fscanf(f1,"%d",nelem); 
 /**/
fclose(f1);
if(*ndime==2 && *npe==2) *ndime=1; 

printf("Tolerancia en los calculos: %lf\n",*tol);
printf("Iteraciones maximas: %d\n",*maxs);
printf("Dimension del problema: %d\n",*ndime);
printf("Nodos por elemento: %d\n",*npe);
printf("Numero de elementos: %d\n",*nelem);


return(1);}

int **matrizConectividad(char *chivoin, int npe, int nelem){
  int **aux=(int**)malloc(nelem*sizeof(int*));
  for(int i=0;i<nelem;i++) aux[i]=(int*)malloc((1+npe)*sizeof(int));
char buff[255];
int *bufint; 
FILE *f1=fopen(chivoin,"r");
fgets(buff,255,f1);
while(strcmp(buff,"#Elemento Material node1 node2 ...\n")!=0){
  fgets(buff,255,f1);
}
for(int i=0;i<nelem;i++){
  fscanf(f1,"%d",&aux[i][0]);
  for(int j=0;j<(1+npe);j++){
      fscanf(f1,"%d",&aux[i][j]);
     }
}
fclose(f1);
return(aux);}

double **matrizNodos(char *chivoin, int *nnodos, int dim){
FILE *f1=fopen(chivoin,"r");
char buff[255];
int bufint; 
double bufdbl; 
while(strcmp(buff,"#Numero de nodos \n")!=0){
  fgets(buff,255,f1);
}
fscanf(f1,"%d",nnodos);
  double **aux=(double**)malloc(*nnodos*sizeof(double*));
  for(int i=0;i<*nnodos;i++) aux[i]=(double*)malloc((dim)*sizeof(double));
fgets(buff,255,f1); fgets(buff,255,f1);fgets(buff,255,f1);
for(int i=0;i<*nnodos;i++){
  fscanf(f1,"%lf",&aux[i][0]);
  for(int j=0;j<dim;j++){
      fscanf(f1,"%lf",&aux[i][j]);
      if(dim==1){
        bufdbl=aux[i][j];   
        fscanf(f1,"%lf",&aux[i][j]);
        aux[i][j]=bufdbl; 
      }
     }
}
fclose(f1);
return(aux);}
double **matrizCondiciones(char *chivoin, int *ncond){
FILE *f1=fopen(chivoin,"r");
char buff[255]; 
int bufint; 
double bufdbl; 
while(strcmp(buff,"#Numero de condiciones \n")!=0){
  fgets(buff,255,f1);
}
fscanf(f1,"%d",ncond);
if(*ncond==0) return(NULL); 
  double **aux=(double**)malloc(*ncond*sizeof(double*));
  for(int i=0;i<(*ncond);++i) aux[i]=(double*)malloc(2*sizeof(double));
  fgets(buff,255,f1); fgets(buff,255,f1); fgets(buff,255,f1);
  for(int i=0;i<(*ncond);++i){
    fscanf(f1,"%lf",&aux[i][0]);
    fscanf(f1,"%lf",&aux[i][1]);
  }
fclose(f1);
return(aux);}
double **matrizCondicionesF(char *chivoin, int *ncondf){
FILE *f1=fopen(chivoin,"r");
char buff[255]; 
int bufint; 
double bufdbl; 
while(strcmp(buff,"#Flujo \n")!=0){
  fgets(buff,255,f1);
}
fgets(buff,255,f1);
fscanf(f1,"%d",ncondf);
if(*ncondf==0) return(NULL); 
  double **aux=(double**)malloc(*ncondf*sizeof(double*));
  for(int i=0;i<(*ncondf);++i) aux[i]=(double*)malloc(3*sizeof(double));
  fgets(buff,255,f1); fgets(buff,255,f1); fgets(buff,255,f1);
  for(int i=0;i<(*ncondf);++i){
    fscanf(f1,"%lf",&aux[i][0]);
    fscanf(f1,"%lf",&aux[i][0]);
    fscanf(f1,"%lf",&aux[i][1]);
    fscanf(f1,"%lf",&aux[i][2]);
  }
fclose(f1);
return(aux);}
double **matrizMateriales(char *chivoin, int *nmat){
FILE *f1=fopen(chivoin,"r");
char buff[255]; 
int bufint; 
double bufdbl; 
while(strcmp(buff,"#Numero de materiales \n")!=0){
  fgets(buff,255,f1);
}
fscanf(f1,"%d",nmat);
  double **aux=(double**)malloc(*nmat*sizeof(double*));
  for(int i=0;i<(*nmat);++i) aux[i]=(double*)malloc(4*sizeof(double));
  fgets(buff,255,f1); fgets(buff,255,f1); fgets(buff,255,f1);
  for(int i=0;i<(*nmat);++i){
    fscanf(f1,"%lf",&aux[i][0]);
    fscanf(f1,"%lf",&aux[i][0]);
    fscanf(f1,"%lf",&aux[i][1]);
    fscanf(f1,"%lf",&aux[i][2]);
    fscanf(f1,"%lf",&aux[i][3]);
  }
 fclose(f1);

return(aux);}
void resultados(char *problema,int nnodos, double *phi, int npi,int nelem,int pdim,int npe,
    double **Flujos){
  char archi[80];
  char tipo_elemento[30];
//  strcpy(archi,problema);
//  strcat(archi,".post.res");
  //printf("%s",archi);
  FILE *f1=fopen(problema,"w");
  file:///usr/share/applications/firefox.desktop
  fprintf(f1,"GiD Post Result File 1.0\n \n");
  fprintf(f1,"Result \"Temperature\" \"Calor\" 1 Scalar OnNodes\n");
  fprintf(f1,"Values\n");
  for(int i=0;i<nnodos;++i){
    fprintf(f1,"%d %E\n",i+1,phi[i]);
  }
  fprintf(f1,"End Values\n");
  if(pdim==1){
    strcpy(tipo_elemento,"Linear");  
  }
  else if(pdim==2){
    if(npe==3) strcpy(tipo_elemento,"Triangle"); 
    if(npe==4) strcpy(tipo_elemento,"Quadrilateral"); 
  }
  else if(pdim==3){
    strcpy(tipo_elemento,"Tetrahedra");
  }
  else{
    printf("Too many dimensions");
  }
  fprintf(f1,"GaussPoints \"Gauss internal\" ElemType %s \n",tipo_elemento);
  fprintf(f1,"Number Of Gauss Points: %d\n",npi);
  fprintf(f1,"Natural Coordinates: internal\nend gausspoints\n");
  
  fprintf(f1,"\nResult \"Flux\" \"Calor\" 1 Vector OnGaussPoints \"Gauss internal\"\n");
  fprintf(f1,"Values\n");
  for(int i=0;i<nelem;++i){
    fprintf(f1,"%d",i+1);
    for(int p=0;p<npi;++p){
      fprintf(f1,"   ");
      for(int j=0;j<pdim;++j){
        fprintf(f1,"%E ",Flujos[i*npi+p][j]);
      }
      fprintf(f1,"\n");
    }
  }
  fprintf(f1,"End Values\n");
  fclose(f1);
}

