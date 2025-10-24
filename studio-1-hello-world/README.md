# CSCI 2510: Studio - Hello, world!

CSCI 2510 — Studio: Hello, world!
Name: Anne Henehan
Date: 2025-09-11

2) Header(s) required for fprintf():
   #include <stdio.h>

3) Signature difference between fprintf() and printf():
   fprintf(FILE *restrict stream, const char *restrict format, ...);
   printf(const char *restrict format, ...);
   // Difference: fprintf() takes an explicit FILE* (destination stream);
   // printf() omits it and writes to stdout.

4) Described difference (from DESCRIPTION):
   printf() is equivalent to fprintf(stdout, ...); fprintf() writes to the
   stream you specify (file, pipe, socket, etc.), while printf() always goes
   to standard output.

5) hello_fprintf.c — compile & run output (paste your own terminal output):
   $ gcc hello_fprintf.c -o hello_fprintf
   $ ./hello_fprintf
   Hello, world!

7) Manual section number for system calls (from `man man`):
   2

8) Header required to use write() (from `man 2 write`, SYNOPSIS):
   #include <unistd.h>

9) Copy command used to create hello_write.c (left intentionally blank per instructions):

11) hello_write.c source using write() (paste your final code here too if your instructor wants it):
   (See submitted hello_write.c file.)

10) What happens if the 3rd arg to write() is much larger than the buffer?
    The kernel will attempt to read that many bytes from the address you passed.
    Since your buffer isn’t that large, write() will read past it — that’s
    undefined behavior in user space. You may print extra “garbage” bytes,
    or even crash with a segmentation fault if you cross into unmapped memory.
    (It’s not a safe operation; the count must match the actual buffer length.)

11) When to use a C library function vs. an OS system call:
    - Use C library (e.g., fprintf, fopen) for portability and convenience:
      code compiles on different platforms and handles buffering/formatting.
    - Use a system call (e.g., write, open) when you need low-level, precise
      control or OS-specific behavior (nonblocking I/O, file descriptors,
      splice/sendfile, etc.), or to avoid stdio buffering in a tiny program.

12) hello.sh prints "Hello World!" (run example):
    $ ./hello.sh
    Hello World!