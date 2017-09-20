#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

void invert(int* img, int imsize);
void getRank(int* img,int* ftr,int* rank,int imsize,int fsize,int rsize);
int eleMult(int* buff, int* ftr, int fsize);
void bestmatch(int* rank, int rsize);

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

	//printf("reaading image from \"problem3.dat\".\nWill fail if image size and feature size are different from problem3a\n");
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

	invert(img, imsize);

	int rsize = (imsize - fsize + 1);
	int *rank = (int *) malloc(rsize * rsize * sizeof(int));
	getRank(img, ftr, rank, imsize, fsize, rsize);
	bestmatch(rank, rsize);
	/*
	for (int l = 0; l < rsize; l++)
	{
		for (int m = 0; m < rsize; m++)
		{
			printf("%2d\t", rank[(rsize * l) + m]);
		}
		printf("\n");
	}
	*/
	free(rank);
	free(img);
	free(ftr);
	return(0);
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
void getRank(int* img,int* ftr,int* rank,int imsize,int fsize,int rsize)
{
	for (int i = 0; i < rsize * rsize; i++)
	{
		int *matbuff = (int *) malloc(fsize * fsize * sizeof(int));
		for (int j = 0; j < fsize * fsize; j++)
		{
			matbuff[j] = img[(i % rsize) + imsize * (i / rsize) + j + (imsize - fsize) * (j / fsize)];
		}
		rank[i] = eleMult(matbuff, ftr, fsize);
		free(matbuff);
	}
}
int eleMult(int* matbuff, int* ftr, int fsize)
{
	int num = 0;
	for (int i = 0; i < fsize * fsize; i++)
	{
		num = num + matbuff[i] * ftr[i];
	}
	return(num);
}
void bestmatch(int* rank, int rsize)
{
	int max = rank[0];
	for (int i = 0; i <(rsize * rsize) ; i++)
	{
		if (rank[i] > max)
		{
			max = rank[i];
			i++;
		}
	}
	//printf("max = %d\n", max);
	int nmax = 0;
	for (int i = 0; i <(rsize * rsize) ; i++)
	{
		if (rank[i] == max)
		{
			nmax++;
		}
	}
	if (nmax > 1)
	{
		int maxarr[nmax];
		int i = 0;
		int imax = 0;
		while(i < (rsize * rsize) && imax < nmax)
		{
			if (rank[i] == max)
			{
				maxarr[imax] = i;
				//printf("%d\n", maxarr[imax]);
				imax++;
			}
			i++;
		}
		int mindist = 2*(rsize - 1);
		for (int imax = 0; imax < nmax; imax++)
		{
			int nmindist = 0;
			if (((maxarr[imax] / rsize) + (maxarr[imax] % rsize)) < mindist)
			{
				mindist = ((maxarr[imax] / rsize) + (maxarr[imax] % rsize));
				imax++;
			}

		}
		int nmindist = 0;
		for (int imax = 0; imax < nmax; imax++)
		{
			if (((maxarr[imax] / rsize) + (maxarr[imax] % rsize)) == mindist)
			{
				nmindist++;
			}
		}
		if (nmindist > 1)
		{
			//tie of ties
			int colmax = 0;
			for (int imax = 0; imax < nmax; imax++)
			{
				if ((((maxarr[imax] / rsize) + (maxarr[imax] % rsize)) == mindist) && ((maxarr[imax] / rsize) > colmax ))
				{
					colmax = (maxarr[imax] / rsize);
					imax++;
				}
			}
			int imax = 0;
			while (imax < nmax)
			{
				if ((((maxarr[imax] / rsize) + (maxarr[imax] % rsize)) == mindist) && ((maxarr[imax] / rsize) == colmax ))
				{
					printf("[%d,%d]\n", (maxarr[imax] % rsize), (maxarr[imax] / rsize));
				}
				imax++;
			}

		}
		else
		{
			imax = 0;
			while (imax < nmax)
			{
				if (((maxarr[imax] / rsize) + (maxarr[imax] % rsize)) == mindist)
				{
					printf("[%d,%d]\n", (maxarr[imax] % rsize), (maxarr[imax] / rsize));
				}
				imax++;
			}
		}



	}
	else
	{
		int i = 0;
		while(i < (rsize * rsize))
		{
			if (rank[i] == max)
			{
				printf("[%d,%d]\n", (i % rsize), (i /rsize));
			}
			i++;
		}
	}
	
}