#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int myopen (const char *pathname, const char *mode);
int myclose (int fD);
size_t myread (void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t mywrite(int fD, const char *buf, size_t count);


#define BUFFER_SIZE 4096;
size_t bytesInBuffer = 0;
int totalNumberOfMyWriteCalls = 0;

int main (int argc, char* argv[])
{

    char *textToWrite = malloc (4400);
    char *textToWrite2 = malloc (4300);

    for (int i=0;i<4400;i++)
    {
        *(textToWrite+i) = 'a';
    }

    for (int i=0;i<4300;i++)
    {
        *(textToWrite2+i) = 'b';
    }

    //need to change the data being written
    //to like some sort of story or something
    //'a's can be super misleading

    size_t bytesWritten;

    int fD = myopen ("randomFile.txt", "w");
    bytesWritten = mywrite (fD, textToWrite, strlen(textToWrite));
    printf ("The number of bytes written by textToWrite are: %zu \n", bytesWritten);
    printf ("Leftover bytes in buffer: %zu\n", bytesInBuffer); //for debugging
    bytesWritten = mywrite (fD, textToWrite2, strlen(textToWrite2));
    printf ("The number of bytes written by textToWrite2 are: %zu \n", bytesWritten);
    printf ("Leftover bytes in buffer: %zu\n", bytesInBuffer); //for debugging

    int closeResult = myclose (fD);
    if (closeResult == 0)
    {
        printf ("File closed successfully. \n");
    }

}

int myopen (const char *pathname, const char *mode)
{
  //makes use of the system call "open" to allow
  //for the opening of files according to their flags

  int fileDesc;

  if (strcmp (mode, "r") == 0)
  {
      fileDesc = open (pathname, O_RDONLY);
  }
  else if (strcmp (mode, "w") == 0)
  {
      fileDesc = open (pathname, O_WRONLY);
  }
  else if (strcmp (mode, "rw") == 0)
  {
      fileDesc = open (pathname, O_RDWR);
  }

  return fileDesc;

  //still need to figure out how to implement
  //the file creation flags specified - used in
  //exceptional situations like when there is an error

  //so the purpose of these is to make an environment in which
  //read and write can work
}

int myclose (int fD)
{
    if (bytesInBuffer!=0)
    {
        if (bytesInBuffer<=4096)
        {
            //int currentBitInBuffer = totalNumberOfMyWriteCalls*4096;
            size_t closingBytesWritten =
            write (fD, "bs"/*need to figure out how
            to keep track of buf*/ ,bytesInBuffer);
                printf ("Bytes written on closing: %zu\n", closingBytesWritten);
        }
    }

    return close (fD);
}

size_t mywrite(int writeableFileDesc, const char *buf, size_t count)
{
    //note to self: buf is a pointer to a buffer of at least
    //'count' number of bytes

    //creates a buffer so that the sys call write is only
    //called when "enough bytes" = 4096 or x (if x i.e. total
    //is less than 4096.

    size_t noOfBytesWritten;
    int numberOfCalls = 0;

    bytesInBuffer += count;

    while (bytesInBuffer>=4096)
    {
        noOfBytesWritten = write (writeableFileDesc, buf, 4096);
        bytesInBuffer = bytesInBuffer - 4096;
        numberOfCalls ++;
    }

    /*need to actually keep track of the data being written*/

    totalNumberOfMyWriteCalls += numberOfCalls;

    //need to call the above statement correctly i.e.
    //in a way that will count the bytes first and then make the system
    //call only once 4096 bytes have asked to be written or when
    //the all the bytes have been asked and the total number of bytes
    //is less than 4096

    return numberOfCalls*noOfBytesWritten; // NOTE: this isn't exactly
    // what fwrite returns but I've set it like this right now so I
    // can see what's happening
}

size_t myread (void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    //creates a buffer so that the sys call read is only
    //called when it's worth it - similar to mywrite
    return 0;
}

//other two functions myseek and myflush to be added still
