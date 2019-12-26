#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define Nm 1000000
#define Ne 1000
/*
int es_primo(int n){
  if(n%2!=0 && n%3!=0 && n%5!=0 && n%7!=0 && n%11!=0 && n%13!=0 && n%17!=0 && n%19!=0 || n==2 || n==3 || n==5 || n==7 || n==11 || n==13 || n==17 || n==19)
  {
    return 1;
  }
  else
  {
  return 0;
  }

}
*/
int es_primo(int n){
  int a=0;
  for(int i=1; i<=n;i++){
  
    if(n%i==0){ 
      a++;
      if(a>2){return 0;}
    }
  } 
  if(a==2){
    return 1;
  }
  else{
    return 0;
  }
}


int main(int argc, char* argv[]){
    
  int rank, size, Ve[Ne], pos;
  MPI_Init(&argc, &argv);

 	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Status status;
  MPI_Request request;
  
  
  
  if (rank==0){
    int ocupados[size],Vm[Nm], contadorTotal;
    pos=0;  contadorTotal=0;

    for(int i=0; i<size; i++){ocupados[i]= 0;}
    
    //inicializo el vector principal con valores aleatorios
    for (int i=0; i<Nm; i++){
      Vm[i]= rand()%1000000; 
    }
    
    //mientras no se termine de repartir el vector entre procesos esclavos...
    while(pos< Nm){
      //genero la partición del vector a enviar y calculo los primos de ese grupo
      
      printf("Grupo de primos para enviar:\n");
      for (int j=pos;j<pos+1000;j++){
        Ve[j-pos]=Vm[j]; 
        if(es_primo(Vm[j])==1){
          printf("%d, ", Vm[j]);
        }
      }
      pos += 1000;
      
      
      
      for(int p=1;p<size; p++){
        //MPI_Test(&request, &flag, &status);
        if(ocupados[p]==0){
          printf("\n\nEnviando 1000 elementos a proceso %d", p);
          MPI_Isend( &Ve, Ne, MPI_INT, p,0,MPI_COMM_WORLD, &request);
          ocupados[p]=1;
          break;
        }
      }           
      
      printf("\n%d elementos del vector enviados en total \n \n", pos);
      
      
      int flag, contador;
      for(int p=1; p<size; p++){
        MPI_Iprobe(p, 2, MPI_COMM_WORLD, &flag, &status);
        if (flag)
        {
          MPI_Irecv(&contador,1,MPI_INT, p,2,MPI_COMM_WORLD, &request);
          ocupados[p]=0;
          MPI_Wait(&request, &status);
          contadorTotal+=contador;  
          printf("\nproceso: %d, encuentra %d primos. \n\n", p, contador);
        }
      }
    }
    
    //cuando acaban los envios imprimo el numero de primos del vector y comunico a los esclavos que ya no hay mas envios
    printf("\nEn el vector hay %d numeros primos en total. ", contadorTotal);
    
    for(int p=1; p<size; p++){
      int f=0;
      MPI_Isend(&f, 1, MPI_INT, p, 1,MPI_COMM_WORLD, &request); 
    }
    printf("\nFin del proceso 0\n");
  }else{
      int fin=1;
      int flag;
      
      for (int i=0;i<1000;i++){ 
        Ve[i]=0;
      }
      
      while(fin){
      //printf("%d ", pos);
        MPI_Iprobe(0, 0, MPI_COMM_WORLD, &flag, &status);
        if (flag)
        {
          MPI_Irecv(&Ve,Ne,MPI_INT, 0,0,MPI_COMM_WORLD, &request);
          MPI_Wait(&request, &status);  
          int contador=0;
          for (int i=0;i<1000;i++){
            contador+=es_primo(Ve[i]);
            
          } 
           // printf(" primos de:%d son %d. ", rank, contador);
            MPI_Isend( &contador, 1, MPI_INT, 0,2,MPI_COMM_WORLD, &request);
          /*printf("-fin del vector.             
          
                                                                 -");*/
          //MPI_Isend( &contador, 1, MPI_INT, 1,0,MPI_COMM_WORLD, &request);
        }
        
        //comprobamos si hay un mensaje del maestro que indique el final de los envios
        MPI_Iprobe(0, 1, MPI_COMM_WORLD, &flag, &status);
        if (flag)
        {
          MPI_Irecv(&fin, 1,MPI_INT, 0,1,MPI_COMM_WORLD, &request);
          printf("\nFin del proceso %d.\n", rank);
        }
     } 
  }

  
  
  MPI_Finalize();
	return 0;
}