//omp davidson
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <sys/time.h>
#include <cmath>
#include <omp.h>

using namespace Eigen;
double cal_ev_k(Eigen::MatrixXd A, int k)
{
	SelfAdjointEigenSolver<MatrixXd> es(A);
	double lam = es.eigenvalues()[k];
	return lam;
}

int main(int argc, char* argv[])
{
	int ex = atoi(argv[2]);
	omp_set_num_threads(atoi(argv[1]));
	//--construct A--
	int N = 1 << ex;		
	float spf = 0.001;
	MatrixXd A = MatrixXd::Ones(N,N);
	A = spf * A;
#pragma omp parallel for
	for(int i=0; i< N; i++)
	{
		A(i,i) += (i+1)*1.0;
	}
	MatrixXd AT = A.transpose();
	A = (A + AT) * 0.5;
	//std::cout << A << std::endl;
	int k = 0;
	int l = k + 4;
	double lam_abs = cal_ev_k(A, k);

	struct timeval start, end;
	unsigned long cpu_time;
	gettimeofday(&start, NULL);

	MatrixXd I = MatrixXd::Identity(N,N);
	MatrixXd b = MatrixXd::Identity(N,l);
	MatrixXd Ab = A * b;
	MatrixXd At = b.adjoint() * Ab;
	SelfAdjointEigenSolver<MatrixXd> es;
	es.compute(At);
	VectorXd E = es.eigenvalues();
	double lam = E[k];
	MatrixXd V = es.eigenvectors();
	VectorXd alp = V.col(k);

	int step = 1;
	int M = l;

//--initialize for the while loop--
	VectorXd sum1;
	VectorXd sum2;
	double temp1;
	VectorXd q;
	VectorXd ksi;
	MatrixXd temp2;
	MatrixXd mult;
	VectorXd d;
	VectorXd bn;
	VectorXd Abn;
	MatrixXd Atn;
	double lam_old = lam;

	while (M < N)
	{
//--initialize tenporary variables--
		sum1 = VectorXd::Zero(N);
		sum2 = VectorXd::Zero(N);
		temp1 = 0.0;
		q = VectorXd::Zero(N);
		ksi = VectorXd::Zero(N);
		temp2 = MatrixXd::Zero(N,N);
		mult = MatrixXd::Identity(N,N);
		d = VectorXd::Zero(N);
		bn = VectorXd::Zero(N);
		Abn = VectorXd::Zero(N);
		Atn = MatrixXd::Zero(M+1,M+1);

		for (int i = 0; i < M; i++)
		{
			sum1 += alp[i] * Ab.col(i);
			sum2 += (alp[i] * lam) * b.col(i);	
		}
		q = sum1 - sum2;

//--Weinstein lower bound convergence check--
		//std::cout << lam << "\t" << fabs(lam - q.norm()) << "\t";
		//printf("error: %.15lf\n", fabs(lam - lam_abs));
		if (lam < fabs(lam - q.norm()))
		{
			printf("%d\t%d\t%d\t%d\t%.15lf\t%.15lf\t", Eigen::nbThreads(), N, k, step-1, lam_old, fabs(lam_abs - lam_old));
			break;
		}
		else if (fabs(lam_abs - lam) < 1e-14)
		{
			printf("%d\t%d\t%d\t%d\t%.15lf\t%.15lf\t", Eigen::nbThreads(), N, k, step-1, lam_old, fabs(lam_abs - lam_old));
			break;
		}
		else
		{
			lam_old = lam;
		}

		step ++;

	#pragma omp parallel for schedule(auto)
		for (int i = 0; i < N; i++)
		{
			ksi[i] = (1/(lam - A(i,i))) * q[i];
		}

		for (int i = 0; i < M; i++)
		{
			mult *= I - (b.col(i) * b.col(i).transpose());
		}

		d = mult * ksi;
		bn = (1/d.norm()) * d;
		Abn = A * bn;

	#pragma omp parallel for schedule(auto)
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < M; j++)
			{
				Atn(i,j) = At(i,j);
			}
		}

		b.conservativeResize(N, M+1);
		b.col(M) = bn;
		Ab.conservativeResize(N, M+1);
		Ab.col(M) = Abn;

	#pragma omp parallel for schedule(auto)
		for (int i = 0; i < M + 1; i++)
		{
			Atn(i,M) = b.col(i).adjoint() * Abn;
		}

	#pragma omp parallel for schedule(auto)
		for (int i = 0; i < M; i++)
		{
			Atn(M,i) = bn.adjoint() * Ab.col(i);
		}

		At = MatrixXd::Zero(M+1,M+1);
		At = Atn;
		E = VectorXd::Zero(M+1);
		V = MatrixXd::Zero(M+1, M+1);
		es.compute(At);
		E = es.eigenvalues();
		V = es.eigenvectors();
		lam = E[k];
		alp = V.col(k);

		M++;
	}

	gettimeofday(&end, NULL);
	cpu_time = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec;
	printf("%lf\n", cpu_time/1000.0);
	return(0);
}
