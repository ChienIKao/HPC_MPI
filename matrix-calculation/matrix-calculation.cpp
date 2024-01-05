#include <bits/stdc++.h>
#define SIZE 1000

using namespace std;

void printGraph(double **graph) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			printf("%.1lf ", graph[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

int main() {
	double **A, **B, **C;
	A = (double **)malloc(SIZE * sizeof(double *));
	B = (double **)malloc(SIZE * sizeof(double *));
	C = (double **)malloc(SIZE * sizeof(double *));
	for (int i = 0; i < SIZE; i++) {
		A[i] = (double *)malloc(SIZE * sizeof(double));
		B[i] = (double *)malloc(SIZE * sizeof(double));
		C[i] = (double *)malloc(SIZE * sizeof(double));
	}

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			A[i][j] = 1.1;
			B[i][j] = 1.1;
		}
	}

	// calculate
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			double f = 0.0;
			for (int k = 0; k < SIZE; k++) {
				f += A[i][k] * B[k][j];
			}
			C[i][j] = f;
		}
	}

	printGraph(C);
	printf("\033[36mTime: %.3lf sec\033[0m\n",
	       ((double)clock() / CLOCKS_PER_SEC));

	free(A);
	free(B);
	free(C);
	return 0;
}