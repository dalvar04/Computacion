#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]){

  int valorEntero, position, rank;
  float valorReal1, valorReal2;
  char buffer[1000];
  MPI_Status status;

  MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   
  if(rank==0){
     printf("Introduce un numero entero: \n");
     scanf ("%d", &valorEntero);
     printf("Introduce un numero real: \n");
     scanf ("%lf", &valorReal1);
     printf("Introduce otro numero real: \n");
     scanf ("%lf", &valorReal2);
     
     position=0;     
     MPI_Pack(&valorEntero, 1, MPI_INT, buffer, 1000, &position, MPI_COMM_WORLD);
     MPI_Pack(&valorReal1, 1, MPI_FLOAT, buffer, 1000, &position, MPI_COMM_WORLD);
     MPI_Pack(&valorReal1, 1, MPI_FLOAT, buffer, 1000, &position, MPI_COMM_WORLD);
     
     MPI_Bcast(buffer, position, MPI_PACKED, 0, MPI_COMM_WORLD);
  }
  else{
    MPI_Recv(buffer, 1000, MPI_PACKED, 0, 0, MPI_COMM_WORLD, &status);
    position = 0;
    MPI_Unpack(buffer, 1000, &position, &valorEntero, rank, MPI_INT, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 1000, &position, &valorReal1, rank, MPI_FLOAT, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 1000, &position, &valorReal2, rank, MPI_FLOAT, MPI_COMM_WORLD);
    
    printf("Soy el proceso %d -> valor Entero: %d, valor Real 1: %lf, valor Real 2: %lf", rank, valorEntero, valorReal1, valorReal2);
  }

}