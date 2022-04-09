#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    
    FILE* fp;                   // pointer to file
    char* filepath = argv[1];   // file name

    int numStudents = 0;    // number of students (n)
    int numChapters = 0;    // number of chapters (x)
    int numHomeworks = 0;   // number of homeworks (y)
    
    if (argc != 2) // Incorrect usage of the program
    {
        printf("\n Usage : ./avgCalculator [filename]\n");
        return 1;
    }
    

    printf("Enter the number of students (n > 1): ");   // gets user input for the number of students
    scanf("%d", &numStudents);

    printf("Enter the number of chapters (x > 1): ");   // gets user input for the number of chapters
    scanf("%d", &numChapters);

    printf("Enter the number of homeworks (y >= 1): "); // gets user input for the number of homeworks
    scanf("%d", &numHomeworks);

    double grades[numStudents][numChapters * numHomeworks]; // 2D array to store the grades

    fp = fopen(filepath, "r"); // opens the file in read mode

    if (fp == NULL) {       // File does not exist
        return 1;
    }

    // Reads the text file input and stores the input read into the 2D array
    for (int i = 0; i < numStudents; i++) {
        for (int j = 0; j < numChapters * numHomeworks; j++) {
            fscanf(fp, "%lf ", &grades[i][j]);
        }
    }

    fclose(fp); // Closes the file

    // Prints out the 2D array contents
    // for (int i = 0; i < numStudents; i++) {
    //     for (int j = 0; j < numChapters * numHomeworks; j++) {
    //         printf("%lf ", grades[i][j]);
    //     }
    //     printf("\n");
    // }

    // Director Process
    // printf("\nDirector Process\n");

    for (int i = 0; i < numChapters; i++) {
        // Creating the Manager Processes
        if (fork() == 0) {
            // printf("Manager %d with pid: %d\n", i+1, getpid());
            for (int j = 0; j < numHomeworks; j++) {
                // Creating the Worker Processes
                if (fork() == 0) {
                    double total = 0;
                    double avg = 0;
                    // printf("i: %d  j: %d  col #: %d\n", i, j, i*numHomeworks+j);
                    for (int n = 0; n < numStudents; n++) {
                        total += grades[n][i*numHomeworks+j];   // calculates the total sum of the grades
                    }
                    avg = total / numStudents;  // the average is calculated
                    // printf("Worker %d for Manager %d with pid: %d\n", j+1, i+1, getpid());
                    printf("Average of X%dY%d: %lf\n", i+1, j+1, avg);      // prints out the averages
                    exit(0);
                }
            }
            // Waits for the Worker Processes
            for (int j = 0; j < numHomeworks; j++) {
                wait(NULL);
            }
            exit(0);
        }
    }
    // Waits for the Manager Processes
    for (int i = 0; i < numChapters; i++) {
        wait(NULL);
    }

    return 0;
}