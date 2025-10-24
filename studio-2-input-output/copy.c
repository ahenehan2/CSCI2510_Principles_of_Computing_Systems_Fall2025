// copy.c — a tiny "cat" clone using Unix system calls read() and write()
// Anne Henehan — CSCI 2510: Studio - Input/Output
//
// HOW IT WORKS (high level):
// - We repeatedly read bytes from standard input (file descriptor 0)
// - We immediately write those bytes to standard output (file descriptor 1)
// - We stop when read() returns 0 (EOF). If read()/write() errors, we print why and exit.
//
// WHY SYSTEM CALLS?
// - read() and write() are low-level, POSIX system calls (not stdio). They live in <unistd.h>.
// - read() returns how many bytes it actually read (could be less than the buffer size).
// - write() might also write fewer bytes than we asked, so we loop until all bytes are written.
//
// HOW IT WORKS (low level):
// - read() reads bytes from the given file descriptor into the given buffer
// - write() writes bytes from the given buffer to the given file descriptor



#include <unistd.h>   // read(), write(), STDIN_FILENO, STDOUT_FILENO
#include <errno.h>    // errno, EINTR
#include <string.h>   // strerror()
#include <stdio.h>    // perror()
#include <stdlib.h>   // exit()

// You can tweak this later to test performance vs. correctness
#define BUFFER_SIZE 200

int main(void) {
    // Temporary storage for bytes we read before writing them out
    char buffer[BUFFER_SIZE];

    for (;;) {
        // Read up to BUFFER_SIZE bytes from standard input (keyboard/file/pipe)
        // Return values:
        //  >0  : number of bytes actually read
        //   0  : EOF (end of file) — stop
        //  -1  : error (check errno)
        ssize_t nread = read(STDIN_FILENO, buffer, BUFFER_SIZE);

        if (nread == 0) {
            // EOF reached: we're done
            break;
        }
        if (nread < 0) {
            // If a signal interrupted the read, try again
            if (errno == EINTR) continue;
            perror("read");
            return 1;
        }

        // Write exactly the bytes we read. write() can write fewer than requested,
        // so loop until we've written them all.
        ssize_t written = 0;
        while (written < nread) {
            ssize_t nw = write(STDOUT_FILENO, buffer + written, nread - written);
            if (nw < 0) {
                if (errno == EINTR) continue; // interrupted; retry
                perror("write");
                return 1;
            }
            written += nw;
        }
    }

    return 0; // success
}



