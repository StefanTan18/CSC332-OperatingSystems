#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main() {

    pid_t pid, c1_pid, c2_pid; // variables to store pids of the parent and children

    c1_pid = fork(); // Child one is created
    
    if (c1_pid == 0) {
        // Child One Code
        printf("I am child one, my pid is: %d\n", getpid()); // child one prints out a message
    }

    else {
        c2_pid = fork(); // Child two is created

        if (c2_pid == 0) {
            // Child Two Code
            printf("I am child two, my pid is: %d\n", getpid()); // child two prints out a message
        }

        else {
            // Parent Code
            int status;
            waitpid(c1_pid, &status, 0); // waiting for child one
            waitpid(c2_pid, &status, 0); // waiting for child two
            
            printf("In parent process. Parent pid: %d\n", getpid()); // parent prints out a message
        }
    }
    return 0;
}