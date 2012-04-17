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

/* Function prototypes */
INT_PTR CALLBACK AadbgTricksDlgTabHandler(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK OllyFixesDlgHandlerTabHandler(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK SettingsDlgTabHandler(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK CustomHideSettingsDlgTabHandler(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK AboutDlgHandler(HWND, UINT, WPARAM, LPARAM);

/* Global variables declaration */
HWND hRadioNoCounter = NULL, hCounterPlusOne = NULL, hRandomCounter = NULL;

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

HWND hConfigLv;