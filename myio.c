FILE *myopen (constant char *pathname, const char *mode);
int myclose (FILE *stream);
size_t myread (void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t mywrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

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

size_t mywrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    //creates a buffer so that the sys call write is only
    //called when "enough bytes" = 4096 or x (if x i.e. total
    //is less than 4096.
}

size_t myread (void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    //creates a buffer so that the sys call read is only
    //called when it's worth it - similar to mywrite
}

//other two functions myseek and myflush to be added still
