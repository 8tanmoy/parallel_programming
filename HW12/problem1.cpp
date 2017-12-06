#include <iostream>
#include <omp.h>
#define OMPI_SKIP_MPICXX  /* Don't use OpenMPI's C++ bindings (they are deprecated) */
#include <mpi.h>

float* read_array(const char* filename, int len) {
	float *x = (float*) malloc(len * sizeof(float));
	FILE *fp = fopen(filename, "r");
        for( int i=0; i<len; i++){
			int r=fscanf(fp,"%f",&x[i]);		
			if(r == EOF){
				rewind(fp);
			}
	}
	fclose(fp);
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

int main(int argc, char *argv[]) {
	mpi::context ctx(&argc, &argv);

	if(!ctx) {
		std::cerr << "MPI Initialization failed\n";
		return -1;
	}

	//make a global array
	int N;
	float tot;
	double starttime, endtime;
	N = atoi(argv[1]);

/*	float ref = 0.0;
	float *refarr = read_array("problem1.inp",N);
	for (int l = 0; l < N; l++)
	{
		ref += refarr[l];
	}
	std::cout << "reference sum = " << 2*ref << "\n";
*/
	
	if(ctx.rank() == 0) {
		float *A0 = read_array("problem1.inp",N);
		float tot0;
		float tot_rcvd;
		starttime = MPI_Wtime();
#pragma omp parallel for reduction(+:tot0)
	for(int j = 0; j < N; j++)
	{
		tot0 += A0[j];
	}
		constexpr int source_rank = 1;  // We expect a message from Task 1
		MPI_Status status;
		MPI_Recv(&tot_rcvd, 1, MPI_FLOAT, source_rank, 0, MPI_COMM_WORLD, &status);

//		std::cout << "Calculated tot0 = " << tot0 << " on root task.\n";
//		std::cout << "Received tot1 = " << tot_rcvd << " on root task.\n";

		tot = tot0 + tot_rcvd;
		endtime = MPI_Wtime();
		std::cout << N << "\n";
		std::cout << endtime - starttime << "\n";
		std::cout << tot << "\n";
	} else {
		float *A1 = read_array("problem1.inp",N);
		float tot1;
#pragma omp parallel for reduction(+:tot1)
	for(int k = 0; k < N; k++)
	{
		tot1 += A1[k];
	}
		constexpr int dest_rank = 0;  // We send a message to Task 0
		MPI_Send(&tot1, 1, MPI_FLOAT, dest_rank, 0, MPI_COMM_WORLD);
	}
}
