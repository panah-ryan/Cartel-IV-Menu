#pragma once
#include "natives.h"

void writeToTextFile(char* filePath, char* title, char* fmt, ...);
std::string GetLineFromFile(const char* filePath, int lineNumber);
int CountLinesInFile(const char* filePath);

void CTL_GET_LINES_IN_FILE(scrThread_Info* cxt); //works
void CTL_READ_LINE_FROM_TEXT_FILE(scrThread_Info* cxt); //works
void CTL_WRITE_TO_FILE(scrThread_Info* cxt);

void CTL_STRING_TO_FLOAT(scrThread_Info* cxt);
void CTL_FLOAT_TO_STRING(scrThread_Info* cxt);

void CTL_OPEN_FILE_FOR_READING(scrThread_Info* cxt); //tempramental, skips some lines for some reason.
void CTL_READ_LINE_FROM_OPENED_FILE(scrThread_Info* cxt);
void CTL_CLOSE_READING_FILE(scrThread_Info* cxt);

void CTL_OPEN_FILE_FOR_WRITING(scrThread_Info* cxt);
void CTL_WRITE_LINE_TO_OPENED_FILE(scrThread_Info* cxt);
void CTL_CLOSE_OPENED_WRITING_FILE(scrThread_Info* cxt);

void CTL_GET_SPAWNER_MEMORY(scrThread_Info* cxt);

void CTL_GET_FILES_IN_DIRECTORY(scrThread_Info* cxt); //bool CTL_GET_FILES_IN_DIRECTORY(char* path, char* extension, int *count) //works
void CTL_GET_FILES_IN_DIRECTORY_ENTRY(scrThread_Info* cxt);
void CTL_DELETE_FILE(scrThread_Info* cxt); //allows deletion of files inside GAME:\\Cartel directory