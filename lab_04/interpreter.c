#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int bufferLength = 100;                     // length of the buffer is set to 100
    char cmdLine[bufferLength];                 // char array to hold the input that is read

    int pipeIndex = 0;                          // used to store the index of the pipe '|'

    while(1) {
        printf("command: ");                    // print prompt that asks for user input
        fgets(cmdLine, bufferLength, stdin);    // gets the user input

        size_t len = strlen(cmdLine);
        // replaces the new line character with a null character
        if (cmdLine[len - 1] == '\n') {
            cmdLine[len - 1] = '\0';
        }

        // terminates the program if the user inputs "exit"
        if (strcmp(cmdLine, "exit") == 0) {
            break;
        }

        // splits the input into the command and its arguments based on the whitespace
        char delim[] = " ";
        char* arg_list[bufferLength];
        int arg_count = 0;

        char* ptr = strtok(cmdLine, delim);

        while (ptr != NULL) {
            arg_list[arg_count] = ptr;
            // checks to see if there is a pipe
            if (strcmp(ptr, "|") == 0) {
                pipeIndex = arg_count;
            }
            ptr = strtok(NULL, delim);
            arg_count++;
        }
        arg_list[arg_count] = NULL;             // last element of the array is set to be a NULL

        pid_t c_pid;                            // variable to store the pid of the child process
        int pipefd[2];                          // variables to store pipe file descriptors

        pipe(pipefd);
        c_pid = fork(); // child is forked

        // Checks to see if the fork failed
        if (c_pid == -1) {
            printf("Fork failed.\n");
            return 1;
        }

        // Child Process Code
        if (c_pid == 0) {
            // user inputted a command with a pipe
            if (pipeIndex != 0) {
                // gets the first command
                char* firstCmd[pipeIndex + 1];
                memcpy(firstCmd, arg_list, pipeIndex * sizeof(*arg_list));
                firstCmd[pipeIndex] = NULL;
                dup2(pipefd[1], STDOUT_FILENO); // allows the output to be sent to the write end of the pipe
                close(pipefd[0]);               // closes the read end of the pipe
                close(pipefd[1]);               // closes the write end of the pipe
                execvp(firstCmd[0], firstCmd);  // executes the first command
            }

            execvp(arg_list[0], arg_list);      // executes the command as is if there is no pipe
        }
        
        // Parent Process Code
        if (c_pid > 0) {
            int status;
            waitpid(c_pid, &status, 0);         // waits for the child process with a pid of c_pid

            if (pipeIndex != 0) {
                
                c_pid = fork(); //child is forked

                // Checks to see if the fork failed
                if (c_pid == -1) {
                    printf("Fork failed.\n");
                    return 1;
                }

                // Child Process Code
                if (c_pid == 0) {
                    // gets the second command
                    int secondCmdLen = arg_count - pipeIndex;
                    char* secondCmd[secondCmdLen];
                    memcpy(secondCmd, &arg_list[pipeIndex + 1], secondCmdLen * sizeof(*arg_list));
                    dup2(pipefd[0], STDIN_FILENO);      // allows the read end of the pipe to be sent to the input
                    close(pipefd[1]);                   // closes the write end of the pipe
                    close(pipefd[0]);                   // closes the read end of the pipe
                    execvp(secondCmd[0], secondCmd);    // executes the second command
                }

                // Parent Process Code
                if (c_pid > 0) {
                    int status;
                    close(pipefd[0]);                   // closes the read end of the pipe
                    close(pipefd[1]);                   // closes the write end of the pipe
                    waitpid(c_pid, &status, 0);         // waits for the child process with a pid of c_pid
                }
            }
        }
    }

    return 0;
}