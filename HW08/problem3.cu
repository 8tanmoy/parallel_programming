#ifdef _WIN32
#  define NOMINMAX 
#endif

// includes, system
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

__global__ void MatMult(double *dA, double *dB, double *dC, int nRows, int nInnerDimension, int nCols, int TileSize)
{
	int bx = blockIdx.x;
	int by = blockIdx.y;
	int tx = threadIdx.x; 
	int ty = threadIdx.y;
	int aBegin = nRows * TileSize * by;
	int aEnd   = aBegin + nRows - 1;
	int aStep  = TileSize;
	int bBegin = bx * TileSize;
	int bStep  = TileSize * nInnerDimension;
	double Csub = 0.0;

	volatile __shared__ double As[32][32];
	volatile __shared__ double Bs[32][32];
	for (int a = aBegin, b = bBegin; a <= aEnd; a += aStep, b +=bStep)
	{
		As[ty][tx] = dA[a + nRows * ty + tx];
		Bs[ty][tx] = dB[b + nInnerDimension * ty + tx];

		__syncthreads();

		for (int k = 0; k < TileSize; ++k)
		{
			Csub += As[ty][k] * Bs[k][tx];
		}

		__syncthreads();
		//__threadfence_block();
	}
	int c = nInnerDimension * TileSize * by + TileSize * bx;
	dC[c + nInnerDimension * ty + tx] = Csub;
}

double* read_array(const char* filename, int len) {
	double *x = (double*) malloc(len * sizeof(double));
	FILE *fp = fopen(filename, "r");
	for (int i = 0; i < len; i++) {
		fscanf(fp, "%lf", &x[i]);
	}
	fclose(fp);
	return x;
}

void computeOnDevice(double* hA,double* hB, double* hC, int nRows,
	int nInnerDimension,int nCols, int tileSize, float* incTime );

////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////

int main( int argc, char** argv) 
{
	if(argc!=5)
        {
                printf("Usage: ./problem2 i j k N\n");
                return 0;
        }


	int nRows = atoi(argv[1]);
	int nInnerDimension = atoi(argv[2]);
	int nCols = atoi(argv[3]);
	int num_elementsA= nRows*nInnerDimension;
	int num_elementsB=nInnerDimension*nCols;
	int num_elementsC= nRows*nCols;
	int tileSize = atoi(argv[4]);  //change this for scaling analysis
	float incTime=0; // Time for GPU
	double* hA = read_array("problem3.inp",num_elementsA);
	double* hB = read_array("problem3.inp",num_elementsB);
	double* hC = (double*) malloc(num_elementsC * sizeof(double));

	// **===-------- Modify the body of this function -----------===**
	computeOnDevice( hA, hB,hC, nRows, nInnerDimension, nCols, tileSize, &incTime);
	// **===-----------------------------------------------------------===**


	printf("%f\n%f\n%d\n%d\n%d\n",hC[num_elementsC-1],incTime,tileSize,nRows,nCols);
	// cleanup memory
	free(hA);
	free(hB);
	free(hC);

	return 0;
}


void computeOnDevice(double* hA,double* hB, double* hC, int nRows, int nInnerDimension, int nCols, int TileSize, float* incTime)
{
	cudaEvent_t startEvent_inc, stopEvent_inc;
	float elapsedTime_inc;
	cudaEventCreate(&startEvent_inc);
	cudaEventCreate(&stopEvent_inc);
	cudaEventRecord(startEvent_inc,0);

	double* Ad;
	cudaMalloc((void**)&Ad, nRows * nInnerDimension * sizeof(double));
	cudaMemcpy(Ad, hA, nRows * nInnerDimension * sizeof(double), cudaMemcpyHostToDevice);

	double* Bd;
	cudaMalloc((void **)&Bd, nInnerDimension * nCols * sizeof(double));
	cudaMemcpy(Bd, hB, nInnerDimension * nCols * sizeof(double), cudaMemcpyHostToDevice);

	double* Cd;
	cudaMalloc((void **)&Cd, nRows * nCols * sizeof(double));

	dim3 dimBlock(TileSize, TileSize);

	int tempx = nRows;
	if (nInnerDimension > nRows) tempx = nInnerDimension;
	tempx = (tempx + TileSize - 1)/TileSize;
	int tempy = nCols;
	if (nInnerDimension > nCols) tempy = nInnerDimension;
	tempy = (tempy + TileSize - 1)/TileSize;

	dim3 dimGrid(tempx, tempy);

	MatMult<<<dimGrid, dimBlock, sizeof(double) * TileSize * TileSize>>>(Ad, Bd, Cd, nRows, nInnerDimension, nCols, TileSize); //, sizeof(double) * TileSize * TileSize

	cudaMemcpy(hC, Cd, nRows * nCols * sizeof(double), cudaMemcpyDeviceToHost);
	cudaFree(Ad);
	cudaFree(Bd);
	cudaFree(Cd);

	cudaEventRecord(stopEvent_inc,0);
	cudaEventSynchronize(stopEvent_inc);
	cudaEventElapsedTime(&elapsedTime_inc, startEvent_inc, stopEvent_inc);
	*incTime = elapsedTime_inc;

	return;
}

