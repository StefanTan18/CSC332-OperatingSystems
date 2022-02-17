#include <stdio.h> 
#include <unistd.h> 
#include <errno.h>

int main (int argc, char* argv[])
{
    char* filepath = argv[1]; // File name
    int returnval;
    
    // Check file existence
    returnval = access (filepath, F_OK);
    if (returnval == 0)
        printf ("\n %s exists\n", filepath); // File does exist
    else
    {
        if (errno == ENOENT)
            printf ("%s does not exist\n", filepath); // File does not exist message is printed
        else if (errno == EACCES)
            printf ("%s is not accessible\n", filepath); // File is not accessible is printed
        return 0;
    }

    // Check read access ...
    returnval = access(filepath, R_OK);
    if (returnval == 0) {
        printf("%s has read permission\n", filepath); // File is determined to have read permission
    }
    else {
        printf("%s does not have read permission\n", filepath); // File is determined to not have read permission
    }

    // Check write access ...
    returnval = access(filepath, W_OK);
    if (returnval == 0) {
        printf("%s has write permission\n", filepath); // File is determined to have write permission
    }
    else {
        printf("%s does not have write permission\n", filepath); // File is determined to not have write permission
    }

    return 0;
}