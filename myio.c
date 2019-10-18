#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

//NOTES: no globals; dynamic allocation in the open function.

struct fileStruct *myopen (const char *pathname, const char *mode);
int myclose (struct fileStruct *stream);
size_t myread (void *ptr, size_t size, size_t nmemb, struct fileStruct *stream);
size_t mywrite (const char *buf, size_t size, struct fileStruct *stream);

#define BUFFER_SIZE 4096

struct fileStruct
{
    int fD;
    char textBuffer [BUFFER_SIZE];
    int bytesInBuffer; //number of actually occupied slots
};

int main (int argc, char* argv[])
{

    struct fileStruct *fileOne = myopen ("randomFile.txt", "w");

    char *textToWrite = "insert some long text here";

    size_t bytesWritten;
    bytesWritten = mywrite (textToWrite, strlen(textToWrite), fileOne);

    printf ("mywrite (textToWrite, strlen(textToWrite), fileOne) returns: %zu \n", bytesWritten);

    printf ("fileOne->bytesInBuffer is: %d\n", fileOne->bytesInBuffer); //for debugging

    int closeResult = myclose (fileOne);
    if (closeResult == 0)
    {
        printf ("File closed successfully. \n");
    }

}

struct fileStruct *myopen (const char *pathname, const char *mode)
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

  struct fileStruct *fileOpened = malloc (sizeof(struct fileStruct));
  fileOpened -> fD = fileDesc;
  fileOpened -> bytesInBuffer = 0;
  return fileOpened;

  //still need to figure out how to implement
  //the file creation flags specified - used in
  //exceptional situations like when there is an error

  //so the purpose of these is to make an environment in which
  //read and write can work
}

int myclose (struct fileStruct *stream)
{
    if (stream->bytesInBuffer!=0)
    {
        if (stream->bytesInBuffer<=4096)
        {
            size_t closingBytesWritten =
            write (stream->fD, stream->textBuffer ,stream->bytesInBuffer);
                printf ("Bytes written on closing: %zu\n",
                 closingBytesWritten);
        }
    }

    free (stream);

    return close (stream->fD);
}

size_t mywrite (const char *buf, size_t size, struct fileStruct* stream)
{
    //note to self: buf is a pointer to a buffer of at least
    //'count' number of bytes

    //creates a buffer so that the sys call write is only
    //called when "enough bytes" = 4096 or x (if x i.e. total
    //is less than 4096.

    size_t noOfBytesWritten=0;

    //adding the text the user wants to write to the buffer of text
    //also keeping track of that number of bytes

    //when it doesn't send the number of bytes over 4096
    if ((stream->bytesInBuffer+strlen(buf))<=4096)
    {
        for (int i=0; i<strlen(buf);i++)
        {
            stream->textBuffer[stream->bytesInBuffer+i] = *(buf+i);
        }
        stream->bytesInBuffer+=strlen(buf);
    }

    //when the new my write call sends the bytes over 4096
    while ((stream->bytesInBuffer+strlen(buf))>=4096)
    {
        //add bytes till buffer fills
        for (int i=0; i<4096-(stream->bytesInBuffer);i++)
        {
             stream->textBuffer[(stream->bytesInBuffer)+i] = *(buf+i);
        }
        noOfBytesWritten += write (stream->fD, stream->textBuffer, 4096);
        stream->bytesInBuffer=0;
    }

    return noOfBytesWritten; // NOTE: this isn't exactly
    // what fwrite returns but I've set it like this right now so I
    // can see what's happening
}

size_t myread (void *ptr, size_t size, size_t nmemb, struct fileStruct *stream)
{
    //creates a buffer so that the sys call read is only
    //called when it's worth it - similar to mywrite
    return 0;
}

//other two functions myseek and myflush to be added still
