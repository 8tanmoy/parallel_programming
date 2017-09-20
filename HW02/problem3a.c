#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

void generate(int* img, int imsize);
void writeimg(int* img, int* ftr, int imsize, int fsize, char* fname);
int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("invalid number of arguments\n");
		exit(0);
	}
	int imsize;
	int fsize;
	imsize = atoi(argv[1]);
	fsize = atoi(argv[2]);
	if (imsize > 1024 || fsize > 9)
	{
		printf("invalid img/feature size\n");
		exit(0);
	}
	if (imsize < fsize)
	{
		printf("feature size should be should not be more than image size\n");
		exit(0);
	}
	int *img = (int *) malloc(imsize * imsize * sizeof(int));
	int *ftr = (int *) malloc(fsize * fsize * sizeof(int));
	if (img == NULL || ftr == NULL)
	{
		printf("malloc failed\n");
		exit(0);
	}
	generate(img, imsize);
	generate(ftr, fsize);
	writeimg(img, ftr, imsize, fsize, "problem3.dat");

	free(img);
	free(ftr);
	return(0);
}
void generate(int* img, int imsize)
{
	for (int i = 0; i < (imsize * imsize); i++)
	{
		img[i] = 2 * (rand() % 2) - 1;
		//img[i] = i;
	}
}
void writeimg(int* img, int* ftr, int imsize, int fsize, char* fname)
{
	FILE *fo;
	fo = fopen(fname, "w");
	for (int i = 0; i < imsize; ++i)
	{
		for (int j = 0; j < imsize; j ++)
		{
			fprintf(fo, "%2d\t", img[(imsize * i) + j]);
		}
		fprintf(fo, "\n");
	}
	for (int i = 0; i < fsize; ++i)
	{
		for (int j = 0; j < fsize; j ++)
		{
			fprintf(fo, "%2d\t", ftr[(fsize * i) + j]);
		}
		fprintf(fo, "\n");
	}
	fclose(fo);
}
