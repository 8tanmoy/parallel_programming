#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("invalid number of arguments\n");
		exit(0);
	}
	int nthreads = atoi(argv[1]);
	omp_set_num_threads(nthreads);
	static int dim = 1024;
	long int N = dim*dim;
	double start, runtime;
	FILE *fpA;
	FILE *fpB;
	fpA = fopen("inputA.inp", "r");
	fpB = fopen("inputB.inp", "r");
	double *A = (double *) malloc(N * sizeof(double));
	double *B = (double *) malloc(N * sizeof(double));
	double *C = (double *) malloc(N * sizeof(double));

	long int l;
	for (l = 0; l < N; l++)
	{
		fscanf(fpA, "%lf", &A[l]);
		fscanf(fpB, "%lf", &B[l]);
	}
	fclose(fpA);
	fclose(fpB);
	int i, j, k;

	int m, n, nt;
	const int ts = 32;
	nt = dim/ts;

	start = omp_get_wtime();

#pragma omp parallel for schedule(auto)
	for (m = 0; m < dim; m+=ts )
	{
		for (n = 0; n < dim; n+=ts )
		{
			for (i = m; i < (m + ts); i++)
			{
				for (k = 0; k < dim; k++)
				{
					for (j = n; j < (n + ts); j++)
					{
						C[i * dim + j] = C[i * dim + j] + A[i * dim + k] * B[k * dim + j];
					}
				}
			}
		}
	}

	runtime = omp_get_wtime() - start;
	printf("%lf\n", C[N-1]);
	printf("%lf\n", runtime*1000);
	printf("%d\n", nthreads);
	free(A);
	free(B);
	free(C);
	return(0);
}
