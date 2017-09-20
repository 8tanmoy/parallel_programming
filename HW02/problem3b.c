#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

void writeimg(int* img, int* ftr, int imsize, int fsize, char* fname);
void invert(int* img, int imsize);
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

	//printf("reaading image from \"problem3.dat\".\nWill fail if image size and feature size are different from that entered in problem3a\n");
	FILE *fi;
	fi = fopen("problem3.dat","r");
	for (int i = 0; i < (imsize * imsize); i++)
	{
		fscanf(fi, "%d", &img[i]);
	}
	for (int j = 0; j < (fsize * fsize); j++)
	{
		fscanf(fi, "%d", &ftr[j]);
	}
	fclose(fi);

	//writeimg(img, ftr, imsize, fsize, "problem3-check.dat");
	invert(img, imsize);
	writeimg(img, ftr, imsize, fsize, "problem3b.dat");

	free(img);
	free(ftr);
	return(0);
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
void invert(int* img, int imsize)
{
	int *rowbuff = (int *) malloc(imsize * sizeof(int));
	for (int i = 0; i < (imsize / 2); i++)
	{
		memcpy(rowbuff, &img[imsize * i], imsize * sizeof(int));
		memcpy(&img[imsize * i], &img[(imsize - 1 - i) * imsize], imsize * sizeof(int));
		memcpy(&img[(imsize - 1 - i) * imsize], rowbuff, imsize * sizeof(int));
	}
	free(rowbuff);
}
