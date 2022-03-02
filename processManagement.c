#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

// Example with pid, fork, sleep and wait (NOT PART OF LAB 2)
int main() {

    pid_t pid, c_pid;
    int x = 6;

    c_pid = fork();

    if (c_pid == -1) {
        printf("fork failed.\n");
    }

    if (c_pid == 0) {
        printf("in child process. Child pid: %d\n", getpid());
        // sleep(2);
        x = x + 2;
        printf("in child value of x: %d\n", x);
    }
    
    if (c_pid > 0) {
        // sleep(1);
        // wait(NULL);  // waits for any child process to finish

        int status;
        waitpid(c_pid, &status, 0); // waits for the child process with a pid of c_pid

        printf("in parent process. Parent pid: %d\n", getpid());
        x = x - 2;
        printf("in parent value of x: %d\n", x);
    }

    printf("My work is done.\n");
}