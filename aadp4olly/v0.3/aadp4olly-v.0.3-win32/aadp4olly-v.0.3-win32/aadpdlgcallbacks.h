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

#include <tchar.h>
#include <Strsafe.h>
#include <Commctrl.h>

#pragma comment(lib, "comctl32.lib")

#ifndef TABS_H
#include "external\tabs.h"
#endif

#ifndef __AADPDLGCALLBACKS_H__
#define __AADPDLGCALLBACKS_H__

INT_PTR CALLBACK CustomHideSettingsDlgTabHandler(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK AadbgTricksDlgTabHandler(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK OllyFixesDlgHandlerTabHandler(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK AboutDlgHandler(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK SettingsDlgTabHandler(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK aadp4Ollyproc(HWND hWin,UINT msg,WPARAM wp,LPARAM lp);

#endif