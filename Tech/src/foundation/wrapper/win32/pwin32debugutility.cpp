// pwin32debugutility.cpp 
// Debug helper functions under windows
//
// Copyright 2012 - 2014 Future Interface Li. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "../pdebugutility.h"

#include <PFoundation/plog.h>
#include <PFoundation/pcrt.h>

#include <windows.h>
#include <dbghelp.h>


void P_APIENTRY pDebugOutputString(const pchar* message)
{
    OutputDebugStringA(message);
    OutputDebugStringA("\n");
}

void P_APIENTRY pDebugBreak()
{
    __debugbreak();
}

void P_APIENTRY pDebugPrintStack(pint32 skip)
{
    typedef USHORT (WINAPI *CaptureStackBackTraceType)(__in ULONG, __in ULONG, __out PVOID*, __out_opt PULONG);
    CaptureStackBackTraceType func = (CaptureStackBackTraceType)(GetProcAddress(LoadLibrary(TEXT("kernel32.dll")), 
                "RtlCaptureStackBackTrace"));

    if (func == NULL)
    {
        return ; 
    }

    // Quote from Microsoft Documentation:
    // ## Windows Server 2003 and Windows XP:  
    // ## The sum of the FramesToSkip and FramesToCapture parameters must be less than 63.
    const int kMaxCallers = 62; 

    HANDLE hProcess = GetCurrentProcess();
    SymInitialize(hProcess, NULL, TRUE);

    PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)malloc(sizeof(SYMBOL_INFO) + 256);
    pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    pSymbol->MaxNameLen = 255;
    
    void* callers[kMaxCallers];
    int count = (func)(0, kMaxCallers, callers, NULL);

    // To skip the pDebugPrintStack function frame, we start from 1 instead of 0.
    for(int i = skip; i < count; i++)
    {
        //PLOG_INFO("*** %d called from %016I64LX\n", i, callers[i]);

        DWORD addr = (DWORD)(callers[i]);
        DWORD64 symDisplacement = 0;
        if (SymFromAddr(hProcess, addr, 0, pSymbol))
        {
            IMAGEHLP_LINE lineInfo = {sizeof(IMAGEHLP_LINE)};
            DWORD dwLineDisplacement;

            if (SymGetLineFromAddr(hProcess, addr, &dwLineDisplacement, &lineInfo))
            {
                PLOG_INFO("%s:%s(),Line %u", lineInfo.FileName, pSymbol->Name, lineInfo.LineNumber); 
            }
            
            // Stop the walk at main function. 
            if (pstrcmp(pSymbol->Name, "main") == 0)
            {
                break;
            }
        }
    }
    
    SymCleanup(GetCurrentProcess());
}


