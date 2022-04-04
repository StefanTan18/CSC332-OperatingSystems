// C program to find maximum number of thread within
// a process
#include<stdio.h>
#include<pthread.h>

// This function demonstrates the work of thread
// which is of no use here, So left blank
void *create ( void * args){
}

int main()
{
	int max = 0, ret = 0;
    pthread_t th;
    // Iterate until 0 is returned
    while (ret == 0) {
        ret = pthread_create(&th, NULL, create, NULL);

        max++;
    }
    printf(" %d ", max);
}