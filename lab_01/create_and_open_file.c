#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <string.h> 
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd; // File descriptor
    char* filepath = argv[1]; // File name
    
    if (2 != argc) // Incorrect usage of the program
    {
        printf("\n Usage : ./create_and_open_file [filename]\n");
        return 1;
    }
    errno = 0;
    fd = open(filepath, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); // Created and opened file with -rw-r--r—- permissions
    if (-1 == fd) // Error occured when opening file
    {
        printf("\n open() failed with error [%s]\n",strerror(errno));
        return 1;
    }
    else
    {
        printf("\n Creation of %s with Open() Successful\n", filepath); // Was able to open and create the file successfully
        close(fd); // Closes the file
    }
    return 0;
}