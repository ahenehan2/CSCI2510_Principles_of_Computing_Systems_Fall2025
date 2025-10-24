# CSCI 3500: Studio 4 - Error Checking

Any non-trivial program requires you to call functions provided by
either the operating system or system libraries. These functions almost
always provide a return value, indicating whether an operation succeeded
or failed. Checking these return values are vital to producing robust
code, and greatly simplify debugging.

*Checking return values appropriately will be a requirement for lab 2
and all subsequent labs!*

In this studio, you will:

1.  Look up return values in a function\'s man page
2.  Check return values and handle errors appropriately
3.  Print appropriate error messages with `perror()`

------------------------------------------------------------------------

Please complete the required exercises below, as well as any optional
enrichment exercises that you wish to complete.

As you work through these exercises, please record your answers in a
text file. When finished, submit your work via the Git repository.

------------------------------------------------------------------------

### Required Exercises

1.  Create a new file called `open.c`. Write a short program that
    accepts a single command line argument. This argument should be
    interpreted as a file name that your program attempts to open. Once
    the file is opened, your program should print the contents of the
    file to standard output. For example, the command
    \"`./open mars.txt`\" should open the file `mars.txt` and print it
    to the console. Use the `open()` function documented at
    `man 2 open`.

    *Hint: start with your program from studio 2, called `copy.c`.
    Modify this program to open a file, and then modify the `read()`
    statement to read from your file descriptor rather than standard
    input. Alternately, start with your program from Lab 1.*

    As the answer to this exercise, copy and paste your working code.

#include <fcntl.h>      // open
#include <unistd.h>     // read, write, close
#include <errno.h>      // errno
#include <stdio.h>      // perror, printf
#include <stdlib.h>     // exit
#include <string.h>     // strerror

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *path = argv[1];

    // open() returns a non-negative file descriptor on success, -1 on error.
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        // #6 perror() prints "<prefix>: <human-readable errno>"
        perror("Error opening file");
        // #7 exit with non-zero to signal failure
        return 1;
    }

    // read -> write loop
    char buf[4096];
    while (1) {
        ssize_t n = read(fd, buf, sizeof(buf));
        if (n == 0) {
            // EOF
            break;
        } else if (n == -1) {
            perror("Error reading file");
            close(fd);  // best effort; if this fails we’ll report below
            return 1;
        }

        // Handle short writes
        ssize_t written = 0;
        while (written < n) {
            ssize_t m = write(STDOUT_FILENO, buf + written, (size_t)(n - written));
            if (m == -1) {
                perror("Error writing to stdout");
                close(fd);
                return 1;
            }
            written += m;
        }
    }

    // close() returns 0 on success, -1 on error.
    if (close(fd) == -1) {
        perror("Error closing file");
        return 1;
    }

    return 0;
}

2.  What happens when your argument specifies a file that doesn\'t
    exist?

    open() returns -1, sets errno to ENOENT (“No such file or directory”), my perror("Error opening file") prints a helpful message, and the program exits with a non-zero status.

3.  Create a text file called `perm.txt` and fill it with some short
    message. Execute the command \"`chmod -rw perm.txt`\". This command
    removes read and write permissions from the file. Now try to execute
    your program on this file- what happens?

    Opening perm.txt with O_RDONLY fails: open() returns -1, sets errno to EACCES (permission denied). perror prints that, and the program exits with non-zero.

4.  Look at the man page for `open()`. There is a
    section called \"RETURN VALUE\". What value does `open()` return in
    the event of an error?

    -1 (and it sets errno to indicate the error).

5.  If you haven\'t already, insert an `if` statement directly after
    your call to `open()` to check for errors. Your error detection
    should check the criteria in the man page exactly. For example, if
    the reported error value for `open()` is -1, then check with the
    conditional \"`return_value == -1`\" rather than something like
    \"`return_value < 0`\". As the answer to this exercise, copy and
    paste the conditional you check.

    if (fd == -1) {
    /* ... */
}

6.  The function `open()` also sets a special variable called `errno`.
    Many system calls and library functions will set this variable when
    they execute. If an error occurs, this variable tells the system
    what happened. You can print a helpful error message with the
    function `perror()`.

    Insert a call to `perror()` inside your if statement from the last
    exercise. The only argument to this function is a short message that
    should describe the circumstances of the call. For example,
    `"Error opening file"` or `"Error reading input"`.

    As the answer to this exercise, copy and paste your call to
    `perror()`.

    perror("Error opening file");

7.  Finally, your program should exit gracefully when an error occurs.
    Insert a call to `return` or `exit()` with a negative value inside
    your `if` statement, but after `perror()`. Leave this answer blank.

8.  Now, what happens when the argument to your program specifies a file
    that doesn\'t exist?

    Same as #2: open() → -1, errno = ENOENT, perror prints a clear message, program exits with non-zero.

    Error opening file: No such file or directory

9. What happens when the argument specifies the file without read or
    write permissions- `perm.txt`?

    Same as #3: open() → -1, errno = EACCES, perror shows “Permission denied,” program exits non-zero.

    Error opening file: Permission denied

10. Look at the following manual pages and determine what value(s) are
    returned in the event of an error, and whether these functions set
    `errno`:
    -   man 2 open
    -   man 2 close
    -   man 2 read
    -   man 2 write
    -   man 3 printf

open(2): returns -1 on error; sets errno.
close(2): returns -1 on error; sets errno.
read(2): returns -1 on error (0 means EOF); sets errno.
write(2): returns -1 on error; sets errno.
printf(3): returns a negative value on error (otherwise number of chars printed). On POSIX systems, sets errno on error; also ferror(stdout) can be used to test the stream error flag.
(practical tip I’m keeping in mind: for stdio like printf, if something seems off, check both the return value and ferror(stdout).)

11. Give two reasons why error checking in this manner simplifies
    software development.

    1.It makes bugs much easier to diagnose. Using perror() along with errno immediately reveals why a system call failed, instead of leaving me to guess whether it was a missing file, a permission issue, or something else entirely.
    2. It prevents small errors from turning into larger problems. By checking return values after each system call, the program can stop or handle the issue right away, rather than continuing in an invalid state and causing more confusing failures later.

### Optional Enrichment Exercises

1.  The special variable `errno` is documented at `man errno`. There are
    a huge number of possible error statuses. Some of these are generic
    (e.g. insufficient permissions), while others are very specific
    (e.g. network host is unreachable).

    Having detailed error reporting presents the possibility that your
    programs can detect an error and attempt to self-correct, rather
    than quitting or crashing. For example, a common error status is
    `EAGAIN` or `EBUSY`, both of which indicate that an OS resource is
    currently unavailable. Rather than quitting, your program could
    simply wait and try again later.

    Browse through `man errno` and think about how you might handle some
    of the errors that can arise.

- EAGAIN / EWOULDBLOCK (resource temporarily unavailable): for non-blocking I/O or temporarily busy resources, I can retry after a short backoff (maybe usleep or nanosleep) instead of bailing.
- EBUSY (device or resource busy): similar idea—retry with a limit, or fall back to a different resource.
- EINTR (interrupted by signal): recommended practice is to retry the system call if it’s interruptible.
- ENOSPC (no space left on device): try flushing, truncating logs, or ask the user to free space, then retry.
- EMFILE/ENFILE (too many open files): close unused descriptors and retry open(). also a good signal to audit FD lifecycle.
- EPIPE (broken pipe during write): stop writing to that stream; maybe log it and continue other work instead of crashing.
- ECONNRESET/ETIMEDOUT (network hiccups): reconnect with exponential backoff.
my takeaway: if I check return values and branch on errno, I can often recover or degrade gracefully. if I ignore them, I just get crashes.

------------------------------------------------------------------------
