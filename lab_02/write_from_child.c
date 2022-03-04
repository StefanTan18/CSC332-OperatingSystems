#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[]) {

    if (2 != argc) // Incorrect usage of the program
    {
        printf("\n Usage : ./write_from_child [filename]\n");
        return 1;
    }

    pid_t c_pid;
    int fd;
    char* filename = argv[1]; // File name
    int pipefd[2];

    // pipe file descriptors is created
    pipe(pipefd);

    // Child is forked
    c_pid = fork();

    // Fork failed
    if (c_pid == -1) {
        printf("fork failed.\n");
    }

    if (c_pid == 0) {
        // Child Process
        //printf("in child process. Child pid: %d\n", getpid());

        close(pipefd[0]);   // close read end of the pipe

        errno = 0;  // For error handling
        fd = open(filename, O_RDONLY);

        // Error Handling: Error occured when opening file
        if (fd == -1)
        {
            if (errno == ENOENT) {
                printf ("%s does not exist\n", filename); // File given does not exist
                perror("open"); // Explanation of error is printed
            }
            else if (errno == EACCES) {
                printf ("%s is not accessible\n", filename); // Do not have read permissions for the file
                perror("open"); // Explanation of error is printed
            }
            return 1;
        }

        char buffer[1]; // Buffer to hold the char that is read from the file
        while(read(fd, buffer, 1)) {
            write(pipefd[1], buffer, 1); // Writes the buffer that is read to the write end of the pipe
        }
        close(fd); // Closes readme.txt
        close(pipefd[1]); // Closes write end of the pipe
    }
    
    else if (c_pid > 0) {
        // Parent Process
        int status;
        waitpid(c_pid, &status, 0); // waits for the child process with a pid of c_pid

        close(pipefd[1]);   // closes write end of the pipe

        errno = 0;  // For error handling
        fd = open(filename, O_WRONLY | O_APPEND);   // opens the file in write only mode and will write at the end of file

        // Error Handling: Error occured when opening file
        if (fd == -1)
        {
            if (errno == EACCES) {
                printf ("%s is not accessible\n", filename); // Do not have write permissions for the file
                perror("open"); // Explanation of error is printed
            }
            return 1;
        }

        // Parent will append to readme.txt using the read end of the pipe
        char buffer[1]; // Buffer to hold the char that is read from the file
        write(fd, "\n\nParent is writing:\n", 21);
        while(read(pipefd[0], buffer, 1)) {
            write(fd, buffer, 1);
        }
        close(fd); // Closes readme.txt
        close(pipefd[0]); // closes read end of the pipe
    }
    return 0;
}