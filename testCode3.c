/*This program tests code based on the
feedback given on 17th november.*/

#include "myio.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main (int argc, char* argv[])
{

/*testing: myread() produces incorrect results (both
singly and in sequence).*/

//singly
char *writeTextSingly = "Hello World!";
//writing using mywrite
struct file *mywriteFile_Singly = myopen ("testFileMyWrite", "w+");
mywrite (writeTextSingly, strlen(writeTextSingly), mywriteFile_Singly);
myclose(mywriteFile_Singly);
//writing using fwrite
FILE *fwritefile_Singly = fopen ("testFileFWrite", "w+");
fwrite(writeTextSingly, 1, strlen(writeTextSingly), fwritefile_Singly);
fclose (fwritefile_Singly);

//in sequence
char *writeTextSequenceOne = "Hello World! ";
char *writeTextSequenceTwo = "How are you today?";
//writing using mywrite
struct file *mywriteFile_Sequential = myopen ("testFileMyWriteSeq", "w+");
mywrite (writeTextSequenceOne, strlen(writeTextSequenceOne), mywriteFile_Sequential);
mywrite (writeTextSequenceTwo, strlen(writeTextSequenceTwo), mywriteFile_Sequential);
myclose (mywriteFile_Sequential);
FILE *fwritefile_Sequential = fopen ("testFileFWriteSeq", "w+");
fwrite(writeTextSequenceOne, 1, strlen(writeTextSequenceOne), fwritefile_Sequential);
fwrite(writeTextSequenceTwo, 1, strlen(writeTextSequenceTwo), fwritefile_Sequential);
fclose (fwritefile_Sequential);

/*testing: mywrite() produces incorrect results when
requesting more than BUFFER_SIZE bytes.*/

/*testing: sequence of mywrite() calls produces incorrect results.*/

/*testing: intermingled reads and writes produces incorrect results.*/
}
