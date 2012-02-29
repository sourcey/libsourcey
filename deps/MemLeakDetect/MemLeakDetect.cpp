/*************************************************************
 Author		: David A. Jones
 File Name	: MemLeakDetect.h
 Date		: July 30, 2004
 Synopsis		 
			A trace memory feature for source code to trace and
			find memory related bugs. 


****************************************************************/
// See MemLeakDetect.h for full history.
// Based on http://www.codeproject.com/cpp/MemLeakDetect.asp
#ifdef _DEBUG
#include <tchar.h>
#include "MemLeakDetect.h"
#include <fstream>
#include <time.h>

#include <Psapi.h>					// Only needed for GetModuleBaseName().
#pragma comment(lib, "Psapi.lib")	// Only needed for GetModuleBaseName().

//#pragma warning(disable:4312)	// 'type cast' : conversion from 'long' to 'void *' of greater size
//#pragma warning(disable:4313)
//#pragma warning(disable:4267)
#pragma warning(disable:4100)	// Unreferenced formal parameter.

static CMemLeakDetect*	g_pMemTrace			= NULL;
static _CRT_ALLOC_HOOK	pfnOldCrtAllocHook	= NULL;

static int catchMemoryAllocHook(int	allocType, 
						 void	*userData, 
						 size_t size, 
						 int	blockType, 
						 long	requestNumber, 
		  const unsigned char	*filename, // Can't be UNICODE
						 int	lineNumber) ;

static int MyTrace(LPCTSTR lpszFormat, ...);


static void DeleteOldTempFiles(const TCHAR dir[], const TCHAR type[], int DaysAge);

static int MyTrace(LPCTSTR lpszFormat, ...)
{
 	va_list args;
	va_start( args, lpszFormat);
#ifndef UNICODE
	char buffer[1024];
	vsprintf_s( buffer, lpszFormat, args );
	return _CrtDbgReport(_CRT_WARN,NULL,NULL,NULL,buffer);
#else
	TCHAR buffer[1024];
	vswprintf_s( buffer, lpszFormat, args );
	char fmtbuf[1024] ;
	WideCharToMultiByte(CP_ACP, 0, buffer, -1,
		fmtbuf, 1024, NULL, NULL ) ;

	return _CrtDbgReport(_CRT_WARN,NULL,NULL,NULL,fmtbuf);
#endif
}

static int catchMemoryAllocHook(int	allocType, 
						 void	*userData, 
						 size_t size, 
						 int	blockType, 
						 long	requestNumber, 
		  const unsigned char	*filename,  // Can't be UNICODE
						 int	lineNumber)
{
	_CrtMemBlockHeader *pCrtHead;
	long prevRequestNumber;
#ifdef UNICODE
	wchar_t Wname[1024] ;
	Wname[0] = L'\0' ;
#endif

	// internal C library internal allocations
	if ( blockType == _CRT_BLOCK )
	{
		return( TRUE );
	}
	// check if someone has turned off mem tracing
	if  ((( _CRTDBG_ALLOC_MEM_DF & _crtDbgFlag) == 0) && 
		(( allocType			== _HOOK_ALLOC)		|| 
			( allocType			== _HOOK_REALLOC)))
	{
		if (pfnOldCrtAllocHook)
		{
			pfnOldCrtAllocHook(allocType, userData, size, blockType, requestNumber, filename, lineNumber);
		}
		return TRUE;
	}

	// protect if mem trace is not initialized
	if (g_pMemTrace == NULL)
	{
		if (pfnOldCrtAllocHook)
		{
			pfnOldCrtAllocHook(allocType, userData, size, blockType, requestNumber, filename, lineNumber);
		}
		return TRUE;
	}

	// protect internal mem trace allocs
	if (g_pMemTrace->isLocked)
	{
		if (pfnOldCrtAllocHook)
		{
			pfnOldCrtAllocHook(allocType, userData, size, blockType, requestNumber, filename, lineNumber);
		}
		return( TRUE);
	}
	// lock the function
	g_pMemTrace->isLocked = true;
	//
#ifdef UNICODE
	int len ;
	if (NULL != filename)
	{
		len = (int)strlen((char *)filename) + 1 ;
		MultiByteToWideChar(CP_ACP, 0, (char *)filename, len, Wname, len) ;
	}
	else
		len = 0 ;
#else
	#define Wname (char*)filename
#endif
	if (allocType == _HOOK_ALLOC)
	{
		g_pMemTrace->addMemoryTrace((void *) requestNumber, size, Wname, lineNumber);
	}
	else
	if (allocType == _HOOK_REALLOC)
	{
		if (_CrtIsValidHeapPointer(userData))
		{
			pCrtHead = pHdr(userData);
			prevRequestNumber = pCrtHead->lRequest;
			//
			if (pCrtHead->nBlockUse == _IGNORE_BLOCK)
			{
				if (pfnOldCrtAllocHook)
				{
					pfnOldCrtAllocHook(allocType, userData, size, blockType, requestNumber, filename, lineNumber);
				}
				goto END;
			}
	   		g_pMemTrace->redoMemoryTrace((void *) requestNumber, (void *) prevRequestNumber, size, Wname, lineNumber);
		}
	}
	else
	if (allocType == _HOOK_FREE)
	{
		if (_CrtIsValidHeapPointer(userData))
		{
			pCrtHead = pHdr(userData);
			requestNumber = pCrtHead->lRequest;
			//
			if (pCrtHead->nBlockUse == _IGNORE_BLOCK)
			{
				if (pfnOldCrtAllocHook)
				{
					pfnOldCrtAllocHook(allocType, userData, size, blockType, requestNumber, filename, lineNumber);
				}
				goto END;
			}
	   		g_pMemTrace->removeMemoryTrace((void *) requestNumber, userData);
		}
	}
END:
	// unlock the function
	g_pMemTrace->isLocked = false;
	return TRUE;
}

void CMemLeakDetect::addMemoryTrace(void* addr,  size_t asize,  TCHAR *fname, DWORD lnum)
{
	AllocBlockInfo ainfo;
	//
	if (m_AllocatedMemoryList.Lookup(addr, ainfo))
	{
		// already allocated
		AfxTrace(_T("ERROR!CMemLeakDetect::addMemoryTrace() Address(0x%p) already allocated\n"), addr);
		return;
	}
	//
	ainfo.address		= addr;
	ainfo.lineNumber	= lnum;
	ainfo.size			= asize;
	ainfo.occurance		= memoccurance++;
	MLD_STACKWALKER(&ainfo.traceinfo[0]);
	//
	if (fname)
		_tcsncpy_s(&ainfo.fileName[0], MLD_MAX_NAME_LENGTH, fname, MLD_MAX_NAME_LENGTH);

	else
	  ainfo.fileName[0] = 0;
	//
	m_AllocatedMemoryList.SetAt(addr, ainfo);
};
void CMemLeakDetect::redoMemoryTrace(void* addr,  void* oldaddr, size_t asize,  TCHAR *fname, DWORD lnum)
{
	AllocBlockInfo ainfo;

	if (m_AllocatedMemoryList.Lookup(oldaddr,(AllocBlockInfo &) ainfo))
	{
		m_AllocatedMemoryList.RemoveKey(oldaddr);
	}
	else
	{
		AfxTrace(_T("ERROR!CMemLeakDetect::redoMemoryTrace() didnt find Address(0x%08X) to free\n"), oldaddr);
	}
	//
	ainfo.address		= addr;
	ainfo.lineNumber	= lnum;
	ainfo.size			= asize;
	ainfo.occurance		= memoccurance++;
	MLD_STACKWALKER(&ainfo.traceinfo[0]);
	//
	if (fname)
		_tcsncpy_s(&ainfo.fileName[0], MLD_MAX_NAME_LENGTH, fname, MLD_MAX_NAME_LENGTH);
	else
	  ainfo.fileName[0] = 0;

	m_AllocatedMemoryList.SetAt(addr, ainfo);
};
void CMemLeakDetect::removeMemoryTrace(void* addr, void* realdataptr)
{
	AllocBlockInfo ainfo;
	//
	if (m_AllocatedMemoryList.Lookup(addr,(AllocBlockInfo &) ainfo))
	{
		m_AllocatedMemoryList.RemoveKey(addr);
	}
	else
	{
	   //freeing unallocated memory
		AfxTrace(_T("ERROR!CMemLeakDetect::removeMemoryTrace() didnt find Address(0x%08X) to free\n"), addr);
	}
};
void CMemLeakDetect::cleanupMemoryTrace()
{
	m_AllocatedMemoryList.RemoveAll();
};

void CMemLeakDetect::dumpMemoryTrace()
{
	POSITION			pos;
	LPVOID				addr;
	AllocBlockInfo		ainfo;
	TCHAR				buf[MLD_MAX_NAME_LENGTH];
	TCHAR				fileName[MLD_MAX_NAME_LENGTH];

	TCHAR				symInfo[MLD_MAX_NAME_LENGTH];
	TCHAR				srcInfo[MLD_MAX_NAME_LENGTH];
	size_t				totalSize						= 0;
	int					numLeaks						= 0;
	STACKFRAMEENTRY*	p								= 0;

	ofstream myfile;
#ifdef UNICODE
		char dest[1024] ;
#endif
	struct tm timeinfo;
	__time64_t long_time;
	_time64(&long_time);
	// Convert to local time.
	_localtime64_s(&timeinfo, &long_time);

	TCHAR TempDir[MAX_PATH];
	TCHAR ProcName[MAX_PATH];
	GetTempPath(MAX_PATH, TempDir);
	ProcName[0] = _T('\0');
	GetModuleBaseName(GetCurrentProcess(), NULL, ProcName, sizeof(ProcName)/sizeof(TCHAR));

	_stprintf_s(fileName, MLD_MAX_NAME_LENGTH, _T("%smldetector-(%s)_"), TempDir, ProcName); 
	_tcsftime(buf,MLD_MAX_NAME_LENGTH, _T("%b%d-%Y__%H-%M-%S.log"),&timeinfo);

	_tcscat_s(fileName,MLD_MAX_NAME_LENGTH, buf);

	myfile.open (fileName); 

	DeleteOldTempFiles(TempDir, _T("mldetector-(*.log"), 7);

	//
	_tcscpy_s(symInfo, MLD_MAX_NAME_LENGTH, MLD_TRACEINFO_NOSYMBOL);
	_tcscpy_s(srcInfo, MLD_MAX_NAME_LENGTH, MLD_TRACEINFO_NOSYMBOL);
	//
	pos = m_AllocatedMemoryList.GetStartPosition();
	//

	while(pos != m_AllocatedMemoryList.end())
	{
		numLeaks++;
		_stprintf_s(buf, MLD_MAX_NAME_LENGTH, _T("Memory Leak(%d)------------------->\n"), numLeaks);
		AfxTrace(buf);
#ifdef UNICODE
		WideCharToMultiByte( CP_ACP, 0, buf, -1, dest, 1024, NULL, NULL );
		myfile << dest;
#else
		myfile << buf;
#endif
		//
		m_AllocatedMemoryList.GetNextAssoc(pos, (LPVOID &) addr, (AllocBlockInfo&) ainfo);

		if (ainfo.fileName[0] != NULL)
		{
			_stprintf_s(buf, MLD_MAX_NAME_LENGTH, _T("Memory Leak <0x%p> bytes(%d) occurance(%d) %s(%d)\n"), 
					ainfo.address, ainfo.size, ainfo.occurance, ainfo.fileName, ainfo.lineNumber);
		}
		else
		{
			_stprintf_s(buf, MLD_MAX_NAME_LENGTH, _T("Memory Leak <0x%p> bytes(%d) occurance(%d)\n"), 
					ainfo.address, ainfo.size, ainfo.occurance);
		}
		//
		AfxTrace(buf);
#ifdef UNICODE
		WideCharToMultiByte( CP_ACP, 0, buf, -1, dest, 1024, NULL, NULL );
		myfile << dest;
#else
		myfile << buf;
#endif
		//
		p = &ainfo.traceinfo[0];
		while(p[0].addrPC.Offset)
		{
			symFunctionInfoFromAddresses( p[0].addrPC.Offset, p[0].addrFrame.Offset, symInfo, MLD_MAX_NAME_LENGTH);
			symSourceInfoFromAddress(     p[0].addrPC.Offset, srcInfo );
			_stprintf_s(buf, MLD_MAX_NAME_LENGTH, _T("%s->%s()\n"), srcInfo, symInfo);
			AfxTrace(_T("%s->%s()\n"), srcInfo, symInfo);
#ifdef UNICODE
		WideCharToMultiByte( CP_ACP, 0, buf, -1, dest, 1024, NULL, NULL );
		myfile << dest;
#else
		myfile << buf;
#endif
			p++;
		}
		totalSize += ainfo.size;
	}
	_stprintf_s(buf, MLD_MAX_NAME_LENGTH, _T("\n-----------------------------------------------------------\n"));
	AfxTrace(buf);
#ifdef UNICODE
		WideCharToMultiByte( CP_ACP, 0, buf, -1, dest, 1024, NULL, NULL );
		myfile << dest;
#else
		myfile << buf;
#endif
	if(!totalSize) 
	{
		_stprintf_s(buf, MLD_MAX_NAME_LENGTH, _T("No Memory Leaks Detected for %d Allocations\n\n"), memoccurance);
		AfxTrace(buf);
#ifdef UNICODE
		WideCharToMultiByte( CP_ACP, 0, buf, -1, dest, 1024, NULL, NULL );
		myfile << dest;
#else
		myfile << buf;
#endif
	}
	else
	{
		_stprintf_s(buf, MLD_MAX_NAME_LENGTH, _T("Total %d Memory Leaks: %d bytes Total Alocations %d\n\n"), numLeaks, totalSize, memoccurance);
	}

	AfxTrace(buf);

#ifdef UNICODE
	WideCharToMultiByte( CP_ACP, 0, buf, -1, dest, 1024, NULL, NULL );
	const TCHAR *umb = _T("Unicode");
	myfile << dest;
#else
	myfile << buf;
	const TCHAR *umb = _T("Multibyte");
#endif
#ifdef _WIN64
	const TCHAR *w64 = _T("64 bit");
#else
	const TCHAR *w64 = _T("32 bit");
#endif
#ifdef NDEBUG
	const TCHAR *dbg = _T("release build.");
#else
	const TCHAR *dbg = _T("debug build.");
#endif
	_stprintf_s(TempDir, MAX_PATH, _T("%s %s %s\n"), umb, w64, dbg);
#ifdef UNICODE
	WideCharToMultiByte( CP_ACP, 0, TempDir, -1, dest, 1024, NULL, NULL );
	myfile << dest;
	AfxTrace(TempDir);
#else
	myfile << TempDir;
	AfxTrace(TempDir);
#endif

	myfile.close();
}

void CMemLeakDetect::Init()
{
	m_func = (CaptureStackBackTraceType)(GetProcAddress( m_k32 = LoadLibrary(_T("kernel32.dll")), "RtlCaptureStackBackTrace"));
	m_dwsymBufSize		= (MLD_MAX_NAME_LENGTH + sizeof(PIMAGEHLP_SYMBOL));
	m_hProcess			= GetCurrentProcess();
	m_pSymbol				= (IMAGE_SYM)GlobalAlloc( GMEM_FIXED, m_dwsymBufSize);

	m_AllocatedMemoryList.InitHashTable(10211, TRUE);
	initSymInfo( NULL );
	isLocked				= false;
	g_pMemTrace			= this;
	pfnOldCrtAllocHook	= _CrtSetAllocHook( catchMemoryAllocHook ); 
}
void CMemLeakDetect::End()
{
	isLocked				= true;
	_CrtSetAllocHook(pfnOldCrtAllocHook);
	dumpMemoryTrace();
	cleanupMemoryTrace();
	cleanupSymInfo();
	GlobalFree(m_pSymbol);
	g_pMemTrace				= NULL;
	FreeLibrary(m_k32);
}

CMemLeakDetect::CMemLeakDetect()
{
	Init();
}

CMemLeakDetect::~CMemLeakDetect()
{
	End();
}

// PRIVATE STUFF
void CMemLeakDetect::symbolPaths( TCHAR* lpszSymbolPath)
{
	TCHAR lpszPath[MLD_MAX_NAME_LENGTH];

   // Creating the default path where the dgbhelp.dll is located
   // ".;%_NT_SYMBOL_PATH%;%_NT_ALTERNATE_SYMBOL_PATH%;%SYSTEMROOT%;%SYSTEMROOT%\System32;"
	_tcscpy_s( lpszSymbolPath, MLD_MAX_NAME_LENGTH, _T(".;..\\;..\\..\\"));

	// environment variable _NT_SYMBOL_PATH
	if ( GetEnvironmentVariable(_T("_NT_SYMBOL_PATH"), lpszPath, MLD_MAX_NAME_LENGTH ))
	{
		_tcscat_s( lpszSymbolPath, MLD_MAX_NAME_LENGTH, _T(";"));
		_tcscat_s( lpszSymbolPath, MLD_MAX_NAME_LENGTH, lpszPath );
	}

	// environment variable _NT_ALTERNATE_SYMBOL_PATH
	if ( GetEnvironmentVariable( _T("_NT_ALTERNATE_SYMBOL_PATH"), lpszPath, MLD_MAX_NAME_LENGTH ))
	{
		_tcscat_s( lpszSymbolPath, MLD_MAX_NAME_LENGTH, _T(";"));
		_tcscat_s( lpszSymbolPath, MLD_MAX_NAME_LENGTH, lpszPath );
	}

	// environment variable SYSTEMROOT
	if ( GetEnvironmentVariable( _T("SYSTEMROOT"), lpszPath, MLD_MAX_NAME_LENGTH ) )
	{
	    _tcscat_s( lpszSymbolPath, MLD_MAX_NAME_LENGTH, _T(";"));
		_tcscat_s( lpszSymbolPath, MLD_MAX_NAME_LENGTH, lpszPath);
		_tcscat_s( lpszSymbolPath, MLD_MAX_NAME_LENGTH, _T(";"));

		// SYSTEMROOT\System32
		_tcscat_s( lpszSymbolPath, MLD_MAX_NAME_LENGTH, lpszPath );
		_tcscat_s( lpszSymbolPath, MLD_MAX_NAME_LENGTH, _T("\\System32"));
	}
}

BOOL CMemLeakDetect::cleanupSymInfo()
{
	return SymCleanup( GetCurrentProcess() );
}

// Initializes the symbol files
BOOL CMemLeakDetect::initSymInfo( TCHAR* lpszUserSymbolPath )
{
	TCHAR   lpszSymbolPath[MLD_MAX_NAME_LENGTH];
    DWORD   symOptions = SymGetOptions();

	symOptions |= SYMOPT_LOAD_LINES; 
	symOptions &= ~SYMOPT_UNDNAME;
	SymSetOptions( symOptions );

    // Get the search path for the symbol files
	symbolPaths( lpszSymbolPath);
	//
	if (lpszUserSymbolPath)
	{
		_tcscat_s(lpszSymbolPath, MLD_MAX_NAME_LENGTH, _T(";"));
		_tcscat_s(lpszSymbolPath, MLD_MAX_NAME_LENGTH, lpszUserSymbolPath);
	}

#ifdef UNICODE
	int len = (int)_tcslen(lpszSymbolPath) + 1 ;
	char dest[1024] ;
	WideCharToMultiByte( CP_ACP, 0, lpszSymbolPath, -1, dest, len, NULL, NULL );
	BOOL bret = SymInitialize( GetCurrentProcess(), dest, TRUE);
#else
	BOOL bret = SymInitialize( GetCurrentProcess(), lpszSymbolPath, TRUE) ;
#endif
	return bret;
}

/*void CMemLeakDetect::symStackTrace(STACKFRAMEENTRY* pStacktrace )
{
	STACKFRAME     callStack;
	BOOL           bResult;
	CONTEXT        context;
	HANDLE		   hThread  = GetCurrentThread();

	// get the context
	memset( &context, NULL, sizeof(context) );
	context.ContextFlags = CONTEXT_FULL;
	if ( !GetThreadContext( hThread, &context ) )
	{
    //   AfxTrace("Call stack info(thread=0x%X) failed.\n", hThread );
	   return;
	}
	//initialize the call stack
	memset( &callStack, NULL, sizeof(callStack) );
	callStack.AddrPC.Offset    = context.Eip;
	callStack.AddrStack.Offset = context.Esp;
	callStack.AddrFrame.Offset = context.Ebp;
	callStack.AddrPC.Mode      = AddrModeFlat;
	callStack.AddrStack.Mode   = AddrModeFlat;
	callStack.AddrFrame.Mode   = AddrModeFlat;
	//
	for( DWORD index = 0; index < MLD_MAX_TRACEINFO; index++ ) 
	{
		bResult = StackWalk(IMAGE_FILE_MACHINE_I386,
							m_hProcess,
							hThread,
							&callStack,
							NULL, 
							NULL,
							SymFunctionTableAccess,
							SymGetModuleBase,
							NULL);
		
		
		//if ( index == 0 )
		 //  continue;

		if( !bResult || callStack.AddrFrame.Offset == 0 ) 
			break;
		//
		pStacktrace[0].addrPC	 = callStack.AddrPC;
		pStacktrace[0].addrFrame = callStack.AddrFrame;
		pStacktrace++;
	}
	//clear the last entry
	memset(pStacktrace, NULL, sizeof(STACKFRAMEENTRY));
}*/

//
// This code is still under investigation
// I have to test this code and make sure it is compatible
// with the other stack walker!
//
void CMemLeakDetect::symStackTrace2(STACKFRAMEENTRY* pStacktrace )
{
	long			StackIndex				= 0;

	ADDR			block[63];
	memset(block,0,sizeof(block));

	USHORT frames = (m_func)(3,59,(void**)block,NULL);

	for (int i = 0; i < frames ; i++)
	{
		ADDR			InstructionPtr = (ADDR)block[i];

		pStacktrace[StackIndex].addrPC.Offset	= InstructionPtr;
		pStacktrace[StackIndex].addrPC.Segment	= NULL;
		pStacktrace[StackIndex].addrPC.Mode		= AddrModeFlat;
		//
		StackIndex++;
	}
	pStacktrace[StackIndex].addrPC.Offset = 0;
	pStacktrace[StackIndex].addrPC.Segment = 0;
}

BOOL CMemLeakDetect::symFunctionInfoFromAddresses( ADDR fnAddress, ADDR stackAddress, TCHAR *lpszSymbol,
													UINT BufSizeTCHARs)

{
	ADDR             dwDisp	= 0;

	::ZeroMemory(m_pSymbol, m_dwsymBufSize );
	m_pSymbol->SizeOfStruct		= sizeof(IMAGEHLP_LINE64);
	//m_pSymbol->MaxNameLength	= DWORD64 - sizeof(IMAGEHLP_SYMBOL64);

    // Set the default to unknown
	_tcscpy_s( lpszSymbol, MLD_MAX_NAME_LENGTH, MLD_TRACEINFO_NOSYMBOL);

	// Get symbol info for IP
	if ( SymGetSymFromAddr( m_hProcess, (ADDR)fnAddress, &dwDisp, m_pSymbol ) )
	{
#ifdef UNICODE
		int len = (int)strlen(m_pSymbol->Name) + 1 ;
		wchar_t dest[1024] ;
		MultiByteToWideChar(CP_ACP, 0, m_pSymbol->Name, len, dest, len );
		_tcscpy_s(lpszSymbol, BufSizeTCHARs, dest);
#else
		_tcscpy_s(lpszSymbol, BufSizeTCHARs, m_pSymbol->Name);
#endif
		return TRUE;
	}
	//create the symbol using the address because we have no symbol
	_stprintf_s(lpszSymbol, BufSizeTCHARs, _T("0x%08X"), fnAddress);
	return FALSE;
}

BOOL CMemLeakDetect::symSourceInfoFromAddress(ADDR address, TCHAR* lpszSourceInfo)
{
	BOOL           ret = FALSE;
	IMAGE_LN  lineInfo;
	DWORD          dwDisp;
	TCHAR          lpModuleInfo[MLD_MAX_NAME_LENGTH] = MLD_TRACEINFO_EMPTY;

	_tcscpy_s( lpszSourceInfo, MLD_MAX_NAME_LENGTH, MLD_TRACEINFO_NOSYMBOL);

	memset( &lineInfo, NULL, sizeof( IMAGEHLP_LINE ) );
	lineInfo.SizeOfStruct = sizeof( IMAGEHLP_LINE );

	if ( SymGetLineFromAddr( m_hProcess, address, &dwDisp, &lineInfo ) )
	{
	   // Using the "sourcefile(linenumber)" format
#ifdef UNICODE
		wchar_t dest[1024] ;
		int len = (int)strlen((char *)lineInfo.FileName) + 1 ;
		MultiByteToWideChar(CP_ACP, 0, (char *)lineInfo.FileName, len, dest, len) ;
		_stprintf_s(lpszSourceInfo, MLD_MAX_NAME_LENGTH, _T("%s(%d): 0x%08X"), dest, lineInfo.LineNumber, address );//	<--- Size of the char thing.
#else
		_stprintf_s(lpszSourceInfo, MLD_MAX_NAME_LENGTH, _T("%s(%d): 0x%08X"), lineInfo.FileName, lineInfo.LineNumber, address );//	<--- Size of the char thing.
#endif
		ret = TRUE;
	}
	else
	{
        // Using the "modulename!address" format
	  	symModuleNameFromAddress( address, lpModuleInfo );

		if ( lpModuleInfo[0] == _T('?') || lpModuleInfo[0] == _T('\0'))
		{
			// Using the "address" format

			_stprintf_s(lpszSourceInfo,MLD_MAX_NAME_LENGTH,  _T("0x%p"), lpModuleInfo, address );	// Tim ???
		}
		else
		{
			_stprintf_s(lpszSourceInfo, MLD_MAX_NAME_LENGTH, _T("%sdll! 0x%08X"), lpModuleInfo, address );
		}
		ret = FALSE;
	}
	//
	return ret;
}

BOOL CMemLeakDetect::symModuleNameFromAddress( ADDR address, TCHAR* lpszModule )
{
	BOOL              ret = FALSE;
	IMAGEHLP_MODULE   moduleInfo;

	::ZeroMemory( &moduleInfo, sizeof(IMAGEHLP_MODULE) );
	moduleInfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE);

	if ( SymGetModuleInfo( m_hProcess, (ADDR)address, &moduleInfo ) )
	{
		// Note. IMAGEHLP_MODULE::ModuleName seems to be hardcoded as 32 char/wchar_t (VS2008).
#ifdef UNICODE
		int len = (int)_tcslen(lpszModule) + 1 ;
		char dest[1024] ;
		WideCharToMultiByte( CP_ACP, 0, lpszModule, -1, dest, len, NULL, NULL );

		strcpy_s(moduleInfo.ModuleName, 32, dest);	// bloody ANSI!
#else
		strcpy_s(moduleInfo.ModuleName, 32, lpszModule);
#endif
		ret = TRUE;
	}
	else
	{
		_tcscpy_s( lpszModule, MLD_MAX_NAME_LENGTH, MLD_TRACEINFO_NOSYMBOL);
	}
	
	return ret;
}

static void DeleteOldTempFiles(const TCHAR dir[], const TCHAR type[], int days)
{
	union tu
	{
		FILETIME fileTime;
		ULARGE_INTEGER ul;
	};	// Seems simplest way to do the Win32 time manipulation.
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	TCHAR curdir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, curdir);	// Ignoring failure!
	SetCurrentDirectory(dir);

	hFind = FindFirstFile(type, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		SYSTEMTIME st;
		tu ft;

		GetSystemTime(&st);
		SystemTimeToFileTime(&st, &ft.fileTime);

		while (FindNextFile(hFind, &FindFileData) != 0)
		{
			if (FILE_ATTRIBUTE_DIRECTORY != FindFileData.dwFileAttributes)
			{
				tu t;
				t.fileTime = FindFileData.ftCreationTime;

				_int64 delta = (ft.ul.QuadPart - t.ul.QuadPart) / 10000000;	// Seconds.
				int ddays = (int)(delta /= (24 * 3600));
				//_tprintf (TEXT("Next file name is: %s delta days %d\n"), FindFileData.cFileName, ddays);
				if (ddays >= days)
				{
					//_tprintf (TEXT("Next file to delete is: %s delta days %d\n"), FindFileData.cFileName, ddays);
					DeleteFile(FindFileData.cFileName);
				}
				//else
				//{
				//	_tprintf (TEXT("Skipping: %s delta days %d\n"), FindFileData.cFileName, ddays);
				//}
			}
		}
		FindClose(hFind);

	}
	SetCurrentDirectory(curdir);
}
#endif
