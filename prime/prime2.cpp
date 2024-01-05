#include <time.h>
#include <iostream>
#include <vector>
#define N 100000000

bool prime[N] = {0};
std::vector<int> pri = {2};

int main() {
    for (int i = 3; i < N; i += 2) {
        if (!prime[i]) pri.push_back(i);

        for (int p : pri) {
            if (i * p >= N) break;
            prime[i * p] = true;
            if (i % p == 0) break;
        }
    }
    std::cout << pri.size() << "\n";
    std::cout << "Time: " << ((double)clock() / CLOCKS_PER_SEC) << " sec" << std::endl;
}