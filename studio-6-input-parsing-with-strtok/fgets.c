#include <stdio.h>

int main(void) {
    char buffer[1024];

    printf("Enter something: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        perror("fgets");
        return 1;
    }

    printf("%s\n", buffer);
    return 0;
}