#include <stdio.h>
#include <stdlib.h>

int main()
{
	float A[96][209];
	float B[96][209];
	const int N = 96*209;
	float *C;
	C = (float *) malloc(N * sizeof(float));
	float* ap = &A[0][0];
	float* bp = &B[0][0];
	for (int i = 0; i < N; i++)
	{
		C[i] = *(ap + i) + *(bp + i);
	}
	free(C);
	return(0);
}