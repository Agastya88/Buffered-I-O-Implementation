struct fileStruct *myopen (const char *pathname, const char *mode);
int myclose (struct fileStruct *stream);
size_t myread (char *ptr, size_t nmmemb, struct fileStruct *stream);
size_t mywrite (const char *buf, size_t nmemb, struct fileStruct *stream);
size_t getFilesize(const char* filename);
