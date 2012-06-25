/*************************************************************
 Author		: David A. Jones
 File Name	: MemLeakDetect.h
 Date		: July 30, 2004
 Synopsis	:		 
			A trace memory feature for source code to trace and
			find memory related bugs. 

 Future		:
				1) Memory corruption
				2) Freeing memory without allocating
				3) Freeing memory twice
				4) Not Freeing memory at all
				5) over running memory boundardies

		July 2009: tim.s.stevens@bt.com (UNICODE/ANSI 32 bit only, more secure CRT with VS 2008).
		Feb  2011: Doug Rogers <rogers.doug@gmail.com>, igor.jambrek@gmail.com, OfekSH & tim. (Compiles as 64 & 32 bit).
		Based on http://www.codeproject.com/cpp/MemLeakDetect.asp

****************************************************************/
/*
Compiles clean in Visual Studio 2008 SP1 in 32 & 64 UNICODE and MultiByte builds.

By default, disabled in Release mode, since it relies on the Debug MS Runtime DLLs, the licence terms of which only
allow redistribution in Release mode. However, if you do want to use it in Release mode, then comment out the
"#ifdef _DEBUG" lines that guard the complete MemLeakDetect.h & .cpp files, and link against the Debug runtimes
(e.g. /MTd instead of /MT) in Release mode.

Please don't use precompiled headers for this file.

To catch most malloc/free or new/delete leaks, simply add this block of code (& #define MEMORY_LEAK_CHECK)
at the application level:

#ifdef _DEBUG
	#ifdef MEMORY_LEAK_CHECK
		#include "MemLeakDetect.h"
		static CMemLeakDetect memLeakDetect;
	#endif
#endif

A typical leak might be:

	int *pfoo = new int[1000];

Then forgetting to do

	delete [] pfoo;

Then when running under a debugger, if there is a leak, you'll get this kind of output in the Output pane.
You'll also get files with names like "mldetector-(AppName.exe)_Feb16-2011__21-53-43.log"
written to your %TEMP% directory:

Memory Leak(1)------------------->
Memory Leak <0xBC> bytes(86) occurance(0)
c:\code\ta2svn\sandbox\pjh\software\common\memleakdetect.cpp(201): 0x0044B7C3->CMemLeakDetect::addMemoryTrace()
c:\code\ta2svn\sandbox\pjh\software\common\memleakdetect.cpp(140): 0x0044B4B2->catchMemoryAllocHook()
0x0012D874->_malloc_dbg()
0x0012D874->_malloc_dbg()
0x0012D874->_malloc_dbg()
0x0012D874->malloc()
0x0012D874->??2@YAPAXI@Z()
f:\dd\vctools\crt_bld\self_x86\crt\src\newaop.cpp(7): 0x004B4D1E->operator new[]()
c:\code\ta2svn\sandbox\pjh\software\hw_app\hw_app.cpp(145): 0x00442276->wmain()
f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c(579): 0x004B56C8->__tmainCRTStartup()
f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c(399): 0x004B550F->wmainCRTStartup()
0x0012D874->RegisterWaitForInputIdle()

-----------------------------------------------------------
Total 1 Memory Leaks: 86 bytes Total Alocations 276

You can then double-click in the Output pane on the leak ((145) in the example above) and be taken to the source line
which caused the leak.
*/

#if !defined(MEMLEAKDETECT_H)
#define MEMLEAKDETECT_H
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC

#include <map>
#define _CRTBLD
#include <windows.h>
#include <..\crt\src\dbgint.h>
#include <ImageHlp.h>
#include <crtdbg.h>

#pragma comment( lib, "imagehlp.lib" )

using namespace std;

// if you want to use the custom stackwalker otherwise
// comment this line out
#define MLD_CUSTOMSTACKWALK			1
//
#define MLD_MAX_NAME_LENGTH			256
#define MLD_MAX_TRACEINFO			63
#define MLD_TRACEINFO_EMPTY			_T("")
#define MLD_TRACEINFO_NOSYMBOL		_T("?(?)")

#ifdef  MLD_CUSTOMSTACKWALK
#define MLD_STACKWALKER				symStackTrace2
#else
#define MLD_STACKWALKER				symStackTrace
#endif

#define AfxTrace MyTrace

#ifndef _WIN64
typedef DWORD ADDR;
typedef PIMAGEHLP_SYMBOL IMAGE_SYM;
typedef IMAGEHLP_LINE IMAGE_LN;
#else
typedef DWORD64 ADDR;
typedef PIMAGEHLP_SYMBOL64 IMAGE_SYM;
typedef IMAGEHLP_LINE64 IMAGE_LN;
#endif

class CMemLeakDetect
{
	public:

		typedef struct 	{
				ADDRESS				addrPC;
				ADDRESS				addrFrame;
			
		} STACKFRAMEENTRY;

		typedef struct tagAllocBlockInfo
		{
			//	Added constructor to zero memory - thanks to bugfix from OfekSH.
				tagAllocBlockInfo() { ZeroMemory(traceinfo, sizeof(traceinfo) ); }
				void*				address; 
				size_t				size;
				TCHAR				fileName[MLD_MAX_NAME_LENGTH];
				DWORD				lineNumber;
				DWORD				occurance;
				STACKFRAMEENTRY		traceinfo[MLD_MAX_TRACEINFO];

		} AllocBlockInfo;

		//typedef int POSITION;
		typedef map<LPVOID, AllocBlockInfo>				KEYMAP;
		typedef map<LPVOID, AllocBlockInfo>::iterator	POSITION;
		typedef pair<LPVOID, AllocBlockInfo>			KEYVALUE;

		class CMapMem
		{
			public:

				KEYMAP			m_Map;
				POSITION		m_Pos;

				inline BOOL Lookup(LPVOID pAddr,  AllocBlockInfo& aInfo) { 

					m_Pos = m_Map.find(pAddr);
					//
					if (m_Pos == m_Map.end())
					{
						return FALSE;
					}
					//
					pAddr = m_Pos->first;
					aInfo = m_Pos->second;

					return TRUE;
				};

				inline POSITION end() { 

					return m_Map.end(); 
				};

				inline void RemoveKey(LPVOID pAddr) { 
					
					m_Map.erase(pAddr);
				};

				inline void RemoveAll() {
					m_Map.clear();
				};

				void SetAt(LPVOID pAddr, AllocBlockInfo& aInfo) {

					m_Map[pAddr] = aInfo;
				};

				inline POSITION GetStartPosition() { 
					POSITION pos = m_Map.begin(); 
					return pos;
				};

				inline void GetNextAssoc(POSITION& pos, LPVOID& rAddr, AllocBlockInfo& aInfo) {

					rAddr = pos->first;
					aInfo = pos->second;
					pos++;
				};

				void InitHashTable(int preAllocEntries, BOOL flag) 	{
					 preAllocEntries	= NULL;
					 flag				= NULL;
				};

		};

		CMemLeakDetect();
		~CMemLeakDetect();
		void Init();
		void End();
		void addMemoryTrace(void* addr,  size_t asize,  TCHAR *fname, DWORD lnum);
		void redoMemoryTrace(void* addr,  void* oldaddr, size_t asize,  TCHAR *fname, DWORD lnum);
		void removeMemoryTrace(void* addr, void* realdataptr);
		void cleanupMemoryTrace();
		void dumpMemoryTrace();
		//

		//CMap<LPVOID, LPVOID, AllocBlockInfo, AllocBlockInfo> m_AllocatedMemoryList;
		CMapMem			 m_AllocatedMemoryList;
	DWORD memoccurance;
	bool  isLocked;
	//
	private:
		typedef USHORT (WINAPI *CaptureStackBackTraceType)(__in ULONG, __in ULONG, __out PVOID*, __out_opt PULONG);
		HMODULE m_k32;
		CaptureStackBackTraceType m_func;
		BOOL initSymInfo(TCHAR* lpUserPath);
		BOOL cleanupSymInfo();
		void symbolPaths( TCHAR* lpszSymbolPaths);
		void symStackTrace(STACKFRAMEENTRY* pStacktrace);
		void symStackTrace2(STACKFRAMEENTRY* pStacktrace);
		BOOL symFunctionInfoFromAddresses(ADDR fnAddress, ADDR stackAddress, TCHAR *lpszSymbol, UINT BufSizeTCHARs);

		BOOL symSourceInfoFromAddress(ADDR address, TCHAR* lpszSourceInfo);
		BOOL symModuleNameFromAddress(ADDR address, TCHAR* lpszModule);

		HANDLE				m_hProcess;
		PIMAGEHLP_SYMBOL	m_pSymbol;
		DWORD				m_dwsymBufSize;
};
#endif
#endif
