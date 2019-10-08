#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <errno.h>

int myopen (const char *pathname, const char *mode);
int myclose (FILE *stream);
size_t myread (void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t mywrite(char *pathname, const char *buf, size_t count);

#define BUFFER_SIZE 4096;

int main (int argc, char* argv[])
{

    char *textToWrite = "Sammy and Agastya - Assignment 2";
    size_t bytesWritten;
    bytesWritten = mywrite ("./randomFile.txt", textToWrite, strlen(textToWrite));
    printf ("The Number of Bytes Written Are: %zu \n", bytesWritten);
    //call functions for testing
}

int myopen (const char *pathname, const char *mode)
{
  //makes use of the system call "open" to allow
  //for the opening of files according to their flags

  int file;

  if (strcmp (mode, "r") == 0)
  {
      file = open (pathname, O_RDONLY);
  }
  else if (strcmp (mode, "w") == 0)
  {
      file = open (pathname, O_WRONLY);
  }
  else if (strcmp (mode, "rw") == 0)
  {
      file = open (pathname, O_RDWR);
  }

  return file;

  //still need to figure out how to implement
  //the file creation flags specified - used in
  //exceptional situations like when there is an error

  //so the purpose of these is to make an environment in which
  //read and write can work
}

int myclose (FILE *stream)
{
    return 0;
}

size_t mywrite(char *pathname, const char *buf, size_t count)
{
    //note to self: buf is a pointer to a buffer of at least
    //'count' number of bytes

    //creates a buffer so that the sys call write is only
    //called when "enough bytes" = 4096 or x (if x i.e. total
    //is less than 4096.

    int writeableFile;

    writeableFile = myopen (pathname, "w");

    size_t noOfBytes = write (writeableFile, buf, count);

    //need to call the above statement correctly i.e.
    //in a way that will count the bytes first and then make the system
    //call only once 4096 bytes have asked to be written or when
    //the all the bytes have been asked and the total number of bytes
    //is less than 4096

    return noOfBytes;
}

size_t myread (void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    //creates a buffer so that the sys call read is only
    //called when it's worth it - similar to mywrite
    return 0;
}

//other two functions myseek and myflush to be added still
