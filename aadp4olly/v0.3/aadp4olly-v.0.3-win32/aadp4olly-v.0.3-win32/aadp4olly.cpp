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

#include "aadp4olly.h"

//BOOL SetPrivilege(
//    HANDLE hToken,          // token handle
//    LPCTSTR Privilege,      // Privilege to enable/disable
//    BOOL bEnablePrivilege   // TRUE to enable.  FALSE to disable
//	)
//{
//    TOKEN_PRIVILEGES tp;
//    LUID luid;
//    TOKEN_PRIVILEGES tpPrevious;
//    DWORD cbPrevious=sizeof(TOKEN_PRIVILEGES);
//
//    if(!LookupPrivilegeValue( NULL, Privilege, &luid )) return FALSE;
//
//    // 
//    // first pass.  get current privilege setting
//    // 
//    tp.PrivilegeCount           = 1;
//    tp.Privileges[0].Luid       = luid;
//    tp.Privileges[0].Attributes = 0;
//
//    AdjustTokenPrivileges(
//            hToken,
//            FALSE,
//            &tp,
//            sizeof(TOKEN_PRIVILEGES),
//            &tpPrevious,
//            &cbPrevious
//            );
//
//    if (GetLastError() != ERROR_SUCCESS) return FALSE;
//
//    // 
//    // second pass.  set privilege based on previous setting
//    // 
//    tpPrevious.PrivilegeCount       = 1;
//    tpPrevious.Privileges[0].Luid   = luid;
//
//    if(bEnablePrivilege) {
//        tpPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
//    }
//    else {
//        tpPrevious.Privileges[0].Attributes ^= (SE_PRIVILEGE_ENABLED &
//            tpPrevious.Privileges[0].Attributes);
//    }
//
//    AdjustTokenPrivileges(
//            hToken,
//            FALSE,
//            &tpPrevious,
//            cbPrevious,
//            NULL,
//            NULL
//            );
//
//    if (GetLastError() != ERROR_SUCCESS) return FALSE;
//
//    return TRUE;
//}
//
//int AdjustPrivileges(void)
//{
//	HANDLE hToken;
//
//    if(!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken))
//    {
//        if(GetLastError() == ERROR_NO_TOKEN)
//        {
//            if(!ImpersonateSelf(SecurityImpersonation))
//            return RTN_ERROR;
//
//            if(!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken))
//			{
//                MessageBox(NULL, TEXT("OpenThreadToken error"), TEXT("Ups!"), MB_ICONERROR);
//				return RTN_ERROR;
//            }
//         }
//        else
//			return RTN_ERROR;
//     }
//
//	if(!SetPrivilege(hToken, SE_DEBUG_NAME, TRUE))
//	{
//        CloseHandle(hToken);
//
//        return RTN_ERROR;
//    }
//
//	return RTN_OK;
//}
//
void _InitGlobalArray(PAADPTRICK tricks, int arraySize, int sourceArrayIds[], char* sourceArrayNames[])
{
	int i;

	for(i = 0; i < arraySize; i++)
	{
		tricks[i].functionState = FALSE;
		tricks[i].functionId = sourceArrayIds[i];
		strcpy_s(tricks[i].functionName, MAX_PATH, sourceArrayNames[i]);
	}

}

void InitGlobalArrays(void)
{
	int i;

	//_InitGlobalArray(aadpTricks, SIZEAADBTRICKSARRAY, TabAadbTricksControlsId, TabAadbtricksFuncNames);
	//_InitGlobalArray(ollyFixes, SIZEOLLYFIXESARRAY, TabOllyFixesControlsId, TabOllyFixesFuncNames);
	//_InitGlobalArray(aadpTricks, SIZEAADBTRICKSARRAY, TabAdvSettingsControlsId, TabOllySettingsFuncNames);
	// initialize the aadp tab tricks array
	for(i = 0; i < SIZEAADBTRICKSARRAY; i++)
	{
		aadpTricks[i].functionState = FALSE;
		aadpTricks[i].functionId = TabAadbTricksControlsId[i];
		strcpy_s(aadpTricks[i].functionName, MAX_PATH, TabAadbtricksFuncNames[i]);
	}

	// initialize the gettickcount option
	GetTickCountOpt.GetTickCountPatchType = NOCOUNTER;

	strcpy_s(aadpTricks[SIZEAADBTRICKSARRAY].functionName, MAX_PATH, "hd_GetTickCount");
	aadpTricks[SIZEAADBTRICKSARRAY].functionState = FALSE;
	aadpTricks[SIZEAADBTRICKSARRAY].functionId = CHECK_GETTICKCOUNT;
	aadpTricks[SIZEAADBTRICKSARRAY].Reserved = &GetTickCountOpt;

	//((GETTICKCOUNTOPTION*)(aadpTricks[SIZEAADBTRICKSARRAY+1].Reserved))->GetTickCountPathType = NOCOUNTER;


	 // initialize the aadp tab OllyFixes array
	for(i = 0; i < SIZEOLLYFIXESARRAY; i++)
	{
		ollyFixes[i].functionState = FALSE;
		ollyFixes[i].functionId = TabOllyFixesControlsId[i];
		strcpy_s(ollyFixes[i].functionName, MAX_PATH, TabOllyFixesFuncNames[i]);
	}

	// initialize the aadp tab advanced settings
	for(i = 0; i < SIZEADVSETTINGSARRAY; i++)
	{
		aadpSettings[i].functionState = FALSE;
		aadpSettings[i].functionId = TabAdvSettingsControlsId[i];
		strcpy_s(aadpSettings[i].functionName, MAX_PATH, TabOllySettingsFuncNames[i]);
	}
}

int aadp_Readintfromini(char* ConfigName, char* Key, int DefValue)
{
	return 1;
}

int aadp_Readstringfromini(char* Key, char* OutputString, char* DefString)
{
	return 1;
}

HANDLE aadp_CreateConfigFile(HMODULE hMod)
{
	int DirLen, i;
	HANDLE hFile;

	GetModuleFileName(hMod, aadpPathToConfigFile, MAX_PATH);

	DirLen = strlen(aadpPathToConfigFile);
	i = DirLen;
	while(aadpPathToConfigFile[i] != '\\')
	{
		aadpPathToConfigFile[i] = '\x00';
		i--;
	}

	strcat_s(aadpPathToConfigFile, MAX_PATH, "aadp4ollyconfig.ini");

	hFile = CreateFile(aadpPathToConfigFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(GetLastError() == ERROR_FILE_NOT_FOUND)
	{
		hFile = CreateFile(aadpPathToConfigFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(GetLastError() != ERROR_ALREADY_EXISTS)
		{
			return hFile;
		}
	}

	return hFile;
}

int aadp_Writeinttoini(char* ConfigName, char* Key, int Value)
{
	char szValue[MAX_PATH];
	return WritePrivateProfileString(ConfigName, Key, itoa(Value, szValue, 10), aadpPathToConfigFile);

}

void aadp_ListBox_AddData(HWND hListBox, char* Data)
{
	int dlen;

	dlen = strlen(Data);
	while(*Data != 0)
	{
		ListBox_AddString(hListBox, Data);
		Data += dlen + 1;
		dlen = strlen(Data);
	}
}

int aadp_GetSectionNamesInIniFile(char* PathToIni, void* pBuffer, int BufferSize)
{
	/* If we call GetPrivateProfileString with AppName = NULL, then, it returns
	all the section names in a list like this: SECTION1.SECTION2.SECTION3.SECTIONN being "."
	the null terminator character.

	See the "remarks" section in the GetPrivateProfileString documentation on MSDN:
	http://msdn.microsoft.com/en-us/library/windows/desktop/ms724353%28v=vs.85%29.aspx
	*/

	return GetPrivateProfileString(NULL, "Current", NULL, (LPSTR)pBuffer, BufferSize, PathToIni);
}

void SetRadioBtOption(HWND hWin, int PatchType)
{
	switch(PatchType)
	{
		case NOCOUNTER:
			CheckRadioButton(hWin, RADIO_NOCOUNTER, RADIO_RANDOMCOUNTER, RADIO_NOCOUNTER);
			break;
						
		case COUNTERPLUSONE: 
			CheckRadioButton(hWin, RADIO_NOCOUNTER, RADIO_RANDOMCOUNTER, RADIO_COUNTERPLUSONE);
			break;

		case RANDOMCOUNTER:
			CheckRadioButton(hWin, RADIO_NOCOUNTER, RADIO_RANDOMCOUNTER, RADIO_RANDOMCOUNTER);
			break;
	}
}

void EnableRadioButtons(HWND hWin, int State)
{
	// Change radio buttons state
	Button_Enable(hRadioNoCounter, (BOOL)State);
	Button_Enable(hCounterPlusOne, (BOOL)State);
	Button_Enable(hRandomCounter, (BOOL)State);
}

bool IsRadioButtonChecked(HWND hRadioBt)
{
	bool Checked = FALSE;

	if(Button_GetCheck(hRadioBt) == BST_CHECKED)
		Checked = TRUE;
	return Checked;
}

void _CheckForOptions(HWND hWin, HMODULE hModule, int arraySize, PAADPTRICK tricks)
{
	int i, GtPatchType;

	for(i = 0; i < arraySize; i++)
	{
		if(Pluginreadintfromini(hModule, tricks[i].functionName, CW_USEDEFAULT) == 1)
		{
				CheckDlgButton(hWin, tricks[i].functionId, BST_CHECKED);
				tricks[i].functionState = TRUE;
				
				if(tricks[i].functionId == CHECK_GETTICKCOUNT)
				{
					GtPatchType = Pluginreadintfromini(hModule, "GtOpt", CW_USEDEFAULT);
					SetRadioBtOption(hWin, GtPatchType);
				}
		}
		else
		{
				CheckDlgButton(hWin, tricks[i].functionId, BST_UNCHECKED);
				tricks[i].functionState = FALSE;

				if(tricks[i].functionId == CHECK_GETTICKCOUNT)
					EnableRadioButtons(hWin, 0);
		}
	}

}

void CheckForOptions(HWND hWin, int Tab)
{
	switch(Tab)
	{
		case TABAADBTRICKS:
			_CheckForOptions(hWin, hModule, SIZEAADBTRICKSARRAY+1, aadpTricks);
			break;

		case TABOLLYFIXES:
			_CheckForOptions(hWin, hModule, SIZEOLLYFIXESARRAY, ollyFixes);
			break;

		case TABADVSETTINGS:
			_CheckForOptions(hWin, hModule, SIZEADVSETTINGSARRAY, aadpSettings);
			break;

		default: break;
	}
}

int CheckForBSTChecked(HWND hw, DWORD ID, char* Key)
{
	int State;

	if(IsDlgButtonChecked(hw, ID) == BST_CHECKED)
	{
		State = BST_CHECKED;
		if(!Pluginwriteinttoini(hModule, Key, 1))
			Addtolist(0, HIGHLIGHTED,"Could't write config to Ollydbg.ini");

		// additional check for the gettickcount option
		if(ID == CHECK_GETTICKCOUNT)
		{
			if(aadpTricks[SIZEAADBTRICKSARRAY].functionState == TRUE)
			{
				if(IsRadioButtonChecked(hRadioNoCounter))
				{
					((GETTICKCOUNTOPTION*)(aadpTricks[SIZEAADBTRICKSARRAY].Reserved))->GetTickCountPatchType = NOCOUNTER;
				}
				else
				{
					if(IsRadioButtonChecked(hCounterPlusOne))
					{
						((GETTICKCOUNTOPTION*)(aadpTricks[SIZEAADBTRICKSARRAY].Reserved))->GetTickCountPatchType = COUNTERPLUSONE;
					}
					else
					{
						((GETTICKCOUNTOPTION*)(aadpTricks[SIZEAADBTRICKSARRAY].Reserved))->GetTickCountPatchType = RANDOMCOUNTER;
					}
				}

				if(!Pluginwriteinttoini(hModule, "GtOpt", ((GETTICKCOUNTOPTION*)(aadpTricks[SIZEAADBTRICKSARRAY].Reserved))->GetTickCountPatchType))
					Addtolist(0, HIGHLIGHTED,"Could't write config to Ollydbg.ini");
			}
		}
	}
	else
	{
		State = BST_UNCHECKED;
		if(!Pluginwriteinttoini(hModule, Key, 0))
			Addtolist(0, HIGHLIGHTED,"Could't write config to Ollydbg.ini");
	}

	return State;
}

void SetOptions(HWND hWin, int Tab)
{
	int count;
	switch(Tab)
	{
		case TABAADBTRICKS:
			for(count = 0; count < SIZEAADBTRICKSARRAY+1; count++)
				aadpTricks[count].functionState = CheckForBSTChecked(hWin, aadpTricks[count].functionId, aadpTricks[count].functionName);
			break;

		case TABOLLYFIXES:
			for(count = 0; count < SIZEOLLYFIXESARRAY; count++)
				ollyFixes[count].functionState = CheckForBSTChecked(hWin, ollyFixes[count].functionId, ollyFixes[count].functionName);
			break;

		case TABADVSETTINGS:
			for(count = 0; count < SIZEADVSETTINGSARRAY; count++)
				aadpSettings[count].functionState = CheckForBSTChecked(hWin, aadpSettings[count].functionId, aadpSettings[count].functionName);
			break;

		default: break;
	}
}

void UI_CheckAllOptions(HWND hWin, int State, int Tab)
{
	int count;

	switch(Tab)
	{
		case TABAADBTRICKS:
			for(count = 0; count < SIZEAADBTRICKSARRAY+1; count++)
			{
				aadpTricks[count].functionState = State;
				CheckDlgButton(hWin, aadpTricks[count].functionId, State);
				EnableRadioButtons(hWin, State);
			}
			break;

		case TABOLLYFIXES:
			for(count = 0; count < SIZEOLLYFIXESARRAY; count++)
			{
				ollyFixes[count].functionState = State;
				CheckDlgButton(hWin, ollyFixes[count].functionId, State);
			}
			break;

		case TABADVSETTINGS:
			for(count = 0; count < SIZEADVSETTINGSARRAY; count++)
			{
				aadpSettings[count].functionState = State;
				CheckDlgButton(hWin, aadpSettings[count].functionId, State);
			}
			break;

		default: break;
	}
}

void DestroyGlobalHandles(void)
{
	// Destroy controls handles
	CloseHandle(hRadioNoCounter);
	CloseHandle(hCounterPlusOne);
	CloseHandle(hRandomCounter);
}

bool _AreAllOptionsEnabled(PAADPTRICK tricks, int arraySize)
{
	bool AllActive = TRUE;
	int i;

	for(i = 0; i < arraySize; i++)
	{
		if(tricks[i].functionState != TRUE)
		{
			AllActive = FALSE;
			break;
		}
	}
	return AllActive;
}

bool AreAllOptionsEnabled(int Tab)
{
	bool AllActive = FALSE;

	switch(Tab)
	{
		case TABAADBTRICKS: 
			AllActive = _AreAllOptionsEnabled(aadpTricks, SIZEAADBTRICKSARRAY+1); 
			break;

		case TABOLLYFIXES: 
			AllActive = _AreAllOptionsEnabled(ollyFixes, SIZEOLLYFIXESARRAY); 
			break;

		case TABADVSETTINGS: 
			AllActive = _AreAllOptionsEnabled(aadpSettings, SIZEADVSETTINGSARRAY); 
			break;

		default: break;
	}
	return AllActive;
}

int GetControlIdInArray(int ControlId, PAADPTRICK TricksArray, int ArraySize)
{
	int i, Index = -1;
	for(i = 0; i < ArraySize; i++)
		if(TricksArray[i].functionId == ControlId)
		{
			Index = i;
			break;
		}
	return Index;
}

void HandleChecks(HWND hWin, WPARAM wParam, int Tab)
{
	int Index, bId, checkId;
	unsigned int a, b;
	
	bId = WORD(wParam & 0xffff);
	a = IsDlgButtonChecked(hWin, bId);

	switch(Tab)
	{
		case TABAADBTRICKS: 
			Index = GetControlIdInArray(bId, aadpTricks, SIZEAADBTRICKSARRAY+1);
			aadpTricks[Index].functionState = a;
			checkId = CHECK_SELECTALL;
			break;

		case TABOLLYFIXES: 
			Index = GetControlIdInArray(bId, ollyFixes, SIZEOLLYFIXESARRAY);
			ollyFixes[Index].functionState = a;
			checkId = CHECK_TABFIXES_SELECTALL;
			break;

		case TABADVSETTINGS: 
			Index = GetControlIdInArray(bId, aadpSettings, SIZEADVSETTINGSARRAY);
			aadpSettings[Index].functionState = a;
			checkId = CHECK_TABADVSETTINGS_SELECTALL;
			break;
	}

	b = IsDlgButtonChecked(hWin, checkId);

	if(a == BST_UNCHECKED && b == BST_CHECKED)
	{
		CheckDlgButton(hWin, checkId, BST_UNCHECKED);

		switch(Tab)
		{
			// update the state of the control in the corresponding array
			case TABAADBTRICKS: aadpTricks[Index].functionState = FALSE; break;
			case TABOLLYFIXES: ollyFixes[Index].functionState = FALSE; break;
			case TABADVSETTINGS: aadpSettings[Index].functionState = FALSE; break;
		}
	}
	else
	{
		if(a == BST_CHECKED)
		{
			switch(Tab)
			{
				// update the state of the control in the corresponding array
				case TABAADBTRICKS: aadpTricks[Index].functionState = TRUE; break;
				case TABOLLYFIXES: ollyFixes[Index].functionState = TRUE; break;
				case TABADVSETTINGS: aadpSettings[Index].functionState = TRUE; break;
			}

			if(AreAllOptionsEnabled(Tab))					
				CheckDlgButton(hWin, checkId, BST_CHECKED);
		}
	}
}

LRESULT CALLBACK aadp4Ollyproc(HWND hWin,UINT msg,WPARAM wp,LPARAM lp) {
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

void WriteAadpConfig(char* ConfigName)
{
	int i;

	for(i = 0; i < SIZEAADBTRICKSARRAY; i++)
		aadp_Writeinttoini(ConfigName, aadpTricks[i].functionName, aadpTricks[i].functionState);

	for(i = 0; i < SIZEOLLYFIXESARRAY; i++)
		aadp_Writeinttoini(ConfigName, ollyFixes[i].functionName, ollyFixes[i].functionState);

	for(i = 0; i < SIZEADVSETTINGSARRAY; i++)
		aadp_Writeinttoini(ConfigName, aadpSettings[i].functionName, aadpSettings[i].functionState);
}

void ReadAadpConfig(char* ConfigName)
{
	int i;

	for(i = 0; i < SIZEAADBTRICKSARRAY; i++)
		aadpTricks[i].functionState = aadp_Readintfromini(ConfigName, aadpTricks[i].functionName, USEDEFAULT);

	for(i = 0; i < SIZEOLLYFIXESARRAY; i++)
		ollyFixes[i].functionState = aadp_Readintfromini(ConfigName, ollyFixes[i].functionName, ollyFixes[i].functionState);

	for(i = 0; i < SIZEADVSETTINGSARRAY; i++)
		aadpSettings[i].functionState = aadp_Readintfromini(ConfigName, aadpSettings[i].functionName, aadpSettings[i].functionState);

}

int aadp_DeleteSectionFromIni(char* szAux, char* PathToIni)
{
	return WritePrivateProfileString(szAux, NULL, NULL, PathToIni);
}

bool Static_SetTxt(HWND hWin, int StaticId, char* szText)
{
	return SetDlgItemText(hWin, StaticId, szText);
}

bool aadp_GetCurrentConfig(char* Config)
{
	return true;
}

INT_PTR CALLBACK CustomHideSettingsDlgTabHandler(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int iState, iCount;
	HANDLE hFile;
	int* pBuff;
	char szConfigName[MAX_PATH], szSectionsNamesList[MAX_BYTES], szAux[MAX_PATH], CurrentConfig[MAX_PATH];
	char szDefString[] = {""};

	switch(uMsg)
	{
		case WM_INITDIALOG:
			if(!aadp_GetCurrentConfig(CurrentConfig))
				strcpy_s(CurrentConfig, MAX_PATH, "Current config: None");

			Static_SetTxt(hWin, STATIC_CURRENTCONFIG, CurrentConfig);

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

void GetRadioBtHandles(HWND hWin)
{
	hRadioNoCounter = GetDlgItem(hWin, RADIO_NOCOUNTER);
	hCounterPlusOne = GetDlgItem(hWin, RADIO_COUNTERPLUSONE);
	hRandomCounter = GetDlgItem(hWin, RADIO_RANDOMCOUNTER);
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

static void Createaadp4ollywindow(void) {
  InitCommonControls();
  DialogBoxParamA(hinst, (LPCSTR)IDD_AADP4OLLY, (HWND)_Plugingetvalue(VAL_HWMAIN), (DLGPROC)aadp4Ollyproc, 0);

}

BOOL WINAPI DllMain(HINSTANCE hi, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH)
		hinst = hi;
	return 1;
}

extc int _export cdecl ODBG_Plugindata(char shortname[32]) {
  strcpy_s(shortname, sizeof("aadp4olly"), "aadp4olly");
  return PLUGIN_VERSION;
}

extc int _export cdecl ODBG_Plugininit(int ollydbgversion,HWND hw,ulong *features){
	
	//hwmain = hw;

	if (ollydbgversion < PLUGIN_VERSION)
		return -1;

	Addtolist(0,0,"aadp4plugin v0.2");
	Addtolist(0,-1,"  Written by +NCR/CRC! [ReVeRsEr]");

	InitGlobalArrays();

	//if(AdjustPrivileges() == RTN_ERROR)
	//	MessageBox(hw, TEXT("Couldn\'t adjust privileges"), TEXT("AdjustPrivileges error"), MB_ICONERROR);

	CheckForOptions(AadbgTricksDlgHwnd, TABAADBTRICKS);
	//CheckForOptions(OllyFixesDlgHwnd, TABOLLYFIXES);
	//CheckForOptions(CustomSettingsDlgHwnd, TABADVSETTINGS);
	//CheckForOptions();
	return 0;

}

extc void _export cdecl ODBG_Pluginmainloop(DEBUG_EVENT *debugevent) {
	t_status status;
	DWORD pid = -1;
	int i;

	status = Getstatus();

	// Si hay un proceso cargado y ese proceso esta detenido ...
	if(status != STAT_NONE && status == STAT_STOPPED)
	{
		// Incremento un flag
		Flag++;
		if(Flag == 1) // es la primera vez que pasa? ...
		{
			// esta el checkbox de IsDebuggerPresent activado? ...
			pid = _Plugingetvalue(VAL_PROCESSID);

			for(i = 0; i < SIZEAADBTRICKSARRAY; i++)
			{
				switch(aadpTricks[i].functionId)
				{
					case CHECK_RDTSC: 
						break; 

					case CHECK_BEINGDEBUGGED: 
						if(!hd_IsDebuggerPresent(pid))
							Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch BeingDebugged flag on PEB :(");
						break; 

					case CHECK_FORCEFLAGS: 
						break; 

					case CHECK_SETINFOTHREAD: 
						if(!hd_HookZwSetInformationThread(pid))
							Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch ZwSetInformationThread :(");
						break;

					case CHECK_ZWQUERYINFOPROCESS: 
						if(!hd_HookZwQueryInformationProcess(pid))
							Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch ZwQueryInformationProcess :(");
						break; 

					case CHECK_NTGLOBALFLAGS: 
						if(!hd_NtGlobalFlags(pid))
							Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch NtGlobalFlags flag on PEB :(");
						break; 

					case CHECK_HEAPFLAGS: 
						if(!hd_HeapFlagsOnPEB(pid))
							Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch HeapFlags flag on PEB :(");
						break; 

					case CHECK_OUTPUTDEBUGSTRING: 
						if(!hd_HookOutputDebugString(pid))
							Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch OutputDebugString :(");
						break; 

					case CHECK_ZWQUERYOBJECT: 
						if(!hd_ZwQueryObject(pid))
							Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch ZwQueryObject :(");
						break; 

					case CHECK_ZWOPENPROCESS:
						if(!hd_ZwOpenProcess(pid, GetCurrentProcessId()))
							Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch ZwOpenProcess :(");
						break; 
					
					case CHECK_FINDWINDOW:
						if(!hd_FindWindow((HWND)_Plugingetvalue(VAL_HWMAIN), "", pid))
							Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch FindWindow :(");
						break; 

					case CHECK_UNHANDLEDEXCEPTIONFILTER: 
						if(!hd_UnhandledExceptionFilter(pid))
							Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch UnhandledExceptionFilter :(");
						break; 

					case CHECK_SUSPENDTHREAD: 
						if(!hd_SuspendThread(pid))
							Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch SuspendThread :(");
						break; 

					case CHECK_BLOCKINPUT: 
						if(!hd_BlockInput(pid))
							Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch BlockInput :(");
						break; 

					case CHECK_TERMINATEPROCESS: 
						if(!hd_TerminateProcess(pid))
							Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch TerminateProcess :(");
						break; 

					case CHECK_PROCESS32NEXT: 
						if(!hd_Process32Next(pid))
							Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch Process32Next :(");
						break; 

					case CHECK_MODULE32NEXT: 
						if(!hd_Module32Next(pid))
							Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch Module32Next :(");
						break; 

					case CHECK_ZWQUERYSYSINFO: 
						if(!hd_ZwQuerySystemInformation(pid))
							Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch ZwQuerySystemInformation :(");
						break;
	
					default: break;
				}
			}

			//if(bGetTickCount)
			//{
			//	if(!hd_GetTickCount(pid))
			//		Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch GetTickCount :(");
			//}
		}
	}
}

extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) {
	char str[] = "0 &Options\tAlt+Q|1 &Help,2 &Check for &updates,3 &About";
	if (origin!=PM_MAIN)
		return 0;
	strcpy_s(data, 4096, str);
	return 1;
}

extc int _export cdecl ODBG_Pluginshortcut(int origin,int ctrl,int alt,int shift,int key,void *item) {
  if(origin == PM_MAIN)
  {
	  if(key==VK_ESCAPE)
		  EndDialog(hwPluginWin, 0);

	  if (ctrl==0 && alt==1 && shift==0 && key=='Q') 
	  {
		Createaadp4ollywindow();
		//CheckForOptions(AadbgTricksDlgHwnd, TABAADBTRICKS);
		//CheckForOptions(OllyFixesDlgHwnd, TABOLLYFIXES);
		//CheckForOptions(CustomSettingsDlgHwnd, TABADVSETTINGS);

		return 1;
	  }                   
  }
  return 0;
};

extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) {
  if (origin!=PM_MAIN)
    return;

  switch (action) {
    case 0:
		Createaadp4ollywindow();
		//CheckForOptions(AadbgTricksDlgHwnd, TABAADBTRICKS);
		//CheckForOptions(OllyFixesDlgHwnd, TABOLLYFIXES);
		//CheckForOptions(CustomSettingsDlgHwnd, TABADVSETTINGS);
      break;

    case 1:
		ShellExecuteA(NULL, "open", "http://code.google.com/p/aadp", 0, 0, SW_SHOWNORMAL);
      break;

	case 2: MessageBox((HWND)_Plugingetvalue(VAL_HWMAIN), TEXT("UPDATE!"), TEXT("UPDATE!"), MB_OK); 
		break;

    case 3:
      MessageBoxA((HWND)_Plugingetvalue(VAL_HWMAIN),
		  "aadp4olly plugin v0.3\nWritten by +NCR/CRC! [ReVeRsEr]",
        "aadp4olly", MB_OK|MB_ICONINFORMATION);
      break;

    default: break;
  };
}

extc int _export cdecl ODBG_Pluginclose(void) {
	return 0;
}

extc void _export cdecl ODBG_Pluginreset(void) {
	Flag = 0;
	CheckForOptions(AadbgTricksDlgHwnd, TABAADBTRICKS);
	//CheckForOptions(OllyFixesDlgHwnd, TABOLLYFIXES);
	//CheckForOptions(CustomSettingsDlgHwnd, TABADVSETTINGS);

}
