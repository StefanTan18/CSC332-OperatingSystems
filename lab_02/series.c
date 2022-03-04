#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[]) 
{
    if (2 != argc) // Incorrect usage of the program
    {
        printf("\n Usage : ./series [end of the series number]\n");
        return 1;
    }

    pid_t pid, c1_pid, c2_pid; // variables to store pids of the parent and children
    int s1, s2, s3; // variables to store the sum of the 3 series
    int end_of_series = atoi(argv[1]); // stores user inputted end of series number as an int
    // printf("%d\n", end_of_series); Testing to make sure input is correct

    c1_pid = fork(); // Child one is created
    
    if (c1_pid == 0) {
        // Child One Code
        s1 = 0;
        // Sum of the odd numbers from 1 to the user inputted end of series
        for (int i = 1; i <= end_of_series; i += 2) {
                s1 += i;
        }
        printf("I am child one. Sum of Series 1: %d\n", s1); // child one prints out a message
    }

    else {
        c2_pid = fork(); // Child two is created

        if (c2_pid == 0) {
            // Child Two Code
            s2 = 0;
            // Sum of the even numbers from 2 to the user inputted end of series
            for (int i = 2; i <= end_of_series; i += 2) {
            s2 += i;
            }
            printf("I am child two. Sum of Series 2: %d\n", s2); // child two prints out a message
        }

        else {
            // Parent Code
            int status;
            waitpid(c1_pid, &status, 0); // waiting for child one
            waitpid(c2_pid, &status, 0); // waiting for child two
            
            s3 = 0;
            // Sum of the numbers from 1 to the user inputted end of series (S1 + S2 = S3)
            for (int i = 1; i <= end_of_series; i++) {
                s3 += i;
            }
            printf("I am the parent process. Sum of Series 3: %d\n", s3); // parent prints out a message
        }
    }
    return 0;
}