#include <stdio.h>
#include <mpi.h>

#define N 8

int main(int argc, char* argv[]){

	int size, rank, i, from, to, ndat, part, tag, VA[N], suma, comprobacion;
	MPI_Status info;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
   int rebuf[N/size];
	//Inicializo el vector
	for (i=0; i<N; i++) {
		VA[i] = 0;
	}
	for (i=0; i<N/size; i++){
    rebuf[i]=0;
  }
	
	//Si soy maestro
	if (rank == 0){
		for (i=1; i<N; i++) {
			VA[i] = i;
      comprobacion=comprobacion+VA[i]*2;
		}
	   printf("Comprobacion: %d", comprobacion);
   }
   suma=0;
		//MPI_Bcast(VA, N, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Scatter(VA, N/size, MPI_INT, rebuf , N/size, MPI_INT, 0, MPI_COMM_WORLD);
   
	  
	  for (i=0; i<N/size; i++){
       rebuf[i]= rebuf[i]*2;
       suma=suma+rebuf[i];
       
    }
    int sumaGlobal;
    MPI_Reduce(&suma, &sumaGlobal,1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    //MPI_Gather(rebuf, N/size,MPI_INT, VA, N/size, MPI_INT, 0, MPI_COMM_WORLD);
    
    
	//si soy maestro...
	if(rank==0) {
    
			
			printf(" Suma: %d ", sumaGlobal);
   }
   

	MPI_Finalize();
	return 0;
}
