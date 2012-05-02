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
*/

#ifndef _WINDOWS_
#include <windows.h>
#endif

#ifndef _INC_WINDOWSX
#include <Windowsx.h>
#endif

#ifndef __AADPGLOBALS_H__
#include "aadpglobals.h"
#endif

#ifndef __AADPCONSTANTS_H__
#include "aadpconstants.h"
#endif

#ifndef __AADPRESOURCES_H__
#include "resources\\resource.h"
#endif

#ifndef __OLLYHEADER_H__
#include "external\Plugin.h"
#endif

#ifndef __HELPERS_H__
#define __HELPERS_H__
#pragma once

/* 
	When linking, the definition of this variable gives LNK2005. To fix this
	/FORCE:MULTIPLE was added in the linker options.
	See http://msdn.microsoft.com/en-us/library/72zdcz6f%28v=vs.71%29.aspx
*/

void WriteAadpConfig(char* ConfigName);
void _InitGlobalArray(PAADPTRICK tricks, int arraySize, int sourceArrayIds[], char* sourceArrayNames[]);
void InitGlobalArrays(void);
void SetRadioBtOption(HWND hWin, int PatchType);
void EnableRadioButtons(HWND hWin, int State);
void _CheckForOptions(HWND hWin, HMODULE hModule, int arraySize, PAADPTRICK tricks);
void CheckForOptions(HWND hWin, int Tab);
void SetOptions(HWND hWin, int Tab);
void UI_CheckAllOptions(HWND hWin, int State, int Tab);
void DestroyGlobalHandles(void);
void HandleChecks(HWND hWin, WPARAM wParam, int Tab);
void GetRadioBtHandles(HWND hWin);
void UI_UpdateCheckBoxes(int Tab);
void ReadAadpConfig(char* ConfigName);
void aadp_ListBox_AddData(HWND hListBox, char* Data);

int CheckForBSTChecked(HWND hw, DWORD ID, char* Key);
int GetControlIdInArray(int ControlId, PAADPTRICK TricksArray, int ArraySize);
int Static_SetTxt(HWND hWin, int StaticId, char* szText);
int aadp_DeleteSectionFromIni(char* szAux, char* PathToIni);
int aadp_GetPathToIniFile(char* Out_PathToIni, int BuffLen);
int aadp_Readintfromini(char* ConfigName, char* Key, int DefValue);
int aadp_Readstringfromini(char* ConfigName, char* Key, char* OutputString, int OutputStringSize, char* DefString);
int aadp_Writeinttoini(char* ConfigName, char* Key, int Value);
int aadp_GetSectionNamesInIniFile(char* PathToIni, void* pBuffer, int BufferSize);

bool IsRadioButtonChecked(HWND hRadioBt);
bool _AreAllOptionsEnabled(PAADPTRICK tricks, int arraySize);
bool AreAllOptionsEnabled(int Tab);
bool aadp_SetConfigToFalse(char* ConfigToSet);
bool aadp_SetConfigAsCurrent(char* ConfigToSetAsCurrent);
bool aadp_GetCurrentConfig(char* OutConfigBuffer, int OutConfigBufferLen);

HANDLE aadp_CreateConfigFile(HMODULE hMod);
#endif