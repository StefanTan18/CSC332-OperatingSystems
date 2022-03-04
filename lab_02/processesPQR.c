#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main() {

    // Intialization of Variables
    pid_t fq = 0, fr = 0;
    int a = 10, b = 25;

    fq = fork(); // Child Process Q is forked

    if (fq == 0) { // Child Process Q is forked successfully
        // Child Process Q Code
        a = a + b;
        printf("I am Process Q and my parent is Process P. a = %d  b = %d  pid = %d\n", a, b, getpid());

        fr = fork(); // Another child called Process R is forked

        if (fr != 0) {
            // This is also Child Process Q Code
            b = b + 20;
            printf("I am still Process Q and my parent is Process P. a = %d  b = %d  pid = %d\n", a, b, getpid());
        }

        else {
            // Child Process R Code
            a = (a * b) + 30;
            printf("I am Process R and my parent is Process Q. a = %d  b = %d  pid = %d\n", a, b, getpid());
        }
    }

    else {
        // parent Process P
        b = a + b - 5;
        printf("I am parent Process P. a = %d  b = %d  pid = %d\n", a, b, getpid());
    }
}