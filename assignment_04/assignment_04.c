#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_CYLINDERS 10000     // number of cylinders that will be serviced
#define NUM_REQUESTS 1000       // number of cylinders requests

// Sorts the given array in ascending order
int* sortarr(int *arr) {
    for (int i = 0; i < NUM_REQUESTS - 1; i++) {
        for (int j = 0; j < NUM_REQUESTS - i - 1; j++) {
            // Swaps if the array value is greater than its neighbor on the right
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    return arr;
}

// FCFS (First Come First Serve) Disk-Scheduling Algorithm
int fcfs(int head, int *requests) {
    int head_mov = 0;       // total amount of head movement
    int curr;               // current request that is being handled


    // Handles the requests in the order of the requests
    for (int i = 0; i < NUM_REQUESTS; i++) {
        curr = requests[i];             // updates the current request that is being handled
        head_mov += abs(curr - head);   // calculates the distance between the requests and adds it to the head movement
        head = curr;                    // updates the position of the disk head
    }

    return head_mov;    // returns the total amount of head movement
}

// SCAN (Elevator) Disk-Scheduling Algorithm
int scan(int head, int *requests) {
    int head_mov = 0;           // total amount of head movement
    int curr, init_index;       // current request that is being handled, index of the intital position of the disk head

    // Locates the index of the intital position of the disk head within the sorted requests
    for (int i = 0; i < NUM_REQUESTS; i++) {
        // If the request is greater than the initial position of the head then set the index to be i-1
        if (requests[i] > head) {
            init_index = i - 1;
            break;
        }
    }

    // First handles the requests to the left of the initial position of the disk head
    for (int i = init_index; i >= 0; i--) {
        curr = requests[i];             // updates the current request that is being handled
        head_mov += abs(curr - head);   // calculates the distance between the requests and adds it to the head movement
        head = curr;                    // updates the position of the disk head
    }

    // Once it reaches the end it then handles the requests to the right
    for (int i = init_index + 1; i < NUM_REQUESTS; i++) {
        curr = requests[i];             // updates the current request that is being handled
        head_mov += abs(curr - head);   // calculates the distance between the requests and adds it to the head movement
        head = curr;                    // updates the position of the disk head
    }

    return head_mov;    // returns the total amount of head movement
}

// C-SCAN Disk-Scheduling Algorithm
int cscan(int head, int *requests) {
    int head_mov = 0;       // total amount of head movement
    int curr, init_index;   // current request that is being handled, index of the intital position of the disk head

    // Locates the index of the intital position of the disk head within the sorted requests
    for (int i = 0; i < NUM_REQUESTS; i++) {
        // If the request is greater than the initial position of the head then set the index to be i
        if (requests[i] > head) {
            init_index = i;
            break;
        }
    }

    // First handles the requests towards the end of the disk
    for (int i = init_index; i < NUM_REQUESTS; i++) {
        curr = requests[i];             // updates the current request that is being handled
        head_mov += abs(curr - head);   // calculates the distance between the requests and adds it to the head movment
        head = curr;                    // updates the position of the disk head
    }

    head_mov += (NUM_CYLINDERS - 1) - head;     // adds the remaining distance from the last request to the end of the disk
    head_mov += NUM_CYLINDERS - 1;              // adds the distance from one end of the disk to the other end of the disk
    head = 0;                                   // set the position of the disk head to the other end of the disk

    for (int i = 0; i < init_index; i++) {
        curr = requests[i];             // updates the current request that is being handled
        head_mov += abs(curr - head);   // calculates the distance between the requests and adds it to the head movement
        head = curr;                    // updates the position of the disk head
    }

    return head_mov;    // returns the total amount of head movement
}

int main(int argc, char *argv[]) {

    // Stops the program if no input of the initial position of the disk head is given
    if (argc != 2) {
        printf("Incorrect number of arguments. Please run the program with the intial position (0 - 9999) of the disk head.\n");
        return 1;
    }

    int initial = atoi(argv[1]);    // gets the initial position of the disk head

    int rand_series[NUM_REQUESTS];  // holds the random series of 1000 cylinders requests

    srand(time(NULL));              // sets the starting point for producing a series of random integers

    // Generates a random series of 1000 cylinder requests numbered between 0 to 9000
    for (int i = 0; i < NUM_REQUESTS; i++) {
        rand_series[i] = rand() % 10000;
    }

    // Prints the total amoount of head movement of each required algorithm in a table
    printf("┌────────────────────────────────┬──────────────────────────────┐\n");
    printf("│   Disk-Scheduling Algorithms   │   Amount of Head Movements   │\n");
    printf("├────────────────────────────────┼──────────────────────────────┤\n");
    printf("│             FCFS               │            %d           │\n", fcfs(initial, rand_series));
    printf("├────────────────────────────────┼──────────────────────────────┤\n");
    // Sorts the generated random series for the SCAN and C-SCAN algorithm
    sortarr(rand_series);
    printf("│             SCAN               │             %d            │\n", scan(initial, rand_series));
    printf("├────────────────────────────────┼──────────────────────────────┤\n");
    printf("│            C-SCAN              │             %d            │\n", cscan(initial, rand_series));
    printf("└────────────────────────────────┴──────────────────────────────┘\n");

    return 0;

}