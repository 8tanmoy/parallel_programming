#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <unistd.h>

double func(double x);
int main(int argc, char* argv[])
{
	int num = 1000000;
	if(argc > 2)
	{
		printf("invalid number of arguments\n");
		exit(0);
	}

	double step = 100.0/num;
	double sum = 0.0;
	int i;
	char* buff;
	buff = (char *) malloc(10*sizeof(char));
	gethostname(buff, 10);
	double x, start, runtime;
	double s0, s1, s2, s3;
	int nthreads = atoi(argv[1]);
	omp_set_num_threads(nthreads);
	start = omp_get_wtime();

s0 = 17*(func(0) + func(100));
s1 = 59*(func(step) + func(100-step));
s2 = 43*(func(2*step) + func(100 - (2*step)));
s3 = 49*(func(3*step) + func(100 - (3*step)));
s0 = s0 + s1 + s2 + s3;
s0 = s0/48;
	
	int num2 = num - 8;
	double* sint = (double *) malloc(num2 * sizeof(double));
	double* esint = (double *) malloc(num2 * sizeof(double));
	double* cost = (double *) malloc(num2 * sizeof(double));
	double* esincost = (double *) malloc(num2 * sizeof(double));

#pragma omp parallel for private(x) schedule(auto)
	for(i = 0; i < num2; i++)
	{
		x = step*(i+4);
		sint[i] = sin(x);
		esint[i] = exp(sint[i]);
		x = x/40;
		cost[i] = cos(x);
		esincost[i] = esint[i] * cost[i];
	}
#pragma omp parallel for reduction(+:sum) schedule(auto)
	for(i = 0; i < num2; i++)
	{
		sum = sum + esincost[i];
	}
	runtime = omp_get_wtime() - start;
	free(sint);
	free(esint);
	free(cost);
	free(esincost);

	sum = (sum + s0)* step;
	printf("%d\n",nthreads);
	printf("%f\n", 1000.0*runtime);
	printf("%s\n", buff);
	printf("%.10f\n", sum);
}
double func(double x)
{
	double esin, cs, fout;
	esin = exp(sin(x));
	x = x / 40.0;
	cs = cos(x);
	cs = cs*esin;
	return(cs);
}
