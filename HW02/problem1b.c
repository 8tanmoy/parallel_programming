#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

int printarray(int arr[], int N);
int compare(const void* a,const void* b);
int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		FILE *fpi;

		fpi = fopen("problem1.in", "r");
		if (fpi == NULL)
		{
			puts("No data file");
			exit(0);
		}
		else
		{
			int N;
			fscanf(fpi, "%d", &N);
			printf("%d\n", N);
			int A[N];
			for (int k = 0; k < N; k++)
			{
				fscanf(fpi, "%d", &A[k]);
			}

			struct timeval start, end;
			unsigned long cpu_time;

			gettimeofday(&start, NULL);
			qsort(A, N, sizeof(int), &compare);
			gettimeofday(&end, NULL);
			cpu_time = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec;
			printf("%lu\n", cpu_time);

			FILE *fpo;
			fpo = fopen("problem1.out","w"); //change this to problem1.out
			for (int k = 0; k < N; k++)
			{
				fprintf(fpo, "%d\n", A[k]);
			}
			fclose(fpo);
		}
		fclose(fpi);
	}
	else if (argc == 2)
	{
		int N;
		N = atoi(argv[1]);
		printf("%d\n", N);
		
		struct timeval start, end;
		unsigned long cpu_time;
		int A[N];

		srand(time(NULL));
		for (int k = 0; k < N; k++)
		{
			A[k] = rand();
		}

		gettimeofday(&start, NULL);
		qsort(A, N, sizeof(int), &compare);
		gettimeofday(&end, NULL);
		cpu_time = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec;
		printf("%lu\n", cpu_time);

		FILE *fpo;
		fpo = fopen("problem1.out","w"); //change this to problem1.out
		for (int k = 0; k < N; k++)
		{
			fprintf(fpo, "%d\n", A[k]);
		}
		fclose(fpo);

	}
	else
	{
		puts("wrong number of arguments\n");
		exit(0);
	}
	return(0);
}
int printarray(int arr[], int N)
{
	printf("[");
	for (int o = 0; o < N; o ++)
	{
		printf("%10d,", arr[o]);
	}
	printf("]\n");
	return(0);
}
int compare(const void* a,const void* b)
{
	return(*(int*)a - *(int*)b);
}