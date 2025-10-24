#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 200

int main(void) {
    char buf[BUFFER_SIZE];
    while (fgets(buf, BUFFER_SIZE, stdin) != NULL) {
        if (fputs(buf, stdout) == EOF) {
            perror("fputs");
            return 1;
        }
    }
    if (ferror(stdin)) { perror("fgets"); return 1; }
    return 0;
}
