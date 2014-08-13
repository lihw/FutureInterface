// main.cpp 
// Main entry and main loop in windows.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include <PPlatform/pwin32window.h>

#include <PFoundation/pactivity.h>
#include <PFoundation/pcontext.h>
#include <PFoundation/pmemorydebugger.h>


#include <windows.h>
#include <stdio.h>
#include <crtdbg.h>
#include <dbghelp.h>


P_EXTERN void pMain(int argc, char* argv[]);


int pwin32main(int argc, char* argv[])
{
    // Enable memory leak checks and heap validation. 
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetBreakAlloc(-1);

    int exitCode = EXIT_SUCCESS;

    // Create the memory debugger.
    PMemoryDebugger::create();

    PActivity* activity = PNEW(PActivity(argc, argv));
    if (!activity->initialize())
    {
        PDELETE(activity);
        return EXIT_FAILURE;
    }
    
    pMain(argc, argv);

    // Set console title.
    SetConsoleTitle(L"Console");
    
    // Disable the close button of the console window.
    HWND hConsoleWindow = GetConsoleWindow();
    if (hConsoleWindow != NULL)
    {
        HMENU hMenu = GetSystemMenu(hConsoleWindow, 0);
        if (hMenu != NULL)
        {
            DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
            DrawMenuBar(hConsoleWindow);
        }
    }

    PContext* context = activity->findContext(puint32(0));
    PASSERT(context != P_NULL);
    if (context == P_NULL)
    {
        exitCode = EXIT_FAILURE;
        PDELETE(activity);
        return exitCode;
    }

    PWin32Window window(context);

    if (!window.create())
    {
        exitCode = EXIT_FAILURE;
        PDELETE(activity);
        return exitCode;
    }
    
    // Initialize the context. 
    context->setState(P_CONTEXT_STATE_UNINITIALIZED);
    if (!context->initialize(context->properties()->m_windowWidth, context->properties()->m_windowHeight))
    {
        exitCode = EXIT_FAILURE;
    }
    else
    {
        if (!context->onInitialized())
        {
            context->setState(P_CONTEXT_STATE_ERROR);
        }
        else
        {
            PLOG_DEBUG("Starting program main loop");
            context->setState(P_CONTEXT_STATE_RUNNING);
        }

        if (context->state() == P_CONTEXT_STATE_ERROR)
        {
            exitCode = EXIT_FAILURE;
        }

        // The mainloop of window.
        window.run();

        // Right before destroy the context, user might have
        // something to do.
        context->onDestroy();
    }

    context->destroy();
    
    // Destroy native window.
    window.destroy();

    // Destroy the activity
    activity->uninitialize();
    PDELETE(activity);

    // Destroy the memory debugger.
    PMemoryDebugger::destroy();

    // If debugger is present, a pause is required to keep the console output
    // visible. Otherwise the pause is automatic. 
    if (IsDebuggerPresent())
    {
        system("pause");
    }

    return exitCode;
}
