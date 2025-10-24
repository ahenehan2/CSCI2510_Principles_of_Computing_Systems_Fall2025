//Replace this file with the contents of your hello_fprintf.c
// Anne Henehan
// 2025-09-09
// Program prints a message using fprintf()

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    // print message
    fprintf(stdout, "Hello, world!\n");

    // variables
    int a;
    double b;
    char c;

    // show sizes
    printf("int: %zu\n", sizeof(a));
    printf("double: %zu\n", sizeof(b));
    printf("char: %zu\n", sizeof(c));
    printf("long long: %zu\n", sizeof(long long));

    return 0;
}
