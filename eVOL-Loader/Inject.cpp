//===============================================================================================//
// Copyright (c) 2012, Stephen Fewer of Harmony Security (www.harmonysecurity.com)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are permitted 
// provided that the following conditions are met:
// 
//     * Redistributions of source code must retain the above copyright notice, this list of 
// conditions and the following disclaimer.
// 
//     * Redistributions in binary form must reproduce the above copyright notice, this list of 
// conditions and the following disclaimer in the documentation and/or other materials provided 
// with the distribution.
// 
//     * Neither the name of Harmony Security nor the names of its contributors may be used to
// endorse or promote products derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.
//===============================================================================================//
//#define WIN32_LEAN_AND_MEAN
#include <cstdio>
#include <cstdlib>
#include <Windows.h>
#include <TlHelp32.h>
#include "LoadLibraryR.h"

#pragma comment(lib,"Advapi32.lib")

#define BREAK_WITH_ERROR( e ) { printf( "[-] %s. Error=%d", e, GetLastError() ); break; }

unsigned int GetProcessPID(const char* process_name)
{
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	unsigned int count = 0;
	unsigned int pid = 0;

	if (snap == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	if (!WaitForSingleObject(snap, 0) == WAIT_TIMEOUT)
	{
		return 0;
	}

	PROCESSENTRY32 proc;
	proc.dwSize = sizeof(PROCESSENTRY32);
	BOOL ret = Process32Next(snap, &proc);

	while (ret)
	{
		if (!_stricmp(proc.szExeFile, process_name))
		{
			count++;
			pid = proc.th32ProcessID;
		}
		ret = Process32Next(snap, &proc);
	}

	if (count > 1)
	{
		pid = 0;
	}

	CloseHandle(snap);

	return pid;
}

// Simple app to inject a reflective DLL into a process vis its process ID.
int main(int argc, char* argv[])
{
	HANDLE hFile = NULL;
	HANDLE hModule = NULL;
	HANDLE hProcess = NULL;
	HANDLE hToken = NULL;
	LPVOID lpBuffer = NULL;
	LPVOID lpRemotePathBuffer = NULL;
	CHAR szCurrentDirectory[MAX_PATH];
	DWORD dwLength = 0;
	DWORD dwBytesRead = 0;
	DWORD dwProcessId = 0;
	TOKEN_PRIVILEGES priv = { 0 };

#ifdef _X64_
	const char* cpDllFile = "eVOL-2022.x64.dll";
#else
#ifdef _X86_
	const char* cpDllFile = "eVOL-2022.dll";
#else _ARM_
	const char* cpDllFile = "eVOL-2022.arm.dll";
#endif
#endif

	do
	{
		dwProcessId = GetProcessPID("hl.exe");
		if (!dwProcessId)
			BREAK_WITH_ERROR("Failed to find hl.exe");

		hFile = CreateFileA(cpDllFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			BREAK_WITH_ERROR("Failed to open the DLL file");

		dwLength = GetFileSize(hFile, NULL);
		if (dwLength == INVALID_FILE_SIZE || dwLength == 0)
			BREAK_WITH_ERROR("Failed to get the DLL file size");

		lpBuffer = HeapAlloc(GetProcessHeap(), 0, dwLength);
		if (!lpBuffer)
			BREAK_WITH_ERROR("Failed to get the DLL file size");

		if (ReadFile(hFile, lpBuffer, dwLength, &dwBytesRead, NULL) == FALSE)
			BREAK_WITH_ERROR("Failed to alloc a buffer!");

		if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		{
			priv.PrivilegeCount = 1;
			priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &priv.Privileges[0].Luid))
				AdjustTokenPrivileges(hToken, FALSE, &priv, 0, NULL, NULL);

			CloseHandle(hToken);
		}

		hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, dwProcessId);
		if (!hProcess)
			BREAK_WITH_ERROR("Failed to open the target process");

		lpRemotePathBuffer = VirtualAllocEx(hProcess, NULL, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (!lpRemotePathBuffer)
			BREAK_WITH_ERROR("Failed to allocate dll directory");

		RtlZeroMemory(szCurrentDirectory, MAX_PATH);

		if (!GetCurrentDirectory(MAX_PATH, szCurrentDirectory))
			BREAK_WITH_ERROR("Failed to locate the process path");

		strcat_s(szCurrentDirectory, MAX_PATH - 1, "\\");

		if (!WriteProcessMemory(hProcess, lpRemotePathBuffer, szCurrentDirectory, MAX_PATH - 1, NULL))
			BREAK_WITH_ERROR("Failed to write the current directory path");

		hModule = LoadRemoteLibraryR(hProcess, lpBuffer, dwLength, lpRemotePathBuffer);
		if (!hModule)
			BREAK_WITH_ERROR("Failed to inject the DLL");

		printf("[+] Injected the '%s' DLL into process %d.", cpDllFile, dwProcessId);

		WaitForSingleObject(hModule, -1);

	} while (0);

	if (lpBuffer)
		HeapFree(GetProcessHeap(), 0, lpBuffer);

	if (hProcess)
		CloseHandle(hProcess);

	return 0;
}