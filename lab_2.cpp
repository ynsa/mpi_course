//#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "C:\Program Files (x86)\Microsoft SDKs\MPI\Include\mpi.h"
#include <iostream>

using namespace std;

double f(double x) {
	return pow(x, 2);
}

int main(int argz, char **argv) {
	//cout << argz << endl;
	//cout << argv[0] << endl;
	//cout << argv[1] << endl;
	//cout << argv[2] << endl;
	//system("PAUSE");
	//return 0;
	printf("here\n");
	int source = 0;
	int dest = 0;
	int tag_block = 1;
	int tag_ask = 2; // ask for a new block

	int myrank = 0;
	int p = 0;
	int n = atoi(argv[1]);
	int r = atoi(argv[2]);
	int block_count = ceil((double)n / r);

	MPI_Status status;
	MPI_Init(&argz, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	int a = 0;
	int b = 10;
	double h = (double)(b - a) / n;
	int flag = 1;

	double buf[] = { 0, 0 };

	if (myrank == 0) {
		printf("h: %d | n: %d ", h, n);
		for (int i = 0; i < block_count; ++i) {
			MPI_Recv(&flag, 1, MPI_INT, MPI_ANY_SOURCE, tag_ask, MPI_COMM_WORLD, &status);
			printf("MPI_Recv from %d", status.MPI_SOURCE);
			double x_start = a + i * r * h;
			double x_n = 0;
			if (i == block_count - 1 && n % r != 0) {
				x_n = n % r;
			}
			else {
				x_n = r;
			}
			double buf_root[] = { x_start, x_n };
			printf("buf_root for %d: x: %f n: %f", status.MPI_SOURCE, x_start, x_n);
			MPI_Send(buf_root, 2, MPI_DOUBLE, status.MPI_SOURCE, tag_block, MPI_COMM_WORLD);
		}
		printf("MPI_Bcast");
		MPI_Bcast(&flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
		double total_sum = 0;
		MPI_Reduce(buf, &total_sum, p, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		cout << "Total sum: " << total_sum << endl;
	}
	else {
		MPI_Request req;
		double local_sum = 0;
		
		while (true) {
			MPI_Isend(&flag, 1, MPI_INT, 0, tag_ask, MPI_COMM_WORLD, &req);
			printf("MPI_Isend from %d", myrank);
			MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			printf("MPI_Probe from %d | tag: %d", status.MPI_TAG);
			if (status.MPI_TAG == tag_ask) {
				MPI_Reduce(&local_sum, NULL, p, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
				break;
			}
			else {
				MPI_Recv(buf, 2, MPI_DOUBLE, 0, tag_block, MPI_COMM_WORLD, &status);
				double local_local_sum = 0;
				double x = buf[0];
				for (int i = 0; i < (int)buf[1]; ++i) {
					local_local_sum += (f(x) + f(x + h)) * h / 2;
					x = x + h;
				}
				local_sum += local_local_sum;
			}

		}
	}
	MPI_Finalize();
	system("PAUSE");
}