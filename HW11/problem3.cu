#include<thrust/device_vector.h>
#include<thrust/count.h>
#include<thrust/reduce.h>
#include<thrust/functional.h>
#include<iostream>
#include<thrust/execution_policy.h>

struct is_greater_than
{
	int threshold;
	is_greater_than(int t) { threshold = t; }
	__host__ __device__
	int operator()(int x) { return x > threshold; }
};

int main()
{
	const int N 		= 15;
	int day[N]			= {0,0,1,2,5,5,6,6,7,8,9,9,9,10,11};
	int site[N] 		= {2,3,0,1,1,2,0,1,2,1,3,4,0,1,2};
	int measurement[N] 	= {9,5,6,3,3,8,2,6,5,10,9,11,8,4,1};
	int C1[N]			= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int D1[N] 			= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int C2[N] 			= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int D2[N] 			= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	thrust::equal_to<int> binary_pred;
	//int maxday = thrust::reduce(thrust::host, day, day + N, -1, thrust::maximum<int>());
	int maxsite = thrust::reduce(thrust::host, site, site + N, -1, thrust::maximum<int>());
	//printf("maxday = %d, maxsite = %d\n", maxday, maxsite);
	thrust::sort_by_key(thrust::host, day, day + N, measurement);
	thrust::pair<int*, int*> maxfall;
	maxfall = thrust::reduce_by_key(thrust::host, day, day + N, measurement, C1, D1, binary_pred, thrust::maximum<int>());
	is_greater_than pred(5);
	int result = thrust::count_if(thrust::host, D1, D1 + N, pred);
	printf("%d\n", result);

	thrust::pair<int*,int*> totfall;
	thrust::sort_by_key(thrust::host, site, site + N, measurement);
	totfall = thrust::reduce_by_key(thrust::host, site, site + N, measurement, C2, D2, binary_pred, thrust::plus<int>());
	int j = 0;
	while (C2[j] <= maxsite)
	{
		printf("%d ",D2[j]);
		if (C2[j] == maxsite) break;
		j++;
	}
	return 0;
}
