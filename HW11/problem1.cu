#include <iostream>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/copy.h>
#include <thrust/scan.h>
#include <thrust/execution_policy.h>

int checkResults(float*res, float* cudaRes,int length)
{
	int nDiffs=0;
	const float smallVal = .3f; // Keeping this extra high as we have repetitive addition and sequence matters
	for(int i=0; i<length; i++)
		if(fabs(cudaRes[i]-res[i])>smallVal)
			{nDiffs++;
       //printf("%f %f\n",cudaRes[i],res[i]);
      }
	return nDiffs;
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

void inclusiveScan_SEQ(float *in, float *out,int length) {
	float sum=0.f;
	for (int i =0; i < length; i++) {
		sum+=in[i];
		out[i]=sum;
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
	float *out 		= (float *)malloc(size); 
	float *cuda_out = (float *)malloc(size);
	float time 		= 0.f;

	cudaEvent_t startEvent_inc, stopEvent_inc;
	cudaEventCreate(&startEvent_inc);
	cudaEventCreate(&stopEvent_inc);

	initializeArray(fp,in, N);

	thrust::host_vector<float> H(N);
	for (int i = 0; i < N; i++)
	{
		H[i] = in[i];
	}
	cudaEventRecord(startEvent_inc,0); // starting timing for inclusive  

	thrust::device_vector<float> D = H;
	thrust::inclusive_scan(D.begin(), D.end(), D.begin());
	thrust::copy(D.begin(), D.end(), cuda_out);

	cudaEventRecord(stopEvent_inc,0);  //ending timing for inclusive
	cudaEventSynchronize(stopEvent_inc);
	cudaEventElapsedTime(&time, startEvent_inc, stopEvent_inc);

	inclusiveScan_SEQ(in, out, N);
	int nDiffs = checkResults(out, cuda_out,N);

	if(nDiffs)printf("Test Failed\n"); // This should never print
	printf("%d\n%f\n%f\n",N,cuda_out[N-1],time);
	//dynamically delete all
	return(0);
}