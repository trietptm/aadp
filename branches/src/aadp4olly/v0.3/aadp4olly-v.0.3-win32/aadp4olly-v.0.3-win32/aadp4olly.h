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

#include <windows.h>
#include <Commctrl.h>
#include <tchar.h>
#include <Strsafe.h>
#include <Windowsx.h>

#include "resources\\resource.h"
#include "external\\plugin.h"
#include "external\\tabs.h"
#include "aadlib\\aadlib-v0.2-win32.h"

/* Additional libraries declaration */
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "external\\OLLYDBG.LIB")
#pragma comment(lib, "aadlib\\aadlib-v0.2-win32.lib")

/* Constants declaration */
#define VERSIONHI		1
#define VERSIONLO		10
#define HIGHLIGHTED		1
#define GRAYED			-1
#define STANDARD		0

#define TABAADBTRICKS		11
#define TABOLLYFIXES		12
#define TABADVSETTINGS		13
#define TABCUSTOMSETTINGS	14

#define SIZEAADBTRICKSARRAY 18
#define SIZEOLLYFIXESARRAY	5
#define SIZEADVSETTINGSARRAY 12

#define NOCOUNTER 1
#define COUNTERPLUSONE 2
#define RANDOMCOUNTER 3

typedef struct _AADPTRICK 
{
	char functionName[MAX_PATH];
	int functionId;
	bool functionState;
	void* Reserved;
} AADPTRICK, *PAADPTRICK;

typedef struct _GETTICKCOUNTOPTION
{
	int GetTickCountPatchType;
} GETTICKCOUNTOPTION, *PGETTICKCOUNTOPTION;

/* Function prototypes */
INT_PTR CALLBACK AadbgTricksDlgTabHandler(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK OllyFixesDlgHandlerTabHandler(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK SettingsDlgTabHandler(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK CustomHideSettingsDlgTabHandler(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK AboutDlgHandler(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam);

/* Global variables declaration */
HWND hRadioNoCounter = NULL, hCounterPlusOne = NULL, hRandomCounter = NULL, hConfigLb = NULL;
HWND MainTabDlgHwnd, AadbgTricksDlgHwnd, OllyFixesDlgHwnd, SettingsDlgHwnd, CustomSettingsDlgHwnd, AboutDlgHwnd;

int TabAadbTricksControlsId[SIZEAADBTRICKSARRAY] = {CHECK_RDTSC, CHECK_BEINGDEBUGGED, CHECK_FORCEFLAGS, CHECK_SETINFOTHREAD,
CHECK_ZWQUERYINFOPROCESS, CHECK_NTGLOBALFLAGS, CHECK_HEAPFLAGS, CHECK_OUTPUTDEBUGSTRING, 
CHECK_ZWQUERYOBJECT, CHECK_ZWOPENPROCESS, CHECK_FINDWINDOW, CHECK_UNHANDLEDEXCEPTIONFILTER, 
CHECK_SUSPENDTHREAD, CHECK_BLOCKINPUT, CHECK_TERMINATEPROCESS, CHECK_PROCESS32NEXT, CHECK_MODULE32NEXT, 
CHECK_ZWQUERYSYSINFO};

int TabOllyFixesControlsId[SIZEOLLYFIXESARRAY] = {CHECK_FIXFLDZ, CHECK_FIXFUISTQ, CHECK_FIXFORMATSTRING,
CHECK_FIXNUMBEROFRVA, CHECK_FIXANALYSISCRASH};

int TabAdvSettingsControlsId[SIZEADVSETTINGSARRAY] = {CHECK_CTRLG, CHECK_EPOUTSIDECODE, CHECK_MOREPATCHES, 
CHECK_COMPRESSEDCODE, CHECK_LOADDLLWARNING, CHECK_SINGLESTEPCOMPATIBILITY, CHECK_MAXIMIZEOLLY, 
CHECK_MAXIMIZECHILDS, CHECK_SKIPEXCEPTIONS, CHECK_REALTIMEPATCHING, CHECK_BREAKONTLS, CHECK_ANTIANTIATTACH};

char* TabAadbtricksFuncNames[SIZEAADBTRICKSARRAY] = {"hd_Rdtsc", "hd_IsDebuggerPresent", "hd_ForceFlags", "hd_ZwSetInformationThread", 
"hd_ZwQueryInformationProcess", "hd_NtGlobalFlags", "hd_HeapFlags", "hd_OutputDebugString", "hd_ZwQueryObject", "hd_ZwOpenProcess", 
"hd_FindWindow", "hd_UnhandledExceptionFilter", "hd_SuspendThread", "hd_BlockInput", "hd_TerminateProcess", "hd_Process32Next", 
"hd_Module32Next", "hd_QuerySystemInformation"};

char* TabOllyFixesFuncNames[SIZEOLLYFIXESARRAY] = {"hd_FixFldz", "hd_FixFuistq", "hd_FixNumberOfRvaAndSizes", "hd_FixFormatStringBug", "hd_FixAnalysisCrash"};

char* TabOllySettingsFuncNames[SIZEADVSETTINGSARRAY] = {"hd_EnableCtrlG", "hd_SkipEpOutsideCode", "hd_SkipMorePatches", 
"hd_SkipCompressedCode", "hd_SkipLoadDll", "hd_MaximizeOlly", "hd_MaximizeChilds", "hd_SkipSomeExceptions", "hd_EnableRealTimePatching", 
"hd_BreakOnTls", "hd_AntiAntiAttach", "hd_EnableSingleStep"};
