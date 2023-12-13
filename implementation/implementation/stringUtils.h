#ifndef STRINGUTILS_H
#define STRINGUTILS_H
#include <string.h>
#include <string>
#include <ctype.h>

const int BUFFER_SIZE = 1024;

bool checkNullTerminated(char string[]) {
    int i = 0;
    while (i < BUFFER_SIZE && string[i] != '\0')
        i++;
    return (string[i] == '\0');
}

bool safeCmp(char string1[], const char string2[]) {
    if (strlen(string1) != strlen(string2))
        return false;
    return (strncmp(string1, string2, strlen(string1)) == 0);
}

bool checkFileFormat(char filepath[]) {
    char filepathCopy[BUFFER_SIZE];
    strcpy_s(filepathCopy, sizeof(filepathCopy), filepath);
    const char* dot = ".";
    char* nextToken = NULL;
    char* fileExtension = strtok_s(filepathCopy, dot, &nextToken);
    fileExtension = strtok_s(nullptr, dot, &nextToken);

    if (fileExtension == nullptr)
        return false;
    for (int i = 0; i < strlen(fileExtension); i++)
        if (!islower(fileExtension[i]))
            return false;
    return true;
}


bool checkCredentials(char string[]) {
    for (int i = 0; i < strlen(string); i++)
        if (!isalnum(string[i]) && string[i] != ';')
            return false;
    return true;
}
bool checkInput(char string[]) {
    for (int i = 0; i < strlen(string); i++) 
        if (!isalnum(string[i]) && string[i] != ' ' && string[i] != '.' && string[i] != '\\')
            return false;
    return true;
}

/*void escapeBackslash(char string[]) {
    char filepath[BUFFER_SIZE];
    char ans[BUFFER_SIZE];
    strcpy_s(filepath, sizeof(filepath), string);
    const char* backslash = "\\";
    char* nextToken = NULL;
    char* part = strtok_s(filepath, dot, &nextToken);

    while (part != nullptr) {
        part = strtok_s(nullptr, dot, &nextToken);
    }
}*/
#endif