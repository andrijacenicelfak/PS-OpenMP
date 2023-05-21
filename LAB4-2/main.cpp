#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
/*
2. Napisati sekvencijalni program kojim se generiše skalarni proizvod dva vektora. Napisati
OpenMP program kojim se generiše skalarni proizvod dva vektora, podelom iteracija petlje
između različitih niti sa i bez korišćenja odredbe redukcije za kombinovanje parcijalnih rezultata
u nitima. Uporediti vremena izvršenja u oba slučaja sa sekvencijalnim vremenom izvršenja.
Uporediti ova rešenja za različite dimenzije vektora.
*/

void ScalarMul(int N) {
	int* A = (int*)malloc(sizeof(int) * N);
	int* B = (int*)malloc(sizeof(int) * N);
	int rez = 0;
	double time = 0, t1, t2, t3;
	for (int i = 0; i < N; i++) {
		A[i] = i;
		B[i] = 1;
	}
	printf("Calculating for N : %d\n", N);
	time = omp_get_wtime();
	for (int i = 0; i < N; i++) {
		rez += A[i] * B[i];
	}
	t1 = omp_get_wtime() - time;
	time = omp_get_wtime();
	rez = 0;
#pragma omp parallel for
	for (int i = 0; i < N; i++)
#pragma omp critical
		rez += A[i] * B[i];
	t2 = omp_get_wtime() - time;
	rez = 0;
	time = omp_get_wtime();
#pragma omp parallel for reduction(+: rez)
	for (int i = 0; i < N; i++)
		rez += A[i] * B[i];
	t3 = omp_get_wtime() - time;

	printf("Serial time :\t %4.5lfms\n", t1 * 1000);
	printf("Critical time :\t %4.5lfms, Ubrzanje : %2.2lf\n", t2 * 1000, t1/t2);
	printf("Reduce time :\t %4.5lfms, Ubrzanje : %2.2lf\n\n", t3 * 1000, t1/t3);

	free(A);
	free(B);
}

int main() {
	int num_of_cores = omp_get_num_procs();
	omp_set_num_threads(num_of_cores);
	printf("Num of proc : %d\n", num_of_cores);
	ScalarMul(5);
	ScalarMul(10);
	ScalarMul(40);
	ScalarMul(100);
	ScalarMul(1000);
	ScalarMul(5000);
	ScalarMul(10000);
	ScalarMul(50000);
	return 0;
}