#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define NUM_THREADS 1                       // number of threads that will be used for this program

int rangePerThread = 10000 / NUM_THREADS;   // attempts to divide the integers between each thread evenly
int maxDivisorCount = 0;                    // keeps track of the number of divisors of the integer with the most divisors 
int intWithLargestCount;                    // keeps track of the integer with the largest number of divisors

void *numOfDivisors(void *arg);             // function prototype for the numOfDivisors function

// Struct used to pass into the arguments of the function numOfDivisors() that will be ran in the threads
struct range_struct {
    int begin;
    int end;
};

int main() {

    struct timeval start, end;
    gettimeofday(&start, NULL);     // time at the start

    pthread_t workers[NUM_THREADS]; // identifiers of the thread
    pthread_attr_t attr;            // attributes of the thread

    pthread_attr_init(&attr);       // intializes the thread with the default attribute values

    int leftover = 10000 % NUM_THREADS;         // checks to see if all the integers have been split between the threads
    int init = 1;                               // the range of the integers to check starts from 1
    struct range_struct ranges[NUM_THREADS];    // intializes an array of the structs to be used as the parameter

    // Creates the threads and passes its respective parameter to be used for the numOfDivisors() function
    for (int i = 0; i < NUM_THREADS; i++) {
        ranges[i].begin = init + (i * rangePerThread);      // assign the corresponding begin value for the struct
        ranges[i].end = ranges[i].begin + rangePerThread;   // assign the corresponding end value for the struct
        // Any leftover integers will be added to the final thread created
        if (leftover && i == NUM_THREADS - 1) {
            ranges[i].end += leftover;
        }
        pthread_create(&workers[i], &attr, numOfDivisors, &ranges[i]); // thread is created and arguments are passed
    }

    // Waits for all the threads that were created to exit
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(workers[i], NULL);
    }

    gettimeofday(&end, NULL);     // time at the end
    double calc_time = end.tv_sec + end.tv_usec / 1e6 - start.tv_sec - start.tv_usec / 1e6; // elapsed time is calculated (sec)

    // Integer in the range of 1-10000 with the largest number of divisors along with calculation time are printed
    printf("Integer with the Largest Number of Divisors: %d\n", intWithLargestCount);
    printf("Time in Seconds Spent to Calculate: %f\n", calc_time);
    
    //printf("COUNT: %d\n", maxDivisorCount);

    return 0;
}

// Function for the read that will calculate the integer with the largest number of divisors within the given range
void *numOfDivisors(void *arg) {

    struct range_struct *arg_struct = (struct range_struct*) arg;
    int start = arg_struct->begin;  // Sets the beginning of the range
    int end = arg_struct->end;      // Sets the end of the range
    
    //printf("Start: %d   End: %d     Range: %d\n", start, end, rangePerThread);

    // Iterates from 1 to the current integer to check for the possible divisors of that number
    for (int i = start; i < end; i++) {
        int count = 0;                      // count is intialized at 0
        for (int j = 1; j <= i; j++) {
            // If a divisor is found then the count gets incremented
            if (i % j == 0) {
                count++;
            }
            // If the number of divisors of this integer exceed the current max number of divisors then replace it
            if (count > maxDivisorCount) {
                intWithLargestCount = i;
                maxDivisorCount = count;
            }
        }
    }

    pthread_exit(0);    // Thread is terminated
}