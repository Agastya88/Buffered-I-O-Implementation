/*This program tests code based on the
feedback given on 17th november.*/

#include "myio.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main (int argc, char* argv[]){

//TEST: what happens when I read for bytes > no of bytes in the file
//Number of bytes in file = 31
//TEST2: intermingling read & write calls

char *readLocation = malloc (12*sizeof(char));
char *readLocation2 = malloc (12*sizeof(char));
char *readLocation3 = malloc (12*sizeof(char));
char *readLocation4 = malloc (12*sizeof(char));
char *readLocationFRead = malloc (12*sizeof(char));
char *readLocationFRead2 = malloc (12*sizeof(char));
char *readLocationFRead3 = malloc (12*sizeof(char));
char *readLocationFRead4 = malloc (12*sizeof(char));

struct file *myFile= myopen ("myReadBufferOverflowTestFile", "r+");
printf ("myRead Returns: %ld\n", myread (readLocation, 12, myFile));
printf ("Data: %s\n", readLocation);
printf ("myWrite Returns: %ld\n", mywrite ("8734567512345", 13, myFile));
printf ("myRead2 Returns: %ld\n", myread (readLocation2, 12, myFile));
printf ("Data2: %s\n", readLocation2);
printf ("myWrite Returns: %ld\n", mywrite ("8734567512345", 13, myFile));
printf ("myRead3 Returns: %ld\n", myread (readLocation3, 12, myFile));
printf ("Data3: %s\n", readLocation3);
printf ("myWrite Returns: %ld\n", mywrite ("8734567512345", 13, myFile));
printf ("myRead4 Returns: %ld\n", myread (readLocation4, 12, myFile));
printf ("Data4: %s\n", readLocation4);
printf ("myWrite Returns: %ld\n", mywrite ("8734567512345", 13, myFile));
myclose (myFile);

printf ("------------_X----------------\n");

FILE *fFile = fopen ("fReadBufferOverflowTestFile", "r+");
printf ("fRead Returns: %ld\n", fread (readLocationFRead, 1, 12, fFile));
printf ("Data: %s\n", readLocationFRead);
printf ("fWrite Returns: %ld\n", fwrite ("8734567512345", 1, 13, fFile));
printf ("fRead2 Returns: %ld\n", fread (readLocationFRead2, 1, 12, fFile));
printf ("Data2: %s\n", readLocationFRead2);
printf ("fWrite Returns: %ld\n", fwrite ("8734567512345", 1, 13, fFile));
printf ("myRead3 Returns: %ld\n", fread (readLocationFRead3, 1, 12, fFile));
printf ("Data3: %s\n", readLocationFRead3);
printf ("fWrite Returns: %ld\n", fwrite ("8734567512345", 1, 13, fFile));
printf ("fRead4 Returns: %ld\n", fread (readLocationFRead4, 1, 12, fFile));
printf ("Data4: %s\n", readLocationFRead4);
printf ("fWrite Returns: %ld\n", fwrite ("8734567512345", 1, 13, fFile));

if (strcmp (readLocation, readLocationFRead) == 0
&& strcmp (readLocation2, readLocationFRead2) == 0
&& strcmp (readLocation3, readLocationFRead3) == 0
&& strcmp (readLocation4, readLocationFRead4) == 0){
    printf ("Works\n");
}

free (readLocation);
free (readLocation2);
free (readLocation3);
free (readLocation4);
free (readLocationFRead);
free (readLocationFRead2);
free (readLocationFRead3);
free (readLocationFRead4);
fclose (fFile);

}
