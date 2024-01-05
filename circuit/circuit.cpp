#include <iostream>
#include <time.h>
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
    int cs = 1000, count = 0;

    for (int it=0; it<cs; it++) {
        for (int bits=0; bits<=65535; bits++) {
            if (circuit(bits) && it == 0) {
                // printf("Found solution: ");
                // print_bit(bits, 16);
                // printf("\n");
                count++;
            }
        }
    }

    printf("\033[32mTotal: %d\033[0m\n", count);
    printf("\033[36mTime: %lf sec\033[0m\n", ((double)clock() / CLOCKS_PER_SEC));
    return 0;
}