#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <cuda_runtime.h>

__global__ void matXvec(int* mat, int* vec, int* prod)
{
	//mat[threadIdx.x + blockDim.x * threadIdx.y] = mat[threadIdx.x + blockDim.x * threadIdx.y] * vec[threadIdx.x];
	//int ty = threadIdx.y;
	//int tx = threadIdx.x;
	//atomicAdd(&prod[ty], mat[ty * 32 + tx]);
	atomicAdd(&prod[threadIdx.y], vec[threadIdx.x] * mat[threadIdx.y * blockDim.x + threadIdx.x]);
}

int* read_array(const char* filename, int len) {
	int *x = (int*) malloc(len * sizeof(int));
	FILE *fp = fopen(filename, "r");
	for (int i = 0; i < len; i++) {
		fscanf(fp, "%d", &x[i]);
	}
	fclose(fp);
	return x;
}

int main(int argc, char *argv[]) {
	if (argc != 1) {
		printf("Invalid argument Usage: ./problem1");
		return -1;
	}

	const int rowWidth=32;
	const int colWidth=16;	
	int *hA = read_array("inputA.inp",rowWidth*colWidth );
	int *hB = read_array("inputB.inp", rowWidth);
	int *hC = (int*) malloc(colWidth * sizeof(int));
	int *refC = (int*) malloc(colWidth * sizeof(int));
	// TODO - allocate host memory for refC (you have to figure out how much)
	// The skeleton currently segfaults because refC is accessed without allocation

	// TODO do a reference host implementation (Ch) here. ie populate answer in refC
	for(int i = 0; i < colWidth; i++)
	{
		refC[i] = 0;
		for(int j = 0; j < rowWidth; j++)
		{
			refC[i] = refC[i] + hA[j + rowWidth * i] * hB[j];
		}
		//printf("refC[%d]=%d\n",i, refC[i]);
	}

	int *dA, *dB, *dC;
	// TODO allocate device memory for dA,dB and dC
	cudaMalloc((void**)&dA, sizeof(int) * rowWidth * colWidth);
	cudaMalloc((void**)&dB, sizeof(int) * rowWidth);
	cudaMalloc((void**)&dC, sizeof(int) * colWidth);

	// TODO copy data from host to GPU 
	cudaMemcpy(dA, hA, sizeof(int) * rowWidth * colWidth, cudaMemcpyHostToDevice);
	cudaMemcpy(dB, hB, sizeof(int) * rowWidth, cudaMemcpyHostToDevice);

	// TODO call your kernel
	dim3 threadsPerBlock(rowWidth, colWidth);
	matXvec<<<1, threadsPerBlock>>>(dA, dB, dC);

	// TODO copyback results
	cudaMemcpy(hC, dC, sizeof(int) * colWidth, cudaMemcpyDeviceToHost);

	cudaFree(dA);
	cudaFree(dB);
	cudaFree(dC);

	float Error=0;

	for(int i=0;i<colWidth;i++)
		Error+=(hC[i]-refC[i])*(hC[i]-refC[i]);
	printf("%f\n%d",sqrt(Error),hC[colWidth-1]);

	free(refC);
	free(hB);
	free(hA);

	return 0;
}
