#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define N 1600
#define P 2

int main(int argc, char* argv[]){
   
   int rank, new_rank, size, new_size, reorder; 
   int buff[(N/P) * (N/P)];
   int dims[3], period[3],coords[3], A[3][N][N];
 	 int localMax = 0;

   int globalMaxR = 0;
   int globalMaxG  = 0;
   int globalMaxB = 0;
   
   srand(time(NULL));
	
	
	 MPI_Init(&argc, &argv);
	 MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Status status;
   MPI_Comm new_comm;
  
   MPI_Request request;
  
  //Creo el tipo de datos MPI_Type_vector
   MPI_Datatype block;
   MPI_Type_vector(N/P, N/P, N, MPI_INT, &block);	
  
   MPI_Type_commit(&block);
  
  //creo la topología cartesiana
   dims[0]=3; dims[1]=P; dims[2]=P; reorder=0;
   period[0]=0; period[1]=1;  period[2]=0;
  
   MPI_Cart_create(MPI_COMM_WORLD, 3, dims, period, reorder, &new_comm);
    
	
   MPI_Comm_rank(new_comm,&new_rank);
   MPI_Comm_size(new_comm, &new_size);
   

  
  //si soy el proceso maestro...
   if(new_rank==0){
       //inicializo la matriz con valores aleatorios de 0 a 255
       for(int k=0; k<3; k++){
         for(int i=0; i<N; i++){
           for(int j=0; j<N; j++){
           
           A[k][i][j]=rand()%256; 	     
          
          // printf("%d ",A[k][i][j]);
           }//printf("\n");
  	     }//printf("\n\n");
       }
    
	    //reparto los cuadrantes de las matrices entre los 12 procesos
     for(int k=0; k<3; k++){
       for(int i = 0; i < P; i++){
         for(int j =0; j< P ; j++){
         
           coords[0] = k;
           coords[1] = i;
           coords[2] = j;
           
           MPI_Cart_rank(new_comm, coords, &new_rank);
           
            //printf("soy %d y envio a %d\n", new_rank, A[k][i * (N/P)][j * (N/P)]);
		        MPI_Isend(&(A[k][i * (N/P)][j * (N/P)]), 1, block, new_rank, 1, new_comm, &request);

           }
           new_rank=0;
         }
       }
	   //proceso 0 recibe su cuadrante y calcula su maximo
	   MPI_Recv(&buff, (N/P)*(N/P), MPI_INT, 0, 1, new_comm, &status);
	   		//printf("Soy el proceso 0: ");
  		for(int p = 0; p <  (N/P)*(N/P); p++){
  			if(buff[p] > localMax){
          localMax = buff[p];
        }
         //printf("%d ", buff[p]);
       
       
  		
  			
  		}
		printf("Soy el proceso 0. Mi maximo local es %d\n", localMax);
   
     
   } else {
        //los procesos 1-11 reciben su cuadrante y calculan su maximo
		   MPI_Recv(&buff, (N/P)*(N/P), MPI_INT, 0, 1, new_comm, &status);
			//	printf("Soy el proceso %d: ", new_rank);
  		for(int p = 0; p <  (N/P)*(N/P); p++){
  			if(buff[p] > localMax){
          localMax = buff[p];
        }
        // printf("%d ", buff[p]);
       
  		}
		
	  	printf("\nSoy el proceso %d. Mi maximo local es %d\n", new_rank, localMax);
   }
   
  //Cada proceso envía el maximo del cuadrante que han recibido al maestro con diferente etiqueta, dependiendo de su matriz(sea R,G o B)
   if(new_rank<4){
 
     MPI_Isend(&localMax, 1, MPI_INT, 0, 1, new_comm, &request);

   }else if (new_rank<8){
  
     MPI_Isend(&localMax, 1, MPI_INT, 0, 2, new_comm, &request);

   }else{
        MPI_Isend(&localMax, 1, MPI_INT, 0, 3, new_comm, &request);
    
   }
  
	
      //Si es el maestro recibe los maximos locales de los demas procesos y calcula con ello el maximo de cada una de las 3 matrices (R, G y B)
		 if(new_rank == 0){
        int num;
        for(int i = 0; i < size; i ++){
           
           
           MPI_Recv(&num, 1, MPI_INT, i, MPI_ANY_TAG, new_comm, &status);
           
           if(i < 4){
             printf("El proceso %d ha enviado como maximo el numero %d que pertenece a la matriz R\n", i, num);
             if(num > globalMaxR){
               globalMaxR = num;
               }
           
           
           } else if (i < 8){
               printf("El proceso %d ha enviado como maximo el numero %d que pertenece a la matriz G\n", i, num);
               if(num > globalMaxG){
                 globalMaxG = num;
               }
           
           }else if (i < 12){
               printf("El proceso %d ha enviado como maximo el numero %d que pertenece a la matriz B\n", i, num);
               if(num > globalMaxB){
                 globalMaxB = num;
               }          
           }     
        }
        //Se imprime el maximo de R, G y B
			  printf("\nEl valor maximo de la matriz R es %d \n", globalMaxR);
        printf("\nEl valor maximo de la matriz G es %d \n", globalMaxG);
        printf("\nEl valor maximo de la matriz B es %d \n", globalMaxB);
						 
		 }

  MPI_Finalize();
	return 0;
}