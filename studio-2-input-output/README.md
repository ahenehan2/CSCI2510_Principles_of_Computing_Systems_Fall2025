# CSCI 2510: Studio - Input/Output

Linux stream redirection is a powerful feature where program inputs and
outputs can be hooked together seamlessly by the system to build
compound commands. In this studio, you will:

1.  Use the `read()` and `write()` functions to read input and write
    output
2.  Redirect program inputs and outputs

------------------------------------------------------------------------

Please complete the required exercises below, as well as any optional
enrichment exercises that you wish to complete.

As you work through these exercises, please record your answers in a
text file. When finished, submit your work by pushing your text file and
source code to Canvas. 

------------------------------------------------------------------------

### Required Exercises

1.  One basic Linux tool you might know about is a program called `cat`.
    This program con*cat*enates (appends) any provided input to standard
    output. It\'s a useful way to print text files to the terminal, or
    to write bits of data into a text file.

    Go ahead and type the command \"`cat`\" and hit enter. Then, write a
    few lines of text to the console (and hit enter after each line). As
    the answer to this exercise, describe what happens.

    You can type CTRL-D on a blank line in order to quit `cat`. Typing
    CTRL-D sends a special \"end of file\" character to the terminal. It
    is common for terminal programs to act like they're reading a file,
    so CTRL-D commonly terminates such programs. For example, if you type
    CTRL-D at an empty terminal prompt, it's likely that your terminal will
    close itself!

    My response: Running `cat` with no arguments echoed each line I typed to the terminal immediately; pressing CTRL-D on a blank line sent EOF and exited `cat`.

2.  The original purpose of `cat` is to print text files to the console.
    Find your favorite text file if you\'d like, or you can use one
    of mine. This repository should already contain the `mars.txt` file
    for you to use.

    In the last exercise, the `cat` program got its data from standard
    input (i.e. the keyboard). One neat feature of Linux is that it
    allows us to *redirect* streams like standard input and standard
    output. Using your text file, redirect standard input with the
    \'`<`\' character as such:

    `cat < mars.txt`

    As the answer to this exercise, describe what happens.

    My response: The contents of `mars.txt` printed to the terminal; stdin was redirected from the file instead of the keyboard.

3.  We can also redirect output. This time, redirect standard output
    with the \'`>`\' character into a new text file. Use the syntax:

    `cat < mars.txt > new.txt`

    Open up the `new.txt` file. As the answer to this exercise, describe
    what happens.

    My response: Nothing printed to the screen because stdout was redirected to `new.txt`. Opening `new.txt` shows an exact copy of `mars.txt` (overwrites if it already existed).

4.  Now try the following command `cat > new.txt`

    Enter a few lines of text by pressing enter. Open up your text file to see the
    results. Remember that you can quit `cat` with CTRL-D.

    As the answer to this exercise, describe what happens.

    My response: The lines I typed were written into `new.txt`; single `>` overwrote any previous contents. CTRL-D ended input.

5.  One last useful tip. Repeat the last exercise, but instead of using
    a single greater-than symbol, use two. (That is, use \'`>>`\'
    instead of \'`>`\'). Try running this command several times.

    What happens now?

    My response: Using `>>` appended my new lines to the end of `new.txt`. Re-running the command continues appending.

6.  Now we\'ll write a short program that mimics the behavior of `cat`.
    Start by creating a new file called `copy.c` and fill it out with
    the framework for an empty program (see Studio 01 if you\'d like to
    see that framework again).

    Look at the manual pages for the system calls `read()` and
    `write()`, and look at your code from Studio 01 to see how you used
    `write()`. What header file do you need to include to use these
    system calls?

    My response: `#include <unistd.h>` (provides `read()`, `write()`, `STDIN_FILENO`, and `STDOUT_FILENO`).

7.  Both functions require a *buffer* (a fixed region of memory) to
    operate. At the top of your program, before the `main()` declaration,
    define a preprocessor constant with the size of your buffer as such:

    `#define bufferSize 200`

    Then allocate an array of characters to be your buffer:

    `char buffer[ bufferSize ];`

    Leave this answer blank.

8.  The behavior of `cat` is to read input forever until it encounters
    an end of file character. Look up the documentation for the `read()`
    system call. What is the return value ***type*** for `read()`? What
    is the specific return value that indicates the end of a file?

    My response: Return type is `ssize_t`. EOF is indicated when `read()` returns `0` (errors return `-1` with `errno` set).



9.  Your last job is to translate the following algorithm into C code.
    Inside an infinite loop:

    1.  Read from standard input (`STDIN_FILENO`) into the buffer.
    2.  If the return value indicates end of file, then use the
        `break` command to stop the loop.
    3.  Write the contents of the buffer to standard output
        (`STDOUT_FILENO`). Be careful to use the return value from
        `read()` so as not to write more than was read.

    When you are satisfied with your code, compile it and test it in all
    the ways we tested `cat` above. Does your program\'s behavior match?
    Note that you will need to invoke your program with \"`./`\", as in:

    `./copy < mars.txt`

    My response: Implemented in `copy.c` using a loop that reads into a buffer, breaks on `read()` returning 0 (EOF), and writes exactly the number of bytes read (handling short writes and `EINTR`). Tested with `./copy`, `./copy < mars.txt`, and `./copy < mars.txt > out.txt`; behavior matches `cat`.

10. Do you think that modifying `bufferSize` will affect the correctness
    of your program? What if `bufferSize` is very small, or even equal
    to one? Try a few different values for `bufferSize` and record the
    results.

    My response: Changing `bufferSize` does not affect correctness because I always write exactly the number of bytes returned by `read()`. With very small buffers (e.g., 1), output is still correct but slower (more system calls). Larger buffers (e.g., 4096) are faster on large files.


11. What do you think the tradeoff is between having a small
    `bufferSize` versus having a large one?

    My response: Small buffer ⇒ minimal memory but many syscalls (slower). Large buffer ⇒ fewer syscalls (usually faster) but more memory with diminishing returns beyond a few KB. A practical default range is 1–8 KB.



### Optional Enrichment Exercises

1.  You can write the same program on top of the C standard library as
    well! Instead of using `read()` and `write()`, try using the libc
    functions `fgets()` and `fprintf()`.

------------------------------------------------------------------------
