#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 5

int main(int argc, char* argv[]){
	
	
	int matriz[N][N],rank, size, count, blocklens[N], displacements[N];
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size ( MPI_COMM_WORLD , &size);
	MPI_Datatype indexed_type;
	MPI_Status status;
	
	if(rank==0){
		count=N;
		
		for(int x=0;x<N;x++){
			blocklens[x]= N-x; 
            displacements[x]=x*N+x;
            for(int y=0;y<N;y++){
			  matriz[x][y]=y+x*N;
			  printf("%d - ", matriz[x][y]);
			}
			printf("\n");
		}
			
	
		MPI_Type_indexed(count, blocklens, displacements, MPI_INT, &indexed_type);
		MPI_Type_commit(&indexed_type);
			
		
		for(int p=0; p<size; p++){
			MPI_Send(matriz, 1, indexed_type, p, 0,MPI_COMM_WORLD);
		}
	}
	else{
		int sizeb=((N*N-N)/2)+5;
		//int b[sizeb];
		int b[N*N];
		MPI_Recv(b, sizeb , MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		printf("Soy proceso %d \n", rank);
		for(int i=0; i<sizeof(b)/sizeof(int); i++){
			printf("%d ", b[i]);
		}
		printf("\n");


	}
}