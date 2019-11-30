#include <stdio.h>
#include <mpi.h>



int main(int argc, char* argv[]){

	int size, rank, machlen, tag;
  char messageSend[100], messageRecv[100], machName[MPI_MAX_PROCESSOR_NAME];
	MPI_Status info;
  MPI_Request request;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Get_processor_name(machName, &machlen);
   
  if (rank == 0){
  
  sprintf(messageSend,"Soy el proceso %d, mi machine name es: %s", rank, &machName );
    MPI_Sendrecv(&messageSend,100, MPI_CHAR, rank+1,rank+1, &messageRecv,100, MPI_CHAR,size-1,size-1, MPI_COMM_WORLD,&info);
   //MPI_Send(&messageSend, 100,MPI_CHAR,rank+1,tag,MPI_COMM_WORLD);
  //MPI_Wait(&request, &info);
  
  //printf("Proceso %d envia a proceso %d un mensaje\n", rank, rank+1);
  printf("Proceso %d recibe el mensaje: %s. Envia el suyo al proceso %d \n", rank, messageRecv, rank+1);
  }
  else if(rank< size-1){
    
    sprintf(messageSend,"Soy el proceso %d, mi machine name es: %s", rank, &machName );
    
    //MPI_Isend(&messageSend, 100,MPI_CHAR,rank+1,tag,MPI_COMM_WORLD, &request);
    MPI_Sendrecv(&messageSend,100, MPI_CHAR, rank+1,rank+1, &messageRecv,100, MPI_CHAR,rank-1,rank-1, MPI_COMM_WORLD,&info);
    //MPI_Irecv(&messageRecv,100,MPI_CHAR,rank-1,tag,MPI_COMM_WORLD, &request);
    
    MPI_Wait(&request, &info);  
    printf("Proceso %d recibe el mensaje: %s. Envia el suyo al proceso %d \n", rank, messageRecv, rank+1);
    
    
 }
 else{
   sprintf(messageSend,"Soy el proceso %d, mi machine name es: %s", rank, &machName );
   MPI_Sendrecv(&messageSend,100, MPI_CHAR, 0, 0, &messageRecv,100, MPI_CHAR, rank-1, rank-1, MPI_COMM_WORLD,&info);
   //MPI_Recv(&messageRecv,100,MPI_CHAR,rank-1,tag,MPI_COMM_WORLD, &info);
   //MPI_Wait(&request, &info);
   //printf("Proceso %d recibe mensaje del proceso anterior: %s \n", rank, messageRecv);
   printf("Proceso %d recibe el mensaje: %s. Envia el suyo al proceso %d \n", rank, messageRecv, 0);
 }
}