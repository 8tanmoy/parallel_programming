#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <unistd.h>

int* read_matrix(FILE *fp, int len) {
	int *x = (int *) malloc(sizeof(int) * len * len);
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len; j++) {
			fscanf(fp, "%d ", &x[i * len + j]);
		}
	}
	return x;
}

int main(int argc, char* argv[]) {
	if (argc != 4) {
		fprintf(stderr, "Usage %s imageSize featureSize\n", argv[0]);
		return -1;
	}

	int nthreads = atoi(argv[1]);
	int imageSize = atoi(argv[2]);
	int featureSize = atoi(argv[3]);
	char* buff;
	buff = (char *) malloc(10 * sizeof(char));
	gethostname(buff, 10);	

	if (featureSize > imageSize) {
		fprintf(stderr, "Error! Feature image cannot be larger\n");
		return -1;
	}

	FILE *fp = fopen("problem3.dat", "r");
	int* image = read_matrix(fp, imageSize);
	int* feature = read_matrix(fp, featureSize);
	fclose(fp);
	
	//omp parameters
	omp_set_num_threads(nthreads);
	double start, runtime;

	// Reverse the rows
	int* reversedImage = (int *) malloc(sizeof(int) * imageSize * imageSize);
	start = omp_get_wtime();
#pragma omp parallel for schedule(auto)	
	for (int i = 0; i < imageSize; i++){
		memcpy(reversedImage + i * imageSize, image + (imageSize - i - 1) * imageSize, imageSize * sizeof(int));
	}

	// cross correlation
	int finalX, finalY, temp, max = -featureSize * featureSize; // set max to minimum

#pragma omp parallel for schedule(auto)
	for (int i = 0; i <= imageSize - featureSize; i++) {
		for (int j = 0; j <= imageSize - featureSize; j++) {
			temp = 0;
			for (int ii = 0; ii < featureSize; ii++) {
				for (int jj = 0; jj < featureSize; jj++) {
					temp += reversedImage[(i + ii) * imageSize + (j + jj)] * feature[ii * featureSize + jj];
				}
			}
			if (temp > max) {
				max = temp;
				finalX = i;
				finalY = j;
			}
		}
	}
	runtime = omp_get_wtime() - start;
	printf("%d\n%d\n%d\n", nthreads, imageSize, featureSize);
	printf("%f\n", runtime*1000);
	printf("%s\n", buff);
	printf("%d\n%d\n", finalX, finalY);
	printf("%d\n",max);
	free(image);
	free(feature);
	free(reversedImage);
	return 0;
}
