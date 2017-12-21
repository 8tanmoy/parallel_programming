//eigen serial
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <sys/time.h>

using namespace Eigen;

int main(int argc, char* argv[])
{
	int N = 1 << atoi(argv[1]);
	float spf = 0.001;

	MatrixXd A = MatrixXd::Random(N,N);
	A = spf * A;

	for(int i=0; i< N; i++)
	{
		A(i,i) += (i+1)*1.0;
	}

	MatrixXd AT = A.transpose();
	A = (A + AT) * 0.5;
	
	//std::cout << A << std::endl;
	int k = 0;

	//---regular diagonalization in eigen c---
	struct timeval start, end;
	double cpu_time;
	gettimeofday(&start, NULL);

	SelfAdjointEigenSolver<MatrixXd> es(A);
	VectorXd E = es.eigenvalues();
	MatrixXd V = es.eigenvectors();
	double lam = E[k];

	gettimeofday(&end, NULL);

	cpu_time = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec;

	std::cout << N << "\t" << k << "\t" << lam << "\t" << cpu_time/1000.0 << "\n";
	return(0);
}

/*

SparseMatrix<double> sm1(10,10);
std::cout << sm1 << "\n";
double r = ((double) rand() / (RAND_MAX));
std::cout << r << "\n";

*/


//openACC invoke GPU, cuFFT, cuBLAS, cuSparse, Thrust, NPP...
