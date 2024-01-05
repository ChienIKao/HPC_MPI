#include <iostream>
#include <vector>
#include <time.h>

#define N 100000000

using namespace std;

int main() {
    vector<bool> marked(N+1, false); 
    int k = 2;
    while (k * k <= N) {
        for (int i=k*k; i<=N; i+=k) marked[i] = true;    

        for (int i=k+1; i<=N; i++) {
            if (marked[i] == false) {
                k = i;
                break;
            }
        }
    }

    int res =  0;
    for (int i=2; i<=N; i++) {
        if (marked[i] == false) res++;
    }

    printf("\033[32mTotal prime number: %d\033[0m\n", res);
    printf("\033[36mTime: %.3lf sec\033[0m\n", ((double)clock() / CLOCKS_PER_SEC));
    return 0;
}
