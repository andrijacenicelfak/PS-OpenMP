#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
/*
5. Napisati OpenMP program kojim se generiše maksimalna vrednost elemanata vektora,
podelom iteracija petlje između različitih niti korišćenjem direktive critical za kombinovanje
parcijalnih rezultata u nitima. Uporediti vremena izvršenja u oba slučaja sa sekvencijalnim
vremenom izvršenja. Uporediti ova rešenja za različite vrednosti dimenizija vektora.
*/

void FindMax(int N) {
	int* A = (int*)malloc(sizeof(int) * N);
	int cmax = 0;
	double time = 0, t1, t2, t3;
	for (int i = 0; i < N; i++) {
		A[i] = rand() % N;
	}
	printf("Calculating for N : %d\n", N);
	cmax = -9999;
	time = omp_get_wtime();
	for (int i = 0; i < N; i++) {
		if (A[i] > cmax)
			cmax = A[i];
	}
	t1 = omp_get_wtime() - time;
	printf("rez : %d\n", cmax);

	cmax = -9999;
	time = omp_get_wtime();
#pragma omp parallel for
	for (int i = 0; i < N; i++) {
#pragma omp critical
		if (A[i] > cmax) {
			cmax = A[i];
		}
	}
	t2 = omp_get_wtime() - time;
	//printf("rez : %d\n", cmax);
	printf("Serial time :\t %4.5lfms\n", t1 * 1000);
	printf("Critical time :\t %4.5lfms, Ubrzanje : %2.2lf\n", t2 * 1000, t1 / t2);

	free(A);
}

int main() {
	int num_of_cores = omp_get_num_procs();
	omp_set_num_threads(num_of_cores);
	printf("Num of proc : %d\n", num_of_cores);
	FindMax(10);
	FindMax(100);
	FindMax(1000);
	FindMax(5000);
	FindMax(200000);
	FindMax(600000);
	FindMax(1000000);
	FindMax(4000000);
	return 0;
}