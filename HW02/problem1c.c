#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

int swap(int *x, int *y);
int partition(int A[], int p, int r);
int sort(int A[], int p, int r);
int compare(const void* a,const void* b);
int main(int argc, char *argv[])
{
	if (argc != 1)
	{
		printf("please do not use command line arguments\n");
		exit(0);
	}	
	else
	{	
		FILE *fp;
		fp = fopen("problem1c.out", "w");
		for (int N = 10; N <20; N++)
		{
			struct timeval start, end;
			unsigned long cpu_time;
			int size = pow(2, N);
			int *A = (int *) malloc(size * sizeof(int));
			srand(time(NULL));
			for (int k = 0; k < size; k++)
			{
				A[k] = rand();
			}

			gettimeofday(&start, NULL);
			qsort(A, size, sizeof(int), &compare);
			gettimeofday(&end, NULL);
			cpu_time = ((end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec);
			//printf("%d\t%ld\t", size, cpu_time);
			fprintf(fp, "%d\t%ld\t",size, cpu_time);
			free(A);

			int *B = (int *) malloc(size * sizeof(int));
			int p = 0;
			int r = size - 1;
			srand(time(NULL));
			for (int l = 0; l < size; l++)
			{
				B[l] = rand();
			}
			gettimeofday(&start, NULL);
			sort(B, p, r);
			gettimeofday(&end, NULL);
			cpu_time = ((end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec);
			//printf("%ld\t", cpu_time);
			//printf("\n");
			fprintf(fp, "%ld\n", cpu_time);
			free(B);

		}
		fclose(fp);
	}
	return(0);
}
int compare(const void* a,const void* b)
{
	return(*(int*)a - *(int*)b);
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