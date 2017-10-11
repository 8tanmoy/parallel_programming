#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char * argv[])
{
	if(argc != 2)
	{
		exit(0);
	}
	else
	{
		int nthreads = atoi(argv[1]);
		int N = 1800*1200;
		FILE *fp;
		fp = fopen("picture.inp", "r");
		int i,j;
		int *img = (int *) malloc(N * sizeof(int));
		for (i = 0; i < N; i++)
		{
		        fscanf(fp, "%d", &img[i]);
		}
		fclose(fp);
		omp_set_num_threads(nthreads);
		int h0, h1, h2, h3, h4, h5, h6;
		double start, runtime, mintime;
		mintime = 100.000;
		for (i = 0; i < 10; i++)
{
h0 = 0; h1 = 0; h2 = 0; h3 = 0; h4 = 0; h5 = 0; h6 = 0;
start = omp_get_wtime();
#pragma omp parallel for reduction(+:h0,h1,h2,h3,h4,h5,h6) schedule(auto)
		for(j = 0; j < N; j++)
		{
			if(img[j] == 0) {h0++;}
			if(img[j] == 1) {h1++;}
			if(img[j] == 2) {h2++;}
			if(img[j] == 3) {h3++;}
			if(img[j] == 4) {h4++;}
			if(img[j] == 5) {h5++;}
			if(img[j] == 6) {h6++;}
		}
runtime = omp_get_wtime() - start;
if ((runtime*1000) < mintime ) {mintime = runtime;}
}
		printf("%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n", h0, h1, h2, h3, h4, h5, h6, nthreads);
		printf("%f\n", mintime);
		free(img);
		return(0);
	}
}

