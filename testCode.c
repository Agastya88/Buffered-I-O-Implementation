#include "myio.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

//This program tests the functionality of read and write individually.

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
