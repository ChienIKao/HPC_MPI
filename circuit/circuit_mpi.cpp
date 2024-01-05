#include <iostream>
#include <time.h>
#include <mpi.h>
#include <bitset>

using namespace std;

int circuit(int bits) {
    int a = (bits>>15)&1, b = (bits>>14)&1, c = (bits>>13)&1, d = (bits>>12)&1, 
        e = (bits>>11)&1, f = (bits>>10)&1, g = (bits>>9)&1, h = (bits>>8)&1, 
        i = (bits>>7)&1, j = (bits>>6)&1, k = (bits>>5)&1, l = (bits>>4)&1, 
        m = (bits>>3)&1, n = (bits>>2)&1, o = (bits>>1)&1, p = (bits>>0)&1;

    int res = 
    (a | b)&(~b | ~d)&(c | d)&(~d | ~e)&(e | ~f)&(f | g)&
    (f | ~g)&(h | ~i)&(i | j)&(i | ~j)&(~j | ~k)&(k | l)&
    (l | j)&(m | n)&(o | p)&(~p | g)&(n | ~o)&(~h | ~n);

    return res;
}

bool getBit(int bits, int idx) {
    return ((bits & (1<<idx)) > 0);
}

void print_bit(int bits, int n) {
    for (int i=n-1; i>=0; i--) {
        printf("%d", getBit(bits, i));
    }
}

int main () {
    int size, rank, cs = 1000;
    
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double start_time = MPI_Wtime();

    int bits_per_processor = 65536 / size;
    int start_bits = rank * bits_per_processor;
    int end_bits = (rank + 1) * bits_per_processor;

    int count = 0;
    for (int it=0; it<cs; it++) {
        for (int bits = start_bits; bits < end_bits; bits++) {
            if (circuit(bits) && it == 0) {
                // printf("Process %d found solution: ", rank);
                // print_bit(bits, 16);
                // printf("\n");
                count++;
            }
        }
    }

    int global_count = 0;
    MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    if (rank == 0) {
        printf("\033[32mTotal: %d\033[0m\n", global_count);
        printf("\033[36mTime: %.3lf sec\n\033[0m", end_time - start_time);
    }
    MPI_Finalize();
    return 0;
}