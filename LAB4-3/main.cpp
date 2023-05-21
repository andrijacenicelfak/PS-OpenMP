#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

/*
3. Napisati OpenMP program kojim se pronalazi ukupan broj prostih brojeva između 1 i N,
podelom iteracija petlje između različitih niti sa i bez korišćenja odredbe redukcije za
kombinovanje parcijalnih rezultata u nitima. Uporediti vremena izvršenja u oba slučaja sa
sekvencijalnim vremenom izvršenja. Uporediti ova rešenja za različite vrednosti N.
*/
int prost(int a) {
	if (a == 2 || a == 3) return 1;
	for (int i = 3; i < a / 2; i += 2) {
		if (a % i == 0) return 0;
	}
	return 1;
}

void BrojProstih(int N, int brojProcesora) {
	double time, t1, t2, t3, t4,t5;
	int rez = 0;

	printf("Calculating for N : %d\n", N);

	time = omp_get_wtime();
	for (int i = 2; i < N; i++)
	{
		int p = prost(i);
		rez += p;
	}
	t1 = omp_get_wtime() - time;

	time = omp_get_wtime();
#pragma omp parallel for
	for (int i = 2; i < N; i+= brojProcesora)
	{
		int p = prost(i);
#pragma omp critical
		rez += p;
	}
	t2 = omp_get_wtime() - time;

	rez = 0;
	time = omp_get_wtime();
#pragma omp parallel for
	for (int i = 2; i < N; i += brojProcesora)
	{
		int mrez = 0;
		for (int j = i; (j < N) && (j < i + brojProcesora); j++) {
			mrez += prost(j);
		}
#pragma omp critical
		rez += mrez;
	}
	t4 = omp_get_wtime() - time;

	rez = 0;
	time = omp_get_wtime();
#pragma omp parallel for reduction(+:rez)
	for (int i = 2; i < N; i++) {
		rez += prost(i);
	}
	t3 = omp_get_wtime() - time;

	rez = 0;
	time = omp_get_wtime();
#pragma omp parallel for reduction(+:rez)
	for (int i = 2; i < N; i += brojProcesora)
	{
		for (int j = i; (j < N) && (j < i + brojProcesora); j++) {
			rez += prost(j);
		}
	}
	t5 = omp_get_wtime() - time;

	printf("Serial time :\t %.5lfms\n", t1 * 1000);
	printf("Critical time :\t %.5lfms, Ubrzanje : %2.2lf\n", t2 * 1000, t1 / t2);
	printf("C. time ++ :\t %.5lfms, Ubrzanje : %2.2lf\n", t4 * 1000, t1 / t4);
	printf("Reduce time :\t %.5lfms, Ubrzanje : %2.2lf\n", t3 * 1000, t1 / t3);
	printf("R time++ :\t %.5lfms, Ubrzanje : %2.2lf\n\n", t5 * 1000, t1 / t5);
}

int main() {
	int num_of_cores = omp_get_num_procs();
	omp_set_num_threads(num_of_cores);
	printf("Num of proc : %d\n", num_of_cores);

	BrojProstih(10, num_of_cores);
	BrojProstih(50, num_of_cores);
	BrojProstih(500, num_of_cores);
	BrojProstih(1000, num_of_cores);
	BrojProstih(10000, num_of_cores);
	BrojProstih(500000, num_of_cores);

	return 0;
}