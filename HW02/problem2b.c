#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

void scan(int in[], int N, unsigned long* cpu_time);
int main(int argc, char* argv[])
{
	if (argc != 1)
	{
		printf("please do not use command line arguments\n");
		exit(0);
	}	
	else
	{
		FILE *fp;
		fp = fopen("problem2b.out","w");
		for (int j = 5; j < 13; j++)
		{
			unsigned long cpu_time;
			int size = pow(2, j);
			int A[size];
			srand(time(NULL));
			for (int k = 0; k < size; k++)
			{
				A[k] = (rand() % 21) - 10;
			}
			scan(A, size, &cpu_time);
			fprintf(fp, "%d\t%ld\n",size, cpu_time);
		}
		fclose(fp);
	}
	return(0);
}
void scan(int in[], int N, unsigned long* cpu_time)
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
}