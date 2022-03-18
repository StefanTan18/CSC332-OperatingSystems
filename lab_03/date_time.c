#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main() {

    pid_t c_pid;    // variable to store the pid of the child process

    c_pid = fork(); // child is forked

    // Checks to see if the fork failed
    if (c_pid == -1) {
        printf("Fork failed.\n");
        return 1;
    }

    // Child Process Code
    if (c_pid == 0) {
        printf("Child process successfully forked. Child pid: %d\n", getpid()); // Child pid is printed

        execl("/bin/date", "/bin/date", NULL);  // the date command is executed
    }
    
    // Parent Process Code
    if (c_pid > 0) {
        int status;
        waitpid(c_pid, &status, 0); // waits for the child process with a pid of c_pid
    }

    return 0;
}