#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "myio.h"

struct fileStruct *myopen (const char *pathname, const char *mode)
{
  //creates an environment where read and write can function properly;

  int fileDesc;
  struct fileStruct *fileOpened = malloc (sizeof(struct fileStruct));

  //determing file type
  if (strcmp (mode, "r") == 0)
  {
      fileDesc = open (pathname, O_RDONLY, 0666);
      fileOpened -> positionInReadBuffer = 0;
      fileOpened -> bytesInWriteBuffer = 0;
  }
  else if (strcmp (mode, "w") == 0)
  {
      fileDesc = open (pathname, O_WRONLY| O_CREAT | O_TRUNC, 0666);
      fileOpened -> positionInReadBuffer = 0;
      fileOpened -> bytesInWriteBuffer = 0;
  }
  else if (strcmp (mode, "r+") == 0)
  {
      fileDesc = open (pathname, O_RDWR, 0666);
      fileOpened -> positionInReadBuffer = 0;
      fileOpened -> bytesInWriteBuffer = 0;
  }
  else if (strcmp (mode, "w+") == 0)
  {
      fileDesc = open (pathname, O_RDWR| O_CREAT | O_TRUNC, 0666);
      fileOpened -> positionInReadBuffer = 0;
      fileOpened -> bytesInWriteBuffer = 0;
  }

  //checking if open is a success or failure
  if (fileDesc==-1)
  {
      printf ("File Open Failed\n");
      printf("Error Opening File.  Description:  %s\n", strerror(errno));
  }
  else
  {
      printf ("File Opened Successfully.\n");
  }

  fileOpened -> fD = fileDesc;

  return fileOpened;

  //still need to implement some stuff like
  //the file creation flags specified - used in
  //exceptional situations like when there is an error
}

int myflush (struct fileStruct *stream)
{
  size_t closingBytesWritten;

  if (stream->bytesInWriteBuffer!=0)
  {
      if (stream->bytesInWriteBuffer<=BUFFER_SIZE)
      {
          closingBytesWritten
          = write (stream->fD, stream->writeBuffer ,stream->bytesInWriteBuffer);
      }
  }

  stream->bytesInWriteBuffer = 0;
  memset (stream->writeBuffer, 0, sizeof (stream->writeBuffer));
  //clearing buffer

  if (closingBytesWritten==-1)
  {
    return EOF;
  }

  else
  {
    return 0;
  }

}

int myclose (struct fileStruct *stream)
{

    printf ("Calling myflush On Closing.\n");
    int flushResult = myflush (stream);

    if (flushResult != 0)
    {
      printf ("myFlush Failed During Closing\n");
    }

    return close (stream->fD);

}

size_t mywrite (const char *buf, size_t nmemb, struct fileStruct* stream)
{
    /*creates a buffer so that the sys call write is only
    called when "enough bytes" accumulate*/

    size_t noOfBytesWritten=0;

    /*the code below is basically adding the text the user wants to write
    to the buffer of text also keeping track of that number of bytes*/

    int bufferLength = stream->bytesInWriteBuffer+nmemb;

    //when it doesn't send the number of bytes over bufferSize
    if (bufferLength<BUFFER_SIZE)
    {
        for (int i=0; i<strlen(buf);i++)
        {
            stream->writeBuffer[stream->bytesInWriteBuffer+i] = *(buf+i);
        }
        stream->bytesInWriteBuffer+=nmemb;
        noOfBytesWritten = nmemb;
    }

    //when the new my write call sends the bytes over bufferSize
    while (bufferLength>=BUFFER_SIZE)
    {
        //add bytes till buffer fills
        for (int i=0; i<BUFFER_SIZE-(stream->bytesInWriteBuffer);i++)
        {
             stream->writeBuffer[(stream->bytesInWriteBuffer)+i] = *(buf+i);
        }

        noOfBytesWritten += write (stream->fD, stream->writeBuffer, BUFFER_SIZE);

        stream->bytesInWriteBuffer=0;
        memset (stream->writeBuffer, 0, sizeof (stream->writeBuffer));
        //above two lines are clearing the buffer
        bufferLength = bufferLength - BUFFER_SIZE;

        //bufferLength is now the updated number of bytes to be written
        //in a way it is the excess bytes from "buf" that didn't get written
    }

    int numberOfExcessBytes = bufferLength;

    //we need a way to add the excess bytes to the buffer
    int  locationOfFirstExcessByte = nmemb-bufferLength;

    if (numberOfExcessBytes<BUFFER_SIZE)
    {
        for (int i=0, j = locationOfFirstExcessByte; j<nmemb; i++,j++)
        {
            stream-> writeBuffer[i] = *(buf+j);
        }
        stream->bytesInWriteBuffer = bufferLength;
    }

    else if (numberOfExcessBytes>=BUFFER_SIZE)
    {
        bufferLength = numberOfExcessBytes;
    }

    return noOfBytesWritten;
}

size_t myread (char *ptr, size_t nmemb, struct fileStruct *stream)
{
    //myRead front loads the operation, unlike myWrite; what this
    //essentially means is that with the first call to myread
    //we read for the full buffer size; and then we keep track
    //of it according to the manner in which myRead is called

    int noOfBytesRead = 0;

    printf ("Calling myflush Before Reading.\n");
    //incase file is both read and write
    int flushResult = myflush (stream);

    if (flushResult != 0)
    {
      printf ("myFlush Failed During Closing\n");
    }

    //if there is no front loaded read call
    if (stream->positionInReadBuffer == 0)
    {
        if (nmemb <= BUFFER_SIZE)
        {
            size_t sysCallReturnValue = read (stream->fD, stream -> readBuffer, BUFFER_SIZE);
            if (sysCallReturnValue == -1)
            {
                printf ("Error in myread functionality.\n");
            }

            if (sysCallReturnValue != 0)
            {
                for (int i=0; i<nmemb;i++)
                {
                    if (stream->readBuffer[i] != '\0')
                    {
                      *(ptr+i) = stream->readBuffer [i];
                      stream->positionInReadBuffer = i;
                      noOfBytesRead++;
                    }
                }
            }
          }

        while (nmemb>BUFFER_SIZE)
        {
            size_t sysCallReturnValue = read (stream->fD, stream -> readBuffer, BUFFER_SIZE);
            if (sysCallReturnValue == -1)
            {
                printf ("Error in myread functionality.\n");
            }

            if (sysCallReturnValue != 0)
            {
                for (int i=0;i<BUFFER_SIZE; i++)
                {
                  if (stream->readBuffer [stream->positionInReadBuffer] != '\0')
                  {
                      *(ptr+i) = stream->readBuffer [stream->positionInReadBuffer];
                      stream->positionInReadBuffer++;
                      noOfBytesRead++;
                  }
                }
                continue;
            }

            nmemb = nmemb - BUFFER_SIZE;

            if (nmemb <= BUFFER_SIZE)
            {
                size_t sysCallReturnValue = read (stream->fD, stream -> readBuffer, BUFFER_SIZE);

                if (sysCallReturnValue == -1)
                {
                    printf ("Error in myread functionality.\n");
                }

                if (sysCallReturnValue !=0)
                {
                    for (int i=0; i<nmemb;i++)
                    {
                      if (stream->readBuffer [i] != '\0')
                      {
                        *(ptr+i) = stream->readBuffer [i];
                        stream->positionInReadBuffer = i;
                        noOfBytesRead++;
                      }
                    }
                }
            }
        }

    }

    //if there is a front loaded read call
    else
    {
        int bytesThatCanBeReadWithoutSysCall = BUFFER_SIZE - stream -> positionInReadBuffer;

        if (bytesThatCanBeReadWithoutSysCall>=nmemb)
        //then simply just give them what they want
        {
            for (int j=0; j<nmemb; stream->positionInReadBuffer++, j++)
            {
                *(ptr+j) = stream->readBuffer [stream->positionInReadBuffer];
                noOfBytesRead++;
            }
        }

        else if (bytesThatCanBeReadWithoutSysCall<nmemb)
        //now you cannot just give them what they want, because you
        //don't have it, so you need a sys call
        {
            //first give them the ones that are already in the buffer
            for (int j=0; j<bytesThatCanBeReadWithoutSysCall; stream->positionInReadBuffer++, j++)
            {
                *(ptr+j) = stream->readBuffer [stream->positionInReadBuffer];
                noOfBytesRead++;
            }

            //then give them the ones that require sys calls
            int noOfBytesThatStillNeedToBeRead = nmemb - bytesThatCanBeReadWithoutSysCall;
            stream -> positionInReadBuffer = 0;
            int updateValForPtrLocation = bytesThatCanBeReadWithoutSysCall;

            if (noOfBytesThatStillNeedToBeRead <= BUFFER_SIZE)
            {
                size_t sysCallReturnValue = read (stream->fD, stream -> readBuffer, BUFFER_SIZE);
                if (sysCallReturnValue == -1)
                {
                    printf ("Error in myread functionality.\n");
                }

                for (int j=updateValForPtrLocation;
                    stream->positionInReadBuffer<noOfBytesThatStillNeedToBeRead;
                    stream->positionInReadBuffer++, j++)
                {
                    *(ptr+j) = stream -> readBuffer [stream->positionInReadBuffer];
                    noOfBytesRead++;
                }
            }

            else
            {
                //here I can just give them what they want directly use the system
                //call since they want more than the buffer_size, thus making it
                //more efficient
                ptr = ptr+ updateValForPtrLocation*(sizeof(char));
                size_t sysCallReturnValue = read (stream->fD, ptr, noOfBytesThatStillNeedToBeRead);
                noOfBytesRead = noOfBytesThatStillNeedToBeRead;
                if (sysCallReturnValue == -1)
                {
                    printf ("Error in myread functionality.\n");
                }
                stream->positionInReadBuffer = 0;
            }
        }
    }

    return noOfBytesRead;
}

int myseek(struct fileStruct *stream, long offset, int whence)
{
  //so how this function works is by adjusting the offset for read/writeBuffer
  //as well as the file offset buffer

  //let's say the file is a read only file
  if (whence == SEEK_CUR)
  {
    if (stream->positionInReadBuffer+=offset<BUFFER_SIZE)
    {
      stream->positionInReadBuffer += offset;
      return 0;
    }
    else if (stream->positionInReadBuffer+=offset<0)
    {
      lseek (stream->fD, offset, whence);
      stream->positionInReadBuffer = BUFFER_SIZE-(stream->positionInReadBuffer+offset);
      return 0;
    }
    else
    {
      size_t sysCallReturnValue = read (stream->fD, stream -> readBuffer, BUFFER_SIZE);
      if (sysCallReturnValue == -1)
      {
          printf ("Error in myread functionality.\n");
      }
      stream->positionInReadBuffer = stream->positionInReadBuffer+offset - BUFFER_SIZE;
      return 0;
    }
  }

  else if (whence == SEEK_SET)
  {
    stream->positionInReadBuffer = offset;
  }

  //if the offset asked for changes location to some bytes that I have, then I
  //do not need to make a system call to lseek, I can just change my position in the
  //bufferSize

  //if the offset puts me into some area I do not know of, then I need to make
  //a call to lseek and get to this new area of memory

  //whence = SEEK_SET - the file offset is set to offset bytes

  //whence = SEEK_CUR - the file offset is set to it's current location
  //plus offset bytes

  //other two functions myseek and myflush to be added still

  //the file offset is determined by the type of the file; in a read only file
  //it is the positionInReadBuffer; in a write only file it is the positionInWriteBuffer;
  //for a read-write file it is the positionInRWBuffer (still to be made by while
  //working with read and write together)

  return 1;
}
