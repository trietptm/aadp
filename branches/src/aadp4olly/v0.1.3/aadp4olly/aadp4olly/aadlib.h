#pragma comment(lib, "aadlib.lib")

bool __stdcall hd_GetTickCount(DWORD pid);
bool __stdcall hd_HeapFlagsOnPEB(DWORD pid);
bool __stdcall hd_NtGlobalFlags(DWORD pid);
bool __stdcall hd_IsDebuggerPresent(DWORD pid);
bool __stdcall hd_HookZwQueryInformationProcess(DWORD pid);
bool __stdcall hd_HookZwSetInformationThread(DWORD pid);
bool __stdcall hd_HookOutputDebugString(DWORD pid);

HANDLE __stdcall hd_GetProcHandleByPid(DWORD pid);
DWORD __stdcall hd_GetProcPidByHandle(HANDLE pHandle);
void* __stdcall hd_GetPEBAddress(DWORD pid);
bool __stdcall hd_DllInjector(DWORD ProcessId, char* DLLLibPath, bool WaitForResponse);