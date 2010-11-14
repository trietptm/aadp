#pragma comment(lib, "aadlib.lib")

__declspec(dllexport) bool __stdcall hd_GetTickCount(DWORD pid);
__declspec(dllexport) bool __stdcall hd_HeapFlagsOnPEB(DWORD pid);
__declspec(dllexport) bool __stdcall hd_NtGlobalFlags(DWORD pid);
__declspec(dllexport) bool __stdcall hd_IsDebuggerPresent(DWORD pid);
__declspec(dllexport) bool __stdcall hd_HookZwQueryInformationProcess(DWORD pid);
__declspec(dllexport) bool __stdcall hd_HookZwSetInformationThread(DWORD pid);
__declspec(dllexport) bool __stdcall hd_HookOutputDebugString(DWORD pid);

__declspec(dllexport) HANDLE __stdcall hd_GetProcHandleByPid(DWORD pid);
__declspec(dllexport) DWORD __stdcall hd_GetProcPidByHandle(HANDLE pHandle);
__declspec(dllexport) void* __stdcall hd_GetPEBAddress(DWORD pid);
__declspec(dllexport) bool __stdcall hd_DllInjector(DWORD ProcessId, char* DLLLibPath, bool WaitForResponse);