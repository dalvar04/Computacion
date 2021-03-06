#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define N 120
#define P 4
//para 4 procesos -> P=2
//para 9 procesos -> P=3
//para 16 procesos -> P=4

int main(int argc, char* argv[]){

   
  int rank, new_rank, size, new_size, reorder; 
   int buff[(N/P) * (N/P)];
   int dims[2], period[2],coords[2], A[N][N];
  	int localSum = 0;
	int globalSum = 0;
   
   srand(time(NULL));
	
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Status status;
  MPI_Comm new_comm;
  
  MPI_Request request;
  
  MPI_Datatype block;
   MPI_Type_vector(N/P, N/P, N, MPI_INT, &block);	
  
   MPI_Type_commit(&block);
  
   dims[0]=P; dims[1]=P; reorder=0;
   period[0]=0; period[1]=1;
  
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, period, reorder, &new_comm);
    
	
    MPI_Comm_rank(new_comm,&new_rank);
    
   MPI_Comm_size(new_comm, &new_size);
   
  
   
   if(new_rank==0){
	int total = 0;
     for(int i=0; i<N; i++){
       for(int j=0; j<N; j++){
         A[i][j]=rand()%9; 
		 total += A[i][j];
         //printf("%d ",A[i][j]);
       }
	   //printf("\n");
     }

		printf("La suma total de forma secuencial es %d\n", total);
	
     for(int i = 0; i < P; i++){
       for(int j =0; j< P ; j++){
         coords[0] = i;
         coords[1] = j;
         MPI_Cart_rank(new_comm, coords, &new_rank);

		  MPI_Isend(&(A[i * (N/P)][j * (N/P)]), 1, block, new_rank, 1, new_comm, &request);

       }
       
       
       }
	   
	   MPI_Recv(&buff, (N/P)*(N/P), MPI_INT, 0, 1, new_comm, &status);
	   
		for(int p = 0; p <  (N/P)*(N/P); p++){
			localSum += buff[p];
		
			
		}
		printf("Soy el proceso 0. Mi suma local es %d\n", localSum);
     
   } else {
        
		 MPI_Recv(&buff, (N/P)*(N/P), MPI_INT, 0, 1, new_comm, &status);
				
		for(int p = 0; p <  (N/P)*(N/P); p++){
			localSum += buff[p];

		}
		
		printf("Soy el proceso %d. Mi suma local es %d\n", new_rank, localSum);
   
   
   
   }
		 MPI_Reduce(&localSum, &globalSum,1, MPI_INT, MPI_SUM, 1, new_comm);
		 
		 if(new_rank == 1){
			 printf("La suma total en paralelo es %d \n", globalSum);
			 
			 
		 }
         

     
 
 
  MPI_Finalize();
	return 0;
}