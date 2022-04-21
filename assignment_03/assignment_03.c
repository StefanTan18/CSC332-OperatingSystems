#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// To use without the locks, comment out the lines involving the pthread_mutex

#define NUM_THREADS 5                               // number of threads that will be used for this program

char *dir[2] = {"North", "South"};                  // stores "North" and "South" to be used to print the output  
int numNorth = 0;                                   // number of north farmers
int numSouth = 0;                                   // number of south farmers

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // intializes the mutex

// function that will be ran when the farmer is on the bridge
void onBridge(char* direction, int number) {
    printf("%s Tunbridge #%d is traveling on the bridge...\n", direction, number); // farmer on the bridge
    int randNum = rand() % 4;                                                      // a random number from 0-3 is assigned
    // printf("Sleep Time: %d\n", randNum);
    sleep(randNum);                                                                // sleeps for this random number of sec
}

// North Tunbridge farmer
void *northFarmer(void *param) {
    pthread_mutex_lock(&mutex);                                                     // entering the critical section
    numNorth++;                                                                     // # of north farmer increases
    printf("%s Tunbridge #%d farmer can cross the bridge\n", dir[0], numNorth);     // farmer is entering the bridge
    onBridge(dir[0], numNorth);                                                     // farmer is on the bridge
    printf("%s Tunbridge #%d farmer has left the bridge\n\n", dir[0], numNorth);    // farmer left the bridge
    pthread_mutex_unlock(&mutex);                                                   // exiting the critical section
    pthread_exit(0);                                                                // thread is terminated
}

// South Tunbridge farmer
void *southFarmer(void *param) {
    pthread_mutex_lock(&mutex);                                                     // entering the critical section
    numSouth++;                                                                     // number of south farmer increases
    printf("%s Tunbridge #%d farmer can cross the bridge\n", dir[1], numSouth);     // farmer is entering the bridge
    onBridge(dir[1], numSouth);                                                     // farmer is on the bridge
    printf("%s Tunbridge #%d farmer has left the bridge\n\n", dir[1], numSouth);    // farmer left the bridge
    pthread_mutex_unlock(&mutex);                                                   // exiting the critical section
    pthread_exit(0);                                                                // thread is terminated
}

int main() {

    pthread_t north[NUM_THREADS];       // identfiers of the North Tunbridge farmers
    pthread_t south[NUM_THREADS];       // identifiers of the South Tunbridge farmers
    pthread_attr_t attr;                // attributes of the thread

    pthread_attr_init(&attr);           // intializes the thread with the default attribute values

    pthread_mutex_init(&mutex, NULL);   // intializes the mutex lock

    srand(time(NULL));                  // sets the starting point for producing a series of random integers

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&north[i], NULL, northFarmer, NULL);     // creates the thread for north farmers
        pthread_create(&south[i], NULL, southFarmer, NULL);     // creates the thread for south farmers
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(north[i], NULL);                           // joins the threads of the north farmers
        pthread_join(south[i], NULL);                           // joins the threads of the south farmers
    }

    pthread_mutex_destroy(&mutex);                              // destroys the mutex as it is no longer needed

    return 0;
}