//Modified from:
//https://developer.nvidia.com/sites/default/files/akamai/cuda/files/Misc/mygpu.pdf
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<cuda_runtime.h>
#include<cusolverDn.h>
#define MILLION 1000000L;
#include<sys/time.h>

void printMatrix(int m, int n, const float*A, int lda, const char* name)
{
    for(int row = 0 ; row < m ; row++){
        for(int col = 0 ; col < n ; col++){
            float Areg = A[row + col*lda];
            printf("%s(%d,%d) = %f\n", name, row+1, col+1, Areg);
        }
    }
}

int main (int argc , char* argv[])
{
    cusolverDnHandle_t cusolverH;
    cusolverStatus_t cusolver_status = CUSOLVER_STATUS_SUCCESS;
    
    cudaError_t cudaStat = cudaSuccess;
    const int m = 1 << atoi(argv[1]); // number of rows and columns of A
    const int lda = m; // leading dimension of A
    float *A; // mxm matrix
    float *V; // mxm matrix of eigenvectors
    float *W; // m- vector of eigenvalues
    
    // prepare memory on the host
    A = ( float *) malloc (lda*m* sizeof ( float ));
    V = ( float *) malloc (lda*m* sizeof ( float ));
    W = ( float *) malloc (m* sizeof ( float ));
    
    // define random A
    //for(int i=0;i<lda*m;i++) A[i] = rand ()/( float ) RAND_MAX ;
    srand(time(NULL));
   
    float spf = 0.001; 
    for (int i = 0; i < m; i++)
    {
        for(int j = 0; j < m; j++)
        {
            //A[j + i*m] = spf * (((float) rand() / (RAND_MAX)) -1);
            A[j + i*m] = spf * 1.0;
        }
    }
    for (int i = 0; i < m; i++)
    {
        A[i + i*m] += (i+1)* 1.0;
    }
    for (int i = 0; i < m; i++)
    {
        for(int j = 0; j < m; j++)
        {
            A[j + i*m] = 0.5*(A[j + i*m] + A[i + j*m]);
        }
    }
	int k = 0;
    //printMatrix(m,m,A,lda,"A");	
	        
    // declare arrays on the device
    float *d_A; // mxm matrix A on the device
    float *d_W; // m- vector of eigenvalues on the device
    int * devInfo ; // info on the device
    float * d_work ; // workspace on the device
    int lwork = 0; // workspace size
    int info_gpu = 0; // info copied from device to host
    
    struct timespec start, stop;
    double accum;

    // create cusolver handle
    cusolver_status = cusolverDnCreate(&cusolverH);
    // prepare memory on the device
    cudaStat = cudaMalloc (( void **)& d_A , sizeof ( float )* lda*m);
    cudaStat = cudaMalloc (( void **)& d_W , sizeof ( float )*m);
    cudaStat = cudaMalloc (( void **)& devInfo , sizeof (int ));
    cudaStat = cudaMemcpy (d_A ,A, sizeof ( float )* lda*m, cudaMemcpyHostToDevice); // copy A->d_A
    // compute eigenvalues and eigenvectors
    cusolverEigMode_t jobz = CUSOLVER_EIG_MODE_VECTOR;
    // use lower left triangle of the matrix
    cublasFillMode_t uplo = CUBLAS_FILL_MODE_LOWER;
    // compute buffer size and prepare workspace
    cusolver_status = cusolverDnSsyevd_bufferSize ( cusolverH, jobz , uplo , m, d_A , lda , d_W , &lwork);
    cudaStat = cudaMalloc (( void **)&d_work , sizeof ( float )* lwork);

    clock_gettime(CLOCK_REALTIME, &start);

    // compute the eigenvalues and eigenvectors for a symmetric ,
    // real mxm matrix ( only the lower left triangle af A is used )
    cusolver_status = cusolverDnSsyevd(cusolverH, jobz, uplo, m, d_A, lda, d_W, d_work, lwork, devInfo);
    cudaStat = cudaDeviceSynchronize();

    clock_gettime(CLOCK_REALTIME, &stop);
    accum = (stop.tv_sec - start.tv_sec) * 1000 + (stop.tv_nsec - start.tv_nsec)/(double)MILLION;
    //printf("Ssyevd time: %lf sec\n", accum);

    cudaStat = cudaMemcpy (W, d_W , sizeof ( float )*m, cudaMemcpyDeviceToHost ); // copy d_W ->W
    cudaStat = cudaMemcpy (V, d_A , sizeof ( float )* lda*m, cudaMemcpyDeviceToHost ); // copy d_A ->V
    cudaStat = cudaMemcpy (&info_gpu , devInfo , sizeof (int), cudaMemcpyDeviceToHost ); // copy devInfo -> info_gpu
    //printf (" after syevd : info_gpu = %d\n", info_gpu );
    //printf (" eigenvalues :\n"); // print first eigenvalues

    printf ("%d\t%d\t%.15lf\t%lf\n", m, k, W[k], accum);

    // free memory
    cudaFree (d_A );
    cudaFree (d_W );
    cudaFree ( devInfo );
    cudaFree ( d_work );
    cusolverDnDestroy ( cusolverH );
    cudaDeviceReset ();

    return 0;
}


 /*
 //TP: start time
     cudaEvent_t startEvent_inc, stopEvent_inc;
     float elapsedTime_inc;
     cudaEventCreate(&startEvent_inc);
     cudaEventCreate(&stopEvent_inc);
     cudaEventRecord(startEvent_inc,0);

//TP: end time
    cudaEventRecord(stopEvent_inc,0);
    cudaEventSynchronize(stopEvent_inc);
    cudaEventElapsedTime(&elapsedTime_inc, startEvent_inc, stopEvent_inc);
    printf("elapsed time: %f ms",elapsedTime_inc);
*/
