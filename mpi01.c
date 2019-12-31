#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define Nm 1000000
#define Ne 1000


//funcion calcula primos precisamente
int es_primo_preciso(int n){
  int a=0;
  for(int i=1; i<=n;i++){
  
    if(n%i==0){ 
      a++;
      if(a>2 || a==2 && i!=n){return 0;}
    }
  } 
  if(a==2){
    return 1;
  }
  else{
    return 0;
  }
}

//funcion que filtra primos
int es_primo(int n){
  if(n%2!=0 && n%3!=0 && n%5!=0 && n%7!=0 && n%11!=0 && n%13!=0 || n==2 || n==3 || n==5 || n==7 || n==11 || n==13)
  {
    return es_primo_preciso(n);
  }
  else
  {
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
    //varibles para medir el tiempo de ejecucion
    double t1, t2, t3;  
    t1=MPI_Wtime();
     
    int ocupados[size],Vm[Nm], contadorTotal, contadorSecuencial;
    pos=0;  contadorTotal=0, contadorSecuencial=0;

    //utilizo el vector ocupados para controlar el reparto dinamico del vecto. los inicializo a 0, como desocupados.
    for(int i=0; i<size; i++){ocupados[i]= 0;}
    
    //inicializo el vector principal con valores aleatorios
    for (int i=0; i<Nm; i++){
      Vm[i]= rand()%1000000; 
    }
    
    //mientras no se termine de repartir el vector entre procesos esclavos...
    while(pos< Nm){
      
      //Compruebo que haya algun proceso disponible para poder enviarle
      int libre=0;
      for(int p=1;p<size; p++){
        if(ocupados[p]==0){
          libre=1;
          break;
        }
      }
      
      if(libre==1){ //si hay algun proceso libre en el vector realizo la reparticion
      
        //genero la partición del vector a enviar y calculo los primos de ese grupo
        printf("Grupo de primos para enviar:\n");
        for (int j=pos;j<pos+1000;j++){
          Ve[j-pos]=Vm[j]; 
          if(es_primo(Vm[j])==1){
            printf("%d, ", Vm[j]);
            contadorSecuencial++;
          }
        }
        pos += 1000; 
        
        //envio la particion del vector al primer proceso que este libre
        for(int p=1;p<size; p++){
          //printf("Voy a enviar a %d\n", p);
          if(ocupados[p]==0){
            printf("\n\nEnviando 1000 elementos a proceso %d", p);
            MPI_Isend( &Ve, Ne, MPI_INT, p,0,MPI_COMM_WORLD, &request);
            ocupados[p]=1;
            break;
          }
        }           
        
        printf("\n%d elementos del vector enviados en total \n \n", pos);
      }
      
      //Compruebo si el maestro espera recibir mensaje de algun esclavo, y en ese caso lo recibe
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
    
   // Una vez se ha enviado todo el vector, nos aseguramos de recibir todos los envios de los procesos esclavo que faltaban.
    int libre=0; 
    while(libre==0){ //mientras no esten todos los procesos desocupados...
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
      for(int p=1;p<size; p++){
        if(ocupados[p]==1){
          libre=0;
          break;
        }
        else {libre=1;}
      }
    }
    
    //cuando acaban los envios imprimo el numero de primos del vector y comunico a los esclavos que ya no hay mas envios
    printf("\nEn el vector hay %d numeros primos en total. ", contadorTotal);
    
    for(int p=1; p<size; p++){
      int f=0;
      MPI_Isend(&f, 1, MPI_INT, p, 1,MPI_COMM_WORLD, &request); 
    }
    printf("\nPrimos calculados secuencialmente: %d\n", contadorSecuencial);
    printf("\nFin del proceso 0\n");
    
    t2=MPI_Wtime();
    t3=t2-t1;
    printf("Tiempo total del programa: %lf segundos.\n", t3);
  }else{ //Si es proceso esclavo...
  
      int fin=1;
      int flag;
      
      for (int i=0;i<1000;i++){
        Ve[i]=0;
      }
      
      while(fin){ //Mientras el maestro todavía tenga elementos del vector principal que enviar...
      
        //Compruebo si tengo que recibir un mensaje del maestro
        MPI_Iprobe(0, 0, MPI_COMM_WORLD, &flag, &status);
        if (flag)
        {
          MPI_Irecv(&Ve,Ne,MPI_INT, 0,0,MPI_COMM_WORLD, &request);
          MPI_Wait(&request, &status); 
          
          //contabilizo los primos del vector recibido 
          int contador=0;
          for (int i=0;i<1000;i++){ 
            contador+=es_primo(Ve[i]);
            
          } 
           //Envio el contador de primos del vector recibido al proceso maestro
            MPI_Isend( &contador, 1, MPI_INT, 0,2,MPI_COMM_WORLD, &request);
          
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