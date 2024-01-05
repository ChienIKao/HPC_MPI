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

	int **graph;
	graph = (int **)malloc(ROW * sizeof(int *));
	for (int i = 0; i < ROW; i++) graph[i] = (int *)malloc(COL * sizeof(int));

	// rank 0 init a 2d array
	if (rank == 0) {
		for (int i = 0; i < ROW; i++) {
			for (int j = 0; j < COL; j++) {
				if (i == j)
					graph[i][j] = 0;
				else {
					graph[i][j] = rand() % 100 + 1; // 1 ~ 100
					if (graph[i][j] <= 10) graph[i][j] = 200;
				}
			}
		}

		// printf("\n===== Initial ======\n\n");
		// printGraph(graph);
	}

	// rank 0 send data to other rank
	if (rank == 0) {
		MPI_Request send_request;
		for (int i = 0; i < ROW; i++) {
			int dest = i % size, tag = i;
			if (dest != 0) {
				MPI_Isend(graph[i], COL, MPI_INT, dest, tag, MPI_COMM_WORLD,
				          &send_request);
				MPI_Wait(&send_request, MPI_STATUS_IGNORE);
			}
		}
	} else {
		MPI_Request recv_request;
		for (int i = 0; i < ROW; i += size) {
			int tag = i + rank;
			if (tag < ROW) {
				MPI_Irecv(graph[tag], COL, MPI_INT, 0, tag, MPI_COMM_WORLD,
				          &recv_request);
				MPI_Wait(&recv_request, MPI_STATUS_IGNORE);
			}
		}
	}

	for (int k = 0; k < ROW; k++) {
		int root = k % size;
		MPI_Bcast(graph[k], COL, MPI_INT, root, MPI_COMM_WORLD);
		for (int i = 0; i < ROW; i += size) {
			for (int j = 0; j < COL; j++) {
				if (i + rank < ROW)
					graph[i + rank][j] = min(graph[i + rank][j],
					                         graph[i + rank][k] + graph[k][j]);
			}
		}
	}

	// printf("rank %d:\n", rank);
	// printGraph(graph);
	// printf("\n");

	// rank 0 receive data from other rank
	if (rank == 0) {
		MPI_Request recv_request;
		for (int i = 0; i < ROW; i++) {
			int source = i % size, tag = i;
			if (source != 0) {
				MPI_Irecv(graph[i], COL, MPI_INT, source, tag, MPI_COMM_WORLD,
				          &recv_request);
				MPI_Wait(&recv_request, MPI_STATUS_IGNORE);
			}
		}
	} else {
		MPI_Request send_request;
		for (int i = 0; i < ROW; i += size) {
			int tag = i + rank;
			if (tag < ROW) {
				MPI_Isend(graph[tag], COL, MPI_INT, 0, tag, MPI_COMM_WORLD,
				          &send_request);
				MPI_Wait(&send_request, MPI_STATUS_IGNORE);
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
	MPI_Finalize();
	return 0;
}