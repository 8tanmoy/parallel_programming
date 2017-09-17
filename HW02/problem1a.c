#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


int swap();
int partition(int A[], int p, int r);
int sort(int A[], int p, int r);
int printarray(int arr[], int N);
int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		FILE *fp;

		fp = fopen("problem1.in", "r");
		if (fp == NULL)
		{
			puts("No data file");
			exit(0);
		}
		else
		{
			int N;
			fscanf(fp, "%d", &N);
			printf("%d\n", N);
			int A[N];
			for (int k = 0; k < N; k++)
			{
				fscanf(fp, "%d", &A[k]);
			}

			clock_t start, end;
			float cpu_time;
			int p = 0;
			int r = N - 1;

			start = clock();
			sort(A, p, r);
			end = clock();
			cpu_time = (end - start)/1000.0;
			printf("%f\n", cpu_time);
		}
		fclose(fp);
	}
	else if (argc == 2)
	{
		int N;
		N = atoi(argv[1]);
		printf("%d\n", N);
		
		clock_t start, end;
		float cpu_time;
		int A[N];
		int p = 0;
		int r = N - 1;

		srand(time(NULL));
		for (int k = 0; k < N; k++)
		{
			A[k] = rand();
		}

		start = clock();
		sort(A, p, r);
		end = clock();
		cpu_time = (end - start)/1000.0;
		printf("%f\n", cpu_time);
	}
	else
	{
		puts("wrong number of arguments\n");
		exit(0);
	}
	return(0);
}
int partition(int A[], int p, int r)
{
	int x;
	int i, j;

	x = A[r];
	i = p - 1;
	for (j = p; j < r; j ++)
	{
		if (A[j] <= x)
		{
			i++;
			swap(&A[i], &A[j]);
		}
	}
	swap(&A[i+1], &A[r]);
	return(i+1);
}
int swap(int *x, int *y)
{
	int t;
	t = *x;
	*x = *y;
	*y = t;
	return(0);
}
int sort(int A[], int p, int r)
{
	if (p < r)
	{	
		int q;
		q = partition(A, p, r);
		sort(A, p, q - 1);
		sort(A, q + 1, r);
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