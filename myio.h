#include <sys/types.h>

#define BUFFER_SIZE 20

struct file
{
    int fD;
    char writeBuffer [BUFFER_SIZE];
    int bytesInWriteBuffer;
    char readBuffer [BUFFER_SIZE];
    int positionInReadBuffer;
};

struct file *myopen (const char *pathname, const char *mode);
int myclose (struct file *stream);
size_t myread (char *ptr, size_t nmmemb, struct file *stream);
size_t mywrite (const char *buf, size_t nmemb, struct file *stream);
int myflush (struct file *stream);
int myseek(struct file *stream, long offset, int whence);
