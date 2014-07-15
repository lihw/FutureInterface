// pwin32window.cpp
// The native window for win32.
//
// Copyright 2012 - 2014 Future Interface Li. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#include "pwin32window.h"

#include <PPlatform/pwin32surface.h>

#include <PFoundation/pinput.h>
#include <PFoundation/PContext.h>
#include <PFoundation/pdevice.h>

#include <PFoundation/passert.h>
#include <PFoundation/plog.h>
#include <PFoundation/pnew.h>
#include <PFoundation/ptime.h>
#include <PFoundation/pglframebuffer.h>


#include <windows.h>
#include <WindowsX.h>
#include <stdio.h>


P_EXTERN PInputKey P_APIENTRY pInputNativeGetTranslatedKey(puint32 keyCode);

// Win32 message dispatcher.
static LRESULT CALLBACK pWin32WindowMainWndProc_internal(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

PWin32Window::PWin32Window(PContext* context)
{
    m_hDC     = P_NULL;
    m_hWnd    = P_NULL;
    m_surface = P_NULL;
    m_context = context;
}

PWin32Window::~PWin32Window()
{
    PASSERT(m_hDC == P_NULL);
    PASSERT(m_hWnd == P_NULL);
    PASSERT(m_surface == P_NULL);
}

pbool PWin32Window::create()
{
    // Create the window.
    puint32 width = m_context->properties()->m_windowWidth;
    puint32 height = m_context->properties()->m_windowHeight;
    if (!createWindow(width, height))
    {
        return false;
    }

    // Create the GL surface.
    m_surface = PNEW(PWin32Surface(m_hWnd, m_hDC));
    if (!m_surface->create(m_context->properties()))
    {
        PDELETE(m_surface);
        destroyWindow();
        return false;
    }

    // Store window pointer to window handler. 
    SetWindowLongPtr(m_hWnd, GWL_USERDATA, (LONG)this);

    // Show window and put it on foreground. 
    ShowWindow(m_hWnd, SW_SHOWNORMAL);
    UpdateWindow(m_hWnd);
    SetForegroundWindow(m_hWnd);

    return true;
}

void PWin32Window::destroy()
{
    // Destroy surface
    if (m_surface != P_NULL)
    {
        m_surface->destroy();
        PDELETE(m_surface);
    }

    // Destroy window.
    destroyWindow();
}

pbool PWin32Window::createWindow(puint32 windowWidth, puint32 windowHeight)
{
    // Get the window and height of current display.
    puint32 displayWidth = GetSystemMetrics(SM_CXSCREEN); 
    puint32 displayHeight = GetSystemMetrics(SM_CYSCREEN);
    PLOG_DEBUG("Display resolution: %d x %d", displayWidth, displayHeight);

    // If resolution set to automatic, get maximum window size available.
    if (windowWidth == 0)
    {
        windowWidth = displayWidth;
    }

    if (windowHeight == 0)
    {
        windowHeight = displayHeight;
    }
    
    // Register win32 window class. 
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = pWin32WindowMainWndProc_internal;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = NULL;
    wcex.hIcon          = NULL;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = TEXT("PWin32Window");
    wcex.hIconSm        = NULL;

    RegisterClassEx(&wcex);
    
    // Create a win32 window and set it up. 
    RECT rect;

    puint32 windowWidthActual;
    puint32 windowHeightActual;
    pint32 windowPositionX;
    pint32 windowPositionY;

    DWORD windowStyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_THICKFRAME;
    DWORD extendedWindowStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

    // Set desired window position to the center of the screen. 
    puint32 desiredWindowWidth = windowWidth;
    puint32 desiredWindowHeight = windowHeight;
    int desiredWindowPositionX = (GetSystemMetrics(SM_CXSCREEN) - (int)desiredWindowWidth) / 2;
    int desiredWindowPositionY = (GetSystemMetrics(SM_CYSCREEN) - (int)desiredWindowHeight) / 2;
    SetRect(&rect, desiredWindowPositionX, desiredWindowPositionY, 
        desiredWindowPositionX + (int)desiredWindowWidth, desiredWindowPositionY + (int)desiredWindowHeight);

    // Set window style and position. 
    AdjustWindowRectEx(&rect, windowStyle, FALSE, extendedWindowStyle);

    // Calculate width and height. 
    windowPositionX = rect.left;
    windowPositionY = rect.top;
    windowWidthActual = rect.right - rect.left;
    windowHeightActual = rect.bottom - rect.top;

    // Create the window with given parameters.
    const pchar* title = m_context->name().c_str();
    wchar_t windowTitle[1024];
#if defined P_DEBUG
    size_t convertedChars;
    mbstowcs_s(&convertedChars, windowTitle, 1024, title, strlen(title) + 1);
#else
    size_t windowTitleChars;
    mbstowcs_s(&windowTitleChars, windowTitle, 1024, title, strlen(title) + 1);
#endif

    m_hWnd = CreateWindowEx(extendedWindowStyle, TEXT("PWin32Window"), windowTitle, windowStyle,
                                        windowPositionX, windowPositionY, windowWidthActual, windowHeightActual,
                                        NULL, NULL, NULL, NULL);
    if (m_hWnd == NULL)
    {
        PLOG_ERROR("fail to create a window with error code 0x%x", GetLastError());
        return false;
    }

    m_style = windowStyle;

    RECT clientRect;
    GetClientRect(m_hWnd, &clientRect);

    windowWidth = clientRect.right - clientRect.left;
    windowHeight = clientRect.bottom - clientRect.top;
    
    SetWindowTextA(m_hWnd, title);

    // Store the display device context handle for the window. 
    m_hDC = GetDC(m_hWnd);
    if (m_hDC == P_NULL)
    {
        PLOG_ERROR("can't get device context of the current window!");
        destroyWindow();
        return false;
    }
    
    // On Windows, the context's window size is equal to screen resolution.
    m_context->properties()->m_windowWidth = windowWidth;
    m_context->properties()->m_windowHeight = windowHeight;
    PLOG_DEBUG("Viewport resolution: %d x %d", windowWidth, windowHeight);

    return true;
}

void PWin32Window::destroyWindow()
{
    if (m_hDC != P_NULL)
    {
        ReleaseDC(m_hWnd, m_hDC);
        m_hDC = P_NULL;
    }

    DestroyWindow(m_hWnd);
    
    m_hWnd = P_NULL;
}
    
void PWin32Window::run()
{
    while((m_context->state() == P_CONTEXT_STATE_RUNNING) ||
          (m_context->state() == P_CONTEXT_STATE_PAUSED))
    {
        MSG message;
        while (PeekMessage(&message, m_hWnd, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    
        if (m_context->state() == P_CONTEXT_STATE_RUNNING)
        {
            // Update context state. 
            if (!m_context->update())
            {
                m_context->setState(P_CONTEXT_STATE_ERROR);
            }
            else
            {
                m_surface->update();
            }

            // Update the window title bar
            const pchar* contextName = m_context->name().c_str();
            pchar buffer[1024];
            psprintf(buffer, 1024, "%s  %5.3f fps", contextName, 
                m_context->clock().fps());
            SetWindowTextA(m_hWnd, buffer);
        }
    }
}

LRESULT CALLBACK pWin32WindowMainWndProc_internal(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT lresult;
    pbool handled = false;

    // Retrieve the PWin32Window object from the user data of the win32 window handle.

    switch (message)
    {
        case WM_CLOSE:
        case WM_QUERYENDSESSION:
        {
            PWin32Window* window = reinterpret_cast<PWin32Window*>(GetWindowLongPtr(hWnd, GWL_USERDATA));
            lresult = window->closeEvent()? 0 : 1;
            handled = true;
            break;
        }
        case WM_DESTROY:
        {
            // window has been destroyed
            ::PostQuitMessage(0);
            break;
        }
        case WM_KILLFOCUS:
        {
            PWin32Window* window = reinterpret_cast<PWin32Window*>(GetWindowLongPtr(hWnd, GWL_USERDATA));
            window->focusLostEvent();
            break;
        }
        case WM_SETFOCUS:
        {
            PWin32Window* window = reinterpret_cast<PWin32Window*>(GetWindowLongPtr(hWnd, GWL_USERDATA));
            window->focusGainedEvent();
            break;
        }
        case WM_SIZE:
        {
            PWin32Window* window = reinterpret_cast<PWin32Window*>(GetWindowLongPtr(hWnd, GWL_USERDATA));
                    
            RECT windowSize;
            if (GetClientRect(hWnd, &windowSize))
            {
                window->resizeEvent(windowSize.right, windowSize.bottom);
            }

            break;
        }
        case WM_LBUTTONDOWN:
        {
            PWin32Window* window = reinterpret_cast<PWin32Window*>(GetWindowLongPtr(hWnd, GWL_USERDATA));

            window->mousePressEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam),  (wParam & MK_CONTROL) > 0);
            SetCapture(window->getHWND());   // Set capture on, to get drag movement outside the window. 
            break;
        }
        case WM_LBUTTONUP: 
        {
            PWin32Window* window = reinterpret_cast<PWin32Window*>(GetWindowLongPtr(hWnd, GWL_USERDATA));

            window->mouseReleaseEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam),  (wParam & MK_CONTROL) > 0);
            ReleaseCapture();   // Release mouse capture after click/drag has ended. 
            break;
        }
        case WM_MOUSEMOVE:
        {
            PWin32Window* window = reinterpret_cast<PWin32Window*>(GetWindowLongPtr(hWnd, GWL_USERDATA));

            // If lbutton of mouse is pressed down.
            if ((wParam & 0x0013) > 0 && (wParam & MK_LBUTTON) != 0)
            {
                window->mouseMoveEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (wParam & MK_CONTROL) > 0);
            }

            // for tracking the mouse leaving a window 
            TRACKMOUSEEVENT tme;
            tme.cbSize = sizeof(TRACKMOUSEEVENT);
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = window->getHWND();
            tme.dwHoverTime = 0;
            TrackMouseEvent(&tme);
            break;
        }
        case WM_MOUSELEAVE:
        {
            // The mouse has left the window.. don't care because the mouse is still tracked by capturing it earlier. 
            break;
        }
        // Keyboard input. 
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
        {
            puint32 keyCode = (puint32)wParam;
            
            puint32 state = P_KEY_DEVICE_STATE_DOWN;
            if (GetKeyState(VK_SHIFT) & 0x8000)
            {
                state |= P_KEY_DEVICE_STATE_SHIFT;
            }
            if (GetKeyState(VK_MENU) & 0x8000)
            {
                state |= P_KEY_DEVICE_STATE_ALT;
            }
            if (GetKeyState(VK_CONTROL) & 0x8000)
            {
                state |= P_KEY_DEVICE_STATE_CTRL;
            }

            PInputKey key = pInputNativeGetTranslatedKey(keyCode);
            PWin32Window* window = reinterpret_cast<PWin32Window*>(GetWindowLongPtr(hWnd, GWL_USERDATA));
            window->keyPressEvent(keyCode, key, state);
            break;
        }
        case WM_SYSKEYUP:
        case WM_KEYUP:
        {
            PWin32Window* window = reinterpret_cast<PWin32Window*>(GetWindowLongPtr(hWnd, GWL_USERDATA));

            // Pressing down F5 under Win32 will trigger discard the surface
            if (wParam == VK_F3)
            {
                window->pause();
            }
            else if (wParam == VK_F4)
            {
                window->resume();
            }
            else
            {
                puint32 keyCode = (puint32)wParam;
                
                puint32 state = P_KEY_DEVICE_STATE_UP;
                if (GetKeyState(VK_SHIFT) & 0x0001)
                {
                    state |= P_KEY_DEVICE_STATE_SHIFT;
                }
                if (GetKeyState(VK_MENU) & 0x0001)
                {
                    state |= P_KEY_DEVICE_STATE_ALT;
                }
                if (GetKeyState(VK_CONTROL) & 0x0001)
                {
                    state |= P_KEY_DEVICE_STATE_CTRL;
                }

                PInputKey key = pInputNativeGetTranslatedKey(keyCode);
                PWin32Window* window = reinterpret_cast<PWin32Window*>(GetWindowLongPtr(hWnd, GWL_USERDATA));
                window->keyReleaseEvent(keyCode, key, state);
            }
            
            break;
        }
        default:
            handled = false;
            break;
    }
    
    if (!handled)
    {
        lresult = DefWindowProc(hWnd, message, wParam, lParam);
    }

    return lresult;
}

void PWin32Window::mousePressEvent(pint32 x, pint32 y, pbool isCtrlHeld)
{
    if (m_context->state() == P_CONTEXT_STATE_RUNNING ||
        m_context->state() == P_CONTEXT_STATE_PAUSED)
    {
        PInputEventQueue* inputQueue = m_context->device()->getInputEventQueue();

        puint32 timestamp = pTimeGetCurrentTimestamp();

        inputQueue->addPointerEvent(x,
                                    y,
                                    P_POINTING_DEVICE_STATE_DOWN, 
                                    isCtrlHeld,
                                    timestamp);
    }
}

void PWin32Window::mouseMoveEvent(pint32 x, pint32 y, pbool isCtrlHeld)
{
    if (m_context->state() == P_CONTEXT_STATE_RUNNING ||
        m_context->state() == P_CONTEXT_STATE_PAUSED)
    {
        PInputEventQueue* inputQueue = m_context->device()->getInputEventQueue();

        puint32 timestamp = pTimeGetCurrentTimestamp();

        inputQueue->addPointerEvent(x,
                                    y,
                                    P_POINTING_DEVICE_STATE_DRAG, 
                                    isCtrlHeld,
                                    timestamp);
    }
}

void PWin32Window::mouseReleaseEvent(pint32 x, pint32 y, pbool isCtrlHeld)
{
    if (m_context->state() == P_CONTEXT_STATE_RUNNING ||
        m_context->state() == P_CONTEXT_STATE_PAUSED)
    {
        PInputEventQueue* inputQueue = m_context->device()->getInputEventQueue();

        puint32 timestamp = pTimeGetCurrentTimestamp();

        inputQueue->addPointerEvent(x,
                                    y,
                                    P_POINTING_DEVICE_STATE_UP, 
                                    isCtrlHeld,
                                    timestamp);
    }
}

void PWin32Window::keyPressEvent(puint32 nativeScanCode, PInputKey key, puint32 state)
{
    if (m_context->state() == P_CONTEXT_STATE_RUNNING ||
        m_context->state() == P_CONTEXT_STATE_PAUSED)
    {
        puint32 timestamp = pTimeGetCurrentTimestamp();
        PInputEventQueue* inputQueue = m_context->device()->getInputEventQueue();
        inputQueue->addKeyEvent(nativeScanCode, key, state, timestamp);
    }
}

void PWin32Window::keyReleaseEvent(puint32 nativeScanCode, PInputKey key, puint32 state)
{
    if (m_context->state() == P_CONTEXT_STATE_RUNNING ||
        m_context->state() == P_CONTEXT_STATE_PAUSED)
    {
        puint32 timestamp = pTimeGetCurrentTimestamp();
        PInputEventQueue* inputQueue = m_context->device()->getInputEventQueue();
        inputQueue->addKeyEvent(nativeScanCode, key, state, timestamp);
    }
}

void PWin32Window::resizeEvent(pint32 width, pint32 height)
{
    PDevice* device = m_context->device();
    if (device != P_NULL && 
            (m_context->state() == P_CONTEXT_STATE_RUNNING ||
             m_context->state() == P_CONTEXT_STATE_PAUSED))
    {
       device->onResized(width, height);
    }
}

pbool PWin32Window::closeEvent()
{
    if (m_context->state() == P_CONTEXT_STATE_RUNNING ||
        m_context->state() == P_CONTEXT_STATE_PAUSED)
    {
        if (m_context->onClose())
        {
            m_context->device()->setState(P_DEVICE_STATE_CLOSED);
            return true;
        }
    }

    return false;
}

void PWin32Window::focusLostEvent()
{
    PDevice* device = m_context->device();
    if (device != P_NULL)
    {
        // Release all held keys.
        // For the integer 225, please see P_INPUT_NATIVE_KEY_TRANSLATION_TABLE_SIZE
        // in p_input_win32.cpp
        for (puint32 i = 0; i < P_KEY_COUNT; ++i)
        {
            PInputKey key = pInputNativeGetTranslatedKey(i);
            if (key != P_KEY_UNKNOWN && device->getKeyState(key) == P_KEY_DEVICE_STATE_DOWN)
            {
                device->setKeyState((PInputKey)i, P_KEY_DEVICE_STATE_UP);
                puint32 timestamp = pTimeGetCurrentTimestamp();
                device->getInputEventQueue()->addKeyEvent(i, key, P_KEY_DEVICE_STATE_UP, timestamp);
            }
        }

        device->setState(P_DEVICE_STATE_INACTIVE);
        device->onFocusLost();
    }
}

void PWin32Window::focusGainedEvent()
{
    PDevice* device = m_context->device();
    if (device != P_NULL)
    {
        // FIXME: device == P_NULL when the context has not been initialized.
        device->setState(P_DEVICE_STATE_ACTIVE);
        device->onFocusGained();
    }
}

void PWin32Window::resume()
{
    PASSERT(m_context->state() == P_CONTEXT_STATE_PAUSED);
    PASSERT(m_surface == P_NULL);

    // Regain the surface.
    m_surface = PNEW(PWin32Surface(m_hWnd, m_hDC));
    if (!m_surface->create(m_context->properties()))
    {
        PDELETE(m_surface);
        return;
    }
    
    m_context->resume();
    m_context->onResume();
}

void PWin32Window::pause()
{
    PASSERT(m_context->state() == P_CONTEXT_STATE_RUNNING);
    m_context->onPause();
    m_context->pause();
    
    // Clear the current buffer
    PGlFramebuffer::clearFramebuffer(true, true, false);
    m_surface->update();

    PASSERT(m_surface != P_NULL);
        
    // Lost the surface
    m_surface->destroy();
    PDELETE(m_surface);
}

