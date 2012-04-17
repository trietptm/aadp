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

static HINSTANCE hinst;            
static HWND      hwmain;
static HWND      hwcmd;  

HWND hwPluginWin;

int Flag = 0;

bool	bIsDbgPresent = FALSE, 
		bNtGlobalFlags = FALSE, 
		bHeapFlags = FALSE, 
		bODS = FALSE, 
		bGetTickCount = FALSE, 
		bZwSIT = FALSE, 
		bZwQIP = FALSE,
		bSuspendThread = FALSE,
		bUEF = FALSE,
		bModule32Next = FALSE,
		bProcess32Next = FALSE,
		bFindWindow = FALSE,
		bZwQueryObject = FALSE,
		bZwOpenProcess = FALSE,
		bTerminateProcess = FALSE,
		bBlockInput = FALSE,
		bZWQSI = FALSE;

void CheckForOptions(void)
{
	HMODULE hModule;

	hModule = GetModuleHandleA("aadp4olly-v.0.3-win32.dll");
	
	// Check for IsDebuggerPresent
	if(Pluginreadintfromini(hModule, "hd_IsDebuggerPresent", CW_USEDEFAULT) == 1)
	{
			CheckDlgButton(hwPluginWin, CHECK_BEINGDEBUGGED, BST_CHECKED);
			bIsDbgPresent = TRUE;
	}
	else
	{
			CheckDlgButton(hwPluginWin, CHECK_BEINGDEBUGGED, BST_UNCHECKED);
			bIsDbgPresent = FALSE;
	}

	// Check for NtGlobalFlags
	if(Pluginreadintfromini(hModule, "hd_NtGlobalFlags", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, CHECK_NTGLOBALFLAGS, BST_CHECKED);
		bNtGlobalFlags = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, CHECK_NTGLOBALFLAGS, BST_UNCHECKED);
		bNtGlobalFlags = FALSE;
	}

	// Check for HeapFlags
	if(Pluginreadintfromini(hModule, "hd_HeapFlags", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, CHECK_HEAPFLAGS, BST_CHECKED);
		bHeapFlags = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, CHECK_HEAPFLAGS, BST_UNCHECKED);
		bHeapFlags = FALSE;
	}

	// Check for ZwSetInformationThread
	if(Pluginreadintfromini(hModule, "hd_ZwSetInformationThread", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, CHECK_SETINFOTHREAD, BST_CHECKED);
		bZwSIT = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, CHECK_SETINFOTHREAD, BST_UNCHECKED);
		bZwSIT = FALSE;
	}

	// Check for ZwQueryInformationProcess
	if(Pluginreadintfromini(hModule, "hd_ZwQueryInformationProcess", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, CHECK_ZWQUERYINFOPROCESS, BST_CHECKED);
		bZwQIP = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, CHECK_ZWQUERYINFOPROCESS, BST_UNCHECKED);
		bZwQIP = FALSE;
	}

	// Check for GetTickCount
	if(Pluginreadintfromini(hModule, "hd_GetTickCount", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, CHECK_GETTICKCOUNT, BST_CHECKED);
		bGetTickCount = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, CHECK_GETTICKCOUNT, BST_UNCHECKED);
		bGetTickCount = FALSE;
	}

	// Check for OutputDebugString
	if(Pluginreadintfromini(hModule, "hd_OutputDebugString", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, CHECK_OUTPUTDEBUGSTRING, BST_CHECKED);
		bODS = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, CHECK_OUTPUTDEBUGSTRING, BST_UNCHECKED);
		bODS = FALSE;
	}

	// Check for ZwQueryObject
	if(Pluginreadintfromini(hModule, "hd_ZwQueryObject", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, CHECK_ZWQUERYOBJECT, BST_CHECKED);
		bZwQueryObject = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, CHECK_ZWQUERYOBJECT, BST_UNCHECKED);
		bZwQueryObject = FALSE;
	}

	// Check for ZwOpenProcess
	if(Pluginreadintfromini(hModule, "hd_ZwOpenProcess", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, CHECK_ZWOPENPROCESS, BST_CHECKED);
		bZwOpenProcess = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, CHECK_ZWOPENPROCESS, BST_UNCHECKED);
		bZwOpenProcess = FALSE;
	}

	// Check for FindWindow
	if(Pluginreadintfromini(hModule, "hd_FindWindow", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, CHECK_FINDWINDOW, BST_CHECKED);
		bFindWindow = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, CHECK_FINDWINDOW, BST_UNCHECKED);
		bFindWindow = FALSE;
	}

	// Check for Module32Next
	if(Pluginreadintfromini(hModule, "hd_Module32Next", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, CHECK_MODULE32NEXT, BST_CHECKED);
		bModule32Next = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, CHECK_MODULE32NEXT, BST_UNCHECKED);
		bModule32Next = FALSE;
	}

	// Check for Process32Next
	if(Pluginreadintfromini(hModule, "hd_Process32Next", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, CHECK_PROCESS32NEXT, BST_CHECKED);
		bProcess32Next = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, CHECK_PROCESS32NEXT, BST_UNCHECKED);
		bProcess32Next = FALSE;
	}

	// Check for UnhandledExceptionFilter
	if(Pluginreadintfromini(hModule, "hd_UnhandledExceptionFilter", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, CHECK_UNHANDLEDEXCEPTIONFILTER, BST_CHECKED);
		bUEF = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, CHECK_UNHANDLEDEXCEPTIONFILTER, BST_UNCHECKED);
		bUEF = FALSE;
	}

	// Check for SuspendThread
	if(Pluginreadintfromini(hModule, "hd_SuspendThread", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, CHECK_SUSPENDTHREAD, BST_CHECKED);
		bSuspendThread = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, CHECK_SUSPENDTHREAD, BST_UNCHECKED);
		bSuspendThread = FALSE;
	}

	// Check for TerminateProcess
	if(Pluginreadintfromini(hModule, "hd_TerminateProcess", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, CHECK_TERMINATEPROCESS, BST_CHECKED);
		bTerminateProcess = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, CHECK_TERMINATEPROCESS, BST_UNCHECKED);
		bTerminateProcess = FALSE;
	}

	// Check for BlockInput
	if(Pluginreadintfromini(hModule, "hd_BlockInput", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, CHECK_BLOCKINPUT, BST_CHECKED);
		bBlockInput = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, CHECK_BLOCKINPUT, BST_UNCHECKED);
		bBlockInput = FALSE;
	}

	if(Pluginreadintfromini(hModule, "hd_QuerySystemInformation", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, CHECK_ZWQUERYSYSINFO, BST_CHECKED);
		bZWQSI = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, CHECK_ZWQUERYSYSINFO, BST_UNCHECKED);
		bZWQSI = FALSE;
	}

}

void CheckForBSTChecked(HWND hw, DWORD ID, char* Key)
{
	if(IsDlgButtonChecked(hw, ID) == BST_CHECKED)
	{
		if(!Pluginwriteinttoini(GetModuleHandleA("aadp4olly.dll"), Key, 1))
			Addtolist(0, HIGHLIGHTED,"Could't write config to Ollydbg.ini");
	}
	else
	{
		if(!Pluginwriteinttoini(GetModuleHandleA("aadp4olly.dll"), Key, 0))
			Addtolist(0, HIGHLIGHTED,"Could't write config to Ollydbg.ini");
	}

}

void SetOptions(void)
{
	CheckForBSTChecked(hwPluginWin, CHECK_BEINGDEBUGGED, "hd_IsDebuggerPresent");
	CheckForBSTChecked(hwPluginWin, CHECK_SETINFOTHREAD, "hd_ZwSetInformationThread");
	CheckForBSTChecked(hwPluginWin, CHECK_ZWQUERYINFOPROCESS, "hd_ZwQueryInformationProcess");
	CheckForBSTChecked(hwPluginWin, CHECK_NTGLOBALFLAGS, "hd_NtGlobalFlags");
	CheckForBSTChecked(hwPluginWin, CHECK_HEAPFLAGS, "hd_HeapFlags");
	CheckForBSTChecked(hwPluginWin, CHECK_OUTPUTDEBUGSTRING, "hd_OutputDebugString");
	CheckForBSTChecked(hwPluginWin, CHECK_GETTICKCOUNT, "hd_GetTickCount");
	CheckForBSTChecked(hwPluginWin, CHECK_ZWQUERYOBJECT, "hd_ZwQueryObject");
	CheckForBSTChecked(hwPluginWin, CHECK_ZWOPENPROCESS, "hd_ZwOpenProcess");
	CheckForBSTChecked(hwPluginWin, CHECK_FINDWINDOW, "hd_FindWindow");
	CheckForBSTChecked(hwPluginWin, CHECK_UNHANDLEDEXCEPTIONFILTER, "hd_UnhandledExceptionFilter");
	CheckForBSTChecked(hwPluginWin, CHECK_SUSPENDTHREAD, "hd_SuspendThread");
	CheckForBSTChecked(hwPluginWin, CHECK_BLOCKINPUT, "hd_BlockInput");
	CheckForBSTChecked(hwPluginWin, CHECK_TERMINATEPROCESS, "hd_TerminateProcess");
	CheckForBSTChecked(hwPluginWin, CHECK_PROCESS32NEXT, "hd_Process32Next");
	CheckForBSTChecked(hwPluginWin, CHECK_MODULE32NEXT, "hd_Module32Next");
	CheckForBSTChecked(hwPluginWin, CHECK_ZWQUERYSYSINFO, "hd_QuerySystemInformation");
}

void UI_CheckAllOptions(HWND hw, int State, int Tab)
{
	int count;

	switch(Tab)
	{
		case TABAADBTRICKS:
			for(count = 0; count < SIZEAADBTRICKSARRAY; count++)
				CheckDlgButton(hw, TabAadbTricksControlsId[count], State);
			break;

		case TABOLLYFIXES:
			for(count = 0; count < SIZEOLLYFIXESARRAY; count++)
				CheckDlgButton(hw, TabOllyFixesControlsId[count], State);
			break;

		case TABADVSETTINGS:
			for(count = 0; count < SIZEADVSETTINGSARRAY; count++)
				CheckDlgButton(hw, TabAdvSettingsControlsId[count], State);
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

LRESULT CALLBACK aadp4Ollyproc(HWND hw,UINT msg,WPARAM wp,LPARAM lp) {
  hwPluginWin = hw;
  static HWND AadbgTricksDlgHwnd, OllyFixesDlgHwnd, SettingsDlgHwnd, CustomSettingsDlgHwnd, AboutDlgHwnd;
  static HWND MainTabDlgHwnd;
  int Index;

  switch(msg)
  {
  case WM_INITDIALOG:

	AadbgTricksDlgHwnd = CreateDialog(hinst, MAKEINTRESOURCE(ANTIDBGTRICKS), hw, &AadbgTricksDlgTabHandler);
	OllyFixesDlgHwnd = CreateDialog(hinst, MAKEINTRESOURCE(OLLYFIXES), hw, &OllyFixesDlgHandlerTabHandler);
	SettingsDlgHwnd = CreateDialog(hinst, MAKEINTRESOURCE(ADVANCEDSETTINGS), hw, &SettingsDlgTabHandler);
	CustomSettingsDlgHwnd = CreateDialog(hinst, MAKEINTRESOURCE(CUSTOMHIDESETTINGSDLG), hw, &CustomHideSettingsDlgTabHandler);
	AboutDlgHwnd = CreateDialog(hinst, MAKEINTRESOURCE(ABOUT), hw, &AboutDlgHandler);

	MainTabDlgHwnd = GetDlgItem(hw, IDC_TAB1);

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
	
	CheckForOptions();

	return 1;

  case WM_NOTIFY:
	switch(((NMHDR *)lp)->code)
	{
	case TCN_SELCHANGE: // Get currently selected tab window to front
		TabToFront(MainTabDlgHwnd, -1);
		break;
	default:
		return false;
	}
	break;

  case WM_COMMAND:
    switch(wp)
    {
    case IDOK:
		SetOptions();
		EndDialog(hw, 0);
		return 0;
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
		DestroyGlobalHandles();

		// Destroy the modal dialog
		EndDialog(hw, 0);
    }
  }
  return 0;
}

INT_PTR CALLBACK OllyFixesDlgHandlerTabHandler(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
int State;

	switch(uMsg)
	{
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case CHECK_TABFIXES_SELECTALL:
					State = IsDlgButtonChecked(hWin, CHECK_TABFIXES_SELECTALL);
					UI_CheckAllOptions(hWin, State, TABOLLYFIXES);
					break;

				default: break;
			}
			break;

		default:
			return false;
	}

	return true;
}

INT_PTR CALLBACK AboutDlgHandler(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam)
{
HWND TabWindow;
int Index;

	switch(Message)
	{
		case WM_COMMAND:
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
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case CHECK_TABADVSETTINGS_SELECTALL:
					State = IsDlgButtonChecked(hWin, CHECK_TABADVSETTINGS_SELECTALL);
					UI_CheckAllOptions(hWin, State, TABADVSETTINGS);
					break;

				default: break;
			}
			break;

		default:
			return false;
	}

	return true;
}

INT_PTR CALLBACK CustomHideSettingsDlgTabHandler(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam)
{
HWND TabWindow;
int Index;

	switch(Message)
	{
		case WM_COMMAND:
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

void HandleGetTickCountBt(HWND hWin, int State)
{
	// Change checkbox's state
	CheckDlgButton(hWin, CHECK_GETTICKCOUNT, State);

	// Change radio buttons state
	Button_Enable(hRadioNoCounter, (BOOL)State);
	Button_Enable(hCounterPlusOne, (BOOL)State);
	Button_Enable(hRandomCounter, (BOOL)State);

	// Set the default option
	CheckRadioButton(hWin, RADIO_NOCOUNTER, RADIO_RANDOMCOUNTER, RADIO_NOCOUNTER);
}

INT_PTR CALLBACK AadbgTricksDlgTabHandler(HWND hWin, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
HWND TabWindow;
int State;

	switch(uMsg)
	{
		case WM_INITDIALOG:
			// We handle the radio button state by separated
			GetRadioBtHandles(hWin);

			// Handle the current state
			State = IsDlgButtonChecked(hWin, CHECK_GETTICKCOUNT);
			HandleGetTickCountBt(hWin, State);
			return 1;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case CHECK_SELECTALL:
					// Get the state of the CHECK_ALL button
					State = IsDlgButtonChecked(hWin, CHECK_SELECTALL);
					HandleGetTickCountBt(hWin, State);
					
					// Handle the remaining checkboxes
					UI_CheckAllOptions(hWin, State, TABAADBTRICKS);
					break;

				case CHECK_GETTICKCOUNT:
					State = IsDlgButtonChecked(hWin, CHECK_GETTICKCOUNT);
					HandleGetTickCountBt(hWin, State);
					break;
			}
			break;

		default:
			return false;
	}

	return true;
}

static void Createaadp4ollywindow(void) {
  InitCommonControls();
  DialogBoxParamA(hinst, (LPCSTR)IDD_AADP4OLLY, hwmain, (DLGPROC)aadp4Ollyproc, 0);

}

BOOL WINAPI DllMain(HINSTANCE hi,DWORD reason,LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH)
		hinst = hi;
	return 1;
};

extc int _export cdecl ODBG_Plugindata(char shortname[32]) {
  strcpy_s(shortname, sizeof("aadp4olly"), "aadp4olly");
  return PLUGIN_VERSION;
};

extc int _export cdecl ODBG_Plugininit(int ollydbgversion,HWND hw,ulong *features){
	
	hwmain = hw;

	if (ollydbgversion < PLUGIN_VERSION)
		return -1;

	Addtolist(0,0,"aadp4plugin v0.2");
	Addtolist(0,-1,"  Written by +NCR/CRC! [ReVeRsEr]");

	CheckForOptions();
	return 0;

}

extc void _export cdecl ODBG_Pluginmainloop(DEBUG_EVENT *debugevent) {
	t_status status;
	DWORD pid = -1;

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
			if(bIsDbgPresent)
			{
				// entonces, patcheo el BeingDebugged flag
				if(!hd_IsDebuggerPresent(pid))
					Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch BeingDebugged flag on PEB :(");
			}
			if(bNtGlobalFlags)
			{
				if(!hd_NtGlobalFlags(pid))
					Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch NtGlobalFlags flag on PEB :(");
			}
			if(bHeapFlags)
			{
				if(!hd_HeapFlagsOnPEB(pid))
					Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch HeapFlags flag on PEB :(");
			}

			if(bZwQIP)
			{	
				if(!hd_HookZwQueryInformationProcess(pid))
					Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch ZwQueryInformationProcess :(");
			}

			if(bZwSIT)
			{
				if(!hd_HookZwSetInformationThread(pid))
					Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch ZwSetInformationThread :(");
			}

			if(bGetTickCount)
			{
				if(!hd_GetTickCount(pid))
					Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch GetTickCount :(");
			}

			if(bODS)
			{
				if(!hd_HookOutputDebugString(pid))
					Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch OutputDebugString :(");
			}

			if(bZwQueryObject)
			{
				if(!hd_ZwQueryObject(pid))
					Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch ZwQueryObject :(");
			}

			if(bZwOpenProcess)
			{
				if(!hd_ZwOpenProcess(pid, GetCurrentProcessId()))
					Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch ZwOpenProcess :(");
			}

			if(bFindWindow)
			{
				if(!hd_FindWindow((HWND)_Plugingetvalue(VAL_HWMAIN), "", pid))
					Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch FindWindow :(");
			}

			if(bUEF)
			{
				if(!hd_UnhandledExceptionFilter(pid))
					Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch UnhandledExceptionFilter :(");
			}

			if(bSuspendThread)
			{
				if(!hd_SuspendThread(pid))
					Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch SuspendThread :(");
			}

			if(bBlockInput)
			{
				if(!hd_BlockInput(pid))
					Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch BlockInput :(");
			}

			if(bTerminateProcess)
			{
				if(!hd_TerminateProcess(pid))
					Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch TerminateProcess :(");
			}

			if(bProcess32Next)
			{
				if(!hd_Process32Next(pid))
					Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch Process32Next :(");
			}

			if(bModule32Next)
			{
				if(!hd_Module32Next(pid))
					Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch Module32Next :(");
			}

			if(bZWQSI)
			{
				if(!hd_ZwQuerySystemInformation(pid))
					Addtolist(0, HIGHLIGHTED, "aadp4olly error: Can't patch ZwQuerySystemInformation :(");
			}
		}
	}
}

extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) {
	char str[] = "0 &Options\tAlt+Q|1 &Help,2 &Check for &update,3 &About";
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
		CheckForOptions();
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
	  CheckForOptions();
      break;
    case 1:
		ShellExecuteA(NULL, "open", "http://code.google.com/p/aadp", 0, 0, SW_SHOWNORMAL);
      break;
	case 2: MessageBox(hwmain, TEXT("UPDATE!"), TEXT("UPDATE!"), MB_OK); break;
    case 3:
      MessageBoxA(hwmain,
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
	CheckForOptions();
}
