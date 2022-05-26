#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define timesAgentWillPlace 10      // number of times the agent will place items on the table

sem_t agent_sem, smoker1_sem, smoker2_sem, smoker3_sem; // semaphores for all the threads

pthread_mutex_t mutex;      // mutex lock for the thread

void *agent_process() {
    int n = timesAgentWillPlace;

    for (int i = 0; i < n; i++) {

        printf("in agent process\n");

        pthread_mutex_lock(&mutex);     // locks the mutex to prevent any other thread from running

        int randNum = rand() % 3 + 1;   // rand num from 1-3 is generated

        if (randNum == 1) {
            // Agent puts the tobacco on the table
            // Agent puts the paper on the table
            printf("Agent places the tobacco and paper on the table.\n");
            sem_post(&smoker1_sem); // Wakes up smoker with the match
        }

        else if (randNum == 2) {
            // Agent puts the tobacco on the table
            // Agent puts the match on the table
            printf("Agent places the tobacco and match on the table.\n");
            sem_post(&smoker2_sem); // Wakes up smoker with the paper
        }

        else {
            // Agent puts the match on the table
            // Agent puts the paper on the table
            printf("Agent places the match and paper on the table.\n");
            sem_post(&smoker3_sem); // Wakes up smoker with the tobacco
        }
    
        pthread_mutex_unlock(&mutex);   // unlocks the mutex to allow another thread to run
        sem_wait(&agent_sem);           // agent sleeps
        
    }
    pthread_exit(0);        // thread exits
}

void *smoker1_process() {
    for(int i = 0; i < 4; i++) {
        // printf("SMOKER 1 SEMAPHORE BEFORE: %d\n", smoker1_sem);
        sem_wait(&smoker1_sem);         // smoker 1 sleeps
        // printf("IN SMOKER 1\n");
        pthread_mutex_lock(&mutex);     // locks the mutex to prevent any other thread from running
        // Smoker 1 picks up the tobacco
        // Smoker 1 picks up the paper
        printf("Smoker 1 with the match picks up the tobacco and paper.\n");
        // Smoke (but don't inhale)
        // printf("Smoker 1 smokes.\n");
        sem_post(&agent_sem);           // wakes the agent
        pthread_mutex_unlock(&mutex);   // unlocks the mutex to allow another thread to run
        // // Smoke (but don't inhale)
        printf("Smoker 1 smokes.\n");
    }
    pthread_exit(0);        // thread exits
}

void *smoker2_process() {
    for(int i = 0; i < 4; i++) {
        sem_wait(&smoker2_sem);         // smoker 2 sleeps
        // printf("IN SMOKER 2\n");
        pthread_mutex_lock(&mutex);     // locks the mutex to prevent any other thread from running
        // Smoker 2 picks up the match
        // Smoker 2 picks up the tobacco
        printf("Smoker 2 with the paper picks up the match and tobacco.\n");
        // Smoke (but don't inhale)
        // printf("Smoker 2 smokes.\n");
        sem_post(&agent_sem);           // wakes the agent
        pthread_mutex_unlock(&mutex);   // unlocks the mutex to allow another thread to run
        // // Smoke (but don't inhale)
        printf("Smoker 2 smokes.\n");
    }
    pthread_exit(0);        // thread exits
}

void *smoker3_process() {
    for(int i = 0; i < 4; i++) {
        sem_wait(&smoker3_sem);
        // printf("IN SMOKER 3\n");
        pthread_mutex_lock(&mutex);         // locks the mutex to prevent any other thread from running
        // Smoker 3 picks up the match
        // Smoker 3 picks up the paper
        printf("Smoker 3 with the tobacco picks up the match and paper.\n");
        // Smoke (but don't inhale)
        // printf("Smoker 3 smokes.\n");
        sem_post(&agent_sem);               // wakes the agent
        pthread_mutex_unlock(&mutex);       // unlocks the mutex to allow another thread to run
        // // Smoke (but don't inhale)
        printf("Smoker 3 smokes.\n");
    }
    pthread_exit(0);            // thread exits
}

int main() {

    // Pthreads for the agent and smokers
    pthread_t agent;
    pthread_t smoker1, smoker2, smoker3;

    // Intializes the semaphores for the agent and the smokers with an intial value of 0
    sem_init(&agent_sem, 0, 0);
    sem_init(&smoker1_sem, 0, 0);
    sem_init(&smoker2_sem, 0, 0);
    sem_init(&smoker3_sem, 0, 0);
    pthread_mutex_init(&mutex, NULL);   // intializes the mutex lock

    srand(time(NULL));

    // creates the pthreads for the agent
    pthread_create(&agent, NULL, agent_process, NULL);

    // creates the pthreads for the smokers
    pthread_create(&smoker1, NULL, smoker1_process, NULL);
    pthread_create(&smoker2, NULL, smoker2_process, NULL);
    pthread_create(&smoker3, NULL, smoker3_process, NULL);

    // agent is joined and the smoker threads are killed
    pthread_join(agent, NULL);

    pthread_kill(smoker1, 0);
    pthread_kill(smoker2, 0);
    pthread_kill(smoker3, 0);

    // pthread_join(smoker1, NULL);
    // pthread_join(smoker2, NULL);
    // pthread_join(smoker3, NULL);

    pthread_mutex_destroy(&mutex);

    return 0;
}