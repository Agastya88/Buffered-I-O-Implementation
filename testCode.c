#include "myio.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

//This program tests the functionality of read and write individually.

int main(int argc, char* argv[])
{
    printf ("\n");
    printf ("------------WRITE TESTING BEGINS HERE-------------\n");
    struct file *fileOne = myopen ("workingFile.txt", "w");
    //first text being written  using mywrite;
    // prints result and leftover bytes;
    char *textToWrite = "Some random words are ";
    size_t bytesWrittenInCallOne;
    bytesWrittenInCallOne = mywrite (textToWrite, strlen(textToWrite), fileOne);
    printf ("mywrite: %zu \n", bytesWrittenInCallOne);
    //another text being written  using mywrite;
    //prints result and leftover bytes;
    char *textToWriteTwo = "being written using ";
    size_t bytesWrittenInCallTwo;
    bytesWrittenInCallTwo = mywrite (textToWriteTwo, strlen(textToWriteTwo), fileOne);
    printf ("mywrite callTwo returns: %zu \n", bytesWrittenInCallTwo);
    //testing works till here; now I'm gonna try and throw it over 4096
    //huge third text being written  using mywrite;
    //prints result and leftover bytes;
    char *textToWriteThree = "my mywrite function";
    size_t bytesWrittenInCallThree;
    bytesWrittenInCallThree = mywrite (textToWriteThree, strlen(textToWriteThree), fileOne);
    printf ("mywrite callThree returns: %zu \n", bytesWrittenInCallThree);
    FILE *fileOneTester = fopen ("workingFile3.txt", "w");
    printf ("fwrite returns: %zu \n", fwrite(textToWrite, 1, strlen(textToWrite), fileOneTester));
    printf ("fwrite callTwo returns: %zu \n", fwrite(textToWriteTwo, 1, strlen(textToWriteTwo), fileOneTester));
    printf ("fwrite callThree returns: %zu \n", fwrite(textToWriteThree, 1, strlen(textToWriteThree), fileOneTester));
    int closeResult = myclose (fileOne);
    if (closeResult == -1){
        printf("Error Closing File.  Description:  %s\n", strerror(errno));
    }
    else if (closeResult == 0){
        printf ("File Closed Successfully.\n");
    }
    fclose (fileOneTester);

    printf ("------------WRITE TESTING ENDS HERE-------------\n");
    printf ("\n");
    printf ("------------READ TESTING BEGINS HERE-------------\n");
    //TEST: call myread on the file I just wrote to see if I can read it
    struct file *fileTwo = myopen ("workingFile.txt", "r");
    char *readPtr = malloc (17*sizeof(char));
    char *readPtr2 = malloc (40*sizeof(char));
    size_t bytesReadInCallOne = myread (readPtr, 17,
         fileTwo);
    printf ("myread callOne returns: %zu \n", bytesReadInCallOne);
    printf ("readPtr returns: %s\n", readPtr);
    free (readPtr);
    size_t bytesReadInCallTwo = myread (readPtr2, 40,
         fileTwo);
    printf ("myread callTwo returns: %zu \n", bytesReadInCallTwo);
    printf ("readPtr2 returns: %s\n", readPtr2);
    free (readPtr2);

    FILE *fileTwoTester = fopen ("workingFile3.txt", "r");
    char *readPtrTest = malloc (17*sizeof(char));
    char *readPtr2Test = malloc (40*sizeof(char));
    size_t bytesReadInCallOneTest = fread (readPtrTest,1, 17,
         fileTwoTester);
    printf ("fread callOne returns: %zu \n", bytesReadInCallOneTest);
    printf ("readPtr returns: %s\n", readPtrTest);
    free (readPtrTest);
    size_t bytesReadInCallTwoTest = fread (readPtr2Test, 1, 40,
         fileTwoTester);
    printf ("fread callTwo returns: %zu \n", bytesReadInCallTwoTest);
    printf ("readPtr2 returns: %s\n", readPtr2Test);

    int closeResultTwo = myclose (fileTwo);
    if (closeResultTwo == -1){
        printf("Error Closing File.  Description:  %s\n", strerror(errno));
    }
    else if (closeResultTwo == 0){
        printf ("File Closed Successfully.\n");
    }
    free (fileTwo);
    //close file once I'm done reading
    //free memory when done with all operations on the file
    printf ("------------READ TESTING ENDS HERE-------------\n");
    printf ("\n");
}
