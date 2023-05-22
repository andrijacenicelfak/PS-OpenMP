#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
/*
6. Napisati OpenMP kod koji sadrži sledeću petlju:
for (i=0;i<m;i++)
	for (j=0;j<n;j++)
		a[i][j]=2*a[i-1][j];
i izvršiti njenu paralelizaciju. Proučiti da li postoje zavisnosti između iteracija i po kom indeksu
je moguća paralelizacija. Ispitati da li zamena petlji uzrokuje promenu u performansama.

*****
i mora da bude 1 na pocetku
*****

a[1][0] = 2 * a[0][0]
a[1][1] = 2 * a[0][1]
a[1][2] = 2 * a[0][2]
a[1][3] = 2 * a[0][3]
a[1][4] = 2 * a[0][4]
a[1][5] = 2 * a[0][5]
....
a[1][n-2] = 2 * a[0][n-2]
a[1][n-1] = 2 * a[0][n-1]

a[2][0] = 2 * a[1][0]
a[2][1] = 2 * a[1][1]
a[2][2] = 2 * a[1][2]
a[2][3] = 2 * a[1][3]
a[2][4] = 2 * a[1][4]
a[2][5] = 2 * a[1][5]
....
a[2][n-2] = 2 * a[1][n-2]
a[2][n-1] = 2 * a[1][n-1]

po j nema zavisnosti dok po i ima

a[1][0] = 2 * a[0][0]
a[2][0] = 2 * a[1][0]
a[3][0] = 2 * a[2][0]

*/
void Petlja(int N) {
	int **a = (int**)malloc(sizeof(int*) *  N);
	for (int i = 0; i < N; i++)
		a[i] = (int*)malloc(sizeof(int) * N);
	double time;
	double t1, t2, t3;

#pragma omp parallel for
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			a[i][j] = j;
		}
	}

	printf("Calculating for N : %d\n", N);
	time = omp_get_wtime();
	for (int i = 1; i < N; i++) {
		for (int j = 0; j < N; j++) {
			a[i][j] = 2 * a[i - 1][j];
		}
	}
	t1 = omp_get_wtime() - time;

	time = omp_get_wtime();
	for (int i = 1; i < N; i++) {
#pragma omp parallel for
		for (int j = 0; j < N; j++) {
			a[i][j] = 2 * a[i - 1][j];
		}
	}
	t2 = omp_get_wtime() - time;

	time = omp_get_wtime();
#pragma omp parallel for
	for (int j = 0; j < N; j++) {
		for (int i = 1; i < N; i++) {
			a[i][j] = 2 * a[i - 1][j];
		}
	}
	t3 = omp_get_wtime() - time;
	printf("Serial time :\t %4.5lfms\n", t1 * 1000);
	printf("for j       :\t %4.5lfms, Ubrzanje : %2.2lf\n", t2 * 1000, t1 / t2);
	printf("zam. j      :\t %4.5lfms, Ubrzanje : %2.2lf\n", t3 * 1000, t1 / t3);
	for (int i = 0; i < N; i++)
		free(a[i]);
	free(a);
}
int main() {
	int num_of_cores = omp_get_num_procs();
	omp_set_num_threads(num_of_cores);
	printf("Num of proc : %d\n", num_of_cores);

	Petlja(10);
	Petlja(100);
	Petlja(1000);
	Petlja(5000);

	return 0;
}
