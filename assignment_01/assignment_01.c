#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main() {

    pid_t pid, c1_pid, c2_pid; // variables to store pids of the parent and children
    int pipefd[2];             // variables to store pipe file descriptors, pipefd[0] = read end, pipefd[1] = write end

    // pipe file descriptors is created
    pipe(pipefd);

    c1_pid = fork(); // Child 1 is forked

    // Checks to see if fork for child 1 failed
    if (c1_pid < 0) {
        printf("Fork for child 1 failed.\n");
        return 1;
    }
    
    // Child 1 Process Code
    else if (c1_pid == 0) {

        /* FOR TESTING PURPOSES
        printf("I am child one, my pid is: %d\n", getpid());
        */

        dup2(pipefd[1], STDOUT_FILENO);     // Allows the output to be sent to the write end of the pipe
        close(pipefd[0]);                   // Closes the read end of the pipe
        close(pipefd[1]);                   // Closes the write end of the pipe
        execlp("ls", "ls", "-F", NULL);     // Executes the ls -F command
    }

    else {
        c2_pid = fork(); // Child 2 is forked

        // Checks to see if fork for child 2 failed
        if (c2_pid < 0) {
        printf("Fork for child 2 failed.\n");
        return 1;
        }
    
        // Child 2 Process Code
        else if (c2_pid == 0) {

            /* FOR TESTING PURPOSES
            printf("I am child two, my pid is: %d\n", getpid());
            */

            dup2(pipefd[0], STDIN_FILENO);  // Allows the read end of the pipe to be sent to the input
            close(pipefd[0]);               // Closes the read end of the pipe
            close(pipefd[1]);               // Closes the write end of the pipe
            execlp("nl", "nl", NULL);       // Executes the nl command
        }

        // Parent Process Code
        else {
            int status;
            close(pipefd[0]);               // Closes the read end of the pipe
            close(pipefd[1]);               // Closes the write end of the pipe
            waitpid(c1_pid, &status, 0);    // waiting for child one
            waitpid(c2_pid, &status, 0);    // waiting for child two
            
            /* FOR TESTING PURPOSES
            printf("In parent process. Parent pid: %d\n", getpid());
            */
        }
    }
    return 0;
}