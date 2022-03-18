#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    char* p1 = argv[1]; // Name of Process 1 Output File
    char* p2 = argv[2]; // Name of Process 2 Output File

    if (argc != 3) // Incorrect usage of the program
    {
        printf("\n Usage : ./Parent_Prcs.c [Process1 Executable] [Process2 Executable]\n");
        return 1;
    }
    
    pid_t pid, c1_pid, c2_pid; // variables to store pids of the parent and children

    c1_pid = fork(); // Child one is created
    
    if (c1_pid == 0) {
        // Child One Code
        // printf("I am child one, my pid is: %d\n", getpid()); // child one prints out a message

        char* args_list[] = {p1, NULL};     // arguments list for the command that will be executed
        execv(p1, args_list);               // executes process 1
        return 0;
    }

    else {
        c2_pid = fork(); // Child two is created

        if (c2_pid == 0) {
            // Child Two Code
            // printf("I am child two, my pid is: %d\n", getpid()); // child two prints out a message

            char* args_list[] = {p2, NULL};     // arguments list for the command that will be executed
            execv(p2, args_list);               // executes process 2
            return 0;
        }

        else {
            // Parent Code
            int status;
            waitpid(c1_pid, &status, 0); // waiting for child one
            waitpid(c2_pid, &status, 0); // waiting for child two
            
            // printf("In parent process. Parent pid: %d\n", getpid()); // parent prints out a message
        }
    }
    return 0;
}