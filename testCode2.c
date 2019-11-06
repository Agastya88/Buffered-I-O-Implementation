#include "myio.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

//This program tests the functionality of myseek on a file that
//can be both read and written.

int main (int argc, char* argv[])
{

  struct fileStruct *fileOne = myopen ("workingFile2.txt", "r+");

  //reading 160 bytes of data
  char *readPtr = malloc (160*sizeof(char));
  size_t bytesReadInCallOne = myread (readPtr, 160,
       fileOne);
  printf ("myread callOne returns: %zu \n", bytesReadInCallOne);
  printf ("readPtr returns: %s\n", readPtr);

  int seekReturn1 = myseek (fileOne, 20, SEEK_CUR);
  if (seekReturn1 == -1)
  {
    printf ("myseek failed");
  }

  //reading 173 bytes of data
  char *readPtr2 = malloc (173*sizeof(char));
  size_t bytesReadInCallTwo = myread (readPtr2, 173,
       fileOne);
  printf ("myread callTwo returns: %zu \n", bytesReadInCallTwo);
  printf ("readPtr2 returns: %s\n", readPtr2);

  int seekReturn2 = myseek (fileOne, 1000, SEEK_CUR);
  if (seekReturn2 == -1)
  {
    printf ("myseek failed");
  }

  char *readPtr3 = malloc (13*sizeof(char));
  size_t bytesReadInCallThree = myread (readPtr3, 13,
       fileOne);
  printf ("myread callThree returns: %zu \n", bytesReadInCallThree);
  printf ("readPtr3 returns: %s\n", readPtr3);

  char *readPtr4 = malloc (13*sizeof(char));
  size_t bytesReadInCallFour = myread (readPtr4, 13,
       fileOne);
  printf ("myread callFour returns: %zu \n", bytesReadInCallFour);
  printf ("readPtr4 returns: %s\n", readPtr4);

  printf ("Position in read buffer at the end of all this: %d\n",
  fileOne -> positionInReadBuffer);

  int seekReturn3 = myseek (fileOne, -20, SEEK_CUR);
  if (seekReturn3 == -1)
  {
    printf ("myseek failed");
  }

  char *readPtr5 = malloc (10*sizeof(char));
  size_t bytesReadInCallFive = myread (readPtr5, 10,
       fileOne);
  printf ("myread callFive returns: %zu \n", bytesReadInCallFive);
  printf ("readPtr5 returns: %s\n", readPtr5);

  // prints result and leftover bytes;
  char *textToWrite = "This article is the intellectual property of Wired.com";
  size_t bytesWrittenInCallOne;
  bytesWrittenInCallOne = mywrite (textToWrite, strlen(textToWrite), fileOne);
  printf ("mywrite callOne returns: %zu \n", bytesWrittenInCallOne);
  printf ("fileOne->bytesInWriteBuffer is: %d\n", fileOne->bytesInWriteBuffer);

  int seekReturn4 = myseek (fileOne, -20, SEEK_CUR);
  if (seekReturn4 == -1)
  {
    printf ("myseek failed");
  }

  char *readPtr6 = malloc (10*sizeof(char));
  size_t bytesReadInCallSix = myread (readPtr6, 10,
       fileOne);
  printf ("myread callFix returns: %zu \n", bytesReadInCallSix);
  printf ("readPtr6 returns: %s\n", readPtr6);

  myclose (fileOne);

}
