////#define _CRT_SECURE_NO_WARNINGS
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "C:\Program Files (x86)\Microsoft SDKs\MPI\Include\mpi.h"
//#include <iostream>
//
//using namespace std;
//
//int main(int argz, char **argv) {
//	int main_rank = 0;
//	int number_amount;
//	int world_rank = 0;
//	int size = 0;
//	MPI_Init(&argz, &argv);
//	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//
//	if (world_rank != 0) {
//		number_amount = world_rank;
//		char* numbers = new char[number_amount];
//		for (int i = 0; i < number_amount; ++i) {
//			numbers[i] = i + 1 + '0';
//		}
//
//		MPI_Send(numbers, number_amount, MPI_CHAR, main_rank, 0, MPI_COMM_WORLD);
//	}
//	else{
//		MPI_Status status, status_resv;
//		for (int i = 1; i < size; ++i) {
//			MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
//
//			MPI_Get_count(&status, MPI_CHAR, &number_amount);
//
//			char* number_buf = (char*)malloc(sizeof(char) * number_amount);
//
//			MPI_Recv(number_buf, number_amount, MPI_CHAR, status.MPI_SOURCE, 0, MPI_COMM_WORLD, &status_resv);
//
//			for (int i = 0; i < number_amount; ++i) {
//				cout << number_buf[i];
//			}
//			//printf_s("%s\n",  number_buf);
//			cout << endl;
//			printf("Received %d numbers from %d.\n", strlen(number_buf), status_resv.MPI_SOURCE);
//			free(number_buf);
//		}
//	}
//	MPI_Finalize();
//	//int source = 0;
//	//int dest = 0;
//	//int tag = 150;
//	//char message[100];
//	//int myrank = 0;
//	//int size = 0;
//	//MPI_Status status;
//	//MPI_Init(&argz, &argv);
//	//MPI_Comm_size(MPI_COMM_WORLD, &size);
//	//MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
//	//if (myrank != 0) {
//	//	string s(myrank, '*');
//	//	//cout << "myrank: " << myrank << "  " << s.c_str() << endl;
//	//	//string text;
//	//	//sprintf(text, "Hello from process %d!", myrank);
//	//	sprintf_s(message, "Hello from process %d! %s", myrank, s);
//	//	// тег должен совпадать при передаче и принятии
//	//	//printf("r: %d  len: %d\n", myrank, strlen(message));
//	//	cout << "strlen(message): " << strlen(message) << endl;
//	//	MPI_Send(message, strlen(message), MPI_CHAR, dest, tag, MPI_COMM_WORLD);
//	//}
//	//else {
//	//	
//	//	MPI_Status status1, status2;
//	//	for (source = 1; source < size; source++) {
//
//	//		//MPI_Probe(MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status1);
//	//		//printf_s("Probe: rank: %d, error: %d", status1.MPI_SOURCE, status1.MPI_ERROR);
//	//		int len = 23;
//	//		cout << len << endl;
//	//		//MPI_Get_count(&status1, MPI_CHAR, &len);
//	//		//int i = status1.MPI_SOURCE - 1;
//	//		int i = 0;
//	//		char* get_message = (char*)malloc(sizeof(char) * (len));
//	//		//cout << get_message[len - 1] << endl;
//	//		MPI_Recv(&get_message, len, MPI_CHAR, 1, tag, MPI_COMM_WORLD, &status2);
//	//		//printf_s("Get: %s\n", get_message);
//	//		//cout << "strlen(message[" << i << "]): " << strlen(get_message) << endl << endl << "------------------\n";
//
//	//		//free(get_message);
//	//	}
//
//	//	//char** messages = new char*[size];
//	//	//for (source = 1; source < size; source++) {
//
//	//	//	MPI_Probe(MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
//	//	//	printf_s("Probe: rank: %d, error: %d", status.MPI_SOURCE, status.MPI_ERROR);
//	//	//	int len;
//	//	//	MPI_Get_count(&status, MPI_CHAR, &len);
//	//	//	//char* nmessage = new char[source];
//	//	//	int i = status.MPI_SOURCE - 1;
//	//	//	char* get_message = new char[len];
//	//	//	//messages[i] = new char[len + 1];
//	//	//	//resultBuffer = (char**)malloc(sizeof(char*) * size);
//	//	//	//resultBuffer[0] = (char*)malloc(sizeof(char) * (strlen(str) + 1));
//	//	//	//MPI_Recv(&messages[i], len, MPI_CHAR, i + 1, tag, MPI_COMM_WORLD, &status2);
//	//	//	MPI_Recv(&get_message, len, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status2);
//	//	//	//cout << endl << "Sourse: " << source << " S: " << i << "  len: " << status2.internal[0] << endl;
//	//	//	//messages[i] = new char[source];
//	//	//	//cout << "len: " << len << endl;
//	//	//	//memcpy(messages[i], nmessage, source);
//	//	//	//cout << strlen(messages[i]) << endl;
//	//	//	cout << strlen(get_message) << endl;
//	//	//	//realloc(messages[i], status2.internal[0]);
//	//	//	//cout << messages[i] << endl;
//	//	//	//printf("%s\n", messages[i]);
//	//	//	//printf("2: %s\n", message);
//
//	//	//	cout << "strlen(message[" << i << "]): " << strlen(get_message) << endl << endl << "------------------\n";
//	//	//	//cout << "strlen(message[" << i << "]): " << strlen(messages[i]) << endl << endl << "------------------\n";
//
//	//	//}
//	//	//cout << "size = " << size << endl;
//	//}
//	//MPI_Finalize();
//	//system("PAUSE");
//}