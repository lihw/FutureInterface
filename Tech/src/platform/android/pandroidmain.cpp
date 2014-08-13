// pandroidmain.cpp
// The JNI interface of FI Android application
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#include "pandroidmain.h"

#include <PFoundation/pruntime_module.h>
#include <PFoundation/penvironment.h>
#include <PFoundation/ptime.h>
#include <PFoundation/pjnihelper.h>

#include <android/log.h>
#include <jni.h>

P_EXTERN void pMain(int argc, char* argv[]);

P_EXTERN PInputKey P_APIENTRY pInputNativeGetTranslatedKey(puint32 keyCode);

void pAndroidDoNothing()
{
    // calling this function is needed so the pandroidmain.cpp is included in the final .so as well.. 
}

static PContext* pGetContext(pint32 contextId)
{
    PContext* context = P_NULL;
    if (pGetActivity() != P_NULL)
    {
        context = pGetActivity()->findContext(contextId);
    }

    return context;
}

static void pExceptionThrowMessage_internal(JNIEnv* env, const pchar* message)
{
    jclass exceptionClass = env->FindClass("java/lang/Exception");
    if (exceptionClass != P_NULL)
    {
        env->ThrowNew(exceptionClass, message);
    }
}

JNIEXPORT jboolean JNICALL Java_com_fi_tech_PNativeLibrary_initializeLibraryNative(JNIEnv* env, jobject obj, jobject context)
{
    pEnvironmentInitialize(context);

    if (!pGetActivity()->initialize())
    {
        return JNI_FALSE;
    }

    PLOG_INFO("initializeLibraryNative() done");

    return JNI_TRUE;
}

JNIEXPORT void JNICALL Java_com_fi_tech_PContext_createContextNative(JNIEnv* env, jobject obj, jstring name)
{
    // JNI_OnLoad() will only be called when it is not in the memory. Even the Android's activity quits, its memory 
    // footprint still resides in the system memory until memory gabarge collection. Thus when application starts
    // next time, it doesn't need to load the .so again, in which case, pMain() will not be provoked. That's why
    // we put pMain here to be explicitly called by JNI interface.
    //
    // TODO: we need to explicitly control the loading and unloading mechnism of .so. 
    PString contextName = PJniHelper::jstring2string(name);
    char *argv[1];
    argv[0] = contextName.c_str();
    pMain(1, argv);
}

JNIEXPORT jint JNICALL Java_com_fi_tech_PContext_getContextIdByNameNative(JNIEnv* env, jobject obj, jstring contextName)
{
    const char* contextNameStr = env->GetStringUTFChars(contextName, NULL); 
    __android_log_print(ANDROID_LOG_INFO, "Java_com_fi_tech_PContext_getContextIdByNameNative", "query context id using name %s", contextNameStr);

    PASSERT(pGetActivity() != P_NULL);
    if (pGetActivity() == P_NULL)
    {
        pExceptionThrowMessage_internal(env, "No active activity exists");
        env->ReleaseStringUTFChars(contextName, contextNameStr);
        return -1;
    }

    puint32 contextId = pGetActivity()->findContextId(contextNameStr);
    if (contextId == P_CONTEXT_INVALID_ID)
    {
        pExceptionThrowMessage_internal(env, "Failed to find the context by given name");
        env->ReleaseStringUTFChars(contextName, contextNameStr);
        return -1;
    }

    env->ReleaseStringUTFChars(contextName, contextNameStr);
    return (jint)(contextId);
}

JNIEXPORT void JNICALL Java_com_fi_tech_PContext_initializeContextNative(JNIEnv* env, jobject obj, jint contextId , jint width, jint height)
{
    __android_log_print(ANDROID_LOG_INFO, "Java_com_fi_tech_PContext_initializeContextNative", "context init called for %d width=%d, height=%d", contextId, width, height);
    
    PContext* context = pGetContext(contextId);
    if (context == P_NULL )
    {
        pExceptionThrowMessage_internal(env, "Failed to initialize a Tech context because context is NULL");
    }

    if (!context->initialize(width, height))
    {
        pExceptionThrowMessage_internal(env, "Failed to initialize a Tech context");
    }
    if (!context->onInitialized())
    {
        context->setState(P_CONTEXT_STATE_ERROR);
        pExceptionThrowMessage_internal(env, "Something wrong at onInitialize() of context");
    }

    context->setState(P_CONTEXT_STATE_RUNNING);
}

JNIEXPORT void JNICALL Java_com_fi_tech_PContext_resizeNative(JNIEnv* env, jobject obj, jint contextId, jint width, jint height)
{
    __android_log_print(ANDROID_LOG_INFO, "Java_com_fi_tech_PContext_resizeNative", "resize called for context %d", contextId);

    PContext* context = pGetContext(contextId);
    if (context != P_NULL)
    {
        PDevice* device = context->device();
        if (device != P_NULL && 
                (context->state() == P_CONTEXT_STATE_RUNNING ||
                 context->state() == P_CONTEXT_STATE_PAUSED))
        {
           device->onResized(width, height);
           return ;
        }
    
        if (device == P_NULL)
        {
            pExceptionThrowMessage_internal(env, "Failed to resize window due to invalid device");
        }
    }

    pExceptionThrowMessage_internal(env, "Failed to resize window due to invalid context");
}

JNIEXPORT void JNICALL Java_com_fi_tech_PContext_orientationChangeNative(JNIEnv* env, jobject obj, jint contextId, jint orientation)
{
    __android_log_print(ANDROID_LOG_INFO, "Java_com_fi_tech_PContext_orientationChangeNative", "orientation change called for context %d", contextId);

    PContext* context = pGetContext(contextId);
    if (context != P_NULL)
    {
        PDevice* device = context->device();
        if (device != P_NULL && 
                (context->state() == P_CONTEXT_STATE_RUNNING ||
                 context->state() == P_CONTEXT_STATE_PAUSED))
        {
            PDeviceOrientationEnum orient = (PDeviceOrientationEnum)(orientation);
            device->onOrientationChanged(orient);
            return ;
        }
    }

    pExceptionThrowMessage_internal(env, "Failed to initialize context");
}

JNIEXPORT void JNICALL Java_com_fi_tech_PContext_uninitializeContextNative(JNIEnv* env, jobject obj, jint contextId)
{
    __android_log_print(ANDROID_LOG_INFO, "Java_com_fi_tech_PContext_unitializeContextNative", "uninit called for context %d", contextId);
    
    PContext* context = pGetContext(contextId);
    if (context != P_NULL)
    {
        context->onDestroy();
        context->destroy();
        pGetActivity()->removeContext(context);
        PDELETE(context);
        // Shut down engine if there is no context.
        if (pGetActivity()->getNumberOfContexts() == 0)
        {
            pEnvironmentUninitialize();
            pGetActivity()->uninitialize();
        }
    }
    else
    {
        pExceptionThrowMessage_internal(env, "Context uninitialization failed");
    }
}

JNIEXPORT void JNICALL Java_com_fi_tech_PContext_pauseNative(JNIEnv* env, jobject obj, jint contextId) 
{
    __android_log_print(ANDROID_LOG_INFO, "Java_com_fi_tech_PContext_pauseNative", "pause called for context %d", contextId);

    PContext* context = pGetContext(contextId);
    if (context != P_NULL && context->state() == P_CONTEXT_STATE_RUNNING)
    {
        context->pause();
        return ;
    }

    if (context->state() != P_CONTEXT_STATE_QUITING)
    {
        pExceptionThrowMessage_internal(env, "Failed to pause context");
    }
}

JNIEXPORT void JNICALL Java_com_fi_tech_PContext_resumeNative(JNIEnv* env, jobject obj, jint contextId) 
{
    __android_log_print(ANDROID_LOG_INFO, "Java_com_fi_tech_PContext_resumeNative", "resume called for context %d", contextId);
    
    PContext* context = pGetContext(contextId);
    if (context != P_NULL && context->state() == P_CONTEXT_STATE_PAUSED)
    {
        PLOG_INFO("Resume start");        
        bool ret = context->resume();
        PLOG_INFO("Resume end");
        if (ret)
        {
            return ;
        }
    }

    pExceptionThrowMessage_internal(env, "Failed to resume context");
}

// commented-out code: update PView reference on every update 
JNIEXPORT jboolean JNICALL Java_com_fi_tech_PContext_updateNative(JNIEnv* env, jobject obj, jint contextId)
{
    //__android_log_print(ANDROID_LOG_INFO, "Java_com_fi_tech_PContext_updateNative", "update called for context %d", contextId);

    jboolean returnValue = JNI_TRUE;
    PContext* context = pGetContext(contextId);
    if (context != P_NULL &&
        (context->state() == P_CONTEXT_STATE_RUNNING ||
         context->state() == P_CONTEXT_STATE_PAUSED))
    {
        if (!context->update())
        {
            context->setState(P_CONTEXT_STATE_ERROR);

            __android_log_print(ANDROID_LOG_ERROR, "Java_com_fi_tech_PContext_updateNative", "update error in context %d", contextId);

            returnValue = JNI_FALSE;
        }
    }
    else
    {
        __android_log_print(ANDROID_LOG_ERROR, "Java_com_fi_tech_PContext_updateNative", "context %d is invalid or its state is wrong", contextId);
        returnValue = JNI_FALSE;
    }

    return returnValue;
}

JNIEXPORT void JNICALL Java_com_fi_tech_PContext_keyEventNative(JNIEnv* env, jobject obj, jint contextId, jint keyCode, jint state,jlong timeStamp)
{
    __android_log_print(ANDROID_LOG_INFO, "Java_com_fi_tech_PContext_keyEventNative", "key called for context %d", contextId);
    
    PInputKeyDeviceStateEnum keyState;
    if (state == 0) keyState = P_KEY_DEVICE_STATE_DOWN;
    if (state == 1) keyState = P_KEY_DEVICE_STATE_UP;
    if (state == 2) keyState = P_KEY_DEVICE_STATE_REPEAT;

    __android_log_print(ANDROID_LOG_INFO, "Java_com_fi_tech_PContext_keyEventNative", "key entered %d", keyCode);

    PContext* context = pGetContext(contextId);
    if (context == P_NULL)
    pExceptionThrowMessage_internal(env, "Failed to add key event");
    if (context != P_NULL && 
            (context->state() == P_CONTEXT_STATE_RUNNING ||
             context->state() == P_CONTEXT_STATE_PAUSED))
    {
        // TODO: Skip case 2 as it's not needed for now      
        // TODO: Check if button code needs to be translated to Tech from Android. 
        PInputEventQueue* inputQueue = context->device()->getInputEventQueue();
        PInputKey key = pInputNativeGetTranslatedKey(keyCode);
        inputQueue->addKeyEvent(keyCode, key, keyState, timeStamp);

        return ;
    }
        
    pExceptionThrowMessage_internal(env, "Failed to add key event");
}

JNIEXPORT void JNICALL Java_com_fi_tech_PContext_focusEventNative(JNIEnv* env, jobject obj, jint contextId, jboolean focusState)
{
    __android_log_print(ANDROID_LOG_INFO, "Java_com_fi_tech_PContext_focusEventNative", "focus called for context %d", contextId);

    PContext* context = pGetContext(contextId);
    if (context != P_NULL &&
        (context->state() == P_CONTEXT_STATE_RUNNING ||
         context->state() == P_CONTEXT_STATE_PAUSED))
    {
        PDevice* device = context->device();
        if (device != P_NULL)
        {
            if (focusState)
            {
                device->setState(P_DEVICE_STATE_ACTIVE);
                device->onFocusGained();
            }
            else
            {
                // Release all held keys.
                // For the integer 225, please see P_INPUT_NATIVE_KEY_TRANSLATION_TABLE_SIZE
                // in p_input_win32.cpp
                for (puint32 i = 0; i < 225; ++i)
                {
                    PInputKey key = pInputNativeGetTranslatedKey(i);
                    if (key != P_KEY_UNKNOWN && device->getKeyState(key) == P_KEY_DEVICE_STATE_DOWN)
                    {
                        device->setKeyState((PInputKey)i, P_KEY_DEVICE_STATE_UP);
                        device->getInputEventQueue()->addKeyEvent(i, key, P_KEY_DEVICE_STATE_UP, pTimeGetCurrentTimestamp());
                    }
                }

                device->setState(P_DEVICE_STATE_INACTIVE);
                device->onFocusLost();
            }

            return ;
        }
    }
    
    pExceptionThrowMessage_internal(env, "Failed to focus");
}

JNIEXPORT jlong JNICALL Java_com_fi_tech_PContext_touchEventNative(JNIEnv* env, jobject obj, jint contextId, jint cursorCount, jint state)
{
    __android_log_print(ANDROID_LOG_INFO, "Java_com_fi_tech_PContext_touchEventNative", "touch event called for context %d", contextId);

    // The states for cursors and touch events are already set at the Java code, can be passed as-is now. 
    PContext* context = pGetContext(contextId);
    if (context != P_NULL &&
        (context->state() == P_CONTEXT_STATE_RUNNING ||
         context->state() == P_CONTEXT_STATE_PAUSED))
    {
        PDevice* device = context->device();
        if (device != P_NULL)
        {
            PInputTouchStateEnum touchState;
            switch (state)
            {
                case 0: touchState = P_TOUCH_STATE_BEGIN; break;
                case 1: touchState = P_TOUCH_STATE_CONTINUE; break;
                case 2: touchState = P_TOUCH_STATE_END; break;
                default: 
                    pExceptionThrowMessage_internal(env, "Unrecognized touch state");
                    break;
            }
            PInputEventTouch* event = 
                device->getInputEventQueue()->addTouchEvent(cursorCount, touchState);
            return (jlong)(event);
        }
    }
    
    pExceptionThrowMessage_internal(env, "Failed to add touch event");

    return 0;
}

JNIEXPORT void JNICALL Java_com_fi_tech_PContext_touchCursorNative(JNIEnv* env, jobject obj, jint contextId, jlong touchEvent, 
                        jint index, jint id, jfloat x, jfloat y, jfloat pressure, jfloat area, int state, jlong timeStamp)
{
    __android_log_print(ANDROID_LOG_INFO, "Java_com_fi_tech_PContext_touchCursorNative", "touch cursor called for context %d", contextId);
    PInputEventTouch* event = (PInputEventTouch*)(touchEvent);;

    PContext* context = pGetContext(contextId);
    if (context != P_NULL)
    {
        PDevice* device = context->device();
        if (device != P_NULL)
        {
            device->getInputEventQueue()->setTouchCursor(
                    event,
                    index,
                    id,
                    x,
                    y,
                    pressure,
                    area,
                    state,
                    timeStamp);
            return ;
        }
    }
    
    pExceptionThrowMessage_internal(env, "Failed to set cursor");
}

JNIEXPORT void JNICALL Java_com_fi_tech_PContext_setHUDNative(JNIEnv* env, jobject obj, jint contextId, jboolean onoff) 
{
    // TODO:
    PASSERT_NOTIMPLEMENTED();
}

JNIEXPORT void JNICALL Java_com_fi_tech_PContext_setFreeCameraNative(JNIEnv* env, jobject obj, jint contextId, jboolean onoff)
{
    // TODO: 
    PASSERT_NOTIMPLEMENTED();
}


