#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE *fp;
	char ch;

	fp = fopen("problem1.txt","r");
	int fsize;
	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char id[fsize];
	if (fp == NULL)
	{
		puts("No data file");
		exit(0);
	}
	else
	{
		int i, j;
		i = 0;
		while(1)
		{	
			ch = fgetc(fp);
			if (ch == EOF)
			{
				break;
			}
			else
			{
				id[i] = ch;
				i++;
			}
		}
		printf("Hello! I\'m student ");
		for (j = 0; j < 4; j++)
		{
			printf("%c", id[i - 4 + j]);
		}
		printf("\n");
	}
	fclose(fp);
	return(0);
}