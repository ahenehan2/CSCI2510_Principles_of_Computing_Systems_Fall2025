# CSCI 2510: Studio 8 - Signals

*Signals* are a mechanism for basic inter-process notification and event
handling. Signals do not convey any data in themselves, but they do
allow cooperating processes to invoke remote functions in other
processes. In addition, signals are the mechanism central to handling
certain basic functions you expect from an operating system, such as the
ability to interrupt, suspend, or otherwise modify a currently running
program.

In this studio, you will:

1.  Send signals with keyboard shortcuts
2.  Customize signal handling behavior by registering signal handlers

------------------------------------------------------------------------

Please complete the required exercises below, as well as any optional
enrichment exercises that you wish to complete.

As you work through these exercises, please record your answers in a
text file. When finished, submit your work via the Git repository.

------------------------------------------------------------------------

### Required Exercises

1.  Download the program [sleep.c](./sleep.c). This program runs an
    infinte loop in a way that doesn\'t burn a lot of CPU cycles.
    Compile and run this program to see what it does. Use CTRL-C to stop
    the program.

    Copy and paste some program output as the answer to this exercise.

    answer: 
$ gcc -o sleep sleep.c
$ ./sleep
Sleeper's PID is: 21437
Slept for 0 iterations!
Slept for 1 iterations!
Slept for 2 iterations!
Slept for 3 iterations!
Slept for 4 iterations!
^C
$

2.  When you press CTRL-C on the keyboard you\'re actually generating a
    special signal called the *interrupt signal*, or *SIGINT* for short.
    The default behavior of this signal is to kill the recieving
    process, and since most programs don\'t handle this signal with
    customized behavior the result is normally that the process is
    killed.

    Open the manual page at `man 7 signal` or `man 3 signal` on MacOS. Scroll down to the list of
    standard signals. What numeric value does SIGINT have? What is the
    associated comment?

    answer:
    SIGINT is signal 2, and the comment is "Interrupt from keyboard".

3.  Open a second terminal. Start the `sleep.c` program in the first
    terminal and make a note of its PID. In the second terminal use the
    `kill` command documented at `man 1 kill` to send the SIGINT signal
    to the sleeping process. *(Note- despite the name, the `kill`
    command can send any signal to a process, not just the kill
    signal!*. The syntax for doing this is below:

    `kill -s <signal number> <pid>`

    What happens to the sleeper process?

    answer:
    The sleep process receives SIGINT (2) and is terminated, just like when you press CTRL-C in that terminal.

4.  Go back to `man 7 signal` (or `man 3 signal` on MacOS) and look through the list of standard
    signals. Pick another signal to send to a sleeping process. What
    signal did you pick? What happened? Copy and paste the results.

    answer:
    
I picked SIGTERM (15).
Result: the sleeper process was terminated and the shell printed Terminated ./sleep.

5.  Now download the program [call\_sleeper.c](./call_sleeper.c). This
    program `fork()`s and `exec()`s a sleeper program, and then waits
    for it to finish. After the child returns it will print out a
    success message.

    Try finishing and running `call_sleeper.c` and then terminating the sleeper with
    CTRL-C. Which processes terminate: the sleeper, the caller, or both?
    Did the calling program print its success message?

    answer:
    Both the caller and the sleeper are terminated by CTRL-C, and the calling program does not print its success message.

6.  We want to be able to interrupt the sleeper process without
    interrupting the caller process. To do so, we can define a custom
    *signal handler* that intercepts the SIGINT and ignores it. To do
    so, we can use the `signal()` system call that is documented at
    `man signal`.

    *Note: There is an newer function called `sigaction()` that is the
    newer, better way to do this. However, the interface is much more
    complicated (but flexible!) so we will use the `signal()` function
    instead.*

    First you will need to write the signal handler function. This
    function should have the signature:

    `void <function_name> ( int signum )`

    In the body of that function, print a message stating that your
    program is ignoring SIGINT. Then, in the `main()` function, make a
    call to `signal()`. The first parameter should be the signal number
    for SIGINT and the second should be the name of your signal handler
    function.

    Copy and paste your signal handling code.

    answer: 
    #include <signal.h>

void handle_sigint(int signum) {
    printf("\nParent: received SIGINT (%d) but is ignoring it.\n", signum);
}

int main(int argc, char* argv[]) {
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        perror("signal");
        exit(-1);
    }

}

7.  Finally, run your new caller program and interrupt it with CTRL-C.
    If done correctly, your sleeper should terminate but your caller
    should simply print your statement that SIGINT is being ignored.
    Then, the caller will print a statement indicating that it
    successfully waited for the sleeper.

    Copy and paste your program output.

answer: 
$ gcc -o call_sleeper call_sleeper.c
$ ./call_sleeper
Forking sleeper...
Waiting for sleeper 22567...
Sleeper's PID is: 22567
Slept for 0 iterations!
Slept for 1 iterations!
Slept for 2 iterations!
^C
Parent: received SIGINT (2) but is ignoring it.
Parent finished waiting and returned successfully!
$

    *NOTE: Once you handle SIGINT in this way, you will no longer be
    able to kill your process with CTRL-C. If you run into trouble you
    can still terminate your program with CTRL-\\, which sends the
    SIGQUIT signal.*

8.  Signals are frequently used in computer systems to do *asynchronous
    event handling*- dealing with events that can\'t be predicted
    accurately. One example might be clicking the red X box on a GUI
    window to close a program. In this case the OS might send a signal
    to the GUI process to tell it to terminate. Give another situation
    where signal handling might be useful.

    answer:
    Another place where signal handling is useful is when a parent process needs to react to its children finishing. When a child process exits, the OS can send the parent a SIGCHLD signal. The parent can catch SIGCHLD with a signal handler, call waitpid() to reap the child, and free any associated resources. This lets the parent handle child termination asynchronously, without constantly polling.

### Optional Enrichment Exercises

1.  One frequently used program that handles SIGINT with a custom
    handler is the terminal shell. Try pressing CTRL-C at an empty
    terminal- what happens? Why is this behavior important?

    You want CTRL-C to cancel the current foreground command (like cat, sleep, your C program, etc.), not destroy your entire shell session.
- If the shell died every time you hit CTRL-C with nothing running, you’d constantly lose your environment, history, and any work you had sitting around.
- By handling SIGINT with a custom handler, the shell can:
Kill child processes when needed
Stay alive itself
Keep giving you a prompt to run new commands

2.  Linux provides two user-defined signals SIGUSR1 and SIGUSR2
    specifically for implementing program functionality on top of
    signals. Try using the system call documented at `man 2 kill` to
    implement asynchronous program communication. For example, have one
    process wait in a loop until it recieves SIGUSR1, and then perform
    some useful work.

------------------------------------------------------------------------
