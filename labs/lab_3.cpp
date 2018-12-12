// lab_3.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "C:\Program Files (x86)\Microsoft SDKs\MPI\Include\mpi.h"
#include <iostream>

using namespace std;

int* init_block(int* matrix, int block_start, int str_in_block, int col_in_block, int str_len) {
	for (int i = 1; i < str_in_block; ++i) {
		for (int j = block_start; j < block_start + col_in_block; j++) {
			matrix[i * str_len + j] = matrix[(i - 1) * str_len + j] + 1;
			cout << i * str_len + j << ": " << matrix[i * str_len + j] << " | ";
		}
		cout << endl;
	}
	return matrix;
}

void print_matrix(int* matrix, int N_1, int N_2) {
	for (int i = 0; i < N_1; ++i) {
		for (int j = 0; j < N_2; ++j) {
			printf("%d ", matrix[i * N_2 + j]);
		}
		printf("\n");
	}
}

int main(int argz, char **argv) {
	int myrank = 0;
	int p = 0;
	if (sizeof(argv) / sizeof(char) <= 3){
		
		//cout << argv[0] << " " << argv[1]  << " " << argv[2] << " " << argv[3] << endl;
		printf("Covalsky, we have a problem: no enought argv.\n");
		return 0;
	}
	MPI_Status status;
	MPI_Init(&argz, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	if (p == 1) {
		MPI_Finalize();
		return 0;
	}

	cout << "here..." << endl;
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);
	int r = atoi(argv[3]);
	//printf("Myrank: %d | N_1: %d | N_2: %d | r: %d\n", myrank, N_1, N_2, r);
	if (n <= 0 || m <= 0 || r <= 0) {
		printf("Covalsky, we have a problem with argv.\n");
		MPI_Finalize();
		return 0;

	}
	int str_in_block = n / p;
	int block_in_row = m / r;


	if (myrank == p - 1) {
		str_in_block += (n % p);
	}

	int init_value = 0;

	MPI_Request req;
	int* matrix = new int[(str_in_block + 1) * m];
	int* prev_str;
	int len = r;

	if (myrank == 0) {
		matrix = new int[str_in_block * m];
		for (int j = 0; j < block_in_row; ++j) {
			int current_block_num = j;
			cout << "current_block_num: " << current_block_num << endl;
			if (j == block_in_row - 1) {
				len += m % r;
				//cout << "Len: " << len << endl;
			}

			for (int k = 0; k < len; ++k) {
				matrix[j * r + k] = j * r + k;
			}
			for (int l = 1; l < str_in_block; ++l) {
				for (int k = 0; k < len; ++k) {
					matrix[l * m + j * r + k] = matrix[(l - 1) * m + j * r + k] + 1;
					//cout << l * N_1 + j * r + k << ": " << matrix[l * N_1 + j * r + k];
				}
				//cout << endl;
			}
			int* help = new int[len];
			for (int k = 0; k < len; ++k) {
				help[k] = matrix[(str_in_block - 1) * m + current_block_num * r + k];
			}
			//print_matrix(help, 1, len);
			//MPI_Isend(&matrix[(str_in_block - 1) * N_1 + current_block_num * r], len, MPI_INT, myrank + 1, current_block_num, MPI_COMM_WORLD, &req);
			MPI_Isend(help, len, MPI_INT, myrank + 1, current_block_num, MPI_COMM_WORLD, &req);

		}
		int* result_matrix = new int[n * m];

		memcpy(&result_matrix, matrix, str_in_block * m);
		for (int i = 1; i < p; ++i) {
			cout << "Master: try to receive... i: " << i << endl;
			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			int element_count = 0;
			MPI_Get_count(&status, MPI_INT, &element_count);
			printf("%d: MPI_Probe from %d | tag: %d | element_count: %d\n", myrank, status.MPI_SOURCE, status.MPI_TAG, element_count);
			printf("Matrix index start: %d\n", status.MPI_SOURCE * str_in_block * m);
			MPI_Recv(&matrix[status.MPI_SOURCE * str_in_block * m], element_count, MPI_INT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);
			cout << matrix[status.MPI_SOURCE * str_in_block * m + element_count - 1] << endl;
		}
		cout << "Finnaly..." << endl;
		//print_matrix(result_matrix, n, m);
	}
	else {
		//for (int i = 0; i < str_in_block; ++i) {
			for (int j = 0; j < block_in_row; ++j) {
				MPI_Probe(myrank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
				//printf("%d: MPI_Probe from %d | tag: %d\n", myrank, status.MPI_SOURCE, status.MPI_TAG);
				int current_block_num = status.MPI_TAG;
				if (status.MPI_TAG == block_in_row - 1) {
					len += m % r;
				}
				prev_str = new int[len];
				MPI_Recv(&matrix[current_block_num * r], len, MPI_INT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);
				//print_matrix(prev_str, 1, len);
				//cout << "current_block_num: " << current_block_num << endl;
				/*for (int k = 0; k < len; ++k) {
					matrix[current_block_num * r + k] = prev_str[k];
				}*/
				//memcpy(&matrix[current_block_num * r], prev_str, len);
				matrix = init_block(matrix, current_block_num * r, str_in_block + 1, len, m);
				//print_matrix(matrix, str_in_block + 1, m);
				if (myrank != p - 1) {
					/*for (int k = 0; k < len; ++k) {
						prev_str[k] = matrix[str_in_block * N_2 + current_block_num * r + k];
					}*/
					MPI_Isend(&matrix[str_in_block * m + current_block_num * r], len, MPI_INT, myrank + 1, current_block_num, MPI_COMM_WORLD, &req);
				}

			}
			cout << "OK: " << myrank << endl;
			MPI_Send(&matrix[m], str_in_block * m, MPI_INT, 0, 666, MPI_COMM_WORLD);
			//print_matrix(matrix, str_in_block, N_2);
	}
	MPI_Finalize();
	//system("PAUSE");
}