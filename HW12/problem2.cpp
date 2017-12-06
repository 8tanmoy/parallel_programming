#include <vector>
#include <fstream>
#include <numeric>
#include <iterator>
#include <algorithm>
#include "problem2.hu"
#include <iostream>
#define OMPI_SKIP_MPICXX  /* Don't use OpenMPI's C++ bindings (they are deprecated) */
#include <mpi.h>

template <typename T>
auto read_file(char const* name, int size) {
	std::ifstream fin{name};
	std::vector<T> x;
	x.reserve(static_cast<size_t>(size));
	std::copy_n(std::istream_iterator<T>(fin), size, std::back_inserter(x));
	return x;
}

namespace mpi {
	class context {
		int m_rank, m_size;
	public:
		context(int *argc, char **argv[]) : m_rank { -1 } {
			if (MPI_Init(argc, argv) == MPI_SUCCESS) {
				MPI_Comm_rank(MPI_COMM_WORLD, &m_rank);
				MPI_Comm_size(MPI_COMM_WORLD, &m_size);
			}
		}
		~context() {
			if(m_rank >= 0) {
				MPI_Finalize();
			}
		}
		explicit operator bool() const {
			return m_rank >= 0;
		}
		int rank() const noexcept { return m_rank; }
		int size() const noexcept { return m_size; }
	};
}

int main(int argc, char* argv[]) {
	mpi::context ctx(&argc, &argv);
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " N\n";
		return -1;
	}

	double starttime, endtime;
	const auto N = std::atoi(argv[1]);
/*
	float ref = 0.0;
	auto x = read_file<float>("problem2.inp", N);
	for (int l = 0; l < N; l++)
	{
		ref += x[l];
	}
	std::cout << "reference sum = " << ref << "\n";
*/

	if(ctx.rank() == 0) {
		auto A0 = read_file<float>("problem2.inp", N);
		std::vector<float> sum0(static_cast<size_t>(N));
		starttime = MPI_Wtime();
		prefix_scan(A0.data(), sum0.data(), N);
//		std::cout << "sum0[N-1] = " << sum0[N-1] << "\n";

		MPI_Status status;
		constexpr int dest_rank = 1;  // We send a message to Task 1
		MPI_Ssend(&sum0[N - 1], 1, MPI_FLOAT, dest_rank, 0, MPI_COMM_WORLD);

		float last;
		constexpr int source_rank = 1;
		MPI_Recv(&last, 1, MPI_FLOAT, source_rank, 1, MPI_COMM_WORLD, &status);
		endtime = MPI_Wtime();
		std::cout << N << "\n";
		std::cout << endtime - starttime << "\n";
		std::cout << last << "\n";

	} else {
		float sum_rcvd;
		MPI_Status status;
		auto A1 = read_file<float>("problem2.inp", N);
		std::vector<float> sum1(static_cast<size_t>(N));
		prefix_scan(A1.data(), sum1.data(), N);
//		std::cout << "sum1[N-1] = " << sum1[N-1] << "\n";

		constexpr int source_rank = 0;  // We expect a message from Task 0
		MPI_Recv(&sum_rcvd, 1, MPI_FLOAT, source_rank, 0, MPI_COMM_WORLD, &status);

		transform(sum1.begin(), sum1.end(), sum1.begin(), bind2nd(std::plus<float>(), sum_rcvd));

		constexpr int dest_rank = 0;  // We send a message to Task 0
		MPI_Ssend(&sum1[N - 1], 1, MPI_FLOAT, dest_rank, 1, MPI_COMM_WORLD);

	}

//	auto x = read_file<float>("problem2.inp", N);
//	std::vector<float> sum(static_cast<size_t>(N));
//	prefix_scan(x.data(), sum.data(), N);
}
