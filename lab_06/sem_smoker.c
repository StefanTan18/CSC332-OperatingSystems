#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "sem.h"

#define CHILD 0

#define timesAgentWillPlace 10      // defines the times an agent will place the items

int main() {

    pid_t pid1, pid2, pid3, pid4;   // pids for all the processes
    int status;
    int timesPlaced = 0;            // numbers of times the agent had placed the items

    srand(time(NULL));

    // Semaphores are created
    int lock = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    int smoker_match = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    int smoker_paper = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    int smoker_tobacco = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    int agent = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);

    sem_create(lock, 1);
    sem_create(smoker_match, 0);
    sem_create(smoker_paper, 0);
    sem_create(smoker_tobacco, 0);
    sem_create(agent, 0);

    // Fork Failed
    if ((pid1 = fork()) == -1) {
        perror("fork");
        return 1;
    }

    if (pid1 == CHILD) {
        // First Child Process (Agent)
        // printf("Agent's Pid: %d\n", getpid());
        int n = timesAgentWillPlace;
        for (int i = 0; i < n; i++) {
            timesPlaced++;  // times placed gets incremented
            printf("\nRound %d\n", timesPlaced);
            P(lock);
            int randNum = rand() % 3 + 1;

            if (randNum == 1) {
                // Agent puts the tobacco on the table
                // Agent puts the paper on the table
                printf("Agent places the tobacco and paper on the table.\n");
                V(smoker_match);    // Wakes up smoker with the match
            }

            else if (randNum == 2) {
                // Agent puts the tobacco on the table
                // Agent puts the match on the table
                printf("Agent places the tobacco and match on the table.\n");
                V(smoker_paper);    // Wakes up smoker with the paper
            }

            else {
                // Agent puts the match on the table
                // Agent puts the paper on the table
                printf("Agent places the match and paper on the table.\n");
                V(smoker_tobacco);  // Wakes up smoker with the tobacco
            }
            V(lock);
            P(agent);   // Agent sleeps
        }
    }

    else {
        // Fork Failed
        if ((pid2 = fork()) == -1) {
            perror("fork");
            return 1;
        }

        if (pid2 == CHILD) {
            // Second Child Process (Smoker 1 with the match)
            while (1) {
                P(smoker_match);  // Smoker 1 sleeps right away
                P(lock);
                // Smoker 1 picks up the tobacco
                // Smoker 1 picks up the paper
                printf("Smoker 1 with the match picks up the tobacco and paper.\n");
                V(agent);
                V(lock);
                // Smoke (but don't inhale)
                printf("Smoker 1 smokes.\n");
            }
        }

        else {
            // Fork Failed
            if ((pid3 = fork()) == -1) {
                perror("fork");
                return 1;
            }

            if (pid3 == CHILD) {
                // Third Child Process (Smoker 2 with the paper)
                while (1) {
                    P(smoker_paper);  // Smoker 2 sleeps right away
                    P(lock);
                    // Smoker 2 picks up the match
                    // Smoker 2 picks up the tobacco
                    printf("Smoker 2 with the paper picks up the match and tobacco.\n");
                    V(agent);
                    V(lock);
                    // Smoke (but don't inhale)
                    printf("Smoker 2 smokes.\n");
                }
            }

            else {
                // Fork Failed
                if ((pid4 = fork()) == -1) {
                    perror("fork");
                    return 1;
                }

                if (pid4 == CHILD) {
                    // Fourth Child Process (Smoker 3 with the tobacco)
                    while (1) {
                        P(smoker_tobacco);  // Smoker 3 sleeps right away
                        P(lock);
                        // Smoker 3 picks up the match
                        // Smoker 3 picks up the paper
                        printf("Smoker 3 with the tobacco picks up the match and paper.\n");
                        V(agent);
                        V(lock);
                        // Smoke (but don't inhale)
                        printf("Smoker 3 smokes.\n");
                    }
                }

                else {
                    pid1 = wait(&status);
              	    // printf("child(pid = %d) exited with the status %d. \n", pid1, status);

                    // Added this as the program will be stuck waiting for the smoker processes to end
                    kill(pid2, SIGTERM);
                    kill(pid3, SIGTERM);
                    kill(pid4, SIGTERM);

                }
                return 0;
            }
            return 0;
        }
        return 0;
    }
    return 0;
}