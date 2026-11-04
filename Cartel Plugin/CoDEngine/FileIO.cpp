#include "stdafx.h"
#include "FileIO.h"
#include "other.h"
#include <iostream>

struct FileReadHandler
{
	std::ifstream fileStream;
	int currentLine;
};

FileReadHandler readFileHandler;
char lineBuff[255]; //Global so we can take our time with it
byte SpawnerObjectArrayData[3600]; //We might want to move this to heap

void CTL_GET_SPAWNER_MEMORY(scrThread_Info* cxt)
{
	cxt->ResultPtr[0].Any = reinterpret_cast<DWORD>(&SpawnerObjectArrayData[0]);
}

std::string GetLineFromFile(const char* filePath, int lineNumber)
{
	std::ifstream readLog(filePath);  // Open the file for reading

	if (!readLog.is_open())
	{
		std::cerr << "Could not open the file for reading!" << std::endl;
		return "";  // Return an empty string to indicate an error
	}

	std::string line;
	int currentLine = 1;  // Start counting from the first line

	while (std::getline(readLog, line))
	{
		if (currentLine == lineNumber)
		{
			readLog.close();  // Close the file when done
			return line;  // Return the line as a string
		}
		currentLine++;
	}

	readLog.close();  // Close the file if line number is not found
	std::cerr << "Line " << lineNumber << " does not exist in the file!" << std::endl;
	return "";  // Return an empty string if line not found
}

void writeToTextFile(char* filePath, char* title, char* fmt, ...)
{
	SYSTEMTIME localTime;
	GetLocalTime(&localTime);

	char thiccTime[100];
	_snprintf(thiccTime, 100, "[%s %04i/%02i/%02i %02i:%02i] ", title, localTime.wYear, localTime.wMonth, localTime.wDay, localTime.wHour, localTime.wMinute);

	char buffer[1000];
	va_list argList;
	va_start(argList, fmt);
	vsnprintf(buffer, 1000, fmt, argList);
	va_end(argList);

	std::ofstream writeLog(filePath, std::ofstream::app);
	writeLog << thiccTime << buffer << std::endl;
	writeLog.close();

	printf("%s%s\n", thiccTime, buffer);
}

int CountLinesInFile(const char* filePath)
{
	std::ifstream readLog(filePath);  // Open the file for reading

	if (!readLog.is_open())
	{
		std::cerr << "Could not open the file for reading!" << std::endl;
		return -1;  // Return -1 to indicate an error
	}

	int lineCount = 0;
	std::string line;

	// Count lines in the file
	while (std::getline(readLog, line))
	{
		lineCount++;
	}

	readLog.close();
	return lineCount;
}

void CTL_GET_LINES_IN_FILE(scrThread_Info* cxt) //works
{
	const char* fileName = cxt->Params[0].String;

	char directory_buffer[128];
	_snprintf(directory_buffer, sizeof(directory_buffer), "GAME:\\Cartel\\%s", fileName);
	//_snprintf(directory_buffer, sizeof(directory_buffer), "%s\\%s", cartel_folder_path, fileName); 
	cxt->ResultPtr[0].Int = CountLinesInFile(directory_buffer);
}

void CTL_READ_LINE_FROM_TEXT_FILE(scrThread_Info* cxt) //works
{
	char directory_buffer[128];

	const char* fileName = cxt->Params[0].String;
	int line = cxt->Params[1].Int;
	_snprintf(directory_buffer, sizeof(directory_buffer), "GAME:\\Cartel\\%s", fileName); //CTL_WRITE_TO_TEXT_FILE("object_maps\\map_name.ini", dataBuff);
	//_snprintf(directory_buffer, sizeof(directory_buffer), "%s\\%s", cartel_folder_path, fileName); //CTL_WRITE_TO_TEXT_FILE("Maps\\map_name.ini", dataBuff);
	std::string LineString = GetLineFromFile(directory_buffer, line);
	strncpy(lineBuff, LineString.c_str(), 255);
	cxt->ResultPtr[0].String = lineBuff;
}

void CTL_WRITE_TO_FILE(scrThread_Info* cxt)
{
	const char* fileName = cxt->Params[0].String;
	char* data = cxt->Params[1].String;
	char directory_buffer[128];

	//CreateDirectory("GAME:\\Cartel", NULL); //could just run this once on menu init? ehhh.. more bulletproof this way.
//_snprintf(directory_buffer, sizeof(directory_buffer), "%s\\%s", cartel_folder_path, fileName); //CTL_WRITE_TO_TEXT_FILE("Maps\\map_name.ini", dataBuff);
	//printf("Trying to write to %s\n", directory_buffer);
	_snprintf(directory_buffer, sizeof(directory_buffer), "GAME:\\Cartel\\%s", fileName); //CTL_WRITE_TO_TEXT_FILE("Maps\\map_name.ini", dataBuff);
	writeToTextFile(directory_buffer, "CARTEL", data);
}

//better natives
char retBuff[16];
void CTL_FLOAT_TO_STRING(scrThread_Info* cxt)
{
	//char retBuff[16];
	_snprintf(retBuff, 16, "%f", cxt->Params[0].Float);
	cxt->ResultPtr[0].String = retBuff;
}

void CTL_STRING_TO_FLOAT(scrThread_Info* cxt)
{
	cxt->ResultPtr[0].Float = static_cast<float>(atof(cxt->Params[0].String));
}

void CTL_OPEN_FILE_FOR_READING(scrThread_Info* cxt) //tempramental, skips some lines for some reason.
{
	const char* fileName = cxt->Params[0].String;

	char directory_buffer[128];
	//_snprintf(directory_buffer, sizeof(directory_buffer), "%s\\%s", cartel_folder_path, fileName); 
	_snprintf(directory_buffer, sizeof(directory_buffer), "GAME:\\Cartel\\%s", fileName);

	readFileHandler.fileStream.open(directory_buffer);
	readFileHandler.currentLine = 0;  // Initialize line count to 0

	if (!readFileHandler.fileStream.is_open())
	{
		printf("[CARTEL::FileIO] Could not open %s for reading.\n", directory_buffer);
		readFileHandler.fileStream = std::ifstream();  // Invalid file stream
		cxt->ResultPtr[0].Bool = false;
	}
	else
	{
		cxt->ResultPtr[0].Bool = true;
	}
}

void CTL_READ_LINE_FROM_OPENED_FILE(scrThread_Info* cxt)
{
	if (!readFileHandler.fileStream.is_open())
	{
		strncpy(lineBuff, "ERROR: File not open.", 255);
		cxt->ResultPtr[0].String = lineBuff;
		return;
	}
	int lineTarget = cxt->Params[0].Int;
	/*if (readFileHandler.currentLine > lineTarget)
	{
		//Error out
	}*/

	std::string line;
	while (std::getline(readFileHandler.fileStream, line))
	{
		readFileHandler.currentLine++;
		if (readFileHandler.currentLine == lineTarget)
		{
			strncpy(lineBuff, line.c_str(), 255);
			cxt->ResultPtr[0].String = lineBuff;
			return;
		}
	}

	strncpy(lineBuff, "ERROR: Line not present.", 255);
	cxt->ResultPtr[0].String = lineBuff;
}

void CTL_CLOSE_READING_FILE(scrThread_Info* cxt) 
{
	if (readFileHandler.fileStream.is_open())
	{
		readFileHandler.fileStream.close();
		cxt->ResultPtr[0].Bool = true;
	}
	else
		cxt->ResultPtr[0].Bool = false;
}

std::ofstream g_openWriteFile;
bool g_writeFileIsOpen = false;
char g_writeFilePath[128];

void CTL_OPEN_FILE_FOR_WRITING(scrThread_Info* cxt)
{
	const char* fileName = cxt->Params[0].String;
	_snprintf(g_writeFilePath, sizeof(g_writeFilePath), "GAME:\\Cartel\\%s", fileName);

	g_openWriteFile.open(g_writeFilePath, std::ofstream::out); 
	if (g_openWriteFile.is_open())
	{
		g_writeFileIsOpen = true;
		printf("[CARTEL::FileIO] Opened file for writing: %s\n", g_writeFilePath);
	}
	else
	{
		g_writeFileIsOpen = false;
		printf("[CARTEL::FileIO] Failed to open file for writing: %s\n", g_writeFilePath);
	}
	cxt->ResultPtr[0].Bool = g_writeFileIsOpen;
}

void CTL_WRITE_LINE_TO_OPENED_FILE(scrThread_Info* cxt)
{
	if (!g_writeFileIsOpen)
	{
		printf("[CARTEL::FileIO] WRITE_LINE failed: No file is open.\n");
		cxt->ResultPtr[0].Bool = false;
		return;
	}
	const char* lineData = cxt->Params[0].String;
	g_openWriteFile << lineData << "\n";
	cxt->ResultPtr[0].Bool = true;
}

void CTL_CLOSE_OPENED_WRITING_FILE(scrThread_Info* cxt)
{
	if (g_writeFileIsOpen)
	{
		g_openWriteFile.close();
		g_writeFileIsOpen = false;
		printf("[CARTEL::FileIO] Closed writing file: %s\n", g_writeFilePath);
	}
	else
	{
		printf("[CARTEL::FileIO] CLOSE_WRITE_FILE called, but no file was open.\n");
	}
	cxt->ResultPtr[0].Bool = !g_writeFileIsOpen; //return true if file is closed (success)
}


void CTL_DELETE_FILE(scrThread_Info * cxt)
{
	const char* path = cxt->Params[0].String;

	if (!path)
	{
		cxt->ResultPtr[0].Bool = FALSE;
		return;
	}

	const char* base = "GAME:\\Cartel\\";

	// Must start with base path
	if (strncmp(path, base, strlen(base)) != 0)
	{
		printf("[CARTEL::FileIO] DELETE_FILE: rejected (bad prefix): %s\n", path);
		cxt->ResultPtr[0].Bool = FALSE;
		return;
	}

	// Block traversal
	if (strstr(path, ".."))
	{
		printf("[CARTEL::FileIO] DELETE_FILE: rejected (path traversal): %s\n", path);
		cxt->ResultPtr[0].Bool = FALSE;
		return;
	}

	// Optional: block forward slashes or other weirdness
	if (strchr(path, '/'))
	{
		cxt->ResultPtr[0].Bool = FALSE;
		return;
	}

	cxt->ResultPtr[0].Bool = DeleteFile(path);
}


std::vector<std::string> g_FileSerachResults;

std::vector<std::string> GetFilesInDirectory(char* directory, char* extension, int FILE_ATTRIBUTE)
{
	//std::vector<std::string> files;
	WIN32_FIND_DATAA findData;

	//Clear previous results
	g_FileSerachResults.clear();

	// Build search path with extension
	std::string searchPath = std::string(directory) + "\\*" + extension;

	HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("[CARTEL::FileIO] No files found in directory: %s with extension %s\n", directory, extension);
		return g_FileSerachResults;
	}

	do
	{
		if (findData.dwFileAttributes & FILE_ATTRIBUTE) //was if((!findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
		{
			g_FileSerachResults.push_back(findData.cFileName);
		}
	} while (FindNextFile(hFind, &findData));

	FindClose(hFind);
	return g_FileSerachResults; //yes this is global as the menu will keep referencing it.
}


char last_scanned_files_path[255];
void CTL_GET_FILES_IN_DIRECTORY(scrThread_Info* cxt) //bool CTL_GET_FILES_IN_DIRECTORY(bool folders_instead, char* path, char* extension, int *count) //works well
{
	BOOL folders_instead = cxt->Params[0].Bool;

	char path[256];
	strncpy(path, cxt->Params[1].String, sizeof(path));

	char extension[8];
	strncpy(extension, cxt->Params[2].String, sizeof(extension));

	scrValue* fileCount = cxt->Params[3].Reference;

	if (strncmp(path, last_scanned_files_path, 255) == 0) //stop it reloading
	{
		fileCount->Int = static_cast<int>(g_FileSerachResults.size());
		cxt->ResultPtr[0].Bool = TRUE;
#ifdef Moderator
		printf("[CARTEL::FileIO] Blocking reloading of path %s\n", path);
#endif
		return;
	}
	else
		strncpy(last_scanned_files_path, path, 255);

	//printf("CTL_GET_FILES_IN_DIRECTORY: path == %s\n", path);
	//printf("CTL_GET_FILES_IN_DIRECTORY: extension == %s\n", extension);


	std::vector<std::string> extFiles = GetFilesInDirectory(path, extension, folders_instead ? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_NORMAL); //FILE_ATTRIBUTE_DIRECTORY

	if (extFiles.empty())
	{
		printf("[CARTEL::FileIO] extFiles.empty() = TRUE\n");
		fileCount->Int = 0;
		cxt->ResultPtr[0].Bool = FALSE;
		return;
	}

	// Debug print
	/*printf("%s files found (%i):\n", extension, extFiles.size());
	for (size_t i = 0; i < extFiles.size(); ++i)
	{
		printf("%s\n", extFiles[i].c_str());
	}
	*/
	fileCount->Int = static_cast<int>(extFiles.size());
	cxt->ResultPtr[0].Bool = TRUE;
}

void CTL_GET_FILES_IN_DIRECTORY_ENTRY(scrThread_Info* cxt) //CTL_GET_FILES_IN_DIRECTORY_ENTRY(int entry_to_get, char** entry);
{
	int entry_to_get = cxt->Params[0].Int;

	if (entry_to_get < 0 || entry_to_get >= g_FileSerachResults.size())
	{
		cxt->ResultPtr[0].Bool = FALSE;
		return;
	}

	scrValue* entry_title = cxt->Params[1].Reference;

	auto& entry = g_FileSerachResults[entry_to_get];
	entry_title->String = const_cast<char*>(entry.c_str());
	cxt->ResultPtr[0].Bool = TRUE;
}
