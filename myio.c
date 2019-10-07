FILE *myopen (constant char *pathname, const char *mode);
int myclose (FILE *stream);
size_t myread (void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t mywrite(int fd, const void *buf, size_t count);

#include <unistd.h>

#define BUFFER_SIZE 4096;

int main (int argc, char* argv[])
{
  //call functions for testing
}

FILE *myopen (constant char *pathname, const char *mode);
{
  //makes use of the system call "open" to allow
  //for the opening of files according to their flags

  //so the purpose of these is to make an environment in which
  //read and write can work
}

int myclose (FILE *stream)
{

}

size_t mywrite(String filename, const void *buf, size_t count)
{
    //creates a buffer so that the sys call write is only
    //called when "enough bytes" = 4096 or x (if x i.e. total
    //is less than 4096.

    //the write sys call requires a file descriptor,
    //that can be recieved using the open sys call
    //[called by my open and requires file name] as
    //per http://codewiki.wikidot.com/c:system-calls:write
}

size_t myread (void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    //creates a buffer so that the sys call read is only
    //called when it's worth it - similar to mywrite
}

//other two functions myseek and myflush to be added still
