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

	if (ollydbgversion < PLUGIN_VERSION)
		return -1;

	Addtolist(0,0,"aadp4plugin v0.2");
	Addtolist(0,-1,"  Written by +NCR/CRC! [ReVeRsEr]");

	InitGlobalArrays();

	CheckForOptions(AadbgTricksDlgHwnd, TABAADBTRICKS);
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
}
