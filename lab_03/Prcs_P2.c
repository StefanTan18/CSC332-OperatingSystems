#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <string.h> 
#include <errno.h>
#include <unistd.h>

int main()
{
    int sfd, dfd1, dfd2; // File descriptor of source, destination1, and destination2 file
    
    errno = 0;
    sfd = open("source.txt", O_RDONLY); // Opens the source file in read mode
    
    if (-1 == sfd) // Error occured when opening file
    {
        if (errno == ENOENT) {
            printf ("source.txt does not exist\n"); // Source file given does not exist
            perror("open"); // Explanation of error is printed
        }
        else if (errno == EACCES) {
            printf ("source.txt is not accessible\n"); // Do not have read permissions for the source file
            perror("open"); // Explanation of error is printed
        }
        return 1;
    }
    else
    {
        int returnval;
        returnval = access ("destination1.txt", F_OK); // Checks if the file exists
        if (returnval == 0) {
            unlink("destination1.txt"); // Deletes the destination file if it already exists
        }

        returnval = access ("destination2.txt", F_OK); // Checks if the file exists
        if (returnval == 0) {
            unlink("destination2.txt"); // Deletes the destination file if it already exists
        }

        dfd1 = open("destination1.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); // Opens the destination1 file
        dfd2 = open("destination2.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); // Opens the destination2 file

        char buffer1[100];          // Buffer to hold the char for destination1 that is read from the source file
        char buffer2[50];           // Buffer to hold the char for destination2 that is read from the source file
        int charRead1, charRead2;   // Number of characters that is actually read (up to 100)
        int charToReadFlag = 0;     // Flag to help alternate between 100 char read and 50 char read

        do {
            // Read the next 100 chars and replace '1' with 'L' and write it to destination1.txt
            while(charToReadFlag == 0 && (charRead1 = read(sfd, buffer1, 100))) {
                for (int i = 0; i < charRead1; i++) { // Checks each character to see if the character is '1'
                    char c[1]; // char array to hold each character
                    c[0] = buffer1[i];
                    if (c[0] == '1') { // Character is found to be '1'
                        write(dfd1, "L", 1); // Replaces '1' with 'L'
                    }
                    else {
                        write(dfd1, c, 1); // Character is not '1' so nothing is replaced
                    }
                }
                charToReadFlag = 1; // Set the flag to 1 so it will read 50 chars next
            }

            // Read the next 50 chars and replace '3' with 'E' and write it to destination2.txt
            while(charToReadFlag == 1 && (charRead2 = read(sfd, buffer2, 50))) {
                for (int i = 0; i < charRead2; i++) { // Checks each character to see if the character is '1'
                    char c[1]; // char array to hold each character
                    c[0] = buffer2[i];
                    if (c[0] == '3') { // Character is found to be '3'
                        write(dfd2, "E", 1); // Replaces '3' with 'E'
                    }
                    else {
                        write(dfd2, c, 1); // Character is not '3' so nothing is replaced
                    }
                }
                charToReadFlag = 0; // Set the flag to 0 so it will read 100 chars next
            }
        } while (charRead1 && charRead2); // Reads until the end of the file

        close(sfd); // Closes the source file
        close(dfd1); // Closes the destination file
        close(dfd2); // Closes the destination file
    }
    return 0;
}