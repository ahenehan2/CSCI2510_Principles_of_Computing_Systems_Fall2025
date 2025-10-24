#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // getpid, fork, execvp, sleep
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // waitpid

/*
 * CSCI 2510 - Studio 5: fork() and exec()
 * This single program evolves through the required steps.
 * Build:  gcc -Wall -Wextra -O2 -o fork fork.c
 * Run:    ./fork
 */

int main(int argc, char *argv[]) {
    // --- Step 1: print current process PID ---
    // (If you run the unmodified program 3 times, you'll see different PIDs.)
    // printf("Step 1: PID=%d\n", (int)getpid());

    // --- Step 2: create a child with fork() ---
    pid_t child = fork();

    if (child < 0) {
        perror("fork");
        return 1;
    }

    if (child == 0) {
        // --- Child process ---
        // For Step 5: simulate a slow child
        sleep(3);

        // --- Step 7: execvp to run `ls -l` in the child ---
        // Static argv for the child program
        char *child_argv[] = {"ls", "-l", NULL};
        char *child_prog = child_argv[0];

        // Note: Once execvp succeeds, it REPLACES the child process image.
        // Any code after execvp will not run (unless execvp fails).
        execvp(child_prog, child_argv);

        // If we reach here, exec failed.
        perror("execvp");
        _exit(127);
    } else {
        // --- Parent process ---
        // --- Step 6: wait for the specific child before printing ---
        int status = 0;
        if (waitpid(child, &status, 0) == -1) {
            perror("waitpid");
            return 1;
        }

        // After the child (which ran `ls -l`) finishes, print parent PID.
        printf("Parent (PID=%d) waited for child (PID=%d). Exit status=%d\n",
               (int)getpid(), (int)child, WIFEXITED(status) ? WEXITSTATUS(status) : -1);
    }

    return 0;
}
