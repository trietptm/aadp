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

#ifndef _WINDOWS_H_
#include <windows.h>
#endif

#ifndef __AADPCONSTANTS_H__
#include "aadpconstants.h"
#endif

#ifndef __AADPGLOBALS_H__
#include "aadpglobals.h"
#endif

#ifndef __HELPERS_H__
#include "helpers.h"
#endif

#include "aadpdlgcallbacks.h"

LRESULT CALLBACK aadp4Ollyproc(HWND hWin,UINT msg,WPARAM wp,LPARAM lp)
{
  hwPluginWin = hWin;
  int Index;

  switch(msg)
  {
  case WM_INITDIALOG:

	AadbgTricksDlgHwnd = CreateDialog(hinst, MAKEINTRESOURCE(ANTIDBGTRICKS), hWin, &AadbgTricksDlgTabHandler);
	OllyFixesDlgHwnd = CreateDialog(hinst, MAKEINTRESOURCE(OLLYFIXES), hWin, &OllyFixesDlgHandlerTabHandler);
	SettingsDlgHwnd = CreateDialog(hinst, MAKEINTRESOURCE(ADVANCEDSETTINGS), hWin, &SettingsDlgTabHandler);
	CustomSettingsDlgHwnd = CreateDialog(hinst, MAKEINTRESOURCE(CUSTOMHIDESETTINGSDLG), hWin, &CustomHideSettingsDlgTabHandler);
	AboutDlgHwnd = CreateDialog(hinst, MAKEINTRESOURCE(ABOUT), hWin, &AboutDlgHandler);

	MainTabDlgHwnd = GetDlgItem(hWin, IDC_TAB1);

	// Add tab to existing ones (currently index 0)
	AddTab(MainTabDlgHwnd, CustomSettingsDlgHwnd, "Custom Hide Settings", 3);
	
	// Add tab at first position
	Index = AddTab(MainTabDlgHwnd, AadbgTricksDlgHwnd, "Anti-Anti-Debugger tricks", 0);
	
	// Add tab at position 2
	AddTab(MainTabDlgHwnd, OllyFixesDlgHwnd, "Ollydbg fixes", 1);
	AddTab(MainTabDlgHwnd, SettingsDlgHwnd, "Advanced Settings", 2);
	AddTab(MainTabDlgHwnd, AboutDlgHwnd, "About aadp4olly", -1);

	// Bring window of tab1 to front
	TabToFront(MainTabDlgHwnd, Index);
	
	return 1;

  case WM_NOTIFY:
	switch(((NMHDR *)lp)->code)
	{
		case TCN_SELCHANGE: // Get currently selected tab window to front
			TabToFront(MainTabDlgHwnd, -1);
			break;

		default: return false;
	}
	break;

  case WM_COMMAND:
    switch(wp)
    {
    case IDOK:
		SetOptions(AadbgTricksDlgHwnd, TABAADBTRICKS);
		SetOptions(OllyFixesDlgHwnd, TABOLLYFIXES);
		SetOptions(SettingsDlgHwnd, TABADVSETTINGS);

		EndDialog(hWin, 0);
		//return 0;

    case IDCANCEL:
		// Tab clean-up
		TabCleanup(MainTabDlgHwnd);

		// Destroy windows handles
		DestroyWindow(AadbgTricksDlgHwnd);
		DestroyWindow(OllyFixesDlgHwnd);
		DestroyWindow(SettingsDlgHwnd);
		DestroyWindow(CustomSettingsDlgHwnd);
		DestroyWindow(AboutDlgHwnd);

		// Destroy all global handles
		//DestroyGlobalHandles();

		// Destroy the modal dialog
		EndDialog(hWin, 0);
    }
  }
  return 0;
}

INT_PTR CALLBACK OllyFixesDlgHandlerTabHandler(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int State;

	switch(uMsg)
	{
		case WM_INITDIALOG:
			CheckForOptions(hWin, TABOLLYFIXES);

			if(AreAllOptionsEnabled(TABOLLYFIXES))
				CheckDlgButton(hWin, CHECK_TABFIXES_SELECTALL, BST_CHECKED);
			else
				CheckDlgButton(hWin, CHECK_TABFIXES_SELECTALL, BST_UNCHECKED);

			return 1;

		case WM_COMMAND:
			/* Fix: HandleChecks() was returning -1 because it receives an ID not found in the 
			array. The ID was from the select all checkbox. So, there was an integrer overflow indexing
			the array and overwriting the hWin variable. Due this, I moved the HandleChecks() call to the default
			case in the switch. Thanks to marciano for his help with this fix.
			*/
			//HandleChecks(hWin, wParam, TABOLLYFIXES);

			switch(LOWORD(wParam))
			{
				case CHECK_TABFIXES_SELECTALL:
					State = IsDlgButtonChecked(hWin, CHECK_TABFIXES_SELECTALL);
					UI_CheckAllOptions(hWin, State, TABOLLYFIXES);
					break;

				default: HandleChecks(hWin, wParam, TABOLLYFIXES);
					break;
			}
			break;

		case WM_DESTROY:
			break;

		default:
			return false;
	}

	return true;
}

INT_PTR CALLBACK AboutDlgHandler(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_COMMAND:
			break;

		case WM_DESTROY:
			break;

		default:
			return false;
	}

	return true;
}

INT_PTR CALLBACK SettingsDlgTabHandler(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int State;

	switch(uMsg)
	{
		case WM_INITDIALOG:
			CheckForOptions(hWin, TABADVSETTINGS);

			if(AreAllOptionsEnabled(TABADVSETTINGS))
				CheckDlgButton(hWin, CHECK_TABADVSETTINGS_SELECTALL, BST_CHECKED);
			else
				CheckDlgButton(hWin, CHECK_TABADVSETTINGS_SELECTALL, BST_UNCHECKED);

			return 1;

		case WM_COMMAND:
			/* Fix: HandleChecks() was returning -1 because it receives an ID not found in the 
			array. The ID was from the select all checkbox. So, there was an integrer overflow indexing
			the array and overwriting the hWin variable. Due this, I moved the HandleChecks() call to the default
			case in the switch. Thanks to marciano for his help with this fix.
			*/
			//HandleChecks(hWin, wParam, TABADVSETTINGS);

			switch(LOWORD(wParam))
			{
				case CHECK_TABADVSETTINGS_SELECTALL:
					State = IsDlgButtonChecked(hWin, CHECK_TABADVSETTINGS_SELECTALL);
					UI_CheckAllOptions(hWin, State, TABADVSETTINGS);
					break;

				default: HandleChecks(hWin, wParam, TABADVSETTINGS); 
					break;
			}
			break;

		case WM_DESTROY:
			break;

		default:
			return false;
	}

	return true;
}

INT_PTR CALLBACK AadbgTricksDlgTabHandler(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int State;

	switch(uMsg)
	{
		case WM_INITDIALOG:
			GetRadioBtHandles(hWin);

			CheckForOptions(hWin, TABAADBTRICKS);

			if(AreAllOptionsEnabled(TABAADBTRICKS))
				CheckDlgButton(hWin, CHECK_SELECTALL, BST_CHECKED);
			else
				CheckDlgButton(hWin, CHECK_SELECTALL, BST_UNCHECKED);

			return 1;

		case WM_COMMAND:
			/* Fix: HandleChecks() was returning -1 because it receives an ID not found in the 
			array. The ID was from the select all checkbox. So, there was an integrer overflow indexing
			the array and overwriting the hWin variable. Due this, I moved the HandleChecks() call to the default
			case in the switch. Thanks to marciano for his help with this fix.
			*/
			//HandleChecks(hWin, wParam, TABAADBTRICKS);

			switch(LOWORD(wParam))
			{
				case CHECK_SELECTALL:
					// Get the state of the CHECK_ALL button
					State = IsDlgButtonChecked(hWin, CHECK_SELECTALL);
					
					// Handle the remaining checkboxes
					UI_CheckAllOptions(hWin, State, TABAADBTRICKS);
					break;

				case CHECK_GETTICKCOUNT:
					State = IsDlgButtonChecked(hWin, CHECK_GETTICKCOUNT);
					EnableRadioButtons(hWin, State);

					if(State)
					{
						aadpTricks[SIZEAADBTRICKSARRAY].functionState = TRUE;
						SetRadioBtOption(hWin, Pluginreadintfromini(hModule, "GtOpt", CW_USEDEFAULT));
					}
					else
					{
						aadpTricks[SIZEAADBTRICKSARRAY].functionState = FALSE;
					}

					break;

				default: HandleChecks(hWin, wParam, TABAADBTRICKS);
					break;
			}
			break;

		default: return false;
	}

	return true;
}

INT_PTR CALLBACK CustomHideSettingsDlgTabHandler(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HANDLE hFile;
	int iState, iCount;
	char szConfigName[MAX_PATH], szSectionsNamesList[MAX_BYTES], szAux[MAX_PATH], szAux2[MAX_PATH], CurrentConfig[MAX_PATH];
	char szDefString[] = {""};

	switch(uMsg)
	{
		case WM_INITDIALOG:
			if(!aadp_GetCurrentConfig(CurrentConfig, MAX_PATH))
				strcpy_s(szAux, MAX_PATH, "Current config: None");
			else
				sprintf_s(szAux, MAX_PATH, "Current config: %s", CurrentConfig);

			Static_SetTxt(hWin, STATIC_CURRENTCONFIG, szAux);

			hConfigLb = GetDlgItem(hWin, LB_CONFIGLIST);

			if(_Pluginreadstringfromini(hModule, "ConfigFilePath", aadpPathToConfigFile, szDefString))
			{
				aadp_GetSectionNamesInIniFile(aadpPathToConfigFile, szSectionsNamesList, MAX_BYTES);
				aadp_ListBox_AddData(hConfigLb, szSectionsNamesList);
			}

			break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case BT_SAVECONFIG:
					GetDlgItemText(hWin, EDIT_NAME, szConfigName, MAX_PATH);
					if(strlen(szConfigName) != 0)
					{	
						ListBox_AddString(hConfigLb, szConfigName);

						hFile = aadp_CreateConfigFile(hModule);

						if(hFile != INVALID_HANDLE_VALUE)
						{
							if(_Pluginwritestringtoini(hModule, "ConfigFilePath", aadpPathToConfigFile))
								WriteAadpConfig(szConfigName);
							else
								Addtolist(0, HIGHLIGHTED, "Can\'t write to Ollydbg configuration file.");

							CloseHandle(hFile);
						}
					}
					else
						MessageBox(hWin, TEXT("You must specify a name for the configuration"), TEXT("No config name!"), MB_ICONWARNING);
					break;

				case BT_APPLYCONFIG:
					iCount = ListBox_GetCount(hConfigLb);
					if(iCount > 0)
					{
						if(ListBox_GetSelCount(hConfigLb) > 0 && ListBox_GetSelCount(hConfigLb) <= 1)
						{
							while(iCount >= 0)
							{
								iState = ListBox_GetSel(hConfigLb, iCount);
								if((iState > 0) && (iState != LB_ERR))
								{
									ListBox_GetText(hConfigLb, iCount, szAux);

									if(!aadp_SetConfigAsCurrent(szAux))
										Addtolist(0, HIGHLIGHTED, "Can\'t set %s configuration as current.", szAux);
									break;
								}

								iCount--;
							}

							sprintf_s(szAux2, MAX_PATH, "Current config: %s", szAux);
							Static_SetTxt(hWin, STATIC_CURRENTCONFIG, szAux2);
						}
						else
						{
							MessageBox(hWin, TEXT("You can only apply one configuration at a time!"), TEXT("Selection warning"), MB_ICONWARNING);
						}
					}
					break;

				case BT_REMOVEFROMCONFIGLIST:
					iCount = ListBox_GetCount(hConfigLb);
					if(( iCount > 0) && (ListBox_GetSelCount(hConfigLb) > 0))
					{
						while(iCount >= 0)
						{
							iState = ListBox_GetSel(hConfigLb, iCount);
							if((iState > 0) && (iState != LB_ERR))
							{
								ListBox_GetText(hConfigLb, iCount, szAux);
								aadp_DeleteSectionFromIni(szAux, aadpPathToConfigFile);
								ListBox_DeleteString(hConfigLb, iCount);
							}

							iCount--;
						}
					}
					break;
			}
			break;

		case WM_DESTROY:
			break;

		default:
			return false;
	}

	return true;
}
