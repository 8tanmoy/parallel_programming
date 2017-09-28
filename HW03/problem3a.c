#include <stdio.h>
#include <stdlib.h>

int main()
{
	const int r = 2, c = 3;
	int x[r][c];
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
		{
			printf("&x[%d][%d] = %p\n", i , j,&x[i][j]);
		}
	}

	return(0);
}