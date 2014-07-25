// pwin32surface.cpp
// The OpenGL surface using EGL
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#include "pwin32surface.h"

#include <PFoundation/pcontextproperties.h>

#include <PFoundation/passert.h>
#include <PFoundation/plog.h>
#include <PFoundation/pnew.h>

#include <EGL/egl.h>

// Initialize the EGL display
// \param display specifies the display to connect to. EGL_DEFAULT_DISPLAY
// indicates the default display.
static EGLDisplay pEglDisplayInitialize_internal(EGLNativeDisplayType displayType);
// Initialize the EGL surface.
// \param eglDisplay the EGL display
// \param eglWindow the native window type.
// \param eglConfiguration the configuration for this surface.
// \param out_eglSurface the returned EGL surface.
// \param out_eglContext the returned EGL context.
// \return true if successful and false otherwise.
static pbool pEglSurfaceInitialize_internal(EGLDisplay eglDisplay, HWND eglWindow, 
        EGLConfig eglConfiguration, EGLSurface& out_eglSurface, EGLContext& out_eglContext);

//  Make the the EGL surface configuration with given one.
//  \param eglDisplay the EGL display.
//  \param eglConfigurationAttributeArray the input EGL surface configuration.
//  \param out_eglConfiguration the returned surface configuration.
static EGLConfig pEglConfigure_internal(EGLDisplay eglDisplay, EGLint* eglConfigurationAttributeArray);

// Check the EGL error
// \param file the file name where this function is used.
// \param line the line number where this function is at 
static pbool pEglErrorCheck_internal(const pchar* file, puint32 line);
#define pEglErrorCheckError() pEglErrorCheck_internal(__FILE__, __LINE__) 


PWin32Surface::PWin32Surface(HWND hWnd, HDC hDC)
{
    PASSERT(hWnd != NULL);
    m_hWnd = hWnd;
    PASSERT(hDC != NULL);
    m_hDC = hDC;

    m_eglDisplay       = P_NULL;
    m_eglSurface       = P_NULL;
    m_eglContext       = P_NULL;
    m_eglConfiguration = P_NULL;
}

PWin32Surface::~PWin32Surface()
{
    PASSERT(m_eglDisplay == P_NULL);
    PASSERT(m_eglSurface == P_NULL);
    PASSERT(m_eglContext == P_NULL);
}

pbool PWin32Surface::create(PContextProperties *properties)
{
    // Create private data object.
    m_eglDisplay = pEglDisplayInitialize_internal((EGLNativeDisplayType)m_hDC);
    if (m_eglDisplay == EGL_NO_DISPLAY)
    {
        return false;
    }

    // Set EGL attributes.
    EGLint eglConfigurationAttributeArray[128];
    pint32 i = 0;
    eglConfigurationAttributeArray[i++] = EGL_RED_SIZE;
    eglConfigurationAttributeArray[i++] = properties->m_rgba[0];
    eglConfigurationAttributeArray[i++] = EGL_GREEN_SIZE;
    eglConfigurationAttributeArray[i++] = properties->m_rgba[1];
    eglConfigurationAttributeArray[i++] = EGL_BLUE_SIZE;
    eglConfigurationAttributeArray[i++] = properties->m_rgba[2];
    eglConfigurationAttributeArray[i++] = EGL_ALPHA_SIZE;
    eglConfigurationAttributeArray[i++] = properties->m_rgba[3];
    eglConfigurationAttributeArray[i++] = EGL_SURFACE_TYPE;
    eglConfigurationAttributeArray[i++] = EGL_WINDOW_BIT;
    eglConfigurationAttributeArray[i++] = EGL_DEPTH_SIZE;
    eglConfigurationAttributeArray[i++] = properties->m_depth;
    eglConfigurationAttributeArray[i++] = EGL_STENCIL_SIZE;
    eglConfigurationAttributeArray[i++] = properties->m_stencil;
    
    if (properties->m_multisamples > 0)
    {
        eglConfigurationAttributeArray[i++] = EGL_SAMPLES;
        eglConfigurationAttributeArray[i++] = properties->m_multisamples;
    }

#if defined PT_OPENGL_ES20 
    eglConfigurationAttributeArray[i++] = EGL_RENDERABLE_TYPE;
    eglConfigurationAttributeArray[i++] = EGL_OPENGL_ES2_BIT;
#endif
    eglConfigurationAttributeArray[i++] = EGL_NONE;

    m_eglConfiguration = pEglConfigure_internal(m_eglDisplay, eglConfigurationAttributeArray);
    if (m_eglConfiguration == P_NULL)
    {
        eglTerminate(m_eglDisplay);
        m_eglDisplay = P_NULL;
        return false;
    }

    // Find the actual properties 
    EGLBoolean result;
    EGLint redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, samples;

    result = eglGetConfigAttrib(m_eglDisplay, m_eglConfiguration,  EGL_RED_SIZE, &redBits);
    result &= eglGetConfigAttrib(m_eglDisplay, m_eglConfiguration,  EGL_GREEN_SIZE, &greenBits);
    result &= eglGetConfigAttrib(m_eglDisplay, m_eglConfiguration,  EGL_BLUE_SIZE, &blueBits);
    result &= eglGetConfigAttrib(m_eglDisplay, m_eglConfiguration,  EGL_ALPHA_SIZE, &alphaBits);
    result &= eglGetConfigAttrib(m_eglDisplay, m_eglConfiguration,  EGL_DEPTH_SIZE, &depthBits);
    result &= eglGetConfigAttrib(m_eglDisplay, m_eglConfiguration,  EGL_STENCIL_SIZE, &stencilBits);
    result &= eglGetConfigAttrib(m_eglDisplay, m_eglConfiguration,  EGL_SAMPLES, &samples);

    if (result == EGL_TRUE)
    {
        PLOG_DEBUG("EGL surface created, RGBA=(%d,%d,%d,%d), depth=%d, stencil=%d, multisamples=%d",
                redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, samples);

        // Save these actual surface parameters.
        properties->m_rgba[0] = redBits;
        properties->m_rgba[1] = greenBits;
        properties->m_rgba[2] = blueBits;
        properties->m_rgba[3] = alphaBits;
        properties->m_depth   = depthBits;
        properties->m_stencil = stencilBits;
        properties->m_multisamples = samples;
    }
    else
    {
        PLOG_ERROR("fail to get EGL surface parameters");
        eglTerminate(m_eglDisplay);
        m_eglDisplay = P_NULL;
        return false;
    }

    if (!pEglSurfaceInitialize_internal(m_eglDisplay, m_hWnd, 
            m_eglConfiguration, m_eglSurface, m_eglContext))
    {
        eglTerminate(m_eglDisplay);
        m_eglDisplay = P_NULL;
        return false;
    }

    // Set this surface active.
    if (!setActive(true))
    {
        destroy();
        return false;
    }
    
    return true;
}

void PWin32Surface::destroy()
{
    PASSERT(m_eglContext != P_NULL);
    PASSERT(m_eglDisplay != P_NULL);
    PASSERT(m_eglSurface != P_NULL);

    EGLBoolean result;

    result = eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (result != EGL_TRUE || !pEglErrorCheckError()) 
    {
        PLOG_WARNING("failed to remove current EGL surface and context");
    }

    result = eglDestroySurface(m_eglDisplay, m_eglSurface);
    if (result != EGL_TRUE || !pEglErrorCheckError()) 
    {
        PLOG_WARNING("failed to destroy EGL surface");
    }
    m_eglSurface = P_NULL;

    result = eglDestroyContext(m_eglDisplay, m_eglContext);
    if (result != EGL_TRUE || !pEglErrorCheckError()) 
    {
        PLOG_WARNING("failed to destroy EGL context");
    }
    m_eglContext = P_NULL;

    result = eglTerminate(m_eglDisplay);
    if (result != EGL_TRUE || !pEglErrorCheckError()) 
    {
        PLOG_WARNING("failed to terminate EGL display");
    }
    m_eglDisplay = P_NULL;
}

pbool PWin32Surface::setActive(pbool active)
{
    PASSERT(m_eglSurface != P_NULL);
    PASSERT(m_eglContext != P_NULL);
    PASSERT(m_eglDisplay != P_NULL);

    EGLBoolean result;

    if (active)
    {
        result = eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);
        if (result != EGL_TRUE || !pEglErrorCheckError()) 
        {
            PLOG_WARNING("failed to switch to new EGL context and surface");
            return false;
        }
    }
    else
    {
        result = eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (result != EGL_TRUE || !pEglErrorCheckError()) 
        {
            PLOG_WARNING("failed to switch to NULL EGL context and surface");
            return false;
        }
    }

    return true;
}

void PWin32Surface::update()
{
    PASSERT(m_eglSurface != P_NULL);
    PASSERT(m_eglContext != P_NULL);
    PASSERT(m_eglDisplay != P_NULL);

    EGLBoolean result;
    result = eglSwapBuffers(m_eglDisplay, m_eglSurface);
    if (result != EGL_TRUE) 
    {
        pEglErrorCheckError(); 
        PLOG_WARNING("failed to flush EGL buffer to screen");
    }
}

/*
void PWin32Surface::readPixels(void* pixels)
{
    PASSERT(m_eglSurface != P_NULL);
    PASSERT(m_eglContext != P_NULL);
    PASSERT(m_eglDisplay != P_NULL);

    EGLint width, height;
    EGLint redBits, greenBits, blueBits, alphaBits;
    EGLBoolean result;

    // Query surface and configuration attributes.
    result = eglQuerySurface(m_eglDisplay, m_eglSurface, EGL_WIDTH, &width);
    result &= eglQuerySurface(m_eglDisplay, m_eglSurface, EGL_HEIGHT, &height);
    result &= eglGetConfigAttrib(m_eglDisplay, m_eglConfiguration,  EGL_RED_SIZE, &redBits);
    result &= eglGetConfigAttrib(m_eglDisplay, m_eglConfiguration,  EGL_GREEN_SIZE, &greenBits);
    result &= eglGetConfigAttrib(m_eglDisplay, m_eglConfiguration,  EGL_BLUE_SIZE, &blueBits);
    result &= eglGetConfigAttrib(m_eglDisplay, m_eglConfiguration,  EGL_ALPHA_SIZE, &alphaBits);

    if (result != EGL_TRUE || !pEglErrorCheckError())
    {
        PLOG_WARNING("Unable to query the surface");
        return ;
    }

    // Dump the data from surface to bitmap.
    EGLint nbits = redBits + greenBits + blueBits + alphaBits;
    puint8* bits = new puint8 [nbits >> 3];
    PASSERT(bits);
    if (bits == P_NULL)
    {
        PLOG_WARNING("new puint8 failed");
        return;
    }
    HBITMAP hbitmap = CreateBitmap(width, height, 1, nbits, bits);
    if (hbitmap == P_NULL)
    {
        PLOG_WARNING("error in CreateBitmap");
        PDELETE(bits);
        return;
    }

    result = eglCopyBuffers(m_eglDisplay, m_eglSurface, hbitmap);
    if (result != EGL_TRUE) 
    {
        pEglErrorCheckError(); 

        DeleteObject(hbitmap);
        PDELETEARRAY(bits);
        
        PLOG_WARNING("failed to copy EGL surface content to memory");

        return ;
    }

    // Copy the pixels from hbitmap to pixels.
    BITMAP bmp;
    ::GetObject(hbitmap, sizeof(BITMAP), &bmp);
    PFoundation_memcpy(pixels, bmp.bmBits, width * height * (nbits >> 3));    

    PDELETEARRAY(bits);
    DeleteObject(hbitmap);
}
*/

EGLDisplay pEglDisplayInitialize_internal(EGLNativeDisplayType displayType)
{
    EGLDisplay eglDisplay;

    eglDisplay = eglGetDisplay(displayType);
    if (!pEglErrorCheckError() || eglDisplay == EGL_NO_DISPLAY)
    {
        PLOG_ERROR("Unable to init EGL display");
        return P_NULL;
    }

    // Got a valid display, now init EGL. 
    EGLint eglVersionMajor, eglVersionMinor;
    EGLBoolean initializeResult = eglInitialize(eglDisplay, &eglVersionMajor, &eglVersionMinor);
    if (!pEglErrorCheckError() || initializeResult != EGL_TRUE)
    {
        PLOG_ERROR("EGL initialization failed.");
        return P_NULL;
    }
    else
    {
        PLOG_DEBUG("EGL version: %d.%d", eglVersionMajor, eglVersionMinor);
    }

    return eglDisplay;
}

pbool pEglErrorCheck_internal(const pchar* file, puint32 line)
{
    EGLint error;
    do 
    {
        error = eglGetError();
        if (error != EGL_SUCCESS)
        {
            pchar* errorString;
            switch (error)
            {
                case EGL_NOT_INITIALIZED: errorString = "EGL_NOT_INITIALIZED"; break;
                case EGL_BAD_ACCESS: errorString = "EGL_BAD_ACCESS"; break;
                case EGL_BAD_ALLOC: errorString = "EGL_BAD_ALLOC"; break;
                case EGL_BAD_ATTRIBUTE: errorString = "EGL_BAD_ATTRIBUTE"; break;
                case EGL_BAD_CONFIG: errorString = "EGL_BAD_CONFIG"; break;
                case EGL_BAD_CONTEXT: errorString = "EGL_BAD_CONTEXT"; break;
                case EGL_BAD_CURRENT_SURFACE: errorString = "EGL_BAD_CURRENT_SURFACE"; break;
                case EGL_BAD_DISPLAY: errorString = "EGL_BAD_DISPLAY"; break;
                case EGL_BAD_MATCH: errorString = "EGL_BAD_MATCH"; break;
                case EGL_BAD_NATIVE_PIXMAP: errorString = "EGL_BAD_NATIVE_PIXMAP"; break;
                case EGL_BAD_NATIVE_WINDOW: errorString = "EGL_BAD_NATIVE_WINDOW"; break;
                case EGL_BAD_PARAMETER: errorString = "EGL_BAD_PARAMETER"; break;
                case EGL_BAD_SURFACE: errorString = "EGL_BAD_SURFACE"; break;
                default: errorString = "Unknown EGL error"; break;
            }

            pLog(P_LOG_ERROR, P_LOG_CHANNEL_OPENGLEGL, file, line, "%s", errorString); 
            return false;
        }
    } 
    while (error != EGL_SUCCESS);

    return true;
}

EGLConfig pEglConfigure_internal(EGLDisplay eglDisplay, EGLint* eglConfigurationAttributeArray)
{
    EGLBoolean result = EGL_TRUE;

    EGLConfig eglConfig = P_NULL;

    pint32 eglConfigurationTotalCount;
    result = eglGetConfigs(eglDisplay, NULL, 0, &eglConfigurationTotalCount);
    if (!pEglErrorCheckError() || result != EGL_TRUE || eglConfigurationTotalCount < 1)
    {
        PLOG_ERROR("Unable to get an EGL configuraiton");
        return P_NULL;
    }

    EGLConfig* configurationArray = PNEWARRAY(EGLConfig, eglConfigurationTotalCount);
    PASSERT(configurationArray != P_NULL);
    if (configurationArray == P_NULL)
    {
        PLOG_ERROR("new EGLConfig failed");
        return P_NULL;
    }

    // Find the closest matching configuration.
    pint32 eglConfigurationCount;            
    result = eglChooseConfig(eglDisplay, eglConfigurationAttributeArray, 
            configurationArray, eglConfigurationTotalCount, &eglConfigurationCount);
    if (result != EGL_TRUE || !pEglErrorCheckError())
    {
        PLOG_ERROR("Error in finding the closest matching EGL configuration");
        PDELETEARRAY(configurationArray);
        return P_NULL;
    }

    // If no configs found, check if AA was requested and try again without it. 
    if (eglConfigurationCount == 0)
    {
        PLOG_WARNING("Could not find an EGL configuration with given settings. Trying again without anti-aliasing.");

        for (puint32 i = 0; eglConfigurationAttributeArray[i] != EGL_NONE; i++)
        {
            if (eglConfigurationAttributeArray[i] == EGL_SAMPLE_BUFFERS)
            {
                ++i;
                eglConfigurationAttributeArray[i] = EGL_DONT_CARE;
            }
            if(eglConfigurationAttributeArray[i] == EGL_SAMPLES)
            {
                ++i;
                eglConfigurationAttributeArray[i] = EGL_DONT_CARE;
            }
        }

        // Try again without AA.
        result = eglChooseConfig(eglDisplay, eglConfigurationAttributeArray, 
            configurationArray, eglConfigurationTotalCount, &eglConfigurationCount);
        if (result != EGL_TRUE || !pEglErrorCheckError())
        {
            PLOG_ERROR("Error in finding the closest matching EGL configuration without AA");
            PDELETE(configurationArray);
            return P_NULL;
        }
    }

    // Still none found: just take the ones that are available and see if one
    // with correct color depth can be found 
    if (eglConfigurationCount == 0)
    {
        PLOG_WARNING("Could not find an EGL configuration that matches the requirements. Performance may be reduced.");
        pint32 eglConfigurationTotalCountConfirmed;
        result = eglGetConfigs(eglDisplay, configurationArray,
                eglConfigurationTotalCount,
                &eglConfigurationTotalCountConfirmed);
        if (result != EGL_TRUE || !pEglErrorCheckError())
        {
            PLOG_ERROR("Error in get all EGL configurations");
            PDELETE(configurationArray);
            return P_NULL;
        }
        if (eglConfigurationTotalCountConfirmed != eglConfigurationTotalCount)
        {
            PLOG_ERROR("Wrong EGL total configuration number");
            PDELETE(configurationArray);
            return P_NULL;
        }

        eglConfigurationCount = eglConfigurationTotalCount;
    }

    // Find out which color depths were requested 
    EGLint redAsked = 5;          // bit depths that were requested in eglConfigurationAttributeArray 
    EGLint greenAsked = 6;
    EGLint blueAsked = 5;
    for (pint32 i = 0; eglConfigurationAttributeArray[i] != EGL_NONE; i++)
    {
        switch (eglConfigurationAttributeArray[i]) 
        {
            case EGL_RED_SIZE:
            {
                i++;
                redAsked = eglConfigurationAttributeArray[i];
                break;
            }
            case EGL_GREEN_SIZE:
            {
                i++;
                greenAsked = eglConfigurationAttributeArray[i];
                break;
            }
            case EGL_BLUE_SIZE:
            {
                i++;
                blueAsked = eglConfigurationAttributeArray[i];
                break;
            }
            default:
            {
                break;
            }
        }
    }

    // Search for a configuration that has the correct color format 
    for (pint32 i = 0; i < eglConfigurationCount; i++)
    {
        EGLint redBits = 0;      
        EGLint greenBits = 0;
        EGLint blueBits = 0;            

        result = eglGetConfigAttrib(eglDisplay, configurationArray[i], EGL_RED_SIZE, &redBits);
        result &= eglGetConfigAttrib(eglDisplay, configurationArray[i], EGL_GREEN_SIZE, &greenBits);
        result &= eglGetConfigAttrib(eglDisplay, configurationArray[i], EGL_BLUE_SIZE, &blueBits);

        if (!pEglErrorCheckError())
        {
            PLOG_ERROR("Error in getting EGL config attribute");
            PDELETE(configurationArray);
            return P_NULL;
        }

        if (result == EGL_TRUE && redBits == redAsked && blueBits == blueAsked && greenBits == greenAsked) 
        {
            eglConfig = configurationArray[i];
            break;
        }
    }
    
    // None found, pick the first one 
    if (eglConfig == 0) 
    {
        eglConfig = configurationArray[0];
    }

    PDELETEARRAY(configurationArray);

    return eglConfig;
}

pbool pEglSurfaceInitialize_internal(EGLDisplay eglDisplay, HWND eglWindow, 
        EGLConfig eglConfiguration, EGLSurface& out_eglSurface, EGLContext& out_eglContext)
{
    EGLSurface eglSurface = EGL_NO_SURFACE;
    EGLContext eglContext = EGL_NO_CONTEXT;

    // Create EGL surface with the configuration.
    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfiguration, eglWindow, P_NULL);
    if (!pEglErrorCheckError())
    {
        PLOG_ERROR("Unable to initialize EGL surface");
        return false;
    }

    // Create EGL context.
    EGLint contextAttributes[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    eglContext = eglCreateContext(eglDisplay, eglConfiguration, eglGetCurrentContext(), contextAttributes);
    
    if (!pEglErrorCheckError())
    {
        eglDestroySurface(eglDisplay, eglSurface);
        eglSurface = P_NULL;
        PLOG_ERROR("Unable to initialize EGL context");
        return false;
    }

    // Activate EGL surface. 
    EGLBoolean result = eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    if (result != EGL_TRUE || !pEglErrorCheckError())
    {
        PLOG_ERROR("Unable to activate EGL context");
        
        eglDestroySurface(eglDisplay, eglSurface);
        eglSurface = P_NULL;
        eglDestroyContext(eglDisplay, eglContext);
        eglContext = P_NULL;

        return false;
    }

    out_eglSurface = eglSurface;
    out_eglContext = eglContext;
    
    return true;
}

