#include <iostream>
#include <vector>
#include <time.h>
#include <mpi.h>

#define N 100000000

using namespace std;

int main() {
    vector<bool> marked(N+1, false); 
    marked[0] = true;
    marked[1] = true;
    marked[2] = false;

    for (int i=4; i<=N; i+=2) marked[i] = true;

    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double start_time = MPI_Wtime();

    int block_per_process = N / size;
    int from = rank * block_per_process;
    int to = (rank + 1) * block_per_process;

    int k = 2, local_min, global_min;
    while (k * k <= N) {

        // for (int i=from; i<to; i++) {
        //     if (i % k == 0 && i != k) marked[i] = true;
        // }  

        for (int i=from; i<to; i+=k) {
            while (i % k != 0) i++;

            if (i != k) marked[i] = true;
        }   

        for (int i=from; i<to; i++) {
            if (marked[i] == false && i > k) {
                local_min = i;
                break;
            } 
        }

        MPI_Allreduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
        k = global_min;
    }

    int local_count = 0, global_count = 0;
    for (int i=from; i<to; i++) {
        if (marked[i] == false) local_count++;
    }

    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    if (rank == 0) {
        printf("\033[32mTotal prime number: %d\033[0m\n", global_count);
        printf("\033[36mTime: %.3lf sec\n\033[0m", end_time - start_time);
    }

    MPI_Finalize();
}