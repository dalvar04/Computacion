#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main(int argc, char* argv[]){
	
	int rank;
	
	MPI_Init(&argc, &argv);
	MPI_Datatype parimpar;
	MPI_Status status;
	
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Type_vector(40/2, 1, 2, MPI_INT, &parimpar);
	MPI_Type_commit(&parimpar);
	
	if(rank==0){
		int v[40];
		for(int i=0; i<40; i++){ v[i]=i; }

		MPI_Send(&v[0], 1, parimpar, 1, 0, MPI_COMM_WORLD);
		MPI_Send(&v[1], 1, parimpar, 2, 0, MPI_COMM_WORLD);
		
	}
	else if(rank==1){
		int buff[40/2];
		MPI_Recv(&buff, 40/2, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		int media=0;
		for(int i=0; i<40/2;i++){
			media+=buff[i];
		}
		media=media/(40/2);
		printf("soy proceso %d, mi media es: %d \n", rank, media);
		
	}else if(rank==2){
		int buff[40/2];
		MPI_Recv(&buff, 40/2, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		int media=0;
		for(int i=0; i<40/2;i++){
			media+=buff[i]; 
		}
		media=media/(40/2);
		printf("soy proceso %d, mi media es: %d \n", rank, media);
	}
	
	
	MPI_Finalize();
	return 0;
}