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
        printf("\n Usage : ./displaycontent [filename]\n");
        return 1;
    }
    errno = 0;
    fd = open(filepath, O_RDONLY); // Opens the file in read mode
    
    if (-1 == fd) // Error occured when opening file
    {
        if (errno == ENOENT) {
            printf ("%s does not exist\n", filepath); // File given does not exist
            perror("open"); // Explanation of error is printed
        }
        else if (errno == EACCES) {
            printf ("%s is not accessible\n", filepath); // Do not have read permissions for the file
            perror("open"); // Explanation of error is printed
        }
        return 1;
    }
    else
    {
        char buffer[1]; // Buffer to hold the char that is read from the file
        int outFD = 1; // File descriptor of stdout is 1
        while(read(fd, buffer, 1)) {
            write(outFD, buffer, 1); // Writes the buffer that is read to stdout
        }
        close(fd); // Closes the file
    }
    return 0;
}