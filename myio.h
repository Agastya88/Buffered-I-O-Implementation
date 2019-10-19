struct fileStruct *myopen (const char *pathname, const char *mode);
int myclose (struct fileStruct *stream);
size_t myread (void *ptr, size_t size, size_t nmemb, struct fileStruct *stream);
size_t mywrite (const char *buf, size_t size, struct fileStruct *stream);
