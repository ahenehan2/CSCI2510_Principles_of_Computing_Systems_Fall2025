//Replace this file with the contents of your hello_fprintf.c
// Anne Henehan
// 2025-09-09
// Program prints a message using fprintf()

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    fprintf(stdout, "Hello, world!\n");
    return 0;
}

// Anne Henehan
// 2025-09-09
// Prints a message using the write() system call

#include <unistd.h>   // write, STDOUT_FILENO

int main(int argc, char* argv[]) {
    const char msg[] = "Hello, world!\n";
    // sizeof(msg) is total bytes including the '\0'; write one less
    (void)write(STDOUT_FILENO, msg, sizeof(msg) - 1);
    return 0;
}
