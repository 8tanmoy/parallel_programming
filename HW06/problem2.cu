#include <iostream>
#include <cuda.h>

__global__ void addTidBid(int* data)
{
	data[threadIdx.x + blockIdx.x * blockDim.x] = (threadIdx.x + blockIdx.x);
	//printf("i am tid %d blockDim %d blockIdx %d\n", threadIdx.x, blockDim.x, blockIdx.x);
}

int main()
{
	const int numElems = 16;
	int *hArray = (int *) malloc(numElems * sizeof(int));
	int* dArray;													//initialize pointer on host
	cudaMalloc((void **)&dArray, sizeof(int) * numElems);	//allocate memory on device
	cudaMemset(dArray, 0, numElems * sizeof(int));					//initilize them to zero
	addTidBid<<<2,8>>>(dArray);										//kernel launched 2blocks 8threads
	cudaMemcpy(hArray, dArray, sizeof(int) * numElems, cudaMemcpyDeviceToHost);
	cudaFree(dArray);
	for (int i = 0; i < numElems; i++)
		std::cout << hArray[i] << std::endl;

	free(hArray);
	return(0);
}
