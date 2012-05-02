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

#include "helpers.h"

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

void UI_UpdateCheckBoxes(int Tab)
{
	int i;

	switch(Tab)
	{
		case TABAADBTRICKS: 
			for(i = 0; i < SIZEAADBTRICKSARRAY; i++)
				CheckDlgButton(AadbgTricksDlgHwnd, aadpTricks[i].functionId, aadpTricks[i].functionState);
			break;

		case TABOLLYFIXES:
			for(i = 0; i < SIZEOLLYFIXESARRAY; i++)
				CheckDlgButton(OllyFixesDlgHwnd, ollyFixes[i].functionId, ollyFixes[i].functionState);
			break;

		case TABADVSETTINGS:
			for(i = 0; i < SIZEADVSETTINGSARRAY; i++)
				CheckDlgButton(SettingsDlgHwnd, aadpSettings[i].functionId, aadpSettings[i].functionState);
			break;

		default: 
			break;
	}
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
		ollyFixes[i].functionState = aadp_Readintfromini(ConfigName, ollyFixes[i].functionName, USEDEFAULT);

	for(i = 0; i < SIZEADVSETTINGSARRAY; i++)
		aadpSettings[i].functionState = aadp_Readintfromini(ConfigName, aadpSettings[i].functionName, USEDEFAULT);

}

int aadp_DeleteSectionFromIni(char* szAux, char* PathToIni)
{
	return WritePrivateProfileString(szAux, NULL, NULL, PathToIni);
}

bool aadp_GetCurrentConfig(char* OutConfigBuffer, int OutConfigBufferLen)
{
	/*	
		1. Enumerate all sections in the .ini file.
		2. Iterate over all sections.
			2.1. Get the "Current" key of every section.
			2.2. If the "Current" key is set to true (1), then, return the name of the section.
		3. If none of the sections have the "Current" key set to true (1), then, apply the default config.
		4. Default config is: all options set to false (0).
	*/

	int slen;
	char szPathToIni[MAX_PATH], szSectionsNamesList[MAX_BYTES];
	char* SectionsNamesList = szSectionsNamesList;

	if(aadp_GetPathToIniFile(szPathToIni, MAX_PATH))
	{
		if(aadp_GetSectionNamesInIniFile(szPathToIni, szSectionsNamesList, MAX_BYTES))
		{
			slen = strlen(SectionsNamesList);
			while(*SectionsNamesList != 0)
			{
				if(aadp_Readintfromini(SectionsNamesList, "Current", USEDEFAULT) == 1)
				{
					strcpy_s(OutConfigBuffer, OutConfigBufferLen, SectionsNamesList);
					return true;
				}

				SectionsNamesList += slen + 1;
			}
		}
	}

	return false;
}

bool aadp_SetConfigToFalse(char* ConfigToSet)
{
	if(aadp_Writeinttoini(ConfigToSet, "Current", 0))
		return true;

	return false;
}

bool aadp_SetConfigAsCurrent(char* ConfigToSetAsCurrent)
{
	if(aadp_Writeinttoini(ConfigToSetAsCurrent, "Current", 1))
	{
		ReadAadpConfig(ConfigToSetAsCurrent);

		UI_UpdateCheckBoxes(TABAADBTRICKS);
		UI_UpdateCheckBoxes(TABOLLYFIXES);
		UI_UpdateCheckBoxes(TABADVSETTINGS);

		return true;
	}

	return false;
}

int Static_SetTxt(HWND hWin, int StaticId, char* szText)
{
	return SetDlgItemText(hWin, StaticId, szText);
}

void GetRadioBtHandles(HWND hWin)
{
	hRadioNoCounter = GetDlgItem(hWin, RADIO_NOCOUNTER);
	hCounterPlusOne = GetDlgItem(hWin, RADIO_COUNTERPLUSONE);
	hRandomCounter = GetDlgItem(hWin, RADIO_RANDOMCOUNTER);
}

int aadp_GetPathToIniFile(char* Out_PathToIni, int BuffLen)
{
	char szDefString[] = {""};
	char szAux[MAX_PATH];
	if(_Pluginreadstringfromini(hModule, "ConfigFilePath", szAux, szDefString))
	{
		strcpy_s(Out_PathToIni, BuffLen, szAux);
		return true;
	}
	return false;
}

int aadp_Readintfromini(char* ConfigName, char* Key, int DefValue)
{
	return GetPrivateProfileInt(ConfigName, Key, DefValue, aadpPathToConfigFile);
}

int aadp_Readstringfromini(char* ConfigName, char* Key, char* OutputString, int OutputStringSize, char* DefString)
{
	char szDef[] = {""};
	return GetPrivateProfileString(ConfigName, Key, szDef, OutputString, OutputStringSize, aadpPathToConfigFile);
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
	_itoa_s(Value, szValue, MAX_PATH, 10);
	return WritePrivateProfileString(ConfigName, Key, szValue, aadpPathToConfigFile);

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
