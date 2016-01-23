aadp is a collection of plugins that aims to hide most of the well knowns debuggers from most of anti-debugging techniques.

Currently, we have plugins for the followings debuggers:

  * Ollydbg v1.10
  * ImmunityDebugger v1.73
  * ImmunityDebugger >= v1.80
  * OllySND v1.10 (Un-Official release)

Also, there are plans to develop plugins for the following debuggers:
  * IDA Debugger (Local & Remote)
  * WinDbg (Local & Remote)

aadp4olly, aadp4immdbg and aadp4ollysnd are distributed under the [LGPLv3](http://www.gnu.org/licenses/lgpl.html) license.

### Latest Changes ###


---


### aadlib ###

  * 9/02/2012: Source code released under the [LGPLv3](http://www.gnu.org/licenses/lgpl.html) license.

### aadp4immdbg ###

**Version 0.2.1** (20/02/2012)

  * I just added a build of _aadp4immdbg_ for _ImmunityDebugger_ >= 1.80. I built it using the PDK present in the **mapimp** project http://code.google.com/p/mapimp/. I didn't do many tests so use it under your own risk :P. If you find a bug, report it!.

**Version 0.2.1** (30/11/2010)

  * first stable release for ImmunityDebugger.

### aadp4immdbg TODO List ###
  * add kernel mode support
  * add x64 compatibility mode
  * add bugfixes for ImmunityDebugger


---


### aadp4olly ###

**Version 0.2.1** (30/11/2010)

  * fixed bugs in the SuspendThread, OutputDebugString, Process32Next, Module32Next, UnhandledExceptionFilter and the TerminateProcess functions under Windows XP SP0 reported by LCF-AT.

**Version 0.2** (29/11/2010)

added Anti-Antidebugging features for the following tricks:
  * BlockInput
  * SuspendThread
  * UnhandledExceptionFilter
  * Process32Next
  * Module32Next
  * ZwQuerySystemInformation
  * ZwQueryObject
  * TerminateProcess
  * !ZwOPenProcess
  * FindWindow

  * now, the plugin should support XP (ALL), Windows Vista (ALL) and Windows 7 (ALL) OS.

**Version 0.1.3** (21/11/2010)

  * Fixed another issue with the search pattern for the "Zw" functions in XP SP0 (reported by LCF-AT).

**Version 0.1.2** (18/11/2010)

  * Fixed a little issue with the search pattern for the "Zw" functions in XP SP0 (reported by LCF-AT).

  * **IMPORTANT NOTE**: if you are not using XP SP0 is better to use the v0.1.1.


**Version 0.1.1** (14/11/2010)

  * Fixed a bug when the plugin's window is closed (reported by marciano).

**Version 0.1** (13/11/2010)

aadp4olly hide Ollydbg from the following tricks:

  * IsDebuggerPresent (via PEB patching, BeingDebugged flag)
  * NtGlobalFlags
  * HeapFlags
  * GetTickCount
  * ZwQueryInformationProcess
  * ZwSetInformationThread
  * OutputDebugStringA

**Screenhots**

![http://aadpproject.files.wordpress.com/2010/12/sc_aadp_2010-11-29_21-34.png](http://aadpproject.files.wordpress.com/2010/12/sc_aadp_2010-11-29_21-34.png)

### aadp4olly TODO List ###
  * add kernel mode support (x86 & x64)
  * add x64 compatibility mode (Olly running under WOW)
  * add bugfixes for Ollydbg

Thanks to [ReversingLabs](http://www.reversinglabs.com) for [TitanEngine](http://www.reversinglabs.com/products/TitanEngine.php).