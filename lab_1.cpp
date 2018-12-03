#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "C:\Program Files (x86)\Microsoft SDKs\MPI\Include\mpi.h"
#include <iostream>

using namespace std;

int main(int argz, char **argv) {
	int main_rank = 0;
	int number_amount;
	int world_rank = 0;
	int size = 0;
	MPI_Init(&argz, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (world_rank != 0) {
		number_amount = world_rank;
		char* numbers = new char[number_amount];
		for (int i = 0; i < number_amount; ++i) {
			numbers[i] = i + 1 + '0';
		}

		MPI_Send(numbers, number_amount, MPI_CHAR, main_rank, 0, MPI_COMM_WORLD);
	}
	else {
		MPI_Status status, status_resv;
		for (int i = 1; i < size; ++i) {
			MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

			MPI_Get_count(&status, MPI_CHAR, &number_amount);

			char* number_buf = (char*)malloc(sizeof(char) * (number_amount + 1)); // should add '/0' for right answer!

			MPI_Recv(number_buf, number_amount, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status_resv);
			number_buf[number_amount] = '\0';

			printf_s("%s\n",  number_buf);
			cout << endl;
			printf("Received %d numbers from %d.\n", strlen(number_buf), status_resv.MPI_SOURCE);
			free(number_buf);
		}
	}
	MPI_Finalize();
}