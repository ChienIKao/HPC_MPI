#include <bits/stdc++.h>
#include <mpi.h>
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
	int size, rank;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double start = MPI_Wtime();

	int paddingRow;
	if (SIZE % size != 0)
		paddingRow = SIZE + size - SIZE % size;
	else
		paddingRow = SIZE;

	double **A, **B, **C;
	A = (double **)malloc(paddingRow * sizeof(double *));
	B = (double **)malloc(paddingRow * sizeof(double *));
	C = (double **)malloc(paddingRow * sizeof(double *));
	for (int i = 0; i < paddingRow; i++) {
		A[i] = (double *)malloc(SIZE * sizeof(double));
		B[i] = (double *)malloc(SIZE * sizeof(double));
		C[i] = (double *)malloc(SIZE * sizeof(double));
	}

	if (rank == 0) {
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				A[i][j] = 1.1;
				B[i][j] = 1.1;
			}
		}
	}

	// root 0 broadcast matrix B
	for (int i = 0; i < SIZE; i++)
		MPI_Bcast(B[i], SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	// root 0 scatter matrix A
	double *row;
	row = (double *)malloc(size * SIZE * sizeof(double));

	for (int i = 0; i < paddingRow; i += size) {
		if (rank == 0) {
			for (int r = 0; r < size; r++) {
				for (int j = 0; j < SIZE; j++) {
					row[r * SIZE + j] = A[i + r][j];
				}
			}
		}

		MPI_Scatter(row, SIZE, MPI_DOUBLE, A[i + rank], SIZE, MPI_DOUBLE, 0,
		            MPI_COMM_WORLD);
	}

	// calculate
	for (int i = 0; i < SIZE; i += size) {
		for (int j = 0; j < SIZE; j++) {
			double f = 0.0;
			for (int k = 0; k < SIZE; k++) {
				f += A[i + rank][k] * B[k][j];
			}
			C[i + rank][j] = f;
		}
	}

	for (int i = 0; i < paddingRow; i += size) {
		MPI_Gather(C[i + rank], SIZE, MPI_DOUBLE, row, SIZE, MPI_DOUBLE, 0,
		           MPI_COMM_WORLD);

		if (rank == 0) {
			for (int r = 0; r < size; r++) {
				for (int j = 0; j < SIZE; j++) {
					C[i + r][j] = row[r * SIZE + j];
				}
			}
		}
	}

	double end = MPI_Wtime();
	if (rank == 0) {
		printGraph(C);
		printf("\033[36mTime: %.3lf sec\033[0m\n", end - start);
	}

	free(A);
	free(B);
	free(C);
	free(row);
	MPI_Finalize();
	return 0;
}