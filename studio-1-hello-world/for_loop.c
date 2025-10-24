#include <stdio.h>
#define N 100000

int main() {
    double a= 1.0;
    for (int i = 1; i <= N; i++) {
        printf("Number: %d\n", i);
        a+=2;
    }
    return 0;
}