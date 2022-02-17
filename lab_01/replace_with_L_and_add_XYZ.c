#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <string.h> 
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int sfd, dfd; // File descriptor of source and destination file
    char* source = argv[1]; // Source file name
    char* destination = argv[2]; // Destination file name
    
    if (3 != argc) // Incorrect usage of the program
    {
        printf("\n Usage : ./replace_with_L_and_add_XYZ [SourceFile] [DestinationFile]\n");
        return 1;
    }
    errno = 0;
    sfd = open(source, O_RDONLY); // Opens the source file in read mode
    
    if (-1 == sfd) // Error occured when opening file
    {
        if (errno == ENOENT) {
            printf ("%s does not exist\n", source); // Source file given does not exist
            perror("open"); // Explanation of error is printed
        }
        else if (errno == EACCES) {
            printf ("%s is not accessible\n", source); // Do not have read permissions for the source file
            perror("open"); // Explanation of error is printed
        }
        return 1;
    }
    else
    {
        int returnval;
        returnval = access (destination, F_OK); // Checks if the file exists
        if (returnval == 0) {
            unlink(destination); // Deletes the destination file if it already exists
        }

        dfd = open(destination, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); // Opens the destination file

        char buffer[100]; // Buffer to hold the char that is read from the source file
        int charRead; // Number of characters that is actually read (up to 100)
        while((charRead = read(sfd, buffer, 100))) {
            for (int i = 0; i < charRead; i++) { // Checks each character to see if the character is '1'
                char c[1]; // char array to hold each character
                c[0] = buffer[i];
                if (c[0] == '1') { // Character is found to be '1'
                    write(dfd, "L", 1); // Replaces '1' with 'L'
                }
                else {
                    write(dfd, c, 1); // Character is not '1' so nothing is replaced
                }
            }
            write(dfd, "XYZ", 3); // Adds the characters "XYZ" to the destination file after writing 100 characters 
        }
        close(sfd); // Closes the source file
        close(dfd); // Closes the destination file
    }
    return 0;
}