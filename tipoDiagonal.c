#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 5

int main(int argc, char* argv[]){
	
	int matriz[N][N], rank, size;	
	
	MPI_Init(&argc, &argv);
	MPI_Datatype block;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Status status;

	MPI_Type_vector(N, 1, N+1, MPI_INT, &block);	
	MPI_Type_commit(&block);
	
	for (int i=0; i<N; i++){
			for(int j=0; j<N; j++){
				matriz[i][j]=0;
			}
	}
	
	if(rank==0){
		for(int i=0;i<N; i++){ matriz[i][i]=i*i; /*printf("%d ", matriz[i][i]);*/}
		
		//envio
		for(int p=1; p<size; p++){
			MPI_Send(&(matriz[0][0]), 1, block, p, 0, MPI_COMM_WORLD);
		}
	}else{
		
		int buff[N];
		MPI_Recv(&buff, N, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		printf("Soy proceso: %d \n", rank);
		for (int i=0; i<N; i++){
			
				//printf("%d ", o);
			printf("%d ", buff[i]);
			printf("\n");
			
		}
		
	}
	MPI_Finalize();
	return 0;
}
