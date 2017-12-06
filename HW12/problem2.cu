#include <iostream>
#include <cuda.h>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/copy.h>
#include <thrust/scan.h>

void prefix_scan(float *in, float *out, int N)
{
	thrust::host_vector<float> H(N);
	for (int i = 0; i < N; i++)
	{
		H[i] = in[i];
	}
	thrust::device_vector<float> D = H;
	thrust::inclusive_scan(D.begin(), D.end(), D.begin());
	thrust::copy(D.begin(), D.end(), out);
	return;
}