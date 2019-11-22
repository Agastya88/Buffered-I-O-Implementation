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

char *readLocation = malloc (11*sizeof(char));
char *readLocation2 = malloc (8*sizeof(char));
char *readLocation3 = malloc (3*sizeof(char));
char *readLocationFRead = malloc (11*sizeof(char));
char *readLocationFRead2 = malloc (8*sizeof(char));
char *readLocationFRead3 = malloc (3*sizeof(char));

struct file *myFile= myopen ("myReadBufferOverflowTestFile", "r+");
printf ("myRead Returns: %ld\n", myread (readLocation, 11, myFile));
printf ("Data: %s\n", readLocation);
printf ("myWrite Returns: %ld\n", mywrite ("873", 3, myFile));
printf ("myRead2 Returns: %ld\n", myread (readLocation2, 8, myFile));
printf ("Data2: %s\n", readLocation2);
printf ("myWrite Returns: %ld\n", mywrite ("My", 2, myFile));
printf ("myRead3 Returns: %ld\n", myread (readLocation3, 3, myFile));
printf ("Data3: %s\n", readLocation3);
free (readLocation);
free (readLocation2);
free (readLocation3);
myclose (myFile);

printf ("------------_X----------------\n");

FILE *fFile = fopen ("fReadBufferOverflowTestFile", "r+");
printf ("fRead Returns: %ld\n", fread (readLocationFRead, 1, 11, fFile));
printf ("Data: %s\n", readLocationFRead);
printf ("fWrite Returns: %ld\n", fwrite ("873", 1, 3, fFile));
printf ("fRead2 Returns: %ld\n", fread (readLocationFRead2, 1, 8, fFile));
printf ("Data2: %s\n", readLocationFRead2);
printf ("myWrite Returns: %ld\n", fwrite ("My", 1, 2, fFile));
printf ("myRead3 Returns: %ld\n", fread (readLocationFRead3, 1, 3, fFile));
printf ("Data3: %s\n", readLocation3);
free (readLocationFRead);
free (readLocationFRead2);
free (readLocationFRead3);
fclose (fFile);

}
