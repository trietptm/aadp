// $Id$

/*
 Anti-Anti-Debugging Plugin for Ollydbg v0.3 - LGPL 3.0

 Copyright (C) 2012 +NCR/CRC! [ReVeRsEr] http://crackinglandia.blogspot.com

 This library is free software: you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation, either
 version 3 of the License, or any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library.  If not, see <http://www.gnu.org/licenses/>.

 Author: Nahuel C. Riva
 Date: -
 @: nahuelriva@gmail.com
 Blog: http://crackinglandia.blogspot.com
 Twitter: @crackinglandia
*/

#ifndef __AADPLIB_H__
#define __AADPLIB_H__

#pragma comment(lib, "aadlib\\aadlib-v0.2-win32.lib")

// Functions that do the magic ...
BOOL __stdcall hd_GetTickCount(DWORD pid);
BOOL __stdcall hd_HeapFlagsOnPEB(DWORD pid);
BOOL __stdcall hd_NtGlobalFlags(DWORD pid);
BOOL __stdcall hd_IsDebuggerPresent(DWORD pid);
BOOL __stdcall hd_HookZwQueryInformationProcess(DWORD pid);
BOOL __stdcall hd_HookZwSetInformationThread(DWORD pid);
BOOL __stdcall hd_HookOutputDebugString(DWORD pid);
BOOL __stdcall hd_ZwQueryObject(DWORD pid);
BOOL __stdcall hd_ZwOpenProcess(DWORD DebugeePid, DWORD DbgPid);
BOOL __stdcall hd_Module32Next(DWORD pid);
BOOL __stdcall hd_Process32Next(DWORD pid);
BOOL __stdcall hd_BlockInput(DWORD pid);
BOOL __stdcall hd_SuspendThread(DWORD pid);
BOOL __stdcall hd_TerminateProcess(DWORD pid);
BOOL __stdcall hd_FindWindow(HWND hWnd, char* lpString, DWORD pid);
BOOL __stdcall hd_UnhandledExceptionFilter(DWORD pid);
BOOL __stdcall hd_ZwQuerySystemInformation(DWORD pid);

// Auxiliary functions
HANDLE __stdcall hd_GetProcHandleByPid(DWORD pid);
DWORD __stdcall hd_GetProcPidByHandle(HANDLE pHandle);
void* __stdcall hd_GetPEBAddress(DWORD pid);
BOOL __stdcall hd_DllInjector(DWORD ProcessId, char* DLLLibPath, bool WaitForResponse);
#endif
