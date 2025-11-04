#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int fd[2];  // fd[0] = read end, fd[1] = write end
    pid_t c1, c2;

    // 1. create the pipe in the parent
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    // 2. fork first child (Program 1 = print)
    c1 = fork();
    if (c1 < 0) {
        perror("fork");
        exit(1);
    }

    if (c1 == 0) {
        // --- child 1 (Program 1) ---
        // we want Program 1's STDOUT -> write end of pipe

        // dup2(write_end, STDOUT_FILENO)
        if (dup2(fd[1], STDOUT_FILENO) == -1) {
            perror("dup2 child1");
            exit(1);
        }

        // child 1 does NOT need the read end
        close(fd[0]);
        // we already duplicated fd[1] to stdout, so we can close original
        close(fd[1]);

        char *cmd = "./print";
        char *argv[] = {"print", NULL};
        execvp(cmd, argv);
        perror("execvp print");
        exit(1);
    }

    // 3. fork second child (Program 2 = echo)
    c2 = fork();
    if (c2 < 0) {
        perror("fork");
        exit(1);
    }

    if (c2 == 0) {
        // --- child 2 (Program 2) ---
        // we want Program 2's STDIN <- read end of pipe

        if (dup2(fd[0], STDIN_FILENO) == -1) {
            perror("dup2 child2");
            exit(1);
        }

        // child 2 does NOT need the write end
        close(fd[1]);
        // we already duplicated fd[0] to stdin, so close original
        close(fd[0]);

        char *cmd = "./echo";
        char *argv[] = {"echo", NULL};
        execvp(cmd, argv);
        perror("execvp echo");
        exit(1);
    }

    // --- parent process ---
    // parent is not reading or writing the pipe → close both ends
    close(fd[0]);
    close(fd[1]);

    // wait for both children
    waitpid(c1, NULL, 0);
    waitpid(c2, NULL, 0);

    return 0;
}