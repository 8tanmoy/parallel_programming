#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void scan(int in[], int N, long* last, unsigned long* cpu_time);
int printarray(int arr[], int N);
int main(int argc, char* argv[])
{
	int N;
	long last;
	unsigned long cpu_time;
	char* fname;
	FILE *fp;

	if (argc != 2)
	{
		printf("invalid number of arguments\n");
		exit(0);
	}
	else
	{
		fname = argv[1];
	}

	fp = fopen(fname, "r");
	fscanf(fp, "%d", &N);
	int in[N];
	for (int i = 0; i < N; i++)
	{
		fscanf(fp, "%d", &in[i]);
	}
	fclose(fp);
	//printarray(in, N);
	scan(in, N, &last, &cpu_time);
	printf("%d\n%ld\n%ld\n", N, last, cpu_time);
	return(0);
}
void scan(int in[], int N, long* last, unsigned long* cpu_time)
{
	long out[N];
	out[0] = 0;
	struct timeval start, end;
	gettimeofday(&start, NULL);
	for (int i = 1; i < N; i++)
	{
		out[i] = in[i - 1] + out[i - 1];
	}
	gettimeofday(&end, NULL);
	*cpu_time = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec;
	*last = out[N - 1];
	//printarray(out, N);
}
int printarray(int arr[], int N)
{
	printf("[");
	for (int o = 0; o < N; o ++)
	{
		printf("%6d,", arr[o]);
	}
	printf("]\n");
	return(0);
}