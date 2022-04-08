#include <unistd.h>
#include <stdio.h>

// Example with exec() commands (NOT PART OF LAB 3)
int main(int argc, char* argv[]) {
    execv("/bin/echo", &argv[0]);
    printf("EXECV Failed\n");

    // execlp("echo", "/bin/echo", "Hello World", NULL);

    return 0;
}