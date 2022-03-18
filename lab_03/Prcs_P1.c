#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <string.h> 
#include <errno.h>
#include <unistd.h>

int main()
{
    int fd1, fd2; // File descriptors for destination1.txt and destination2.txt
    
    errno = 0;
    // Create destination1.txt with read, write, and execute permissions
    fd1 = open("destination1.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH);
    if (fd1 == -1) // Error occured when opening file
    {
        printf("\n open() failed with error [%s]\n",strerror(errno));
        return 1;
    }
    else
    {
        // Was able to open and create the destination2.txt successfully
        // printf("Created destination1.txt with read, write, and execute permissions.\n"); // Was able to open and create the file successfully
        close(fd1); // Closes destination1.txt
    }

    // For destination2.txt
    errno = 0;
    // Create destination2.txt with read, write, and execute permissions
    fd2 = open("destination2.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH);
    if (fd2 == -1) // Error occured when opening file
    {
        printf("\n open() failed with error [%s]\n",strerror(errno));
        return 1;
    }
    else
    {
        // Was able to open and create the destination2.txt successfully
        // printf("Created destination2.txt with read, write, and execute permissions.\n");
        close(fd2); // Closes destination2.txt
    }
    return 0;
}