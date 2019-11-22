#include <stdio.h>
#include <mpi.h>

#define N 10

int main(int argc, char* argv[]){

	int size, rank, i, from, to, ndat, part, tag, VA[N];
	MPI_Status info;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	//Inicializo el vector
	for (i=0; i<N; i++) {
		VA[i] = 0;
	}
	
	
	
	//Si soy maestro
	if (rank == 0){
		for (i=1; i<N; i++) {
			VA[i] = i;
		}
	}	
	MPI_Bcast(VA, N, MPI_INT, 0, MPI_COMM_WORLD);
	
	//si soy esclavo...
	if(rank!=0) {
		printf("Proceso %d: VA antes de recibir datos: \n",rank);
		
  }
		
		// 1. Imprimir el vector antes de recibir nada
     for (i=1; i<N; i++) {
			printf("%d", VA[i]);
  
    
    /*   
		// 3. Calcular cuántos datos se han recibido del maestro
     MPI_Get_count(&info,MPI_INT,&ndat);
		
		printf("Proceso %d recibe VA de %d: tag %d, ndat %d; \nVA = ", rank, info.MPI_SOURCE, info.MPI_TAG, ndat);
		
		for (i=0; i<ndat; i++) {
			printf("%d  ",VA[i]);
		}*/
		printf("\n\n");
	}

	MPI_Finalize();
	return 0;
}
