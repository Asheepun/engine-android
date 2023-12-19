#ifndef FILES_H_
#define FILES_H_

#include "engine/strings.h"
#include "engine/assetManager.h"

//typedef char FileLine[STRING_SIZE];

char *getFileData_mustFree(const char *, long int *, AAssetManager *);

String *getFileLines_mustFree(const char *, int *, AAssetManager *);

void writeDataToFile(const char *, char *, long int);

String *getDirectoryFileNames_mustFree(const char *, int *);

#endif
