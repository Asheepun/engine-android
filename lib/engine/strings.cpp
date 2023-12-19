#include "engine/strings.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

String string(const char *inputString){
	String newString;
	String_set(newString, inputString, STRING_SIZE);
	return newString;
}

void String_log(SmallString string){
	printf("%s\n", (char *)string);
}

void String_set(char *destString, const char *sourceString, int size){

	memset(destString, 0, size);

	if(strlen(sourceString) >= size){
		printf("SOURCE STRING TO BIG!\n");
		printf("%s\n", sourceString);
	}

	memcpy(destString, sourceString, strlen(sourceString));
	
}

void String_append(char *baseString, const char *appendString){

	memcpy(baseString + strlen(baseString), appendString, strlen(appendString));

}

void String_append_char(char *baseString, char character){
	
	baseString[strlen(baseString) + 1] = *"\0";
	baseString[strlen(baseString)] = character;
	//memcpy(baseString + strlen(baseString), appendString, strlen(appendString));

}

void String_append_int(char *baseString, int value){

	char buff[STRING_SIZE];
	memset(buff, 0, STRING_SIZE);

	sprintf(buff, "%i", value);

	memcpy(baseString + strlen(baseString), buff, strlen(buff));

}

void String_append_float(char *baseString, float value){

	char buff[STRING_SIZE];
	memset(buff, 0, STRING_SIZE);

	sprintf(buff, "%f", value);

	memcpy(baseString + strlen(baseString), buff, strlen(buff));

}

void String_clearRange(char *string, int startIndex, int stopIndex){

	memset(string + startIndex, 0, stopIndex - startIndex);

}
