#include <mpi.h>

int main(int argz, char **argv){
	int source = 0;
	int dest = 0;
	int tag = 150;
	char message[100];
	int myrank = 0;
	int size = 0;
	MPI_status status;
	MPI_Init(&argz, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	if (myrank != 0){
		sprint(message, "Hello from process %d", myrank);
		// тег должен совпадать при передаче и принятии
		MPI_Send(message, strlen(message), MPI_CHAR, dest, tag, MPI_COMM_WORLD);
	}
	else{
		for(source = 1; source < size; source++){
			MPI_Recv(message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
			printf("%s\n", message + 1); // +1 для символа завершения строки

		}
	}
	MPI_Finalize(...);
}