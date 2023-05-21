#include <omp.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

	omp_set_num_threads(4);
	#pragma omp parallel
	{
		int ID = omp_get_thread_num();
		if (ID == 0)
			printf("Thread num : %d\n", omp_get_num_threads());
		printf("I am : %d\n", ID);
	}

	return 0;
}