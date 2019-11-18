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

struct file *myopen(const char *pathname, const char *mode){
  //creates an environment where read and write can function properly;
  int fileDesc;
  struct file *fileOpened = malloc (sizeof(struct file));
  if (!fileOpened){
   return NULL;
  }
  //determing file type
  if (strcmp (mode, "r") == 0){
   fileDesc = open (pathname, O_RDONLY, 0666);
  }
  else if (strcmp (mode, "w") == 0){
   fileDesc = open (pathname, O_WRONLY| O_CREAT | O_TRUNC, 0666);
  }
  else if (strcmp (mode, "r+") == 0){
   fileDesc = open (pathname, O_RDWR, 0666);
  }
  else if (strcmp (mode, "w+") == 0){
   fileDesc = open (pathname, O_RDWR| O_CREAT | O_TRUNC, 0666);
  }
  //checking if open is a success or failure
  if (fileDesc==-1){
   free (fileOpened);
   return NULL;
  }
  fileOpened->positionInReadBuffer = 0;
  fileOpened->bytesInWriteBuffer = 0;
  fileOpened->fD = fileDesc;
  return fileOpened;
}

int myflush(struct file *stream){
  size_t flushingBytesWritten=0;
  //checking if their is a need to flush
  if (stream->bytesInWriteBuffer!=0){
    if (stream->bytesInWriteBuffer<=BUFFER_SIZE){
     flushingBytesWritten
     = write (stream->fD, stream->writeBuffer ,stream->bytesInWriteBuffer);
    }
  }
  //clearing the write buffer once flushing is completed
  stream->bytesInWriteBuffer = 0;
  //error checking
  if (flushingBytesWritten==-1){
   return EOF;
  }
  else{
   return 0;
  }
}

int myclose(struct file *stream){

    int flushResult = myflush (stream);
    if (flushResult != 0){
     return EOF;
    }
    return close (stream->fD);
}

size_t mywrite(const char *buf, size_t nmemb, struct file* stream){
    /*creates a buffer so that the sys call write is only
    called when "enough bytes" accumulate*/
    size_t noOfBytesWritten=0;
    int bufferLength = stream->bytesInWriteBuffer+nmemb;
    //when it doesn't send the number of bytes over bufferSize
    if (bufferLength<BUFFER_SIZE){
        for (int i=0; i<nmemb;i++){
            stream->writeBuffer[stream->bytesInWriteBuffer+i] = *(buf+i);
            noOfBytesWritten++;
        }
        stream->bytesInWriteBuffer+=nmemb;
    }
    else{
    //when the new my write call sends the bytes over bufferSize
      int alreadyWritten = 0;
      while (bufferLength>=BUFFER_SIZE){
        //add bytes till buffer fills
        for (int i=0; i<BUFFER_SIZE-(stream->bytesInWriteBuffer);i++){
        stream->writeBuffer[(stream->bytesInWriteBuffer)+i] = *(buf+i+alreadyWritten);
        noOfBytesWritten++;
        }
        write (stream->fD, stream->writeBuffer, BUFFER_SIZE);
        alreadyWritten+=BUFFER_SIZE-(stream->bytesInWriteBuffer);
        //increases when it goes through a whole buffer
        stream->bytesInWriteBuffer=0;
        //clears buffer
        bufferLength = bufferLength - BUFFER_SIZE;
        //bufferLength is now the updated number of bytes to be written
        //in a way it is the excess bytes from "buf" that didn't get written
      }

      int numberOfExcessBytes = bufferLength;
      //we need a way to add the excess bytes to the buffer
      int  locationOfFirstExcessByte = nmemb-bufferLength;
      if (numberOfExcessBytes<BUFFER_SIZE){
       for (int i=0, j = locationOfFirstExcessByte; j<nmemb; i++,j++){
        stream-> writeBuffer[i] = *(buf+j);
        noOfBytesWritten++;
        }
       stream->bytesInWriteBuffer = bufferLength;
      }
      else if (numberOfExcessBytes>=BUFFER_SIZE){
       bufferLength = numberOfExcessBytes;
      }
  }
     return noOfBytesWritten;
}

size_t myread(char *ptr, size_t nmemb, struct file *stream){
 /*myRead front loads the operation, unlike myWrite; what this
 essentially means is that with the first call to myread
 we read for the full buffer size; and then we keep track
 of it according to the manner in which myRead is called*/
 int noOfBytesRead = 0;
 //incase file is rw, we call flush before reading
 int flushResult = myflush (stream);
 if (flushResult != 0){
   return 0;
 }
 //if there is no front loaded read call
 if (stream->positionInReadBuffer == 0){
  if (nmemb <= BUFFER_SIZE){
   size_t sysCallReturnValue = read (stream->fD, stream->readBuffer, BUFFER_SIZE);
   if (sysCallReturnValue == -1){
    return 0;
   }
   if (sysCallReturnValue != 0){
    for (int i=0; i<nmemb;i++){
     if (stream->readBuffer[i] != '\0'){
      *(ptr+i) = stream->readBuffer [i];
      stream->positionInReadBuffer = i+1;
      noOfBytesRead++;
      }
     }
    }
   }
  while (nmemb>BUFFER_SIZE){
   size_t sysCallReturnValue = read (stream->fD, stream->readBuffer, BUFFER_SIZE);
   if (sysCallReturnValue == -1){
    return 0;
   }
   if (sysCallReturnValue != 0){
    for (int i=0;i<BUFFER_SIZE; i++){
     if (stream->readBuffer [stream->positionInReadBuffer] != '\0'){
      *(ptr+i) = stream->readBuffer [stream->positionInReadBuffer];
      stream->positionInReadBuffer++;
      noOfBytesRead++;
      }
     }
    continue;
   }
   nmemb = nmemb - BUFFER_SIZE;
   if (nmemb <= BUFFER_SIZE){
    size_t sysCallReturnValue = read (stream->fD, stream->readBuffer, BUFFER_SIZE);
    if (sysCallReturnValue == -1){
     return 0;
    }
    if (sysCallReturnValue !=0){
     for (int i=0; i<nmemb;i++){
      if (stream->readBuffer [i] != '\0'){
       *(ptr+i) = stream->readBuffer [i];
       stream->positionInReadBuffer = i+1;
       noOfBytesRead++;
      }
     }
    }
   }
  }
 }
 //if there is a front loaded read call
 else{
  int bytesThatCanBeReadWithoutSysCall = BUFFER_SIZE - stream->positionInReadBuffer;
  if (bytesThatCanBeReadWithoutSysCall>=nmemb){
   //then simply just give them what they want
   for (int j=0; j<nmemb; stream->positionInReadBuffer++, j++){
    *(ptr+j) = stream->readBuffer [stream->positionInReadBuffer];
    noOfBytesRead++;
   }
  }
  else if (bytesThatCanBeReadWithoutSysCall<nmemb){
   //now you cannot just give them what they want, because you
   //don't have it, so you need a system call
   //first give them the ones that are already in the buffer
   for (int j=0; j<bytesThatCanBeReadWithoutSysCall; stream->positionInReadBuffer++, j++){
   *(ptr+j) = stream->readBuffer [stream->positionInReadBuffer];
   noOfBytesRead++;
   }
   //then give them the ones that require sys calls
   int noOfBytesThatStillNeedToBeRead = nmemb - bytesThatCanBeReadWithoutSysCall;
   stream->positionInReadBuffer = 0;
   int updateValForPtrLocation = bytesThatCanBeReadWithoutSysCall;
   if (noOfBytesThatStillNeedToBeRead <= BUFFER_SIZE){
    size_t sysCallReturnValue = read (stream->fD, stream->readBuffer, BUFFER_SIZE);
    if (sysCallReturnValue == -1){
     return 0;
    }
    for (int j=updateValForPtrLocation; stream->positionInReadBuffer<noOfBytesThatStillNeedToBeRead;
    stream->positionInReadBuffer++, j++){
     *(ptr+j) = stream->readBuffer [stream->positionInReadBuffer];
     noOfBytesRead++;
    }
   }
   else{
    //here I can just give them what they want directly use the system
    //call since they want more than the buffer_size, thus making it
    //more efficient
    ptr = ptr+updateValForPtrLocation*(sizeof(char));
    size_t sysCallReturnValue = read (stream->fD, ptr, noOfBytesThatStillNeedToBeRead);
    noOfBytesRead += noOfBytesThatStillNeedToBeRead;
    if (sysCallReturnValue == -1){
     return 0;
    }
    stream->positionInReadBuffer = 0;
   }
  }
 }
 return noOfBytesRead;
}

int myseek(struct file *stream, long offset, int whence){
  int flushResult = myflush (stream);
  if (flushResult != 0){
   return -1;
  }
  if (whence == SEEK_CUR){
  //working on the read buffer for seek_cur
   if (stream->positionInReadBuffer+offset<0){
    int rOffset = lseek (stream->fD, offset, SEEK_CUR);
    if (rOffset == -1){
     return -1;
    }
    stream->positionInReadBuffer = 0;
    return 0;
   }
   else if (stream->positionInReadBuffer+offset<BUFFER_SIZE){
    stream->positionInReadBuffer += offset;
    return 0;
   }
   else if (stream->positionInReadBuffer+offset>BUFFER_SIZE){
    int rOffset = lseek (stream->fD, stream->positionInReadBuffer+offset, SEEK_SET);
    stream->positionInReadBuffer = 0;
    if (rOffset == -1){
     return -1;
     }
    return 0;
    }
  }

  //seek_set for read and write buffers together
  else if (whence == SEEK_SET){
   int rOffset = lseek (stream->fD, offset, SEEK_SET);
   if (rOffset == -1){
    return -1;
   }
   //resetting read at this position
   stream->positionInReadBuffer = 0;
   //resetting write buffer at this position
   stream->bytesInWriteBuffer = 0;
   memset (stream->writeBuffer, 0, sizeof (stream->writeBuffer));
   return 0;
 }
 return -1;
}
