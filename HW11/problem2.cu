#include <iostream>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/copy.h>
#include <thrust/scan.h>
#include <thrust/execution_policy.h>
#include <thrust/count.h>

void computeSum( float* reference, float* idata, const unsigned int len) 
{
  reference[0] = 0;
  double total_sum = 0;
  unsigned int i;
  for( i = 0; i < len; ++i) 
  {
      total_sum += idata[i];
  }
  *reference = total_sum;
}

void initializeArray(FILE* fp,float* arr, int nElements)
{
	for( int i=0; i<nElements; i++){
		int r=fscanf(fp,"%f",&arr[i]);
		if(r == EOF){
			rewind(fp);
		}
		arr[i]-=5; // This is to make the data zero mean. Otherwise we reach large numbers and lose precision
	}
}

int main(int argc, char* argv[]) {
	if(argc!=2){
		printf("Usage %s N\n",argv[0]);
		return 1;
	}
	int N 			= atoi(argv[1]);
	FILE *fp 		= fopen("problem1.inp","r");
	int size 		= N * sizeof(float);
	float *in 		= (float *)malloc(size);
	float time 		= 0.f;

	cudaEvent_t startEvent_inc, stopEvent_inc;
	cudaEventCreate(&startEvent_inc);
	cudaEventCreate(&stopEvent_inc);

	initializeArray(fp,in, N);

	float reference = 1.0f;  
	computeSum(&reference , in, N);

	thrust::host_vector<float> H(N);
	for (int i = 0; i < N; i++)
	{
		H[i] = in[i];
	}
	cudaEventRecord(startEvent_inc,0); // starting timing for inclusive  

	thrust::device_vector<float> D = H;
	float result = thrust::reduce(D.begin(), D.end()); //, (int) 0, thrust::plus<int>());
	
	cudaEventRecord(stopEvent_inc,0);  //ending timing for inclusive
	cudaEventSynchronize(stopEvent_inc);
	cudaEventElapsedTime(&time, startEvent_inc, stopEvent_inc);

	// Run accuracy test
	float epsilon = 0.3f;
	unsigned int result_regtest = (abs(result - reference) <= epsilon);

	if(!result_regtest)	printf("Test failed device: %f  host: %f\n",result,reference);

	printf("%d\n%f\n%f\n",N,result,time);
	return(0);
}