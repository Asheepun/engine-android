#include "engine/files.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "dirent.h"

char *getFileData_mustFree(const char *path, long int *fileSizeOut, AAssetManager *assetManager_p){

    AAsset *asset_p = AAssetManager_open(assetManager_p,path,AASSET_MODE_BUFFER);

    if(asset_p == NULL){
        printf("COULD NOT READ FILE: %s\n", path);
        return NULL;
    }

    long int fileSize = AAsset_getLength(asset_p);
    char *buffer = (char *)AAsset_getBuffer(asset_p);

    char *data = (char *)malloc(fileSize + 1);

    memcpy(data, buffer, fileSize);

    data[fileSize] = 0;

    *fileSizeOut = fileSize;

    AAsset_close(asset_p);

    return data;

    /*
	//printf("\nIN FILE READ\n");
	FILE *fileHandle = fopen(path, "rb");

	if(fileHandle == NULL){
		printf("COULD NOT READ FILE: %s\n", path);
		return NULL;
	}

	fseek(fileHandle, 0, SEEK_END);
	long fileSize = ftell(fileHandle);
	fseek(fileHandle, 0, SEEK_SET);

	char *data = (char *)malloc(fileSize + 1);
	fread(data, fileSize, 1, fileHandle);
	fclose(fileHandle);

	data[fileSize] = 0;

	*fileSizeOut = fileSize;


	return data;
     */

}

String *getFileLines_mustFree(const char *path, int *numberOfLines_out, AAssetManager *assetManager_p){

	long int dataSize;

	char *data = getFileData_mustFree(path, &dataSize, assetManager_p);
	
	int numberOfLines = 1;
	for(int i = 0; i < dataSize; i++){
		if(data[i] == *"\n"){
			numberOfLines++;
		}
	}

	String *lines = (String *)malloc(numberOfLines * sizeof(String));
	memset(lines, 0, numberOfLines * sizeof(String));

	int currentLine = 0;
	int currentChar = 0;
	for(int i = 0; i < dataSize; i++){

		if(data[i] == *"\n"){
			if(lines[currentLine][currentChar - 1] == *"\r"){
				lines[currentLine][currentChar - 1] = *"\0";
			}
			currentLine++;
			currentChar = 0;
		}else{
			if(currentChar < STRING_SIZE){
				lines[currentLine][currentChar] = data[i];
			}
			currentChar++;
		}

	}

	free(data);

	*numberOfLines_out = numberOfLines;

	return lines;

}

void writeDataToFile(const char *path, char *data_p, long int fileSize){

	FILE *fileHandle = NULL;

	fileHandle = fopen(path, "w");

	for(int i = 0; i < fileSize; i++){
		fputc(data_p[i], fileHandle);
	}
	
	fclose(fileHandle);

}

String *getDirectoryFileNames_mustFree(const char *path, int *output_n_files){

	DIR *dataDir = opendir(path);
	struct dirent* dirEntry;

	if(dataDir == NULL){
		printf("Could not read directory %s\n", path);
		return NULL;
	}

	int n_files = 0;

	while((dirEntry = readdir(dataDir)) != NULL){
		if(strcmp(dirEntry->d_name, ".") != 0
		&& strcmp(dirEntry->d_name, "..") != 0){
			n_files++;
		}
	}
	
	closedir(dataDir);

	String *fileNames = (String *)malloc(n_files * sizeof(String));

	dataDir = opendir(path);

	int n = 0;

	while((dirEntry = readdir(dataDir)) != NULL){
		if(strcmp(dirEntry->d_name, ".") != 0
		&& strcmp(dirEntry->d_name, "..") != 0){
			fileNames[n] = string(dirEntry->d_name);
			n++;
		}
	}

	closedir(dataDir);

	*output_n_files = n_files;

	return fileNames;

}
