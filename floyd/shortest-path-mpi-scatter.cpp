#include <bits/stdc++.h>
#include <mpi.h>

#define ROW 1000
#define COL 1000

using namespace std;

void printGraph(int **graph) {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (graph[i][j] == 200)
				printf("inf ");
			else
				printf("%3d ", graph[i][j]);
		}
		printf("\n");
	}
}

int main() {
	int rank, size;
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	double start_time = MPI_Wtime();
	srand(26); // seed

	int paddingRow;
	if (ROW % size != 0)
		paddingRow = ROW + size - ROW % size;
	else
		paddingRow = ROW;

	int **graph;
	graph = (int **)malloc(paddingRow * sizeof(int *));
	for (int i = 0; i < paddingRow; i++)
		graph[i] = (int *)malloc(COL * sizeof(int));

	// rank 0 init a 2d array
	if (rank == 0) {
		for (int i = 0; i < ROW; i++) {
			for (int j = 0; j < COL; j++) {
				if (i == j)
					graph[i][j] = 0;
				else {
					graph[i][j] = rand() % 100 + 1;
					if (graph[i][j] <= 10) graph[i][j] = 200;
				}
			}
		}

		// printf("\n===== Initial ======\n\n");
		// printGraph(graph);
	}

	int *data;
	data = (int *)malloc(COL * size * sizeof(int));

	for (int i = 0; i < paddingRow; i += size) {
		if (rank == 0) {
			for (int r = 0; r < size; r++) {
				for (int j = 0; j < COL; j++) {
					data[r * COL + j] = graph[i + r][j];
				}
			}
		}

		MPI_Scatter(data, COL, MPI_INT, graph[i + rank], COL, MPI_INT, 0,
		            MPI_COMM_WORLD);
	}

	for (int k = 0; k < ROW; k++) {
		MPI_Bcast(graph[k], COL, MPI_INT, k % size, MPI_COMM_WORLD);
		for (int i = 0; i < ROW; i += size) {
			for (int j = 0; j < COL; j++) {
				if (i + rank < ROW)
					graph[i + rank][j] = min(graph[i + rank][j],
					                         graph[i + rank][k] + graph[k][j]);
			}
		}
	}

	for (int i = 0; i < paddingRow; i += size) {
		MPI_Gather(graph[i + rank], COL, MPI_INT, data, COL, MPI_INT, 0,
		           MPI_COMM_WORLD);
		if (rank == 0) {
			for (int r = 0; r < size; r++) {
				for (int j = 0; j < COL; j++) {
					graph[i + r][j] = data[r * COL + j];
				}
			}
		}
	}

	double end_time = MPI_Wtime();

	if (rank == 0) {
		// printf("\n====== Result ======\n\n");
		// printGraph(graph);
		printf("\033[36mTime: %.3lf sec\033[0m\n", end_time - start_time);
	}

	free(graph);
	free(data);
	MPI_Finalize();
	return 0;
}