// $Id$

/*
 Anti-Anti-Debugging Plugin for Ollydbg - LGPL 3.0

 Copyright (C) 2010 +NCR/CRC! [ReVeRsEr] http://crackinglandia.blogspot.com

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

static char      cmdlinewinclass[32];

static int       posx;
static int       posy;

static int       poponstop;

int Flag = 0;

HWND hcbIsDbgPresent, hcbNtGlobalFlags, hcbHeapFlags, hcbOds, hcbGetTickCount, 
	  hcbZwTIP, hcbZwQIP, hbt_save, hbt_cancel, hedt, hwPluginWin;

bool bIsDbgPresent = FALSE, bNtGlobalFlags = FALSE, bHeapFlags = FALSE, bODS = FALSE, 
bGetTickCount = FALSE, bZwSIT = FALSE, bZwQIP = FALSE;

void CheckForBSTChecked(HWND hw, DWORD ID, char* Key)
{
	if(IsDlgButtonChecked(hw, ID) == BST_CHECKED)
	{
		if(!Pluginwriteinttoini(GetModuleHandleA("aadp4olly.dll"), Key, 1))
			MessageBoxA(hw, "Could't write config to Ollydbg.ini", "Ups!", MB_ICONERROR);
	}
	else
	{
		if(!Pluginwriteinttoini(GetModuleHandleA("aadp4olly.dll"), Key, 0))
			MessageBoxA(hw, "Could't write config to Ollydbg.ini", "Ups!", MB_ICONERROR);
	}

}

void SetOptions(void)
{
	CheckForBSTChecked(hwPluginWin, ID_CB_ISDBGPRESENT, "hd_IsDebuggerPresent");
	CheckForBSTChecked(hwPluginWin, ID_CB_ZWSETINFOTHREAD, "hd_ZwSetInformationThread");
	CheckForBSTChecked(hwPluginWin, ID_CB_ZWQUERYINFOPROCESS, "hd_ZwQueryInformationProcess");
	CheckForBSTChecked(hwPluginWin, ID_CB_NTGLOBALFLAGS, "hd_NtGlobalFlags");
	CheckForBSTChecked(hwPluginWin, ID_CB_HEAPFLAGS, "hd_HeapFlags");
	CheckForBSTChecked(hwPluginWin, ID_CB_ODSTRING, "hd_OutputDebugString");
	CheckForBSTChecked(hwPluginWin, ID_CB_GETTICKCOUNT, "hd_GetTickCount");
}

LRESULT CALLBACK aadp4Ollyproc(HWND hw,UINT msg,WPARAM wp,LPARAM lp) {
	RECT rc;
	PAINTSTRUCT ps;
	HBRUSH hbr;
	HDC dc;

	hwPluginWin = hw;

	switch(msg)
	{
		case WM_COMMAND:
			if(ID_BT_CANCEL == LOWORD(wp))
			{	
				EnableWindow(hwmain, TRUE);
				DestroyWindow(hw);
			}

			if(ID_BT_SAVE == LOWORD(wp))
			{
				EnableWindow(hwmain, TRUE);
				SetOptions();
				DestroyWindow(hw);
			}
			break;

		case WM_DESTROY:
			hwcmd = NULL;
			break;

		case WM_SETFOCUS:
			SetFocus(hwcmd);
			break;

		case WM_CLOSE:
			return DefWindowProc(hw,msg,wp,lp);
			break;

		case WM_PAINT:
			dc=BeginPaint(hw,&ps);
			GetClientRect(hw,&rc);
			hbr=CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
			FillRect(dc,&rc,hbr);
			DeleteObject(hbr);
			EndPaint(hw,&ps);
			break;

		default: return DefWindowProc(hw,msg,wp,lp); 
	};
	return 0;
	
}

void CenterWindowOnScreen(HWND hWin){
	RECT rc;

	int screenWidth;
	int screenHeight;

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	GetClientRect(hwcmd,&rc);

	SetWindowPos(hwcmd, 0, (screenWidth - (rc.right - rc.left))/2, (screenHeight - (rc.bottom - rc.top))/2, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
}

void CheckForOptions(void)
{
	HMODULE hModule;

	hModule = GetModuleHandleA("aadp4olly.dll");
	
	if(Pluginreadintfromini(hModule, "hd_IsDebuggerPresent", CW_USEDEFAULT) == 1)
	{
			CheckDlgButton(hwPluginWin, ID_CB_ISDBGPRESENT, BST_CHECKED);
			bIsDbgPresent = TRUE;
	}
	else
	{
			CheckDlgButton(hwPluginWin, ID_CB_ISDBGPRESENT, BST_UNCHECKED);
			bIsDbgPresent = FALSE;
	}

	if(Pluginreadintfromini(hModule, "hd_NtGlobalFlags", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, ID_CB_NTGLOBALFLAGS, BST_CHECKED);
		bNtGlobalFlags = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, ID_CB_NTGLOBALFLAGS, BST_UNCHECKED);
		bNtGlobalFlags = FALSE;
	}

	if(Pluginreadintfromini(hModule, "hd_HeapFlags", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, ID_CB_HEAPFLAGS, BST_CHECKED);
		bHeapFlags = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, ID_CB_HEAPFLAGS, BST_UNCHECKED);
		bHeapFlags = FALSE;
	}

	if(Pluginreadintfromini(hModule, "hd_ZwSetInformationThread", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, ID_CB_ZWSETINFOTHREAD, BST_CHECKED);
		bZwSIT = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, ID_CB_ZWSETINFOTHREAD, BST_UNCHECKED);
		bZwSIT = FALSE;
	}

	if(Pluginreadintfromini(hModule, "hd_ZwQueryInformationProcess", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, ID_CB_ZWQUERYINFOPROCESS, BST_CHECKED);
		bZwQIP = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, ID_CB_ZWQUERYINFOPROCESS, BST_UNCHECKED);
		bZwQIP = FALSE;
	}

	if(Pluginreadintfromini(hModule, "hd_GetTickCount", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, ID_CB_GETTICKCOUNT, BST_CHECKED);
		bGetTickCount = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, ID_CB_GETTICKCOUNT, BST_UNCHECKED);
		bGetTickCount = FALSE;
	}

	if(Pluginreadintfromini(hModule, "hd_OutputDebugString", CW_USEDEFAULT) == 1)
	{
		CheckDlgButton(hwPluginWin, ID_CB_ODSTRING, BST_CHECKED);
		bODS = TRUE;
	}
	else
	{
		CheckDlgButton(hwPluginWin, ID_CB_ODSTRING, BST_UNCHECKED);
		bODS = FALSE;
	}

}

static HWND Createaadp4ollywindow(void) {
  HFONT hf;
  RECT rc;

	if(hwcmd == NULL){
		hwcmd = CreateWindowExA(0L,cmdlinewinclass, (LPCSTR)L"aadp4olly", 
			WS_POPUPWINDOW|WS_CAPTION|WS_VISIBLE|DS_3DLOOK,
			posx, posy, DX, DY, hwmain, NULL, (HINSTANCE)Plugingetvalue(VAL_HINST), NULL);
	}

	GetClientRect(hwcmd,&rc);

	hcbIsDbgPresent = CreateWindowExA(0, "BUTTON", "IsDebuggerPresent (via PEB)", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
								5,5,rc.right-50, rc.bottom-200, hwcmd, (HMENU)ID_CB_ISDBGPRESENT, (HINSTANCE)Plugingetvalue(VAL_HINST), NULL);

	hf=((HFONT *)Plugingetvalue(VAL_FONTS))[Plugingetvalue(VAL_DEFFONT)];
    SendMessage(hcbIsDbgPresent,WM_SETFONT,(WPARAM)hf,1);
    SendMessage(hcbIsDbgPresent,CB_LIMITTEXT,TEXTLEN-1,1);

	hcbNtGlobalFlags = CreateWindowExA(0, "BUTTON", "NtGlobalFlags", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
								5,25,rc.right-50, rc.bottom-200, hwcmd, (HMENU)ID_CB_NTGLOBALFLAGS, (HINSTANCE)Plugingetvalue(VAL_HINST), NULL);

	hf=((HFONT *)Plugingetvalue(VAL_FONTS))[Plugingetvalue(VAL_DEFFONT)];
    SendMessage(hcbNtGlobalFlags,WM_SETFONT,(WPARAM)hf,1);
    SendMessage(hcbNtGlobalFlags,CB_LIMITTEXT,TEXTLEN-1,1);


	hcbHeapFlags = CreateWindowExA(0, "BUTTON", "HeapFlags", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
								5,45,rc.right-50, rc.bottom-200, hwcmd, (HMENU)ID_CB_HEAPFLAGS, (HINSTANCE)Plugingetvalue(VAL_HINST), NULL);

	hf=((HFONT *)Plugingetvalue(VAL_FONTS))[Plugingetvalue(VAL_DEFFONT)];
    SendMessage(hcbHeapFlags,WM_SETFONT,(WPARAM)hf,1);
    SendMessage(hcbHeapFlags,CB_LIMITTEXT,TEXTLEN-1,1);

	hcbOds = CreateWindowExA(0, "BUTTON", "OutputDebugString (format string)", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
								5,65,rc.right-10, rc.bottom-200, hwcmd, (HMENU)ID_CB_ODSTRING, (HINSTANCE)Plugingetvalue(VAL_HINST), NULL);

	hf=((HFONT *)Plugingetvalue(VAL_FONTS))[Plugingetvalue(VAL_DEFFONT)];
    SendMessage(hcbOds,WM_SETFONT,(WPARAM)hf,1);
    SendMessage(hcbOds,CB_LIMITTEXT,TEXTLEN-1,1);

	hcbGetTickCount = CreateWindowExA(0, "BUTTON", "GetTickCount", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
								5,85,rc.right-50, rc.bottom-200, hwcmd, (HMENU)ID_CB_GETTICKCOUNT, (HINSTANCE)Plugingetvalue(VAL_HINST), NULL);

	hf=((HFONT *)Plugingetvalue(VAL_FONTS))[Plugingetvalue(VAL_DEFFONT)];
    SendMessage(hcbGetTickCount,WM_SETFONT,(WPARAM)hf,1);
    SendMessage(hcbGetTickCount,CB_LIMITTEXT,TEXTLEN-1,1);

	hcbZwTIP = CreateWindowExA(0, "BUTTON", "ZwSetInformationThread", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
								5,105,rc.right-50, rc.bottom-200, hwcmd, (HMENU)ID_CB_ZWSETINFOTHREAD, (HINSTANCE)Plugingetvalue(VAL_HINST), NULL);

	hf=((HFONT *)Plugingetvalue(VAL_FONTS))[Plugingetvalue(VAL_DEFFONT)];
    SendMessage(hcbZwTIP,WM_SETFONT,(WPARAM)hf,1);
    SendMessage(hcbZwTIP,CB_LIMITTEXT,TEXTLEN-1,1);

	hcbZwQIP = CreateWindowExA(0, "BUTTON", "ZwQueryInformationProcess", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
								5,125,rc.right-50, rc.bottom-200, hwcmd, (HMENU)ID_CB_ZWQUERYINFOPROCESS, (HINSTANCE)Plugingetvalue(VAL_HINST), NULL);

	hf=((HFONT *)Plugingetvalue(VAL_FONTS))[Plugingetvalue(VAL_DEFFONT)];
    SendMessage(hcbZwQIP,WM_SETFONT,(WPARAM)hf,1);
    SendMessage(hcbZwQIP,CB_LIMITTEXT,TEXTLEN-1,1);

	hbt_save = CreateWindowExA(0, "BUTTON", "Save", WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON,
								30,160,rc.right-180, rc.bottom-200, hwcmd, (HMENU)ID_BT_SAVE, (HINSTANCE)Plugingetvalue(VAL_HINST), NULL);

	hf=((HFONT *)Plugingetvalue(VAL_FONTS))[Plugingetvalue(VAL_DEFFONT)];
    SendMessage(hbt_save,WM_SETFONT,(WPARAM)hf,1);
    SendMessage(hbt_save,CB_LIMITTEXT,TEXTLEN-1,1);

	hbt_cancel = CreateWindowExA(0, "BUTTON", "Cancel", WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON,
								150,160,rc.right-180, rc.bottom-200, hwcmd, (HMENU)ID_BT_CANCEL, (HINSTANCE)Plugingetvalue(VAL_HINST), NULL);

	hf=((HFONT *)Plugingetvalue(VAL_FONTS))[Plugingetvalue(VAL_DEFFONT)];
    SendMessage(hbt_cancel,WM_SETFONT,(WPARAM)hf,1);
    SendMessage(hbt_cancel,CB_LIMITTEXT,TEXTLEN-1,1);

	hedt = CreateWindowExA(0, "STATIC", "CracksLatinoS! 2010", WS_CHILD|WS_VISIBLE|SS_CENTER,
								55,200,rc.right-110, rc.bottom-200, hwcmd, NULL, (HINSTANCE)Plugingetvalue(VAL_HINST), NULL);

	hf=((HFONT *)Plugingetvalue(VAL_FONTS))[Plugingetvalue(VAL_DEFFONT)];
    SendMessage(hedt,WM_SETFONT,(WPARAM)hf,1);
    SendMessage(hedt,CB_LIMITTEXT,TEXTLEN-1,1);

	CenterWindowOnScreen(hwcmd);
	
	if(hwcmd == NULL)
		return NULL;

	SetForegroundWindow(hwcmd);
	return hwcmd;
}

BOOL WINAPI DllEntryPoint(HINSTANCE hi,DWORD reason,LPVOID reserved) {
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

	if (Registerpluginclass(cmdlinewinclass,NULL,hinst,aadp4Ollyproc)<0)
		return -1;

	Addtolist(0,0,"aadp4plugin v0.1");
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
					MessageBoxA(hwcmd, "Can't patch BeingDebugged flag on PEB :(", "aadp4olly error", MB_ICONERROR);
			}
			if(bNtGlobalFlags)
			{
				if(!hd_NtGlobalFlags(pid))
					MessageBoxA(hwcmd, "Can't patch NtGlobalFlags on PEB :(", "aadp4olly error", MB_ICONERROR);
			}
			if(bHeapFlags)
			{
				if(!hd_HeapFlagsOnPEB(pid))
					MessageBoxA(hwcmd, "Can't patch HeapFlags on PEB :(", "aadp4olly error", MB_ICONERROR);
			}

			if(bZwQIP)
			{	
				if(!hd_HookZwQueryInformationProcess(pid))
					MessageBoxA(hwcmd, "Can't patch ZwQueryInformationProcess :(", "aadp4olly error", MB_ICONERROR);
			}

			if(bZwSIT)
			{
				if(!hd_HookZwSetInformationThread(pid))
					MessageBoxA(hwcmd, "Can't patch ZwSetInformationThread :(", "aadp4olly error", MB_ICONERROR);
			}

			if(bGetTickCount)
			{
				if(!hd_GetTickCount(pid))
					MessageBoxA(hwcmd, "Can't patch GetTickCount :(", "aadp4olly error", MB_ICONERROR);
			}

			if(bODS)
			{
				if(!hd_HookOutputDebugString(pid))
					MessageBoxA(hwcmd, "Can't patch OutputDebugStringA :(", "aadp4olly error", MB_ICONERROR);
			}
		}

	}
}

extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) {
  if (origin!=PM_MAIN)
    return 0;                          // No pop-up menus in OllyDbg's windows
  strcpy_s(data,sizeof("0 &Options\tAlt+Q|1 &Help,2 &About"), "0 &Options\tAlt+Q|1 &Help,2 &About");
  return 1;
}

extc int _export cdecl ODBG_Pluginshortcut(int origin,int ctrl,int alt,int shift,int key,void *item) {
  if(origin == PM_MAIN)
  {
	  if (ctrl==0 && alt==1 && shift==0 && key=='Q') 
	  {
		Createaadp4ollywindow();
		CheckForOptions();
		EnableWindow(hwmain, FALSE);
		return 1;
	  }                       // Shortcut recognized
  }
  return 0;                            // Shortcut not recognized
};

extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) {
  if (origin!=PM_MAIN)
    return;

  switch (action) {
    case 0:                            // "Command line", creates window
      Createaadp4ollywindow();
	  CheckForOptions();
      break;
    case 1:                            // "Help", opens help file
      //WinHelp(hwmain,(LPCWSTR)L"aadp4olly.hlp",HELP_CONTENTS,0);
		ShellExecuteA(NULL, "open", "http://code.google.com/p/aadp", 0, 0, SW_SHOWNORMAL);
      break;
    case 2:                            // "About", displays plugin info
      MessageBoxA(hwmain,
		  "aadp4olly plugin v0.1\nWritten by +NCR/CRC! [ReVeRsEr]",
        "aadp4olly",MB_OK|MB_ICONINFORMATION);
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

extc void _export cdecl ODBG_Plugindestroy(void) {
  Unregisterpluginclass(cmdlinewinclass);
}

