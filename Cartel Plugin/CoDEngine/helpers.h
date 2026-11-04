#pragma once

#include "stdafx.h"

#ifdef _DEBUG
void DebugPrint(const char* fmt, ...);
#define Log(fmt, ...) DebugPrint(fmt, ##__VA_ARGS__);
#else
#define Log(...)
#endif

BOOL CReadFile(PCHAR File, byte** Out, PDWORD pLength);
//BOOL CReadFile(PCHAR File, byte* Out, PDWORD pLength);
BOOL CWriteFile(PCHAR File, PBYTE Buffer, DWORD Length);
void XNotify(wchar_t *Message);
void XamShowKeyboardUI(LPCWSTR titleText, LPCWSTR descriptionText, int* error, char* out, int length, LPCWSTR defText = L"");
DWORD XUserFindUser(XUID qwXuid, int dwIndex, char* szGamertag, int dwGamertagLen, XUID* qwOutXuid, int r8);

bool DoesFileExist(const char* pFilename, DWORD* pSize);

// Reports which pages still exist without allocating anything, so it is safe to
// call after the title has unloaded (MmIsAddressValid only reads page tables).
void DumpMemValidity(const char* when);

// Hash a file without ever holding it in memory. CReadFile mallocs the whole file;
// for a ~500 KB .sco that forces the heap to take a fresh segment from the system,
// and mid-session those are title pages that get unmapped when the title unloads -
// leaving a dangling entry on the free list that kills the next boot. These read in
// 64 KB chunks through a VirtualAlloc'd buffer, so the CRT heap is never involved.
BOOL CHashFileSha(PCHAR File, byte* out, DWORD outLen);
BOOL CHashFileMd5(PCHAR File, byte* out, DWORD outLen);