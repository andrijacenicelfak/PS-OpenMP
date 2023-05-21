#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
/*
4. Napisati OpenMP program kojim se generiše prosečna vrednost elemanata vektora, podelom
iteracija petlje između različitih niti sa i bez korišćenja odredbe redukcije za kombinovanje
parcijalnih rezultata u nitima. Uporediti vremena izvršenja u oba slučaja sa sekvencijalnim
vremenom izvršenja. Uporediti ova rešenja za različite vrednosti dimenizija vektora.
*/

void Prosecno(int N) {
	int* A = (int*)malloc(sizeof(int) * N);
	double rez = 0;
	double time = 0, t1, t2, t3;
	for (int i = 0; i < N; i++) {
		A[i] = i;
	}
	printf("Calculating for N : %d\n", N);

	time = omp_get_wtime();
	for (int i = 0; i < N; i++)
		rez += A[i] / N;
	t1 = omp_get_wtime() - time;

	rez = 0;
	time = omp_get_wtime();
#pragma omp parallel for
	for (int i = 0; i < N; i++) {
#pragma omp critical
		rez += A[i] / N;
	}
	t2 = omp_get_wtime() - time;
	
	rez = 0;
	time = omp_get_wtime();
#pragma omp parallel for reduction(+:rez)
	for (int i = 0; i < N; i++) {
		rez += A[i] / N;
	}
	t3 = omp_get_wtime() - time;


	printf("Serial time :\t %4.5lfms\n", t1 * 1000);
	printf("Critical time :\t %4.5lfms, Ubrzanje : %2.2lf\n", t2 * 1000, t1 / t2);
	printf("Reduce time :\t %4.5lfms, Ubrzanje : %2.2lf\n\n", t3 * 1000, t1 / t3);

}


int main() {
	int num_of_cores = omp_get_num_procs();
	omp_set_num_threads(num_of_cores);
	printf("Num of proc : %d\n", num_of_cores);

	Prosecno(20);
	Prosecno(100);
	Prosecno(2000);
	Prosecno(10000);
	Prosecno(200000);
	Prosecno(500000);
	return 0;
}