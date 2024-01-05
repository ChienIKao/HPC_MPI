#include <bits/stdc++.h>

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
	srand(26); // random seed

	int **graph;
	graph = (int **)malloc(ROW * sizeof(int *));
	for (int i = 0; i < ROW; i++) {
		graph[i] = (int *)malloc(COL * sizeof(int));
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

	// shortest path calculate
	for (int k = 0; k < ROW; k++) {
		for (int i = 0; i < ROW; i++) {
			for (int j = 0; j < COL; j++) {
				graph[i][j] = min(graph[i][j], graph[i][k] + graph[k][j]);
			}
		}
	}

	// printf("\n====== Result ======\n\n");
	// printGraph(graph);

	printf("\033[36mTime: %.3lf sec\033[0m\n",
	       ((double)clock() / CLOCKS_PER_SEC));

	free(graph);
	return 0;
}