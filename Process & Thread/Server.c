#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef struct SubTotal
{
	   int inti;
       int array2[sizeof(inti)*2];
        sem_t S, E, F;
}subtotal;

typedef struct Metrix_C
{
		int inti1;
        int array1[sizeof(inti1)*M*K];
        sem_t S, E, F;
}metrix;

typedef struct Metrix_A
{
		int inti2;
        int arrayA[sizeof(inti2)*M*N];
        sem_t SA, EA, FA;
}metrixA;

typedef struct Metrix_B
{
		int inti3;
        int arrayB[sizeof(inti3)*N*K];
        sem_t SB, EB, FB;
}metrixB;


int main(int argc, char *argv[])
{
if(argc == 6)
{
	//File Pointers
	FILE* pointerA=NULL;
	FILE* pointerB=NULL;
		
	//Variables Related to the 3 Metrix
	int M = atoi(argv[3]);
	int N = atoi(argv[4]);
	int K = atoi(argv[5]);
	int metrix_A[M][N];
	int metrix_B[N][K];
	int metrix_C[M][K];
	int r1,c1,r2,c2,i,j;

	//Variables Related to 4 Shared Memories
	int shmid,shmidm;
	key_t key;
	key_t keym;
	size_t SHMSZ = sizeof(subtotal);
	size_t SHMSZM = sizeof(metrix);
	size_t SHMSZB = sizeof(metrixB);
	size_t SHMSZA = sizeof(metrixA);
	int size = sizeof(array2[]);
	int size1 = sizeof(array1[]);
	int size2 = sizeof(arrayA[]);
	int size3 = sizeof(arrayB[]);
		
	int sema1,sema2,sema3;
		
	//Variables Related to Child Processes
	int x=0;
	int child_id;
		
	//First Metric
	pointerA = fopen(argv[1], "r");

	for(r1=0;r1<M;r1++){
		for(c1=0;c1<N;c1++){
			fscanf(pointerA, "%d", &metrix_A[r1][c1]);
		}
	}

	//Printing methods Start (For the A  metrix)
	printf("Elements of the First Metric (Metric A)\n");
	for(r1=0;r1<M;r1++){
		for(c1=0;c1<N;c1++){
			printf("\t%d", metrix_A[r1][c1]);
		}
		printf("\n");
	}
	printf("\n");
	//prnting method close
	fclose(pointerA);



	//Second Metric
	pointerB = fopen(argv[2], "r");

	for(r2=0;r2<N;r2++){
		for(c2=0;c2<K;c2++){
			fscanf(pointerB, "%d", &metrix_B[r2][c2]);
		}
	}

	//Printing methods Start (For the B  metrix)
	printf("Elements of the Second Metric (Metric B)\n");
	for(r2=0;r2<N;r2++){
		for(c2=0;c2<K;c2++){
			printf("\t%d", metrix_B[r2][c2]);
		}
		printf("\n");
	}
	printf("\n");
	//prnting method close
	fclose(pointerB);

	//Multiply the above two Metrices and find the values of the third metrix
	for (i=0;i<r1;i++)
	{
		for (j=0;j<c2;j++)
		{
			metrix_C[i][j] = 0;
			for (int k=0;k<c1;k++)
			{
				metrix_C[i][j] = metrix_C[i][j] + metrix_A[i][k] * metrix_B[k][j];
			}

		}
		
	}

	//Printing methods Start(for the C metrix)
	printf("Elements of the Result Metric after the Multiplication(Metric C)\n");
	for(i=0;i<r1;i++){
		for(j=0;j<c2;j++){
			printf("\t%d", metrix_C[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	//Printing methods End

	//Create a Shared memory to Store the A Metrix
	if ((shmid = shmget(key, SHMSZA, IPC_CREAT | 0666)) < 0) {
	perror("Shared Memory Creation error");
	exit(1);
	}
	//Link the Datastructure of Subtotal with the shared Memory
	metrixB (*SharedMemA) = shmat(shmid, NULL, 0);
	//Create a Semaphore Value to Maintain the critical section and store the address in the shared Memory
	if((sema1=sem_init(&SharedMemA->SA,22,1)) == -1){
	perror("Semaphore Value for Critical Section is not Created");
	}
	//Create a Semaphore Value to Identify the Buffer is Empty and store the address in the shared Memory
	if((sema2=sem_init(&SharedMemA->EA,22,size)) == -1){
	perror("Semaphore Value to Check the Buffer is Empty is not Created");
	}
	//Create a Semaphore Value to Identify the Buffer is Full and store the address in the shared Memory 
	if((sema3=sem_init(&SharedMemA->FA,22,0)) == -1){
	perror("Semaphore Value to Check the Buffer is Full is not Created");
	}
	
	//Create a Shared memory to Store the B Metrix
	if ((shmid = shmget(key, SHMSZB, IPC_CREAT | 0666)) < 0) {
		perror("Shared Memory Creation error");
		exit(1);
	}
	//Link the Datastructure of Subtotal with the shared Memory 
	metrixB *SharedMemB = shmat(shmid, NULL, 0);
	//Create a Semaphore Value to Maintain the critical section and store the address in the shared Memory
	if((sema1=sem_init(&SharedMemB->SB,22,1)) == -1){
	perror("Semaphore Value for Critical Section is not Created");
	}
	//Create a Semaphore Value to Identify the Buffer is Empty and store the address in the shared Memory
	if((sema2=sem_init(&SharedMemB->EB,22,size)) == -1){
	perror("Semaphore Value to Check the Buffer is Empty is not Created");
	}
	//Create a Semaphore Value to Identify the Buffer is Full and store the address in the shared Memory
	if((sema3=sem_init(&SharedMemB->FB,22,0)) == -1){
	perror("Semaphore Value to Check the Buffer is Full is not Created");
	}
	
	
	//Create a Shared memory to Store the Sub Total
	if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
		perror("Shared Memory Creation error");
		exit(1);
	}
	//Link the Datastructure of Subtotal with the shared Memory
	subtotal *SharedMem1 = shmat(shmid, NULL, 0);
	//Create a Semaphore Value to Maintain the critical section and store the address in the shared Memory
	if((sema1=sem_init(&SharedMem1->S,22,1)) == -1){
	perror("Semaphore Value for Critical Section is not Created");
	}
	//Create a Semaphore Value to Identify the Buffer is Empty and store the address in the shared Memory
	if((sema2=sem_init(&SharedMem1->E,22,size)) == -1){
	perror("Semaphore Value to Check the Buffer is Empty is not Created");
	}
	//Create a Semaphore Value to Identify the Buffer is Full and store the address in the shared Memory
	if((sema3=sem_init(&SharedMem1->F,22,0)) == -1){
	perror("Semaphore Value to Check the Buffer is Full is not Created");
	}


	//Create a Shared memory to Store the C Metrix
	if ((shmid = shmget(keym, SHMSZM, IPC_CREAT | 0666)) < 0) {
		perror("Shared Memory Creation error");
		exit(1);
	}
	//Link the Datastructure of C Metrix with the shared Memory 
	metrix *SharedMem2 = shmat(shmidm, NULL, 0);
	//Create a Semaphore Value to Maintain the critical section and store the address in the shared Memory
	if((sema1=sem_init(&SharedMem2->S,22,1)) == -1){
	perror("Semaphore Value for Critical Section is not Created");
	}
	//Create a Semaphore Value to Identify the Buffer is Empty and store the address in the shared Memory
	if((sem_init(&SharedMem2->E,22,size1)) == -1){
	perror("Semaphore Value to Check the Buffer is Empty is not Created");
	}
	//Create a Semaphore Value to Identify the Buffer is Full and store the address in the shared Memory	
	if((sem_init(&SharedMem2->F,22,0)) == -1){
	perror("Semaphore Value to Check the Buffer is Full is not Created");
	}

	//Put The Values of the A Metrix in to the Shared Memory
	for(i=0;i<M;i++){
		for(j=0;j<N;j++){
			sem_wait(&SharedMemB->EA);
			sem_wait(&SharedMemB->SA);
			SharedMem2->array1[i][j] = metrix_A[i][j];
			sem_post(&SharedMem2->SA);
			sem_post(&SharedMem2->FA);
		}
	}
	
	//Put The Values of the B Metrix in to the Shared Memory
	for(i=0;i<N;i++){
		for(j=0;j<K;j++){
			sem_wait(&SharedMemB->EB);
			sem_wait(&SharedMemB->SB);
			SharedMem2->array1[i][j] = metrix_B[i][j];
			sem_post(&SharedMem2->SB);
			sem_post(&SharedMem2->FB);
		}
	}
	
	//Put The Values of the C Metrix in to the Shared Memory
	for(i=0;i<M;i++){
		for(j=0;j<K;j++){
			sem_wait(&SharedMem2->E);
			sem_wait(&SharedMem2->S);
			SharedMem2->array1[i][j] = metrix_C[i][j];
			sem_post(&SharedMem2->S);
			sem_post(&SharedMem2->F);
		}
	}

	//Create New Child Processes
	for(i=0;i<M;i++){	
		if((child_id=fork())==0)
		{
			int stotal;
			int num[1][4];
			for(i=0;i<x;i++){
				for(j=0;j<K;j++){
					sem_wait(&SharedMem2->full);
					sem_wait(&SharedMem2->mutex);
					num[i][j] = SharedMem2->array1[i][j];
					sem_post(&SharedMem2->mutex);
					sem_post(&SharedMem2->empty);
					stotal = stotal + num[i][j];
				}
			}
			int pid = getpid();
			sem_wait(&SharedMem1->empty);
			sem_wait(&SharedMem1->mutex);
			SharedMem1->array2[0] = pid;
			SharedMem1->array2[1] = stotal;
			sem_post(&SharedMem1->mutex);
			sem_post(&SharedMem1->full);

			exit(0);
		}
		if(id>0)
		{
			int Fulltotal;
			int status;
			wait(&status);
			sem_wait(&SharedMem1->full);
			sem_wait(&SharedMem1->mutex);
			int pid = SharedMem1->array2[0];
			int subTota = SharedMem1->array2[1];
			sem_post(&SharedMem1->mutex);
			sem_post(&SharedMem1->empty);
			printf("process ID %d\n",pid);
			printf("Sub Total %d\n",subTota);
			Fulltotal = Fulltotal+subTota;
		}
		x=x+1;
	}
	printf("Total of all the Sub Totals: %d\n",Fulltotal);   
}
else{
	printf("Invalid Number of Arguments");
}
exit(0);
}

