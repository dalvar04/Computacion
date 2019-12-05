#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]){
	int rank, new_rank, row;
  
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm row_comm;
	
	row=rank%3;
	
	MPI_Comm_split(MPI_COMM_WORLD, row, rank, &row_comm);
	
	MPI_Comm_rank(row_comm, &new_rank);
	
	
	printf("soy proceso %d de MPI_COMM_WORLD y el proceso %d de row_comm \n", rank, new_rank);

}