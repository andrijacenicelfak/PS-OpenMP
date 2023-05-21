#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
/*
1. Napisati sekvencijalni program kojim se vrši množenje dve matrice A i B reda N.
Korišćenjem OpenMP direktive izvršiti paralelizaciju petlje, tako da se izvrši distribucija
iteracija između niti.
Podesiti broj niti tako da bude jednak broju jezgara računara. Izmeriti vreme množenja matrica
za sekvencijalni slučaj i za slučaj posle uvođenja openMP direktiva. Uporediti oba rešenja za
različite dimenzije matrica.
*/

void MatrixMul(int N) {
	int* A, * B, * C;
	A = (int*)malloc(sizeof(int) * N * N);
	B = (int*)malloc(sizeof(int) * N * N);
	C = (int*)malloc(sizeof(int) * N * N);
	double time;
	double t1, t2;
	printf("Calculating for N : %d\n", N);
#pragma omp parallel for
	for (int index = 0; index < N * N; index++) {
		A[index] = index;
		B[index] = ((index / N) == (index % N)) ? 1 : 0;
	}
	time = omp_get_wtime();
	for (int index = 0; index < N * N; index++) {
		int i = index / N;
		int j = index % N;
		C[index] = 0;
		for (int k = 0; k < N; k++)
			C[index] += A[i*N + k] * B[k* N + j];
	}
	t1 = omp_get_wtime() - time;
	printf("SER : %4.5lfms\n", (t1) * 1000);
	time = omp_get_wtime();
#pragma omp parallel for
	for (int index = 0; index < N * N; index++) {
		int i = index / N;
		int j = index % N;
		C[index] = 0;
		for (int k = 0; k < N; k++)
			C[index] += A[i * N + k] * B[k * N + j];
	}
	t2 = omp_get_wtime() - time;
	printf("OMP : %4.5lfms\n", (t2) * 1000);
	printf("Ubrzanje : %2.2lf\n", t1 / t2);

	//for (int i = 0; i < N; i++) {
	//	for (int j = 0; j < N; j++) {
	//		printf("%2d ", C[i * N + j]);
	//	}
	//	printf("\n");
	//}
	free(A);
	free(B);
	free(C);
}

int main(int argc, char* argv[]) {

	int num_of_cores = omp_get_num_procs();
	omp_set_num_threads(num_of_cores);
	printf("Num of proc : %d\n", num_of_cores);
	MatrixMul(5);
	MatrixMul(10);
	MatrixMul(15);
	MatrixMul(20);
	MatrixMul(30);
	MatrixMul(60);

	return 0;
}