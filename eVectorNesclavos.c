#include <stdio.h>
#include <mpi.h>

#define N 100

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
	
		//Añadir el código necesario para:
		
		//1. Calcular cuántos datos se envían a cada esclavo (por ejemplo, repartir 100 datos entre 4 esclavos-> 25 a cada uno)
     part=N/size-1;
      
		//2. Para todos los esclavos...
		//		2.1 Identificar destino y tag
		//      2.2. Distinguir entre los esclavos y el último esclavo
     for(i=1; i<size; i++){
       to=i; tag=0;
       if(i!= size-1){
         MPI_Send(&VA[part * (i-1)],part,MPI_INT,to,tag,MPI_COMM_WORLD);
       }
       else{
         part=N/(size-1) +N%(size-1);
	       MPI_Send(&VA[part * (rank-1) +part],part,MPI_INT,to,tag,MPI_COMM_WORLD);
       }
     }
   
   /*
   if (rank!=size-1){
    to=rank; tag=0;
    MPI_Send(&VA[part * (rank-1)],part,MPI_INT,to,tag,MPI_COMM_WORLD);
    }else{
        to=rank; 
    
        part=N/(size-1) +N%(size-1);
		    MPI_Send(&VA[part * (rank-1) +part],part,MPI_INT,to,tag,MPI_COMM_WORLD);*/
    /*to=2;
    MPI_Send(&VA[part * (rank-1)],part,MPI_INT,to,tag,MPI_COMM_WORLD);
    
    to=3;
    MPI_Send(&VA[part * (rank-1)],part,MPI_INT,to,tag,MPI_COMM_WORLD);
   
		//		2.3. Si no es el último esclavo, se le envía el número de datos que se calculó antes
		//		2.3. Si es el último esclavo, se le envían los datos restantes*/
   //}
    
	}
	
	//si soy esclavo...
	else {
		printf("Proceso %d: VA antes de recibir datos: \n",rank);
		
		}
		// Añadir el código necesario para: 
		
		// 1. Imprimir el vector antes de recibir nada
     for (i=0; i<N; i++) {
			printf("%d", VA[i]);
     }
		// 2. Recibir los datos del maestro
     
   
     from = 0; tag = 0;
     if(rank<size-1){
	     MPI_Recv(&VA[(rank-1)*(N/(size-1))],N/(size-1),MPI_INT,from,tag,MPI_COMM_WORLD,&info);
      }
      else{
        MPI_Recv(&VA[(rank-1)*(N/(size-1))+N/(size-1)],N/(size-1)+N%(size-1),MPI_INT,from,tag,MPI_COMM_WORLD,&info);
      }
    
       
		// 3. Calcular cuántos datos se han recibido del maestro
     MPI_Get_count(&info,MPI_INT,&ndat);
		
		printf("Proceso %d recibe VA de %d: tag %d, ndat %d; \nVA = ", rank, info.MPI_SOURCE, info.MPI_TAG, ndat);
		
		for (i=0; i<ndat; i++) {
			printf("%d  ",VA[i]);
		}
		printf("\n\n");
	

	MPI_Finalize();
	return 0;
}