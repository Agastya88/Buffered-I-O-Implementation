#include "myio.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int main (int argc, char* argv[]){
    char *readLocation = malloc (500*sizeof(char));
    char *fReadLocation = malloc (500*sizeof(char));

    struct file *myFile= myopen ("testFileDecember2", "r+");
    printf ("myRead Returns: %ld\n", myread (readLocation, 500, myFile));
    myclose (myFile);

    FILE *fFile = fopen ("testFileDecember2", "r+");
    printf ("fRead Returns: %ld\n", fread (fReadLocation, 1, 500, fFile));
    fclose (fFile);

    if (strcmp(readLocation, fReadLocation)==0){
        printf ("works");
    }

    free (readLocation);
    free (fReadLocation);
}
