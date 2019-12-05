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
 fileOpened->prevSysRV = 0;
 fileOpened->positionInFile = 0;
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
        //clearing the write buffer once flushing is completed
        stream->bytesInWriteBuffer = 0;
        stream->positionInReadBuffer = 0;
    }
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
    int returnFD = stream->fD;
    free (stream);
    return close (returnFD);
}

size_t mywrite(const char *buf, size_t nmemb, struct file* stream){
    /*creates a buffer so that the sys call write is only
    called when "enough bytes" accumulate*/
    int currentFileOffset = lseek (stream->fD, 0, SEEK_CUR);
    if (stream->positionInReadBuffer!=0){
        lseek (stream->fD, stream->positionInFile-currentFileOffset, SEEK_CUR);
    }
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
    stream->positionInReadBuffer=0;
    stream->positionInFile += noOfBytesWritten;
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
            stream->prevSysRV = sysCallReturnValue;
            if (sysCallReturnValue == -1){
                return 0;
            }
            if (sysCallReturnValue<nmemb){
                nmemb = sysCallReturnValue;
            }
            if (sysCallReturnValue != 0){
                for (int i=0; i<nmemb;i++){
                    *(ptr+i) = stream->readBuffer [i];
                    stream->positionInReadBuffer = i+1;
                    noOfBytesRead++;
                }
            }
        }
        int alreadyRead = 0;
        while (nmemb>BUFFER_SIZE){
            stream->positionInReadBuffer = 0;
            size_t sysCallReturnValue = read (stream->fD, stream->readBuffer, BUFFER_SIZE);
            stream->prevSysRV = sysCallReturnValue;
            if (sysCallReturnValue == -1 || sysCallReturnValue == 0){
                return 0;
            }
            if (sysCallReturnValue != 0){
                for (int i=0;i<sysCallReturnValue; i++){
                    *(ptr+i+alreadyRead) = stream->readBuffer [stream->positionInReadBuffer];
                    stream->positionInReadBuffer++;
                    noOfBytesRead++;
                }
                if (sysCallReturnValue == BUFFER_SIZE){
                    nmemb = nmemb - BUFFER_SIZE;
                }
                else if (sysCallReturnValue < BUFFER_SIZE){
                    nmemb = sysCallReturnValue;
                }
                alreadyRead+=sysCallReturnValue;
            }
            if (nmemb <= BUFFER_SIZE){
                size_t sysCallReturnValue = read (stream->fD, stream->readBuffer, BUFFER_SIZE);
                stream->prevSysRV = sysCallReturnValue;
                if (sysCallReturnValue == -1){
                    return 0;
                }
                if (sysCallReturnValue !=0){
                    for (int i=0; i<nmemb;i++){
                        *(ptr+i+alreadyRead) = stream->readBuffer [i];
                        stream->positionInReadBuffer = i+1;
                        noOfBytesRead++;
                    }
                }
            }
        }
    }
    //if there is a front loaded read call
    else{
        int readWOSysCall = stream->prevSysRV - stream->positionInReadBuffer;
        if (readWOSysCall>=nmemb){
            //then simply just give them what they want
            for (int j=0; j<nmemb; stream->positionInReadBuffer++, j++){
                *(ptr+j) = stream->readBuffer [stream->positionInReadBuffer];
                noOfBytesRead++;
            }
        }
        else if (readWOSysCall<nmemb){
            //now you cannot just give them what they want, because you
            //don't have it, so you need a system call
            //first give them the ones that are already in the buffer
            for (int j=0; j<readWOSysCall; stream->positionInReadBuffer++, j++){
                *(ptr+j) = stream->readBuffer [stream->positionInReadBuffer];
                noOfBytesRead++;
            }
            //then give them the ones that require sys calls
            int bytesRemaining = nmemb - readWOSysCall;
            stream->positionInReadBuffer = 0;
            int newPtrLocation = readWOSysCall;
            if (bytesRemaining <= BUFFER_SIZE){
                size_t sysCallReturnValue = read (stream->fD, stream->readBuffer, BUFFER_SIZE);
                stream->prevSysRV = sysCallReturnValue;
                if (sysCallReturnValue == -1){
                    return 0;
                }
                for (int j=newPtrLocation; stream->positionInReadBuffer<bytesRemaining;
                    stream->positionInReadBuffer++, j++){
                        *(ptr+j) = stream->readBuffer [stream->positionInReadBuffer];
                        noOfBytesRead++;
                    }
                }
                else{
                    //here I can just give them what they want directly use the system
                    //call since they want more than the buffer_size, thus making it
                    //more efficient
                    ptr = ptr+newPtrLocation*(sizeof(char));
                    size_t sysCallReturnValue = read (stream->fD, ptr, bytesRemaining);
                    stream->prevSysRV = sysCallReturnValue;
                    noOfBytesRead += sysCallReturnValue;
                    if (sysCallReturnValue == -1){
                        return 0;
                    }
                    stream->positionInReadBuffer = 0;
                }
            }
        }
        stream->positionInFile += noOfBytesRead;
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
        return 0;
    }
    return -1;
}
