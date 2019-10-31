#include <sys/types.h>

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

struct fileStruct *myopen (const char *pathname, const char *mode);
int myclose (struct fileStruct *stream);
size_t myread (char *ptr, size_t nmmemb, struct fileStruct *stream);
size_t mywrite (const char *buf, size_t nmemb, struct fileStruct *stream);
int myflush (struct fileStruct *stream);
int myseek(struct fileStruct *stream, long offset, int whence);
