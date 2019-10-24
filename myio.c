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

#define BUFFER_SIZE 1024

struct fileStruct
{
    int fD;
    char writeBuffer [BUFFER_SIZE];
    int bytesInWriteBuffer;
    char readBuffer [BUFFER_SIZE];
    int positionInReadBuffer;
    int fileSize;
};

int main (int argc, char* argv[])
{

    printf ("\n");
    printf ("------------WRITE TESTING BEGINS HERE-------------\n");

    struct fileStruct *fileOne = myopen ("randomFile.txt", "w");

    //first text being written  using mywrite;
    // prints result and leftover bytes;
    char *textToWrite = "insert some long text here";
    size_t bytesWrittenInCallOne;
    bytesWrittenInCallOne = mywrite (textToWrite, strlen(textToWrite), fileOne);
    printf ("mywrite callOne returns: %zu \n", bytesWrittenInCallOne);
    printf ("fileOne->bytesInWriteBuffer is: %d\n", fileOne->bytesInWriteBuffer);

    //another text being written  using mywrite;
    //prints result and leftover bytes;
    char *textToWriteTwo = "include text here";
    size_t bytesWrittenInCallTwo;
    bytesWrittenInCallTwo = mywrite (textToWriteTwo, strlen(textToWriteTwo), fileOne);
    printf ("mywrite callTwo returns: %zu \n", bytesWrittenInCallTwo);
    printf ("fileOne->bytesInWriteBuffer is: %d\n", fileOne->bytesInWriteBuffer);

    //testing works till here; now I'm gonna try and throw it over 4096
    //huge third text being written  using mywrite;
    //prints result and leftover bytes;

    char *textToWriteThree = "Lorem ipsum dolor sit amet, consectetur adipiscing elit."
    "Morbi consectetur finibus luctus. Quisque ut dui fermentum, aliquam justo non, "
    "luctus diam. Vivamus molestie libero et arcu tincidunt, lobortis porta nisl vulputate."
    " Pellentesque ac iaculis velit. Etiam euismod vestibulum dolor, congue bibendum lacus"
    "porttitor id. Aliquam dictum nulla tellus, ut ultrices velit tempus vel. Quisque magna"
    "dui, sagittis eget luctus vehicula, dapibus ut magna. Fusce nec mollis eros, ac luctus"
    "lectus. Vestibulum purus quam, suscipit vel velit vel, interdum fringilla nisi. Donec"
    "bibendum aliquam tellus, nec porttitor est rutrum a. Nulla dapibus mauris ac tristique"
    "ultricies. Sed semper maximus tellus, sed vestibulum ipsum laoreet congue. In hac habitasse"
     "platea dictumst. Pellentesque habitant morbi tristique senectus et netus et malesuada fames"
     "ac turpis egestas. Morbi egestas eleifend volutpat.Nunc quis tincidunt velit. Sed feugiat ac"
     "nisi ut fermentum. Aenean pellentesque tincidunt mattis. Proin rutrum placerat nisl non iaculis."
     "Praesent velit lacus, imperdiet feugiat ipsum non, efficitur scelerisque justo. Sed viverra"
     "finibus nulla, non sodales mauris tristique eget. In quis quam leo. Donec ut pharetra ligula,"
     "eget pharetra arcu. Vivamus blandit metus in blandit molestie. Praesent ultrices nunc vitae purus"
     "sodales, nec volutpat risus pharetra. Donec sed lacus ipsum. Praesent imperdiet volutpat est vitae"
     "gravida. Duis vitae malesuada ligula, ac ornare est. Integer sed blandit velit. Cras hendrerit"
     "ultricies nisi. Etiam sit amet turpis vel lectus ultrices aliquet. Ut convallis dapibus quam, vitae"
     "ornare sapien porta lobortis. Aliquam commodo velit ac elementum consectetur. Sed sit amet urna vel"
     "nulla semper tempus vitae non velit. Nunc tincidunt sapien vel ipsum dignissim, ut vehicula tortor"
     "porttitor. Curabitur vitae efficitur elit. Maecenas elementum nibh quis viverra auctor. Vivamus ut"
     "facilisis eros. Donec erat nulla, tempus ut nisi in, egestas feugiat massa. Praesent malesuada lectus"
     "ac ligula tempus sagittis. Donec non augue sed purus laoreet pretium. Vestibulum ut lectus sapien. Mauris"
     "condimentum est aliquet sem consectetur finibus. Ut id metus quis arcu laoreet accumsan non at nisl. Fusce"
     "sed ligula at erat fringilla vehicula non nec orci. Donec euismod molestie enim placerat dignissim. Vestibulum"
     "ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Cras rutrum odio eu dolor viverra,"
     "non malesuada eros volutpat. Quisque a ligula quis lectus fringilla facilisis. Nunc venenatis purus leo, vel"
     "elementum erat porta in. Mauris id purus pulvinar, hendrerit nulla ac, semper ante. Pellentesque mi lectus, consequat"
    "eget suscipit ac, porta ut neque. Nulla quis mi in ex sollicitudin venenatis. Donec eu lacus erat. Maecenas molestie"
     "purus nec erat fringilla, ac semper nisi ultrices.Aenean vitae nulla facilisis, porttitor tortor vitae, pharetra lacus."
     "Praesent accumsan ex massa, non elementum magna pharetra ac. Fusce vel enim et arcu feugiat luctus in vitae purus."
     "Aliquam non tincidunt diam, elementum convallis ex. Vivamus finibus purus eu nunc venenatis, et hendrerit tellus"
     "gravida. Integer ultrices eget lorem vitae mollis. Etiam id mattis ipsum, vitae ornare ex. Nullam id orci finibus,"
     "blandit lacus suscipit, varius nibh. Pellentesque eget malesuada ipsum, ac consequat justo. Etiam aliquam eleifend"
     "magna eget vehicula. Phasellus laoreet mauris in viverra molestie. Duis elementum sem eu massa facilisis, malesuada"
     "interdum risus lacinia. Nullam et est nunc. Sed viverra gravida dolor, non molestie neque fermentum et. Mauris cursus"
     "ante nisl, vel egestas tortor rutrum id.Donec in felis quam. Integer scelerisque molestie lectus quis tincidunt. Sed"
     "faucibus, velit ut ultrices pharetra, neque leo hendrerit magna, sit amet aliquet felis libero in lacus. Aenean"
     "neque elit, congue euismod purus nec, vulputate viverra quam. Donec dui ipsum, malesuada sit amet aliquam vel,"
     "fringilla et elit. Nulla lacinia sed augue ut tempus. Suspendisse potenti. Morbi pharetra, nunc non sollicitudin"
      "ornare, tortor dolor finibus neque, vitae porta tortor neque vitae leo.";

    size_t bytesWrittenInCallThree;
    bytesWrittenInCallThree = mywrite (textToWriteThree, strlen(textToWriteThree), fileOne);
    printf ("mywrite callThree returns: %zu \n", bytesWrittenInCallThree);
    printf ("fileOne->bytesInWriteBuffer is: %d\n", fileOne->bytesInWriteBuffer);

    int closeResult = myclose (fileOne);

    if (closeResult == -1)
    {
        printf("Error Closing File.  Description:  %s\n", strerror(errno));
    }

    else if (closeResult == 0)
    {
        printf ("File Closed Successfully.\n");
    }

    free (fileOne); //done with all operations on this file struct

    printf ("------------WRITE TESTING ENDS HERE-------------\n");
    printf ("\n");
    printf ("------------READ TESTING BEGINS HERE-------------\n");

    //TEST: call myread on the file I just wrote to see if I can read it
    struct fileStruct *fileTwo = myopen ("randomFile.txt", "r");

    char *readPtr = malloc (173*sizeof(char));
    char *readPtr2 = malloc (1000*sizeof(char));

    size_t bytesReadInCallOne = myread (readPtr, 173,
         fileTwo); //trying to read 173 bytes from fileTwo into rSL
    printf ("myread callOne returns: %zu \n", bytesReadInCallOne);
    printf ("readPtr returns: %s\n", readPtr);

    free (readPtr);

    size_t bytesReadInCallTwo = myread (readPtr2, 1000,
         fileTwo); //trying to read 1000 bytes from fileTwo into rSL
    printf ("myread callTwo returns: %zu \n", bytesReadInCallTwo);
    printf ("readPtr2 returns: %s\n", readPtr2);

    free (readPtr2);

    int closeResultTwo = myclose (fileTwo);

    if (closeResultTwo == -1)
    {
        printf("Error Closing File.  Description:  %s\n", strerror(errno));
    }

    else if (closeResultTwo == 0)
    {
        printf ("File Closed Successfully.\n");
    }

    free (fileTwo);
    //close file once I'm done reading
    //free memory when done with all operations on the file

    printf ("------------READ TESTING ENDS HERE-------------\n");
    printf ("\n");
}

struct fileStruct *myopen (const char *pathname, const char *mode)
{
  //creates an environment where read and write can function properly;

  int fileDesc;
  struct fileStruct *fileOpened = malloc (sizeof(struct fileStruct));

  //determing file type
  if (strcmp (mode, "r") == 0)
  {
      fileDesc = open (pathname, O_RDONLY);
      fileOpened -> positionInReadBuffer = 0;
      fileOpened -> bytesInWriteBuffer = 0;
  }
  else if (strcmp (mode, "w") == 0)
  {
      fileDesc = open (pathname, O_WRONLY);
      fileOpened -> positionInReadBuffer = 0;
      fileOpened -> bytesInWriteBuffer = 0;
  }
  else if (strcmp (mode, "rw") == 0)
  {
      fileDesc = open (pathname, O_RDWR);
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

int myclose (struct fileStruct *stream)
{
    if (stream->bytesInWriteBuffer!=0)
    {
        if (stream->bytesInWriteBuffer<=BUFFER_SIZE)
        {
            size_t closingBytesWritten =
            write (stream->fD, stream->writeBuffer ,stream->bytesInWriteBuffer);
                printf ("Bytes written on closing: %zu\n",
                 closingBytesWritten);
        }
    }

    if (stream->positionInReadBuffer!=0)
    {
        if (stream->positionInReadBuffer<=BUFFER_SIZE)
        {

        }
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
            //value will be used in checking for sys call success/failer
            for (int i=0; i<nmemb;i++)
            {
                *(ptr+i) = stream->readBuffer [i];
                stream->positionInReadBuffer = i;
                noOfBytesRead++;
            }
        }

        while (nmemb>BUFFER_SIZE)
        {
            size_t sysCallReturnValue = read (stream->fD, stream -> readBuffer, BUFFER_SIZE);
            if (sysCallReturnValue == -1)
            {
                printf ("Error in myread functionality.\n");
            }

            for (int i=0;i<BUFFER_SIZE; i++)
            {
                *(ptr+i) = stream->readBuffer [stream->positionInReadBuffer];
                stream->positionInReadBuffer++;
                noOfBytesRead++;
            }

            nmemb = nmemb - BUFFER_SIZE;

            if (nmemb <= BUFFER_SIZE)
            {
                size_t sysCallReturnValue = read (stream->fD, stream -> readBuffer, BUFFER_SIZE);
                if (sysCallReturnValue == -1)
                {
                    printf ("Error in myread functionality.\n");
                }
                //value will be used in checking for sys call success/failer
                for (int i=0; i<nmemb;i++)
                {
                    *(ptr+i) = stream->readBuffer [i];
                    stream->positionInReadBuffer = i;
                    noOfBytesRead++;
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

//other two functions myseek and myflush to be added still
